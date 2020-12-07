// Link v1.0 by Vinícius Schütz Piva

#ifndef VSCZPV_LINK_H_
#define VSCZPV_LINK_H_

// Link structure, can hold any type of data.
// It takes the data itself, a reference to the next link
// and another to the previous.
typedef struct vlink
{
	void* data;
	struct vlink* next;
	struct vlink* prev;
} vlink;

// Internal inline functions to mitigate code copypasta
vlink* __LINKEDLIST_tofirst(vlink* start);
vlink* __LINKEDLIST_tolast(vlink* start);
vlink* __LINKEDLIST_glydenext(vlink* match, int offset);
vlink* __LINKEDLIST_glydeprev(vlink* match, int offset);

// Purgelink, frees allocated memory of passed link.
void purgelink(vlink* entry);

// Creates a new link object;
// It takes some data and encapsulates it into a empty
// link structure.
vlink* createlink(void* data);

// Inserts a link into another link list.
// It takes a link structure and joins it into another
// link. Beware, this function does *NOT* remove old
// references to previous links, and it's recommended
// to only pass it clean, empty links. That means it can,
// if somewhat erronacly, join two link chains together,
// but it's not guaranteed that it will be a clean join.
// If you want to join an already existing link, make sure
// to nullout its next and prev references, or simply make
// a copy of it with createlink. If you desire to join two
// chains properly, use joinlink instead.
// The mode argument works the same as in Addlink.
void insertlink(vlink* object, vlink* entry, char mode);

// Creates a new link and inserts it into another link chain.
// It takes some data, an existing link and a join mode.
// LINK_APPEND (0) appends it as the last link
// LINK_SLOTIN (1) slots in it as the first link
// LINK_INSAFTER (2) inserts it after the entry.
// LINK_INSBEFORE (3) inserts it before the entry.
void addlink(void* data, vlink* entry, char mode);

// Joins two link chains.
// takes the joiner's last link next reference and links it
// with joinee's first link, and also does the same with
// joinee's prev reference.
void joinlink(vlink* joiner, vlink* joinee);

// Seeks for a link in a link chain.
// Takes an link entry, a seek mode and a offset, and gives
// back the link found. If went out of bounds, retuns NULL.
// LINK_FROMFIRST (0) seeks from start
// LINK_FROMLAST (1) seeks from end
// LINK_FROMENTRYAFT (2) seeks from after of entry
// LINK_FROMENTRYBFR (3) seeks from before of entry
vlink* seeklink(vlink* entry, char mode, int offset);

// Splits two links in half and gives an entry to the lost half.
// LINK_SPLITAFT (0) splits entry and entry->next
// LINK_SPLITBFR (1) splits entry and entry->prev
vlink* splitlink(vlink* entry, char mode);

// Cuts a link out of another link, amount is the lenght of link
// Returns a link* array with 3 entries, the cutout, the before of cut
// and after of cut.
// uses same macro from joinlink.
vlink** cutlink(vlink* entry, char mode, int amount);

// Counts the size of a link.
int countlink(vlink* entry);

#endif // VSCZPV_LINK_H_
