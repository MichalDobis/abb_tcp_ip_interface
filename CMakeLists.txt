cmake_minimum_required(VERSION 3.16)
project(abb_tcp_ip_interface VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)


add_library(${PROJECT_NAME} SHARED src/RobotTcpIp.cpp)

set(INSTALL_INCLUDE_DIR include)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
        )


add_executable(${PROJECT_NAME}_example src/main.cpp)
target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME})
#target_link_libraries(${PROJECT_NAME} INTERFACE ${LOG4CXX_LIBRARIES})

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE_DIR})

