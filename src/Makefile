CC=g++
CFLAGS=-c
LDFLAGS=-pthread
SOURCES=constants.cpp functions.cpp olymp-sandbox.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=olymp-sandbox
BINDIR=../bin/
TESTFILESDIR=../testfiles/

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)

test:
	$(CC) test.cpp -o $(BINDIR)/testapp
	$(BINDIR)/testapp < $(TESTFILESDIR)/input01.txt
