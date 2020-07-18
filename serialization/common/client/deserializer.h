#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "../message.h"

namespace Client {
    void deserialize(std::string& data, Message* msg);
}

#endif