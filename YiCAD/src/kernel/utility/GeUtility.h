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

/// @file GeUtility.h
/// @brief 几何工具类，提供实体类型判断等静态工具方法

#ifndef GEUTILITY_H
#define GEUTILITY_H

class DmEntity;

/// @brief 几何工具类
class GeUtility
{
public:
    /// @brief 判断实体是否为圆弧类型（圆弧、圆或椭圆）
    /// @param e 待判断的实体
    /// @return 若为圆弧类型则返回true
    static bool isEntityArc(const DmEntity* e);
};

#endif // GEUTILITY_H
