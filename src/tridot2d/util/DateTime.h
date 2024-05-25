//
// Copyright (c) 2023 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "strutil.h"

namespace tridot2d {

	class DateTime {
	public:
		int year;
		int month;
		int day;

		int hour;
		int minute;
		int second;

		DateTime();
		DateTime(int day, int month, int year);
		DateTime(int day, int month, int year, int hour, int minute, int second);
		void setDate(int day, int month, int year);
		void setTime(int hour, int minute, int second);
		void now(bool localTime = true);
		uint32_t toTimeStamp(bool localTime = true);
		void fromTimeStamp(uint32_t time, bool localTime = true);
		void resetTime();
		bool operator<(const DateTime& dateTime) const;
		bool operator==(const DateTime& dateTime) const;
		bool operator!=(const DateTime& dateTime) const;
		bool operator>(const DateTime& dateTime) const;
		bool operator<=(const DateTime& dateTime) const;
		bool operator>=(const DateTime& dateTime) const;
		std::string toString() const;
		std::string toStringDate(const std::string format ="dd.mm.yyyy") const;
		std::string toStringTime() const;
		bool fromString(const std::string& str);
		bool fromStringYYYYMMDD(const std::string& str);
	};

}
