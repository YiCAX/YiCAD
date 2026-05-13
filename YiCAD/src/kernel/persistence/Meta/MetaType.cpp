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
/// @file MetaType.cpp
/// @brief 类型系统元类型实现文件

# include <cassert>
#include "MetaType.h"

Type MetaType::classTypeId = Type::badType();
int  MetaType::revId = 0;

//**************************************************************************
// Construction/Destruction

/**
 * A constructor.
 * A more elaborate description of the constructor.
 */
MetaType::MetaType()
{
}

/**
 * A destructor.
 * A more elaborate description of the destructor.
 */
MetaType::~MetaType()
{
}


//**************************************************************************
// separator for other implementation aspects

/// @brief 初始化MetaType类型系统
void MetaType::initialize()
{
	assert(MetaType::classTypeId == Type::badType() && "don't init() twice!");
	/* Set up entry in the type system. */
	MetaType::classTypeId =
		Type::createType(Type::badType(),
			"MetaType",
			MetaType::create);
}

/// @brief 获取类类型ID
/// @return 类型对象
Type MetaType::getClassTypeId()
{
	return MetaType::classTypeId;
}

/// @brief 获取实例类型ID
/// @return 类型对象
Type MetaType::getTypeId() const
{
	return MetaType::classTypeId;
}

/// @brief 获取版本ID列表
/// @param vecRev 版本对列表
void MetaType::getRevId(std::vector<PAIR>& vecRev)
{
	vecRev.push_back(std::make_pair(getClassTypeId().getName(), MetaType::revId));
}


/// @brief 初始化子类
/// @param toInit 待初始化的类型
/// @param ClassName 类名
/// @param ParentName 父类名
/// @param method 实例化方法
void MetaType::initSubclass(Type& toInit, const char* ClassName, const char* ParentName,
	Type::instantiationMethod method)
{
	// don't init twice!
	assert(toInit == Type::badType());
	// get the parent class
	Type parentType(Type::fromName(ParentName));
	// forgot init parent!
	assert(parentType != Type::badType());

	// create the new type
	toInit = Type::createType(parentType, ClassName, method);
}
