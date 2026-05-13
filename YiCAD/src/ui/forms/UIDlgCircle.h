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

/// @file UIDlgCircle.h
/// @brief 圆属性对话框

#ifndef UIDLGCIRCLE_H
#define UIDLGCIRCLE_H

class DmCircle;

#include "ui_UIDlgCircle.h"

class UIDlgCircle : public QDialog, public Ui::UIDlgCircle
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否模态
    /// @param [in] fl 窗口标志
    UIDlgCircle(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UIDlgCircle();

public slots:
    /// @brief 设置圆对象
    /// @param [in,out] c 圆对象引用
    virtual void setCircle(DmCircle& c);

    /// @brief 更新圆属性
    virtual void updateCircle();

protected slots:
    /// @brief 语言切换槽
    virtual void languageChange();

private:
    DmCircle* m_pCircle;   ///< 圆对象指针
};

#endif // UIDLGCIRCLE_H
