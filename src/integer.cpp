// integer.cpp
#include "integer.hpp"

namespace pyc {

    Integer::Integer(const long &value, const Token &token)
        : Token(token.get_type(), token.get_tag()),
          value_(value)
    {
    }

    long Integer::get_value() const {
        return value_;
    }

    std::string Integer::to_string() const {
        return "Integer<" + std::to_string(value_) + ">";
    }

    std::ostream &operator<<(std::ostream &os, const Integer &integer) {
        os << integer.to_string();
        return os;
    }

}
