#ifndef ERRORS_H
#define ERRORS_H
#include <exception>

namespace JS
{

    class InvalidSyntax final : public std::exception {};

#define not_implemented() throw std::runtime_error(std::format("Not implemented: {} line {}", __FILE__, __LINE__));

}

#endif //ERRORS_H
