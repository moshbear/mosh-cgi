#ifndef MOSH_CGI_HTTP_RESPONSEHEADER_H
#define MOSH_CGI_HTTP_RESPONSEHEADER_H

#include <string>
#include <map>
#include <ostream>
#include <utility>
#include <vector>
#include <mosh/cgi/bits/streamable.hpp>
#include <mosh/cgi/http/cookie.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

template <class charT>
class Response_header : public Streamable<charT> {
	typedef Streamable<charT> base_type;
	typedef Response_header<charT> this_type;
public:
	typedef typename base_type::string_type string_type;
	typedef Cookie<charT> cookie_type;

	Response_header(const string_type& http_version_, int status_code_, const string_type& reason_phrase_)
		: base_type(), http_version(http_version_), status_code(status_code_), reason_phrase(reason_phrase_)
	{
	}
	virtual ~Response_header() { }
	
	virtual void render(typename base_type::ostream_type& out) const {
		out << http_version << ' ' << status_code << ' ' << reason_phrase << std::endl;
		for (const auto& h : headers) {
			out << h << std::endl;
		}
		for (const auto& cookie_k : this->cookies) {
			for (const auto& cookie_v : cookie_k.second) {
				out << cookie_v << std::endl;
			}
		}
		out << std::endl;
	}
	
	/*! @brief Set the 3-digit HTTP status code
	 * @throw std::invalid_argument if validation fails (condition 100 <= new_status <= 599 not met)
	 */
	void set_status(int new_status) {
		if (new_status < 100 || new_status > 599)
			throw std::invalid_argument("Status out of range");
		status = new_status;
	}
	//! Get the 3-digit HTTP status code
	int get_status() const {
		return status;
	}

	string_type http_version;
	string_type reason_phrase;
	typename std::vector<string_type> headers;
	typename std::map<string_type, std::vector<cookie_type>> cookies;
 private:
	int status_code;
	Response_header();
};

} } //namespace http // namespace cgi

#endif				/* ! MOSH_CGI_HTTP_RESPONSEHEADER_H */
