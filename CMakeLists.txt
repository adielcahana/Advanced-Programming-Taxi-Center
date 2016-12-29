cmake_minimum_required(VERSION 3.3)
project(ex4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(SOURCE_FILES src/Flow.cpp src/Flow.h
        src/Driver.h src/Trip.cpp src/Trip.h
        src/Passenger.cpp src/Passenger.h src/TaxiCenter.cpp src/TaxiCenter.h
        src/LuxTaxi.cpp src/LuxTaxi.h src/Map.cpp src/Map.h src/Bfs.cpp src/Bfs.h
        src/Taxi.cpp src/Taxi.h src/Parser.cpp src/Parser.h src/AvaliableNotifier.h src/AvaliableListener.h
        src/TripListener.cpp src/TripListener.h src/client.cpp src/client.h src/Protocol.h src/DriverProtocol.cpp src/DriverProtocol.h)

add_executable(client src/Driver.cpp ${SOURCE_FILES} ${TEST_FILES})
add_executable(server src/udp_server2.cpp)