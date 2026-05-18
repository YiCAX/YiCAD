/*
 * Copyright (C) 2026 YiCAD Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// @file ActionBlocksEdit.cpp
/// @brief 编辑块定义的动作类实现文件

#include "ActionBlocksEdit.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>

#include "DmBlock.h"
#include "DmDocument.h"
#include "DmBlockReference.h"
#include "DmBlockTable.h"
#include "DmEntityContainer.h"
#include "EntityTable.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "GuiEventHandler.h"
#include "BlockEditCmd.h"
#include "CmdManager.h"
#include "MacroCmd.h"
#include "Transaction.h"
#include "UIDialogFactory.h"


/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
/// @param selectedRef 选中的块参照，默认为nullptr
ActionBlocksEdit::ActionBlocksEdit(DmDocument* doc,
    GuiDocumentView* docView, DmBlockReference* selectedRef)
    : ActionInterface("Edit Block", doc, docView)
    , m_blockRefBeingEdited(selectedRef)
{
    actionType = DM::ActionBlocksEdit;
}

/// @brief 析构函数
ActionBlocksEdit::~ActionBlocksEdit() = default;

/// @brief 初始化动作
/// @param status 状态参数，默认为0
void ActionBlocksEdit::init(int status)
{
    ActionInterface::init(status);

    // Check for re-entry via undo/redo
    DmBlock* editingBlock = pDocument->getEditingBlock();
    if (editingBlock)
    {
        // Re-entry via undo: block is already set, just restore UI
        reenterEditing(editingBlock);
        return;
    }

    // Normal entry: need a selected block reference
    if (!m_blockRefBeingEdited)
    {
        GUIDIALOGFACTORY->commandMessage(
            tr("No block reference selected. Command cancelled."));
        finish();
        return;
    }

    enterEditing(m_blockRefBeingEdited);
    setStatus(eEditing);
}

/// @brief 触发动作执行
void ActionBlocksEdit::trigger()
{
    // Not used in the new design; init() handles everything
}

/// @brief 重新进入编辑模式（通过Undo/Redo）
/// @param block 块定义指针
void ActionBlocksEdit::reenterEditing(DmBlock* block)
{
    m_isReentry = true;
    m_editingBlock = block;
    m_blockName = block->getName();
    setStatus(eEditing);
    showOptions();
    docView->zoomAuto();
    GUIDIALOGFACTORY->commandMessage(tr("Editing block: %1").arg(m_blockName));
}

/// @brief 正常进入编辑模式
/// @param blockRef 块参照指针
void ActionBlocksEdit::enterEditing(DmBlockReference* blockRef)
{
    if (!blockRef || !pDocument)
    {
        return;
    }

    m_blockRefBeingEdited = blockRef;
    m_blockName = blockRef->getName();

    DmBlockTable* blockTable = pDocument->getBlockTable();
    if (!blockTable)
    {
        return;
    }

    DmBlock* block = blockTable->find(m_blockName);
    if (!block)
    {
        GUIDIALOGFACTORY->commandMessage(
            tr("Block definition not found: %1").arg(m_blockName));
        return;
    }

    m_editingBlock = block;

    // Create a transaction with BlockEditEnterCmd
    // Use addToCurrentCmd() NOT addAndExecuteCmd() to avoid double execute
    Transaction t("Block Edit Begin", pDocument);
    t.start();
    auto* enterCmd = new BlockEditEnterCmd(pDocument, m_blockName, docView);
    pDocument->getCmdManager()->addToCurrentCmd(enterCmd);
    t.commit();

    // Save the macro command for cancel support
    CmdManager* cmdMgr = pDocument->getCmdManager();
    m_enterMacroCmd = cmdMgr->getLastUndoCmd();
    m_undoCountAtEnter = cmdMgr->getUndoCount();

    setStatus(eEditing);
    showOptions();

    docView->zoomAuto();

    GUIDIALOGFACTORY->commandMessage(tr("Editing block: %1").arg(m_blockName));
}

/// @brief 完成编辑
/// @param save 是否保存修改（更新块参照）
void ActionBlocksEdit::completeEditing(bool save)
{
    if (!pDocument)
    {
        finish();
        return;
    }

    DmBlock* editingBlock = pDocument->getEditingBlock();
    if (editingBlock)
    {
        // Create a transaction with BlockEditExitCmd
        Transaction t("Block Edit End", pDocument);
        t.start();
        auto* exitCmd = new BlockEditExitCmd(pDocument, m_blockName, docView, save);
        pDocument->getCmdManager()->addToCurrentCmd(exitCmd);
        t.commit();
    }

    m_enterMacroCmd = nullptr;
    m_editingBlock = nullptr;

    hideOptions();
    docView->setMouseCursor(DM::CadCursor);
    docView->redraw();
    finish();
}

/// @brief 取消编辑，丢弃所有修改
void ActionBlocksEdit::cancelEditing()
{
    if (!pDocument)
    {
        finish();
        return;
    }

    CmdManager* cmdMgr = pDocument->getCmdManager();

    if (m_enterMacroCmd)
    {
        // Find the enter command and roll back everything after it
        int index = cmdMgr->indexOfCmd(m_enterMacroCmd);
        if (index != -1)
        {
            // This undoes and deletes all commands from the enter to current,
            // including the enter command itself (which calls setEditBlock(nullptr))
            cmdMgr->rollbackAndRemoveAfter(index);
        }
        m_enterMacroCmd = nullptr;
    }
    else
    {
        // Re-entry case or no enter command: just exit edit mode
        DmBlock* editingBlock = pDocument->getEditingBlock();
        if (editingBlock)
        {
            pDocument->setEditBlock(nullptr);
            pDocument->regenerate();
        }
    }

    m_editingBlock = nullptr;

    hideOptions();
    docView->setMouseCursor(DM::CadCursor);
    docView->redraw();
    finish();
}

/// @brief 检测自进入编辑以来是否有修改
/// @return true表示有修改
bool ActionBlocksEdit::hasModifications() const
{
    if (!pDocument)
    {
        return false;
    }
    CmdManager* cmdMgr = pDocument->getCmdManager();
    return cmdMgr->getUndoCount() > m_undoCountAtEnter;
}

/// @brief 鼠标移动事件处理
/// @param e 鼠标事件指针
void ActionBlocksEdit::mouseMoveEvent(QMouseEvent* e)
{
    if (getStatus() == eEditing)
    {
        auto* handler = docView->getEventHandler();
        if (handler && handler->getDefaultAction())
        {
            handler->getDefaultAction()->mouseMoveEvent(e);
        }
    }
}

/// @brief 鼠标按下事件处理
/// @param e 鼠标事件指针
void ActionBlocksEdit::mousePressEvent(QMouseEvent* e)
{
    if (getStatus() == eEditing)
    {
        auto* handler = docView->getEventHandler();
        if (handler && handler->getDefaultAction())
        {
            handler->getDefaultAction()->mousePressEvent(e);
        }
    }
}

/// @brief 鼠标释放事件处理
/// @param e 鼠标事件指针
void ActionBlocksEdit::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (getStatus() == eEditing)
        {
            auto* handler = docView->getEventHandler();
            if (handler && handler->getDefaultAction())
            {
                handler->getDefaultAction()->mouseReleaseEvent(e);
            }
        }
    }
    else if (e->button() == Qt::RightButton)
    {
        if (getStatus() == eEditing)
        {
            int ret = QMessageBox::question(nullptr,
                tr("Block Edit"),
                tr("Finish editing and save changes?"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

            if (ret == QMessageBox::Yes)
            {
                completeEditing(true);
            }
            else if (ret == QMessageBox::No)
            {
                completeEditing(false);
            }
            // Cancel: continue editing
        }
        else
        {
            finish();
        }
    }
}

/// @brief 键盘按下事件处理
/// @param e 键盘事件指针
void ActionBlocksEdit::keyPressEvent(QKeyEvent* e)
{
    if (getStatus() == eEditing)
    {
        auto* handler = docView->getEventHandler();
        if (handler && handler->getDefaultAction())
        {
            handler->getDefaultAction()->keyPressEvent(e);
        }
    }
    else
    {
        ActionInterface::keyPressEvent(e);
    }
}

/// @brief 检查是否可以中断
/// @return true表示可以中断
bool ActionBlocksEdit::canBeInterrupt()
{
    return true;
}

/// @brief 检查是否为独占动作
/// @return false表示非独占
bool ActionBlocksEdit::isExclusive()
{
    return false;
}

/// @brief 检查是否为子动作
/// @return false表示不是子动作
bool ActionBlocksEdit::isSubAction()
{
    return false;
}

/// @brief 显示选项栏
void ActionBlocksEdit::showOptions()
{
    ActionInterface::showOptions();
    GUIDIALOGFACTORY->requestOptions(this, true);
}

/// @brief 隐藏选项栏
void ActionBlocksEdit::hideOptions()
{
    ActionInterface::hideOptions();
    GUIDIALOGFACTORY->requestOptions(this, false);
}

/// @brief 更新鼠标按钮提示
void ActionBlocksEdit::updateMouseButtonHints()
{
    switch (getStatus())
    {
    case eEditing:
        GUIDIALOGFACTORY->updateMouseWidget(
            tr("Edit block entities"), tr("Finish / Cancel"));
        break;
    default:
        GUIDIALOGFACTORY->updateMouseWidget();
        break;
    }
}

/// @brief 更新鼠标光标
void ActionBlocksEdit::updateMouseCursor()
{
    if (getStatus() == eEditing)
    {
        docView->setMouseCursor(DM::ArrowCursor);
    }
    else
    {
        docView->setMouseCursor(DM::CadCursor);
    }
}

// EOF
