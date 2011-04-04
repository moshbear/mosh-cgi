/* cgi/utils/string/CaseInsensitiveComparator.h - A binary predicate functor for templated case-insensitive
 * 			string comparisons
 *
 * Copyright (C) 2011, Andrey Vul <andrey@moshbear.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 */


#ifndef _CGI_UTILS_STRING_CASEINSENSITIVECOMPARATOR_H_
#define _CGI_UTILS_STRING_CASEINSENSITIVECOMPARATOR_H_

#include <cctype>
#include <cwctype>
#include <functional>

namespace cgi { namespace string {

template <class charT>
struct CaseInsensitiveComparator : public std::binary_function<charT, charT, bool> {
	bool operator()(const charT& c1, const charT& c2);
};
// hopefully, for the following specializations, the compiler will be smart enough
// to inline the std::tow?lower and reduce the overhead of object creation and function call

// functor specialization for char strings
template<> inline bool CaseInsensitiveComparator<char>::operator()(const char& c1, const char& c2)
{ return std::tolower(c1) == std::tolower(c2); }
// functor specialization for wchar_t strings
template<> inline bool CaseInsensitiveComparator<wchar_t>::operator()(const wchar_t& wc1, const wchar_t& wc2)
{ return std::towlower(wc1) == std::towlower(wc2); }

} }

#endif
