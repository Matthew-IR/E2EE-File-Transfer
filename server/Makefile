CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = main.cpp
TARGET = server.o

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean