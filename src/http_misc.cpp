//! @file http_misc.cpp Implementation for http/misc functions
/*!*************************************************************************
* Copyright (C) 2011 m0shbear                                              *
*                                                                          *
* This file is part of mosh-cgi.                                           *
*                                                                          *
* This library is free software: you can redistribute it and/or modify it  *
* under the terms of the GNU Lesser General Public License as  published   *
* by the Free Software Foundation, either version 3 of the License, or (at *
* your option) any later version.                                          *
*                                                                          *
* This library is distributed in the hope that it will be useful, but      *
* WITHOUT ANY WARRANTY; without even the implied warranty of               *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser *
* General Public License for more details.                                 *
*                                                                          *
* You should have received a copy of the GNU Lesser General Public License *
* along with mosh-cgi.  If not, see <http://www.gnu.org/licenses/>.       *
****************************************************************************/

#include <string>
#include <mosh/cgi/http/misc.hpp>
#include <mosh/fcgi/http/misc.hpp>
#include <mosh/cgi/bits/namespace.hpp>
#include <mosh/fcgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

//! Prints the current UTC time, in microsecond resolution to a string, with format fmt
std::string time_to_string(const std::string& fmt, unsigned long add_seconds) {
	return MOSH_FCGI::http::time_to_string(fmt, add_seconds);
}

}

MOSH_CGI_END

