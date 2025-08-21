//
// Copyright (c) 2023 Julian Hinxlage. All rights reserved.
//

#include "Log.h"
#include "util/DateTime.h"
#include <cstdarg>
#include <mutex>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <stdio.h>


namespace tridot2d {

	const char* logLevelName(LogLevel level) {
		switch (level)
		{
		case LogLevel::TRACE:
			return "TRACE";
		case LogLevel::DEBUG:
			return "DEBUG";
		case LogLevel::INFO:
			return "INFO";
		case LogLevel::WARNING:
			return "WARNING";
		case LogLevel::ERROR:
			return "ERROR";
		case LogLevel::FATAL:
			return "FATAL";
		default:
			return "DEFAULT";
		}
	}

	std::mutex mutex;
	LogLevel consoleLevel = LogLevel::TRACE;
	class LogFile {
	public:
		std::string filename;
		LogLevel level;
		std::ofstream stream;
	};
	std::vector<LogFile> logFiles;
	std::function<void(LogLevel level, const std::string& message)> logCallback = nullptr;

	void Log::setConsoleLogLevel(LogLevel level) {
		consoleLevel = level;
	}

	void Log::addLogFile(const std::string& file, LogLevel level) {
		if (file.empty()) {
			return;
		}
		if (!std::filesystem::exists(std::filesystem::path(file).parent_path())) {
			std::filesystem::create_directories(std::filesystem::path(file).parent_path());
		}

		std::unique_lock<std::mutex> lock(mutex);
		logFiles.push_back({ file, level });
		logFiles.back().stream.open(file, std::ios_base::app);
	}

	void Log::setLogCallback(const std::function<void(LogLevel level, const std::string& message)>& callback) {
		logCallback = callback;
	}

	void Log::removeLogFile(const std::string& file) {
		std::unique_lock<std::mutex> lock(mutex);
		for (int i = 0; i < logFiles.size(); i++) {
			if (logFiles[i].filename == file) {
				logFiles[i].stream.close();
				logFiles.erase(logFiles.begin() + i);
				i--;
			}
		}
	}

	void Log::removeAllLogFiles() {
		std::unique_lock<std::mutex> lock(mutex);
		for (int i = 0; i < logFiles.size(); i++) {
			logFiles[i].stream.close();
			logFiles.erase(logFiles.begin() + i);
			i--;
		}
	}

	void Log::log(LogLevel level, const char* fmt, va_list args) {
		std::unique_lock<std::mutex> lock(mutex);

		DateTime time;
		time.now();

		for (auto& f : logFiles) {
			if (level >= f.level) {
				f.stream << "[" << time.toStringDate() << "] ";
				f.stream << "[" << time.toStringTime() << "] ";
				f.stream << "[" << logLevelName(level) << "] ";
				char buf[102400];
				try {
					int i = vsnprintf(buf, 102400, fmt, args);
					buf[i] = '\0';
				}
				catch (...) {
					buf[0] = '\0';
				}
				f.stream << (const char*)buf << "\n";
				f.stream.flush();
			}
		}

		if (logCallback) {
			std::stringstream s;
			s << "[" << time.toStringDate() << "] ";
			s << "[" << time.toStringTime() << "] ";
			s << "[" << logLevelName(level) << "] ";
			char buf[102400];
			try {
				int i = vsnprintf(buf, 102400, fmt, args);
				buf[i] = '\0';
			}
			catch (...) {
				buf[0] = '\0';
			}
			s << (const char*)buf << "\n";

			logCallback(level, s.str());
		}

		if (level >= consoleLevel) {
			printf("[%s] ", time.toStringDate().c_str());
			printf("[%s] ", time.toStringTime().c_str());
			printf("[%s] ", logLevelName(level));
			vprintf(fmt, args);
			printf("\n");
		}
	}

	void Log::log(LogLevel level, const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		log(level, fmt, args);
		va_end(args);
	}

	void Log::trace(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		log(LogLevel::TRACE, fmt, args);
		va_end(args);
	}
	
	void Log::debug(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		log(LogLevel::DEBUG, fmt, args);
		va_end(args);
	}
	
	void Log::info(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		log(LogLevel::INFO, fmt, args);
		va_end(args);
	}
	
	void Log::warning(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		log(LogLevel::WARNING, fmt, args);
		va_end(args);
	}
	
	void Log::error(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		log(LogLevel::ERROR, fmt, args);
		va_end(args);
	}
	
	void Log::fatal(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		log(LogLevel::FATAL, fmt, args);
		va_end(args);
	}

}
