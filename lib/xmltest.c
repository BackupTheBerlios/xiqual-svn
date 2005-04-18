/*
 * Experimental parser for XML-like datafiles.
 *
 * TODO:
 * Hooks for the document tokeniser portion to recognise special
 * tags ("<!", "<?", "<whatever:" and such)
 * Parserhooks: Must store list of attributes to define context we're
 * currently in. If you go to a sub-element with a tagname that means
 * something different, this could be handy.
 * List of contexts, each with lists of tags and hooks?
 * [ installhook(tag, context, function) ]
 *

 * Quote from XML in a Nutshell:
	Namespaces are not part of XML 1.0. They were invented about a year
	after the original XML specification was released. However, care was
	taken to ensure backwards compatibility. Thus, an XML 1.0 parser that
	does not know about namespaces should not have any trouble reading a
	document that uses namespaces. Colons are legal characters in XML 1.0
	element and attribute names. The parser will simply report that some
	of the names contain colons. Possible problems arise in the rare cases
	where different qualified names resolve to the same full name or where
	the same qualified name indicates a different full name in different
	parts of a document.

	A namespace-aware parser does add a couple of checks to the normal
	well-formedness checks that a parser performs. Specifically, it
	checks to see that all prefixes are mapped to URIs. It will reject
	documents that use unmapped prefixes (except for xml and xmlns when
	used as specified in the XML 1.0 or Namespaces in XML specifications.)
	It will further reject any element or attribute names that contain more
	than one colon. Otherwise, it behaves almost exactly like a non-
	namespace-aware parser. Other software that sits on top of the raw XML
	parser, an XSLT engine for example, may treat elements differently
	depending on to which namespace they belong. However, the XML parser
	itself mostly doesn't care as long as all well-formedness and namespace
	constraints are met.

 * XML format checklist:
 * All tags are either an empty tag (<tag/>) or have a matching closing tag.
 * All attribute values are surrounded by quotes (single or double:
 *  <tag attrib="value"> is the same as <tag attrib='value'>)
 * Documents must have escaping of all special characters (&, <)
 * Comments start with <!-- and end with -->
 * The encoding should be UTF-8 or UTF-16, otherwise specify encoding
 * (This parser won't support anything other than UTF-8 for now)
 * XML is case-sensitive! (I prefer lower-case tags and attributes, like XHTML)
 * Whitespace is significant between tags, but not in a tag's attributes
 * (You can put the ending > any number of lines below the opening, but
 *  text between the opening and closing tag is read unmodified)
 */

/* Hook-system:
 1.Must be able to hook in handlers for <!, <? and similar special tags.

 2.Switchable option: Ignore special tags (anything with colons in the name
   portion, <!, <? and other schema-related things). Maybe ignore by pattern,
   or names starting with specific symbols?

 3.This little library is heading towards 2-pass land. Make a parse() call
   to go through the lists and call hooks on each element? The hook could
   return TRUE or FALSE to tell if a node should be discarded or not.

 3b.Alternative: Early and late calls. Early calls are called when the XML
   document is being read into memory, late calls can act on a full XML
   structure. Early could diminish the number of tags kept in memory.
   Late hooks could make references to later tags, if the user wants to be
   tricky. Early hooks won't have access to child nodes.

 4.Text hooks are necessary. The creator of an XML-enabled application defines
   what goes between opening and closing tags, so this parser follows protocol
   and preserves whitespace. If you use the examples from the XML Nutshell
   book, you'll see much odd spacing. The library application in the schema
   section is a perfect example. A user hook could remove trailing newlines
   and such from names and descriptions.

Something like this might work:
addhook(char *context, char *tagname, void *func, void data):
If context does not exist, add to list of contexts
Add function to context's function list
Data is passed along with pointer to current tag when func is called

When the parser function discovers a tag it has a hook for, it travels up
the list of parents looking for a parent or grandparent with that tag's
context as name. Or something.

What contexts could look like:
Context should be the concatenation of all ancestors.
If we've got:
 <library>
  <book>
   <isbn>45151511357</isbn>
  </book>
 </library>
then book's context would be "library", and isbn's
context "library.book". Each tag should have a pointer
to this string, and the document should have a linked
list of these strings.

Now that instance of the element isbn would look for
hooks defined for context "library.book" with id "isbn".

A context "supplier.catalog.book.isdn" wouldn't match
at all, for example.

Required functions for parser:
xml_collate(): Pick tags to support, with full context spec.
xml_parse(): Walk through tree, skip anything you haven't specified
with a collate function, call hooks.
xml_prune(): Walk tree without calling hooks, but remove
all uninteresting tags.
*/

#include "stdio.h"
#include "stdlib.h"

#include "xixml.h"
#include "xiqual.h"


int main(int argc, char *argv[])
{
	struct xmldoc *doc;
	struct xmltag *tag;
	size_t count=0;
	char *src, *dst;

	if(argc == 1) {
		src = "library.xml";
	} else {
		src = argv[1];
	}

	if(argc > 2) {
		dst = argv[2];
	} else {
		dst = "*";
	}

	fprintf(stderr, "Processing file '%s'\n", src);
	doc = xml_loaddoc(src);
	if(!doc) {
		fprintf(stderr, "Couldn't load the file '%s'.\nError:%s\n",
			src, xi_lasterror);
		return 1;
	}
	fprintf(stderr, "File %s is ", doc->name);
	if((doc->version) || (doc->revision)) {
		fprintf(stderr, "XML version %d.%d\n", doc->version, doc->revision);
	} else {
		fprintf(stderr, "an unknown format.\n");
	}
	if(doc->encoding) fprintf(stderr, "Encoding: %s\n", doc->encoding);

	count = xml_savedoc(doc, dst);

	tag = doc->root;
	xml_freedoc(doc);
	if(argc < 3) dst = "stdout";
	fprintf(stderr, "(%d bytes written to %s)\n", count, dst);
	return 0;
}
