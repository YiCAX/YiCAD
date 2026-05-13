/*
 * Copyright (C) 2026 YiCAD Contributors
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

/// @file ActionDrawRay.cpp
/// @brief 射线绘制动作类实现

#include "ActionDrawRay.h"

#include <QMouseEvent>

#include "Debug.h"
#include "DmRay.h"
#include "GuiCommandEvent.h"
#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Preview.h"
#include "Snapper.h"
#include "Transaction.h"

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionDrawRay::ActionDrawRay(DmDocument* doc, GuiDocumentView* docView)
    : PreviewActionInterface("Draw ray", doc, docView)
    , m_data(new RayData{})
{
    actionType = DM::ActionDrawRay;
}

/// @brief 析构函数
ActionDrawRay::~ActionDrawRay()
{
}

/// @brief 重置射线数据
void ActionDrawRay::reset()
{
    m_data.reset(new RayData{});
}

/// @brief 初始化动作，重置数据并调用基类初始化
/// @param status 初始状态
void ActionDrawRay::init(int status)
{
    PreviewActionInterface::init(status);

    reset();
}

/// @brief 触发射线创建，将射线实体添加到文档中
void ActionDrawRay::trigger()
{
    PreviewActionInterface::trigger();

    Transaction t(tr("Draw Ray").toStdString(), pDocument);
    t.start();
    DmRay* ray = new DmRay(nullptr, *m_data);
    ray->setDocument(pDocument);
    pDocument->getEntityTable()->add(ray);
    t.commit();

    docView->moveRelativeZero(m_data->getBasePoint());
}

/// @brief 鼠标移动事件处理，实时预览射线方向
/// @param e 鼠标事件
void ActionDrawRay::mouseMoveEvent(QMouseEvent* e)
{
    DmVector mouse = snapPoint(e);

    if ((getStatus() == SetDir) && (m_data->getBasePoint().valid))
    {
        deletePreview();
        m_data->setDirection(mouse - m_data->getBasePoint());

        DmRay* ray = new DmRay(preview->getEntityContainer(), *m_data);
        ray->setDocument(pDocument);
        preview->addEntity(ray);
        drawPreview();
    }
}

/// @brief 鼠标释放事件处理，确认基点或回退状态
/// @param e 鼠标事件
void ActionDrawRay::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        GuiCoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    }
    else if (e->button() == Qt::RightButton)
    {
        deletePreview();
        switch (getStatus())
        {
        default:
        case SetBasePoint:
            init(getStatus() - 1);
            break;

        case SetDir:
            setStatus(SetBasePoint);
            break;
        }
    }
    else
    {
        // TODO: 处理其他鼠标按钮事件（如中键）
    }
}

/// @brief 坐标输入事件处理，根据当前状态设置基点或方向
/// @param e 坐标事件
void ActionDrawRay::coordinateEvent(GuiCoordinateEvent* e)
{
    if (e == nullptr)
    {
        return;
    }

    DmVector mousePos = e->getCoordinate();

    switch (getStatus())
    {
    case SetBasePoint:
        m_data->setBasePoint(mousePos);
        setStatus(SetDir);
        docView->moveRelativeZero(mousePos);
        break;
    case SetDir:
        m_data->setDirection(mousePos - m_data->getBasePoint());
        trigger();
        break;
    default:
        break;
    }
}

/// @brief 命令行事件处理，支持 help 等命令
/// @param e 命令事件
void ActionDrawRay::commandEvent(GuiCommandEvent* e)
{
    // TODO: e 可能为 nullptr，需要添加空指针检查
    QString str = e->getCommand().toLower();

    switch (getStatus())
    {
    case SetBasePoint:
        if (checkCommand("help", str))
        {
            QString cmds = msgAvailableCommands()
                           + getAvailableCommands().join(", ");
            GUIDIALOGFACTORY->commandMessage(cmds);
            return;
        }
        break;
    case SetDir:
        break;
    default:
        break;
    }
}

/// @brief 获取可用命令列表
/// @return 可用命令字符串列表
QStringList ActionDrawRay::getAvailableCommands()
{
    return {};
}

/// @brief 显示选项面板
void ActionDrawRay::showOptions()
{
    ActionInterface::showOptions();
}

/// @brief 隐藏选项面板
void ActionDrawRay::hideOptions()
{
    ActionInterface::hideOptions();
}

/// @brief 更新鼠标按钮提示文本
void ActionDrawRay::updateMouseButtonHints()
{
    switch (getStatus())
    {
    case SetBasePoint:
        GUIDIALOGFACTORY->updateMouseWidget(
            tr("Specify first point"), tr("Cancel"));
        break;
    case SetDir:
        GUIDIALOGFACTORY->updateMouseWidget(
            tr("Specify direction"), tr("Back"));
        break;
    default:
        break;
    }
}

/// @brief 更新鼠标光标样式
void ActionDrawRay::updateMouseCursor()
{
    docView->setMouseCursor(DM::CadCursor);
}
