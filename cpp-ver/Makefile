all:
	g++ -std=c++17 -g -msse2 *.cpp -o test_bt.out
	./test_bt.out

sanitize:
	g++ -std=c++17 -fsanitize=address -g -msse2 *.cpp -o test_bt.out
	./test_bt.out

test:
	g++ -std=c++17 -msse2 *.cpp -o test_bt.out
	./test_bt.out test > results.csv

fmt:
	clang-format *.cpp -i

clean:
	rm -rf *.o *.out
