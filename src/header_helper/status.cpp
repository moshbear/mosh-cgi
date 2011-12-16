//! @file header_helpers/status.cpp - HTTP status codes
/* 
 *  Copyright (C) 2011 m0shbear
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
#include <sstream>
#include <mosh/cgi/http/helpers/helper.hpp>
#include <mosh/cgi/http/helpers/status_helper.hpp>
#include <mosh/cgi/http/helpers/status.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

namespace helpers {

//! HTTP status headers
namespace status {
	
/*! @brief Generate a HTTP @c status header
 *  @param[in] st HTTP status code
 *  @return status header with corresponding code
 *  @sa status_helper
 */
std::string print_status (unsigned st) {
	std::stringstream ss;
	ss << "Status: ";
	ss << st;
	ss << " " + status_helper::get_string(st);
	ss << "\r\n";
	return ss.str();
}
	
//! Create a helper consisting of status line generators 
Helper helper() {
	Helper h;
	h.do_u = print_status;
	return h;
}

} // status

} // helpers

} // http

MOSH_CGI_END


