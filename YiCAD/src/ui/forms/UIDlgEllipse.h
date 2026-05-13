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

/// @file UIDlgEllipse.h
/// @brief 椭圆/椭圆弧属性编辑对话框

#ifndef UIDLGELLIPSE_H
#define UIDLGELLIPSE_H

class DmEllipse;

#include "ui_UIDlgEllipse.h"

class UIDlgEllipse : public QDialog, public Ui::UIDlgEllipse
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否为模态对话框
    /// @param [in] fl 窗口标志
    UIDlgEllipse(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIDlgEllipse();

public slots:
    /// @brief 设置要编辑的椭圆实体
    /// @param [in] e 椭圆引用
    virtual void setEllipse(DmEllipse& e);

    /// @brief 更新椭圆实体以匹配用户修改
    virtual void updateEllipse();

protected slots:
    virtual void languageChange();

private:
    DmEllipse* m_pEllipse = nullptr; ///< 待编辑的椭圆实体指针
};

#endif // UIDLGELLIPSE_H
