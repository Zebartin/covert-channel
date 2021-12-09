#include "config.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    // 等待第一次握手以建立通信
    loop_until_exists(FILE_A_PATH);
    loop_until_exists(FILE_B_PATH);
    loop_until_exists(FILE_C_PATH);

    // 由于 st_atime 的最小粒度为 1,所以此处显式等待 1s,让 receiver 获取文件B的一个有效的时间戳
    // sleep(1);

    printf("START SENDING\n");

    // 告知数据已发送
    my_touch(FILE_B_PATH);
    // 等待ack
    loop_until_modified(FILE_C_PATH);

    printf("FINISH SENDING\n");

    // 结束握手
    my_touch(FILE_A_PATH);
    return 0;
}