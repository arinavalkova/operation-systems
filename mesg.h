#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAXBUFF 80
#define PERM 0666

typedef struct our_msgbuf
{
	long mtype;
	char type_buff[MAXBUFF];
	char msg_buff[MAXBUFF];
} Message;

int send_message( int qid, Message *qbuf )
{
        int  result, length;
        length = sizeof(Message) - sizeof(long);

        if ((result = msgsnd( qid, qbuf, length, 0)) == -1)
        {
                return(-1);
        }

        return(result);
}

