/*
 * Insanely non-validating XML reader.
 * Slowly becoming more and more like the
 * specs dictate for XML 1.0. Can handle
 * XHTML of all variations tried so far.
 * May or may not choke (and bail out) on
 * badly formed XML - more error messages
 * are added now and then.
 */

#include "xixml.h"
#include "xios.h"
#include "xiqual.h"


static char textnodememerr[] = "Couldn't allocate memory for textnode";
static char tagnodememerr[] = "Couldn't allocate memory for tagnode";
static char xmldocmemerr[] = "Couldn't allocate memory for XML document";
static char xmldocctxmemerr[] = "Couldn't allocate memory for context";
static char xmldocdeclerr[] = "Malformed <?xml declaration";
static char xmldocnorooterr[] = "Document has no root element";
static char xmldocnotfinishederr[] = "Tag missing >";
static char xmldocnotclosederr[] = "Tag not closed or stray close tag";
static char xmldocstraytagerr[] = "Stray tag outside root element";
static char xmldocstraycloseerr[] = "Stray closing tag - check spelling";
static char xmldocnofileerr[] = "No file, file protected or file empty";


/**
 Free an XML document.
@param doc An xmldoc structure
@author Ronny Bangsund
@see xml_newdoc(), xml_loaddoc(), xmldoc
*/
void xml_freedoc(struct xmldoc *doc)
{
	struct hook *hook;

	xml_freetag(doc->root);
	while((hook = (struct hook *)doc->hooks.head)) {
		if(hook->name) free(hook->name);
		if(hook->id) free(hook->id);
		list_remove(&doc->hooks, (Node *)hook);
		free(hook);
	}
	if(doc->encoding) free(doc->encoding);
	free(doc);
}


/*
 * Tokenise attributes. Somewhat malformed
 * attributes won't mess up completely.
 * They just won't be included.
 */
void xml_getattribs(struct xmltag *tag, char *s)
{
	char *start, *end, *name, *value;
	char buf[2];

	start = s;
	buf[1] = 0;
	while(start[0]) {
		while(isspace(start[0])) {
			/* Skip whitespace */
			start[0] = 0;
			start++;
		}
		end = strstr(start, "=");
		if(!end) return;

		end[0] = 0; end++;
		name = start;
		/* XML 1.0 allows double or single quotes.
		 * Grab whatever comes after the equals sign,
		 * as that will be the quote used. Note that
		 * this parser expects no spaces. This seems
		 * to be what the XML syntax requires, too.
		 */
		while(end[0]) {
			if((end[0] == 34) || (end[0] == 39)) {
				buf[0] = end[0];
				end++;
				value = end;
				end = strstr(end, buf);
				if(!end) return;

				end[0]=0;
				/* We let the user handle datatypes,
				 * so the variable will always be a string.
				 */
				(void)cfg_newvar(PREFS_NAME, name,
					VAR_TYPE, V_STRING,
					VAR_VALUE, value,
					PREFS_LIST, &tag->attribs,
					TAG_END);
			} else {
				end++;
			}
		}
		start = end; start++;
	}
}


char *xml_newctx(char *ancestors, char *parent)
{
	char	*ctx=0;
	size_t	len=0;

	if(!parent) return NULL;

	if(ancestors) len = strlen(ancestors);
	len += strlen(parent) + 3;
	ctx = calloc(1, len);
	if(!ctx) {
		xi_lasterror = xmldocctxmemerr;
		return NULL;
	}
	/* Double colons shouldn't be reserved in XML */
	if((ancestors) && (ancestors[0])) {
		strcpy(ctx, ancestors);
		strcat(ctx, "::");
	}
	strcat(ctx, parent);
	return ctx;
}


void xml_handle_tag(xmldoc *doc, char *text)
{
	size_t len;
	ulong empty=0;
	xmltag *tag;

	len = strlen(text)-1;
	if(text[len] == '/') {
		text[len] = 0;
		empty = 1;
	}
	tag = xml_newtag(text);
	if(!tag) return;

	tag->empty = empty;
	if(doc->root) {
		list_add(&doc->current->children, (Node *)tag);
		tag->parent = doc->current;
		tag->context = xml_newctx(tag->parent->context, tag->parent->name);
	} else {
		doc->root = tag;
	}
	if(!empty) doc->current = tag;
}


struct xmltag *xml_newtext(char *text)
{
	struct xmltag *tag;

	if((!text) || (text[0] == 0)) {
		xi_seterror(textnodememerr);
		return NULL;
	}

	tag = calloc(1, sizeof(struct xmltag));
	if(!tag) {
		xi_seterror(textnodememerr);
		return NULL;
	}

	tag->name = text;
	tag->type = XML_TEXT;
//	tag->context = xml_newctx(tag->parent->context, tag->parent->name);
	return tag;
}


struct xmltag *xml_newtag(char *name)
{
	struct xmltag *tag;
	char *ptr, *attr;

	tag = calloc(1, sizeof(struct xmltag));
	if((!name) || (!tag)) {
		xi_seterror(tagnodememerr);
		return NULL;
	}

	attr = name;
	while(attr[0]) {
		if(isspace(attr[0])) {
			attr[0] = 0;
			ptr = attr; ptr++;
			xml_getattribs(tag, ptr);
		}
		attr++;
	}
	tag->name = strdup(name);
	if(!tag->name) {
		xi_seterror(tagnodememerr);
		free(tag);
		return NULL;
	}
	tag->type = XML_TAG;
	return tag;
}


struct xmltag *xml_freetag(struct xmltag *tag)
{
	struct xmltag *child;
	struct variable *var;

	if(tag->name) free(tag->name);
	if(tag->context) free(tag->context);
	while((child = (struct xmltag *)tag->children.head)) {
		list_remove(&tag->children, (Node *)child);
		xml_freetag(child);
	}
	while((var = (struct variable *)tag->attribs.head)) {
		list_remove(&tag->attribs, (Node *)var);
		cfg_freevar(var);
	}
	free(tag);
	return NULL;
}


struct xmldoc *xml_tok(struct xmldoc *doc, char *buf)
{
	struct xmltag *tag;
	char *start=0, *end=0, *header=0;
	char charbuf[2], *name;
	struct Node *n;
	int opentags=0, len=0;
	struct strtoken *tok;

	if(!doc) return NULL;

	start = strstr(buf, "<");
	if(!start) {
		xml_freedoc(doc);
		return NULL;
	}

	end = start;
	charbuf[1] = 0;
	/* Get the XML header/declaration and version.
	 * Note the space!
	 * A hook for <?xml-* tags should be added later.
	 */
	if(strncmp(start, "<?xml ", 6)==0) {
		end = strstr(start, "?>");
		if(!end) {
			xi_seterror(xmldocdeclerr);
			xml_freedoc(doc);
			return NULL;
		}
		end[0] = 0; end += 2;
		start += 6;
		tok = str_tokenise(start, " \n\t\r");
		if(tok) {
			n = tok->tokens.head;
			while(n) {
				if(strncmp(n->data, "version", 7)==0) {
					start = n->data;
					start += 9;
					doc->version = strtol(start, &start, 10);
					if(start[0] == '.') doc->revision = strtol(start+1, NULL, 10);
				}
				if(strncmp(n->data, "encoding", 8)==0) {
					start = n->data; start += 9;
					charbuf[0] = start[0];
					start[0] = 0; start++;
					header = strstr(start, charbuf);
					header[0] = 0;
					if(header) doc->encoding = strdup(start);
				}
				n = n->next;
			}
			str_freetoken(tok);
		}
	}
	start = end;

	/* Empty documents not allowed */
	start = strstr(start, "<");
	if(!start) {
		xi_seterror(xmldocnorooterr);
		return doc;
	}

	while(start[0]) {
		tag = 0;
		if(start[0] == '<') {
			/* Mark the end of this tag */
			end = strstr(start, ">");
			/* Missing brackets are a serious problem - die! */
			if(!end) {
				xi_seterror(xmldocnotfinishederr);
				xml_freedoc(doc);
				return NULL;
			}

			end[0] = 0; end++;

			/* Handle special tags (just removing for now) */
			if((strncmp(start, "<?", 2)==0) ||
				(strncmp(start, "<!", 2)==0) ) {
				start = end;
				continue;
			}

			if(strncmp(start, "</", 2)==0) {
				if(((!opentags) || (!doc->current->parent)) &&
					(doc->current != doc->root)) {
					xi_seterror(xmldocstraycloseerr);
					xml_freedoc(doc);
					return NULL;
				}

				start += 2;
				if(strncmp(doc->current->name, start, strlen(start)) != 0) {
					xi_seterror(xmldocnotclosederr);
					xml_freedoc(doc);
					return NULL;
				}

				opentags--;
				doc->current = doc->current->parent;
				start = end;
				continue;
			}

			start++;
			if(doc->root) opentags++;
			xml_handle_tag(doc, start);
			start = end;
		} else {
			/* We'll add this as a textnode, unless we
			 * are out of tags. */
			end = strstr(start, "<");
			if(!end) return doc;

			len = end-start;
			/* There might be no current document if
			 * we've just encountered a special tag,
			 * like <! or <?.
			 */
			if((len) && (doc->current)) {
				if(!doc->root) {
					xi_seterror(xmldocstraytagerr);
					xml_freedoc(doc);
					return NULL;
				}
				name = strndup(start, len);
				tag = xml_newtext(name);
				if(!tag) return doc;

				list_add(&doc->current->children, (Node *)tag);
				tag->parent = doc->current;
			}
			start = end;
		}
	}
	return doc;
}


/**
 Creates an xmldoc structure from a string buffer.
@doc
 This is one of two possible methods to allocate an XML document
 structure. The other method is a convenience function that loads
 the file into memory.

 If the loader detects unbalanced tags anywhere, it will fail.
 Some binary files could be parsed, but the end result will usually
 be an empty xmldoc structure. Do some checking for overlapping tags,
 unfinished opening tags etc.

 NOTE: The buffer will be modified.
@param buf NULL-terminated memory with XML-like text
@param name Name for the document. If called from xml_loaddoc(),
 the name will contain whatever path was also specified.
@return Pointer to an xmldoc structure; see above for error conditions.
@author Ronny Bangsund
@see xml_loaddoc(), xml_freedoc(), xmldoc
*/
struct xmldoc * xml_newdoc(char *buf, char *name)
{
	struct xmldoc *doc;

	doc = calloc(1, sizeof(struct xmldoc));
	if(!doc) {
		xi_seterror(xmldocmemerr);
		return NULL;
	}

	doc->name = strdup(name);
	if(!doc->name) {
		xi_seterror(xmldocmemerr);
		free(doc);
		xi_seterror("No document name set!");
		return NULL;
	}

	return xml_tok(doc, buf);
}


/**
 Load an XML document and allocate an xmldoc structure.
@doc
 This is the most convenient way to load a document if
 you already have it on disk somewhere.

 NOTE: This reader is very rough, and won't do much beyond
 reading XML 1.0 documents and splitting into a shrubbery
 of nodes. Hook functions will be added as I develop it
 further.

 The current release is guaranteed to work for well-formed
 documents. Validation hooks may come later.

 It's only going to be sufficient for loading of
 simple datafiles for games (mostly configuration files,
 but I see no reason why a bit of Lua code shouldn't be
 possible).

 XML defines all text between opening and closing tags as
 sacred. In other words, every bit of formatting is preserved.
 Hook functions here will allow you to sanitise any data that
 shouldn't be full of newlines and such, for example names.
@param name Filename to load.
@author Ronny Bangsund
@see xml_newdoc(), xml_freedoc(), xmldoc, variable
*/
struct xmldoc * xml_loaddoc(char *name)
{
	autofile *file;
	struct xmldoc *doc;

	file = file_new(name);
	if(!file) {
		xi_seterror(xmldocnofileerr);
		return NULL;
	}

	if(file->size == 0) {
		xi_seterror(xmldocnofileerr);
		return NULL;
	}

	file_load(file);
	if(!file->buf) {
		xi_seterror(xmldocmemerr);
		file_free(file);
		return NULL;
	}
	doc = xml_newdoc(file->buf, file->name);
	file_free(file);
	return doc;
}
