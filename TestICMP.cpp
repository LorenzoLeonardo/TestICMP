// TestICMP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <string>

using namespace std;

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

int __cdecl main(int argc, char** argv) {

    // Declare and initialize variables

    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

    // Validate the parameters

 

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        printf("\tUnable to open handle.\n");
        printf("IcmpCreatefile returned error: %ld\n", GetLastError());
        return 1;
    }
    string ip = "192.168.0.";
    string c;
    for (int i = 1; i < 255; i++)
    {
        ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
        ReplyBuffer = (VOID*)malloc(ReplySize);
        if (ReplyBuffer == NULL) {
            printf("\tUnable to allocate memory\n");
            return 1;
        }
        c = ip + to_string(i);

        inet_pton(AF_INET, c.c_str(), &ipaddr);

        dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
            NULL, ReplyBuffer, ReplySize, 1000);
        struct in_addr ReplyAddr;
        if (dwRetVal != 0) {
            PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
           
            ReplyAddr.S_un.S_addr = pEchoReply->Address;
            printf("\tSent icmp message to %s\n", argv[1]);
            if (dwRetVal > 1) {
                printf("\tReceived %ld icmp message responses\n", dwRetVal);
                printf("\tInformation from the first response:\n");
            }
            else {
                printf("\tReceived %ld icmp message response\n", dwRetVal);
                printf("\tInformation from this response:\n");
            }
            char ipAd[32];
            inet_ntop(AF_INET, (const void*)&ReplyAddr, ipAd, sizeof(ipAd));
            printf("\t  Received from %s\n", ipAd);
            printf("\t  Status = %ld\n",
                pEchoReply->Status);
            printf("\t  Roundtrip time = %ld milliseconds\n",
                pEchoReply->RoundTripTime);
        }
        else
        {


            printf("\tIcmpSendEcho returned error:%s %ld\n", c.c_str(), GetLastError());
           
        }
        free(ReplyBuffer);
    }
    IcmpCloseHandle(hIcmpFile);
    return 0;
}

