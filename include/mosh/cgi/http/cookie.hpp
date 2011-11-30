//! @file mosh/cgi/http/cookie.hpp HTTP Cookie class
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


#ifndef MOSH_CGI_HTTP_COOKIE_HPP
#define MOSH_CGI_HTTP_COOKIE_HPP

#include <string>
#include <sstream>
#include <locale>
#include <boost/lexical_cast.hpp>
#include <mosh/cgi/http/_/misc.hpp>
#include <mosh/cgi/streamable.hpp>
#include <mosh/cgi/bits/ci_strcomp.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

template <class charT>
class Cookie : public Streamable<charT>
{
	typedef Streamable<charT> base_Streamable;
	typedef Cookie<charT> this_type;
	typedef typename base_Streamable::string_type string_type;
public:
	Cookie() : Cookie("", "") {
	}
	Cookie(const string_type& name_, const string_type& value_)
	: Cookie(name_, value_, "" /* comment */, "" /* domain */, 0 /* max_age */, "" /* path */) {
	}

	Cookie(const string_type& name_, const string_type& value_, const string_type& comment_,
	           const string_type& domain_, unsigned long max_age_, const string_type& path_, bool secure_ = false)
	: base_Streamable(), name(name_), value(value_), comment(comment_), domain(domain_), max_age(max_age_), path(path_),
			secure(secure_), removed(false) {
	}

	Cookie(const this_type& cookie)
	: Cookie(cookie.name, cookie.value, cookie.comment, cookie.domain, cookie.max_age, cookie.path,
			cookie.secure) {
		removed = cookie.removed;
	}
	virtual ~Cookie() {
	}
	bool operator==(const this_type& cookie) const {
		return (this == &cookie)||
			(ci_equality(name, cookie.name)
			&& ci_equality(value, cookie.value)
			&& ci_equality(comment, cookie.comment)
			&& ci_equality(domain, cookie.domain)
			&& max_age == cookie.max_age
			&& ci_equality(path, cookie.path)
			&& secure == cookie.secure
			&& removed == cookie.removed);
	}
	bool operator !=(const this_type& cookie)const {
		return !operator==(cookie);
	}

	void remove() {
		removed = true;
	}
	
	this_type& markSecurity(const bool sec = false) {
		secure = sec;
		return *this;
	}

	static Cookie<charT> deleted(const string_type& name_, const string_type& domain_, 
					const string_type& path_, bool secure_)
	{
		Cookie<charT> c(name_, "" /* value */, "" /* comment */,  domain_, 0, path_, secure_);
		c.removed = true;
		return c;
	}

	virtual void render(typename base_Streamable::ostream_type& out) const {
		out << T_STRING(charT, "Set-Cookie:");
		out << name;
		out << '=' << '"';
		out << value;
		out << '"';
		if (!comment.empty())
			out << T_STRING(charT, "; Comment=") << comment;
		if (!domain.empty())
			out << T_STRING(charT, "; Domain=") << domain;
		if (removed) {
			out << T_STRING(charT, "; Expires=Fri, 01-Jan-1971 01:00:00 GMT");
			out << T_STRING(charT, "; Max-Age=0");
		} else if (max_age) {
			out << T_STRING(charT, "; Max-Age=") << max_age;
			out << T_STRING(charT, "; Expires=") << time_to_string();
		}
		if (!path.empty())
			out << T_STRING(charT, "; Path=") << path;
		if (secure)
			out << T_STRING(charT, "; Secure");
		
		out << T_STRING(charT, "; Version=1");
	}

	string_type comment;
	string_type domain;
	unsigned long max_age;
	string_type path;
	bool secure;
	bool removed;
protected:
	// Returns current time + max_age, in UTC form
	string_type time_to_string() const {
		return T_STRING(charT, time_to_string("%a, %d-%b-%Y %H:%M:%S GMT", max_age));
	}
};

MOSH_CGI_END

#endif
