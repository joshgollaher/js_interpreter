#ifndef VALUE_H
#define VALUE_H

#include <functional>
#include <unordered_map>
#include <variant>

namespace JS
{
    class Value
    {
    public:
        using Array = std::vector<std::shared_ptr<Value>>;
        using Object = std::unordered_map<std::string, std::shared_ptr<Value>>;
        using Function = std::function<std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>)>; // TODO

        enum class Type
        {
            NUMBER,
            BOOLEAN,
            FUNCTION,
            STRING,
            ARRAY,
            OBJECT,
            UNDEFINED,
            NAN,
            NIL, // Aka null
            INFINITY,
            NEG_INFINITY
        };

        Value() : m_type(Type::UNDEFINED) {}
        explicit Value(const std::string& str) : m_type(Type::STRING), m_data(str) {}
        explicit Value(double num) : m_type(Type::NUMBER), m_data(num) {}
        explicit Value(bool boolean) : m_type(Type::BOOLEAN), m_data(boolean) {}
        explicit Value(Array array) : m_type(Type::ARRAY), m_data(array) {}
        explicit Value(Function func) : m_type(Type::FUNCTION), m_data(func) {}
        explicit Value(Object obj) : m_type(Type::OBJECT), m_data(obj) {}
        explicit Value(std::nullptr_t) : m_type(Type::NIL) {}
        explicit Value(const Type special_type)
        {
            switch(special_type)
            {
            case Type::INFINITY:
            case Type::NEG_INFINITY:
            case Type::NAN:
            case Type::UNDEFINED:
                m_type = special_type;
            default:
                throw std::runtime_error("Invalid Value type");
            }
        }

        template<typename T>
        T& as()
        {
            if(!std::holds_alternative<T>(m_data))
            {
                throw std::runtime_error("Failed to unwrap Value");
            }

            return std::get<T>(m_data);
        }

        template<typename T>
        const T& as() const
        {
            if(!std::holds_alternative<T>(m_data))
            {
                throw std::runtime_error("Failed to unwrap Value");
            }

            return std::get<T>(m_data);
        }

        Type type() const { return m_type; }

    private:
        Type m_type;
        std::variant<std::monostate, std::string, double, Array, Object, Function, bool> m_data {std::monostate{}};
    };
}


#endif //VALUE_H
