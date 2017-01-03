all:

	g++ DriverFlow.cpp DriverFlow.h *src/*.cpp *src/*.h *comunication/*.cpp *comunication/*.h -std=c++0x -g -o client.out

	g++ TaxiCenterFlow.cpp TaxiCenterFlow.h *src/*.cpp *src/*.h *comunication/*.cpp *comunication/*.h -std=c++0x -g -o server.out