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

/// @file DmObject.h
/// @brief CAD 对象基类，提供ID管理、删除标记、文档关联和持久化支持

#ifndef DMOBJECT_H
#define DMOBJECT_H

#include "DmFlags.h"
#include "DmId.h"
#include <memory>

class DmDocument;
class ITable;

/// @brief CAD 对象基类，封装标识、删除标记和持久化
class DmObject : public DmFlags
{
    TYPESYSTEM_HEADER();

public:
    /// @brief 判断对象是否已被标记为删除
    /// @return 已删除返回 true，否则返回 false
    bool isErased() const;

    /// @brief 获取对象唯一标识
    /// @return 对象ID
    DmId getId() const;

    /// @brief 重置对象唯一标识
    void resetId();

    /// @brief 获取所属文档指针
    /// @return 文档指针，可能为 nullptr
    DmDocument* getDocument() const;

    /// @brief 设置所属文档
    /// @param pDoc 文档指针
    virtual void setDocument(DmDocument* pDoc)
    {
        m_pDocument = pDoc;
    }

    /// @brief 更新对象状态，由继承类实现具体逻辑
    virtual void update()
    {
    }

    /// @brief 设置删除标记
    /// @param erased 是否标记为已删除
    void setErased(bool erased);

    // persistent helper
    virtual void saveStream(OutputStream& wrt) const override;
    virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
    virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

protected:
    friend class DmIdManager;
    friend class Cmd;
    friend class TableBase;

private:
    /// @brief 设置对象唯一标识（仅友元类可调用）
    /// @param id 新的ID值
    void setId(DmId id);

protected:
    DmId m_ulID;                      ///< 对象id
    DmDocument* m_pDocument = nullptr; ///< 对象所属文档
    bool m_bIsErased = false;         ///< 标记是否已删除，已删除对象不立即从内存释放
};

#endif // DMOBJECT_H
