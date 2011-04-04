

#ifndef _CGI_HTTP_STATUSHEADER_H_
#define _CGI_HTTP_STATUSHEADER_H_ 1

#include <string>
#include <ostream>
#include <util/STL_Foreach.h>
#include <cgi/http/Header.h>

namespace cgi { namespace http {

// ============================================================
// Class StatusHeader
// ============================================================

/*! \class StatusHeader StatusHeader.h cgi/StatusHeader.h
 * \brief  header to set a specific request status
 *
 */
template <class charT>
class StatusHeader : public Header<charT> {
	typedef Header<charT> base_type;
public:
	typedef typename base_type::string_type string_type;
	int status;
	StatusHeader(int status_, const string_type& message)
	: base_type(message), status(status_) {
	}
	virtual ~StatusHeader() {
	}
	virtual void render(std::basic_ostream<charT>& out) const {
		out << "Status: " << status << ' ' << this->data << std::endl;
		FOREACH(const auto& iter, this->cookies, out << iter.second << std::endl;)
	}
private:
	StatusHeader();
};
} } // namespace http // namespace cgi

#endif				/* ! _CGI_HTTP_STATUSHEADER_H_ */
