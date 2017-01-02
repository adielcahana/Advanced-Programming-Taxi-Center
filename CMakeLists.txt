cmake_minimum_required(VERSION 3.3)
project(ex4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lboost_serialization")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(BOOST_ROOT CACHE PATH "/usr/include/boost/")
find_package(Boost 1.58.0 COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES
        src/Driver.h src/Driver.cpp src/Trip.cpp src/Trip.h
        src/TaxiCenterFlow.h src/DriverFlow.h
        src/Passenger.cpp src/Passenger.h src/TaxiCenter.cpp src/TaxiCenter.h
        src/LuxTaxi.cpp src/LuxTaxi.h src/Map.cpp src/Map.h src/Bfs.cpp src/Bfs.h
        src/Taxi.cpp src/Taxi.h src/Parser.cpp src/Parser.h src/AvaliableNotifier.h
        src/AvaliableListener.h src/TripListener.cpp src/TripListener.h
        src/DriverInfo.cpp src/DriverInfo.h)

set(COMUNICATION_FILES
        comunication/TaxiCenterProtocol.cpp comunication/TaxiCenterProtocol.h
        comunication/Server.cpp comunication/Server.h comunication/Udp.h
        comunication/Udp.cpp comunication/Socket.cpp comunication/Protocol.h
        comunication/DriverProtocol.h comunication/DriverProtocol.cpp
        comunication/Client.h comunication/Client.cpp)


add_executable(client src/DriverFlow.cpp ${SOURCE_FILES} ${COMUNICATION_FILES})
target_link_libraries(client ${Boost_LIBRARIES})
add_executable(server src/TaxiCenterFlow.cpp ${SOURCE_FILES} ${COMUNICATION_FILES})
target_link_libraries(server ${Boost_LIBRARIES})
add_executable(main src/main.cpp ${SOURCE_FILES} ${COMUNICATION_FILES})
target_link_libraries(main ${Boost_LIBRARIES})