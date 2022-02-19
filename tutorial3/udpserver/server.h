#include <netinet/in.h>
#ifndef SERVER_H
#define SERVER_H

#include <sys/queue.h>
#include <arpa/inet.h>

/*******************************************************************************
 * 
 * defines
 * 
 * ****************************************************************************/

#define PACK_SIZE 270			// maximum size of a packet string
#define BUFFER_SIZE 1024	
#define THREAD_POOL_SIZE 3

/*******************************************************************************
 * 
 * structs
 * 
 * ****************************************************************************/

struct Request{
	socklen_t fromlen;
	struct sockaddr_in from;
	char buffer[PACK_SIZE];
    STAILQ_ENTRY(Request) pointers;     //pointer toward the next item in the list
};

typedef struct Request request;
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;


/*******************************************************************************
 * 
 * functions
 * 
 * ****************************************************************************/
request * create_request();
SA_IN * create_in_addr(short af_inet, unsigned long ip, int port);
void handle_connection(request *req);
void * thread_function(void *arg);
void sig_handler(int signo);

#endif