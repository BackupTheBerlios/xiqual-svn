/*
 * EA IFF 85 minimal (mostly) compliant parser.
 * Supports reading and writing of chunks.
 * IFF (or, rather, IFF85) is Electronic Arts' good, old
 * Interchange Format File specification. It is still used
 * in a mutated form in their recent games.
 * The newest variation has an awfully long header, though.
 *
 * Part of libxitools, an increasingly more ambitious library project.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 * The structure of this reader is based on Jörg Dietrich's c_iff library;
 * no code has been lifted from it, though. Lots of minor differences; also prepared
 * for future handling à là IFFparse.library. 
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: iffparse.h,v 1.17 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: iffparse.h,v $
 * Revision 1.17  2003/12/11 08:36:38  nny
 *
 * Modified Files:
 * All.
 *
 * Revision 1.16  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 *
 */

#if !defined(IFFPARSE_H)
#define IFFPARSE_H

#include "lists.h"
#include "stdlib.h"
#include "xiendian.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/*
 * The universal IFF identifiers
 * Remember: All these IDs are in BIG endian format in the files;
 * use htonl() before writing the ID, and ntohl() after reading it.
 * The library will pass IDs in local format to your callbacks, so
 * this should normally not be an issue.
 */

/**
 Quick macro to make a ulong of four characters.
@doc
 This only creates them in internal order. You still need to convert
 to big-endian format before writing it to an IFF.

 The first character is the leftmost in a string; little-endian machines
 will store this backwards, so _SwapBE32() will be required.
@param a First character.
@param b Second character.
@param c Third character.
@param d Fourth character.
@see _SwapBE32()
*/
#define MAKE_ID(a,b,c,d)	\
	((ulong) (a)<<24 | (ulong) (b)<<16 | (ulong) (c)<<8 | (ulong) (d))

#define ID_FORM	MAKE_ID('F','O','R','M')
#define ID_LIST	MAKE_ID('L','I','S','T')
#define ID_CAT	MAKE_ID('C','A','T',' ')
#define ID_PROP	MAKE_ID('P','R','O','P')
#define ID_NULL	MAKE_ID(' ',' ',' ',' ')		/* Four spaces (filler; skip) */

/**
 Handle returned by iff_new() and iff_open() calls.
@doc
 All reading/writing functions need this to see
 where they are, and to store temporary nodes
 while building an IFF.
@author Ronny Bangsund
@see iff_new(),  iff_open()
*/
typedef struct IFFHandle {
/**
*/
	FILE	*iff;
/**
 FORM type.
@doc
 See the IFF85 text for a thorough introduction.
*/
	ulong	type; /* Type of file */
/**
 Identifier, somewhat less strict.
@doc
 Used by the reading functions. Soon to be replaced.
*/
	ulong	id; /* Current chunk's ID */
/**
 Size of current chunk.
@doc
 Used by the reading functions. Soon to be replaced.
*/
	ulong	chunksize;
/**
 Makes the reader able to scan backwards.
@doc
 Used by the reading functions. Soon to be replaced.
*/
	ulong	prevchunksize;	/* Eases backwards-searches, making it more useful for random access */
/**
 Size of the IFF.
*/
	ulong	size;
/**
 Position in file.
@doc
 Usually the result of an ftell().
*/
	ulong	pos;
/**
 Linked list of chunk context nodes.
@doc
 Not used by the reader functions yet. The API is more or less
 locked down, but behind the scenes, there is some inconsistency
 between reading and writing of IFF. Fixing it soon.
*/
	List	chunks;
/**
 Are we in write mode?
@doc
 iff_close() checks this to see if the final filesize needs
 to be corrected, and if chunks are to be finalised.
*/
	char	write;
} IFFHandle;


typedef struct IFFChunk {
	struct IFFChunk *next, *prev;
	ulong	type; /* Blank if not a subform */
	ulong	id;
	ulong	size; /* Increases as we iff_write() to this chunk */
	ulong	pos; /* At what point we start in the file */
} IFFChunk;


/* For the new iffp_* interface, if I ever write it */
/*
 IFF structure for IFF parsing interface v2.
@doc
 This structure is an improvement of the previous interface,
 with proper context-based chunk handling and hooks.
@author Ronny Bangsund
@see iffp_new()
*/
typedef struct IFF {
	void	*stream;	/* Data specific to your read/write handler */
	ulong	flags;
//	ulong	depth;	/* Depth of context stack */
	List	chunks;
/*
 Stream handler hook pointer.
@param command Operation to be performed (IFFCMD_).
@param buf Pointer to the data buffer.
@param len Number of bytes to be handled.
*/
	void	(*streamhandler)(ulong command, char *buf, size_t len);
	void	(*readhook)(char *buf, size_t len);
	void	(*writehook)(char *buf, size_t len);
} IFF;

/* Flags */
enum {
	IFF_READ=0,
	IFF_WRITE,
	IFF_FSEEK,	/* Forward seek only */
	IFF_RSEEK	/* Random seek available */
};

/*
 A context on the IFF stack.
@doc
 Each node represents one chunk in an IFF.
 This structure is only allocated by libxitools when required;
 you will be passed these from the various iffp_ functions.
*/
typedef struct ContextNode {
/*
*/
	struct ContextNode	*next;
/*
*/
	struct ContextNode	*prev;
/*
 IFF chunk ID.
*/
	ulong	id;
/*
 IFF FORM type.
*/
	ulong	type;
/*
 Size of this chunk's data.
*/
	size_t	size;
/*
 Number of bytes read/written.
*/
	size_t	scan;
/*
*/
	List	local;
} ContextNode;


extern IFFHandle *iff_open(char *name);
extern void iff_close(IFFHandle *handle);
extern int iff_check(IFFHandle *handle);
extern int iff_readchunkheader(IFFHandle *handle);
extern size_t iff_readchunkdata(IFFHandle *handle, char *buffer, size_t size);
extern void iff_skipchunkdata(IFFHandle *handle);

extern IFFHandle *iff_new(char *name, ulong type);
extern int iff_newsubform(IFFHandle *handle, ulong type);
extern void iff_endsubform(IFFHandle *handle);
extern int iff_newchunk(IFFHandle *handle, ulong id);
extern size_t iff_writechunkdata(IFFHandle *handle, char *buffer, size_t size);
extern void iff_endchunk(IFFHandle *handle);

extern char *IDtoStr(ulong id, char *buf);
extern void iff_fixsize(IFFHandle *handle);
extern int iff_goodID(ulong id);
extern int iff_goodtype(ulong type);

extern void iff_rewind(IFFHandle *handle);
extern void iff_correctchunk(IFFHandle *handle, IFFChunk *chunk);


#ifdef __cplusplus
}
#endif

#endif /* EOF */
