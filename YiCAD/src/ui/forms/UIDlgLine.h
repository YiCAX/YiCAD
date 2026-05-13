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

/// @file UIDlgLine.h
/// @brief 直线属性编辑对话框

#ifndef UIDLGLINE_H
#define UIDLGLINE_H

class DmLine;

#include "ui_UIDlgLine.h"

class UIDlgLine : public QDialog, public Ui::UIDlgLine
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否为模态对话框
    /// @param [in] fl 窗口标志
    UIDlgLine(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIDlgLine();

public slots:
    /// @brief 设置要编辑的直线实体
    /// @param [in] l 直线实体引用
    virtual void setLine(DmLine& l);

    /// @brief 更新直线实体以匹配用户修改
    virtual void updateLine();

protected slots:
    virtual void languageChange();

private:
    DmLine* m_pLine = nullptr; ///< 待编辑的直线实体指针
};

#endif // UIDLGLINE_H
