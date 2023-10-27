#ifndef ROBOT_TCP_IP_ROBOTTCPIP_H
#define ROBOT_TCP_IP_ROBOTTCPIP_H

#include <string>
#include <vector>
#include <memory>
#include <abb_tcp_ip_interface/Datatypes.h>

namespace tcp {
    class Client {
    public:
        Client(const std::string& serverIp, int port);
        [[nodiscard]] std::vector<uint8_t> recv(unsigned int size) const;
        void send(const std::vector<uint8_t>& data) const;
        ~Client();
    private:
        int _fd;

    };
    using ClientPtr = std::shared_ptr<Client>;
} // tcp

class Robot {
public:

    enum class CommandType {
        MOVEJ = 1,
        MOVEL = 2,
        CURRENT_POSITION = 3
    };


    Robot(tcp::ClientPtr client);
    void moveJ(const Transform& transform);
    void moveL(const Transform& transform);
    Transform getCurrentPosition();

private:
    std::shared_ptr<tcp::Client> _client;
    void move(CommandType type, const Transform& transform);
    const static int INT_SIZE = sizeof(uint32_t);
    const static int TRANSFORM_SIZE = sizeof(Transform);
};

#endif //ROBOT_TCP_IP_ROBOTTCPIP_H
