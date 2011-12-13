//! @file mosh/cgi/http/helpers/helper.hpp interface for helpers/*
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

#ifndef MOSH_CGI_HTTP_HELPERS_HELPER_HPP
#define MOSH_CGI_HTTP_HELPERS_HELPER_HPP

#include <string>
#include <functional>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

//! An interface for header helpers
struct Helper {
	/*! @name Functors
	 */
	//@{
	std::function<std::string (unsigned)> do_u;
	std::function<std::string (std::string const&)> do_s;
	
	std::function<std::string (unsigned, std::string const&)> do_u_s;
	std::function<std::string (std::string const&, unsigned)> do_s_u;
	std::function<std::string (std::string const&, std::string const&)> do_s_s;
	//@}	
	Helper()
	{ }
};

}

MOSH_CGI_END

#endif

