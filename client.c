// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#define PORT 8000

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Invalid number of Arguments\n");
        return -1;
    }

    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    // char *hello = "Hello from client";
    int buffersize = 1024;
    char buffer[buffersize], confirm[buffersize];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0
                                                // which is meant to be, and rest is defined below

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converts an IP address in numbers-and-dots notation into either a
    // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // connect to the server address
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    char files[buffersize];
    sprintf(files, "%d", argc - 1);
    send(sock, files, strlen(files), 0);
    valread = read(sock, confirm, 1024);
    for (int i = 0; i < argc - 1; i++)
    {
        // printf("henlo\n");
        send(sock, argv[i + 1], strlen(argv[i + 1]), 0);
        memset(buffer, 0, buffersize);
        // printf("before\n");
        valread = read(sock, buffer, 1024);
        // printf("after\n");
        // send(sock, "confirmed", strlen("confirmed"), 0);
        // printf("%c\n", buffer[0]);
        if (buffer[0] == 'y')
        {
            // printf("work\n");
            int fd;
            fd = open(argv[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0600);
            if(fd == -1)
            {
                send(sock, "n", strlen("n"), 0);
                printf("Not able to read from or write into file input name %s\n", argv[i+1]);
                continue;
            }
            // printf("bye\n");
            send(sock, "y", strlen("y"), 0);
            memset(buffer, 0, buffersize);
            valread = read(sock, buffer, buffersize);
            int size = atoi(buffer);
            send(sock, "confirmed", strlen("confirmed"), 0);
            memset(buffer, 0, buffersize);
            int j = 0;
            float sizesent=0;
            float size2 = size;
            while (size - (j * buffersize) > 0)
            {
                // printf("hi\n");
                valread = read(sock, buffer, buffersize);
                send(sock, "confirmed", strlen("confirmed"), 0);
                write(fd, buffer, valread);
                memset(buffer, 0, buffersize);
                sizesent+=valread;
                printf("%f\r", (float)(sizesent/size2)*100);
                j++;
            }
            valread = read(sock, confirm, buffersize);
        }
        else
        {
            printf("File input error\n");
        }
    }
    // send(sock , hello , strlen(hello) , 0 );  // send the message.
    // printf("Hello message sent\n");
    // valread = read( sock , buffer, 1024);  // receive message back from server, into the buffer
    // printf("%s\n",buffer);
    return 0;
}