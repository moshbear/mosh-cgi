/* cgi/http/ContentHeader.h - HTTP content header class
 * 				
 * Copyright (C) 1996-2007 GNU Cgicc team
 * 	     (C) 2011 Andrey Vul
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


#ifndef _CGI_HTTP_CONTENTHEADER_H_
#define _CGI_HTTP_CONTENTHEADER_H_ 1

#include <ostream>
#include <util/STL_Foreach.h>
#include <cgi/http/Header.h>

namespace cgi { namespace http {

template <class charT>
class ContentHeader : public Header<charT> {
	// for future use, these typedefs may come in very handy
	typedef Header<charT> base_type;
	typedef ContentHeader<charT> this_type;
public:
	// public to allow typename ContentHeader<T>::string_type usage
	typedef typename base_type::string_type string_type; 

	ContentHeader(const string_type& mimeType)
	: base_type(mimeType) {
	}

	virtual ~ContentHeader() {
	}
	virtual void render(typename base_type::ostream_type& out) const {
		out << "Content-Type: " << this->data << std::endl;
		FOREACH(const auto& iter, this->cookies, out << iter.second << std::endl;)
		out << std::endl;
	}
private:
	ContentHeader();
};
} } // namespace http // namespace cgi
#endif				/* ! _CGI_HTTP_CONTENTHEADER_H_ */
