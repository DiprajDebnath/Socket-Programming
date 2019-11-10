/*
 *  A Simple Echo Client
 * 
 *  ./client ip port
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<error.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define ERROR -1
#define BUFFER 1024

main(int arge, char **argv)
{
    struct sockaddr_in remote_server; //contains IP address and port number of the remote echo server 
    int sockdf;
    char input[BUFFER];
    char output[BUFFER];
    int len;

    //construct TCP socket to connect to the remote server
    if((sockdf = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
    {
        perror("socket");
        exit(-1);
    }

    //remote server details
    remote_server.sin_family = AF_INET;
    remote_server.sin_port = htons(atoi(argv[2]));  //remote server port number
    remote_server.sin_addr.s_addr = inet_addr(argv[1]); //IP
    bzero(&remote_server.sin_zero, 8);

    if((connect(sockdf, (struct sockaddr *)&remote_server, sizeof(struct sockaddr_in))) == ERROR)
    {
        perror("connect");
        exit(-1);
    }

    while (1)
    {
        fgets(input, BUFFER, stdin);
        send(sockdf, input, strlen(input), 0);

        len = recv(sockdf, output, BUFFER, 0);
        output[len] = '\0'; //the last byte is null so that the printf statement doesnot crash
        printf("%s \n", output);
    }

    close(sockdf);

}