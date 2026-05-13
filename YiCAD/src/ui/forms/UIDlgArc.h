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

/// @file UIDlgArc.h
/// @brief 圆弧属性对话框

#ifndef UIDLGARC_H
#define UIDLGARC_H

class DmArc;

#include "ui_UIDlgArc.h"

class UIDlgArc : public QDialog, public Ui::UIDlgArc
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否模态
    /// @param [in] fl 窗口标志
    UIDlgArc(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UIDlgArc();

public slots:
    /// @brief 设置圆弧对象
    /// @param [in,out] a 圆弧对象引用
    virtual void setArc(DmArc& a);

    /// @brief 更新圆弧属性
    virtual void updateArc();

protected slots:
    /// @brief 语言切换槽
    virtual void languageChange();

private:
    DmArc* arc;   ///< 圆弧对象指针
};

#endif // UIDLGARC_H
