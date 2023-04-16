#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
struct TTSV
{
    char MSSV[10];
    char HT[255];
    char BIRTH[255];
    float GPA;
};
int main(int argc, char *argv[])
{
    char mssv[10];
    char ht[255];
    char birth[255];
    float gpa;
    if (argc < 2)
    {
        fprintf(stderr, "providing IP and");
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
    while (1)
    {
        printf("SV1:\n");
        fflush(stdin);
        struct TTSV SV1;
        memset(&SV1, '\0', sizeof(struct TTSV));
        printf("MSSV: ");
        fgets(mssv, 10, stdin);
        fflush(stdin);
        printf("Ho va Ten: ");
        fgets(ht, 255, stdin);
        fflush(stdin);
        printf("Ngay Sinh: ");
        fgets(birth, 255, stdin);
        fflush(stdin);
        printf("GPA: ");
        scanf("%f", &gpa);
        fflush(stdin);
        strncpy(SV1.MSSV, mssv, strlen(mssv) - 1);
        strncpy(SV1.HT, ht, strlen(ht) - 1);
        strncpy(SV1.BIRTH, birth, strlen(birth) - 1);
        SV1.GPA = gpa;
        if (strlen(SV1.MSSV) > 8)
        {
            SV1.MSSV[8] = '\0';
        }
        char h[100];
        fgets(h, 100, stdin);
        unsigned char buffer[sizeof(struct TTSV)];
        memcpy(buffer, &SV1, sizeof(struct TTSV));
        send(client, buffer, sizeof(struct TTSV), 0);
    }
    close(client);
}