#include <abb_tcp_ip_interface/RobotTcpIp.h>
#include <abb_tcp_ip_interface/Exceptions.h>
#include <vector>
#include <cstring>
#ifdef _WIN32
#include<windows.h>

#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif
#include <cerrno>

tcp::Client::Client(const std::string& serverIp, int port) {
    
#ifdef _WIN32
    WSADATA wsaData = {0};
    int iResult = 0;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
#else
#endif
_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port
            = htons(port);
    servAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());

    int connectStatus = connect(_fd, (struct sockaddr*)&servAddr,
                      sizeof(servAddr));

    if (connectStatus == -1) {
        throw SocketException("Unable to connect to client. Errno: " + std::string(strerror(errno)));
    }
}

tcp::Client::~Client() {
        #ifdef _WIN32
#else
    close(_fd);
#endif
}

void tcp::Client::send(const std::vector<uint8_t>& data) const {
    ::send(_fd, (char*)&data[0], data.size(), 0);
}

std::vector<uint8_t> tcp::Client::recv(unsigned int size) const {
    std::vector<uint8_t> data(size);

    int receivedBytes = ::recv(_fd, (char*)&data[0], size, 0);
    if (receivedBytes != size) {
        throw SocketException("Unable to read " + std::to_string(size) + " bytes. Number of received bytes: " +
                                 std::to_string(receivedBytes));
    }
    return data;
}

Robot::Robot(tcp::ClientPtr client) : _client(client) { }

void Robot::moveJ(const Transform& transform) {
    move(CommandType::MOVEJ, transform);
}

void Robot::moveL(const Transform& transform) {
    move(CommandType::MOVEL, transform);
}

template<typename T>
std::vector<uint8_t> packData(const T& data){
    std::vector<uint8_t> bytes(sizeof(data));
    std::memcpy(&bytes[0], &data, sizeof(data));
    return bytes;
}

std::vector<uint8_t> packData(const Robot::CommandType& cmd) {
    auto cmd_int = static_cast <uint32_t> (cmd);
    return packData(cmd_int);
}

template <typename T>
T unpackData(const std::vector<uint8_t>& data) {
    T result;
    std::memcpy(&result, &data[0], sizeof(T));
    return result;
}

void Robot::move(CommandType cmd, const Transform& transform) {
    std::vector<uint8_t> bytes;
    bytes.reserve(INT_SIZE + TRANSFORM_SIZE);
    auto rawCmd = packData(cmd);
    auto rawTransform  = packData(transform);



    bytes.insert(bytes.begin(), rawCmd.begin(), rawCmd.end());
    bytes.insert(bytes.end(), rawTransform.begin(), rawTransform.end());
    _client->send(bytes);

    // Waiting for result
    auto error_code = unpackData<int32_t>(_client->recv(INT_SIZE));

    if (error_code) {
        throw RobotException("Execution failed. Error code: " + std::to_string(error_code));
    }
}

Transform Robot::getCurrentPosition() {
    std::vector<uint8_t> bytes(INT_SIZE);
    _client->send(packData(CommandType::CURRENT_POSITION));

    // Waiting for result
    auto result = _client->recv(INT_SIZE + TRANSFORM_SIZE);
    auto error_code = unpackData<int32_t>(result);

    if (error_code) {
        throw RobotException("Communication failed, Received error code: " + std::to_string(error_code));
    }
    return unpackData<Transform>(std::vector<uint8_t>(result.begin() + INT_SIZE, result.end()));
}
