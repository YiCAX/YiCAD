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

/// @file UIDlgEditAttributes.h
/// @brief 块属性编辑对话框，用于设置和获取块引用的属性值

#ifndef UIDLGEDITATTRIBUTES_H
#define UIDLGEDITATTRIBUTES_H

#include "ui_UIDlgEditAttributes.h"

class DmAttributeDefinition;
class DmAttribute;

class UIDlgEditAttributes : public QDialog, public Ui::UIDlgEditAttributes
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] fl 窗口标志
    UIDlgEditAttributes(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 设置对话框数据
    /// @param [in] blockName 块名称
    /// @param [in] attrDefs 属性定义列表
    void setData(const QString& blockName, const std::list<DmAttributeDefinition*>& attrDefs);

    /// @brief 获取用户修改后的属性列表
    /// @return 属性列表
    std::list<DmAttribute*> getAttributes() const;

private slots:
    void slotOk();
    void slotCancel();

private:
    std::vector<DmAttributeDefinition*> m_attrDefs; ///< 输入的属性定义
    std::list<DmAttribute*> m_attrs;                ///< 用来输出的属性
};

#endif // !UIDLGEDITATTRIBUTES_H
