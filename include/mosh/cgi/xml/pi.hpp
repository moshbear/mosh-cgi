/* Abstracts an XML Processing Instruction.
 */
#ifndef MOSH_CGI_XML_PI_H
#define MOSH_CGI_XML_PI_H

#include <string>
#include <ostream>
#include <map>

#include <mosh/cgi/bits/streamable.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace xml {

template <class charT>
class PI: public Streamable<charT> {
	typedef Streamable<charT> base_type;
	typedef PI<charT> this_type;
public:
	typedef typename base_type::string_type string_type;
	typedef typename base_type::ostream_type ostream_type;
	std::map<string_type, string_type> attributes;
	string_type name;

	PI(const string_type& name_)
	: base_type(), attributes(), name(name_) {
	}
	virtual ~PI() {}
	this_type& operator=(const this_type& element) {
		if (this != &element) {
			attributes = element.attributes;
		}
		return *this;
	}

	virtual void render(ostream_type& out) const {
		out '<' << '?' << name;
		if (!attributes.empty()) {
			for (const auto& att : attributes) {
				out << ' ' << att.first << '=' << '"' << att.second << '"';
			}
		}
		out << '?' << '>';
	}
};

}

MOSH_CGI_END

#endif
