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
    pid_t first_pid, last_pid;
    if ((first_pid = fork()) == 0) {
        sleep(1);
        exit(0);
    } else if (first_pid < 0) {
        perror("error!");
        return -1;
    }
    // first_pid += 1; // 修正 // ???,为什么其实不需要?

    // 初始化通信环境,与 sender 第一次握手
    int fd_A = open(FILE_A_PATH, O_CREAT, 0777);
    int fd_B = open(FILE_B_PATH, O_CREAT, 0777);
    int fd_C = open(FILE_C_PATH, O_CREAT, 0777);

    while (1) {
        //  同时监控 文件AB有无被修改
        // 可理解为 loop_until_modified({FILE_A_PATH,FILE_B_PATH}); // loop_until_modified(vector<string> filepaths);
        // 此处可进一步优化
        struct stat s_a, s_b;

        stat(FILE_A_PATH, &s_a);
        stat(FILE_B_PATH, &s_b);
        printf("%s FIRST STAMP %ld\n", FILE_A_PATH, s_a.st_atime);
        printf("%s FIRST STAMP %ld\n", FILE_B_PATH, s_b.st_atime);

        stat(FILE_A_PATH, &s_a);
        stat(FILE_B_PATH, &s_b);
        long long last_a_time = s_a.st_atime;
        long long last_b_time = s_b.st_atime;
        while (last_a_time == s_a.st_atime && last_b_time == s_b.st_atime) {
            stat(FILE_A_PATH, &s_a);
            stat(FILE_B_PATH, &s_b);
            // printf("FILE %s HASN'T CHANGE:%ld\n", FILE_A_PATH, s_a.st_atime);
            // printf("FILE %s HASN'T CHANGE:%ld\n", FILE_B_PATH, s_a.st_atime);
            fflush(NULL);
            // sleep(1);
        }

        // 若文件 A 被修改,说明发送的是终止信号,否则是数据
        if (last_a_time != s_a.st_atime) {
            break;
        } else {
            if ((last_pid = fork()) == 0) {
                sleep(1);
                exit(0);
            } else if (last_pid < 0) {
                perror("error!");
                return -1;
            }
            unsigned int ch = last_pid - first_pid;
            printf("firstpid=%d,lastpid=%d,diff=%d\n", first_pid, last_pid, ch);
            printf("Receiving the %c\n", ch);

            first_pid = last_pid;
            my_touch(FILE_C_PATH);
        }
    }

    // 销毁通信环境
    // 显式地等待 1s,以防止文件过快删除,sender 读不到最新的 st_atime
    sleep(1);
    remove(FILE_A_PATH);
    remove(FILE_B_PATH);
    remove(FILE_C_PATH);
    return 0;
}