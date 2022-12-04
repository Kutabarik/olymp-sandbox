CC=g++
CFLAGS=-c
<<<<<<< HEAD
LDFLAGS=-pthread
SOURCES=olymp-sandbox.cpp
=======
LDFLAGS=
SOURCES=constants.cpp functions.cpp main.cpp
>>>>>>> 765bc5b213825c7fd178aaba8cf7bd034820eff4
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=olymp-sandbox

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)

test:
	./$(EXECUTABLE) -h

