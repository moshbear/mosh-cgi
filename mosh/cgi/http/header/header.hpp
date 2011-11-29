//! @file mosh/cgi/http/header/header.hpp ABC for HTTP headers
/*
 * Copyright (C) 1996-2007 GNU Cgicc team
 *           (C) 2011 m0shbear
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


#ifndef MOSH_CGI_HTTP_HEADER_HPP
#define MOSH_CGI_HTTP_HEADER_HPP

#include <map>
#include <utility>
#include <vector>

#include <mosh/cgi/bits/streamable.hpp>
#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/http/cookie.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

template <class charT>
class Header : public Streamable<charT> {
	typedef Streamable<charT> base_type;
	typedef Header<charT> this_type;
public:
	typedef typename base_type::string_type string_type;
	typedef Cookie<charT> cookie_type;
	string_type data;
	std::map<string_type, std::vector<cookie_type>> cookies;

	Header(const string_type& data_)
	: data(data_) {
	}

	Header(const this_type& header)
	: base_type(), data(header.data), cookies(header.cookies) {
	}

	virtual ~Header() {
	}
	
	virtual void render(std::basic_ostream<charT>& out) const {
		for (const auto& cookie_k : this->cookies) {
			for (const auto& cookie_v : cookie_k.second) {
				out << cookie_v << std::endl;
			}
		}
		out << std::endl;
	}


private:
	Header();

};

}

MOSH_CGI_END

#endif
