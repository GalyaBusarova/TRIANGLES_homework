CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = triangles
TEST_TARGET = triangles_test

all: $(TARGET)

$(TARGET): triangles.cpp
	$(CXX) $(CXXFLAGS) triangles.cpp -o $(TARGET)

$(TEST_TARGET): tests.cpp geometry.hpp
	$(CXX) $(CXXFLAGS) tests.cpp -o $(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

run-file: $(TARGET)
	@echo "Запуск на файле: $(TEST)"
	./$(TARGET) < $(TEST)

tests: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all run test clean