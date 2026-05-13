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

/// @file Debug.h
/// @brief 调试工具类头文件

#ifndef DEBUG_H
#define DEBUG_H

#include <iosfwd>
#ifdef __hpux
#include <sys/_size_t.h>
#endif

class QString;

// print out a debug header
#define DEBUG_HEADER debugHeader(__FILE__, __func__, __LINE__);
void debugHeader(char const* file, char const* func, int line);
#define DEBUG Debug::instance()
#define DEBUG_VERBOSE \
        DEBUG_HEADER \
        Debug::instance()

/// @brief Debugging facilities.
class Debug
{
public:
	enum DebugLevel
	{
		D_NOTHING,
		D_CRITICAL,
		D_ERROR,
		D_WARNING,
		D_NOTICE,
		D_INFORMATIONAL,
		D_DEBUGGING
	};

private:
	Debug();
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
	Debug(Debug&&) = delete;
	Debug& operator=(Debug&&) = delete;

public:
	static Debug* instance();

	static void deleteInstance();
	void setLevel(DebugLevel level);
	DebugLevel getLevel();
	void print(DebugLevel level, const char* format...);
	void print(const char* format,...);
	void printUnicode(const QString& text);
	void timestamp();
	void setStream(FILE* s);

private:
	static Debug* uniqueInstance;

	DebugLevel debugLevel;
	FILE* stream;
};

#endif
