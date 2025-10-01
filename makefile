CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = triangles
TEST_TARGET = triangles_test

all: $(TARGET)

$(TARGET): triangles.cpp geometry.cpp
	$(CXX) $(CXXFLAGS) triangles.cpp geometry.cpp -o $(TARGET)

$(TEST_TARGET): tests.cpp geometry.cpp geometry.hpp
	$(CXX) $(CXXFLAGS) tests.cpp geometry.cpp -o $(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

tests: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all run test clean