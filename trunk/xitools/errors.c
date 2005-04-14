/*
 *  Error-reporting for all subsystems goes here.
 *
 * $Author: nny $
 * $Date: 2003/05/30 08:54:58 $
 * $Id: errors.c,v 1.2 2003/05/30 08:54:58 nny Exp $
 *
 * $Log: errors.c,v $
 * Revision 1.2  2003/05/30 08:54:58  nny
 *
 * Modified Files:
 * 	All. Arse.
 *
 *
 */

#include "prefs.h"
#include "xiqual.h"

const char	*xi_lasterror;

static const char *prefs_err[PREFS_NUMERRORS+1] = {
	"No error", "Unknown error", "Memory allocation failed", "No such file", "Couldn't load file",
	"Couldn't change to directory"
};

const char *prefs_error(int n)
{
	if (n > PREFS_NUMERRORS)
		xi_lasterror = prefs_err[PREFS_ERR];

	xi_lasterror = prefs_err[n];
	return xi_lasterror;
}


/**
 A generic way of setting error strings.
@doc
 Sets the xi_lasterror variable to a user-specified string.
 The string is not duplicated, so it must stay alive for
 as long as the program exists (preferably), or at least
 until a new error string is set.

 A near-future version of this call will check for a user-
 supplied hook (passed to xi_init() on startup) to call
 whenever a new error message is passed to it. This hook
 could be used to display error-message boxes or a way to
 solve problems.

 Using GNU gettext() etc. for internationalised, or even
 just English, text strings is of course recommended.
@author Ronny Bangsund
*/
void xi_seterror(char *text)
{
	xi_lasterror = text;
}
