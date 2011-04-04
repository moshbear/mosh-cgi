#ifndef _CGI_FORM_MULTIPARTHEADER_H_
#define _CGI_FORM_MULTIPARTHEADER_H_ 1

#include <string>
#include <cgi/common/Kv.h>

namespace cgi { namespace form {
// ============================================================
// Class MultipartHeader
// ============================================================
struct MultipartHeader : public cgi::common::KV<char> {
private:
	typedef cgi::common::KV<char> KV;
public:
	MultipartHeader(const std::string& disposition, const std::string&  name, const std::string&  filename, const std::string&  cType);

	inline MultipartHeader(const MultipartHeader&  head)
	: KV() {

		operator=(head);
	}
	inline MultipartHeader(MultipartHeader&& head)
	: KV() {
		operator=(head);
	}
	~MultipartHeader() { }

	MultipartHeader&  operator=(const MultipartHeader&  head);
	MultipartHeader& operator=(MultipartHeader&& head);
	
	std::string contentDisposition;
	std::string contentType;
};

} } // namespace form // namespace cgi


#endif
