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

/// @file DmVariableDict.h
/// @brief 变量字典类，以键值对方式存储变量

#ifndef DMVARIABLEDICT_H
#define DMVARIABLEDICT_H

#include <QHash>

#include "DmVariable.h"

class DmVector;
class QString;

/// @brief Dictionary of variables. The variables are stored as key/value pairs (string/string).
class DmVariableDict
{
public:
	DmVariableDict() = default;

	void clear();

	/// @return Number of variables available.
	int count() const;

	void add(const QString& key, const DmVector& value, int code);
	void add(const QString& key, const QString& value, int code);
	void add(const QString& key, int value, int code);
	void add(const QString& key, double value, int code);

	DmVector getVector(const QString& key, const DmVector& def) const;
	QString getString(const QString& key, const QString& def) const;
	int getInt(const QString& key, int def) const;
	double getDouble(const QString& key, double def) const;

	void remove(const QString& key);

	QHash<QString, DmVariable> const& getVariableDict() const;
	QHash<QString, DmVariable>& getVariableDict();

private:
	QHash<QString, DmVariable> m_variables;
};

#endif
