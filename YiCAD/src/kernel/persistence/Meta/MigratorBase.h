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
/// @file MigratorBase.h
/// @brief 数据迁移器基类头文件

#ifndef PERSISTENCE_MIGRATOR_BASE_H
#define PERSISTENCE_MIGRATOR_BASE_H

#include <TSingleton.hpp>
#include <assert.h>
#include <string>
#include <vector>

/// this is a base class for all migrators, you will derive a migrator from it
/// if you have complex data migrator, and please implement the ExecuteMigraion function
/// add the derived object. they will called post restore the whole document.
/// return false if fail to migrate the data
/// return true if succeed.
class DmMigratorBase
{
public:
	/// @brief 执行数据迁移
	/// @return 迁移成功返回true，失败返回false
	virtual bool ExecuteMigration()
	{
		return false;
	}

	/// @brief 获取错误信息
	/// @return 错误信息字符串
	virtual std::string Error()
	{
		return "";
	}
};

/// singleton migration context
class DmMigrateContext : public TSingleton<DmMigrateContext>
{
public:
	/// @brief 添加迁移器
	/// @param pMigrator 迁移器指针
	/// @return 始终返回true
	bool addMigrator(DmMigratorBase* pMigrator)
	{
		m_vecMigrators.push_back(pMigrator);
		return true; // TODO: 确认返回值语义
	}

	/// @brief 执行所有迁移器的后恢复操作
	/// @return 全部成功返回true，否则返回false
	bool postRestore();

	/// @brief 获取错误信息列表
	/// @return 错误信息列表
	auto errMsgs()
	{
		return m_vecErrorMsgs;
	}

private:
	std::vector<DmMigratorBase*> m_vecMigrators;   ///< 迁移器列表
	std::vector<std::string>     m_vecErrorMsgs;   ///< 错误信息列表
};

#endif //PERSISTENCE_MIGRATOR_BASE_H
