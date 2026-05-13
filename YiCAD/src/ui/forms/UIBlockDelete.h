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

/// @file UIBlockDelete.h
/// @brief 块删除对话框

#ifndef UIBLOCKDELETE_H
#define UIBLOCKDELETE_H

#include "ui_UIBlockDelete.h"

class DmBlockTable;
class DmDocument;
class DmEntityContainer;

class UIBlockDelete : public QDialog, public Ui::UIBlockDelete
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] fl 窗口标志
    UIBlockDelete(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UIBlockDelete();

    /// @brief 设置块表
    /// @param [in] blockTable 块表指针
    void setBlockTable(DmBlockTable* blockTable);

    /// @brief 设置文档
    /// @param [in] doc 文档指针
    void setDocument(DmDocument* doc);

private slots:
    /// @brief 块选择变化槽
    /// @param [in] curBlock 当前选中的块名
    void slotBlockSelectionChanged(const QString& curBlock);

    /// @brief 删除按钮点击槽
    void slotDeleteClicked();

private:
    /// @brief 刷新块列表
    void updateBlockList();

    /// @brief 检查块是否有引用
    /// @param [in] blockName 块名
    /// @return 存在引用返回 true，否则返回 false
    bool hasReferences(const QString& blockName) const;

private:
    DmBlockTable*       m_pBlockTable;          ///< 块表指针
    DmDocument*         m_pDocument;            ///< 文档指针
    DmEntityContainer*  m_pPreviewContainer;    ///< 预览用空容器，由本类负责释放
};

#endif // UIBLOCKDELETE_H
