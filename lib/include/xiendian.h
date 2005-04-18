/*
 * Bigendian to and from smallendian conversions.
 * All this code under the LGPL, since some was lifted from SDL ;)
 * See http://libsdl.org
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:29 $
 * $Id: xiendian.h,v 1.8 2003/05/30 08:54:29 nny Exp $
 *
 * $Log: xiendian.h,v $
 * Revision 1.8  2003/05/30 08:54:29  nny
 *
 * Modified Files:
 * 	All files. Arse.
 *
 * Revision 1.5  2001/11/13 19:26:33  nny
 * Removed tag_inarray.c, as it was just a waste of space.
 * Added CVS variables to most files.
 *
 */

#if !defined(_XIQUAL_ENDIAN_H)
#define _XIQUAL_ENDIAN_H

#include "xios.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _LIL_ENDIAN	1234
#define _BIG_ENDIAN	4321

/* Mr. Lantinga considers this a mess? Hah! */
#if  defined(__i386__) || defined(__ia64__) || defined(WIN32) || \
    (defined(__alpha__) || defined(__alpha)) || \
     defined(__arm__) || \
    (defined(__mips__) && defined(__MIPSEL__)) || \
     defined(__LITTLE_ENDIAN__)
#	define XI_BYTEORDER	_LIL_ENDIAN
#else
#	define XI_BYTEORDER	_BIG_ENDIAN
#endif

#if defined(linux)
#	include <asm/byteorder.h>
#	if defined(__arch__swab16)
#		define XI_Swap16  __arch__swab16
#	endif
#	if defined(__arch__swab32)
#		define XI_Swap32  __arch__swab32
#	endif
#endif /* linux */

/* The next two functions were changed from static __inline__
 * to macros because my VC++ 5.0 installation can't seem to
 * compile it.
 */
#if !defined(XI_Swap16)
#	define XI_Swap16(D) ((D<<8)|(D>>8))
#endif

#if !defined(XI_Swap32)
#define XI_Swap32(D) \
	((D<<24)|((D<<8)&0x00FF0000)|((D>>8)&0x0000FF00)|(D>>24))
#endif

/* Byteswap item from the specified endianness to the native endianness */
#if XI_BYTEORDER == _LIL_ENDIAN
/**
 Swap from little-endian (Intel) 16-bit format to native format.
@doc
 Use if you know for sure that the integer in question is in that format.
 Note that this may also be used to reverse the process.
@see _SwapLE32(), _SwapLE64(), _SwapBE16(), _SwapBE32(), _SwapBE64()
*/
#	define _SwapLE16(X)	(X)
/**
 Swap from little-endian (Intel) 32-bit format to native format.
@doc
 Use if you know for sure that the integer in question is in that format.
 Note that this may also be used to reverse the process.
@see _SwapLE16(), _SwapLE64(), _SwapBE16(), _SwapBE32(), _SwapBE64()
*/
#	define _SwapLE32(X)	(X)
/**
 Swap from little-endian (Intel) 64-bit format to native format.
@doc
 Use if you know for sure that the integer in question is in that format.
 Note that this may also be used to reverse the process.
@see _SwapLE16(), _SwapLE32(), _SwapBE16(), _SwapBE32(), _SwapBE64()
*/
#	define _SwapLE64(X)	(X)
/**
 Swap from big-endian 16-bit format to native format.
@doc
 Use if you know for sure that the integer in question is in that format.
 Note that this may also be used to reverse the process.
@see _SwapLE16(), _SwapLE32(), _SwapLE64(), _SwapBE32(), _SwapBE64()
*/
#	define _SwapBE16(X)	XI_Swap16(X)
/**
 Swap from big-endian 32-bit format to native format.
@doc
 Use if you know for sure that the integer in question is in that format.
 Note that this may also be used to reverse the process.
@see _SwapLE16(), _SwapLE32(), _SwapLE64(), _SwapBE16(), _SwapBE64(), ntohl()
*/
#	define _SwapBE32(X)	XI_Swap32(X)
/**
 Swap from big-endian 64-bit format to native format.
@doc
 Use if you know for sure that the integer in question is in that format.
 Note that this may also be used to reverse the process.
@see _SwapLE16(), _SwapLE32(), _SwapLE64(), _SwapBE16(), _SwapBE32()
*/
#	define _SwapBE64(X)	XI_Swap64(X)
#else
#	define _SwapLE16(X)	XI_Swap16(X)
#	define _SwapLE32(X)	XI_Swap32(X)
#	define _SwapLE64(X)	XI_Swap64(X)
#	define _SwapBE16(X)	(X)
#	define _SwapBE32(X)	(X)
#	define _SwapBE64(X)	(X)
#endif

#ifdef __cplusplus
}
#endif

#endif /* EOF */
