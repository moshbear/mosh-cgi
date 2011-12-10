//! @file header_helper/redirect.cpp - HTTP redirection
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
#include <stdexcept>
#include <mosh/cgi/http/helpers/helper.hpp>
#include <mosh/cgi/http/helpers/redirect.hpp>
#include <mosh/cgi/http/helpers/status.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

namespace helpers {

//! HTTP redirect headers
namespace redirect {
	
/*! @brief Generate a redirection header
 *  @param[in] code HTTP status code (must be 3xx)
 *  @param[in] loc new location
 *  @return a @c status header for the given HTTP code,
 *  @return followed by a @c Location header referencing the new location
 *  @throw std::invalid_argument code is not 3xx
 *  @throw std::invalid_argument code is not valid 3xx
 *  @sa status
 */
std::string print_redir(unsigned code, const std::string& loc) {
	if ((code / 100) != 3) {
		throw std::invalid_argument("http_code must be 3xx (redirection-related)");
	}
	return status::print_status(code)
		+ "Location: " + loc + "\r\n";
}

//! Create a helper consisting of redirection generators
Helper helper() {
	Helper h;
	h.do_u_s = print_redir;
	return h;
}

} // redirect

} // helpers

} // http

MOSH_CGI_END

