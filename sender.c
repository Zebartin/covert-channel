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
    loop_until_exists(FILE_A_PATH);
    loop_until_exists(FILE_B_PATH);

    sleep(5);

    printf("START SENDING\n");

    my_touch(FILE_B_PATH);

    printf("FINISH SENDING\n");
    return 0;
}