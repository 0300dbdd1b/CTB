#ifndef _CTB_FS_H
#define _CTB_FS_H

typedef enum _ctb_fs_mask
{
	CTB_FS_NONE			= 0,
	CTB_FS_FILE			= (1 << 0),
	CTB_FS_DIRECTORY	= (1 << 1),
	CTB_FS_SYMLINK		= (1 << 2),
	CTB_FS_OTHER		= (1 << 3),

	CTB_FS_PERM_READ	= (1 << 4),
	CTB_FS_PERM_WRITE	= (1 << 5),
	CTB_FS_PERM_EXEC	= (1 << 6),
	CTB_FS_PERM_RW		= (CTB_FS_PERM_READ | CTB_FS_PERM_WRITE),
	CTB_FS_PERM_RX		= (CTB_FS_PERM_READ | CTB_FS_PERM_EXEC),
	CTB_FS_PERM_WX		= (CTB_FS_PERM_WRITE | CTB_FS_PERM_EXEC),
	CTB_FS_PERM_RWX		= (CTB_FS_PERM_READ | CTB_FS_PERM_WRITE | CTB_FS_PERM_EXEC),
	
	CTB_FS_IS_OPEN		= (1 << 7),
	CTB_FS_OPEN_READ	= (1 << 8),
	CTB_FS_OPEN_WRITE	= (1 << 9),
	CTB_FS_OPEN_APPEND	= (1 << 10),
	CTB_FS_OPEN_TRUNC	= (1 << 11),
	CTB_FS_OPEN_BINARY	= (1 << 12)
		
}	_ctb_fs_mask;

typedef struct ctb_file
{
	char				*path;
	char				*basename;
	_ctb_fs_mask		mask;
}	ctb_file;


#ifdef CTB_FS_NOPREFIX

ctb_file	ctb_fs_open_file(const char *path | )
{

}

#endif



#ifdef CTB_FS_IMPLEMENTATION
	
#endif


#endif	// !_CTB_FS_H
