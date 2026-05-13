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

/// @file UIDlgDefineAttribute.h
/// @brief 属性定义对话框

#ifndef UIDLGDEFINEATTRIBUTE_H
#define UIDLGDEFINEATTRIBUTE_H

#include "ui_UIDlgDefineAttribute.h"
#include "DmAttributeDefinition.h"

/// @brief 属性定义对话框
class UIDlgDefineAttribute : public QDialog, public Ui_UIDlgDefineAttribute
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否模态
    /// @param [in] fl 窗口标志
    UIDlgDefineAttribute(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UIDlgDefineAttribute();

protected:
    /// @brief 显示事件
    /// @param [in] ev 显示事件
    void showEvent(QShowEvent* ev);

public slots:
    /// @brief 设置属性定义
    /// @param [in,out] attr 属性定义对象引用
    /// @param [in] isNew 是否新建
    virtual void setAttributeDefinition(DmAttributeDefinition& attr, bool isNew);

    /// @brief 更新属性定义
    virtual void updateAttributeDefinition();

    /// @brief 样式变化槽
    virtual void slotStyleChanged();

    /// @brief 取消操作
    virtual void reject();

    /// @brief 确认操作
    virtual void accept();

protected slots:
    /// @brief 语言切换槽
    virtual void languageChange();

private:
    /// @brief 修改文字时，对齐方式修改，对齐点坐标调整
    void adjustAlignmentForModeChange();

    /// @brief 初始化连接
    void init();

    /// @brief 需要时保存设置
    void saveSettingIfNeed();

private:
    bool m_isDlgShow;                     ///< 窗体是否已显示
    bool m_isNew;                         ///< 是否新建
    bool m_saveSettings;                  ///< 是否保存设置
    DmAttributeDefinition* m_pAttrDef;    ///< 属性定义对象指针
};

#endif // !UIDLGDEFINEATTRIBUTE_H
