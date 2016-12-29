a.out: *.o
	g++ -g -Werror -o a.out main.o *.o

main.o: *.cpp
	g++ -g -Werror -c -o main.o *.cpp

clean:
	rm -f *.o a.out
