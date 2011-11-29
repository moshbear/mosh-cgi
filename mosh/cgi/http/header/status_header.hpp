//! @file mosh/cgi/http/header/status_header.hpp Header to set a request status
/* 				
 * Copyright (C) 1996-2007 GNU Cgicc team
 * 	     (C) 2011 m0shbear
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

#ifndef MOSH_CGI_HTTP_STATUSHEADER_HPP
#define MOSH_CGI_HTTP_STATUSHEADER_HPP

#include <string>
#include <ostream>
#include <stdexcept>

#include <mosh/cgi/http/header/header.hpp>
#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN 

namespace http {

//! Header to set a request status
template <class charT>
class Status_header : public Header<charT> {
	typedef Header<charT> base_type;
public:
	typedef typename base_type::string_type string_type;
	
	// ! Create a HTTP status header with a given 3-digit status and  message
	Status_header(int status_, const string_type& message)
	: base_type(message), status(status_) {
		if (status < 100 || status > 599)
			throw std::invalid_argument("Status out of range");
	}
	//! Destructor
	virtual ~Status_header() { }
	
	virtual void render(std::basic_ostream<charT>& out) const {
		out << T_STRING(charT, "Status: ") << status << ' ' << this->data << std::endl;
		Header::render(out);
	}

	/*! @brief Set the 3-digit HTTP status code
	 * @throw std::invalid_argument if validation fails (condition 100 <= new_status <= 599 not met)
	 */
	void set_status(int new_status) {
		if (new_status < 100 || new_status > 599)
			throw std::invalid_argument("Status out of range");
		status = new_status;
	}
	//! Get the 3-digit HTTP status code
	int get_status() const {
		return status;
	}
		
private:
	int status;
	Status_header();
		
};

}

MOSH_CGI_END

#endif
