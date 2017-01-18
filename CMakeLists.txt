cmake_minimum_required(VERSION 3.3)
project(ex4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++0x -g -pthread")

set(SOURCE_FILES
        src/CommunicatorListener.cpp src/CommunicatorListener.h
        src/AvaliableListener.h src/AvaliableNotifier.h
        src/Driver.h src/Driver.cpp src/Trip.cpp src/Trip.h
        TaxiCenterFlow.h DriverFlow.h src/Comunicator.cpp src/Comunicator.h
        src/Passenger.cpp src/Passenger.h src/TaxiCenter.cpp src/TaxiCenter.h
        src/LuxTaxi.cpp src/LuxTaxi.h src/Map.cpp src/Map.h src/Bfs.cpp src/Bfs.h
        src/Taxi.cpp src/Taxi.h src/Parser.cpp src/Parser.h
        src/DriverInfo.cpp src/DriverInfo.h)

set(COMUNICATION_FILES
        comunication/TaxiCenterProtocol.cpp comunication/TaxiCenterProtocol.h
        comunication/Server.cpp comunication/Server.h
        comunication/Socket.cpp comunication/Protocol.h
        comunication/DriverProtocol.h comunication/DriverProtocol.cpp
        comunication/Client.h comunication/Client.cpp comunication/Tcp.cpp
        comunication/Tcp.h)

add_executable(client DriverFlow.cpp ${SOURCE_FILES} ${COMUNICATION_FILES})
add_executable(server TaxiCenterFlow.cpp ${SOURCE_FILES} ${COMUNICATION_FILES})
