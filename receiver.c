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
    // 初始化通信环境,与 sender 第一次握手
    int fd_A = open(FILE_A_PATH, O_CREAT, 0777);
    int fd_B = open(FILE_B_PATH, O_CREAT, 0777);
    int fd_C = open(FILE_C_PATH, O_CREAT, 0777);

    // 等待数据发送
    loop_until_modified(FILE_B_PATH);
    // 发送 ack
    my_touch(FILE_C_PATH);

    // 销毁通信环境
    // 显式地等待 1s,以防止文件过快删除,sender 读不到最新的 st_atime
    sleep(1);
    remove(FILE_A_PATH);
    remove(FILE_B_PATH);
    remove(FILE_C_PATH);
    return 0;
}