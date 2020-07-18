#include "deserializer.h"

void Client::deserialize(std::string& data, Message* msg) {
    size_t pos = 0;
    std::string token;

    pos = data.find(SEP);

    if (pos != std::string::npos) {
        token = data.substr(0, pos);
        msg->messageId = atoi(token.c_str());
        data.erase(0, pos + 1);
    }

    pos = data.find(SEP);

    if (pos != std::string::npos) {
        token = data.substr(0, pos);
        msg->priority = atoi(token.c_str());
        data.erase(0, pos + 1);
    }

    msg->message = data;
}