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

/// @file DmPatternList.h
/// @brief 填充图案表，全局单例管理所有填充图案的加载与查询

#ifndef DMPATTERNLIST_H
#define DMPATTERNLIST_H

#include <map>
#include <memory>

class DmPattern;
class QString;

#define DMPATTERNLIST DmPatternList::instance()

/// @brief 填充图案表（单例模式），维护管理填充图案
class DmPatternList
{
    DmPatternList() = default;

public:
    /// @brief 获取全局唯一实例
    /// @return 图案列表单例指针
    static DmPatternList* instance();

    ~DmPatternList();
    DmPatternList(DmPatternList const&) = delete;
    DmPatternList& operator=(DmPatternList const&) = delete;
    DmPatternList(DmPatternList&&) = delete;
    DmPatternList& operator=(DmPatternList&&) = delete;

    /// @brief 初始化图案列表，从文件加载所有图案
    void init();

    /// @brief 获取图案数量
    /// @return 图案总数
    int countPatterns() const;

    // range based loop support
    std::map<QString, std::unique_ptr<DmPattern>>::iterator begin();
    std::map<QString, std::unique_ptr<DmPattern>>::const_iterator begin() const;
    std::map<QString, std::unique_ptr<DmPattern>>::iterator end();
    std::map<QString, std::unique_ptr<DmPattern>>::const_iterator end() const;

    /// @brief 按名称请求图案，若未加载则自动加载
    /// @param name 图案名称
    /// @return 图案指针，未找到返回 nullptr
    DmPattern* requestPattern(const QString& name);

    /// @brief 检查是否包含指定名称的图案
    /// @param name 图案名称
    /// @return 包含返回 true
    bool contains(const QString& name) const;

private:
    std::map<QString, std::unique_ptr<DmPattern>> m_patterns; ///< 应用中所有图案
};

#endif
