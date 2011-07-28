/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2008-2011 WiredTiger, Inc.
 *	All rights reserved.
 */

#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Quiet compiler warnings about unused parameters.
 */
#define	WT_UNUSED(var)	(void)(var)

#define	UTIL_FILE_OK	0x01		/* file: prefix OK */
#define	UTIL_TABLE_OK	0x02		/* table: prefix OK */

extern const char *progname;		/* Program name */
extern const char *usage_prefix;	/* Global arguments */
extern int verbose;			/* Verbose flag */

extern WT_EVENT_HANDLER *verbose_handler;

int	 util_create(WT_SESSION *, int, char *[]);
int	 util_dump(WT_SESSION *, int, char *[]);
int	 util_dumpfile(WT_SESSION *, int, char *[]);
int	 util_load(WT_SESSION *, int, char *[]);
char	*util_name(const char *, const char *, u_int);
int	 util_printlog(WT_SESSION *, int, char *[]);
int	 util_salvage(WT_SESSION *, int, char *[]);
int	 util_stat(WT_SESSION *, int, char *[]);
int	 util_verify(WT_SESSION *, int, char *[]);
