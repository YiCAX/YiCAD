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

/// @file Printing.cpp
/// @brief 打印功能实现，纸张格式转换

#include "Printing.h"

/// @brief 将YiCAD纸张格式转换为Qt纸张格式
/// @param f YiCAD纸张格式
/// @return 对应的Qt纸张格式，未匹配时返回Custom
QPrinter::PageSize Printing::rsToQtPaperFormat(const DM::PaperFormat f)
{
	switch (f)
	{
	case DM::A0: return QPrinter::A0;
	case DM::A1: return QPrinter::A1;
	case DM::A2: return QPrinter::A2;
	case DM::A3: return QPrinter::A3;
	case DM::A4: return QPrinter::A4;

	case DM::Letter: return QPrinter::Letter;
	case DM::Legal:  return QPrinter::Legal;
	case DM::Tabloid: return QPrinter::Tabloid;

	case DM::Ansi_C: return QPrinter::AnsiC;
	case DM::Ansi_D: return QPrinter::AnsiD;
	case DM::Ansi_E: return QPrinter::AnsiE;

	case DM::Arch_A: return QPrinter::ArchA;
	case DM::Arch_B: return QPrinter::ArchB;
	case DM::Arch_C: return QPrinter::ArchC;
	case DM::Arch_D: return QPrinter::ArchD;
	case DM::Arch_E: return QPrinter::ArchE;

	default:
		return QPrinter::Custom;
	}
}
