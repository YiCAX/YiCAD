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

/// @file DmPen.cpp
/// @brief 画笔类实现：构造函数和析构函数

#include "DmPen.h"
#include "DmLineTypeTable.h"

#include <iostream>

DmPen::DmPen()
    : DmFlags()
{
    setColor(DmColor(0, 0, 0));
    setWidth(DM::Width00);
    setLineType(DmLineTypeTable::Continuous);
}

DmPen::DmPen(const DmColor& c, DM::LineWidth w, DmLineType* t)
    : DmFlags()
{
    setColor(c);
    setWidth(w);
    setLineType(t);
}

DmPen::DmPen(unsigned int f)
    : DmFlags(f)
{
    setColor(DmColor(0, 0, 0));
    setWidth(DM::Width00);
    setLineType(DmLineTypeTable::Continuous);
}

DmPen::~DmPen()
{
}
