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

/// @file DmFlags.cpp
/// @brief 标志位基类持久化实现

#include "DmFlags.h"

TYPESYSTEM_SOURCE_ABSTRACT(DmFlags, Persistence, 0)
// Constructor with initialisation to the given flags.

void DmFlags::saveStream(OutputStream& wrt) const
{
    wrt << (uint32_t)flags;
}

void DmFlags::restoreStream(InputStream& reader, const std::vector<PAIR>& revs)
{
    int fileRev = getRevisionId("DmFlags", revs);
    if (revId > fileRev)
    {
        // 老文件格式
        restoreStreamWithRev(reader, fileRev);
    }
    else
    {
        restoreStream(reader);
    }
}

void DmFlags::restoreStreamWithRev(InputStream& rdr, int rev)
{
    if (0 == rev)
    {
        // 基本版本，无需额外处理
    }
    else // big change, e.g. change super class of DmFlags
    {
        // step1.
        // read all legacy data one by one
    }
}

void DmFlags::restoreStream(InputStream& rdr)
{
    rdr >> (uint32_t&)flags;
}
