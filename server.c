#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 2222
#define BUFFERSIZE 128
#define BACKLOG 1
#define CLEAR_SCREEN_ANSI "\e[1;1H\e[2J"
#define CLEAR_SCREEN_ANSI_SIZE 12
#define NBROFLEVELS 8

const char * questions[] = {"Bienvenidos al TP04\nEn este TP deberán finalizar el juego que ya comenzaron respondiendo correctamente los desafios de cada nivel.\n\nPara verificar que sus respuestas tienen el formato correcto respondan a este desafio con \"entendido\" \n\n",
    "------------- NIVEL 1 -------------\n# up up left down left up right down down left *\n\n",
    "------------- NIVEL 2 -------------\n-.-. --- -. -. . -.-. - .. -. --. / .--. . --- .--. .-.. .\n\n",
    "------------- NIVEL 3 -------------\nMike Echo Golf Uniform Sierra Tango Alpha Echo Lima Charlie Hotel Oscar Charlie Oscar Lima Alpha Tango Echo\n\n",
    "------------- NIVEL 4 -------------\nTJ ANBYDNBCJ J NBCN JLNACQRX NB \"NBOXTXMAXKX\"\n\n",
    "------------- NIVEL 5 -------------\n#FFFF00\n\n",
    "------------- NIVEL 6 -------------\nI AM THE ONE WHO KNOCKS\n\n",
    "------------- NIVEL 7 -------------\nsizeof(int)\n\n",
    "------------- NIVEL 8 -------------\ndEAtP3SAlnA\n\n"};

const char * ans[] = {"entendido\n", "#965874580*\n", "connecting people\n", "me gusta el chocolate\n", "ESGOLODROBO\n",
    "amarillo\n", "Heisenberg\n", "indefinido\n", "Colombia\n"};


int main(int argc, char *argv[])
{

    int socketFd, anotherSocketFd;
    struct sockaddr_in address, anotherAddress;

    if((socketFd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    int addressSize = sizeof(address);

    if(bind(socketFd,(struct sockaddr*)&address, addressSize) != 0)
    {
        perror("No pudo hacer el bind");
        exit(EXIT_FAILURE);
    }

    if(listen(socketFd, BACKLOG)!= 0)
    {
        perror("No pudo hacer el listen");
        exit(EXIT_FAILURE);
    }

    int level = 0;
    int anotherAddressSize = sizeof(anotherAddress);

    anotherSocketFd = accept(socketFd, (struct sockaddr *) &anotherAddress, (socklen_t *)&anotherAddressSize);

    if(anotherSocketFd < 0)
    {
        perror("No se pudo acceptar");
        exit(EXIT_FAILURE);
    }

    int compare;
    char buffer[BUFFERSIZE];

    while(level <= NBROFLEVELS)
    {
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, CLEAR_SCREEN_ANSI_SIZE);
        fprintf(stdout, "%s", questions[level]);

        do
        {
            bzero(buffer,BUFFERSIZE);
            if(read(anotherSocketFd,buffer, BUFFERSIZE-1) < 0)
            {
                perror("No se pudo leer");
            }
            compare = strcmp(ans[level], buffer);
            if(compare != 0)
            {
                fprintf(stdout, "Respuesta incorrecta\n");
            }

        } while(compare != 0);

        level++;
    }

    fprintf(stdout, "Finalizado, pulsione cualquier tecla para terminar\n");

    shutdown(anotherSocketFd, SHUT_RDWR);
    shutdown(socketFd, SHUT_RDWR);
    close(anotherSocketFd);
    close(socketFd);
    return 0;
}
