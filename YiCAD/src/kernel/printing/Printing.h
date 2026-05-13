/*
 * Copyright (C) 2026 The YiCAD Contributors
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

/// @file Printing.h
/// @brief 打印功能命名空间，提供纸张格式转换等打印相关功能

#ifndef PRINTING_H
#define PRINTING_H

#include <QPrinter>
#include "Datamodel.h"

/// @brief 打印相关功能
namespace Printing
{
	/// @brief 将YiCAD纸张格式转换为Qt纸张格式
	/// @param f YiCAD纸张格式
	/// @return 对应的Qt纸张格式
	QPrinter::PageSize rsToQtPaperFormat(DM::PaperFormat f);
}

#endif // PRINTING_H
