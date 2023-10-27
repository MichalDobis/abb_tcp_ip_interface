#include <abb_tcp_ip_interface/RobotTcpIp.h>
#include <iostream>
#include <unistd.h>

int main (int argc, const char * argv[]) {

    std::cout << "Waiting for server...." << std::endl;
    tcp::Client client("192.168.2.1", 11003);
    std::cout << "Connected to the server" << std::endl;

    Robot robot(std::make_shared<tcp::Client>(client));

    auto position = robot.getCurrentPosition();
    std::cout << position.position.x << ", " << position.position.y << ", " << position.position.z << std::endl;
    sleep(1);
    position.position.x += 100;
    robot.moveL(Transform(position));
    std::cout << "target reached" << std::endl;
    sleep(2);
}
