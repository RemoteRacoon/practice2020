#include <string>
#include <iostream>

#define SEQ_LEN 2048
#define SEP '_'

#ifndef MESSAGE_H
#define MESSAGE_H
typedef struct Message {
    int messageId;
    int priority;
    std::string message;

    Message() {
        message.resize(SEQ_LEN * SEQ_LEN);
    }

    void append(std::string& str) {
        this->message.append(str);
    }

} Message;


#endif
