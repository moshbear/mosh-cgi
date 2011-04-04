/* cgi/common/Utils.h - misc string and helper functions
 *
 *  Copyright (C) 1996 - 2007 GNU Cgicc team
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

#ifndef _CGI_COMMON_UTILS_H_
#define _CGI_COMMON_UTILS_H_ 1

/*! \file Utils.h
 * \brief A collection of utility functions.
 *
 * These utility functions are used internally by cgi to
 * decode posted form data, and to read/write from streams.
 */

#include <new>
#include <string>
#include <iostream>
#include <algorithm>
#include <util/Strcasecmp.h>
#include <cgi/utils/Unicode.h>

namespace cgi { namespace form { namespace url {

char hexUnescape(char first, char second);

template <class InputIterator>
std::string decode(InputIterator _Begin, InputIterator _End) {
	std::string result;
	while (_Begin != _End) {
		switch (*_Begin) {
			case '+':
				result += ' ';
				break;
			case '%':
				// don't assume well-formed input
				if (std::distance(_Begin, _End) > 2 && std::isxdigit(*(_Begin+1)) && std::isxdigit(*(_Begin+2))) {
					char ch = *++_Begin;
					result += hexUnescape(ch, *++_Begin);
				} else // just pass % through untouched
					result += '%';
				break;
			default:
				result += *_Begin;
				break;
		}
		_Begin++;
	}
	return result;
}

inline std::string decode(const char* bytes, size_t len) { return decode(bytes, bytes + len); }
/*!
 * \brief Convert encoded characters in form data to normal ASCII.
 *
 * For example, "%21" is converted to '!' and '+' is converted to a space.
 * Normally, this is called internally to decode the query string or post
 * data.
 * \tparam charT output character type. If \c wchar_t, then utfWiden<wchar_t> is called to decode the
 * 		raw UTF-8 output.
 * 		If \c char, then Unicode overhead is avoided.
 * \param src The src string containing the encoded characters
 * \return The converted string
 */
template <class charT>
std::basic_string<charT> decode(const std::string& src);
template<> inline std::string decode<char>(const std::string& src) { return decode(src.begin(), src.end()); }
template<> inline std::wstring decode<wchar_t>(const std::string& src) { return unicode::utfIn<wchar_t>(decode<char>(src)); }
/*!
 * \brief Convert an ASCII string to a URL-safe string.
 *
 * For example, '!' is converted to "%21" and ' ' is converted to '+'.
 * \tparam charT (deduced from input)
 * \param src The src string containing the characters to encode
 * \return The converted string
 */
template <class charT>
std::string encode(const std::basic_string<charT>& src);
template<> std::string encode(const std::string& src);
template<> inline std::string encode(const std::wstring& src) { return encode(unicode::utfOut(src)); }

} } } // namespace url // namespace form // namespace cgi

#endif				/* ! _CGI_COMMON_UTILS_H_ */
