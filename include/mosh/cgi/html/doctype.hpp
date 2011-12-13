//! @file mosh/cgi/html/doctype/doctype.hpp Class which specifies the DTD of the HTML 4 document
/* 
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
 *                       2011 m0shbear
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

//! HTML classes
namespace html {

//! HTML Doctype classes
namespace doctype {
//! Doctype for W3C HTML
template <class charT>
class W3C_html_doctype {
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
	operator std::basic_string<charT> () const {
		if (name.empty()) {
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
			return s.str();
		}
	}
	//! Print the <!DOCTYPE tag
	operator const charT* () const {
		return this->template operator std::basic_string<charT>().c_str();
	}

private:
	//! DTD Name
	std::string name;
	//! DTD Link
	std::string link;
};

//! HTML revision
namespace HTML_revision {
	
	/* Guide to constants:
	 * flags:4 major:4 minor:4
	 * If major has no alternate versions, then minor = 0.
	 * Otherwise, main version has minor 1 and all alternates have increasing minor.
	 * For XHTML, flags[0] is set.
	 */

	//! HTML 4.01 (Strict)
	const unsigned html_4 = 0x11;
	//! HTML 4.01 Strict
	const unsigned html_4_strict = 0x11;
	//! HTML 4.01 Frameset
	const unsigned html_4_frameset = 0x12;
	//! HTML 4.01 Transitional
	const unsigned html_4_transitional = 0x13;
	//! XHTML 1.0 (Strict)
	const unsigned xhtml_10 = 0x121;
	//! XHTML 1.0 Strict
	const unsigned xhtml_10_strict = 0x121;
	//! XHTML 1.0 Frameset
	const unsigned xhtml_10_frameset = 0x122;
	//! XHTML 1.0 Transitional
	const unsigned xhtml_10_transitional = 0x123;
	//! XHTML 1.1
	const unsigned xhtml_11 = 0x130;
	//! XHTML Basic 1.1
	const unsigned xhtml_basic_11 = 0x140;
	//! HTML 5
	const unsigned html_5 = 0x50;

	inline void _validate(unsigned hr) {
		unsigned flg = (hr & 0x0F00) >> 8;
		unsigned maj = (hr & 0xF0) >> 4;
		unsigned min = (hr & 0x0F);
		switch (maj) {
		case 1: // HTML 4
			if (flg != 0 || min < 1 || min > 3)
				goto _throw_;
			return;
		case 2: // XHTML 1.0
			if (flg != 1 || min < 1 || min > 3)
				goto _throw_;
			return;
		case 3: // XHTML 1.1
		case 4: // XHTML Basic 1.1
			if (flg != 1 || min != 0)
				goto _throw_;
			return;
		case 5: // HTML 4
			if (flg != 0 || min != 0)
				goto _throw_;
			return;
		default:;
		}
	_throw_:
		throw std::invalid_argument("MOSH_CGI::html::doctype::HTML_revision");
	}
}

//! Singleton doctype generator
template <typename charT>
class HTML_doctype_generator
{
public:
	W3C_html_doctype<charT> operator () (unsigned hr) const {

		return _generator::instance()(hr);
	}
private:
	class _generator : public Singleton<_generator> {
	public:
		//! Initialization
		_generator() {

			dtds = { 
				{HTML_revision::html_4,
					{"HTML 4.01", "html4/strict"}},
				{HTML_revision::html_4_transitional,
					{"HTML 4.01 Transitional", "html4/loose"}},
				{HTML_revision::html_4_frameset,
					{"HTML 4.01 Frameset", "html4/frameset"}},
				{HTML_revision::xhtml_10,
					{"XHTML 1.0 Strict", "xhtml1/DTD/xhtml1-strict"}},
				{HTML_revision::xhtml_10_transitional,
					{"XHTML 1.0 Transitional", "xhtml1/DTD/xhtml1-transitional"}},
				{HTML_revision::xhtml_10_frameset,
					{"XHTML 1.0 Framset", "xhtml1/DTD/xhtml1-frameset"}},
				{HTML_revision::xhtml_11,
					{"XHTML 1.1", "xhtml11/DTD/xhtml11"}},
				{HTML_revision::xhtml_basic_11,
					{"XHTML Basic 1.1", "xhtml-basic/xhtml-basic11"}},
				{HTML_revision::html_5,
					{"", ""}}
			};
		}
		//! Generate a <!DOCTYPE
		W3C_html_doctype<charT> operator () (unsigned hr) const {
			HTML_revision::_validate(hr);
			const auto d = dtds.find(hr);
			if (d == dtds.end()) {
				throw std::logic_error("unmapped doctype");
			}
			auto& _d = d->second;
			return W3C_html_doctype<charT>(_d.first, _d.second);
		}
	private:
		std::map<unsigned, std::pair<std::string, std::string>> dtds;
	};
};

HTML_doctype_generator<char> SHTML_Doctype;
HTML_doctype_generator<wchar_t> WSHTML_Doctype;

}
}

MOSH_CGI_END

#endif
