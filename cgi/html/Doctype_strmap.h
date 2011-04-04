#ifdef _CGI_HTML_DOCTYPE_H_

#ifndef _CGI_HTML_DOCTYPE_STRMAP_H_
#define _CGI_HTML_DOCTYPE_STRMAP_H_ 1

class StrMap : public util::Singleton<cgi::html::Doctype<charT>::StrMap> {
public:
	std::map<DocumentType, std::pair<std::string, std::string> > lookup;
	StrMap() {
		lookup = {
			{strict, std::pair<std::string, std::string>("Strict", "strict")},
			{transitional, std::pair<std::string, std::string>("Transitional", "transitional")},
			{frames, std::pair<std::string, std::string>("Frameset", "frameset")}

		};
	}

};
#endif				/* ! _CGI_HTML_DOCTYPE_STRMAP_H_ */
#endif
