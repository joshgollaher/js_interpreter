#ifndef ERRORS_H
#define ERRORS_H
#include <exception>

namespace JS
{

    class InvalidSyntax final : public std::exception {};

}

#endif //ERRORS_H
