#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if(argc <2){
        fprintf(stderr,"providing IP and");
        exit(1);
    }
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons((unsigned int)atoi(argv[2]));

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }
    int n;
    char buf[255];
    n = read(client,buf,sizeof(buf));
    if(n<0){
        printf("error on reading");
    }
    printf("@Server: %s",buf);
    bzero(buf,255);
    printf("Enter string: ");
    fgets(buf, sizeof(buf), stdin);
    write(client, buf, strlen(buf));
    bzero(buf,255);

    close(client);
}