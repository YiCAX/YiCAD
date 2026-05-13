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

/// @file UIDlgInsert.h
/// @brief 块参照插入/编辑对话框

#ifndef UIDLGINSERT_H
#define UIDLGINSERT_H

class DmBlockReference;

#include "ui_UIDlgInsert.h"

class UIDlgInsert : public QDialog, public Ui::UIDlgInsert
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否为模态对话框
    /// @param [in] fl 窗口标志
    UIDlgInsert(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIDlgInsert();

public slots:
    /// @brief 设置要编辑的块参照
    /// @param [in] i 块参照引用
    virtual void setInsert(DmBlockReference& i);

    /// @brief 更新块参照以匹配用户修改
    virtual void updateInsert();

protected slots:
    virtual void languageChange();

private:
    DmBlockReference* m_pInsert = nullptr; ///< 待编辑的块参照指针
};

#endif // UIDLGINSERT_H
