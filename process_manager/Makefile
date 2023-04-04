CC=g++
CFLAGS=-c
LDFLAGS=-pthread

# dirs
BINDIR=./bin
SRCDIR=.
TESTFILESDIR=./testfiles


SOURCES=$(SRCDIR)/functions.cpp $(SRCDIR)/process_manager.cpp $(SRCDIR)/procman.cpp
BINOBJ=$(BINDIR)/functions.o $(BINDIR)/process_manager.o $(BINDIR)/procman.o
EXECUTABLE=procman

all: prebuild clean $(SOURCES) $(EXECUTABLE)

prebuild:
	mkdir -p $(BINDIR)

$(EXECUTABLE): $(BINOBJ) 
	$(CC) $(LDFLAGS) $(BINOBJ) -o $(BINDIR)/$(EXECUTABLE)

$(BINDIR)/functions.o:
	$(CC) $(CFLAGS) $(SRCDIR)/functions.cpp -o $(BINDIR)/functions.o

$(BINDIR)/process_manager.o:
	$(CC) $(CFLAGS) $(SRCDIR)/process_manager.cpp -o $(BINDIR)/process_manager.o

$(BINDIR)/procman.o:
	$(CC) $(CFLAGS) $(SRCDIR)/procman.cpp -o $(BINDIR)/procman.o

clean:
	rm -rf $(BINDIR)/*.o $(BINDIR)/$(EXECUTABLE).exe

test:
	$(CC) $(SRCDIR)/test_sample.cpp -o $(BINDIR)/testapp -O3 -static; \
	$(CC) $(SRCDIR)/test_sample2.cpp -o $(BINDIR)/testapp2 -O3 -static

install: $(EXECUTABLE)
	cp $(BINDIR)/$(EXECUTABLE) /usr/local/bin/$(EXECUTABLE)

exec:
	$(BINDIR)/$(EXECUTABLE) --memory=8m --time=1s --app=$(BINDIR)/testapp.exe

exec2:
	$(BINDIR)/$(EXECUTABLE) --memory=8m --time=1s --app=$(BINDIR)/testapp2.exe \
	--input=input01.txt --output=output01.txt