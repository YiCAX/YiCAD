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

/// @file MetaSolids.h
/// @brief 填充(Solid)实体序列化容器头文件

#ifndef PERSISTENCE_META_SOLIDS_H
#define PERSISTENCE_META_SOLIDS_H

#include <vector>
#include <list>

#include "DmEntity.h"
#include "Persistence.h"

class DmDocument;

class MetaSolidsContainer : Persistence
{
public:
    /// @brief 构造函数
    /// @param [in] pDoc 文档指针
    MetaSolidsContainer(DmDocument* pDoc);

    /// @brief 析构函数
    virtual ~MetaSolidsContainer() {};

    /// @brief 获取内存大小
    /// @return 内存大小
    unsigned int getMemSize() const override;

    /// @brief 保存XML数据
    /// @param [in] wrt 写入器引用
    void saveXML(Writer& wrt) const override;

    /// @brief 从XML恢复数据
    /// @param [in] reader XML读取器引用
    void restoreXML(XMLReader& reader) override;

    /// @brief 保存二进制数据流
    /// @param [in] wrt 输出流引用
    void saveStream(OutputStream& wrt) const override;

    /// @brief 从二进制数据流恢复
    /// @param [in] rdr 输入流引用
    void restoreStream(InputStream& rdr) override;

    /// @brief 设置实体列表
    /// @param [in] entities 实体列表引用
    void setEntities(std::list<DmEntity*>& entities);

private:
    DmDocument*             m_pDocument = nullptr;  ///< 文档指针
    std::vector<PAIR>       m_revs;                 ///< 版本信息列表
    std::list<DmEntity*>    m_entities;             ///< 实体列表
};

#endif // PERSISTENCE_META_SOLIDS_H
