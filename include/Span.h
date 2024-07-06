
#ifndef SPAN_H
#define SPAN_H
#include <string>

namespace JS {
    struct FilePosition
    {
        size_t line;
        size_t column;
        size_t character_index;
    };
    struct Span
    {
        const std::string file_name;
        const FilePosition start;
        const FilePosition end;

        [[nodiscard]] std::string to_string() const
        {
            std::ostringstream buf;
            buf << "Span [file=" << file_name << "] at [" << start.line << ":" << start.column << " to " << end.line << ":" << end.column << "]";
            return buf.str();
        }
    };
}

#endif //SPAN_H
