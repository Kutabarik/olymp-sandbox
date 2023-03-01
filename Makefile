CC=g++
CFLAGS=-c
LDFLAGS=-pthread
SRCDIR=src/
BINDIR=bin/
TESTFILESDIR=testfiles/
SOURCES=$(addprefix $(SRCDIR),constants.cpp functions.cpp olymp-sandbox.cpp)
OBJECTS=$(addprefix $(BINDIR),$(notdir $(SOURCES:.cpp=.o)))
EXECUTABLE=$(BINDIR)olymp-sandbox

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

$(BINDIR)%.o: $(SRCDIR)%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BINDIR)*.o $(EXECUTABLE)

test:
	$(CC) $(SRCDIR)test.cpp -o $(BINDIR)testapp
	$(BINDIR)/testapp < $(TESTFILESDIR)/input01.txt
	$(BINDIR)/testapp < $(TESTFILESDIR)/input02.txt
	$(BINDIR)/testapp < $(TESTFILESDIR)/input03.txt
	$(BINDIR)/testapp < $(TESTFILESDIR)/input04.txt
	$(BINDIR)/testapp < $(TESTFILESDIR)/input05.txt
