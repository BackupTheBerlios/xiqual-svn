/*
 * Big number calculations for cryptography.
 * Uses 8192-bit math.
 */

#include "stdio.h"
#include "stdlib.h"

// For the carry bit (32-bit unsigned max +1)
#define BASE	0x100000000ULL

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef USHORT
typedef unsigned short USHORT;
#endif

#ifndef ULONG
typedef unsigned long ULONG;
#endif

#ifndef UDOUBLE
typedef unsigned long long UDOUBLE;
#endif

#define DIGITS(num)	((ULONG)*(*num))
#define SETDIGITS(num, l)	(*(*num) = (ULONG)(l))
#define INCDIGITS(num)	(++*(*num))
#define DECDIGITS(num)	(--*(*num))
#define REMOVEZEROS(num) \
	while((DIGITS(num) > 0) && (*MSDPTR(num) == 0)) {DECDIGITS(num);}

// Least significant digit
#define LSDPTR(num) ((*num) + 1L)
// Most significant digit
#define MSDPTR(num) ((*num) + DIGITS(num))

#define BIGNUM_BITS 32UL
#define BIGNUM_LDBITS 8U
#define BIGNUM_NUMREGS 16U
#define BIGNUM_MAXDIGITS 128U
#define BIGNUM_MAXLONG BIGNUM_MAXDIGITS+1
#define BIGNUM_MAXBITS BIGNUM_MAXDIGITS<<5

typedef ULONG BigNum[BIGNUM_MAXDIGITS+1];
typedef ULONG BigNumD[(BIGNUM_MAXDIGITS<<1)+1];
typedef ULONG BigNumQ[(BIGNUM_MAXDIGITS<<2)+1];



BigNum *num_allocnum()
{
	return calloc(1, sizeof(BigNum));
}

void num_freenum(BigNum *num)
{
	ULONG count;

	count = BIGNUM_MAXDIGITS+1;
	while(count) {
		(*num)[count] = 0;
		count--;
	}
	free(num);
}

void num_add(BigNum *a, BigNum *b, BigNum *sum)
{
	UDOUBLE carry=0;
	ULONG *msda, *msdb;
	ULONG *aptr, *bptr;
	ULONG *sptr = LSDPTR(sum);

	if((*a)[0] < (*b)[0]) {
		aptr = LSDPTR(b);
		bptr = LSDPTR(a);
		msda = MSDPTR(b);
		msdb = MSDPTR(a);
		SETDIGITS(sum, DIGITS(b));
	} else {
		aptr = LSDPTR(a);
		bptr = LSDPTR(b);
		msda = MSDPTR(a);
		msdb = MSDPTR(b);
		SETDIGITS(sum, DIGITS(a));
	}
	while (bptr <= msdb) {
		*(sptr++) = (ULONG)(carry = (UDOUBLE)(*aptr++)
			+ (UDOUBLE)*(bptr++)
			+ (UDOUBLE)(ULONG)(carry >> BIGNUM_BITS));
    }
	while (aptr <= msda) {
		*(sptr++) = (ULONG)(carry = (UDOUBLE)*(aptr++)
			+ (UDOUBLE)(ULONG)(carry >> BIGNUM_BITS));
    }
	if (carry & BASE) {
		*(sptr) = 1;
		INCDIGITS(sum);
    }
}

void num_sub(BigNum *a, BigNum *b, BigNum *diff)
{
	UDOUBLE carry=0;
	ULONG *msda, *msdb;
	ULONG *aptr, *bptr, *dptr;
	ULONG *sptr = LSDPTR(diff);

	aptr = LSDPTR(a);
	bptr = LSDPTR(b);
	dptr = LSDPTR(diff);
	msda = MSDPTR(a);
	msdb = MSDPTR(b);

	SETDIGITS(diff, DIGITS(a));
	while(bptr <= msdb) {
		*(dptr++) = (ULONG)(carry = (UDOUBLE)*(aptr++) - (UDOUBLE)*(bptr++)
			- ((carry & BASE) >> BIGNUM_BITS));
	}

	while(aptr <= msda) {
		*(dptr++) = (ULONG)(carry = (USHORT)*(aptr++)
			- ((carry & BASE) >> BIGNUM_BITS));
	}

	REMOVEZEROS(diff);
}

char *num2hexstr(BigNum *num, char *buf)
{
	ULONG count;
	char tmp[2000];

	count = (*num)[0];
	buf[0] = 0;
	while(count) {
		if(count == (*num)[0]) {
			sprintf(tmp, "%x", (*num)[count]);
		} else {
			sprintf(tmp, "%08x", (*num)[count]);
		}
		strcat(buf, tmp);
		count--;
	}
	return buf;
}

int main()
{
	BigNum one, two, three;
	char buf[2000];

	one[0] = 2;
	one[1] = 0xffffffff;
	one[2] = 0xffffffff;
	one[3] = 0;
	one[4] = 0;

	two[0] = 1;
	two[1] = 2;

	printf("BigNum struct size is %d bytes\n", sizeof(BigNum));
	num2hexstr(&one, buf);
	printf("Var one is 0x%s\n", buf);
	num2hexstr(&two, buf);
	printf("Var two is 0x%s\n", buf);
	num_add(&one, &two, &three);
	num2hexstr(&three, buf);
	printf("Sum of 'one' plus 'two' is 0x%s\n", buf);
	num_sub(&three, &one, &two);
	num2hexstr(&two, buf);
	printf("Difference of 'three' minus 'one' = 0x%s\n", buf);
	return 0;
}
