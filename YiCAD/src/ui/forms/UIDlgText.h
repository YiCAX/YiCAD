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

/// @file UIDlgText.h
/// @brief 文字实体属性编辑对话框

#ifndef UIDLGTEXT_H
#define UIDLGTEXT_H

#include "ui_UIDlgText.h"
#include "DmText.h"

class UIDlgText : public QDialog, public Ui::UIDlgText
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否为模态对话框
    /// @param [in] fl 窗口标志
    UIDlgText(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIDlgText();

protected:
    void showEvent(QShowEvent* ev);

public slots:
    /// @brief 设置要编辑的文字实体
    /// @param [in] t 文字实体引用
    /// @param [in] isNew 是否新建
    virtual void setText(DmText& t, bool isNew);

    /// @brief 更新文字实体以匹配用户修改
    virtual void updateText();

    virtual void slotStyleChanged();
    virtual void reject();
    virtual void accept();

protected slots:
    virtual void languageChange();

private:
    /// @brief 修改文字时，对齐方式修改，对齐点坐标调整
    void adjustAlignmentForModeChange();

    void init();
    void saveSettingIfNeed();

private:
    bool m_isDlgShow = false;    ///< 窗体是否已显示
    bool m_isNew = false;        ///< 是否新建
    bool m_saveSettings = true;  ///< 是否保存设置
    DmText* m_pText = nullptr;   ///< 待编辑的文字实体指针
};

#endif // UIDLGTEXT_H
