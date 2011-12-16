//! @file mosh/cgi/http/header.hpp HTTP header
/*
 * Copyright (C) 2011 m0shbear
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

#include <initializer_list>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <mosh/cgi/http/cookie.hpp>
#include <mosh/cgi/http/helpers/helper.hpp>
#include <mosh/cgi/http/helpers/content_type.hpp>
#include <mosh/cgi/http/helpers/redirect.hpp>
#include <mosh/cgi/http/helpers/response.hpp>
#include <mosh/cgi/http/helpers/status.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

namespace header {

class Header {
public:
	std::map<std::string, std::vector<Cookie>> cookies;

	//! Default constructor
	Header()
	{ }

	//! Helper constructor
	Header(const Helper& h)
	: helper(h)
	{ }

	//! Copy constructor
	Header(const Header& h)
	: helper(h.helper), data(h.data)
	{ }

	//! Move constructor
	Header(Header&& h)
	: helper(h.helper), data(std::move(h.data))
	{ }

	virtual ~Header()
	{ }

	/*! @name Function-call overloads
	 * These overloads are wrappers for the functions described in @c Helper
	 * @sa Helper
	 */
	//@{
	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_u.
	 * If well-defined, the result of do_u is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] u argument to be passed to do_u
	 * @returns @c *this
	 * @throws std::invalid_argument if helper.do_u is null, that is, undefined
	 */
	Header& operator () (unsigned u) {
		if (helper.do_u) {
			data += helper.do_u(u);
			check_crlf();
			return *this;
		} else
			throw std::invalid_argument("operator()(unsigned) is undefined");
	}
	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_u.
	 * If well-defined, the result of do_u is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] u argument to be passed to do_u
	 * @returns a copy of @c *this with the function's return value appended to it
	 * @throws std::invalid_argument if helper.do_u is null, that is, undefined
	 */
	Header operator () (unsigned u) const {
		Header h(*this);
		if (helper.do_u) {
			h.data += helper.do_u(u);
			h.check_crlf();
			return h;
		} else
			throw std::invalid_argument("operator()(unsigned) is undefined");
	}

	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_u.
	 * If well-defined, the result of do_u is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] s string argument
	 * @returns @c *this
	 * @throws std::invalid_argument if helper.do_u is null, that is, undefined
	 */
	Header& operator () (const std::string& s) {
		if (helper.do_s) {
			data += helper.do_s(s);
			check_crlf();
			return *this;
		} else
			throw std::invalid_argument("operator()(string) is undefined");
	}
	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_u.
	 * If well-defined, the result of do_u is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] s string argument
	 * @returns a copy of @c *this with the function's return value appended to it
	 * @throws std::invalid_argument if helper.do_u is null, that is, undefined
	 */
	Header operator () (const std::string& s) const {
		Header h(*this);
		if (helper.do_s) {
			h.data += helper.do_s(s);
			h.check_crlf();
			return h;
		} else
			throw std::invalid_argument("operator()(string) is undefined");
	}

	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_u_s.
	 * If well-defined, the result of do_u_s is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] u 1st argument to be passed to do_u_s
	 * @param[in] s 2nd argument to be passed to do_u_s
	 * @returns @c *this
	 * @throws std::invalid_argument if helper.do_u_s is null, that is, undefined
	 */
	Header& operator () (unsigned u, const std::string& s) {
		if (helper.do_u_s) {
			data += helper.do_u_s(u, s);
			check_crlf();
			return *this;
		} else
			throw std::invalid_argument("operator()(unsigned, string) is undefined");
	}
	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_u_s.
	 * If well-defined, the result of do_u_s is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] u 1st argument to be passed to do_u_s
	 * @param[in] s 2nd argument to be passed to do_u_s
	 * @returns a copy of @c *this with the function's return value appended to it
	 * @throws std::invalid_argument if helper.do_u_s is null, that is, undefined
	 */
	Header operator () (unsigned u, const std::string& s) const {
		Header h(*this);
		if (helper.do_u_s) {
			h.data += helper.do_u_s(u, s);
			h.check_crlf();
			return h;
		} else
			throw std::invalid_argument("operator()(unsigned, string) is undefined");
	}
		
	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_s_u.
	 * If well-defined, the result of do_s_u is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] s 1st argument to be passed to do_s_u
	 * @param[in] u 2nd argument to be passed to do_s_u
	 * @returns @c *this
	 * @throws std::invalid_argument if helper.do_s_u is null, that is, undefined
	 */
	Header& operator () (const std::string& s, unsigned u) {
		if (helper.do_s_u) {
			data += helper.do_s_u(s, u);
			check_crlf();
			return *this;
		} else
			throw std::invalid_argument("operator()(string, unsigned) is undefined");
	}
	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_s_u.
	 * If well-defined, the result of do_s_u is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] s 1st argument to be passed to do_s_u
	 * @param[in] u 2nd argument to be passed to do_s_u
	 * @returns a copy of @c *this with the function's return value appended to it
	 * @throws std::invalid_argument if helper.do_s_u is null, that is, undefined
	 */
	Header operator () (const std::string& s, unsigned u) const {
		Header h(*this);
		if (helper.do_s_u) {
			h.data += helper.do_s_u(s, u);
			h.check_crlf();
			return h;
		} else
			throw std::invalid_argument("operator()(string, unsigned) is undefined");
	}
	
	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_s_s.
	 * If well-defined, the result of do_s_s is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] s1 1st argument to be passed to do_s_s
	 * @param[in] s2 2nd argument to be passed to do_s_s
	 * @returns @c *this
	 * @throws std::invalid_argument if helper.do_s_s is null, that is, undefined
	 */
	Header& operator () (const std::string& s1, const std::string& s2) {
		if (helper.do_s_s) {
			data += helper.do_s_s(s1, s2);
			check_crlf();
			return *this;
		} else
			throw std::invalid_argument("operator()(string, string) is undefined");
	}
	/*! @brief Function-call operator
	 * The behavior of this function depends on the value of @c helper.do_s_s.
	 * If well-defined, the result of do_s_s is appended to the internal buffer.
	 * Otherwise, std::invalid_argument is thrown.
	 * @param[in] s1 1st argument to be passed to do_s_s
	 * @param[in] s2 2nd argument to be passed to do_s_s
	 * @returns a copy of @c *this with the function's return value appended to it
	 * @throws std::invalid_argument if helper.do_s_s is null, that is, undefined
	 */
	Header operator () (const std::string& s1, const std::string& s2) const {
		Header h(*this);
		if (helper.do_s_s) {
			h.data += helper.do_s_s(s1, s2);
			h.check_crlf();
			return h;
		} else
			throw std::invalid_argument("operator()(string, string) is undefined");
	}
	//@}
	/*! @name Appenders
	 * These overloads append additional headers to the existing one.
	 */
	//@{
	/*! @brief Append a complete header line
	 *  @param[in] s line to append
	 */
	Header& operator += (const std::string& s) {
		data += s;
		check_crlf();
		return *this;
	}

	/*! @brief Append a name: value header line
	 *  @param[in] p header pair to append
	 */
	Header& operator += (const std::pair<std::string, std::string>& p) {
		data += p.first;
		data += ": ";
		data += p.second;
		check_crlf();
		return *this;
	}
	
	/*! @brief Append complete header line(s)
	 *  @param[in] h {}-list of line(s) to append
	 */
	Header& operator += (std::initializer_list<std::string> h) {
		for (const auto& s : h) {
			data += s;
			check_crlf();
		}
		return *this;
	}

	/*! @brief Append name: value header line(s)
	 *  @param[in] hp {}-list of header pairs to append
	 */
	Header& operator += (std::initializer_list<std::pair<std::string, std::string>> hp) {
		for (const auto& p : hp) {
			operator += (p);
		}
		return *this;
	}
	//@}
	
	//! String cast operator
	operator std::string () const {
		std::stringstream ss;
		ss << data;
		for (const auto& cookie_k : cookies) {
			for (const auto& cookie_v : cookie_k.second) {
				ss << cookie_v << std::endl;
			}
		}
		ss << "\r\n";
		return ss.str();
	}

	//! String cast operator
	operator const char* () const {
		return this->operator std::string().c_str();
	}


	Helper helper;
private:
	std::string data;
	void check_crlf() {
		auto _e = data.cend();
		if (!(*(_e - 2) == '\r' && *(_e - 1) == '\n'))
			data.append("\r\n");
		size_t _n;
		if ((_n = data.find("\r\n\r\n")) != std::string::npos) {
			data.erase(_n + 2);
		}
	}
			
};

/*! @name Concatenators
  * These overloads create new headers equivalent to 
  * additional headers being appended to an existing one.
  */
//@{
/*! @brief Concatenate a complete header line
 *  @param[in] _h header
 *  @param[in] _s line to append
 */
Header operator + (const Header& _h, const std::string& _s) {
	Header h(_h);
	h += _s;
	return h;
}

/*! @brief Concatenate a complete header line
 *  @param[in] _h header
 *  @param[in] _s line to concatenate
 */
Header operator + (Header&& _h, const std::string& _s) {
	Header h(std::move(_h));
	h += _s;
	return std::move(h);
}

/*! @brief Concatenate a name: value header line
 *  @param[in] _h header
 *  @param[in] _p header pair to concatenate
 */
Header operator + (const Header& _h, const std::pair<std::string, std::string>& _p) {
	Header h(_h);
	h += _p;
	return h;
}
	
/*! @brief Concatenate a name: value header line
 *  @param[in] _h header
 *  @param[in] _p header pair to concatenate
 */
Header operator + (Header&& _h, const std::pair<std::string, std::string>& _p) {
	Header h(_h);
	h += _p;
	return std::move(h);
}
	
/*! @brief Concatenate complete header line(s)
 *  @param[in] _h header
 *  @param[in]_hl {}-list of header lines to concatenate
 */
Header operator + (const Header& _h, std::initializer_list<std::string> _hl) {
	Header h(_h);
	h += _hl;
	return _h;
}

/*! @brief Concatenate complete header line(s)
 *  @param[in] _h header
 *  @param[in]_hl {}-list of header lines to concatenate
 */
Header operator + (Header&& _h, std::initializer_list<std::string> _hl) {
	Header h(std::move(_h));
	h += _hl;
	return std::move(h);
}

/*! @brief Concatenate name: value header line(s)
 *  @param[in] _h header
 *  @param[in] _hp {}-list of header pairs to concatenate
 */
Header operator + (const Header& _h, std::initializer_list<std::pair<std::string, std::string>> _hp) {
	Header h(_h);
	h += _hp;
	return h;
}
	
/*! @brief Concatenate name: value header line(s)
 *  @param[in] _h header
 *  @param[in] _hp {}-list of header pairs to concatenate
 */
Header operator + (Header&& _h, std::initializer_list<std::pair<std::string, std::string>> _hp) {
	Header h(std::move(_h));
	h += _hp;
	return std::move(h);
}	
//@}

std::pair<std::string, std::string> P(std::string&& s1, std::string&& s2) {
	return std::make_pair(std::move(s1), std::move(s2));
}
std::pair<std::string, std::string> P(const std::string& s1, const std::string& s2) {
	return std::make_pair(s1, s2);
}

/*! @name Predefined headers
 */
//@{
const Header content_type (helpers::content_type::helper());
const Header redirect (helpers::redirect::helper());
const Header response (helpers::response::helper());
const Header status (helpers::status::helper());
//@}

}

}

MOSH_CGI_END

#endif
