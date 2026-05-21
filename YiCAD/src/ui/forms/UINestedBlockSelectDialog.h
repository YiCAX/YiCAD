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

/// @file UINestedBlockSelectDialog.h
/// @brief 嵌套块选择对话框，用于选择要编辑的嵌套块层级

#ifndef UINESTEDBLOCKSELECT_DIALOG_H
#define UINESTEDBLOCKSELECT_DIALOG_H

#include <QDialog>
#include <QStringList>

class QListWidget;
class QDialogButtonBox;
class DmDocument;
class DmEntityContainer;
class GuiPreviewWidget;

/// @brief 嵌套块选择对话框
/// 左侧显示块层级列表，右侧显示选中块的预览
class UINestedBlockSelectDialog : public QDialog
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param blockNames 嵌套块名称列表（包含顶层块自身）
    /// @param parent 父窗口
    UINestedBlockSelectDialog(DmDocument* doc, const QStringList& blockNames,
                              QWidget* parent = nullptr);
    ~UINestedBlockSelectDialog() override;

    /// @brief 获取用户选择的块名称
    QString selectedBlockName() const;

private slots:
    void onSelectionChanged(int row);

private:
    DmDocument* m_document;
    QListWidget* m_listWidget;
    GuiPreviewWidget* m_previewWidget;
    QDialogButtonBox* m_buttonBox;
    DmEntityContainer* m_previewContainer;
    QString m_selectedName;
    QStringList m_blockNames;
};

#endif // UINESTEDBLOCKSELECT_DIALOG_H
