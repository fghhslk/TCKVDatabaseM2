
/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  net.h          	                    */
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
 
#ifndef _NET_H_
#define _NET_H_

#define MAX_BUFFER_SIZE     4096
#define MAX_CONNECT_QUEUE   255

/* public function */		
int InitServer(int, char*, int);
int InitClient(int, char*, int);
int ServiceStart(int, int);
int SendMsg(int, char*);
int RecvMsg(int, char*);
void ShutdownSocket(int);

		
#endif /* _NET_H_ */
