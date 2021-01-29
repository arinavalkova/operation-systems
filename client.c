#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "mesg.h"
#define BUFF_SIZE 1024

void delete_queue(int msgid) {
	if (msgctl(msgid, IPC_RMID, 0) < 0) {
		fprintf(stderr, "Error: msgctl() failed with %s\n", strerror(errno));
	}
}

int main() {
	Message message;
	key_t key;
	int msgid, length, n;
	char type_buffer[BUFF_SIZE];
	char msg_buffer[BUFF_SIZE];

	message.mtype = 1L;

	if ((key = ftok("server.c", 'A')) < 0) {
		fprintf(stderr, "Error: ftok() failed with %s\n", strerror(errno));
		delete_queue(msgid);
		exit(1);
	}

	if ((msgid = msgget(key, 0)) < 0) {
		fprintf(stderr, "Error: msgget() failed with %s\n", strerror(errno));
		delete_queue(msgid);
		exit(1);
	}
	while (1) {
		fgets(type_buffer, BUFF_SIZE, stdin);
		fgets(msg_buffer, BUFF_SIZE, stdin);
		if ((length = sprintf(message.msg_buff, msg_buffer)) < 0) {
			fprintf(stderr, "Error: sprintf() failed with %s\n", strerror(errno));
			delete_queue(msgid);
			exit(1);
		}

		 if ((length = sprintf(message.type_buff, type_buffer)) < 0) {
                        fprintf(stderr, "Error: sprintf() failed with %s\n", strerror(errno));
                        delete_queue(msgid);
                        exit(1);
                }

		if (send_message(msgid, (void*) &message) != 0) {
			fprintf(stderr, "Error: msgsnd() failed with %s\n", strerror(errno));
			delete_queue(msgid);
			exit(1);
		}
		
		n = msgrcv(msgid, &message, sizeof(message) - sizeof(long), message.mtype, 0);
                if (n > 0) {
			if (strcmp(message.type_buff, "BYE\n") != 0) {
                        	printf("%s%s", message.type_buff, message.msg_buff);
			}
		} else {
                        fprintf(stderr, "Error: msgrcv() failed with %s", strerror(errno));
			delete_queue(msgid);
                        exit(1);
                }
		
		if (strcmp(type_buffer, "BYE\n") == 0) break;
	}
	delete_queue(msgid);
	exit(0);
}
