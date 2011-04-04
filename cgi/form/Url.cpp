/* cgi/common/Utils.cpp - implementations for non-inline template functions in Utils.hpp
 *
 *  Copyright (C) 1996-2007 GNU Cgicc team
 *                2011 Andrey Vul
 *  Portions Copyright (C) 2007 Eddie
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

#include <cctype>
#include <util/STL_Foreach.h>

#include <cgi/form/Url.h>

namespace {
std::string hexEscape(char c)
{
	std::string result;
	char first, second;

	first = (c &  0xF0) / 16;
	first += first > 9 ? 'A' - 10 : '0';
	second = c &  0x0F;
	second += second > 9 ? 'A' - 10 : '0';

	result += first;
	result += second;

	return result;
}
}

char cgi::form::url::hexUnescape(char first, char second)
{
	int digit;

	digit = (first >= 'A' ? ((first &  0xDF) - 'A') + 10 : (first - '0'));
	digit *= 16;
	digit += (second >= 'A' ? ((second &  0xDF) - 'A') + 10 : (second - '0'));
	return static_cast<char>(digit);
}

/*
   From the HTML standard:
   <http://www.w3.org/TR/html4/interact/forms.html#h-17.13.4.1>

   application/x-www-form-urlencoded

   This is the default content type. Forms submitted with this content
   type must be encoded as follows:

   1. Control names and values are escaped. Space characters are
   replaced by `+', and then reserved characters are escaped as
   described in [RFC1738], section 2.2: Non-alphanumeric characters
   are replaced by `%HH', a percent sign and two hexadecimal digits
   representing the ASCII code of the character. Line breaks are
   represented as "CR LF" pairs (i.e., `%0D%0A').
   2. The control names/values are listed in the order they appear in
   the document. The name is separated from the value by `=' and
   name/value pairs are separated from each other by `&'.

   Note RFC 1738 is obsoleted by RFC 2396.  Basically it says to
   escape out the reserved characters in the standard %xx format.  It
   also says this about the query string:

   query         = *uric
   uric          = reserved | unreserved | escaped
   reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
   "$" | ","
   unreserved    = alphanum | mark
   mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" |
   "(" | ")"
   escaped = "%" hex hex */

template<> std::string cgi::form::url::encode<char>(const std::string& src)
{
	std::string result;
	const std::string mark("-_.!~*'()");
	FOREACH(const char& iter, src,
	{
		if (iter >= 0x7f) {
			result += '%' + hexEscape(iter);
		} else if (iter == ' ') {
			result += '+';
		} else if (std::isalnum(iter) || mark.find(iter) != std::string::npos) {
			result += iter;
		} else {
			result += '%' + hexEscape(iter);
			break;
		}
	})
	return result;
}




