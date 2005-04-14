/*
 * Storing XML, or printing to stdout.
 * Timesaving functions.
 */

#include "xixml.h"

size_t xml_savetag(FILE *stream, xmltag *tag)
{
	struct xmltag *t;
	variable *var;
	size_t count=0;

	t = tag;
	while(t) {
		if(t->type == XML_TEXT) {
			count += fprintf(stream, "%s", t->name);
		} else {
			count += fprintf(stream, "<%s", t->name);
		}
		if(t->attribs.head) {
			var = (variable *)t->attribs.head;
			while(var) {
				count += fprintf(stream, " %s=\"%s\"", var->name, var->string);
				var = var->next;
			}
		}
		if(t->children.head) {
			if(t->type != XML_TEXT) count += fprintf(stream, ">");
			count += xml_savetag(stream, (struct xmltag *)t->children.head);
			if(t->type != XML_TEXT) count += fprintf(stream, "</%s>", t->name);
		} else {
			if((t->empty) && (t->type != XML_TEXT)) {
				count += fprintf(stream, "/>");
			} else {
				if(t->type != XML_TEXT) count += fprintf(stream, "</%s>", t->name);
			}
		}
		t = t->next;
	}
	return count;
}


/**
 Save an XML document to a file.
@doc
 This saves the document to file, returning how many bytes were
 written, or -1 if there is an error.

 If the filename is "*" (an asterisk), the document is written
 to stdout. Number of bytes written is returned as normal.
@param doc XML document structure created with xml_newdoc()
@param name Filename to create, or an asterisk for stdout
return Number of bytes written to the stream
@author Ronny Bangsund
@see xml_loaddoc(), xml_newdoc(), xmldoc
*/
size_t xml_savedoc(struct xmldoc *doc, char *name)
{
	size_t count=0;
	FILE *file;

	if(strcmp(name, "*")==0) {
		file = stdout;
	} else {
		file = fopen(name, "w");
		if(!file) return -1;
	}
	count = xml_savetag(file, doc->root);
	if(file != stdout) fclose(file);
	return count;
}
