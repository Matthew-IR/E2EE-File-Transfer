CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRCS = main.cpp key_exchange.cpp ../common/file_transfer.cpp
TARGET = client

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean