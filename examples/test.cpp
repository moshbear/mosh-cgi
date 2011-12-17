/*!  @file examples/test.cpp
 *   @brief A sanity check for the HTML generator subsystem.
 */
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

#include <iostream>

#include <mosh/cgi/html/element.hpp>
#include <mosh/cgi/html/element/s.hpp>

using namespace std;
using namespace MOSH_CGI;
using namespace MOSH_CGI::html::element;

int main() {
	cout << s::html_begin()({s::P("lang", "en"), s::P("xml=foo","bar")}, "<!ELEMENT foobar (baz*)>") << endl;
	cout << s::head(s::title("hello")) + S("in head") << endl;
	cout << s::body_begin();
	cout << s::div(s::P("test1", "test2"), "Hello") << endl;
	cout << s::body_end() << s::html_end() << endl;
}

