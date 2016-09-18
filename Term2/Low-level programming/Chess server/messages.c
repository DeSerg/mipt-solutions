#include "messages.h"

MessageType composeMessage(enum Type type, size_t size, void* data) 
{
//    printf("\n\n**Composing message**\n");
    fflush(stdout);
    
	MessageType m;
 	m.type = type;
 	m.size = size;
    m.data = NULL;
 	if (size > 0)
    {
//        printf("**Before allocating**\n");
        fflush(stdout);
        m.data = realloc(m.data, size);
//        printf("**After allocating**\n");
        fflush(stdout);
        
        memcpy(m.data, data, size);
    }
    
//    printf("**Composing finished**\n\n");
    fflush(stdout);
    
    return m;
}

int sendMessage (int sd, MessageType* m)
{
//    printf("\n\n**Sending message**\n");
    fflush(stdout);
    
 	if (send(sd, m, sizeof(MessageType), 0) == -1) 
 	{
    	perror("Error while sending message!");
    	exit (EXIT_FAILURE);
  	}
    else
    if (m->size > 0  &&  send(sd, m->data, m->size, 0) == -1)
    {
        perror("Error while sending message!");
    	exit (EXIT_FAILURE);        
    }
        
//    printf("**Sending finished**\n\n");
    fflush(stdout);
    
    return 0;
}

//Получение сообщения
int getMessage(int sd, MessageType* m) 
{
//    printf("\n\n**Receiving message**\n");
    fflush(stdout);
    
 	if (recv(sd, m, sizeof(MessageType), 0) == -1) 
 	{
    	perror("Error while recieving message!");
    	exit (EXIT_FAILURE);
  	}
    m->data = malloc(m->size);
    if (m->size > 0  &&  recv(sd, m->data, m->size, 0) == -1)
    {
        perror("Error while recieving message!\n");
        exit(EXIT_FAILURE);
    }    
    
//    printf("**Receiving finished**\n\n");
    fflush(stdout);
  	return 0;
}

void sendText(int sd, char *str)
{
    MessageType m = composeMessage(text, strlen(str), str);
    sendMessage(sd, &m);
}


void clearMessage(MessageType *m)
{
//    printf("\n\n**Clearing message**\n");
    if (m->size > 0)
        free(m->data);
//    printf("**Clearing finished**\n");
}
