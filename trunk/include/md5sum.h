/*
 * MD5 calculation
 *
 * $Author: nny $
 * $Date: 2003/12/11 08:36:38 $
 * $Id: md5sum.h,v 1.7 2003/12/11 08:36:38 nny Exp $
 *
 * $Log: md5sum.h,v $
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

#if !defined(MD5_H)
#define MD5_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __alpha
typedef unsigned int uint32;
#else
typedef unsigned long uint32;
#endif

/**
 MD5 context for the Rivest/Plumb MD5 checksumming routines.
@author Ron Rivest (algorithm), Colin Plumb (code)
*/
struct MD5Context {
	uint32 buf[4];
	uint32 bits[2];
	unsigned char in[64];
};

extern void MD5Sum(unsigned char *key, unsigned char *buf, unsigned len);
extern void MD5Ascii(unsigned char *key, unsigned char *ascii);
extern void MD5Init(struct MD5Context *context);
extern void MD5Update(struct MD5Context *context, unsigned char const *buf, unsigned len);
extern void MD5Final(unsigned char digest[16], struct MD5Context *context);
extern void MD5Transform(uint32 buf[4], uint32 const in[16]);

typedef struct MD5Context MD5_CTX;

#ifdef __cplusplus
}
#endif

#endif /* EOF */
