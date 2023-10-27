#ifndef ROBOT_TCP_IP_EXCEPTIONS_H
#define ROBOT_TCP_IP_EXCEPTIONS_H

#include <exception>
#include <string>

class Exception : public std::exception{
public:
    inline Exception(const std::string msg) : msg(msg){}

    inline const char *what() const noexcept {
        return msg.c_str();
    }
private:
    const std::string msg;
};

class SocketException : public Exception {
    using Exception::Exception;
};

class RobotException : public Exception {
    using Exception::Exception;
};

#endif //ROBOT_TCP_IP_EXCEPTIONS_H
