#ifndef _CTB_FS_H
#define _CTB_FS_H

#include <stdint.h>

#include <sys/stat.h>
#if defined(_WIN32)
	#define CTB_FS_PATH_SEPARATOR	'/'
	#define	_ctb_fs_stat			_stat
	#define	_ctb_fs_stat_struct		struct _stat
	#define	_ctb_fs_s_ifdir			_S_IFDIR
	#define	_ctb_fs_s_ifreg			_S_IFREG
	#include <direct.h>
#else
	#define CTB_FS_PATH_SEPARATOR	'\\'
	#define	_ctb_fs_stat			stat
	#define	_ctb_fs_stat_struct		struct stat
	#define	_ctb_fs_s_ifdir			S_IFDIR
	#define	_ctb_fs_s_ifreg			S_IFREG
	#include <unistd.h>
#endif

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
	uint64_t			size;
	void				*handle;
	_ctb_fs_mask		mask;
}	ctb_file;


#ifdef CTB_FS_NOPREFIX


#endif


#endif	// !_CTB_FS_H

#ifdef CTB_FS_IMPLEMENTATION

static char * _ctb_basename(char *path)
{
	if (!path) return NULL;
	char *base = strrchr(path, CTB_FS_PATH_SEPARATOR);
	return (base ? base + 1 : path);
}

int	ctb_fs_is_file(ctb_file *file) { return (file && (file->mask & CTB_FS_FILE)); }
int	ctb_fs_is_dir(ctb_file *file) { return (file && (file->mask & CTB_FS_DIRECTORY)); }
int	ctb_fs_is_symlink(ctb_file *file) { return (file && (file->mask & CTB_FS_SYMLINK)); }
int	ctb_fs_is_open(ctb_file *file) { return (file && (file->mask & CTB_FS_IS_OPEN)); }
int	ctb_fs_can_read(ctb_file *file) { return (file && (file->mask & CTB_FS_PERM_READ)); }
int	ctb_fs_can_write(ctb_file *file) { return (file && (file->mask & CTB_FS_PERM_WRITE)); }
int	ctb_fs_exists(ctb_file *file)
{
	_ctb_fs_stat_struct s;
	return (_ctb_fs_stat(path, &s) == 0);
}

int	ctb_fs_delete(char *path) { return (remove(path)); }
int	ctb_fs_move(char *oldpath, char *newpath) { return (rename(oldpath, newpath)); }
ctb_file ctb_fs_open(const char *path, _ctb_fs_mask mode)
{
	ctb_file f = {0};
	f.path = strdup(path);
	f.base = _ctb_basename(path);
	f.mask = mode;
}


#endif

