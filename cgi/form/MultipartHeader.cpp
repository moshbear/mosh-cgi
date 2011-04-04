#include <string>
#include <cgi/common/Kv.h>
#include <cgi/form/MultipartHeader.h>

cgi::form::MultipartHeader::MultipartHeader(const std::string& disposition, const std::string&  name, const std::string&  filename, const std::string&  cType)
	: KV(name, filename), contentDisposition(disposition), contentType(cType) {
	}

cgi::form::MultipartHeader&  cgi::form::MultipartHeader::operator=(const cgi::form::MultipartHeader& head) {
	KV::operator=(head);
	contentDisposition = head.contentDisposition;
	contentType = head.contentType;
	return *this;
}
cgi::form::MultipartHeader& cgi::form::MultipartHeader::operator=(cgi::form::MultipartHeader&& head) {
	KV::operator=(head);
	contentDisposition = std::move(head.contentDisposition);
	contentType = std::move(contentType);
	return *this;
}
