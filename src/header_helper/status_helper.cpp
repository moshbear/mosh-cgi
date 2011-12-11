//! @file header_helpers/status_helper.cpp - String lookup for HTTP status codes
/* 
 *  Copyright (C) 2011 m0shbear
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

#include <string>
#include <stdexcept>
#include <vector>
#include <mosh/cgi/http/helpers/status_helper.hpp>
#include <mosh/cgi/bits/singleton.hpp>
#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/bits/namespace.hpp>

namespace {
	
class _generator : public MOSH_CGI::Singleton<_generator> {
public:
	//! Initialization
	_generator() {
		codes = {
				{ // Informational 1xx
					/* 100 (HTTP/1.1) */ "Continue",
					/* 101 (HTTP/1.1) */ "Switching Protocols",
					/* 102 (WebDAV) (RFC 2518) */ "Processing"
				},
				{ // Successful 2xx
					/* 200 */ "OK",
					/* 201 */ "Created",
					/* 202 */ "Accepted",
					/* 203 (HTTP/1.1) */ "Non-Authoritative Information",
					/* 204 */ "No Content",
					/* 205 */ "Reset Content",
					/* 206 */ "Partial Content",
					/* 207 (WebDAV) (RFC 4918) */ "Multi-Status",
					/* 208 (WebDAV) (RFC 5842) */ "Already Reported",
					/* 209 (UNASSIGNED) */ "",
					/* 210 (UNASSIGNED) */ "",
					/* 211 (UNASSIGNED) */ "",
					/* 212 (UNASSIGNED) */ "",
					/* 213 (UNASSIGNED) */ "",
					/* 214 (UNASSIGNED) */ "",
					/* 215 (UNASSIGNED) */ "",
					/* 216 (UNASSIGNED) */ "",
					/* 217 (UNASSIGNED) */ "",
					/* 218 (UNASSIGNED) */ "",
					/* 219 (UNASSIGNED) */ "",
					/* 220 (UNASSIGNED) */ "",
					/* 221 (UNASSIGNED) */ "",
					/* 222 (UNASSIGNED) */ "",
					/* 223 (UNASSIGNED) */ "",
					/* 224 (UNASSIGNED) */ "",
					/* 225 (UNASSIGNED) */ "",
					/* 226 (RFC 3229) */ "IM Used"
				},
				{ // Redirection 3xx
					/* 300 */ "Multiple Choices",
					/* 301 */ "Moved Permanently",
					/* 302 */ "Found",
					/* 303 (HTTP/1.1) */ "See Other",
					/* 304 */ "Not Modified",
					/* 305 (HTTP/1.1) */ "Use Proxy",
					/* 306 (RESERVED) */ "",
					/* 307 (HTTP/1.1) */ "Temporary Redirect",
					/* 308 (Google Gears) */ "Resume Incomplete"
				},
				{ // Client Error 4xx
					/* 400 */ "Bad Request",
					/* 401 */ "Unauthorized",
					/* 402 */ "Payment Required",
					/* 403 */ "Forbidden",
					/* 404 */ "Not Found",
					/* 405 */ "Method Not Allowed",
					/* 406 */ "Not Acceptable",
					/* 407 */ "Proxy Authentication Required",
					/* 408 */ "Request Timeout",
					/* 409 */ "Conflict",
					/* 410 */ "Gone",
					/* 411 */ "Length Required",
					/* 412 */ "Precondition Failed",
					/* 413 */ "Request Entity Too Large",
					/* 414 */ "Request-URI Too Long",
					/* 415 */ "Unsupported Media Type",
					/* 416 */ "Requested Range Not Satisfiable",
					/* 417 */ "Expectation Failed",
					/* 418 (RFC 2324) */ "I'm a teapot",
					/* 419 (UNASSIGNED) */ "",
					/* 420 (lulz) */ "Insufficient weed",
					/* 421 (UNASSIGNED) */ "",
					/* 422 (WebDAV) (RFC 4918) */ "Unprocessable Entity",
					/* 423 (WebDAV) (RFC 4918) */ "Locked",
					/* 424 (WebDAV) (RFC 4918) */ "Failed Dependency",
					/* 425 (RFC 3648) */ "Unordered Collection",
					/* 426 (RFC 2817) */ "Upgrade Required",
					/* 427 (UNASSIGNED) */ "",
					/* 428 (Proposed) */ "Precondition Required",
					/* 429 (Proposed) */ "Too Many Requests",
					/* 430 (UNASSIGNED) */ "",
					/* 431 (Proposed) */ "Request Header Fields Too Large"
				},
				{ // Server Error 5xx
					/* 500 */ "Internal Server Error",
					/* 501 */ "Not Implemented",
					/* 502 */ "Bad Gateway",
					/* 503 */ "Service Unavailable",
					/* 504 */ "Gateway Timeout",
					/* 505 */ "HTTP Version Not Supported",
					/* 506 (RFC 2295) */ "Variant Also Negotiates",
					/* 507 (WebDAV) (RFC 4918) */ "Insufficient Storage",
					/* 508 (WebDAV) (RFC 5842) */ "Loop Detected",
					/* 509 (Apache Extension) */ "Bandwidth Limit Exceeded",
					/* 510 (RFC 2774) */ "Not Extended",
					/* 511 (Proposed) */ "Network Authentication Required"
				}
			};
	}

	//! Return the highest defined subcode in a given family/series
	unsigned max(unsigned series) {
		if (series < 1 || series > 5) {
			throw std::invalid_argument("status family out of range");
		}
		return codes[series].size() - 1;
	}

	/*! @brief Get the status string of HTTP xyy code
	 *  @param[in] series first digit of HTTP status code
	 *  @param[in] sub last two digits of HTTP status code
	 *  @return the status string
	 *  @retval "" unassigned code
	 */
	std::string operator () (unsigned series, unsigned sub) const {
		return codes[series][sub];
	}
private:
	std::vector<std::vector<std::string>> codes;
};

}

MOSH_CGI_BEGIN

namespace http {

namespace helpers {

//! HTTP status lookup
namespace status_helper {
	
/*! @brief Get the corresponding status string for a given code
 *  @param[in] st HTTP status code
 *  @return the corresponding status string
 *  @throw std::invalid_argument if the status code is invalid
 */
std::string get_string (unsigned st) {
	unsigned series = st / 100;
	unsigned sub = st % 100;
	if (series < 1 || series > 5) {
		throw std::invalid_argument("status family out of range");
	}
	_generator& g = _generator::instance();
	std::string msg;
	if (sub > g.max(series)) {
		goto unassigned;
	}
	msg = g(series, sub);
	if (msg != "") {
		return msg;
	}
unassigned:
	throw std::invalid_argument("unassigned status code");
	
}

} // status_helper

} // helpers

} // http

MOSH_CGI_END

