
#ifndef _CGI_EXCEPTIONS_H_
#define _CGI_EXCEPTIONS_H_ 1

#include <string>
#include <stdexcept>

namespace cgi {

/*!
 * \brief Base class for exceptions thrown by \c Cgi
 */
class Exception : public std::runtime_error {
public:
	Exception(const std::string& s) : std::runtime_error(s) { }
};

#define CGI_EXCEPT(eName) \
	class eName : public Exception {\
	public: eName(const std::string& s) : Exception(s) { } \
	}
/*!
 * \class IsList
 * \brief Scalar value expected; list value produced
 * Certain CGI elements can have more than one value per name. Access on
 * those values results in a list of references, not a reference to a
 * unique value. Should that happen, \c IsList is thrown.
 */
CGI_EXCEPT(IsList);
/*!
 * \class NotFound
 * \brief An element wasn't found
 */
CGI_EXCEPT(NotFound);

#undef CGI_EXCEPT


}

#endif /* !_CGI_EXCEPTIONS_H_ */
	

