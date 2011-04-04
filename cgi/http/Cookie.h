/* cgi/http/Cookie.h - HTTP Cookie claszs
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


#ifndef _CGI_HTTP_COOKIE_H_
#define _CGI_HTTP_COOKIE_H_ 1

#include <string>
#include <sstream>
#include <locale>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/time_facet.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <cgi/common/Utils.h>
#include <cgi/common/Streamable.h>
#include <cgi/common/Kv.h>

namespace cgi { namespace http {
template <class charT>
class Cookie : public virtual cgi::common::KV<charT>,
		public virtual cgi::common::Streamable<charT>
{
	typedef cgi::common::KV<charT> base_KV;
	typedef cgi::common::Streamable<charT> base_Streamable;
	typedef Cookie<charT> this_type;
	typedef typename base_Streamable::string_type string_type;
public:
	Cookie() : Cookie("", "") {
	}
	Cookie(const string_type& name_, const string_type& value_)
	: Cookie(name_, value_, "" /* comment */, "" /* domain */, 0 /* maxAge */, "" /* path */) {
	}

	Cookie(const string_type& name_, const string_type& value_, const string_type& comment_,
	           const string_type& domain_, unsigned long maxAge_, const string_type& path_, bool secure_ = false)
	: base_Streamable(), base_KV(name_, value_), comment(comment_), domain(domain_), maxAge(maxAge_), path(path_),
			secure(secure_), removed(false) {
	}

	Cookie(const this_type& cookie)
	: Cookie(cookie.name, cookie.value, cookie.comment, cookie.domain, cookie.maxAge, cookie.path,
			cookie.secure) {
		removed = cookie.removed;
	}
	virtual ~Cookie() {
	}
	bool operator==(const this_type& cookie) const {using namespace cgi::common;\
		return (this == &cookie)||
			(this->base_KV::operator==(cookie)
			&& stringsAreEqual(comment, cookie.comment)
			&& stringsAreEqual(domain, cookie.domain)
			&& maxAge == cookie.maxAge
			&& stringsAreEqual(path, cookie.path)
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
		out << "Set-Cookie:";
		this->base_KV::render(out);
		if (!comment.empty())
			out << "; Comment=" << comment;
		if (!domain.empty())
			out << "; Domain=" << domain;
		if (removed) {
			out << "; Expires=Fri, 01-Jan-1971 01:00:00 GMT;";
			out << "; Max-Age=0";
		} else if (maxAge) {
			out << "; Max-Age=" << maxAge;
			out << "; Expires=" << timeToString();
		}
		if (!path.empty())
			out << "; Path=" << path;
		if (secure)
			out << "; Secure";
		
		out << "; Version=1";
	}


	string_type comment;
	string_type domain;
	unsigned long maxAge;
	string_type path;
	bool secure;
	bool removed;
protected:
	// Returns current time + maxAge, in UTC form
	string_type timeToString() const {
		using namespace boost::posix_time;
		namespace dt = boost::date_time;
		using namespace boost::gregorian;
		ptime now(second_clock::universal_time());
		now += seconds(maxAge);
		const std::basic_string<charT> fmt(boost::lexical_cast<std::basic_string<charT> >("%a, %d-%b-%Y %H:%M:%S GMT"));
		boost::scoped_ptr<dt::time_facet<ptime, charT> > tf(new dt::time_facet<ptime, charT>(fmt.c_str()));
		std::locale loc(std::locale::classic(), tf.get());
		std::basic_stringstream<charT> str;
		str.imbue(loc);
		str << now;
		std::basic_string<charT> ret = str.str();
		return ret;
	}
};
} } // namespace http // namespace cgi

#endif				/* ! _CGI_HTTP_COOKIE_H */
