CC=g++
CFLAGS=-c
LDFLAGS=-pthread
SRCDIR=src/
BINDIR=bin/
SOURCES=$(addprefix $(SRCDIR),constants.cpp functions.cpp olymp-sandbox.cpp)
OBJECTS=$(addprefix $(BINDIR),$(notdir $(SOURCES:.cpp=.o)))
EXECUTABLE=$(BINDIR)olymp-sandbox

EXAMPLESDIR=examples/
TESTAPPDIR=$(EXAMPLESDIR)testapp/
TESTAPP=testapp
TESTFILESDIR=testfiles/

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

$(BINDIR)%.o: $(SRCDIR)%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BINDIR)*.o $(EXECUTABLE)

testapp:
	$(CC) $(TESTAPPDIR)$(TESTAPP).cpp -o $(BINDIR)$(TESTAPP) -static

test: testapp
	$(EXECUTABLE) --time=1s --memory=16m --app='$(BINDIR)/$(TESTAPP) < $(TESTFILESDIR)/input01.txt'
	$(EXECUTABLE) --time=1s --memory=16m --app='$(BINDIR)/$(TESTAPP) < $(TESTFILESDIR)/input02.txt'
	$(EXECUTABLE) --time=1s --memory=16m --app='$(BINDIR)/$(TESTAPP) < $(TESTFILESDIR)/input03.txt'
	$(EXECUTABLE) --time=1s --memory=16m --app='$(BINDIR)/$(TESTAPP) < $(TESTFILESDIR)/input04.txt'
	$(EXECUTABLE) --time=1s --memory=16m --app='$(BINDIR)/$(TESTAPP) < $(TESTFILESDIR)/input05.txt'