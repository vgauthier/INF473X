/*******************************************************************************
 * 
 * authors: Vincent Gauthier <vincent.gauthier@telecom-sudparis.eu>
 * description: UDP Echo Server with a thread pool to handle the incomming packets
 * notes: Jacob Sorber has done a set of tutorials on how to use the pthread
 *        at the followin address:
 *        https://www.youtube.com/playlist?list=PL9IEJIKnBJjFZxuqyJ9JqVYmuFZHr7CFM
 * 
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/queue.h>
#include <signal.h>

#include "server.h"
#include "utils.h"

// socket descriptor
int sockfd;

// requests queue
// for more information about the queue in sys/queue.h 
// see: http://infnis.wikidot.com/list-from-sys-queue-h
STAILQ_HEAD(request_queue, Request) requests;

// typedefs
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

// thread pool is a global variable
pthread_t thr[THREAD_POOL_SIZE];
pthread_mutex_t mutex_queue = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_queue = PTHREAD_COND_INITIALIZER;

/**
 * @brief Main function
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]) {
    SA_IN *servaddr;
    int port, bytes_read;

    // init the signal handler 
    signal(SIGINT, sig_handler);

    // init the queue
    STAILQ_INIT(&requests);

    // parse the parameter
    if(argc == 2){
        sscanf(argv[1], "%d", &port);
        printf("UDP echo server will listen on port: %d\n", port);
    }else{
        puts("server PORT");
        exit(EXIT_FAILURE);
    }

    //spin the threads
    for(int i=0; i < THREAD_POOL_SIZE; i++){
        check(
            pthread_create(&thr[i], NULL, thread_function, NULL), 
            "Pthread_create Failled"
        );
    }

    //open the socket
    check((sockfd = socket(AF_INET, SOCK_DGRAM, 0)), "Socket Creation Failed");
       
    // Filling server information
    servaddr = create_in_addr(AF_INET, INADDR_ANY, port);

    // Bind the socket with the server address
    check(bind(sockfd, (SA *)servaddr, sizeof(SA)), "Socket Bind Failed");

    while (1) {
        // create a request struct (see server.h for more information)
        request *req = create_request();

        // wait until you receive a UDP packet 
        bytes_read = recvfrom(sockfd, 
                                (char *)req->buffer, 
                                PACK_SIZE, 
                                MSG_WAITALL, 
                                (SA *) &req->from, 
                                &req->fromlen);

        check(bytes_read, "Recvfrom Error");
        req->buffer[bytes_read] = '\0';

        pthread_mutex_lock(&mutex_queue);
        // put the request in the queue
        STAILQ_INSERT_TAIL(&requests, req, pointers);
        // send a signal to advertise that there is a new packet arrived
        pthread_cond_signal(&cond_queue);
        pthread_mutex_unlock(&mutex_queue);
    }
}


/**
 * @brief Create a request object
 * @return *requet
 */
request * create_request(){
    request *req = (request *)malloc(sizeof(request));
	req->fromlen = sizeof(SA);
    return req;
}

/**
 * @brief Create a in addr object
 * 
 * @param af_inet 
 * @param ip 
 * @param port 
 * @return struct sockaddr_in
 */
SA_IN * create_in_addr(short af_inet, unsigned long ip, int port){
    SA_IN *inaddr = (SA_IN *)malloc(sizeof(SA_IN));
    inaddr->sin_family         = af_inet;          
    inaddr->sin_addr.s_addr    = ip;
    inaddr->sin_port           = htons(port);
    return inaddr;
}

/**
 * @brief thread_function: handle the thread pool
 */
void * thread_function(void *arg){
    request *req = NULL;
    while (1) {
        pthread_mutex_lock(&mutex_queue);
        // check if the queue is not empty
        if(!STAILQ_EMPTY(&requests)) {
            // dequeue the first elem of the list
            req = STAILQ_FIRST(&requests);        
            STAILQ_REMOVE_HEAD(&requests, pointers);
            printf("dequeue a request!!\n");
        } else {
            // if empty wait until there is a new request in the queue
            pthread_cond_wait(&cond_queue, &mutex_queue);
        }
        pthread_mutex_unlock(&mutex_queue);
        
        // handle the reply to the input UDP packet
        if(req != NULL){
            handle_connection((void *)req);
            req = NULL;
        }
    }
}

/**
 * @brief handle_connection
 * 
 * @param reqest struct (see server.h for more information about the struct)
 */
void handle_connection(request *req){
    char buffer[BUFFER_SIZE];

     // simulate a long process
    sleep(3);
    //

    // create the reply
    strcpy(buffer, "echo-> ");
    strcat(buffer, req->buffer);
    printf("server send back a UDP packet: %s\n", buffer);

    // send the echo packet 
    int ret = sendto(sockfd, 
                (const char *)buffer, 
                strlen(buffer), 
                0, 
                (SA *)&req->from, 
                req->fromlen);
    check(ret, "Sendto Failled");

    // don't forget to free the request struct
    free(req);
}

/**
 * @brief sig_handler
 * 
 * @param signo 
 */
void sig_handler(int signo)
{
    if (signo == SIGINT){
        puts("\nSINGINT Received: Exit The Server");
        for(int i=0; i<THREAD_POOL_SIZE; i++){
            pthread_kill(thr[i], SIGTERM);
        }
        exit(EXIT_SUCCESS);
    }
}