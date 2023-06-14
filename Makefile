OS=windows
CC=g++
CFLAGS=-c
LDFLAGS=-pthread

# dirs
BINDIR=./bin
OBJDIR=./obj
SRCDIR=./src/sandbox
TESTFILESDIR=./testfiles
EXAMPLESDIR=./examples
EXAMPLEAPP=testapp

SOURCE1=${OS}
SOURCE2=config
SOURCE3=definitions
SOURCE4=logger
SOURCE5=process_manager
SOURCE6=result_info
SOURCE7=olymp-sandbox

SOURCES=
SOURCES:=$(SOURCES) $(SRCDIR)/$(SOURCE1).cpp
SOURCES:=$(SOURCES) $(SRCDIR)/$(SOURCE2).cpp
SOURCES:=$(SOURCES) $(SRCDIR)/$(SOURCE3).cpp
SOURCES:=$(SOURCES) $(SRCDIR)/$(SOURCE4).cpp
SOURCES:=$(SOURCES) $(SRCDIR)/$(SOURCE5).cpp
SOURCES:=$(SOURCES) $(SRCDIR)/$(SOURCE6).cpp
SOURCES:=$(SOURCES) $(SRCDIR)/$(SOURCE7).cpp

BINOBJ=
BINOBJ:=${BINOBJ} $(OBJDIR)/$(SOURCE1).o
BINOBJ:=${BINOBJ} $(OBJDIR)/$(SOURCE2).o
BINOBJ:=${BINOBJ} $(OBJDIR)/$(SOURCE3).o
BINOBJ:=${BINOBJ} $(OBJDIR)/$(SOURCE4).o
BINOBJ:=${BINOBJ} $(OBJDIR)/$(SOURCE5).o
BINOBJ:=${BINOBJ} $(OBJDIR)/$(SOURCE6).o
BINOBJ:=${BINOBJ} $(OBJDIR)/$(SOURCE7).o
EXECUTABLE=olymp-sandbox

help:
	@echo "make all"
	@echo "make build"
	@echo "make clean"
	@echo "make install"
	@echo "make test"
	@echo "make testos"

all: prebuild build testapp

prebuild:
	mkdir -p $(BINDIR) $(OBJDIR)

build: $(EXECUTABLE)

$(EXECUTABLE): $(BINOBJ) 
	$(CC) $(LDFLAGS) $(BINOBJ) -o $(BINDIR)/$(EXECUTABLE)

$(OBJDIR)/$(SOURCE1).o:
	$(CC) $(CFLAGS) $(SRCDIR)/$(SOURCE1).cpp -o $(OBJDIR)/$(SOURCE1).o

$(OBJDIR)/$(SOURCE2).o:
	$(CC) $(CFLAGS) $(SRCDIR)/$(SOURCE2).cpp -o $(OBJDIR)/$(SOURCE2).o

$(OBJDIR)/$(SOURCE3).o:
	$(CC) $(CFLAGS) $(SRCDIR)/$(SOURCE3).cpp -o $(OBJDIR)/$(SOURCE3).o

$(OBJDIR)/$(SOURCE4).o:
	$(CC) $(CFLAGS) $(SRCDIR)/$(SOURCE4).cpp -o $(OBJDIR)/$(SOURCE4).o

$(OBJDIR)/$(SOURCE5).o:
	$(CC) $(CFLAGS) $(SRCDIR)/$(SOURCE5).cpp -o $(OBJDIR)/$(SOURCE5).o

$(OBJDIR)/$(SOURCE6).o:
	$(CC) $(CFLAGS) $(SRCDIR)/$(SOURCE6).cpp -o $(OBJDIR)/$(SOURCE6).o

$(OBJDIR)/$(SOURCE7).o:
	$(CC) $(CFLAGS) $(SRCDIR)/$(SOURCE7).cpp -o $(OBJDIR)/$(SOURCE7).o

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*.exe

testapp:
	$(CC) $(EXAMPLESDIR)/$(EXAMPLEAPP)/$(EXAMPLEAPP).cpp -o $(BINDIR)/$(EXAMPLEAPP);

test: testapp
	$(BINDIR)/$(EXECUTABLE) --app=$(BINDIR)/$(EXAMPLEAPP) --memory=16m --time=1s --input=$(TESTFILESDIR)/input01.txt

install: $(EXECUTABLE)
	cp $(BINDIR)/$(EXECUTABLE) /usr/local/bin/$(EXECUTABLE)
