//! @file mosh/cgi/html/doctype/doctype.hpp Class which specifies the DTD of the HTML 4 document
/* 
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
 *                       2011 m0shbear
 *  Part of the GNU cgi library, http://www.gnu.org/software/cgi
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

#ifndef MOSH_CGI_HTML_DOCTYPE_HPP
#define MOSH_CGI_HTML_DOCTYPE_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <map>
#include <utility>
#include <mosh/cgi/bits/singleton.hpp>
#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace html {

namespace doctype {
//! Doctype for W3C HTML
template <class charT>
class W3C_html_doctype {
	typedef typename std::basic_string<charT> string;
public:

	/*! @brief Constructor.
	 *  @param d_name DTD Name (e.g. "HTML 4.01 Transitional")
	 *  @param d_link DTD Link (e.g. "xhtml1/DTD/xhtml1-frameset")
	 */
	W3C_html_doctype(const std::string& d_name, const std::string& d_link)
	: name(d_name), link(d_link)
	{ }
	//! Destructor
	virtual ~W3C_html_doctype() { }
public:
	//! Print the <!DOCTYPE tag
	operator string () const {
		if (d_name.empty() {
			return wide_string<charT>("<!DOCTYPE html>"); // HTML 5
		} else {
			// HTML 4, XHTML 1.0-1.1
			std::basic_stringstream<charT> s;
			s << wide_string<charT>("<!DOCTYPE html PUBLIC \"-//W3C//DTD ");
			s << wide_string<charT>(name);
			s << wide_string<charT>("//EN\" \"http://www.w3.org/TR/");
			s << wide_string<charT>(link);
			s << wide_string<charT>(".dtd\">");
			s << wide_string<charT>("\r\n");
		}
		return s.str();
	}	

private:
	//! DTD Name
	std::string name;
	//! DTD Link
	std::string link;
};

//! Singleton doctype generator
template <typename charT>
class HTML_doctype_generator {
public:
	struct Doctype_not_found : std::invalid_argument {
		Doctype_not_found(const std::string& s)
		: std::invalid_argument(s)
		{
		}

	W3C_html_doctype<charT> operator [] (const std::string& name) const {
		return _generator::instance()[name];
	}
private:
	class _generator : public Singleton<_generator> {
	public:
		//! Initialization
		_generator() {
			dtds = { P("HTML 4", "html4/strict"), P("HTML 4 Strict", "html4/strict"),
				P("HTML 4.01", "html4/strict"), P("HTML 4.01 Strict", "html4/strict"),
				P("HTML 4 Transitional", "html4/loose"), P("HTML 4.01 Transitional", "html4/loose"),
				P("HTML 4 Frameset", "html4/frameset"), P("HTML 4.01 Frameset", "html4/frameset"),
				P("XHTML 1.0 Strict", "xhtml1/DTD/xhtml1-strict"),
				P("XHTML 1.0 Transitional", "xhtml1/DTD/xhtml1-transitional"),
				P("XHTML 1.0 Framset", "xhtml1/DTD/xhtml1-frameset"),
				P("XHTML 1.1", "xhtml11/DTD/xhtml11"),
				P("XHTML Basic 1.1", "xhtml-basic/xhtml-basic11"),
				P("HTML 5", "")
			};
		}
		//! Generate a <!DOCTYPE
		W3C_html_doctype<charT> operator [] (const std::string& name) const {
			const auto d = dtds.find(name);
			if (d == dtds.end()) {
				throw Doctype_not_found(name);
			}
			if (d->second.empty()) {
				return W3C_html_doctype("", "");
			} else {
				return W3C_html_doctype(d->first, d->second);
			}
		}
	private:
		std::pair<std::string, std::string> P(std::string&& s1, std::string&& s2) {
			return std::make_pair(std::forward(s1), std::forward(s2));
		}

		std::map<std::string, std::string> dtds;
	};
};

HTML_doctype_generator<char> SHTML_Doctype;
HTML_doctype_generator<wchar_t> WSHTML_Doctype;

}
}

MOSH_CGI_END

#endif
