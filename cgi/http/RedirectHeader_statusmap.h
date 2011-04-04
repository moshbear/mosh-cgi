#ifdef _CGI_HTTP_REDIRECTHEADER_H_

#ifndef _CGI_HTTP_REDIRECTHEADER_STATUSMAP_H_
#define _CGI_HTTP_REDIRECTHEADER_STATUSMAP_H_ 1


class StatusMap : public util::Singleton<cgi::http::RedirectHeader<charT>::StatusMap> {
public:
	std::map<int, std::basic_string<charT> > lookup;
	StatusMap() {
		lookup = {
			{300, boost::lexical_cast<string_type>("Multiple Choices")},
			{301, boost::lexical_cast<string_type>("Moved Permanently")},
			{302, boost::lexical_cast<string_type>("Found")},
			{303, boost::lexical_cast<string_type>("See Other")},
			{304, boost::lexical_cast<string_type>("Not Modified")},
			{305, boost::lexical_cast<string_type>("Use Proxy")},
			{307, boost::lexical_cast<string_type>("Temporary Redirect")}
		};
	}
};
#endif				/* ! _CGI_HTTP_REDIRECTHEADER_STATUSMAP_H_ */
#endif
