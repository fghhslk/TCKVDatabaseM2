/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  client.c                               */
/*  PRINCIPAL AUTHOR      :  SLK                                    */
/*  SUBSYSTEM NAME        :  system                                 */
/*  MODULE NAME           :  client                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/12/10                             */
/*  DESCRIPTION           :  Vampire KV system based on TC M2       */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by SLK,2012/12/10
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "net.h"
#include "dbapi.h"

int main(int argc, char **argv)
{
    char recvBuf[MAX_BUFFER_SIZE] = "";
    char *sendBuf = (char*) malloc(MAX_BUFFER_SIZE * sizeof(char));
    char addr[40];
    int sockfd;
    int port;
    printf("Please input server's IP: ");
    scanf("%s", addr);
    printf("Please input server's PORT: ");
    scanf("%d", &port);
    sockfd = InitClient(sockfd, addr, port);
    printf("Connected!\n");
    if (RecvMsg(sockfd, recvBuf) == 0)
    {
        SendMsg(sockfd, "recved");
        printf("%s", recvBuf);
    }
    while (1) 
    {
        if (RecvMsg(sockfd, recvBuf) == 0)
        {
            //SendMsg(sockfd, "recved");
            printf("%s", recvBuf);
        }
        if (strcmp(recvBuf, "exit success!\n") == 0)
            break;
        //printf("command:\n");
        fgets(sendBuf, MAX_BUFFER_SIZE, stdin);
        printf("%s", sendBuf);
        *(sendBuf + strlen(sendBuf) - 1) = '\0';
	if (SendMsg(sockfd, sendBuf) == 0)
            RecvMsg(sockfd, recvBuf);
    }
    ShutdownSocket(sockfd);
}
