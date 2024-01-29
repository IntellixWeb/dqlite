#ifndef DQLITE_VFS2_H
#define DQLITE_VFS2_H

#include "../include/dqlite.h"

#include <sqlite3.h>

#include <stdint.h>

/**
 * Create a new VFS object that wraps the given VFS object.
 *
 * The returned VFS is allocated on the heap. Its methods are thread-safe if
 * those of the wrapped VFS are, but the methods of the sqlite3_file objects it
 * creates are not thread-safe. Therefore, a database connection that's created
 * using this VFS should only be used on the thread that opened it. The
 * functions below that operate on sqlite3_file objects created by this VFS
 * should also only be used on that thread.
 */
sqlite3_vfs *vfs2_make(sqlite3_vfs *orig, const char *name, unsigned page_size);

/**
 * Retrieve frames that were appended to the WAL by the last write transaction.
 *
 * Call this on the sqlite3_file object for the WAL
 * (SQLITE_FCNTL_JOURNAL_POINTER).
 */
int vfs2_poll(sqlite3_file *file, dqlite_vfs_frame **frames, unsigned *n);

/**
 * Identifying information about a write transaction.
 */
struct vfs2_wal_slice
{
	uint32_t salt[2];
	uint32_t start;
	uint32_t len;
};

/**
 * Retrieve information about frames that the last write transaction appended to
 * the WAL.
 *
 * Call this on the sqlite3_file object for the WAL
 * (SQLITE_FCNTL_JOURNAL_POINTER).
 */
int vfs2_shallow_poll(sqlite3_file *file, struct vfs2_wal_slice *out);

/**
 * Unhide a write transaction that's been committed in Raft, and release the
 * write lock.
 *
 * Call this on the sqlite3_file object for the main file
 * (SQLITE_FCNTL_FILE_POINTER).
 */
int vfs2_apply(sqlite3_file *file);

// TODO access read marks and locks

// TODO access information about checkpoints

// TODO close?

#endif
