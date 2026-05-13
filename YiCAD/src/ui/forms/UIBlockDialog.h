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

/// @file UIBlockDialog.h
/// @brief 块对话框

#ifndef UIBLOCKDIALOG_H
#define UIBLOCKDIALOG_H

#include "ui_UIBlockDialog.h"

class DmBlockTable;
struct DmBlockData;

class UIBlockDialog : public QDialog, public Ui::UIBlockDialog
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否模态
    /// @param [in] fl 窗口标志
    UIBlockDialog(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UIBlockDialog() = default;

    /// @brief 获取块数据
    /// @return 块数据
    virtual DmBlockData getBlockData();

public slots:
    /// @brief 设置块表
    /// @param [in] l 块表指针
    virtual void setBlockList(DmBlockTable* l);

    /// @brief 验证并接受
    virtual void validate();

    /// @brief 取消操作
    virtual void cancel();

protected:
    DmBlockTable* blockTable;    ///< 块表指针

protected slots:
    /// @brief 语言切换槽
    virtual void languageChange();
};

#endif // UIBLOCKDIALOG_H
