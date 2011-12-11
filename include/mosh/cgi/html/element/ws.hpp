//! @file mosh/cgi/html/element/ws.hpp Wide-char elements
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


#ifdef MOSH_CGI_HTML_ELEMENT_HPP

#ifndef MOSH_CGI_HTML_ELEMENT__WS_HPP
#define MOSH_CGI_HTML_ELEMENT__WS_HPP

//! Byte-string html elements
namespace ws {
	//! @c wchar_t specialization of Element<T>
	typedef Element<wchar_t> Element;
	//! <a>
	const Element a (L"a", Type::binary);
	//! <abbr>
	const Element abbr (L"abbr", Type::binary);
	//! <address>
	const Element address (L"address", Type::binary);
	//! <area>
	const Element area (L"area", Type::unary);
	//! <article> (HTML 5)
	const Element article (L"article", Type::binary);
	//! <aside> (HTML 5)
	const Element aside (L"aside", Type::binary);
	//! <audio> (HTML 5)
	const Element audio (L"audio", Type::binary);
	//! <b>
	const Element b (L"b", Type::binary);
	//! <base>
	const Element base (L"base", Type::unary);
	//! <bdi> (HTML 5)
	const Element bdi (L"bdi", Type::binary);
	//! <bdo>
	const Element bdo (L"bdo", Type::binary);
	//! <big> (not in HTML 5)
	const Element big (L"big", Type::binary);
	//! <blockquote>
	const Element blockquote (L"blockquote", Type::binary);
	//! <body>
	const Element body (L"body", Type::binary);
	//! <br>
	const Element br (L"br", Type::unary);
	//! <button>
	const Element button (L"button", Type::binary);
	//! <canvas> (HTML 5)
	const Element canvas (L"canvas", Type::binary);
	//! <caption>
	const Element caption (L"caption", Type::binary);
	//! <cite>
	const Element cite (L"cite", Type::binary);
	//! <code>
	const Element code (L"code", Type::binary);
	//! <col>
	const Element col (L"col", Type::unary);
	//! <colgroup>
	const Element colgroup (L"colgroup", Type::binary);
	//! <command> (HTML 5)
	const Element command (L"command", Type::binary);
	//! <datalist> (HTML 5)
	const Element datalist (L"datalist", Type::binary);
	//! <dd>
	const Element dd (L"dd", Type::binary);
	//! <del>
	const Element del (L"del", Type::binary);
	//! <details> (HTML 5)
	const Element details (L"details", Type::binary);
	//! <dfn>
	const Element dfn (L"dfn", Type::binary);
	//! <div>
	const Element div (L"div", Type::binary);
	//! <dl>
	const Element dl (L"dl", Type::binary);
	//! <dt>
	const Element dt (L"dt", Type::binary);
	//! <em>
	const Element em (L"em", Type::binary);
	//! <embed> (HTML 5)
	const Element embed (L"embed", Type::unary);
	//! <fieldset>
	const Element fieldset (L"fieldset", Type::binary);
	//! <figcaption> (HTML 5)
	const Element figcaption (L"figcaption", Type::binary);
	//! <figure> (HTML 5)
	const Element figure (L"figure", Type::binary);
	//! <footer> (HTML 5)
	const Element footer (L"footer", Type::binary);
	//! <form>
	const Element form (L"form", Type::binary);
	//! <frame> (Frameset DTDs only)
	const Element frame (L"frame", Type::unary);
	//! <frameset> (Frameset DTDs only)
	const Element frameset (L"frameset", Type::binary);
	//! <h1>
	const Element h1 (L"h1", Type::binary);
	//! <h2>
	const Element h2 (L"h2", Type::binary);
	//! <h3>
	const Element h3 (L"h3", Type::binary);
	//! <h4>
	const Element h4 (L"h4", Type::binary);
	//! <h5>
	const Element h5 (L"h5", Type::binary);
	//! <h6>
	const Element h6 (L"h6", Type::binary);
	//! <head>
	const Element head (L"head", Type::binary);
	//! <header> (HTML 5)
	const Element header (L"header", Type::binary);
	//! <hgroup> (HTML 5)
	const Element hgroup (L"hgroup", Type::binary);
	//! <hr>
	const Element hr (L"hr", Type::unary);
	//! <html>
	const Element html (L"html", Type::binary);
	//! <i>
	const Element i (L"i", Type::binary)
	//! <iframe> (Frameset DTDs only / HTML 5)
	const Element iframe (L"iframe", Type::binary);
	//! <img>
	const Element img (L"img", Type::unary);
	//! <input>
	const Element input (L"input", Type::unary);
	//! <ins>
	const Element ins (L"ins", Type::binary);
	//! <keygen> (HTML 5)
	const Element keygen (L"keygen", Type::unary);
	//! <kbd>
	const Element kbd (L"kbd", Type::binary);
	//! <label>
	const Element label (L"label", Type::binary);
	//! <legend>
	const Element legend (L"legend", Type::binary);
	//! <li>
	const Element li (L"li", Type::binary);
	//! <link>
	const Element link (L"link", Type::unary);
	//! <mark> (HTML 5)
	const Element mark (L"mark", Type::binary);
	//! <menu>
	const Element menu (L"menu", Type::binary);
	//! <meta>
	const Element meta (L"meta", Type::unary);
	//! <meter> (HTML 5)
	const Element meter (L"meter", Type::binary);
	//! <nav> (HTML 5)
	const Element nav (L"nav", Type::binary);
	//! <noframes> (Frameset DTDs only)
	const Element noframes (L"noframes", Type::binary);
	//! <noscript>
	const Element noscript (L"noscript", Type::binary);
	//! <object>
	const Element object (L"object", Type::binary);
	//! <ol>
	const Element ol (L"ol", Tyoe::binary);
	//! <optgroup>
	const Element optgroup (L"optgroup", Type::binary);
	//! <option>
	const Element option (L"option", Type::binary);
	//! <output> (HTML 5)
	const Element output (L"output", Type::binary);
	//! <p>
	const Element p (L"p", Type::binary);
	//! <param>
	const Element param (L"param", Type::unary);
	//! <pre>
	const Element pre (L"pre", Type::binary);
	//! <progress> (HTML 5)
	const Element progress (L"progress", Type::binary);
	//! <q>
	const Element q (L"q", Type::binary);
	//! <rb> (XHTML 1.1 Ruby)
	const Element rb (L"rb", Type::binary);
	//! <rb> (XHTML 1.1 Ruby)
	const Element rb (L"rbc", Type::binary);
	//! <rp> (XHTML 1.1 / HTML 5 Ruby)
	const Element rp (L"rp", Type::binary);
	//! <rt> (XHTML 1.1 / HTML 5 Ruby)
	const Element rt (L"rt", Type::binary);
	//! <rtc> (XHTML 1.1 Ruby)
	const Element rt (L"rtc", Type::binary);
	//! <ruby> (XHTML 1.1 / HTML 5 Ruby)
	const Element ruby (L"ruby", Type::binary);
	//! <s>
	const Element s  (L"s", Type::binary);
	//! <samp>
	const Element samp (L"samp", Type::binary);
	//! <script>
	const Element script (L"script", Type::binary);
	//! <section> (HTML 5)
	const Element section (L"section", Type::binary); 
	//! <select>
	const Element select (L"select", Type::binary);
	//! <small>
	const Element small (L"small", Type::binary);
	//! <source> (HTML 5)
	const Element source (L"source", Type::binary); 
	//! <span>
	const Element span (L"span", Type::binary);
	//! <strong>
	const Element strong (L"strong", Type::binary);
	//! <style>
	const Element style (L"style", Type::binary);
	//! <sub>
	const Element sub (L"sub", Type::binary);
	//! <summary> (HTML 5)
	const Element summary (L"summary", Type::binary); 
	//! <sup>
	const Element sup (L"sup", Type::binary);
	//! <table>
	const Element table (L"table", Type::binary);
	//! <tbody>
	const Element tbody (L"tbody", Type::binary);
	//! <td>
	const Element td (L"td", Type::binary);
	//! <textarea>
	const Element textarea (L"textarea", Type::binary);
	//! <tfoot>
	const Element tfoot (L"tfoot", Type::binary);
	//! <th>
	const Element th (L"th", Type::binary);
	//! <thead>
	const Element thead (L"thread", Type::binary);
	//! <time> (HTML 5)
	const Element time (L"time", Type::binary);
	//! <title>
	const Element title (L"title", Type::binary);
	//! <tr>
	const Element tr (L"tr", Type::binary);
	//! <track> (HTML 5)
	const Element track (L"track", Type::unary);
	//! <tt> (not in HTML 5)
	const Element tt (L"tt", Type::binary);
	//! <ul> 
	const Element ul (L"ul", Type::binary);
	//! <var>
	const Element var (L"var", Type::binary);	
	//! <video> (HTML 5)
	const Element video (L"video", Type::binary);
	//!  <wbr> (HTML 5)
	const Element wbr (L"wbr", Type::binary); 
	
	//! @c wchar_t specialization of Html_begin<T>
	typedef HTML_begin<wchar_t> html_begin;
	//! @c wchar_t specialization of Html_end<T>
	typedef HTML_end<wchar_t> html_end;
	//! @c wchar_t specialization of Body_begin<T>
	const Body_begin<wchar_t> body_begin;
	//! @c wchar_t specialization of Body_end<T>
	const Body_end<wchar_t> body_end;
}

#endif

#endif
