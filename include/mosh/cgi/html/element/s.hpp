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

//! Byte-string html elements
namespace s {
	const Element<char> a ("a", Type::binary);
	const Element<char> abbr ("abbr", Type::binary);
	const Element<char> address ("address", Type::binary);
	const Element<char> area ("area", Type::unary);
	const Element<char> article ("article", Type::binary);
	const Element<char> aside ("aside", Type::binary);
	const Element<char> audio ("audio", Type::binary);
	const Element<char> b ("b", Type::binary);
	const Element<char> base ("base", Type::unary);
	const Element<char> bdi ("bdi", Type::binary);
	const Element<char> bdo ("bdo", Type::binary);
	const Element<char> big ("big", Type::binary);
	const Element<char> blockquote ("blockquote", Type::binary);
	const Element<char> body ("body", Type::binary);
	const Element<char> br ("br", Type::unary);
	const Element<char> button ("button", Type::binary);
	const Element<char> canvas ("canvas", Type::binary);
	const Element<char> caption ("caption", Type::binary);
	const Element<char> cite ("cite", Type::binary);
	const Element<char> code ("code", Type::binary);
	const Element<char> col ("col", Type::unary);
	const Element<char> colgroup ("colgroup", Type::binary);
	const Element<char> command ("command", Type::binary);
	const Element<char> datalist ("datalist", Type::binary);
	const Element<char> dd ("dd", Type::binary);
	const Element<char> del ("del", Type::binary);
	const Element<char> details ("details", Type::binary);
	const Element<char> dfn ("dfn", Type::binary);
	const Element<char> div ("div", Type::binary);
	const Element<char> dl ("dl", Type::binary);
	const Element<char> dt ("dt", Type::binary);
	const Element<char> em ("em", Type::binary);
	const Element<char> embed ("embed", Type::unary);
	const Element<char> fieldset ("fieldset", Type::binary);
	const Element<char> figcaption ("figcaption", Type::binary);
	const Element<char> figure ("figure", Type::binary);
	const Element<char> footer ("footer", Type::binary);
	const Element<char> form ("form", Type::binary);
	const Element<char> frame ("frame", Type::unary);
	const Element<char> frameset ("frameset", Type::binary);
	const Element<char> h1 ("h1", Type::binary);
	const Element<char> h2 ("h2", Type::binary);
	const Element<char> h3 ("h3", Type::binary);
	const Element<char> h4 ("h4", Type::binary);
	const Element<char> h5 ("h5", Type::binary);
	const Element<char> h6 ("h6", Type::binary);
	const Element<char> head ("head", Type::binary);
	const Element<char> header ("header", Type::binary);
	const Element<char> hgroup ("hgroup", Type::binary);
	const Element<char> hr ("hr", Type::unary);
	const Element<char> html ("html", Type::binary);
	const Element<char> i ("i", Type::binary)
	const Element<char> iframe ("iframe", Type::binary);
	const Element<char> img ("img", Type::unary);
	const Element<char> input ("input", Type::unary);
	const Element<char> ins ("ins", Type::binary);
	const Element<char> keygen ("keygen", Type::unary);
	const Element<char> kbd ("kbd", Type::binary);
	const Element<char> label ("label", Type::binary);
	const Element<char> legend ("legend", Type::binary);
	const Element<char> li ("li", Type::binary);
	const Element<char> link ("link", Type::unary);
	const Element<char> mark ("mark", Type::binary);
	const Element<char> menu ("menu", Type::binary);
	const Element<char> meta ("meta", Type::unary);
	const Element<char> meter ("meter", Type::binary);
	const Element<char> nav ("nav", Type::binary);
	const Element<char> noframes ("noframes", Type::binary);
	const Element<char> noscript ("noscript", Type::binary);
	const Element<char> object ("object", Type::binary);
	const Element<char> ol ("ol", Tyoe::binary);
	const Element<char> optgroup ("optgroup", Type::binary);
	const Element<char> option ("option", Type::binary);
	const Element<char> output ("output", Type::binary);
	const Element<char> p ("p", Type::binary);
	const Element<char> param ("param", Type::unary);
	const Element<char> pre ("pre", Type::binary);
	const Element<char> progress ("progress", Type::binary);
	const Element<char> q ("q", Type::binary);
	const Element<char> rp ("rp", Type::binary);
	const Element<char> rt ("rt", Type::binary);
	const Element<char> ruby ("ruby", Type::binary);
	const Element<char> s  ("s", Type::binary);
	const Element<char> samp ("samp", Type::binary);
	const Element<char> script ("script", Type::binary);
	const Element<char> section ("section", Type::binary); 
	const Element<char> select ("select", Type::binary);
	const Element<char> small ("small", Type::binary);
	const Element<char> source ("source", Type::binary); 
	const Element<char> span ("span", Type::binary);
	const Element<char> strong ("strong", Type::binary);
	const Element<char> style ("style", Type::binary);
	const Element<char> sub ("sub", Type::binary);
	const Element<char> summary ("summary", Type::binary); 
	const Element<char> sup ("sup", Type::binary);
	const Element<char> table ("table", Type::binary);
	const Element<char> tbody ("tbody", Type::binary);
	const Element<char> td ("td", Type::binary);
	const Element<char> textarea ("textarea", Type::binary);
	const Element<char> tfoot ("tfoot", Type::binary);
	const Element<char> th ("th", Type::binary);
	const Element<char> thead ("thread", Type::binary);
	const Element<char> time ("time", Type::binary);
	const Element<char> title ("title", Type::binary);
	const Element<char> tr ("tr", Type::binary);
	const Element<char> track ("track", Type::unary);
	const Element<char> tt ("tt", Type::binary);
	const Element<char> ul ("ul", Type::binary);
	const Element<char> var ("var", Type::binary);	
	const Element<char> video ("video", Type::binary);
	const Element<char> wbr ("wbr", Type::binary); 
	
	typedef HTML_begin<char> html_begin;
	typedef HTML_end<char> html_end;
	typedef Body_begin<char> body_begin;
	typedef Body_end<char> body_end;
}

#endif

#endif
