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

/// @file MetaTextStyles.h
/// @brief 文字样式实体序列化容器头文件

#ifndef PERSISTENCE_META_TEXTSTYLES_H
#define PERSISTENCE_META_TEXTSTYLES_H

#include <vector>

#include "DmEntity.h"
#include "Persistence.h"

class DmTextStyle;
class DmDocument;

/// 文字样式序列化容器
class MetaTextStylesContainer : Persistence
{
public:
	/// @brief 构造函数
	/// @param pDoc 文档指针
	MetaTextStylesContainer(DmDocument* pDoc);

	/// @brief 析构函数
	virtual ~MetaTextStylesContainer()
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

	/// @brief 保存为二进制流
	/// @param wrt OutputStream对象引用
	void saveStream(OutputStream& wrt) const override;

	/// @brief 从二进制流恢复
	/// @param rdr InputStream对象引用
	void restoreStream(InputStream& rdr) override;

	/// @brief 设置文字样式列表
	/// @param styles 文字样式向量引用
	void setTextStyles(std::vector<DmTextStyle*>& styles);

private:
	DmDocument*                 m_pDocument = nullptr;  ///< 文档指针
	std::vector<PAIR>           m_revs;                 ///< 版本列表
	std::vector<DmTextStyle*>   m_textStyleList;        ///< 文字样式列表
	QString                     m_strActiveStyle;       ///< 当前激活的样式名称
};

#endif //PERSISTENCE_META_TEXTSTYLES_H
