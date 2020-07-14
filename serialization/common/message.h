#define MESSAGE_SIZE 512
#define PACKET_SIZE sizeof(Message)
#include <stdlib.h>

typedef struct Message {
    int messageId;
    int priority;
    char message[MESSAGE_SIZE];
} Message;