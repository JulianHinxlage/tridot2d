//
// Copyright (c) 2023 Julian Hinxlage. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

namespace tridot2d {

	std::string readFile(const std::string& file, bool binary = false);

	bool writeFile(const std::string& file, const std::string& text, bool binary = false);

	std::vector<std::string> split(const std::string& string, const std::string& delimiter, bool includeEmpty = false);

	std::vector<std::string> split(const std::vector<std::string>& strings, const std::string& delimiter, bool includeEmpty = false);

	std::string join(const std::vector<std::string>& strings, const std::string& delimiter);

	std::string replace(const std::string& string, const std::string& search, const std::string& replacement);

	std::string searchPath(std::string path);

	int match(const std::string& string1, const std::string& string2);

	void trimFront(std::string& string, const std::string& prefix, bool trimAllIfNotFound = false);

	void trimBack(std::string& string, const std::string& suffix);

	std::string subString(const std::string& string, const std::string& prefix, const std::string& suffix);

	std::string leftPadd(const std::string& string, char character, int targetLength);

	bool stringContains(const std::string& string, const std::string& subString);

	std::string toLower(const std::string& string);
	
	std::string toUpper(const std::string& string);

	int toInt(const std::string& str, int defaultValue = -1);

	float toFloat(std::string str, float defaultValue = 0, bool allowCommaAsPoint = false);

	uint8_t hexCharToInt(char hex);

	uint64_t hexToInt(const std::string& hex);

	char intToHexChar(uint8_t value);

	std::string intToHex(uint64_t value);

	bool isNumber(const std::string& str);

	template<typename T>
	static T fromString(const std::string& string, T defaultValue = T()) {
		if (std::is_same<T, int>()) {
			try {
				return std::stoi(string);
			}
			catch (...) {}
		}
		else if (std::is_same<T, float>()) {
			try {
				return std::stof(string);
			}
			catch (...) {}
		}
		else if (std::is_same<T, double>()) {
			try {
				return std::stod(string);
			}
			catch (...) {}
		}
		else if (std::is_same<T, bool>()) {
			try {
				if (string == "true") {
					return true;
				}else if(string == "false") {
					return false;
				}
				else {
					return std::stoi(string);
				}
			}
			catch (...) {}
		}
		return defaultValue;
	}

	template<>
	static std::string fromString<std::string>(const std::string& string, std::string defaultValue) {
		return string;
	}

	template<typename T>
	static std::string toString(T value) {
		return std::to_string(value);
	}

	template<>
	static std::string toString<std::string>(std::string value) {
		return value;
	}

}
