/**
  @file list.c
  @author David Lovato, dalovato
  This file is responsible for the functionality of the shopping list,
  including reporting, adding, and removing items from the shopping list.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "item.h"
#include "list.h"

#define INIT_CAP 5

/**
  This function will dynamically allocate an instance of ShoppingList,
  and initialize its fields.  The shopping list will be empty initially
  but has a capacity to store a few items until it has to reallocate the
  items as a larger array.
  @return ShoppingList *, a pointer to the shopping list
*/
ShoppingList *makeShoppingList() {
  ShoppingList *list = malloc(sizeof(int)*2 + sizeof(Item *));
  list->length = 0;
  list->capacity = INIT_CAP;
  list->items = malloc(sizeof(Item *) * list->capacity);
  return list;
}

/**
  This function will free all the dynamically allocated memory used by a
  ShoppingList, including the ShoppingList object itself, its resizeable
  array of Item pointers, and the Item instances themselves.
  @param ShoppingList *list, a pointer to the shopping list
*/
void freeShoppingList(ShoppingList *list) {
  for (int i = 0; i < (list->length); i++) {
    freeItem(list->items[i]);
  }
  free(list->items);
  free(list);
}

/**
  This function will add the given item to the given
  shopping list, resizing its internal array if neccessary.
  @param ShoppingList *list, shopping list
  @param Item *it, item to be added
*/
void shoppingListAdd(ShoppingList *list, Item *it) {
  if (list->length >= (list->capacity - 1)) {
    list->capacity *= 2;
    list->items = realloc(list->items, sizeof(Item *) * list->capacity);
  }
  if (it != NULL) {
    list->items[list->length++] = it;
  } else {
    printf("Invalid command\n");
  }
}

/**
  This function will add the given item from input file to the given
  shopping list, resizing its internal array if neccessary.
  @param ShoppingList *list, shopping list
  @param Item *it, item to be added
  @return bool of whether or not item could be added
*/
bool shoppingListAddInputFile(ShoppingList *list, Item *it) {
  if (list->length >= (list->capacity - 1)) {
    list->capacity *= 2;
    list->items = realloc(list->items, sizeof(Item *) * list->capacity);
  }
  if (it != NULL) {
    list->items[list->length++] = it;
    return true;
  } else {
    return false;
  }
}

/**
  This function will remove the item with the given id from the
  given shopping list.
  @param ShoppingList *list, shopping list
  @param int id, id of item to be removed
  @return true if successful, false if id doesn't match an item
*/
bool shoppingListRemove(ShoppingList *list, int id) {
  for (int i = 0; i < list->length; i++) {
    if ((list->items[i] != NULL) && (id == list->items[i]->id)) {
      freeItem(list->items[i]);
      list->items[i] = NULL;
      return true;
    }
  }
  return false;
}

/**
  This function goes through the shopping list, reporting selected items and
  computing their total cost.  It uses the given function pointer to
  decide which items to report on; for example, it can report all items, just
  those from a particular store, or just the items greater than a given price.
  @param ShoppingList *list, shopping list
  @param bool test( Item *it, void *arg ), items to compare for sorting by id
  @param void *arg, void pointer used for comparison
*/
void shoppingListReport(ShoppingList *list, bool (*test)(Item *it, void *arg), void *arg)
{
  double total = 0.0;
  for (int i = 0; i < list->length; i++) {
    if (test(list->items[i], arg)) {
      total += list->items[i]->price;
      printf("%4d %-12s %7.2lf %s\n", list->items[i]->id,
                                      list->items[i]->store,
                                      list->items[i]->price,
                                      list->items[i]->name);
    }
  }
  printf("%18s%7.2lf\n", "", total);
}

/**
  This function will return true for whenever we need to just report the
  shopping list, except for null items (items that have been removed).
  @param Item *it item to be compared
  @param arg void pointer
  @return true;
*/
bool testReport(Item *it, void *arg) {
  if (it != NULL) {
    return true;
  } else {
    return false;
  }
}

/**
  This function will return true when the store is matched with an
  item.
  @param Item *it item to be compared
  @param arg void pointer, cast to store
  @return true if matches an item
*/
bool testStore(Item *it, void *arg) {
  char store[STORE_MAX] = {'\0'};
  strcpy(store, (char *) arg);
  if ((it != NULL) && (strcmp(store, it->store) == 0)) {
    return true;
  } else {
    return false;
  }
}

/**
  This function will return true when the price is greater
  than the price given to the function.
  @param Item *t item to compare
  @param arg void pointer, cast to double
  @return true if price greater
*/
bool testGreater(Item *it, void *arg) {
  double price = *((double *) arg);
  if ((it != NULL) && (it->price > price)) {
    return true;
  } else {
    return false;
  }
}

/**
  This function will return true when the price is greater
  than the price given to the function.
  @param Item *t item to compare
  @param arg void pointer, cast to double
  @return true if price greater
*/
bool testLess(Item *it, void *arg) {
  double price = *((double *) arg);
  if ((it != NULL) && (it->price < price)) {
    return true;
  } else {
    return false;
  }
}
