#pragma once

#include <sstream>
#include <string>

namespace Exception {
    class InitError : public std::exception {

    public:
        InitError( std::string const & str ) {
            std::stringstream ss;
            ss << "InitError : " << str;
            msg = ss.str();
        }
        virtual const char* what() const noexcept {
            return (msg.c_str());
        }

    private:
        std::string msg;
    };
}
