cmake_minimum_required(VERSION 3.3)
project(ex4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++0x -g -pthread")

set(SOURCE_FILES
        src/Taxi-Center/CommunicatorListener.cpp src/Taxi-Center/CommunicatorListener.h
        src/general/AvaliableListener.h src/general/AvaliableNotifier.h
        src/Driver/Driver.h src/Driver/Driver.cpp src/Navigation/Trip.cpp src/Navigation/Trip.h
        src/TaxiCenterFlow.h src/DriverFlow.h src/Taxi-Center/Comunicator.cpp src/Taxi-Center/Comunicator.h
        src/general/Passenger.cpp src/general/Passenger.h src/Taxi-Center/TaxiCenter.cpp src/Taxi-Center/TaxiCenter.h
        src/Driver/LuxTaxi.cpp src/Driver/LuxTaxi.h src/Navigation/Map.cpp src/Navigation/Map.h src/Navigation/Bfs.cpp src/Navigation/Bfs.h
        src/Driver/Taxi.cpp src/Driver/Taxi.h src/general/Parser.cpp src/general/Parser.h
        src/Driver/DriverInfo.cpp src/Driver/DriverInfo.h
        src/general/ThreadPool.cpp src/general/ThreadPool.h
        src/Navigation/Point.cpp src/Navigation/Point.h)

set(COMUNICATION_FILES
        src/comunication/TaxiCenterProtocol.cpp src/comunication/TaxiCenterProtocol.h
        src/comunication/Server.cpp src/comunication/Server.h
        src/comunication/Socket.cpp src/comunication/Protocol.h
        src/comunication/DriverProtocol.h src/comunication/DriverProtocol.cpp
        src/comunication/Client.h src/comunication/Client.cpp src/comunication/Tcp.cpp
        src/comunication/GuiProtocol.cpp src/comunication/GuiProtocol.h
        src/comunication/Tcp.h)

add_executable(client src/DriverFlow.cpp ${SOURCE_FILES} ${COMUNICATION_FILES})
add_executable(server src/TaxiCenterFlow.cpp ${SOURCE_FILES} ${COMUNICATION_FILES})
