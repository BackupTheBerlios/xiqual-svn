#include "xixml.h"


/**
 Print escaped entities.
@doc
 Only &gt;, &lt;, &amp;, &apos; and &quot; are supported for now.
@param txt Pointer to string buffer starting with entity.
@param stream Filestream to the decoded character to.
@return Pointer to first character after entity.
@author Ronny Bangsund
*/
char *xml_printesc(char *txt, FILE *stream)
{
	char *buf = txt;

	if(strncmp(buf, "&gt;", 4)==0) {
		buf += 4;
		fprintf(stream, ">");
		return buf;
	}
	if(strncmp(buf, "&lt;", 4)==0) {
		buf += 4;
		fprintf(stream, "<");
		return buf;
	}
	if(strncmp(buf, "&amp;", 5)==0) {
		buf += 5;
		fprintf(stream, "&");
		return buf;
	}
	if(strncmp(buf, "&apos;", 6)==0) {
		buf += 6;
		fprintf(stream, "'");
		return buf;
	}
	if(strncmp(buf, "&quot;", 6)==0) {
		buf += 6;
		fprintf(stream, "\"");
		return buf;
	}
}
