#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "mesg.h"

int main() {
        Message message;
        key_t key;
        int msgid, length, n;

        if ((key = ftok("server.c", 'A')) < 0) {
                fprintf(stderr, "Error: ftok() failed with %s\n", strerror(errno));
		exit(1);
        }

        message.mtype = 1L;

        if ((msgid = msgget(key, PERM | IPC_CREAT)) < 0) {
                fprintf(stderr, "Error: msgget() failed with %s\n", strerror(errno));
		exit(1);
        }
	
	while (1) {
		n = msgrcv(msgid, &message, sizeof(message) - sizeof(long), message.mtype, 0);
		if (n > 0) {
			printf("%s", message.msg_buff);
			if (strcmp(message.type_buff, "M\n") == 0) {
				if ((length = sprintf(message.msg_buff, "Alya\n")) < 0) {
                        		fprintf(stderr, "Error: sprintf() failed with %s\n", strerror(errno));
                        		exit(1);
                		}

                 		if ((length = sprintf(message.type_buff, "F\n")) < 0) {
                        		fprintf(stderr, "Error: sprintf() failed with %s\n", strerror(errno));
                        		exit(1);
                		}
				printf("Sending Alya female name\n");
			} else if (strcmp(message.type_buff, "F\n") == 0) {
				 if ((length = sprintf(message.msg_buff, "Denis\n")) < 0) {
                                        fprintf(stderr, "Error: sprintf() failed with %s\n", strerror(errno));
                                        exit(1);
                                }

                                if ((length = sprintf(message.type_buff, "M\n")) < 0) {
                                        fprintf(stderr, "Error: sprintf() failed with %s\n", strerror(errno));
                                        exit(1);
                                }
				printf("Sending Denis male name\n");
			} else {
				 if ((length = sprintf(message.msg_buff, "BYE\n")) < 0) {
                                        fprintf(stderr, "Error: sprintf() failed with %s\n", strerror(errno));
                                        exit(1);
                                }

                                if ((length = sprintf(message.type_buff, "BYE\n")) < 0) {
                                        fprintf(stderr, "Error: sprintf() failed with %s\n", strerror(errno));
                                        delete_queue(msgid);
                                        exit(1);
                                }
				printf("Sending BYE\n");
			}

			if (send_message(msgid, (void*) &message) != 0) {
                        	fprintf(stderr, "Error: msgsnd() failed with %s\n", strerror(errno));
                        	exit(1);
               		 }

			if (strcmp(message.msg_buff, "BYE\n") == 0) break;
		} else {
			fprintf(stderr, "Error: msgrcv() failed with %s", strerror(errno));
			exit(1);
		}
	}
	exit(0);
}

