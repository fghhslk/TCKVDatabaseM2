/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  dbapi.h                                */
/*  PRINCIPAL AUTHOR      :  SLK                                    */
/*  SUBSYSTEM NAME        :  system                                 */
/*  MODULE NAME           :  dbapi                                  */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  Linux                                  */
/*  DATE OF FIRST RELEASE :  2012/11/28                             */
/*  DESCRIPTION           :  the interface to Tokyo Cabinet(tc)     */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by SLK,2012/11/28
 *
 */

#ifndef _DBAPI_H_
#define _DBAPI_H_

/* Public Macro */
#define SUCCESS     0
#define FAILURE     -1

typedef void* tcDatabase;
typedef unsigned int tcKey;
typedef struct Value{
    int len;                    /* Length of string */
    char *str;                  /* Used for Value Content */
}tcValue;

/* Public Functions */
/*
 * Create a database and open a specific database               
 * input: database filename
 * output: none
 * in/out: none
 * return: tcDatebase
 */
tcDatabase CreateDB(const char*);

/* 
 * close database
 * input: database object
 * output: none
 * in/out: none
 * return: SUCCESS(0)/FAILURE(-1)
 */                         
int CloseDB(tcDatabase);

/* 
 * set a record to database
 * input: database object, tcKey, tcValue
 * output: none
 * in/out: none
 * return: SUCCESS(0)/FAILURE(-1)
 */                            
int SetRecord(tcDatabase, tcKey, tcValue);

/*
 * get a record from database
 * input: database object, tcKey, tcValue*
 * output: none
 * in/out: tcValue;
 * return: SUCCESS(0)/FAILURE(-1)
 */               
int GetRecord(tcDatabase, tcKey, tcValue*);

/* delete a record by key 
 * input: database object, tcKey
 * output: none
 * in/out: hdb
 * return: SUCCESS(0)/FAILURE(-1)
 */                        
int DeleteRecord(tcDatabase hdb, tcKey key);                

#endif
