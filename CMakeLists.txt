cmake_minimum_required(VERSION 3.0.0)
project(server)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(TCPServer
            Source.cpp
            # server
            server/TCPServer.cpp
            server/TCPServer.h
            server/ParseRequest.cpp
            server/ParseRequest.h
            server/HTTPResponse.cpp
            server/HTTPResponse.h
            server/Consts.h
            # utils
            utils/Log.cpp
            utils/Log.h
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_link_libraries(TCPServer)
