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

/// @file Debug.cpp
/// @brief 调试工具类实现

#include "Debug.h"

#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <QString>

#include <QDateTime>
#include <QDebug>

Debug* Debug::uniqueInstance = nullptr;
void debugHeader(char const* file, char const* func, int line)
{
	std::cout << file << " : " << func << " : line " << line << std::endl;
}

// Gets the one and only Debug instance (creates a new one on first call only) 
// @return Pointer to the single instance of this singleton class
Debug* Debug::instance()
{
	if (!uniqueInstance)
	{
		QDateTime now = QDateTime::currentDateTime();
		QString nowStr;
		nowStr = now.toString("yyyyMMdd_hhmmss");
		QString fName = QString("debug_%1.log").arg(nowStr);

		uniqueInstance = new Debug;
		uniqueInstance->stream = stderr;
	}
	return uniqueInstance;
}

void Debug::deleteInstance()
{
	if (uniqueInstance)
	{
		fclose(uniqueInstance->stream);
		delete uniqueInstance;
	}
}

// Constructor setting the default debug level.
Debug::Debug()
{
#ifdef NDEBUG
	debugLevel = D_NOTHING;
#else
	debugLevel = D_DEBUGGING;
#endif // NDEBUG

}

// Sets the debugging level.
void Debug::setLevel(DebugLevel level)
{
	if (debugLevel == level)
	{
		return;
	}
	debugLevel = level;
	print(D_NOTHING, "DEBUG::setLevel(%d)", level);
	print(D_CRITICAL, "DEBUG: Critical");
	print(D_ERROR, "DEBUG: Errors");
	print(D_WARNING, "DEBUG: Warnings");
	print(D_NOTICE, "DEBUG: Notice");
	print(D_INFORMATIONAL, "DEBUG: Informational");
	print(D_DEBUGGING, "DEBUG: Debugging");
}

// Gets the current debugging level.
Debug::DebugLevel Debug::getLevel()
{
	return debugLevel;
}

// Prints the given message to stdout.
void Debug::print(const char* format ...)
{
	if (debugLevel == D_DEBUGGING)
	{
		va_list ap;
		va_start(ap, format);
		vfprintf(stream, format, ap);
		fprintf(stream, "\n");
		va_end(ap);
		fflush(stream);
	}
}

// Prints the given message to stdout if the current debug level is lower then the given level
// @param level Debug level.
void Debug::print(DebugLevel level, const char* format ...)
{
	if (debugLevel >= level)
	{
		va_list ap;
		va_start(ap, format);
		vfprintf(stream, format, ap);
		fprintf(stream, "\n");
		va_end(ap);
		fflush(stream);
	}
}

// Prints a time stamp in the format yyyyMMdd_hhmmss.
void Debug::timestamp()
{
	QDateTime now = QDateTime::currentDateTime();
	QString nowStr;

	nowStr = now.toString("yyyyMMdd_hh:mm:ss:zzz ");
	fprintf(stream, "%s", nowStr.toLatin1().data());
	fprintf(stream, "\n");
	fflush(stream);
}

void Debug::setStream(FILE* s)
{
	stream = s;
}

// Prints the unicode for every character in the given string.
void Debug::printUnicode(const QString& text)
{
	for (auto const& v : text)
	{
		print("[%X] %c", v.unicode(), v.toLatin1());
	}
}
