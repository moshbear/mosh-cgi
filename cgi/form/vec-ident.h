/* cgi/form/vec-ident.h: identity functions for various vector classes
 * Copyright (C) 2011 Andrey m0shbear
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

#ifndef _CGI_FORM_FILE_H_
#define _CGI_FORM_FILE_H_ 1

namespace std { template <class T, typename... _> class vector; }
namespace stxxl { template <class T, typename... _> class vector; }

namespace cgi { namespace form {
template <typename vc>
struct ident {
	static bool has_bulk_iterator();
};

template <typename... _> struct ident<stxxl::vector<char, _> > {
	static bool has_bulk_iterator() {
		return false; 
	}
};

template <typename... _> struct ident<std::vector<char, _> > {
	static bool has_bulk_iterator() {
		return true;
	}
};

} } // namespace form // namespace cgi

#endif
