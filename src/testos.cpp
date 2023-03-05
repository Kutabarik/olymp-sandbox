#include <iostream>

int main() {

#if defined(_WIN32)
    std::cout << "windows" << std::endl;
#endif

#if defined(__linux__)
    std::cout << "linux" << std::endl;
#endif

#if defined(__unix__)
    std::cout << "unix" << std::endl;
#endif

#if defined(__unix)
    std::cout << "unix short" << std::endl;
#endif

    return 0;
}