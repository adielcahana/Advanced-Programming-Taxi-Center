all:

	g++ src/DriverFlow.cpp *src/Driver/*.cpp *src/Navigation/*.cpp *src/comunication/*.cpp *src/general/*.cpp -pthread -std=c++0x -g -o client.out

	g++ src/TaxiCenterFlow.cpp *src/Taxi-Center/*.cpp *src/Driver/*.cpp *src/Navigation/*.cpp *src/comunication/*.cpp *src/general/*.cpp -pthread -std=c++0x -g -o server.out

