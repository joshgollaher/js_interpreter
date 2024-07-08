cmake_minimum_required(VERSION 3.26)
project(js)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/AST.cpp
    src/Lexer.cpp
    src/Parser.cpp
    include/Lexer.h
    include/errors.h
    include/Log.h
    src/Log.cpp
    include/AST.h
    include/Span.h
    include/Value.h
    include/Scope.h
    include/Forward.h
    include/Parser.h
)

add_executable(js ${SOURCES})
