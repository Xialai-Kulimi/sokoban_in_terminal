# the compiler: gcc for C program, define as g++ for C++
CC = g++
 
  # compiler flags:
  #  -g     - this flag adds debugging information to the executable file
  #  -Wall  - this flag is used to turn on most compiler warningsCFLAGS  = -g -Wall
CFLAGS =  -Wall
  # The build target 
LIBRARY = Block.cpp Map.cpp
TARGET = main.cpp
TEST_TARGET = test.cpp
  
run: $(TARGET)
	$(CC) $(TARGET) $(LIBRARY) $(CFLAGS) -o a.out
	./a.out
	rm a.out
test: $(TEST_TARGET)
	$(CC) $(TEST_TARGET) $(LIBRARY) $(CFLAGS) -o test.out
	./test.out
	rm test.out
