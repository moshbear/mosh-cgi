//! @file mosh/cgi/http/helpers/response.hpp HTTP response
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

#ifndef MOSH_CGI_HTTP_HELPERS_RESPONSE_HPP
#define MOSH_CGI_HTTP_HELPERS_RESPONSE_HPP

#include <string>
#include <sstream>
#include <mosh/cgi/http/helpers/helper.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

//! Header helpers
namespace helpers {

//! HTTP response headers
namespace response {
	
/*! @brief Generate a HTTP response header
 *  @param[in] ver HTTP version
 *  @param[in] code HTTP status code
 *  @return HTTP/$ver $code $(status($code))
 *  @sa Status_helper
 */
std::string print_response(const std::string& ver, unsigned code);

//! Create a helper consisting of response generators
Helper helper();

}

}

}

MOSH_CGI_END

#endif

