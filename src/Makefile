CC=g++
CFLAGS=-c
LDFLAGS=-pthread
SOURCES=constants.cpp functions.cpp olymp-sandbox.cpp
OBJECTS=$(SOURCES:.cpp=.o)
BINOBJ=$(BINDIR)constants.o $(BINDIR)functions.o $(BINDIR)olymp-sandbox.o
EXECUTABLE=olymp-sandbox
BINDIR=../bin/
TESTFILESDIR=../testfiles/

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(BINOBJ) -o $(BINDIR)$@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $(BINDIR)$@

clean:
	rm -rf $(BINDIR)*.o $(EXECUTABLE)

test:
	$(CC) test.cpp -o $(BINDIR)/testapp
	$(BINDIR)/testapp < $(TESTFILESDIR)/input01.txt
	$(BINDIR)/testapp < $(TESTFILESDIR)/input02.txt
	$(BINDIR)/testapp < $(TESTFILESDIR)/input03.txt
	$(BINDIR)/testapp < $(TESTFILESDIR)/input04.txt
	$(BINDIR)/testapp < $(TESTFILESDIR)/input05.txt
