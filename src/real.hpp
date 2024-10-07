// real.hpp
#ifndef REAL_INCLUDED
#define REAL_INCLUDED

#include "token.hpp"
#include <ostream>

namespace pyc {
    class Real : public Token {
    private:
        double value_;
    public:
        Real(const double &value, const Token &token);
        virtual ~Real() = default;

        double get_value() const;

        std::string to_string() const override;

        friend std::ostream &operator<<(std::ostream &os, const Real &real);
    };
}

#endif // REAL_INCLUDED
