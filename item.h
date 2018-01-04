/**
  @file item.h
  @author David Lovato, dalovato
  This file includes functions, structs, and variables that will be used
  with other programs.
*/

/** Maximum length of a store name */
#define STORE_MAX 12

/** Representation for an item to be purchased. */
typedef struct {
  /** Unique ID for this particular item.  */
  int id;
  
  /** Store where we're supposed to buy the item.  */
  char store[ STORE_MAX + 1 ];
  
  /** Price of this item in dollars (but you might normally store this
      as an integer number of cents). */
  double price;
  
  /** Name of this item.  Pointer to a string of arbitrary length. */
  char *name;
} Item;

/**
  This function will create a new dynamically allocated item based on the
  text in the given string. The string will contain a store, price, and name
  just as expected in the add command or loading input from a file.
  If successful, it returns a pointer to the new item.  If the string
  isn't in the right format, it should return NULL.
  @param *str a pointer to the string of input
  @return Item an item for the shopping list
*/
Item *readItem(char *str);

/**
  This function will free the memory for a given item, including the item
  struct and the memory for the item's name.
  @param Item *it, a pointer to the item
*/
void freeItem(Item *it);
