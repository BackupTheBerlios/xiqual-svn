#if !defined(XIQUAL_XML_H)
#define XIQUAL_XML_H

#include "xios.h"
#include "files.h"
#include "lists.h"
#include "prefs.h"
#include "xistr.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 An XML tag.
@doc
 One of the three core structures in xixml.
@author Ronny Bangsund
*/
typedef struct xmltag {
	struct xmltag *next;
	struct xmltag *prev;
/**
 Opening tag name without angle braces.
@doc
 No closing tag is stored in the document.
*/
	char	*name;
/**
 Current context.
@doc
 This is a string starting with any parents
 in chronological order. This node's name is
 not tacked on; it will only be added to any
 children this node might have.
*/
	char	*context;
/**
 List of variables.
@doc
 These are the XML attributes within the opening tag.
 XML 1.0 defines strict rules for these; all variables must
 have a value.
@see variable
*/
	List	attribs;
/**
 List of nested tags.
*/
	List	children;
/**
 Element type.
@doc
 Valid types: XML_TAG, XML_TEXT, XML_PROCESSING,
 XML_COMMENT and XML_DECLARATION.
*/
	ulong	type;
	struct xmltag *parent;
/**
 Empty indicator.
@doc
 Non-zero if it's an empty tag.
 Used while parsing, mostly.
*/
	int		empty;
} xmltag;


/**
 The XML document structure.
@doc
 All parsing starts here.
*/
typedef struct xmldoc {
/**
 Document name passed at create time.
@doc
 If created via xml_loaddoc(), this contains its full path.
*/
	char	*name;
/**
 Document encoding used.
@doc
 Not actually implemented yet, but will contain ISO8859-1
 or similar keywords for use whenever the parser supports
 different encodings.
*/
	char	*encoding;
	int		version;
	int		revision;
/**
 Document flags.
@doc
 For future use.
*/
	ulong	flags;
/**
 List of function hooks.
@doc
 hook->name is the name sorted by, which is the tagname.
@see hook
*/
	List	hooks;
/**
 Pointer to the root element.
@doc
 XML allows only one tag at the root.
 This is it. All other tags must be
 nested within this.
*/
	struct xmltag *root;
/**
 Internal use.
*/
	struct xmltag *current;
} xmldoc;


extern void xml_freedoc(struct xmldoc *doc);
extern struct xmltag *xml_newtag(char *name);
extern struct xmltag *xml_newtext(char *text);
extern struct xmltag *xml_freetag(struct xmltag *tag);
extern struct xmldoc *xml_tok(struct xmldoc *doc, char *buf);
extern struct xmldoc *xml_newdoc(char *document, char *name);
extern void xml_getattribs(struct xmltag *tag, char *s);
extern struct xmldoc *xml_loaddoc(char *name);
extern size_t xml_savedoc(struct xmldoc *doc, char *name);
extern char *xml_printesc(char *txt, FILE *stream);
extern char *xml_newctx(char *ancestors, char *parent);


/* Types of nodes */
enum {
	XML_TAG = 0,		/* <tag> </tag>, <tag/> */
	XML_TEXT,			/* whatever isn't a tag */
	XML_PROCESSING,		/* <?target ?> */
	XML_COMMENT,		/* <!-- comment --> */
	XML_DECLARATION		/* <!something > (element declarations) */
};


#ifdef __cplusplus
}
#endif

#endif /* _XIQUAL_XML_H */
