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
namespace s {
	const Element<wchar_t> a (L"a", Type::binary);
	const Element<wchar_t> abbr (L"abbr", Type::binary);
	const Element<wchar_t> address (L"address", Type::binary);
	const Element<wchar_t> area (L"area", Type::unary);
	const Element<wchar_t> article (L"article", Type::binary);
	const Element<wchar_t> aside (L"aside", Type::binary);
	const Element<wchar_t> audio (L"audio", Type::binary);
	const Element<wchar_t> b (L"b", Type::binary);
	const Element<wchar_t> base (L"base", Type::unary);
	const Element<wchar_t> bdi (L"bdi", Type::binary);
	const Element<wchar_t> bdo (L"bdo", Type::binary);
	const Element<wchar_t> big (L"big", Type::binary);
	const Element<wchar_t> blockquote (L"blockquote", Type::binary);
	const Element<wchar_t> body (L"body", Type::binary);
	const Element<wchar_t> br (L"br", Type::unary);
	const Element<wchar_t> button (L"button", Type::binary);
	const Element<wchar_t> canvas (L"canvas", Type::binary);
	const Element<wchar_t> caption (L"caption", Type::binary);
	const Element<wchar_t> cite (L"cite", Type::binary);
	const Element<wchar_t> code (L"code", Type::binary);
	const Element<wchar_t> col (L"col", Type::unary);
	const Element<wchar_t> colgroup (L"colgroup", Type::binary);
	const Element<wchar_t> command (L"command", Type::binary);
	const Element<wchar_t> datalist (L"datalist", Type::binary);
	const Element<wchar_t> dd (L"dd", Type::binary);
	const Element<wchar_t> del (L"del", Type::binary);
	const Element<wchar_t> details (L"details", Type::binary);
	const Element<wchar_t> dfn (L"dfn", Type::binary);
	const Element<wchar_t> div (L"div", Type::binary);
	const Element<wchar_t> dl (L"dl", Type::binary);
	const Element<wchar_t> dt (L"dt", Type::binary);
	const Element<wchar_t> em (L"em", Type::binary);
	const Element<wchar_t> embed (L"embed", Type::unary);
	const Element<wchar_t> fieldset (L"fieldset", Type::binary);
	const Element<wchar_t> figcaption (L"figcaption", Type::binary);
	const Element<wchar_t> figure (L"figure", Type::binary);
	const Element<wchar_t> footer (L"footer", Type::binary);
	const Element<wchar_t> form (L"form", Type::binary);
	const Element<wchar_t> frame (L"frame", Type::unary);
	const Element<wchar_t> frameset (L"frameset", Type::binary);
	const Element<wchar_t> h1 (L"h1", Type::binary);
	const Element<wchar_t> h2 (L"h2", Type::binary);
	const Element<wchar_t> h3 (L"h3", Type::binary);
	const Element<wchar_t> h4 (L"h4", Type::binary);
	const Element<wchar_t> h5 (L"h5", Type::binary);
	const Element<wchar_t> h6 (L"h6", Type::binary);
	const Element<wchar_t> head (L"head", Type::binary);
	const Element<wchar_t> header (L"header", Type::binary);
	const Element<wchar_t> hgroup (L"hgroup", Type::binary);
	const Element<wchar_t> hr (L"hr", Type::unary);
	const Element<wchar_t> html (L"html", Type::binary);
	const Element<wchar_t> i (L"i", Type::binary)
	const Element<wchar_t> iframe (L"iframe", Type::binary);
	const Element<wchar_t> img (L"img", Type::unary);
	const Element<wchar_t> input (L"input", Type::unary);
	const Element<wchar_t> ins (L"ins", Type::binary);
	const Element<wchar_t> keygen (L"keygen", Type::unary);
	const Element<wchar_t> kbd (L"kbd", Type::binary);
	const Element<wchar_t> label (L"label", Type::binary);
	const Element<wchar_t> legend (L"legend", Type::binary);
	const Element<wchar_t> li (L"li", Type::binary);
	const Element<wchar_t> link (L"link", Type::unary);
	const Element<wchar_t> mark (L"mark", Type::binary);
	const Element<wchar_t> menu (L"menu", Type::binary);
	const Element<wchar_t> meta (L"meta", Type::unary);
	const Element<wchar_t> meter (L"meter", Type::binary);
	const Element<wchar_t> nav (L"nav", Type::binary);
	const Element<wchar_t> noframes (L"noframes", Type::binary);
	const Element<wchar_t> noscript (L"noscript", Type::binary);
	const Element<wchar_t> object (L"object", Type::binary);
	const Element<wchar_t> ol (L"ol", Tyoe::binary);
	const Element<wchar_t> optgroup (L"optgroup", Type::binary);
	const Element<wchar_t> option (L"option", Type::binary);
	const Element<wchar_t> output (L"output", Type::binary);
	const Element<wchar_t> p (L"p", Type::binary);
	const Element<wchar_t> param (L"param", Type::unary);
	const Element<wchar_t> pre (L"pre", Type::binary);
	const Element<wchar_t> progress (L"progress", Type::binary);
	const Element<wchar_t> q (L"q", Type::binary);
	const Element<wchar_t> rp (L"rp", Type::binary);
	const Element<wchar_t> rt (L"rt", Type::binary);
	const Element<wchar_t> ruby (L"ruby", Type::binary);
	const Element<wchar_t> s  (L"s", Type::binary);
	const Element<wchar_t> samp (L"samp", Type::binary);
	const Element<wchar_t> script (L"script", Type::binary);
	const Element<wchar_t> section (L"section", Type::binary); 
	const Element<wchar_t> select (L"select", Type::binary);
	const Element<wchar_t> small (L"small", Type::binary);
	const Element<wchar_t> source (L"source", Type::binary); 
	const Element<wchar_t> span (L"span", Type::binary);
	const Element<wchar_t> strong (L"strong", Type::binary);
	const Element<wchar_t> style (L"style", Type::binary);
	const Element<wchar_t> sub (L"sub", Type::binary);
	const Element<wchar_t> summary (L"summary", Type::binary); 
	const Element<wchar_t> sup (L"sup", Type::binary);
	const Element<wchar_t> table (L"table", Type::binary);
	const Element<wchar_t> tbody (L"tbody", Type::binary);
	const Element<wchar_t> td (L"td", Type::binary);
	const Element<wchar_t> textarea (L"textarea", Type::binary);
	const Element<wchar_t> tfoot (L"tfoot", Type::binary);
	const Element<wchar_t> th (L"th", Type::binary);
	const Element<wchar_t> thead (L"thread", Type::binary);
	const Element<wchar_t> time (L"time", Type::binary);
	const Element<wchar_t> title (L"title", Type::binary);
	const Element<wchar_t> tr (L"tr", Type::binary);
	const Element<wchar_t> track (L"track", Type::unary);
	const Element<wchar_t> tt (L"tt", Type::binary);
	const Element<wchar_t> ul (L"ul", Type::binary);
	const Element<wchar_t> var (L"var", Type::binary);	
	const Element<wchar_t> video (L"video", Type::binary);
	const Element<wchar_t> wbr (L"wbr", Type::binary); 
	
	typedef HTML_begin<wchar_t> html_begin;
	typedef HTML_end<wchar_t> html_end;
	typedef Body_begin<wchar_t> body_begin;
	typedef Body_end<wchar_t> body_end;
}

#endif

#endif
