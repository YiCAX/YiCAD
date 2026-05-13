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

/// @file DmId.h
/// @brief 实体唯一标识符类，支持比较和哈希

#ifndef DMID_H
#define DMID_H

#include <string>
#include <ostream>

class DmId
{
public:
    explicit DmId();
    DmId(const DmId& id);
    explicit DmId(const std::string& idStr);

    /// @brief 检查ID是否有效（非默认值"0"）
    bool isValid() const;

    /// @brief 获取ID的字符串表示
    std::string asString() const;

    DmId& operator=(const DmId& id);
    bool operator==(const DmId& id) const;
    bool operator!=(const DmId& id) const;
    bool operator<(const DmId& id) const;

    friend std::ostream& operator<<(std::ostream& os, const DmId& id);

private:
    std::string m_idStr; ///< ID字符串
};

// 为了默认支持std::unordered_map
// std::unordered_map第3个参数的默认参数是std::hash<Key>，
// 这里对它进行模板定制，参考：https://blog.csdn.net/y109y/article/details/82669620
namespace std
{
    template<>
    class hash<DmId>
    {
    public:
        size_t operator()(const DmId& id) const
        {
            return hash<string>()(id.asString());
        }
    };
}

#endif // DMID_H
