/*
 * Automagic filehandling:structures & definitions.
 * Load on demand & automagically deallocate data.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: files.h,v 1.7 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: files.h,v $
 * Revision 1.7  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.6  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.3  2001/11/13 19:26:33  nny
 * Removed tag_inarray.c, as it was just a waste of space.
 * Added CVS variables to most files.
 *
 */

#if !defined(XIQUAL_FILES_H)
#define XIQUAL_FILES_H

#include "xios.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 Autofile structure.
@doc
 Use file_new() to create a structure with a filename.
 The other file_* operations can be used to read, write
 and seek in the file.
@author Ronny Bangsund
@see file_new(), file_free(), file_getsize(), file_initbuf(), file_load(),
 file_seek(), file_scan(), file_read(), file_write(), file_readhook(),
 file_writehook()
*/
typedef struct autofile {
/**
 Size of file.
@doc
 Writing operations will add the number of bytes written from
 each call.
*/
	size_t	size;
/**
 Current position in an open file.
@doc
 This usually equals size with a file being written to.
*/
	size_t	pos;
/**
 Number of bytes read in last read operation.
@doc
 The pos element will point to the point after last byte read.
 Start point of the last read chunk of data is pos - read.
*/
	size_t	read;
/**
 Name of file.
@doc
 No special checking is made on this name to ensure it is valid
 for the OS to create.
*/
	char	*name;
/**
 A buffer used in file_read() operations.
@doc
 You must create this buffer with file_initbuf(). The buffersize
 shouldn't need to be larger than the size of a file.
*/
	char	*buf;
/**
 Size of buf element.
@doc
 file_read() will not read more than this much per call.
 The previous buffer contents are of course lost. No special
 care is made to clear the contents before reading, so do not
 expect everything to be nicely NULL-terminated.
*/
	size_t	bufsize; /* Maximum number of bytes to read into buffer */
/**
 The actual FILE pointer.
@doc
 For internal use.
*/
	FILE	*f;
/**
 Hook called when reading.
@doc
 This is called right after reading in all data and ensuring anything
 was read. See md5.c in the base Xiqual directory for an example of
 simple usage of a hook.
*/
	void	(*readhook)(char *buf, size_t len);
} autofile;

extern autofile *file_new(char *filename);
extern void file_free(autofile *af);
extern size_t file_getsize(char *name);
extern size_t file_initbuf(autofile *af, size_t s);
extern size_t file_load(autofile *af);
extern size_t file_seek(autofile *af, size_t pos);
extern size_t file_scan(autofile *af, size_t skip);
extern size_t file_read(autofile *af, size_t s);
extern void file_readhook(autofile *af, void (*readhook)(char *buf, size_t len));

#ifdef __cplusplus
}
#endif

#endif /* EOF */
