// Private: do not manually include

#ifdef MOSH_CGI_HTML_DOCTYPE_HPP

#ifndef MOSH_CGI_HTML_DOCTYPE_STRMAP_HPP
#define MOSH_CGI_HTML_DOCTYPE_STRMAP_HPP

class Str_map : public Singleton<MOSH_CGI::Doctype<charT>::Str_map> {
public:
	std::map<Document_type, std::pair<std::string, std::string>> lookup;
	Str_map() {
		lookup = {
			{Document_type::strict, std::pair<std::string, std::string>("Strict", "strict")},
			{Document_type::transitional, std::pair<std::string, std::string>("Transitional", "transitional")},
			{Document_type::frames, std::pair<std::string, std::string>("Frameset", "frameset")}

		};
	}

};
#endif
#endif
