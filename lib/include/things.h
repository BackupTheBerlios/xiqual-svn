/* libxithings:
 * All things wonderful and mundane, big and small, furry, smooth or scaly;
 * players, non-players and inanimate objects.
 * There is much recursion going on here - heads may spin,
 * brains may shortcircuit.
 */

#if !defined(XIQUAL_THINGS_H)
#define XIQUAL_THINGS_H

#include "lists.h"
#include "xitags.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Used for any attribute, with or without values */
typedef struct attribute {
	struct attribute	*next, *prev;
	char	*data, *name;
	List	scripts;
	int	value;	/* Normal, unmodified stat */
	int	potential;	/* Max potential, if you implement caps */
	int	temp;	/* Current rating due to bonus/penalty from magic/items/conditions */
} attribute;

/* Generic struct for items (tools, wearables, containers) or critters (NPCs, players) */
typedef struct thing {
	struct thing	*next, *prev;
	void	*data;
	List	attributes;	/* Name, powers, slot that it occupies if "wear" or "wield" attribute also exists etc. */
	List	contents;	/* If it has "container" attribute, or is a creature
									(you may want to limit this with some encumbrance implementation */
	List	scripts;		/* Script attributes: name field holds function of script, data the actual script.
									Useful for weapon scripts, magic items with special functionality, potions, AI.. */
	List	slots;		/* What slots does the creature have for wearing/wielding items? */
	List	effects;	/* (attributes) Magic effects active; name is displayed name,
								data is a script to call each 'tick' */
} thing;

/* Clarification of various concepts:
 *
 * Ticks: This is, roughly speaking, the length of a turn. In most MMORPGs, ticks may
 * vary depending on whether you are in combat, outside, or sitting safely somewhere.
 * For instance, in DAoC, a combat tick is 14 seconds - regular effects like mana and health
 * regeneration piggyback on the current combat ticks. If you are standing safely away from
 * combat, the tick is reduced to 6 seconds. Sitting down, this is reduced to 3 seconds.
 * In a turn-based game, a tick could be a turn or a step of movement, depending on how
 * you want to implement effects. Damage-over-time effects would be best suited for turn-
 * based ticks, unless you want taking a step to have a detrimental effect from, say,
 * poison.
 *
 * Slots: These are the locations a critter can wear items. Head, torso, legs, arms, hands and feet
 * are usually for items like clothing and armour, with a back slot being available.
 * The number of slots you want allow for rings and other jewelry could vary. Fingers, neck, ears,
 * wrists and whatever else you may think of. Maybe a slot for a fashionable tail-ring? A cute
 * stud in your nose or tongue? Certain items may require a hole to be prepared, but any modern
 * adventurer is probably equipped for every eventuality.
 *
 * Item scripts: A script may exist for "take", "use", "throw", "drop" etc. If they do, control is 100%
 * in the hands of the actual script. For example, Nethack's "popular" lodestone could be implemented
 * with a "drop" script that simply shows a message that you're unable to drop it, then returns.
 * The item remains in the inventory then. Or, perhaps a "destroy" function? Some items may explode
 * violently..the function would then display any effects, remove itself from the inventory of the
 * critter and cause some damage. "take" may simply update critter->effects with an item's magical
 * bonuses.
 *
 * Why string-based attributes instead of tags or pre-defined attribute integers?
 * Try this: rgrep so / | wc -l
 * Bloody ridiculous how many lines per minute this amounts to. Speed is not an issue.
 *
 * Reason two: Flexibility. Imagine the creature editor having template files for various
 * races of enemies. The file defines attributes Strength, Health, Toughness, Armour
 * and some form of main weapon. If the game in question expands to also include
 * an Agility attribute, just add it to the template, and save. Now a few scripts need to
 * look for this new attribute and use it for whatever purpose (aim, dodge). No need to
 * recompile the main program - a seemingly innocent addition of a variable in a structure
 * might make it necessary to recompile the whole project if you're unlucky! In the worst
 * case, you could even have forgotten to define that particular header file as a requirement
 * in your header file..
 *
 * A third reason, related to #2, is that you need fewer structures in general.
 * The current design needs one structure for living and inanimate objects.
 */


/* Utility function to ease making objects (destructors are included)
 * This ensures that calling any of the list_* functions will always do
 * The Right Thing(TM)
 */

extern void attrib_killer(attribute *attrib);
extern void thing_killer(thing *thing);

extern attribute *create_attribA(tagitem *tags);
extern attribute *create_attrib(ulong tag1, ... );
extern void tag_attribA(attribute *attrib, tagitem *tags);
extern void tag_attrib(attribute *attrib, ulong tag1, ... );
extern attribute *add_attribA(List *list, tagitem *tags);
extern attribute *add_attrib(List *list, ulong tag1, ... );
extern attribute *alloc_attribA(List *list, tagitem *tags);
extern attribute *alloc_attrib(List *list, ulong tag1, ... );
extern attribute *dupe_attrib(attribute *attrib);

extern thing *create_thingA(tagitem *tags);
extern thing *create_thing(ulong tag1, ... );
extern void tag_thingA(thing *thing, tagitem *tags);
extern void tag_thing(thing *thing, ulong tag1, ... );
extern thing *alloc_thingA(List *list, tagitem *tags);
extern thing *alloc_thing(List *list, ulong tag1, ... );
extern thing *dupe_thing(thing *thing);

/* Tags used during initialisation
 */

enum {
	THING_BASE=(('T'<<24) | ('H'<<16) | ('N'<<8) | ('G')),
	TT_NAME,
	TT_DATA,
	TT_VALUE,
	TT_POTENTIAL,
	TT_TEMP,
	TT_ATTRIB,	/* Pointer to an attribute to add to an item/critter's list when created */
	TT_THING,	/* Pointer to an item in this critter/item's inventory */
	TT_SCRIPT,	/* (attribute *) */
	TT_EFFECT,	/* Effect active from a critter's creation */
	TT_SLOT	/* Name of a slot for wearables/tools,
	 such as neck, left hand etc. (char *) */
};

#ifdef __cplusplus
}
#endif

#endif	/* EOF */
