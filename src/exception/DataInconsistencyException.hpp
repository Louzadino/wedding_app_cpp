#ifndef DATA_INCONSISTENCY_EXCEPTION_HPP
#define DATA_INCONSISTENCY_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace exception {

class DataInconsistencyException : public std::runtime_error {
public:
    explicit DataInconsistencyException(const std::string& message);
    DataInconsistencyException(const std::string& message, const std::exception& cause);
};

} // namespace exception

#endif
