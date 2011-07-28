/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2008-2011 WiredTiger, Inc.
 *	All rights reserved.
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * WT_EVICT_LIST --
 *	Encapsulation of an eviction candidate.
 */
struct __wt_evict_list {
	WT_PAGE	 *page;				/* Page */
	WT_BTREE *btree;			/* Underlying file object */
};

/*
 * WT_EVICT_REQ --
 *	Encapsulation of a eviction request.
 */
struct __wt_evict_req {
	WT_SESSION_IMPL *session;			/* Requesting thread */
	WT_BTREE *btree;				/* Btree */
	int	  close_method;			/* Discard pages */

	WT_PAGE **retry;			/* Pages to retry */
	uint32_t  retry_next;			/* Next retry slot */
	uint32_t  retry_entries;		/* Total retry slots */
	uint32_t  retry_allocated;		/* Bytes allocated */
};

/*
 * WT_READ_REQ --
 *	Encapsulation of a read request.
 */
struct __wt_read_req {
	WT_SESSION_IMPL *session;			/* Requesting thread */
	WT_PAGE *parent;			/* Parent */
	WT_REF  *ref;				/* Reference/Address */
	int	 dsk_verify;			/* Verify the disk image */
};

/*
 * WiredTiger cache structure.
 */
struct __wt_cache {
	/*
	 * The cache thread sets/clears the evict_sleeping flag when blocked
	 * on the mtx_evict mutex.  The workQ thread uses the evict_sleeping
	 * flag to wake the cache eviction thread as necessary.
	 */
	WT_MTX *mtx_evict;		/* Cache eviction server mutex */
	u_int volatile evict_sleeping;	/* Sleeping */

	WT_EVICT_LIST *evict;		/* Pages being tracked for eviction */
	uint32_t evict_entries;		/* Total evict slots */
	uint32_t evict_allocated;	/* Bytes allocated */

	/*
	 * File sync can temporarily fail when a tree is active, that is, we may
	 * not be able to immediately reconcile all of the file's pages.  If the
	 * pending_retry value is non-zero, it means there are pending requests
	 * we need to handle.
	 */
	int pending_retry;		/* Eviction request needs completion */

	WT_EVICT_REQ evict_request[20];	/* Eviction requests:
					   slot available if session is NULL */

	/*
	 * The I/O thread sets/clears the read_sleeping flag when blocked on the
	 * mtx_read mutex.  The cache thread uses the read_sleeping flag to wake
	 * the I/O thread as necessary.
	 */
	WT_MTX *mtx_read;		/* Cache read server mutex */
	u_int volatile read_sleeping;	/* Sleeping */
	u_int volatile read_lockout;	/* No reading until memory drains */

	WT_READ_REQ read_request[40];	/* Read requests:
					   slot available if session is NULL */

	uint32_t   read_gen;		/* Page read generation (LRU) */

	void	  *rec;			/* Page reconciliation structure */

	/*
	 * Different threads read/write pages to/from the cache and create pages
	 * in the cache, so we cannot know precisely how much memory is in use
	 * at any specific time.  However, even though the values don't have to
	 * be exact, they can't be garbage, we track what comes in and what goes
	 * out and calculate the difference as needed.
	 */
	uint64_t bytes_read;		/* Bytes/pages read by read server */
	uint64_t pages_read;
	uint64_t bytes_workq;		/* Bytes/pages created by workQ */
	uint64_t pages_workq;
	uint64_t bytes_evict;		/* Bytes/pages discarded by eviction */
	uint64_t pages_evict;
};
#if defined(__cplusplus)
}
#endif
