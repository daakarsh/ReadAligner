main: clean
	$(CXX) -I include -std=c++14 \
	-O3 -DNDEBUG -W -Wall -pedantic \
	-o main main.cpp

clean:
	rm -f main