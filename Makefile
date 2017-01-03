CC			= g++
LDFLAGS		= -pthread
SOURCES		= main.cpp module.cpp subscriber.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
TARGET		= test

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
