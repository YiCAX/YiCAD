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

/// @file FilterInterface.h
/// @brief 文件转换器接口类头文件

#ifndef FILTERINTERFACE_H
#define FILTERINTERFACE_H

#include "DmDocument.h"

#include <QObject>

/// @brief 转换器接口类
class FilterInterface
{
public:
	FilterInterface() = default;
	virtual ~FilterInterface() = default;

	/// @brief 判断类型是否支持导入
	virtual bool canImport(const QString& type) const = 0;

	/// @brief 判断类型是否支持导出
	virtual bool canExport(const QString& type) const = 0;

	/// @brief 导入文件并存储实体到当前实体容器中
	virtual bool fileImport(DmDocument& document, const QString& file) = 0;

	/// @brief 导出指定格式的文件
	virtual bool fileExport(DmDocument& document, const QString& file, const QString& formatType) = 0;

	/// @brief Request the error message for the last import/export action, based on member variable \p errorCode.
	/// The default implementation is for existing filters, inherited without error handling methods.
	/// It is strongly recommend for new implementations to overwrite this method with some useful error messages.
	virtual QString lastError() const
	{
		return QObject::tr("undefined error", "FilterInterface");
	}

	/// @brief Request the error code for the last import/export action.
	/// The default value 0 means no error.
	virtual int lastErrorCode() const
	{
		return errorCode;
	}

	static FilterInterface* createFilter()
	{
		return NULL;
	}

protected:
	/// 上次导入/导出操作的错误代码
	int errorCode = 0;
};

#endif
