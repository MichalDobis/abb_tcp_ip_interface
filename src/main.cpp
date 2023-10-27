#include <abb_tcp_ip_interface/RobotTcpIp.h>
#include <iostream>

int main (int argc, const char * argv[]) {

    std::cout << "Waiting for server...." << std::endl;
    tcp::Client client("192.168.10.1", 11003);
    std::cout << "Connected to the server" << std::endl;

    Robot robot(std::make_shared<tcp::Client>(client));

    auto position = robot.getCurrentPosition();
    robot.moveJ(Transform(Position(0, 0, 0), Quaternion(0, 0, 0, 1)));
}
