#ifndef LOG
#define LOG

#include <iostream>

#define LOG(message) (std::cout << "[INFO]: " << message << std::endl)
#define WARNING(message) (std::cout << "[WARNING]: " << message << std::endl)
#define ERROR(message) (std::cout << "[ERROR]: " << message << std::endl)

#endif
