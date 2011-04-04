/* cgi/http/Header.h - ABC for HTTP headers
 *                              
 * Copyright (C) 1996-2007 GNU Cgicc team
 *           (C) 2011 Andrey Vul
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


#ifndef _CGI_HTTP_HEADER_H_
#define _CGI_HTTP_HEADER_H_ 1

#include <map>
#include <utility>

#include <cgi/common/Streamable.h>
#include <cgi/http/Cookie.h>

namespace cgi { namespace http {
template <class charT>
class Header : public cgi::common::Streamable<charT> {
	typedef cgi::common::Streamable<charT> base_type;
	typedef Header<charT> this_type;
public:
	typedef typename base_type::string_type string_type;
	typedef Cookie<charT> cookie_type;
	typedef typename std::pair<string_type, cookie_type> cookie_entry_type;
	string_type data;
	typename std::multimap<string_type, cookie_type> cookies;
	Header(const string_type& data_)
	: data(data_) {
	}
	Header(const this_type& header)
	: base_type(), data(header.data), cookies(header.cookies) {
	}

	virtual ~Header() {
	}
	
	virtual void render(std::basic_ostream<charT>& out) const = 0;

private:
	Header();

};

} } // namespace http // namespace cgi

#endif				/* ! _CGI_HTTP_HEADER_H_ */
