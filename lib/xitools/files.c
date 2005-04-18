/*
 * Automagic files:Implementation.
 * Load on demand & automagically deallocate data.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:54:55 $
 * $Id: files.c,v 1.4 2003/12/11 08:54:55 nny Exp $
 *
 * $Log: files.c,v $
 * Revision 1.4  2003/12/11 08:54:55  nny
 *
 * Modified Files:
 * All.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"

/**@name <div class="keyword">Autofiles<span>Somewhat simpler loading of files</span></div>
 Automagic file-handling.
@doc
 Well, sorta..you can set an autofile's read hook to do
 funky things while a file is loading piece by piece. It was
 originally meant to be used more throughout Xiqual, but has
 not, for some reason. It would be useful to let higher level
 functions accept an autofile with its hooks in place, so that
 you can do some extra "magic" while a function still does its
 manipulation.
*/


/*@{*/
/**
 Open an autofile.
@doc
 This function will open an existing file.
 af->size will contain the filesize.
@param filename Name of a file to open/create.
@return If the file exists or was created, return an autofile pointer.
@author Ronny Bangsund
@see autofile, file_read(), file_scan(), file_seek(), file_free()
*/
autofile *file_new(char *filename)
{
	autofile *af;

	af = calloc(1, sizeof(autofile));
	if (af) {
		af->name = strdup(filename);
		if (!af->name) {
			free(af);
			af = 0;
		}
		af->size = file_getsize(af->name);

	} /* if (af) */
	return af;
}


/**
 Close autofile and free all buffers.
@doc
 Deallocates all structure members and the autofile after an fclose().
@author Ronny Bangsund
@see autofile, file_new(), fclose()
*/
void file_free(autofile *af)
{
	if(!af) return;

	if (af->f) fclose(af->f);
	if(af->name) free(af->name);
	if(af->buf) free(af->buf);
	free(af);
}


/**
 Allocate a buffer of the given size.
@doc
@param af Autofile to create the buffer in. Only one buffer is allowed.
@param s Number of bytes. Most programs can get away with a few kiloytes.
 Bigger buffers tend to get written from an operating system's cache quicker,
 but it's still no guarantee. Larger buffers mean more efficient read
 operations, though.
@author Ronny Bangsund
@see autofile, file_new(), file_read()
*/
size_t file_initbuf(autofile *af, size_t s)
{
	af->buf = calloc(1, s);
	if (af->buf) {
		af->bufsize = s;
		return s;
	} else {
		return 0;
	}
}


/**
 Load an entire file into memory.
@doc
 Typically used on textfiles. If there is a readhook,
 it will be called on the entire file before returning.
 An extra byte will be allocated to ensure NULL termination.
@author Ronny Bangsund
@see autofile, file_new(), file_initbuf(), file_read(), file_free()
*/
size_t file_load(autofile *af)
{
	if (!af) return 0;

	/* Allocate a buffer for the entire file, plus space for NULL byte */
	if (!af->buf) {
		if (!file_initbuf(af, af->size+1)) return 0;
	}

	if (!af->f) af->f = fopen(af->name, "rb");

	if (af->f) {
		af->read = file_read(af, af->size);
		fclose(af->f);
		af->pos = 0; /* We've closed the file, so we're logically at the beginning */
		af->f = 0;
	} else {
		af->read = 0;
	}
	return af->read;
}


/**
 Seek-wrapper for SEEK_SET functionality.
@doc
 Seek to a specified position from the beginning of the file.
 The pos member of struct autofile is updated.
@param af The autofile to seek in.
@param pos Exact position in file to go to. All reads will continue from there.
@return Current position.
@author Ronny Bangsund
@see autofile, file_new()
*/
size_t file_seek(autofile *af, size_t pos)
{
	af->pos = fseek(af->f, pos, SEEK_SET);
	return af->pos;
}


/**
 Seek-wrapper for SEEK_CUR functionality.
@doc
 Seek from the current position to skip bytes later in the file.
 The pos member of struct autofile is updated.
@param af The autofile to seek in.
@param skip Skip this many bytes towards the end of the file from current position.
@return Current position.
@author Ronny Bangsund
@see autofile, file_new()
*/
size_t file_scan(autofile *af, size_t skip)
{
	size_t ret;
	ret = fseek(af->f, skip, SEEK_CUR);
	af->pos = ret;
	return ret;
}


/**
 Read a number of bytes into the buffer from an autofile.
@doc
 If the read operation works fine, the read hook will be
 called, if one exists. It will not read more than af->bufsize per read.
@param af An autofile structure. The file will be opened if not already open.
@param s Number of bytes to read.
@return Number of bytes actually read. This is also passed to the hook.
@author Ronny Bangsund
@see autofile, file_new(), file_readhook()
*/
size_t file_read(autofile *af, size_t s)
{
	if (!af->buf)
		file_initbuf(af, s);

	if (!af->f)
		af->f = fopen(af->name, "rb");

	if(!af->f)
		return 0;

	if (s > af->bufsize) {
		af->read = fread(af->buf, 1, af->bufsize, af->f);
	} else {
		af->read = fread(af->buf, 1, s, af->f);
	}

	if(af->readhook) {
		af->readhook(af->buf, af->read);
	}
	af->pos += af->read;
	return af->read;
}


/**
 Set an autofile's read hook pointer.
@param af An autofile.
@param readhook Your hook, or NULL to clear.
@author Ronny Bangsund
@see autofile, file_new(), file_read()
*/
void file_readhook(autofile *af, void (*readhook)(char *buf, size_t len))
{
	if(!af)
		return;

	af->readhook = readhook;
}


/*@}*/
