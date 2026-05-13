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

/// @file AttributeData.h
/// @brief 属性数据类

#ifndef ATTRIBUTEDATA_H
#define ATTRIBUTEDATA_H

#include "EntityData.h"
#include <QString>

/// @brief 属性数据
class AttributeData : public EntityData
{
public:
    AttributeData();

    /// @brief 使用标签构造
    /// @param [in] tag 属性标签
    AttributeData(const QString& tag);

    /// @brief 获取标签
    /// @return 标签字符串
    QString getTag() const;

    /// @brief 设置标签
    /// @param [in] tag 标签字符串
    void setTag(const QString& tag);

private:
    QString m_strTag; ///< 标记
    // 默认是基类的text，不另外保存
};

#endif // ATTRIBUTEDATA_H
