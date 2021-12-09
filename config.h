#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

const char* FILE_INPUT_PATH = "./os_course_input_file";
const char* FILE_OUTPUT_PATH = "./os_course_output_file";
const char* FILE_A_PATH = "./A";
const char* FILE_B_PATH = "./B";
const char* FILE_C_PATH = "./C";

int my_touch(const char* path)
{
    // 由于 st_atime 的最小粒度为 1,所以此处显式等待 1s,让 receiver 获取文件B的一个有效的时间戳
    sleep(1);
    static struct timespec newtime[2];
    newtime[0].tv_nsec = UTIME_NOW;
    newtime[1].tv_nsec = UTIME_NOW;
    utimensat(AT_FDCWD, path, newtime, 0);
    printf("HAS TOUCHED %s\n", path);
    fflush(NULL);
    return 0;
}

void loop_until_exists(const char* path)
{
    int fd;
    while ((fd = open(path, O_RDONLY)) == -1) {
        printf("FILE %s DOESN'T EXIT\n", path);
        fflush(NULL);
        sleep(1);
    }
}

void loop_until_modified(const char const* path)
{
    struct stat s;
    stat(path, &s);
    // printf("%s FIRST STAMP %ld\n", path, s.st_atime);
    long long last_time = s.st_atime;

    while (last_time == s.st_atime) {
        stat(path, &s);
        // printf("FILE %s HASN'T CHANGE:%ld\n", path, s.st_atime);
        fflush(NULL);
        // sleep(1);
    }
}
