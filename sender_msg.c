#include "config.h"
#include <sys/msg.h>

#define N 256
int mq_num = 0;
int msgids[N];
const char *buf = "abc";

void sendChar(char c)
{
    int i, j, t;
    for (i = 0; i < 8; i++)
    {
        t = c & 1;
        if (t)
        {
            printf("%d\n", c & 1);
            msgctl(msgids[0], IPC_RMID, NULL);
        }
        // 告知数据已发送
        my_touch(FILE_B_PATH);
        // 等待ack
        loop_until_modified(FILE_C_PATH);
        if (t)
        {
            msgids[0] = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
            if (msgids[0] == -1)
            {
                for (j = 1; j < mq_num; j++)
                    if (msgids[j] != -1)
                    {
                        msgids[0] = msgids[j];
                        msgids[j] = -1;
                    }
            }
        }
        c >>= 1;
    }
}

int main()
{
    // 等待第一次握手以建立通信
    loop_until_exists(FILE_B_PATH);
    loop_until_exists(FILE_C_PATH);

    printf("START SENDING\n");

    char ch;
    int idx = 0, tmp;
    int i;

    while (1)
    {
        tmp = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
        if (tmp == -1)
        {
            printf("Have reached message queue limit [%d]\n", mq_num);
            break;
        }
        if (mq_num == N)
        {
            printf("Can not reach message queue limit, try increasing N.\n");
            msgctl(tmp, IPC_RMID, NULL);
            for (i = 0; i < N; i++)
                msgctl(msgids[i], IPC_RMID, NULL);
            exit(0);
        }
        msgids[mq_num++] = tmp;
    }

    for (i = 0; i < mq_num; i++)
        printf("%d,", msgids[i]);
    printf("\n");

    int fd = open(FILE_INPUT_PATH, O_RDONLY);
    while (read(fd, &ch, 1))
    {
        printf("SENDING %c\n", ch);
        sendChar(ch);
    }
    sendChar(0);
    for (i = 0; i < mq_num; i++)
        msgctl(msgids[i], IPC_RMID, NULL);

    printf("FINISH SENDING\n");

    return 0;
}
