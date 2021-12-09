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
    int fd_A = open(FILE_A_PATH, O_CREAT, 0777);
    int fd_B = open(FILE_B_PATH, O_CREAT, 0777);

    loop_until_modified(FILE_B_PATH);

    remove(FILE_A_PATH);
    remove(FILE_B_PATH);
    return 0;
}