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

/// @file DmObject.cpp
/// @brief CAD 对象基类实现：ID管理、删除标记、流式持久化

#include "DmObject.h"
#include "TableBase.h"
#include <sstream>

TYPESYSTEM_SOURCE_ABSTRACT(DmObject, DmFlags, 0)

/// @brief 获取对象唯一标识
/// @return 对象ID
DmId DmObject::getId() const
{
    return m_ulID;
}

void DmObject::resetId()
{
    m_ulID = DmId();
}

/// @brief 获取所属文档指针
/// @return 文档指针
DmDocument* DmObject::getDocument() const
{
    return m_pDocument;
}

/// @brief 判断对象是否已被标记为删除
/// @return 已删除返回 true
bool DmObject::isErased() const
{
    return m_bIsErased;
}

/// @brief 设置对象唯一标识
/// @param id 新的ID值
void DmObject::setId(DmId id)
{
    m_ulID = id;
}

/// @brief 设置删除标记
/// @param erased 是否标记为已删除
void DmObject::setErased(bool erased)
{
    m_bIsErased = erased;
}

void DmObject::saveStream(OutputStream& wrt) const
{
    DmFlags::saveStream(wrt);
    wrt << (std::string)m_ulID.asString();
}

void DmObject::restoreStream(InputStream& reader, const std::vector<PAIR>& revs)
{
    int fileRev = getRevisionId("DmObject", revs);
    if (revId > fileRev)
    {
        DmFlags::restoreStream(reader, revs);
        // 老文件格式
        restoreStreamWithRev(reader, fileRev);
    }
    else
    {
        restoreStream(reader);
    }
}

void DmObject::restoreStreamWithRev(InputStream& rdr, int rev)
{
    if (rev == 0)
    {
    }
    else // big change, e.g. change supper class of DmEntity
    {
        // step1.
        // read all legacy data one by one
    }
}

void DmObject::restoreStream(InputStream& rdr)
{
    DmFlags::restoreStream(rdr);
    std::string idStr;
    rdr >> (std::string&)idStr;
    m_ulID = DmId(idStr);
}
