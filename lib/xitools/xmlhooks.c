/* XML parsing functions.
 *
 * TODO: A hook for <!ELEMENT tags used in stylesheets?
 * TODO: What about <!DOCTYPE elements?
 * TODO: <!ATTLIST parsing? <!NOTATION?
 * TODO: <!ENTITY?
 */

#include "xixml.h"


/**
 Add a hook to handle a specific tag.
@doc
 This is probably the most interesting function in xixml.
 You can add a hook function to handle each tag in your datafiles with this
 function. The hook may also control the further parsing of the XML file,
 if you want to.

 The hook will get a pointer to the xmltag structure as its first argument,
 and must return 0 if no special handling problems have occurred.
 At the moment, returning anything other than zero will cause
 xml_parse() to skip parsing of any children of this tag. This is to allow
 you to remove any children that aren't behaving, for instance.

 (A future extension might allow for the return code to cause parsing to
 stop altogether)
@return 0 if no error occurred allocating a node to hold your function
@author Ronny Bangsund
*/
int xml_addtaghook(xmldoc *doc, char *tagname, int *func)
{
	return 0;
}

/**
 Add a hook function to handle an escaped character entity.
@doc
 When an entity definition isn't one of the five supported types
 (&gt;, &lt;, &amp;, &apos;, &quot;), the appropriate function you install
 will be called to handle the translation. You will need to add one functions
 for each entity encoding you want to support.

 The function must return a pointer to the string to be output instead
 of the encoded entity. This should preferably be a static buffer.
 Your function will have a pointer to the first character after the
 ampersand as its first and only argument.
@param entity Name of entity without the beginning ampersand and ending colon.
@param func Your hook function.
@return 0 if no error occurred allocating a node to hold your function
@author Ronny Bangsund
@see xml_addgenentityhook()
*/
int *xml_addentityhook(xmldoc *doc, char *entity, int *func)
{
	return 0;
}

/**
 Add a generic hook function for all escaped entities.
@doc
 This is a generic variant of the xml_addentityhook() function.
 If you use this function, xixml will only use the hook you specify
 when it can't recognise an entity. In other words, any hooks created without
 xml_addentityhook() are invalidated.

 The function you pass must return a pointer to the string to output instead
 of the encoded entity, as usual. The first argument is also a pointer to
 the first character after the ampersand.
@param func Your hook function.
@return 0 if no error occurred allocating a node to hold your function
@author Ronny Bangsund
*/
int *xml_addgenentityhook(xmldoc *doc, int *func)
{
	return 0;
}

/**
 Add a hook for a processing instruction.
@doc
 Processing instructions are tags starting with <?. You are forbidden to
 use xml in any combination of case (xml, xMl, Xml etc.), but other basic
 tags such as xml-stylesheet are possible. If you try registering "xml" in
 any case combination, this function returns an error.

 The hook will get a pointer to the relevant xmltag instruction
 (without the question mark) as its first argument.
@param name The processing instruction to add a hook for.
@param func Your hook function.
@return 0 if no error occurred allocating a node to hold your function
@author Ronny Bangsund
*/
int *xml_addprocessinghook(xmldoc *doc, char *name, int *func)
{
	return 0;
}


/**
 Add a hook that catches all unspecified tags.
@doc
 If you need to write a program that catches unknown tags, this is what
 you want. The hook function needs to be similar to the one used in
 xml_addtaghook().
@param func Your hook function.
@return 0 if no error occurred allocating a node to hold your function
@author Ronny Bangsund
@see xml_addtaghook()
*/
int *xml_addgentaghook(xmldoc *doc, char *func)
{
	return 0;
}


/**
 Walk through an XML document and call user-specified hooks.
@doc
 This function will call all the hooks you specify on the relevant
 tags and elements. It returns at the end of the document.

 While walking the tree, uninteresting tags are dropped, according
 to previously supplied lists.
@return 0 if no error occurred while calling hooks.
@author Ronny Bangsund
@see xml_addtaghook(), xml_addgenentityhook(), xml_addentityhook(),
 xml_addgentaghook(), xml_loaddoc(), xml_newdoc(), xml_prune()
*/
int xml_parse(xmldoc *doc)
{
	return 0;
}


/**
 Walk through an XML document and remove unnecessary nodes.
@doc
 This function will drop all tags the caller isn't interested
 in, leaving a bare tree of yummy information goodness.
@return 0 if no error occurred. No error conditions exist as of yet.
@author Ronny Bangsund
@see xml_addtaghook(), xml_addgenentityhook(), xml_addentityhook(),
 xml_addgentaghook(), xml_loaddoc(), xml_newdoc(), xml_parse()
*/
int xml_prune(xmldoc *doc)
{
	return 0;
}


/* Specify contexts to ignore.
 * Use xml_prune() to drop, or have
 * xml_parse() remove as it parses.
 */
void xml_collect(xmldoc *doc)
{
}
