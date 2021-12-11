#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

const char* FILE_INPUT_PATH = "/home/userhigh/in_text";
const char* FILE_OUTPUT_PATH = "/home/userlow/lowlvl/out_text";
const char* FILE_HANDSHAKE_PATH = "/home/userlow/shared/handshake";
const char* FILE_SYNC_PATH = "/home/userlow/shared/sync";

int access_file(const char* path)
{
    // 由于 st_atime 的最小粒度为 1,所以此处显式等待 1s,让 receiver 获取文件B的一个有效的时间戳
    sleep(1);
    char c;
    int fd = open(path, O_RDONLY);
    read(fd, &c, 1);
    close(fd);
    fflush(NULL);
    return 0;
}

void loop_until_exists(const char* path)
{
    int fd;
    while ((fd = open(path, O_RDONLY)) == -1) {
        printf("FILE %s DOESN'T EXIST\n", path);
        fflush(NULL);
        sleep(1);
    }
}

void loop_until_modified(const char* path)
{
    struct stat s;
    stat(path, &s);
    long long last_time = s.st_atime;

    while (last_time == s.st_atime) {
        stat(path, &s);
        fflush(NULL);
    }
}
