#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#define PORT 12345
#define ADDR "127.0.0.1"
//define ADDR "172.29.156.90"


int main(int argc, char* argv[]) {
    struct sockaddr_in addr;
    struct sockaddr_in s_addr;
    int fd;
    int ret;
    ssize_t ret_recv;
    socklen_t len;
    int soc;
    int acc;
    char buf[4097];
    int yes = 1;


    memset(buf, 0, sizeof(buf));

    if((fd = open("test.json", O_WRONLY|O_CREAT, 0600)) < 0) {
        perror("open");
        exit(1);
    }
    
    if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    ret = inet_pton(AF_INET, ADDR, &addr.sin_addr.s_addr);

    setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));
    
    if(ret == 0) {
        fprintf(stderr, "Not in presentation format\n");
        exit(1);
    }
    else if(ret < 0) {
        perror("inet_pton");
        exit(1);
    }

    if((bind(soc, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
        perror("bind");
        exit(1);
    }

    if(listen(soc, 3) < 0) {
        perror("listen");
        exit(1);
    }

    printf("listen...\n");
    printf("\n");

    len = sizeof(s_addr);

    if((acc = accept(soc, (struct sockaddr*)&s_addr, &len)) < 0) {
        perror("accept");
        exit(1);
    }
    
    printf("client ip address: %s\n", inet_ntoa(s_addr.sin_addr));
    printf("client port      : %d\n", ntohs(s_addr.sin_port));
    printf("\n");
    printf("recieving...\n");
    printf("\n");

    while(1) {
        ret_recv = recv(acc, buf, sizeof(buf)-1, 0);
        
        if(write(fd, buf, strlen(buf)) < 0) {
            perror("write");
            exit(1);
        }

        memset(buf, 0, sizeof(buf));

        if(ret_recv == -1) {
            perror("recv");
            exit(1);
        }
        else if(ret_recv < sizeof(buf)-1) {
            break;
        }

    }
    
    printf("finish!\n");

    if(close(fd) < 0) {
        perror("close fd");
        exit(1);
    }

    if(close(acc) < 0) {
        perror("close acc");
        exit(1);
    }

    if(close(soc) < 0) {
        perror("close soc");
        exit(1);
    }

}

