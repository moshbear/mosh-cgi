// redirect_header.hpp internal, do not use directly

#ifdef MOSH_CGI_HTTP_REDIRECTHEADER_HPP

#ifndef MOSH_CGI_HTTP_REDIRECTHEADER_STATUSMAP_HPP
#define MOSH_CGI_HTTP_REDIRECTHEADER_STATUSMAP_HPP


class Status_map : public Singleton<MOSH_CGI::http::Redirect_header<charT>::Status_map> {
public:
	std::map<int, std::basic_string<charT>> lookup;
	Status_map() {
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
#endif
#endif
