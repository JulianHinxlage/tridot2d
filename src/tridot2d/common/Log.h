//
// Copyright (c) 2023 Julian Hinxlage. All rights reserved.
//

#pragma once

#include <string>
#include <functional>

#undef ERROR

namespace tridot2d {

	enum class LogLevel {
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		FATAL,
	};

	class Log {
	public:
		static void setConsoleLogLevel(LogLevel level);
		static void addLogFile(const std::string& file, LogLevel level);
		static void setLogCallback(const std::function<void(LogLevel level, const std::string &message)>& callback);
		static void removeLogFile(const std::string& file);
		static void removeAllLogFiles();

		static void log(LogLevel level, const char* fmt, va_list args);
		static void log(LogLevel level, const char* fmt, ...);
		static void trace(const char *fmt, ...);
		static void debug(const char *fmt, ...);
		static void info(const char *fmt, ...);
		static void warning(const char* fmt, ...);
		static void error(const char *fmt, ...);
		static void fatal(const char *fmt, ...);
	};

}
