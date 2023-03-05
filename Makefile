CC=g++
CFLAGS=-c -D_WIN32
LDFLAGS=-pthread

# dirs
BINDIR=./bin
SRCDIR=./src
TESTFILESDIR=./testfiles


SOURCES=$(SRCDIR)/constants.cpp $(SRCDIR)/functions.cpp $(SRCDIR)/olymp-sandbox.cpp
BINOBJ=$(BINDIR)/constants.o $(BINDIR)/functions.o $(BINDIR)/olymp-sandbox.o
EXECUTABLE=olymp-sandbox

all: prebuild $(SOURCES) $(EXECUTABLE)

prebuild:
	mkdir -p $(BINDIR)

$(EXECUTABLE): $(BINOBJ) 
	$(CC) $(LDFLAGS) $(BINOBJ) -o $(BINDIR)/$(EXECUTABLE)

$(BINDIR)/constants.o:
	$(CC) $(CFLAGS) $(SRCDIR)/constants.cpp -o $(BINDIR)/constants.o

$(BINDIR)/functions.o:
	$(CC) $(CFLAGS) $(SRCDIR)/functions.cpp -o $(BINDIR)/functions.o

$(BINDIR)/olymp-sandbox.o:
	$(CC) $(CFLAGS) $(SRCDIR)/olymp-sandbox.cpp -o $(BINDIR)/olymp-sandbox.o

clean:
	rm -rf $(BINDIR)/*.o $(BINDIR)/*.exe

test:
	$(CC) $(SRCDIR)/test.cpp -o $(BINDIR)/testapp; \
	$(BINDIR)/testapp < $(TESTFILESDIR)/input01.txt

testos:
	$(CC) $(SRCDIR)/testos.cpp -o $(BINDIR)/testos; \
	$(BINDIR)/testos