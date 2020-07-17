#include "random.h"
#include <algorithm>

std::string Server::generate_random_sequence(int length) {
    const char charset[] = 
         "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    auto randchar = [charset]() -> char {
        const size_t max_index = (sizeof(charset) - 1);
        
        return charset[rand() % max_index];
    };

    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);

    return str;
}