//! @file mosh/cgi/html/element/s.hpp Byte-char elements
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
#ifndef MOSH_CGI_HTML_ELEMENT__S_HPP
#define MOSH_CGI_HTML_ELEMENT__S_HPP

#include <mosh/cgi/html/element.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN
namespace html {
namespace element {

//! Byte-string html elements
namespace s {
	//! @c char specialization of Element<T>
	typedef element::Element<char> Element;
	//! <a>
	const Element a ("a", Type::binary);
	//! <abbr>
	const Element abbr ("abbr", Type::binary);
	//! <address>
	const Element address ("address", Type::binary);
	//! <area>
	const Element area ("area", Type::unary);
	//! <article> (HTML 5)
	const Element article ("article", Type::binary);
	//! <aside> (HTML 5)
	const Element aside ("aside", Type::binary);
	//! <audio> (HTML 5)
	const Element audio ("audio", Type::binary);
	//! <b>
	const Element b ("b", Type::binary);
	//! <base>
	const Element base ("base", Type::unary);
	//! <bdi> (HTML 5)
	const Element bdi ("bdi", Type::binary);
	//! <bdo>
	const Element bdo ("bdo", Type::binary);
	//! <big> (not in HTML 5)
	const Element big ("big", Type::binary);
	//! <blockquote>
	const Element blockquote ("blockquote", Type::binary);
	//! <body>
	const Element body ("body", Type::binary);
	//! <br>
	const Element br ("br", Type::unary);
	//! <button>
	const Element button ("button", Type::binary);
	//! <canvas> (HTML 5)
	const Element canvas ("canvas", Type::binary);
	//! <caption>
	const Element caption ("caption", Type::binary);
	//! <cite>
	const Element cite ("cite", Type::binary);
	//! <code>
	const Element code ("code", Type::binary);
	//! <col>
	const Element col ("col", Type::unary);
	//! <colgroup>
	const Element colgroup ("colgroup", Type::binary);
	//! <command> (HTML 5)
	const Element command ("command", Type::binary);
	//! <datalist> (HTML 5)
	const Element datalist ("datalist", Type::binary);
	//! <dd>
	const Element dd ("dd", Type::binary);
	//! <del>
	const Element del ("del", Type::binary);
	//! <details> (HTML 5)
	const Element details ("details", Type::binary);
	//! <dfn>
	const Element dfn ("dfn", Type::binary);
	//! <div>
	const Element div ("div", Type::binary);
	//! <dl>
	const Element dl ("dl", Type::binary);
	//! <dt>
	const Element dt ("dt", Type::binary);
	//! <em>
	const Element em ("em", Type::binary);
	//! <embed> (HTML 5)
	const Element embed ("embed", Type::unary);
	//! <fieldset>
	const Element fieldset ("fieldset", Type::binary);
	//! <figcaption> (HTML 5)
	const Element figcaption ("figcaption", Type::binary);
	//! <figure> (HTML 5)
	const Element figure ("figure", Type::binary);
	//! <footer> (HTML 5)
	const Element footer ("footer", Type::binary);
	//! <form>
	const Element form ("form", Type::binary);
	//! <frame> (Frameset DTDs only)
	const Element frame ("frame", Type::unary);
	//! <frameset> (Frameset DTDs only)
	const Element frameset ("frameset", Type::binary);
	//! <h1>
	const Element h1 ("h1", Type::binary);
	//! <h2>
	const Element h2 ("h2", Type::binary);
	//! <h3>
	const Element h3 ("h3", Type::binary);
	//! <h4>
	const Element h4 ("h4", Type::binary);
	//! <h5>
	const Element h5 ("h5", Type::binary);
	//! <h6>
	const Element h6 ("h6", Type::binary);
	//! <head>
	const Element head ("head", Type::binary);
	//! <header> (HTML 5)
	const Element header ("header", Type::binary);
	//! <hgroup> (HTML 5)
	const Element hgroup ("hgroup", Type::binary);
	//! <hr>
	const Element hr ("hr", Type::unary);
	//! <html>
	const Element html ("html", Type::binary);
	//! <i>
	const Element i ("i", Type::binary);
	//! <iframe> (Frameset DTDs only / HTML 5)
	const Element iframe ("iframe", Type::binary);
	//! <img>
	const Element img ("img", Type::unary);
	//! <input>
	const Element input ("input", Type::unary);
	//! <ins>
	const Element ins ("ins", Type::binary);
	//! <keygen> (HTML 5)
	const Element keygen ("keygen", Type::unary);
	//! <kbd>
	const Element kbd ("kbd", Type::binary);
	//! <label>
	const Element label ("label", Type::binary);
	//! <legend>
	const Element legend ("legend", Type::binary);
	//! <li>
	const Element li ("li", Type::binary);
	//! <link>
	const Element link ("link", Type::unary);
	//! <mark> (HTML 5)
	const Element mark ("mark", Type::binary);
	//! <menu>
	const Element menu ("menu", Type::binary);
	//! <meta>
	const Element meta ("meta", Type::unary);
	//! <meter> (HTML 5)
	const Element meter ("meter", Type::binary);
	//! <nav> (HTML 5)
	const Element nav ("nav", Type::binary);
	//! <noframes> (Frameset DTDs only)
	const Element noframes ("noframes", Type::binary);
	//! <noscript>
	const Element noscript ("noscript", Type::binary);
	//! <object>
	const Element object ("object", Type::binary);
	//! <ol>
	const Element ol ("ol", Type::binary);
	//! <optgroup>
	const Element optgroup ("optgroup", Type::binary);
	//! <option>
	const Element option ("option", Type::binary);
	//! <output> (HTML 5)
	const Element output ("output", Type::binary);
	//! <p>
	const Element p ("p", Type::binary);
	//! <param>
	const Element param ("param", Type::unary);
	//! <pre>
	const Element pre ("pre", Type::binary);
	//! <progress> (HTML 5)
	const Element progress ("progress", Type::binary);
	//! <q>
	const Element q ("q", Type::binary);
	//! <rb> (XHTML 1.1 Ruby)
	const Element rb ("rb", Type::binary);
	//! <rb> (XHTML 1.1 Ruby)
	const Element rbc ("rbc", Type::binary);
	//! <rp> (XHTML 1.1 / HTML 5 Ruby)
	const Element rp ("rp", Type::binary);
	//! <rt> (XHTML 1.1 / HTML 5 Ruby)
	const Element rt ("rt", Type::binary);
	//! <rtc> (XHTML 1.1 Ruby)
	const Element rtc ("rtc", Type::binary);
	//! <ruby> (XHTML 1.1 / HTML 5 Ruby)
	const Element ruby ("ruby", Type::binary);
	//! <s>
	const Element s ("s", Type::binary);
	//! <samp>
	const Element samp ("samp", Type::binary);
	//! <script>
	const Element script ("script", Type::binary);
	//! <section> (HTML 5)
	const Element section ("section", Type::binary); 
	//! <select>
	const Element select ("select", Type::binary);
	//! <small>
	const Element small ("small", Type::binary);
	//! <source> (HTML 5)
	const Element source ("source", Type::binary); 
	//! <span>
	const Element span ("span", Type::binary);
	//! <strong>
	const Element strong ("strong", Type::binary);
	//! <style>
	const Element style ("style", Type::binary);
	//! <sub>
	const Element sub ("sub", Type::binary);
	//! <summary> (HTML 5)
	const Element summary ("summary", Type::binary); 
	//! <sup>
	const Element sup ("sup", Type::binary);
	//! <table>
	const Element table ("table", Type::binary);
	//! <tbody>
	const Element tbody ("tbody", Type::binary);
	//! <td>
	const Element td ("td", Type::binary);
	//! <textarea>
	const Element textarea ("textarea", Type::binary);
	//! <tfoot>
	const Element tfoot ("tfoot", Type::binary);
	//! <th>
	const Element th ("th", Type::binary);
	//! <thead>
	const Element thead ("thread", Type::binary);
	//! <time> (HTML 5)
	const Element time ("time", Type::binary);
	//! <title>
	const Element title ("title", Type::binary);
	//! <tr>
	const Element tr ("tr", Type::binary);
	//! <track> (HTML 5)
	const Element track ("track", Type::unary);
	//! <tt> (not in HTML 5)
	const Element tt ("tt", Type::binary);
	//! <ul> 
	const Element ul ("ul", Type::binary);
	//! <var>
	const Element var ("var", Type::binary);	
	//! <video> (HTML 5)
	const Element video ("video", Type::binary);
	//!  <wbr> (HTML 5)
	const Element wbr ("wbr", Type::binary); 
	
	//! <!-- ... -->
	const Element comment ("!--", Type::comment);
	
	//! @c char specialization of Html_begin<T>
	typedef element::HTML_begin<char> html_begin;
	//! @c char specialization of Html_end<T>
	typedef element::HTML_end<char> html_end;
	//! @c char specialization of Body_begin<T>
	typedef element::Body_begin<char> body_begin;
	//! @c char specialization of Body_end<T>
	typedef element::Body_end<char> body_end;

	//@{
	//! @c char specialization of P
	std::pair<std::string, std::string>
	P(const std::string& s1, const std::string& s2) {
		return element::P(s1, s2);
	}
	//! @c char specialization of P
	std::pair<std::string, std::string>
	P(std::string&& s1, std::string&& s2) {
		return element::P(std::move(s1), std::move(s2));
	}
	//@}
}

}
}

MOSH_CGI_END

#endif
