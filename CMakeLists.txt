cmake_minimum_required(VERSION 3.3)
project(ex4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++0x -g -pthread")

set(SOURCE_FILES
        cpp/src/Taxi-Center/CommunicatorListener.cpp cpp/src/Taxi-Center/CommunicatorListener.h
        cpp/src/general/AvaliableListener.h cpp/src/general/AvaliableNotifier.h
        cpp/src/Driver/Driver.h cpp/src/Driver/Driver.cpp cpp/src/Navigation/Trip.cpp cpp/src/Navigation/Trip.h
        cpp/src/TaxiCenterFlow.h cpp/src/DriverFlow.h cpp/src/Taxi-Center/Comunicator.cpp cpp/src/Taxi-Center/Comunicator.h
        cpp/src/general/Passenger.cpp cpp/src/general/Passenger.h cpp/src/Taxi-Center/TaxiCenter.cpp cpp/src/Taxi-Center/TaxiCenter.h
        cpp/src/Driver/LuxTaxi.cpp cpp/src/Driver/LuxTaxi.h cpp/src/Navigation/Map.cpp cpp/src/Navigation/Map.h cpp/src/Navigation/Bfs.cpp cpp/src/Navigation/Bfs.h
        cpp/src/Driver/Taxi.cpp cpp/src/Driver/Taxi.h cpp/src/general/Parser.cpp cpp/src/general/Parser.h
        cpp/src/Driver/DriverInfo.cpp cpp/src/Driver/DriverInfo.h
        cpp/src/general/ThreadPool.cpp cpp/src/general/ThreadPool.h
        cpp/src/Navigation/Point.cpp cpp/src/Navigation/Point.h)

set(COMUNICATION_FILES
        cpp/src/comunication/TaxiCenterProtocol.cpp cpp/src/comunication/TaxiCenterProtocol.h
        cpp/src/comunication/Server.cpp cpp/src/comunication/Server.h
        cpp/src/comunication/Socket.cpp cpp/src/comunication/Protocol.h
        cpp/src/comunication/DriverProtocol.h cpp/src/comunication/DriverProtocol.cpp
        cpp/src/comunication/Client.h cpp/src/comunication/Client.cpp cpp/src/comunication/Tcp.cpp
        cpp/src/comunication/GuiProtocol.cpp cpp/src/comunication/GuiProtocol.h
        cpp/src/comunication/Tcp.h)

add_executable(client cpp/src/DriverFlow.cpp ${SOURCE_FILES} ${COMUNICATION_FILES})
add_executable(server cpp/src/TaxiCenterFlow.cpp ${SOURCE_FILES} ${COMUNICATION_FILES})
