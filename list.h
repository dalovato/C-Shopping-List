#include <stdbool.h>

/** Representation for a pping list, an arbitrary-length list of Items. */
typedef struct {
  /** Resizable array of pointers to Items. */
  Item **items;
  
  /** Current number of items on the list. */
  int length;
  
  /** Current capacity of the list, how many pointers we have room for. */
  int capacity;
} ShoppingList;

/**
  This function will dynamically allocate an instance of ShoppingList,
  and initialize its fields.  The shopping list will be empty initially
  but has a capacity to store a few items until it has to reallocate the
  items as a larger array.
  @return ShoppingList *, a pointer to the shopping list
*/
ShoppingList *makeShoppingList();

/**
  This function will free all the dynamically allocated memory used by a
  ShoppingList, including the ShoppingList object itself, its resizeable
  array of Item pointers, and the Item instances themselves.
  @param ShoppingList *list, a pointer to the shopping list
*/
void freeShoppingList(ShoppingList *list);

/**
  This function will add the given item to the given
  shopping list, resizing its internal array if neccessary.
  @param ShoppingList *list, shopping list
  @param Item *it, item to be added
*/
void shoppingListAdd(ShoppingList *list, Item *it);

/**
  This function will add the given item from input file to the given
  shopping list, resizing its internal array if neccessary.
  @param ShoppingList *list, shopping list
  @param Item *it, item to be added
  @return bool of whether or not item could be added
*/
bool shoppingListAddInputFile(ShoppingList *list, Item *it);

/**
  This function will remove the item with the given id from the
  given shopping list.
  @param ShoppingList *list, shopping list
  @param int id, id of item to be removed
  @return true if successful, false if id doesn't match an item
*/
bool shoppingListRemove(ShoppingList *list, int id);

/**
  This function goes through the shopping list, reporting selected items and
  computing their total cost.  It uses the given function pointer to
  decide which items to report on; for example, it can report all items, just
  those from a particular store, or just the items greater than a given price.
  @param ShoppingList *list, shopping list
  @param bool test( Item *it, void *arg ), items to compare for sorting by id
  @param void *arg, void pointer used for comparison
*/
void shoppingListReport(ShoppingList *list, bool (*test)(Item *it, void *arg), void *arg);

/**
  This function will return true for whenever we need to just report the
  shopping list.
  @param Item *it item to be compared
  @param arg void pointer
  @return true;
*/
bool testReport(Item *it, void *arg);

/**
  This function will return true when the store is matched with an
  item.
  @param Item *it item to be compared
  @param arg void pointer, cast to store
  @return true if matches an item
*/
bool testStore(Item *it, void *arg);

/**
  This function will return true when the price is greater
  than the price given to the function.
  @param Item *t item to compare
  @param arg void pointer, cast to double
  @return true if price greater
*/
bool testGreater(Item *it, void *arg);

/**
  This function will return true when the price is greater
  than the price given to the function.
  @param Item *t item to compare
  @param arg void pointer, cast to double
  @return true if price greater
*/
bool testLess(Item *it, void *arg);
