/* cgi/utils/String.h - string functions
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

#ifndef _CGI_UTILS_STRING_H_
#define _CGI_UTILS_STRING_H_ 1

/*! \file String.h
 * \brief String utility functions
 *
 * String utilities used internally by the cgi library.
 */

#include <new>
#include <string>
#include <iostream>
#include <algorithm>
#include <cgi/utils/Unicode.h>
#include <cgi/utils/string/CaseInsensitiveComparator.h>

namespace cgi { namespace string {

/*!
 * \brief Compare two strings for equality, ignoring case.
 *
 * For case-sensitive comparison, use (s1 == s2);
 * \tparam charT (deduced from input)
 * \param s1 The first string to compare
 * \param s2 The second string to compare
 * \return \c true if the strings are equal, \c false if they are not
 */
template <class charT>
bool equality(const std::basic_string<charT>& s1, const std::basic_string<charT>& s2) {
	return std::equal(s1.begin(), s1.end(), s2.begin(), CaseInsensitiveComparator<charT>());
}
/*!
 * \brief Compare two strings for equality, ignoring case.
 *
 * For case-sensitive comparison, use (s1 == s2);
 * \tparam charT (deduced from input)
 * \param s1 The first string to compare
 * \param s2 The second string to compare
 * \param n The number of characters to compare.
 * \return \c true if the strings are equal, \c false if they are not
 */
template <class charT>
bool equality(const std::basic_string<charT>& ss1, const std::basic_string<charT>& ss2, size_t n)
{
	return std::equal(ss1.begin(), ss1.begin() + n, ss2.begin(), CaseInsensitiveComparator<charT>());
}

template <class charT, class InputIterator>
std::basic_string<charT> extractBetween(InputIterator _Begin, InputIterator _End, const std::string& sep1, const std::string& sep2);
template <class InputIterator> std::string extractBetween<char, InputIterator>(InputIterator _Begin, InputIterator _End, const std::string& sep1, const std::string& sep2) {
	std::string result;
	InputIterator _Data_Start = std::search
template <class InputIterator> inline std::wstring extractBetween<wchar_t,InputIterator>(InputIterator _Begin, InputIterator _End, const std::string sep1, const std::string& sep2) {
	return unicode::utfIn<wchar_t>(extractBetween<char>(_Begin, _End, sep1, sep2));
}

/*!
 * \brief Extract a substring contained within two separators.
 *
 * For example, after the call
 * \code
 * std::string data = "11foo22";
 * std::string res;
 * res = extractBetween(data, "11", "22");
 * \endcode
 * \c res will be "foo".
 * \tparam charT character type. If \c wchar_t, then the input is converted to Unicode.
 * \param data The data to search.
 * \param separator1 The first logical separator.
 * \param separator2 The second logical separator.
 * \return The substring between the separators.
 */
template <class charT>
std::basic_string<charT> extractBetween(const std::string& data, const std::string& separator1, const std::string& separator2);
template<> std::string extractBetween<char>(const std::string&, const std::string&, const std::string&);
template<> inline std::wstring extractBetween<wchar_t>(const std::string& data, const std::string& sep1, const std::string& sep2) {
	return unicode::utfIn<wchar_t>(extractBetween<char>(data, sep1, sep2));
}

/*!
 * \brief Extract a substring contained between a separator.
 *
 * This function is used internally to decode \c multipart/form-data
 * \tparam charT character type. If \c wchar_t, then the input is converted to Unicode.
 * \param data The data to search.
 * \param separator The separator.
 * \return The substring between the separator.
 */
template <class charT>
inline std::basic_string<charT> extractBetween(const std::string& datas, const std::string& separators)
{
	return extractBetween<charT>(datas, separators, separators);
}

} } // namespace string // namespace cgi

#endif				/* ! _CGI_UTILS_STRING_H_ */
