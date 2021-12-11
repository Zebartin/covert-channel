#include "config.h"
#include <sys/msg.h>

char receiveChar()
{
    char c = 0;
    int i, msg_id;
    for (i = 0; i < 8; i++)
    {
        loop_until_modified(FILE_B_PATH);
        msg_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
        if (msg_id != -1)
        {
            c |= 1 << i;
            msgctl(msg_id, IPC_RMID, NULL);
        }
        my_touch(FILE_B_PATH);
    }
    return c;
}
int main()
{
    // 初始化通信环境,与 sender 第一次握手
    int fd_B = open(FILE_B_PATH, O_CREAT, 0777);
    int fd_out = open(FILE_OUTPUT_PATH, O_WRONLY | O_CREAT, 0777);

    while (1)
    {
        char ch = receiveChar();
        if (ch == 0)
            break;
        write(fd_out, &ch, sizeof(ch));
        printf("Receiving the %c\n", ch);
    }

    close(fd_B);
    close(fd_out);
    // 销毁通信环境
    // 显式地等待 1s,以防止文件过快删除,sender 读不到最新的 st_atime
    sleep(1);
    remove(FILE_B_PATH);
    return 0;
}
