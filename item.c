/**
  @file item.c
  @author David Lovato, dalovato
  This file will hold all the functionality for
  items in the shopping list program.
*/

#include "item.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int id = 1;

/**
  This function will create a new dynamically allocated item based on the
  text in the given string. The string will contain a store, price, and name
  just as expected in the add command or loading input from a file.
  If successful, it returns a pointer to the new item.  If the string
  isn't in the right format, it should return NULL.
  @param *str a pointer to the string of input
  @return Item an item for the shopping list
*/
Item *readItem(char *str) {
  Item *item = (Item *)malloc(sizeof(Item));
  int charParsed = 0;
  item->store[STORE_MAX] = '\0';
  if (sscanf(str, "%12s %lf %n", item->store, &item->price, &charParsed) == 2) {
    item->id = id;
    id++;
    if (item->store[STORE_MAX] != '\0') {
      free(item);
      return NULL;
    }
    char ch = '\0';
    int len = 0;
    int capacity = STORE_MAX;
    item->name = malloc(capacity);
    ch = *(str+charParsed);
    while (ch != '\0') {
      if (len >= capacity - 1) {
        capacity *= 2;
        item->name = (char *) realloc(item->name, capacity);
      }
      item->name[len++] = ch;
      charParsed++;
      ch = *(str + charParsed);
    }
    item->name[len++] = '\0';
    return item;
  } else {
    free(item);
    return NULL;
  }
}

/**
  This function will free the memory for a given item, including the item
  struct and the memory for the item's name.
  @param Item *it, a pointer to the item
*/
void freeItem(Item *it) {
  if (it != NULL) {
    free(it->name);
    free(it);
  }
}
