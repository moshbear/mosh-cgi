//! @file mosh/cgi/http/helpers/status_helper.hpp String lookup for HTTP status codes
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

#ifndef MOSH_CGI_HTTP_HELPERS_STATUS_HELPER_HPP
#define MOSH_CGI_HTTP_HELPERS_STATUS_HELPER_HPP

#include <string>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

//! Header helpers
namespace helpers {

//! HTTP status lookup
namespace status_helper {
	
/*! @brief Get the corresponding status string for a given code
 *  @param[in] st HTTP status code
 *  @return the corresponding status string
 *  @throw std::invalid_argument if the status code is invalid
 */
string get_string (unsigned st);

}

}

}

MOSH_CGI_END

#endif

