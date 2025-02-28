CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = firewall_optimizer
SRC = firewall_optimizer.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean up
clean:
	rm -f $(TARGET)


