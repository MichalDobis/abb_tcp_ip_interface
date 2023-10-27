#ifndef ROBOT_TCP_IP_DATATYPES_H
#define ROBOT_TCP_IP_DATATYPES_H

struct Position {
    float x;
    float y;
    float z;
public:
    Position() = default;
    Position(float x, float y, float z):  x(x), y(y), z(z) {}
};

struct Quaternion {
    float x;
    float y;
    float z;
    float w;
    Quaternion() = default;
    Quaternion(float x, float y, float z, float w):  x(x), y(y), z(z), w(w) {}
};

struct Transform {
    Position position;
    Quaternion orientation;
public:
    Transform() = default;
    Transform(const Position& p, const Quaternion& q) : position(p), orientation(q) {}
};

#endif //ROBOT_TCP_IP_DATATYPES_H
