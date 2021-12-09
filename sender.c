#include "config.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const char* buf = "hi";

int main()
{
    // 等待第一次握手以建立通信
    loop_until_exists(FILE_A_PATH);
    loop_until_exists(FILE_B_PATH);
    loop_until_exists(FILE_C_PATH);

    printf("START SENDING\n");

    char ch;
    int idx = 0;

    ch = buf[idx++];
    while (ch != '\0') {
        printf("SENDING %c\n", ch);
        ch = buf[idx++];
        // 告知数据已发送
        my_touch(FILE_B_PATH);
        // 等待ack
        loop_until_modified(FILE_C_PATH);
    }

    printf("FINISH SENDING\n");

    // 结束握手
    my_touch(FILE_A_PATH);
    return 0;
}