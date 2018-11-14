#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 2222
#define BUFFERSIZE 128

int main(int argc, char *argv[])
{
    int socketFd;
    struct sockaddr_in address;

    if((socketFd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }

    bzero((char *) &address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    if (connect(socketFd,(struct sockaddr *)&address,sizeof(address)) < 0)
    {
        perror("No se pudo conectar al socket");
        exit(EXIT_FAILURE);
    }

    int running = 1;
    int n;
    char buffer[BUFFERSIZE];

    while (running)
    {
        printf("Escribi el mensaje: ");

        bzero(buffer,BUFFERSIZE);
        fgets(buffer,BUFFERSIZE,stdin);

        n = write(socketFd,buffer,strlen(buffer));
        if (n < 0)
        {
            running = 0;
            perror("Socket closed \n");
        }
    }

    return 0;
}
