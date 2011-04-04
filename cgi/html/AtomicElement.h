#ifndef _CGI_HTML_ATOMICELEMENT_H_
#define _CGI_HTML_ATOMICELEMENT_H_ 1

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <cgi/Types.h>
#include <cgi/common/Kv.h>
#include <cgi/html/Element.h>

namespace cgi { namespace html {
template <class charT, class Tag>
class AtomicElement: public Element<charT> {
public:
	AtomicElement()
	: Element<charT>(0, 0, 0, Element<charT>::atomic) {
	}
	AtomicElement(cgi::list<cgi::common::KV<charT, false, true> >::type const& attributes)
	: Element<charT>(&attributes, 0, 0, Element<charT>::atomic) {
	}

	virtual ~AtomicElement() {
	}

	/*!
	 * \brief Clone this element
	 *
	 * \return A newly-allocated copy of this element
	 */
	virtual boost::shared_ptr<Element<charT> > clone() const {
		return boost::shared_ptr<Element<charT> >(AtomicElement<charT, Tag>(*this));
	}
	virtual const std::basic_string<charT> name() const {
		return boost::lexical_cast<std::basic_string<charT> >(Tag::name());
	}
};

} }				// namespace cgi

#endif				/* ! _HTMLATOMICELEMENT_H_ */
