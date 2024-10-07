// integer.hpp
#ifndef INTEGER_INCLUDED
#define INTEGER_INCLUDED

#include "token.hpp"
#include <ostream>

namespace pyc {
    class Integer : public Token {
    private:
        long value_;

    public:
        Integer(const long &value, const Token &token);
        virtual ~Integer() = default;

        long get_value() const;

        std::string to_string() const override;

        friend std::ostream &operator<<(std::ostream &os, const Integer &integer);
    };
}

#endif // INTEGER_INCLUDED
