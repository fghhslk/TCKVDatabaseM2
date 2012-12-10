/********************************************************************/
/* Copyright (C) SSE-USTC, 2012                                     */
/*                                                                  */
/*  FILE NAME             :  dbapi.c                                */
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

#include <tcutil.h>
#include <tchdb.h>
#include "dbapi.h"

/* Public Functions */

/**
 * delete a hash database 
 */
tcDatabase CreateDB(const char *dbName)
{
    int ecode;
    TCHDB *hdb = tchdbnew();
    if (dbName == NULL)
    {
        dbName = "default.hdb";        
    }
    if (!tchdbopen(hdb, dbName, HDBOWRITER | HDBOCREAT))
    {
        ecode = tchdbecode(hdb);
        fprintf(stderr, "open error: %s\n", tchdberrmsg(ecode));
        exit(-1);
    } 
    //printf("open successfully!\n");  
    return (tcDatabase)hdb;
}

/** 
 *close database 
 */
int CloseDB(tcDatabase hdb)
{
    int ecode;
    if (!tchdbclose((TCHDB*)hdb))
    {
        ecode = tchdbecode((TCHDB*)hdb);
        fprintf(stderr, "close error: %s\n", tchdberrmsg(ecode));
        exit(-1);
    }
    tchdbdel((TCHDB*)hdb);
    return SUCCESS;
}

/** 
 * set a record to database 
 */
int SetRecord(tcDatabase hdb, tcKey key, tcValue value)
{
    int ecode;
    if (!tchdbput((TCHDB*)hdb, &key, sizeof(tcKey), value.str, value.len))
    {
        ecode = tchdbecode((TCHDB*)hdb);
        fprintf(stderr, "put error: %s\n", tchdberrmsg(ecode));
        return FAILURE;
    } 
    //printf("set successfully!\n");
    return SUCCESS;  
}

/** 
 * get a record from database 
 */
int GetRecord(tcDatabase hdb, tcKey key, tcValue *pvalue)
{
    int ecode;
    int ret;

    ret = tchdbget3(hdb, &key, sizeof(tcKey), pvalue->str, pvalue->len);
    //printf("%d\n", ret);
    if(ret != -1)
    {
        pvalue->str[ret] = '\0';
        pvalue->len = ret;
        return SUCCESS;
    }	
    ecode = tchdbecode(hdb);
    fprintf(stderr, "get error: %s\n", tchdberrmsg(ecode));
    
    return FAILURE;  
}

/** 
 * delete a record by key 
 */
int DeleteRecord(tcDatabase hdb, tcKey key)
{
    int ecode;
    if(!tchdbout((TCHDB*)hdb, &key, sizeof(tcKey)))
    {
        ecode = tchdbecode(hdb);
        fprintf(stderr, "delete error: %s\n", tchdberrmsg(ecode));
        return FAILURE;
    }
    return SUCCESS;
}


