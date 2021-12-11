#include "config.h"
#include <sys/msg.h>

char receive_char()
{
    char c = 0;
    int i, msg_id;
    // 逐比特接收字节
    for (i = 0; i < 8; i++)
    {
        loop_until_modified(FILE_SYNC_PATH);
        // 检查消息队列是否有空槽
        msg_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
        if (msg_id != -1)
        {
            c |= 1 << i;
            msgctl(msg_id, IPC_RMID, NULL);
            printf("1");
        }
        else
            printf("0");
        access_file(FILE_SYNC_PATH);
    }
    printf("\n");
    return c;
}
int main()
{
    // 初始化通信环境,与 sender 第一次握手
    // 简单起见，不用handshake文件同步了，改用字符\0来表示结束
    int fd_sync = open(FILE_SYNC_PATH, O_CREAT, 0777);
    int fd_out = open(FILE_OUTPUT_PATH, O_WRONLY | O_CREAT, 0777);

    while (1)
    {
        char ch = receive_char();
        if (ch == 0)
            break;
        write(fd_out, &ch, sizeof(ch));
        printf("Received [%c]\n", ch);
    }

    close(fd_sync);
    close(fd_out);
    // 销毁通信环境
    // 显式地等待 1s,以防止文件过快删除,sender 读不到最新的 st_atime
    sleep(1);
    remove(FILE_SYNC_PATH);
    return 0;
}
