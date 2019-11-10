/*
 * A Simple Echo Server
 * 
 * ./server port_no
 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<error.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define ERROR -1
#define MAX_CLIENTS 2
#define MAX_DATA 1024 //size of buffer

main(int args, char **argv)
{
    struct sockaddr_in server; //contains port number to which the server will be bound locally
    struct sockaddr_in client; //will contain any client information connection to us

    int sockdf;  //server socket descriptor
    int new;     //client socket descriptor
    int sockaddr_len = sizeof(struct sockaddr_in);  //this size is required for "bind" and "accept" calls
    int data_len;
    char data[MAX_DATA];

    //make a socket
    if((sockdf = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
    {
        perror("servert socket: ");
        exit(-1);
    }

    //fill the server data structure
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = INADDR_ANY; 
    bzero(&server.sin_zero, 8);

    //bind to the port
    if((bind(sockdf, (struct sockaddr *)&server, sockaddr_len)) == ERROR)
    {
        perror("bind : ");
        exit(-1);
    }

    //listen to the socket for maximum client MAX_CLIENT
    if((listen(sockdf, MAX_CLIENTS)) == ERROR)
    {
        perror("listen");
        exit(-1);
    }

    //in this loop we wait for a client, get a client connection and start the echoing mechanism
    while(1) //warning -> better signal handling required 
    {
        //accept is a blocking call which means till the time a client 
        //does not connect to our server, we will be blocked and just be waiting. 
        if((new = accept(sockdf, (struct sockaddr *)&client, &sockaddr_len)) == ERROR)
        {
            perror("accept");
            exit(-1);
        }

        printf("New Client connected from port number %d and IP %s \n",ntohs(client.sin_port), inet_ntoa(client.sin_addr));

        data_len = 1;

        //this loop runs till the client is connected to us
        while(data_len)
        {
            data_len = recv(new, data, MAX_DATA, 0); //receive data from client socket new

            if(data_len) //same data is sent to client
            {
                send(new, data, data_len, 0);
                data[data_len] = '\0'; //null helps printf 
                printf("Sent mesg: %s", data);
            }
        }

        printf("Client disconnected\n");
        close(new);
    }

    close(sockdf);

}