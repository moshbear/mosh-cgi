/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: CgiEnvironment.h,v 1.19 2007/07/02 18:48:17 sebdiaz Exp $
 *
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
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

#ifndef _CGI_ENVIRONMENT_H_
#define _CGI_ENVIRONMENT_H_ 1

/*! \file CgiEnvironment.h
 * \brief Class encapsulating the CGI runtime environment
 *
 * The \c CgiEnvironment class encapsulates the  environment of
 * the CGI application as described by the HTTP server.  \c CgiEnvironment
 * contains the \c GET or \c POST data along with all environment variables
 * set by the HTTP server specified in the CGI specification.
 */

#include <string>
#include <vector>
#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>
#include <fastcgi++/http.hpp>

#include <cgi/http/Cookie.h>

namespace cgi { 

// ============================================================
// Class CgiEnvironment
// ============================================================

/*! \class CgiEnvironment CgiEnvironment.h cgi/CgiEnvironment.h
 * \brief Class encapsulating the CGI runtime environment
 *
 * The \c CgiEnvironment class encapsulates the  environment of
 * the CGI application as described by the HTTP server.  \c CgiEnvironment
 * contains the \c GET or \c POST data along with all environment variables
 * set by the HTTP server specified in the CGI specification.
 */
template <class charT>
class Environment : private boost::noncopyable {
public:

	friend class Cgi<charT>;

	// ============================================================

	/*! \name Constructors and Destructor */
	//@{

	/*!
	 * \brief Read in the CGI environment passed to the CGI application
	 * by the server
	 *
	 * This function is not usually called directly; instead, an object of type
	 * CgiEnvironment is retrieved by calling the \c getEnvironment() method
	 * on Cgicc.
	 * If you are using %cgi with FastCGI, you will need to pass
	 * a \c CgiInput subclass that %cgi will use to read input.  If
	 * \c input is omitted, standard input and environment
	 * variables will be used.
	 * \param input A CgiInput object to use for reading input
	 * \see Cgicc::getEnvironment
	 */
	Environment(Fastcgipp::Http::Session<charT>& const fcgiBuf_)
	: fcgiBuf(fcgiBuf_) {
		readEnvironmentVariables();
		parseCookies();
	}
	Environment(Environment<charT>&& env)
	: Environment(env.fcgiBuf) {
	}
	/*!
	 * \brief Destructor
	 *
	 * Delete this CgiEnvironment object
	 */
	~Environment() { }
	//@}

	// ============================================================

	/*! \name Overloaded Operators */
	//@{

	/*!
	 * \brief Compare two CgiEnvironments for equality.
	 *
	 * CgiEnvironments are equal if they have the same environment variables.
	 * \param env The CgiEnvironment to compare to this one.
	 * \return \c true if the two CgiEnvironments are equal, \c false otherwise.
	 */
	bool operator==(const Environment<charT>& env) const {
		bool result = (contentLength == env.contentLength);
		result &= (usingHTTPS == env.usingHTTPS);
		result &= (cookie == env.cookie);
		result &= (&fcgiBuf == &env.fcgiBuf);
		return result;
	}

	/*!
	 * \brief Compare two CgiEnvironments for inequality.
	 *
	 * CgiEnvironments are equal if they have the same environment variables.
	 * \param env The CgiEnvironment to compare to this one.
	 * \return \c false if the two CgiEnvironments are equal, \c true otherwise.
	 */
	inline bool operator!=(const Environment<charT>&  env) const {
		return !operator==(env);
	}
	//@}

	// ============================================================

	//! \brief Reference to Fastcgipp FastCGI buffer
	Fastcgipp::Http::Session<charT>& fcgiBuf; 
	std::vector<char>& postData() { return boost::ref(fcgiBuf.postBuffer); }
	std::map<std::string, std::string>& envData() { return boost::ref(fcgiBuf.environment); }

	//! Length of POST data
	size_t contentLength;
	//! Is connection over SSL?
	bool usingHTTPS;
	
	std::string cookie;
	std::multimap<std::basic_string<charT>, cgi::http::Cookie<charT> > cookies;
protected:
	inline void dumpToFile(std::string const& filename) const {
		fcgiBuf.dumpToFile(filename);
	}
private:
	Environment();

	// Parse the list of cookies from a string to a vector
	void parseCookies() {
		std::string data = cookie;
		if (data.empty())
			return;
		size_t pos;
		size_t oldPos = 0;
		while (true) {
			// find the ';' terminating a name=value pair
			pos = data.find(";", oldPos);

			// if no ';' was found, the rest of the string is a single cookie
			if (std::string::npos == pos) {
				parseCookie(data.substr(oldPos));
				return;
			}
			// otherwise, the string contains multiple cookies
			// extract it and add the cookie to the list
			parseCookie(data.substr(oldPos, pos - oldPos));

			// update pos (+1 to skip ';')
			oldPos = pos + 1;
		}
	}

	// Parse a single cookie string (name=value) pair
	void parseCookie(const std::basic_string<charT>& data) {
		// find the '=' separating the name and value
		std::string::size_type pos = data.find("=", 0);

		// if no '=' was found, return
		if (std::string::npos == pos)
			return;

		// skip leading whitespace - " \f\n\r\t\v"
		std::string::size_type wscount = 0;
		std::string::const_iterator data_iter;

		 for (data_iter = data.begin(); data_iter != data.end(); ++data_iter, ++wscount)
			if (0 == std::isspace(*data_iter))
				break;

		// Per RFC 2109, do not unescape the data (thanks to afm@othello.ch)
		std::basic_string<charT> name = data.substr(wscount, pos - wscount);
		std::basic_string<charT> value = data.substr(++pos);

	
		cookies.insert(std::make_pair(name, cgi::http::Cookie<charT>(name, value)));
	}

	// Read in all the environment variables
	void readEnvironmentVariables() {
		try {
			auto const& cl = envData.find("CONTENT_LENGTH");
			if (cl != envData.end())
				contentLength = boost::lexical_cast<size_t>(cl.second);
			else
				contentLength = 0;
		} catch (...) {
			contentLength = 0;
		}
		{
			auto const& hs = envData.find("HTTPS");
			if (hs != envData.end())
				usingHTTPS = (cgi::common::stringsAreEqual(hs.second, "on"));
			else
				usingHTTPS = false;
		}
		{
			auto const& c = envData.find("HTTP_COOKIE");
			if (c != envData.end())
				cookie = c.second;
		}
	}
};

} // namespace cgi

#endif /* !_CGI_ENVIRONMENT_H_ */
