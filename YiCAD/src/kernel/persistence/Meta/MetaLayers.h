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

/// @file MetaLayers.h
/// @brief 图层实体序列化容器头文件

#ifndef PERSISTENCE_META_LAYERS_H
#define PERSISTENCE_META_LAYERS_H

#include "DmEntity.h"
#include "Persistence.h"

class DmDocument;

/// 图层序列化容器
class MetaLayersContainer : Persistence
{
public:
	/// @brief 构造函数
	/// @param pDoc 文档指针
	MetaLayersContainer(DmDocument* pDoc);

	/// @brief 析构函数
	virtual ~MetaLayersContainer()
	{
	}

	/// @brief 获取内存大小
	/// @return 内存大小（字节）
	unsigned int getMemSize() const override;

	/// @brief 保存为XML格式
	/// @param wrt Writer对象引用
	void saveXML(Writer& wrt) const override;

	/// @brief 从XML格式恢复
	/// @param reader XMLReader对象引用
	void restoreXML(XMLReader& reader) override;

private:
	DmDocument*     m_pDocument = nullptr;  ///< 文档指针
	unsigned int    m_iCount = 0;           ///< 图层数量
};

#endif //PERSISTENCE_META_LAYERS_H
