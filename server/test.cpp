#include <iostream>
#include <libpcan.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#define DEFAULT_NODE "/dev/pcanusb32"

main(int argc, char const *argv[])
{
    HANDLE h;
    TPCANRdMsg pMsgBuff;

    const char *szDevNode = DEFAULT_NODE;

    h = LINUX_CAN_Open(szDevNode, O_RDWR);
    if (!h)
    {
        printf("can't open %s\n", szDevNode);
        return 1;
    }
    CAN_Init(h, CAN_BAUD_500K, CAN_INIT_TYPE_ST); // BTR0BTR1	bitrate code in hex (default=0x1c (500 kbps))
    CAN_Status(h);                                // Clear Status

    for (int i = 0; i < 10; i++)
    {
        LINUX_CAN_Read_Timeout(h, &pMsgBuff, 1);
        std::cout << (uint)pMsgBuff.Msg.ID << "  =>  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[0] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[1] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[2] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[3] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[4] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[5] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[6] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[7] << '\n';
        sleep(1);
    }

    TPCANMsg msgBuff;

    msgBuff.ID = 0x1;
    msgBuff.MSGTYPE = MSGTYPE_STANDARD;
    msgBuff.LEN = 8;

    msgBuff.DATA[0] = 0;
    msgBuff.DATA[1] = 0;
    msgBuff.DATA[2] = 0;
    msgBuff.DATA[3] = 0;
    msgBuff.DATA[4] = 0;
    msgBuff.DATA[5] = 1;
    msgBuff.DATA[6] = 2;
    msgBuff.DATA[7] = 3;

    LINUX_CAN_Write_Timeout(h, &msgBuff, 0);

    while (1)
    {
        LINUX_CAN_Read_Timeout(h, &pMsgBuff, 1);
        std::cout << (uint)pMsgBuff.Msg.ID << "  =>  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[0] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[1] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[2] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[3] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[4] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[5] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[6] << "  :  ";
        std::cout << (uint)pMsgBuff.Msg.DATA[7] << '\n';
        sleep(1);
    }

    return 0;
}
