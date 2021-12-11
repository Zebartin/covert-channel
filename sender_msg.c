#include "config.h"
#include <sys/msg.h>

#define N 256
int mq_num = 0;
int msgids[N];
const char *buf = "abc";

void send_char(char c)
{
    int i, j, t;
    // 逐比特传输字节
    for (i = 0; i < 8; i++)
    {
        t = c & 1;
        if (t)
        {
            printf("%d\n", c & 1);
            msgctl(msgids[0], IPC_RMID, NULL);
        }
        // 告知数据已发送
        access_file(FILE_SYNC_PATH);
        // 等待ack
        loop_until_modified(FILE_SYNC_PATH);
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
    // 简单起见，不用handshake文件同步了，改用字符\0来表示结束
    loop_until_exists(FILE_SYNC_PATH);
    char ch;
    int idx = 0, tmp;
    int i;
    // 挤满系统可用的消息队列
    while (1)
    {
        tmp = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
        if (tmp == -1)
        {
            printf("Have reached message queue limit [%d]:\n", mq_num);
            break;
        }
        // N不够大，不能填满
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

    printf("START SENDING\n");
    int fd = open(FILE_INPUT_PATH, O_RDONLY);
    while (read(fd, &ch, 1))
    {
        printf("Sending [%c]\n", ch);
        send_char(ch);
    }
    // 发送\0表示结束
    send_char(0);
    // 释放占用的消息队列
    for (i = 0; i < mq_num; i++)
        msgctl(msgids[i], IPC_RMID, NULL);
    printf("FINISH SENDING\n");
    return 0;
}
