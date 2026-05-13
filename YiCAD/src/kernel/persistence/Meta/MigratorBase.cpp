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
/// @file MigratorBase.cpp
/// @brief 数据迁移器基类实现文件

#include <TSingleton.hpp>
#include <assert.h>
#include "MigratorBase.h"

/// @brief 执行所有迁移器的后恢复操作
/// @return 全部成功返回true，否则返回false
bool DmMigrateContext::postRestore()
{
	bool bResult = true;
	for (auto img : m_vecMigrators)
	{
		if (img != nullptr)
		{
			bResult = bResult && img->ExecuteMigration();
			if (!bResult)
			{
				assert(bResult);
				m_vecErrorMsgs.push_back(img->Error());
			}
		}
	}

	return bResult;
}
