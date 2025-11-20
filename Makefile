CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
TARGET = erp_system
SOURCES = main.cpp
HEADERS = Student.h StudentRegistry.h CSVReader.h

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run

