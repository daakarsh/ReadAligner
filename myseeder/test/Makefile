release: clean
	$(CXX) -I include -std=c++14 \
	-O3 -DNDEBUG \
	-W -Wall -pedantic \
	-lrt -lpthread \
	-o main *.cpp

debug: clean
	$(CXX) -I include -std=c++14 \
	-g -O0 -DSEQAN_ENABLE_DEBUG=1 \
	-W -Wall -pedantic \
	-lrt -lpthread \
	-o main *.cpp

clean:
	rm -f main
