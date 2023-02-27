# olymp-sandbox

## About
This project is a program for organizing programming Olympiads. This program allows participants to send their solutions as files to the server, where a PHP script compiles them (if necessary) into an executable file.

Next, the C++ program runs the executable file in a separate thread and runs it through predefined tests. Each test verifies the correctness of the input and output data under the constraints set by the condition in execution time and memory consumption.

The results of testing participants' solutions are stored in a database, and participants can see their results on a web page.

This program is designed to organize programming Olympiads and helps automate the process of testing participants' solutions. It allows you to save time and effort of the organizers, and also ensures the accuracy and reliability of testing.

## Project requirements
- gcc version 12.2
- PHP 8.1.
- make
- ...

## How to install
```
git clone https://github.com/Kutabarik/olymp-sandbox.git

cd olymp-sandbox

make
```

## How to use
run tests
```
make test 
```

delete all bin files
```
make clean 
```

