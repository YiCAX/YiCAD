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

/// @file DmUtility.cpp
/// @brief DmUtility 工具类的实现

#include "DmUtility.h"

#include <QString>

// Converts a double to a string cutting away unnecessary 0's.
// e.g. 2.70000  -> 2.7
QString DmUtility::doubleToString(double value, int precision)
{
	QString ret;

	ret.setNum(value, 'f', precision);

	if (ret.contains('.'))
	{
		// remove trailing zeros:
		while (ret.at(ret.length() - 1) == '0')
		{
			ret.truncate(ret.length() - 1);
		}
		// remove trailing .
		if (ret.at(ret.length() - 1) == '.')
		{
			ret.truncate(ret.length() - 1);
		}
	}
	return ret;
}
