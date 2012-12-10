/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  server.c                               */
/*  PRINCIPAL AUTHOR      :  SLK                                    */
/*  SUBSYSTEM NAME        :  system                                 */
/*  MODULE NAME           :  server                                 */
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
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include "dbapi.h"
#include "net.h"

#define MAX_STR_LEN 1024

char cmd[MAX_STR_LEN];
char dbName[20];
tcDatabase hdb;
char *sendBuf;
int sockfd = -1;
int sockfd_c = -1;
int port;

char *helpStr = "===========================================================================\nCMDs Guide:\"open [dbname].hdb\" : to open a hash database.\n\"set [key] [value]\" : to add a key-value record to database.\n\"get [key]\"         : to get a record by key from database.\n\"del [key]\"         : to delete a record by key from database.\n\"close\"             : to close the opened database.\n\"exit\"              : to exit this application.\nRemember, you should open a database first!\n===========================================================================\n";
char *welcomeStr = "***************************************************************************\n****      Welcome to  Vampire  NoSql system!        ----version 1.0    ****\n***************************************************************************\nPlease type CMDs: (You can type in [help] for help)\n";

char* itoa(int i) /*定义itoa函数*/
{
    char *a = malloc(42);
    if (a) 
        sprintf(a, "%d", i);
    return a;
}

int GetCmd(char *cmd, int size)
{
    fgets(cmd, size, stdin);
    setbuf(stdin, NULL);
    cmd[strlen(cmd) - 1] = '\0';
    return SUCCESS;
} 

int CheckCmd(char *cmd, char *pattern)
{
    regex_t reg;
    regmatch_t pmatch[1];
    const size_t nmatch = 1;
    regcomp(&reg, pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB);
    if (!regexec(&reg, cmd, nmatch, pmatch, 0))
    {
        regfree(&reg);
        return SUCCESS;
    }     
}

int ExecCmd(char *cmd)
{
    if (CheckCmd(cmd, "help") == SUCCESS)
    {
        sendBuf = helpStr;
        SendMsg(sockfd_c, sendBuf);
    }
    else if (CheckCmd(cmd, "open\\s*\\w*.hdb") == SUCCESS)
    {
        if(strlen(dbName) > 0)
        {
            sendBuf = strcat(
                          strcat(
                              strcat(sendBuf, "Please close "), dbName), "first.\n");
            SendMsg(sockfd_c, sendBuf);
        }
        else
        {
            sscanf(cmd,"open %s",dbName);
            hdb = CreateDB(dbName);
            SendMsg(sockfd_c, "open success.\n");
        }
    }
    else if((hdb == NULL) && (CheckCmd(cmd,"set|get|del|close") == SUCCESS))
    {
        SendMsg(sockfd_c, "Please open a database file first.\n");
    }
    else if (CheckCmd(cmd, "close") == SUCCESS)
    {
        if(hdb != NULL)
        {
            CloseDB(hdb);
            hdb = NULL;
            SendMsg(sockfd_c, "close success.\n");
        }
        dbName[0] = '\0';
    }
    else if (CheckCmd(cmd, "exit") == SUCCESS)
    {
        if (hdb != NULL)
        {
            CloseDB(hdb);
            hdb = NULL;
        }
        SendMsg(sockfd_c, "exit success!\n");
        exit(0);
    }
    else if (CheckCmd(cmd, "set\\s*[0-9]*\\s*.*") == SUCCESS)
    {
        tcKey key;
        tcValue value;
        char str[MAX_STR_LEN] = "\0";
        sscanf(cmd, "set %d %s", &key, str);
        value.str = strstr(cmd, str);
        //value.len = cmd + MAX_STR_LEN - value.str;
        value.len = sizeof(str);
        //printf("%d", value.len);
        if(SetRecord(hdb, key, value) == FAILURE)
        {
            sendBuf = strcat(
                          strcat(
                              strcat(
                                  strcat(
                                      strcat(sendBuf, "ERROR:set "), itoa((int)key)), " "), value.str), "\n");
            SendMsg(sockfd_c, sendBuf);
        }
        else
        {
            SendMsg(sockfd_c, "set success!\n");
        }
    }
    else if (CheckCmd(cmd, "get\\s*[0-9]*") == SUCCESS)
    {
        tcKey key = -1;
        tcValue value;
        char str[MAX_STR_LEN] = "\0";
        value.str = str;
        value.len = MAX_STR_LEN;
        sscanf(cmd, "get %d", &key);
        if(GetRecord(hdb, key, &value) == SUCCESS)
        {
            sendBuf = strcat(
                          strcat(
                              strcat(
                                  strcat(sendBuf, itoa((int)key)), " : "), value.str), "\n");
            SendMsg(sockfd_c, sendBuf);
        }
        else
        {
            SendMsg(sockfd_c, "ERROR: not found!\n");
        }
    }
    else if (CheckCmd(cmd, "del\\s*[0-9]*") == SUCCESS)
    {
        tcKey key = -1;
        tcValue value;
        char str[MAX_STR_LEN] = "\0";
        value.str = str;
        value.len = MAX_STR_LEN;
        sscanf(cmd, "del %d", &key);
        if(DeleteRecord(hdb, key) == FAILURE)
        {
            SendMsg(sockfd_c, "ERROR:delete failure!\n");
        }
        else
        {
            SendMsg(sockfd_c, "delete success!\n");
        }
    }
    else
    {
        SendMsg(sockfd_c, "Command Not Found!\n");   
    }
    return SUCCESS;       
}

int main(int argc, char **argv)
{
    char *cmdStr = (char*) malloc(MAX_BUFFER_SIZE * sizeof(char));
    char recvBuf[MAX_BUFFER_SIZE] = "";
    char addr[40];

    sendBuf = welcomeStr;
    *dbName = '\0';
    
    printf("Please input server's IP: ");
    scanf("%s", addr);
    printf("Please input server's PORT: ");
    scanf("%d", &port);
    cmdStr = strcat(strcat(strcat(cmdStr, "Vampire @"), dbName), ">>");
    printf("OK\n");
    sockfd = InitServer(sockfd, addr, port);
    while (1) 
    {
        sockfd_c = ServiceStart(sockfd, sockfd_c);
        if (sockfd_c == -1)
            continue;
        else
            break;
    }
    printf("Connected!\n");
    SendMsg(sockfd_c, sendBuf);
    RecvMsg(sockfd_c, recvBuf);
    while (1) 
    {
        sendBuf = cmdStr;
        if (SendMsg(sockfd_c, sendBuf) == 0)
        {
            RecvMsg(sockfd_c, recvBuf);
            ExecCmd(recvBuf);
        }     	
    }
    printf("Disconnected!\n");
    ShutdownSocket(sockfd_c);
    ShutdownSocket(sockfd);
    
    return 0;
}
