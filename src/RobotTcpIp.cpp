#include <abb_tcp_ip_interface/RobotTcpIp.h>
#include <netinet/in.h> //structure for storing address information
#include <sys/socket.h> //for socket APIs
#include <abb_tcp_ip_interface/Exceptions.h>
#include <vector>
#include <unistd.h>
#include <cstring>

tcp::Client::Client(const std::string& serverIp, int port) {
    _fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port
            = htons(port); // use some unused port number
    servAddr.sin_addr.s_addr = INADDR_ANY;

    int connectStatus = connect(_fd, (struct sockaddr*)&servAddr,
                      sizeof(servAddr));

    if (connectStatus == -1) {
        throw SocketException("Unable to connect to client");
    }
}

tcp::Client::~Client() {
    close(_fd);
}

void tcp::Client::send(const std::vector<uint8_t>& data) const {
    ::send(_fd, &data[0], data.size(), 0);
}

std::vector<uint8_t> tcp::Client::recv(unsigned int size) const {
    std::vector<uint8_t> data(size);

    int receivedBytes = ::recv(_fd, &data[0], size, 0);
    if (receivedBytes != size) {
        throw SocketException("Unable to read " + std::to_string(size) + " bytes. Number of received bytes: " +
                                 std::to_string(receivedBytes));
    }
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
    std::memcpy(&bytes[0], &data, bytes.size());
    return bytes;
}

std::vector<uint8_t> packData(const Robot::CommandType& cmd) {
    auto cmd_int = static_cast <uint32_t> (cmd);
    return packData(cmd_int);
}

template <typename T>
T unpackData(const std::vector<uint8_t>& data) {
    T result;
    std::memcpy(&result, &data[0], sizeof(result));
    return result;
}

void Robot::move(CommandType cmd, const Transform& transform) {
    std::vector<uint8_t> bytes(INT_SIZE + TRANSFORM_SIZE);

    auto rawCmd = packData(cmd);
    auto rawTransform  = packData(transform);

    bytes.insert(bytes.begin(), rawCmd.begin(), rawCmd.end());
    bytes.insert(bytes.end(), rawTransform.begin(), rawTransform.end());
    _client->send(bytes);

    // Waiting for result
    auto success = unpackData<int>(_client->recv(INT_SIZE));

    if (!success) {
        throw RobotException("Execution failed");
    }
}

Transform Robot::getCurrentPosition() {
    std::vector<uint8_t> bytes(INT_SIZE);
    _client->send(packData(CommandType::CURRENT_POSITION));

    // Waiting for result
    auto result = _client->recv(INT_SIZE + TRANSFORM_SIZE);
    auto success = unpackData<int>(result);
    if (!success) {
        throw RobotException("Communication failed, Received incorrect bytes");
    }
    return unpackData<Transform>(std::vector<uint8_t>(result.begin() + INT_SIZE, result.end()));
}
