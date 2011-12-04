// Cookies demo

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

#include <new>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

//#include <mosh/cgi/http/content_header_classes.hpp>
#include <mosh/cgi/html/element.hpp>

#if HAVE_SYS_UTSNAME_H
#  include <sys/utsname.h>
#endif

#if HAVE_SYS_TIME_H
#  include <sys/time.h>
#endif

#include "styles.h"

using namespace std;
using namespace MOSH_CGI;

// Print the form for this CGI
string printForm() {
	using namespace html::element;

	return SElement["form"]({SP("method", "post"), SP("action", "foo")}, {
			SElement["table"]({
				SElement["tr"]({
					SElement["td"](SP("class", "title"), "Cookie Name"),
					SElement["td"](SP("class", "form"), SElement["input"]({SP("type", "text"), SP("name", "name")}))
				}),
				SElement["tr"]({
					SElement["td"](SP("class", "title"), "Cookie Value"),
					SElement["td"](SP("class", "form"), SElement["input"]({SP("type", "text"), SP("name", "value")}))
				})
			}), 
			SElement["div"](SP("class", "center"),
	                	SElement["p"]({
					SElement["input"]({SP("type", "submit"),SP("name", "submit"),SP("value", "Set the cookie")}),
					SElement["input"]({SP("type", "reset"),SP("value", "nevermind")})
				})
			)
		});

}

// Main Street, USA
int
main(int /*argc*/,
     char ** /*argv*/)
{
/*
try {
#if HAVE_GETTIMEOFDAY
		timeval start;
		gettimeofday(&start, NULL);
#endif
		std::string name;
		std::string value;

		    // Output the headers for an HTML document with the cookie only
		    // if the cookie is not empty
		if (name != "" && value != "")
			cout << HTTPHTMLHeader().setCookie(HTTPCookie(name, value);
		else
			cout << HTTPHTMLHeader();

		// Output the HTML 4.0 DTD info
		cout << HTMLDoctype(HTMLDoctype::eStrict) << endl;
		html::element::Element<char> myHtml = html::element::SElement["html"] << html::element::Attr("lang", "en")
		                                      << html::element::Attr("dir", "ltr");
		// Set up the page's header and title.
		std::string myHead = (html::element::SElement["head"] << (html::element::SElement["style"]
		                      << html::element::Attr("type", "text/css") << styles)
		                      << (html::element::SElelement["title"] << "mosh-cgi HTTPCookie"));

		myHtml << myHead;

		html::element::Element<char> myBody = html::element::SElement["body"];

		myBody << (html::element::SElement["h1"] << "mosh"
			<< (html::element::SElement["span"] << html::element::Attr("class", "red") << "-cgi")
			<< "HTTP Cookies Test Results");
		 myBody << (html::element::SElement["comment"] << "This page generated by mosh-cgi for $ENV[HTTP_REMOTE_HOST]");
		 myBody << (html::element::SElement["h4"] << "Thanks for using mosh"
		 		<< (html::element::SElement["span"]
					<< html::element::Attr("class", "red")
					<< "-cgi")
				<< " $ENV[HTTP_REMOTE_HOST]($ENV[HTTP_REMOTE_ADDR])!");
		if (name != "" && value != "") {
			myBody << (html::element::SElement["p"]
					<< "A cookie with the name"
					<< (html::element::SElement["em"] << name)
					<< "and value"
					<< (html::element::SElement["em"] << value)
					<< "was set." << html::element::SElement["br"]
					<< "In order for the cookie to show up here you must "
					<< (html::element::SElement["a"] << html::element::Attr("href", "$ENV[HTTP_SCRIPT_NAME]")
						<< "refresh"
					)
				);
		// Show the cookie info from the environment
		myBody << (html::element::SElement["h2"] << "Cookie Information from the Environment");
		myBody << (html::element::SElement["div"] << html::element::Attr("align", "center")
				<< (html::element::SElement["table"]
					<< (html::element::SElement["tr"]
						<< (html::element::SElement["td"] << html::element::Attr("class", "title")
							<< "HTTPCookie"
				


		cout << tr() << td("HTTPCookie").set("class","title")
		     << td(env.getCookies()).set("class","data") << tr() << endl;

		cout << table() << cgicc::div() << endl;


		// Show the cookie info from the cookie list
		cout << h2("HTTP Cookies via vector") << endl;

		cout << cgicc::div().set("align","center") << endl;

		cout << table() << endl;

		cout << tr().set("class","title") << td("Cookie Name")
		     << td("Cookie Value") << tr() << endl;

		// Iterate through the vector, and print out each value
		const_cookie_iterator iter;
		for(iter = env.getCookieList().begin();
		        iter != env.getCookieList().end();
		        ++iter) {
			cout << tr().set("class","data") << td(iter->getName())
			     << td(iter->getValue()) << tr() << endl;
		}
		cout << table() << cgicc::div() << endl;


		// Print out the form to do it again
		cout << br() << endl;
		printForm(cgi);
		cout << hr().set("class", "half") << endl;

		// Information on cgicc
		cout << cgicc::div().set("align","center").set("class","smaller") << endl;
		cout << "GNU cgi" << span("cc").set("class","red") << " v";
		cout << cgi.getVersion() << br() << endl;
		cout << "Compiled at " << cgi.getCompileTime();
		cout << " on " << cgi.getCompileDate() << br() << endl;

		cout << "Configured for " << cgi.getHost();
#if HAVE_UNAME
		struct utsname info;
		if(uname(&info) != -1) {
			cout << ". Running on " << info.sysname;
			cout << ' ' << info.release << " (";
			cout << info.nodename << ")." << endl;
		}
#else
		cout << "." << endl;
#endif

#if HAVE_GETTIMEOFDAY
		// Information on this query
		timeval end;
		gettimeofday(&end, NULL);
		long us = ((end.tv_sec - start.tv_sec) * 1000000)
		          + (end.tv_usec - start.tv_usec);

		cout << br() << "Total time for request = " << us << " us";
		cout << " (" << static_cast<double>(us/1000000.0) << " s)";
#endif

		// End of document
		cout << cgicc::div() << endl;
		cout << body() << html() << endl;

		// No chance for failure in this example
		return EXIT_SUCCESS;
	}

	// Did any errors occur?
	catch(const std::exception& e) {

		// Output the HTTP headers for an HTML document, and the HTML 4.0 DTD info
		cout << HTTPHTMLHeader() << HTMLDoctype(HTMLDoctype::eStrict) << endl;
		cout << html().set("lang","en").set("dir","ltr") << endl;

		// Set up the page's header and title.
		// I will put in lfs to ease reading of the produced HTML.
		cout << head() << endl;

		// Output the style sheet portion of the header
		cout << style() << comment() << endl;
		cout << "body { color: black; background-color: white; }" << endl;
		cout << "hr.half { width: 60%; align: center; }" << endl;
		cout << "span.red, strong.red { color: red; }" << endl;
		cout << "div.notice { border: solid thin; padding: 1em; margin: 1em 0; "
		     << "background: #ddd; }" << endl;

		cout << comment() << style() << endl;

		cout << title("GNU cgicc exception") << endl;
		cout << head() << endl;

		cout << body() << endl;

		cout << h1() << "GNU cgi" << span("cc", set("class","red"))
		     << " caught an exception" << h1() << endl;

		cout << cgicc::div().set("align","center").set("class","notice") << endl;

		cout << h2(e.what()) << endl;

		// End of document
		cout << cgicc::div() << endl;
		cout << hr().set("class","half") << endl;
		cout << body() << html() << endl;

		return EXIT_SUCCESS;
	}
	*/
	std::cout << printForm() << endl;
}
