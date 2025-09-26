CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = triangles

all: $(TARGET)

$(TARGET): geometry.cpp triangles.cpp
	$(CXX) $(CXXFLAGS) geometry.cpp triangles.cpp -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean