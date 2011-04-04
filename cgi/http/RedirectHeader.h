#ifndef _CGI_HTTP_REDIRECTHEADER_H_
#define _CGI_HTTP_REDIRECTHEADER_H_ 1

#include <util/STL_Foreach.h>
#include <boost/lexical_cast.hpp>
#include <map>
#include <ostream>
#include <string>
#include <cgi/http/Header.h>
#include <cgi/http/RedirectHeader_statusmap.incl.h>

namespace cgi { namespace http {

template <class charT>
class RedirectHeader: public Header<charT> {
	typedef Header<charT> base_type;
	typedef RedirectHeader<charT> this_type;
public:
	typedef typename base_type::string_type string_type;
private:
	#include <cgi/http/RedirectHeader_statusmap.h>
public:
	RedirectHeader(const string_type& url)
	: RedirectHeader(url, -1) { }
	RedirectHeader(const string_type& url, bool permanent)
	: RedirectHeader(url, permanent ? 301 : 307) { }
	RedirectHeader(const string_type& url, int status)
	: base_type(url), fStatus(status) {
	}
	virtual ~RedirectHeader() {
	}
	virtual void render(typename base_type::ostream_type& out) const {
		const std::map<int, string_type>&& code = (StatusMap::instance());	
		if (code.find(fStatus) != code.end())
			out << "Status: " << boost::lexical_cast<string_type>(code[fStatus]) << std::endl;
		out << "Location: " << this->data << std::endl;
		FOREACH(auto const& iter, this->cookies, out << iter.second << std::endl;)
		out << std::endl;
	}
	
private:
	RedirectHeader();
	int fStatus;
};
} } // namespace http // namespace cgi
#endif				/* ! _CGI_HTTP_REDIRECTHEADER_H_ */
