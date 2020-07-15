#define MESSAGE_SIZE 512
#define SEP '_'

typedef struct Message {
    int messageId;
    int priority;
    char message[MESSAGE_SIZE];

} Message;