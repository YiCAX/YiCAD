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

/// @file DmCharTemplateList.h
/// @brief 文字模板列表类，管理文字模板（DmCharTemplate）

#ifndef DMCHARTEMPLATELIST_H
#define DMCHARTEMPLATELIST_H

#include <map>
#include <QString>

class DmCharTemplate;
class DmFont;

/// @brief 文字模板列表。管理文字模板（DmCharTemplate）的类
class DmCharTemplateList
{
public:
    /// @brief 析构函数，释放所有文字模板
    ~DmCharTemplateList();

    /// @brief 添加文字模板
    /// @param name 文字模板名
    /// @param letter 文字模板指针
    void add(const QString& name, DmCharTemplate* letter);

    /// @brief 查找文字模板
    /// @param name 文字模板名
    /// @return 找到返回模板指针，否则返回nullptr
    DmCharTemplate* find(const QString& name) const;

    /// @brief 获得所属字体
    /// @return 字体指针
    DmFont* getFont() const;

    /// @brief 设置所属字体
    /// @param f 字体指针
    void setFont(DmFont* f);

private:
    std::map<QString, DmCharTemplate*> m_fontChars; ///< 管理的文字模板。里面的DmFontChar由析构函数释放
    DmFont* m_pFont{nullptr}; ///< 文字模板所属字体
};

#endif //!DMCHARTEMPLATELIST_H
