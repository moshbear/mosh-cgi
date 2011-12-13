// Cookies demo
// Refactoring will be done during the Great Merge (of this and mosh-fcgi)

/*
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
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

/*! \file cookie.cpp
 * \brief HTTP cookie demo
 *
 * Tests and demonstrates how to use the HTTPCookie class
 */

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <cstdlib>

extern "C" {
#include <sys/utsname.h>
#include <sys/time.h>
}

#include <mosh/cgi/http/header.hpp>
#define MOSH_CGI_USING_BYTE_ELEMENTS
#include <mosh/cgi/html/element.hpp>

#include "styles.h"

#define S(x) ((x).operator std::string())
#define S2(x) ((x).c_str())

extern "C" void gdb_break() { }

using namespace std;
using namespace MOSH_CGI;
using namespace html;
using namespace html::element;

// Print the form for this CGI
string printForm() {

	return s::form( {SP("method", "post"), SP("action", "foo")}, {
		s::table({
			s::tr({
				s::td(SP("class", "title"), "Cookie Name"),
				s::td(SP("class", "form"), s::input({SP("type", "text"), SP("name", "name")}))
			}),
			s::tr({
				s::td(SP("class", "title"), "Cookie Value"),
				s::td(SP("class", "form"), s::input({SP("type", "text"), SP("name", "value")}))
			})
		}),
		s::div(SP("class", "center"),
			s::p({
				s::input({SP("type", "submit"), SP("name", "submit"), SP("value", "Set the cookie")}),
				s::input({SP("type", "reset"), SP("value", "nevermind")})
			})
		)
	});

}

// Main Street, USA
int main() {
	using namespace http;
	timeval start;
	gettimeofday(&start, NULL);
	string name;
	string value;
	// Output the headers for an HTML document with the cookie only
	// if the cookie is not empty
	{
		header::Header h = header::content_type("text/html") + http::Cookie { "hello", "world" } ;
		if (name != "" && value != "")
			h += header::P(name, value);
		cout << h;
	}
	// Output the HTML 4.0 DTD info
	cout << s::html_begin(doctype::HTML_revision::html_4)({
		SP("lang", "en"),
		SP("dir", "ltr")
	});
	gdb_break();
	// Set up the page's header and title.
	cout << s::head( {
		s::style(SP("type", "text/css"), styles),
		s::title("mosh-cgi cookie")
	});
	cout << s::body_begin();
	gdb_break();
	cout << S(s::h1("mosh")) + S(s::span(SP("class", "red"), "-cgi")) + "HTTP Cookies Test Results";
	cout << s::comment("This page generated by mosh-cgi for $ENV[HTTP_REMOTE_HOST]");
	cout << S(s::h4("Thanks for using mosh" + S(s::span(SP("class", "red"), "-cgi")))) + " $ENV[HTTP_REMOTE_HOST]($ENV[HTTP_REMOTE_ADDR])!";
	if (name != "" && value != "")
		cout << s::p({ "A cookie with the name ", s::em(name), " and value ", s::em(value), "was set. ", s::br,
					   "In order for the cookie to show up here you must ",
					   s::a(SP("href", "$ENV[HTTP_SCRIPT_NAME]"), "refresh")
			});
	// Show the cookie info from the environment
	cout << s::h2("Cookie Information from the Environment");
	cout << s::div(SP("align", "center"),
	s::table(s::tr({
		s::td(SP("class", "title"), "cookie"),
		s::td(SP("class", "data"), "$ENV[HTTP_COOKIE]")
	})));
#if 0
	// Show the cookie info from the cookie list
	cout << h2("HTTP Cookies via vector") << endl;
	
	{
		s::Element _t = s::table();
		_t += s::tr(SP("class", "title"), {
				s::td("Cookie Name"),
				s::td("Cookie Value")
			});
			

		
	// Iterate through the vector, and print out each value
	for (const auto& e : env.cookies) {
		for (const auto& f : e) {
			_t += s::tr(SP("class", "data"), {
					s::td(e.first)
					s::tr(f.value)
				});
		}
	}
	cout << s::div(_t);
#endif

	
	// Print out the form to do it again
	cout << s::br << endl;
	printForm();
	cout << s::hr(SP("class", "half")) << endl;

	
	// Information on this query
	timeval end;
	gettimeofday(&end, NULL);
	string _s;
	{
		stringstream ss;
		ss << (end.tv_sec - start.tv_sec) << " s ";
		ss << setw(6) << setfill('0') << (end.tv_usec - start.tv_usec) << " us";
		_s = ss.str();
	}
	cout << s::p("Total time for request = " + _s);
	cout << s::body_end;
	cout << s::html_end();
	cout << endl;
}
