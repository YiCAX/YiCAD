/*
 * Copyright (C) 2026 YiCAD
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

/// @file Main.h
/// @brief YiCAD 应用程序主入口头文件
/// @details 定义应用程序启动相关的宏和声明

#ifndef MAIN_H
#define MAIN_H

#include <QStringList>

/// @brief 将宏参数转换为字符串（无法用constexpr替代，因为使用了预处理器字符串化运算符#）
#define STR(x) #x

/// @brief 将宏参数展开后转换为字符串（无法用constexpr替代，因为依赖STR宏的字符串化）
#define XSTR(x) STR(x)

#endif
