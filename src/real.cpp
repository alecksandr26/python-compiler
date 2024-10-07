// real.cpp
#include "real.hpp"

namespace pyc {

    Real::Real(const double &value, const Token &token)
        : Token(token.get_type(), token.get_tag()),
          value_(value)
    {
    }

    double Real::get_value() const {
        return value_;
    }

    std::string Real::to_string() const {
        return "Real<" + std::to_string(value_) + ">";
    }

    std::ostream &operator<<(std::ostream &os, const Real &real) {
        os << real.to_string();
        return os;
    }

}
