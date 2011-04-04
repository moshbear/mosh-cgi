/* cgi/common/StringUtils.cpp - common string functions
 *
 *  Copyright (C) 1996-2007 GNU Cgicc team
 *                2011 Andrey Vul
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 */

#include <string>
#include <cgi/utils/String.h>


template<>
std::string cgi::string::extractBetween<char>(const std::string& data, const std::string& separator1, const std::string& separator2) {
	std::string result;
	size_t start;
	if ((start = data.find(separator1)) != std::string::npos) {
		start += separator1.length();
		size_t limit;
		if ((limit = data.find(separator2, start)) != std::string::npos)
			result = data.substr(start, limit - start);
	}
		return result;
}



