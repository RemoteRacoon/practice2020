#define SEQ_LEN 2048
#define SEP '_'

typedef struct Message {
    int messageId;
    int priority;
    char* message[SEQ_LEN];

} Message;