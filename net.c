
/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  net.c          	                    */
/*  PRINCIPAL AUTHOR      :  SLK                                    */
/*  SUBSYSTEM NAME        :  system                                 */
/*  MODULE NAME           :  net                                    */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/11/22                             */
/*  DESCRIPTION           :  the interface to Linux system(socket)  */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by SLK,2012/11/22
 *
 */

#include <stdio.h> 
#include <stdlib.h>
#include <arpa/inet.h> /* internet socket */
#include <string.h>
#include <assert.h>
#include "net.h"

struct sockaddr_in clientAddr;
socklen_t addrLen = sizeof(struct sockaddr);

#define PrepareSocket(sockfd, addr, port)               \
        struct sockaddr_in serverAddr;                  \
        serverAddr.sin_family = AF_INET;                \
        serverAddr.sin_port = htons(port);              \
        serverAddr.sin_addr.s_addr = inet_addr(addr);   \
        memset(&serverAddr.sin_zero, 0, 8);             \
        sockfd = socket(PF_INET,SOCK_STREAM,0);

void ShutdownSocket(int sockfd)
{
     close(sockfd);
}
		
int InitServer(int sockfd, char* addr, int port)
{
    PrepareSocket(sockfd, addr, port);
    int ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr));
    if(ret == -1) 
    {
        printf("Bind Error!\n");
        close(sockfd);
        exit(1);
    } 
    listen(sockfd, MAX_CONNECT_QUEUE);
    return sockfd;
}

int InitClient(int sockfd, char* addr, int port) 
{
    PrepareSocket(sockfd, addr, port);
    int ret = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr));
    if(ret == -1)
    {
        printf("Connect Error!\n");
        exit(1);
    }
    return sockfd;
}

int ServiceStart(int sockfd, int sockfd_c) 
{                                 
    sockfd_c = accept(sockfd, (struct sockaddr *)&clientAddr, &addrLen);                         
    if(sockfd_c == -1) 
    {                                               
        printf("Accept Error!\n"); 
        return sockfd_c;
    } 
    else
        return sockfd_c;
}

int SendMsg(int sockfd, char *buf)
{
    int ret = send(sockfd, buf, strlen(buf), 0);
    //printf("ss:%d\n", ret);           
    if(ret == -1) 
    {                                                      
        return -1;            
    }
    //printf("%s\n", buf);
    return 0;
}

int RecvMsg(int sockfd, char *buf)
{                                  
    int ret = recv(sockfd, buf, MAX_BUFFER_SIZE, 0); 
    *(buf + ret) = '\0';
    //printf("rs:%d\n", ret); 	
    if(ret == -1) 
    {                                                      
        return -1;            
    }
    //printf("%s\n", buf);
    return 0;
}
