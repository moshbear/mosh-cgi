//! @file mosh/cgi/http/helpers/content_type.hpp Content-Type header
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

#ifndef MOSH_CGI_HTTP_HELPERS_CONTENT_TYPE_HPP
#define MOSH_CGI_HTTP_HELPERS_CONTENT_TYPE_HPP

#include <string>
#include <mosh/cgi/http/helpers/helper.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

//! Header helpers
namespace helpers {

//! Content-Type headers
namespace content_type {

/*! @brief Generate Content-Type header
 *  @param[in] ctype content type
 *  @return Content-Type: $ctype
 */
std::string print_ct(const std::string& ctype);

/*! @brief Generate Content-Type header
 *  @param[in] ctype content type
 *  @param[in] cset charset
 *  @return Content-Type: $ctype; $($cset ? charset=$cset : )
 */
std::string print_ct_with_cs(const std::string& ctype, const std::string& cset);
	
//! Create a helper consisting of Content-Type generators
Helper helper();

}

}

}

MOSH_CGI_END

#endif

