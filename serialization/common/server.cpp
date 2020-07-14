#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include "message.h"


using namespace std;

void deserialize(char* data, Message* msg);
void serialize(Message* msg, char* data);


int main() {
    srand(time(NULL));

    Message* msg = new Message;
    msg->priority = 1;
    msg->messageId = 90;
    strcpy(msg->message, "Message from the tcp server.");

    char packet[PACKET_SIZE];

    serialize(msg, packet);

    Message* des = new Message;
    deserialize(packet, des);

}


void serialize(Message* msg, char* data)
{
    memcpy(data, msg, sizeof(msg));
}

void deserialize(char* data, Message* msg)
{
    memcpy(msg, data, sizeof(data));

    cout << msg->message << endl;
    cout << msg->messageId << endl;
    cout << msg->priority << endl;
}
