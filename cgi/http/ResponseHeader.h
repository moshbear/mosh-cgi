#ifndef _CGI_HTTP_RESPONSEHEADER_H_
#define _CGI_HTTP_RESPONSEHEADER_H_

#include <string>
#include <map>
#include <list>
#include <ostream>
#include <utility>
#include <util/STL_Foreach.h>
#include <cgi/common/Streamable.h>
#include <cgi/http/Cookie.h>

namespace cgi { namespace http {

template <class charT>
class ResponseHeader : public cgi::common::Streamable<charT> {
	typedef cgi::common::Streamable<charT> base_type;
	typedef ResponseHeader<charT> this_type;
public:
	typedef typename base_type::string_type string_type;
	typedef Cookie<charT> cookie_type;

	ResponseHeader(const string_type& http_version, int status_code, const string_type& reason_phrase)
		: base_type(), HTTPVersion(http_version), statusCode(status_code), reasonPhrase(reason_phrase)
	{
	}
	virtual ~ResponseHeader() { }
	
	virtual void render(typename base_type::ostream_type& out) const {
		out << HTTPVersion << ' ' << statusCode << ' ' << reasonPhrase << std::endl;
		FOREACH(const auto& iter, headers, out << iter << std::endl;)
		FOREACH(const auto& iter, cookies, out << iter.second << std::endl;)
		out << std::endl;
	}

	string_type HTTPVersion;
	int statusCode;
	string_type reasonPhrase;
	typename std::list<string_type> headers;
	typename std::multimap<string_type, cookie_type> cookies;
 private:
	ResponseHeader();
};

} } //namespace http // namespace cgi

#endif				/* ! _CGI_HTTP_RESPONSEHEADER_H_ */
