#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#define PORT 8000
int main(int argc, char const *argv[])
{
    if (argc != 1)
    {
        printf("Invalid Input\n");
        return -1;
    }
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int buffersize = 1024;
    char buffer[buffersize], confirm[buffersize];
    // char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // This is to lose the pesky "Address already in use" error message
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;         // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc.
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address - listens from all interfaces.
    address.sin_port = htons(PORT);       // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Port bind is done. You want to wait for incoming connections and handle them in some way.
    // The process is two step: first you listen(), then you accept()
    while (1)
    {
        if (listen(server_fd, 3) < 0) // 3 is the maximum size of queue - connections you haven't accepted
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        // returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read(new_socket, buffer, buffersize);
        send(new_socket, "confirmed", strlen("confirmed"), 0);
        int files = atoi(buffer);
        memset(buffer, 0, buffersize);
        for (int i = 0; i < files; i++)
        {
            valread = read(new_socket, buffer, buffersize);
            int fd, size, sizesent = 0, isdir = 0, flag1 = 0;
            fd = open(buffer, O_RDONLY);
            if (fd == -1)
            {
                flag1 = 1;
            }
            struct stat stats1;
            stat(buffer, &stats1);
            if (S_ISDIR(stats1.st_mode))
            {
                flag1 = 1;
            }
            if (flag1 == 0)
            {
                send(new_socket, "y", strlen("y"), 0);
            }
            else
            {
                send(new_socket, "n", strlen("n"), 0);
                continue;
            }
            memset(buffer, 0, buffersize);
            valread = read(new_socket, buffer, buffersize);
            if(buffer[0]=='y')
            {
                if (flag1 == 0)
                {
                    size = lseek(fd, 0, SEEK_END);
                    char filesize[buffersize];
                    sprintf(filesize, "%d", size);
                    send(new_socket, filesize, strlen(filesize), 0);
                    valread = read(new_socket, confirm, buffersize);
                    lseek(fd, 0, SEEK_SET);
                    int j = 0;
                    while (size - (j * buffersize) > 0)
                    {
                        read(fd, buffer, buffersize);
                        send(new_socket, buffer, strlen(buffer), 0);
                        valread = read(new_socket, confirm, 1024);
                        j++;
                        memset(buffer, 0, buffersize);
                    }
                    send(new_socket, "confirmed", strlen("confirmed"), 0);
                }
            }
        }
        // valread = read(new_socket , buffer, 1024);  // read infromation received into the buffer
        // printf("%s\n",buffer);
        // send(new_socket , hello , strlen(hello) , 0 );  // use sendto() and recvfrom() for DGRAM
        // printf("Hello message sent\n");
    }
    return 0;
}