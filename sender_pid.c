#include "config.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void create_processes(char ch)
{
    pid_t s_pid[256];
    int i;
    for (i = 0; i < ch; i++) {
        if ((s_pid[i] = fork()) == 0) {
            sleep(1);
            exit(0);
        } else if (s_pid[i] < 0) {
            perror("error!");
        }
    }
    printf("%d proccesses are created!\n", ch);
}

int main()
{
    // 等待第一次握手以建立通信
    loop_until_exists(FILE_HANDSHAKE_PATH);
    loop_until_exists(FILE_SYNC_PATH);

    printf("START SENDING\n");
    int fd = open(FILE_INPUT_PATH, O_RDONLY);
    char ch;
    int idx = 0;

    while (read(fd, &ch, 1)) {
        printf("Sending [%c]\n", ch);
        create_processes(ch);
        // 告知数据已发送
        access_file(FILE_SYNC_PATH);
        // 等待ack
        loop_until_modified(FILE_SYNC_PATH);
    }
    close(fd);
    printf("FINISH SENDING\n");

    // 结束握手
    access_file(FILE_HANDSHAKE_PATH);
    return 0;
}
