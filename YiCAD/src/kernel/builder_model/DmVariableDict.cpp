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

/// @file DmVariableDict.cpp
/// @brief DmVariableDict 变量字典类的实现

#include "DmVariableDict.h"

#include<iostream>
#include <QString>

#include "Debug.h"

void DmVariableDict::clear()
{
	m_variables.clear();
}

// Adds a variable to the variable dictionary. If a variable with the same name already exists, is will be overwritten.
void DmVariableDict::add(const QString& key, const QString& value, int code)
{
	if (key.isEmpty())
	{
		return;
	}

	m_variables.insert(key, DmVariable(value, code));
}

void DmVariableDict::add(const QString& key, int value, int code)
{
	if (key.isEmpty())
	{
		return;
	}

	m_variables.insert(key, DmVariable(value, code));
}

void DmVariableDict::add(const QString& key, double value, int code)
{
	if (key.isEmpty())
	{
		return;
	}
	m_variables.insert(key, DmVariable(value, code));
}

int DmVariableDict::count() const
{
	return m_variables.count();
}

void DmVariableDict::add(const QString& key, const DmVector& value, int code)
{
	if (key.isEmpty())
	{
		return;
	}

	m_variables.insert(key, DmVariable(value, code));
}

/// @brief Gets the value for the given variable.
///	@param key Key of the variable.
/// @param def Default value.
/// @return The value for the given variable or the given default value if the variable couldn't be found.
DmVector DmVariableDict::getVector(const QString& key, const DmVector& def) const
{
	DmVector ret;

	auto i = m_variables.find(key);
	if (m_variables.end() != i && DM::VariableVector == i.value().getType())
	{
		ret = i.value().getVector();
	}
	else
	{
		ret = def;
	}

	return ret;
}

QString DmVariableDict::getString(const QString& key, const QString& def) const
{
	QString ret;

	auto i = m_variables.find(key);
	if (m_variables.end() != i && DM::VariableString == i.value().getType())
	{
		ret = i.value().getString();
	}
	else
	{
		ret = def;
	}

	return ret;
}

int DmVariableDict::getInt(const QString& key, int def) const
{
	int ret = 0;

	auto i = m_variables.find(key);
	if (m_variables.end() != i && DM::VariableInt == i.value().getType())
	{
		ret = i.value().getInt();
	}
	else
	{
		ret = def;
	}

	return ret;
}

double DmVariableDict::getDouble(const QString& key, double def) const
{
	double ret = 0.0;

	auto i = m_variables.find(key);
	if (m_variables.end() != i && DM::VariableDouble == i.value().getType())
	{
		ret = i.value().getDouble();
	}
	else
	{
		ret = def;
	}

	return ret;
}

/// Removes a variable from the list.
///	TODO: Listeners are notified after the block was removed from the list but before it gets deleted.
void DmVariableDict::remove(const QString& key)
{
	m_variables.remove(key);
}

QHash<QString, DmVariable> const& DmVariableDict::getVariableDict() const
{
	return m_variables;
}

QHash<QString, DmVariable>& DmVariableDict::getVariableDict()
{
	return m_variables;
}
