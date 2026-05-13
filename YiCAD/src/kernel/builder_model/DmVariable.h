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

/// @file DmVariable.h
/// @brief 变量类，支持int/double/string/vector类型

#ifndef DMVARIABLE_H
#define DMVARIABLE_H

#include <QString>

#include "Datamodel.h"
#include "DmVector.h"

/// @brief A variable of type int, double, string or vector.
///	The variable also contains its type and an int code which can identify its contents in any way.
class DmVariable
{
	struct DmVariableContents
	{
		QString s;
		int i;
		double d;
		DmVector v;
	};

public:
	DmVariable() = default;
	DmVariable(const DmVector& v, int c)
		: m_iCode(c)
	{
		setVector(v);
	}

	DmVariable(const QString& v, int c) 
		: m_iCode(c)
	{
		setString(v);
	}

	DmVariable(int v, int c) 
		: m_iCode(c)
	{
		setInt(v);
	}

	DmVariable(double v, int c) 
		: m_iCode(c)
	{
		setDouble(v);
	}

	void setString(const QString& str)
	{
		m_contents.s = str;
		m_eType = DM::VariableString;
	}

	void setInt(int i)
	{
		m_contents.i = i;
		m_eType = DM::VariableInt;
	}

	void setDouble(double d)
	{
		m_contents.d = d;
		m_eType = DM::VariableDouble;
	}

	void setVector(const DmVector& v)
	{
		m_contents.v = v;
		m_eType = DM::VariableVector;
	}

	QString getString() const
	{
		return m_contents.s;
	}

	int getInt() const
	{
		return m_contents.i;
	}

	double getDouble() const
	{
		return m_contents.d;
	}

	DmVector getVector() const
	{
		return m_contents.v;
	}

	DM::VariableType getType() const
	{
		return m_eType;
	}

	int getCode() const
	{
		return m_iCode;
	}

private:
	DmVariableContents	m_contents;
	DM::VariableType	m_eType = DM::VariableVoid;
	int					m_iCode = 0;
};

#endif
