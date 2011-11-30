//! @file mosh/cgi/bits/print.hpp Template class for printing objects with defined operator<<(std::basic_ostream&)
//
/*  (C) 2011 m0shbear
 *
 *  Part of mosh-cgi.
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

#ifndef MOSH_CGI_COMMON_PRINT_HPP
#define MOSH_CGI_COMMON_PRINT_HPP

#include <iostream>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

template <typename LShiftPrintable, typename charT>
std::basic_ostream<charT>& print(std::basic_ostream<charT>& os,
				const LShiftPrintable& t)
{
	os << t;
	return os;
}

MOSH_CGI_END

#endif
