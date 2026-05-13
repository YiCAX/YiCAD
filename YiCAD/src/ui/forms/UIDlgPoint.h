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

/// @file UIDlgPoint.h
/// @brief 点实体属性编辑对话框

#ifndef UIDLGPOINT_H
#define UIDLGPOINT_H

class DmPoint;

#include "ui_UIDlgPoint.h"

class UIDlgPoint : public QDialog, public Ui::UIDlgPoint
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否为模态对话框
    /// @param [in] fl 窗口标志
    UIDlgPoint(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIDlgPoint();

public slots:
    /// @brief 设置要编辑的点实体
    /// @param [in] p 点实体引用
    virtual void setPoint(DmPoint& p);

    /// @brief 更新点实体以匹配用户修改
    virtual void updatePoint();

protected slots:
    virtual void languageChange();

private:
    DmPen pen;                   ///< 画笔对象
    DmPoint* m_pPoint = nullptr; ///< 待编辑的点实体指针
};

#endif // UIDLGPOINT_H
