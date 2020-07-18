#include "serializer.h"

void Server::serialize(Message* msg, std::string& data) {
    data.append(std::to_string(msg->messageId));
    data += SEP;
    data.append(std::to_string(msg->priority));
    data += SEP;
    data.append(msg->message);
}