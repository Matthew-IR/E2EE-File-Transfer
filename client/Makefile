CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = main.cpp key_exchange.cpp
TARGET = client.o

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean