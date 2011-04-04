/* Abstracts an XML Processing Instruction.
 */
#ifndef _CGI_XML_PI_H_
#define _CGI_XML_PI_H_ 1

#include <string>
#include <list>
#include <ostream>

#include <cgi/common/Streamable.h>
#include <util/STL_Foreach.h>
#include <cgi/common/Kv.h>

namespace cgi { namespace xml {
template <class charT>
class PI: public cgi::common::Streamable<charT> {
	typedef cgi::common::Streamable<charT> base_type;
	typedef cgi::xml::PI<charT> this_type;
public:
	typedef typename cgi::common::KV<charT, false, true> attribute_type; 
	typedef typename base_type::string_type string_type;
	typedef typename base_type::ostream_type ostream_type;
	std::list<attribute_type> attributes;
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
	this_type& operator=(this_type&& element) {
		if (this != &element) {
			attributes = std::move(element.attributes);
			name = std::move(element.name);
		}
		return *this;
	}

	virtual void render(ostream_type& out) const {
		out << "<?" << name;
		if (!attributes.empty()) {
			out << ' ';
			FOREACH(auto const& iter, attributes, {
				iter->render(out);
				out << ' ';
			})
		}
		out << "?>";
	}
private:
};
} } // namespace xml // namespace cgi

#endif
