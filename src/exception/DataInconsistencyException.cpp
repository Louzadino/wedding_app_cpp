#include "DataInconsistencyException.hpp"

namespace exception {

// Construtor que recebe apenas a mensagem
DataInconsistencyException::DataInconsistencyException(const std::string& message)
    : std::runtime_error(message) {}

// Construtor que recebe mensagem + exceção encadeada
DataInconsistencyException::DataInconsistencyException(const std::string& message, const std::exception& cause)
    : std::runtime_error(message + " | Causa: " + cause.what()) {}

} // namespace exception
