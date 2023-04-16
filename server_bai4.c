#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
// provides required data types
#include <sys/types.h>
// holds address family and socket functions
#include <sys/socket.h>
// has the sockaddr_in structure
#include <netinet/in.h>
// has functions for read and write operations
#include <fcntl.h>
// basic C header
#include <stdio.h>
// header to help with strings
#include <string.h>
// has macros such as EXIT_FAILURE
#include <stdlib.h>
// port through which connection is to be made

#define CONNECTION_PORT 3050
struct TTSV
{
    char MSSV[10];
    char HT[255];
    char BIRTH[255];
    float GPA;
};
void get_time_now(char *store_time)
{
    time_t now = time(NULL);
    struct tm tm_now;
    localtime_r(&now, &tm_now);
    char buff[100];
    strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", &tm_now);
    strcpy(store_time, buff);
}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    char time_now[255];
    // server socket
    int socket_descriptor;
    // socket created by the bind function
    int client_socket;
    // buffer to store message
    char storage_buffer[255];
    int length_of_address;
    // option value for respective option_name
    int option_value = 1;
    // server and client address structures
    struct sockaddr_in server_address;
    struct sockaddr_in connection_address;
    if (argc < 1)
    {
        fprintf(stderr, "port no provided.... try again");
        exit(1);
    }
    char *message = "This is a message from the server";
    // creating the socket with IPv4 domain and TCP protocol
    socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Check if the socket is created successfully
    if (socket_descriptor < 0)
    {
        perror("Socket creation failed");
        // EXIT_FAILURE is a macro used to indicate unsuccessful execution of the program
        exit(EXIT_FAILURE);
    }
    // set options for the socket
    int status = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
    // check if options are set successfully
    if (status < 0)
    {
        perror("Couldn't set options");
        exit(EXIT_FAILURE);
    }
    //  initializing structure elements for address
    server_address.sin_family = AF_INET;
    // convert port to network byte order using htons
    server_address.sin_port = htons((unsigned int)atoi(argv[1]));
    // any address available
    server_address.sin_addr.s_addr = INADDR_ANY;
    // Assigning null character to the last index of the character array
    server_address.sin_zero[8] = '\0';
    // bind the socket with the values address and port from the sockaddr_in structure
    status = bind(socket_descriptor, (struct sockaddr *)&server_address, sizeof(struct sockaddr));
    // check if the binding was successful
    if (status < 0)
    {
        perror("Couldn't bind socket");
        exit(EXIT_FAILURE);
    }
    // listen on specified port with a maximum of 4 requests
    status = listen(socket_descriptor, 4);
    // check if the socket is listening successfully
    if (status < 0)
    {
        perror("Couldn't listen for connections");
        exit(EXIT_FAILURE);
    }
    length_of_address = sizeof(connection_address);
    // accept connection signals from the client
    client_socket = accept(socket_descriptor, (struct sockaddr *)&connection_address, &length_of_address);
    // check if the server is accepting the signals from the client
    if (client_socket < 0)
    {
        perror("Couldn't establish connection with client");
        exit(EXIT_FAILURE);
    }
    char mark[255];
    while (1)
    {

        if (strstr(mark, "end"))
        {
            break;
        }
        unsigned char buffer[sizeof(struct TTSV)];
        int a = recv(client_socket, buffer, sizeof(struct TTSV), 0);
        // int a  = recv(client_socket,&SV1,sizeof(struct TTSV),0);
        if (a <= 0)
        {
            continue;
        }
        struct TTSV SV1;
        memcpy(&SV1, buffer, sizeof(struct TTSV));
        get_time_now(time_now);
        FILE *fp = fopen(argv[2], "a");
        if (fp == NULL)
        {
            printf("Error opening the file sv_log.txt");
            return -1;
        }
        printf("%s %s %s %.2f\n", SV1.MSSV, SV1.HT, SV1.BIRTH, SV1.GPA);
        // write to the text file
        fprintf(fp, "%s %s %s %s %s %.2f\n", inet_ntoa(connection_address.sin_addr), time_now, SV1.MSSV, SV1.HT, SV1.BIRTH, SV1.GPA);
        // close the file
        fclose(fp);
    }
    close(socket_descriptor);
    close(client_socket);
    return 0;
}