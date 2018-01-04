/**
  This file will handle all the user input and file input/output, using
  list.c and item.c to create a working shopping list.
  @file shopping.c
  @author David Lovato, dalovato
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "list.h"

#define NUM_VALID_COMMANDS 10
#define CMD_LENGTH 30
#define INIT_CHAR_LENGTH 5

/**
  This function helps get the rest of the report, load, and save commmand,
  if there is anything else after the initial command.
  @param char *str, string of input to parse
  @return char *, string of input left to parse and execute
*/
static char *restOfInput(char *str) {
  // Allocate a string with a small, initial capacity.
  int capacity = INIT_CHAR_LENGTH;
  char *buffer = malloc( capacity );

  // Number of characters we're currently using.
  int len = 0;
  
  char ch = *(str);
  if (ch == '\0') {
    free(buffer);
    return NULL;
  } else {
    while (1) {
      if (len >= capacity - 1) {
        capacity *= 2;
        buffer = (char *) realloc(buffer,  capacity);
      }
      if (ch == ' ') {
        buffer[len++] = ch;
        ch = *(str + len);
      } else if (ch == '\0') {
        buffer[len++] = ch;
        return buffer;
      } else {
        buffer[len++] = ch;
        ch = *(str + len);
      }
    }
  }
}

/**
  This function will get the line of input from the user or file.
  @param FILE *stream file stream
  @return char *, char pointer to the start of the line
*/
static char *getLine(FILE *stream) {
  // Allocate a string with a small, initial capacity.
  int capacity = INIT_CHAR_LENGTH;
  char *buffer = malloc( capacity );

  // Number of characters we're currently using.
  int len = 0;
  
  // Read from this file until we reach end-of-file, storing all characters
  // in buffer.
  char ch;
  while (fscanf(stream, "%c", &ch) != EOF) {
    if (len >= capacity - 1) {
      capacity *= 2;
      buffer = (char *) realloc(buffer,  capacity);
    }
    if (ch == '\n') {
      buffer[len++] = '\0';
      return buffer;
    }
    buffer[len++] = ch;
  }
  free(buffer);
  return NULL;
}

/**
  This function will remove an item given an id.
  @param cmd, command passed in by user (whole line of input)
  @param chars_Parsed, this is an int that tells us how much input has been parsed
  @param list, current shopping list
*/
static void removeItem(char *cmd, int chars_Parsed, ShoppingList *list) {
  char *remainingInput = restOfInput(cmd + chars_Parsed);
  int id;
  int i = sscanf(remainingInput, "%d", &id);
  bool b = shoppingListRemove(list, id);
  if (i != 1 || !b) {
    printf("Invalid command\n");
  }
  free(remainingInput);
}

/**
  This function will load a given file, parsed from standard input.
  @param cmd, command passed in by user (whole line of input)
  @param chars_Parsed, this is an int that tells us how much input has been parsed
  @param list, current shopping list
*/
static void load(char *cmd, int chars_Parsed, ShoppingList *list) {
  char *remainingInput = restOfInput(cmd + chars_Parsed);
  FILE *fp = fopen(remainingInput, "r");
  if (!fp) {
    free(remainingInput);
    printf("Can't open file");
  } else {
    char *item = getLine(fp);
    int line = 1;
    while (item != NULL) {
      Item *it = readItem(item);
      free(item);
      bool b = shoppingListAddInputFile(list, it);
      if (!b) {
        printf("Invalid item, line %d\n", line);
      }
      item = getLine(fp);
      line++;
    }
    free(item);
    free(remainingInput);
    fclose(fp);
  }
}

/**
  This function will save the file to a given file name. If no file
  is given, or the file can't be opened, then it will print an error message.
  @param cmd, command passed in by user (whole line of input)
  @param chars_Parsed, this is an int that tells us how much input has been parsed
  @param list, current shopping list
*/
static void save(char *cmd, int chars_Parsed, ShoppingList *list) {
  char *remainingInput = restOfInput(cmd + chars_Parsed);
  FILE *fp = fopen(remainingInput, "w");
  if (!fp) {
    printf("Can't open file");
  } else {
    for (int i = 0; i < list->length; i++) {
      if (list->items[i] != NULL) {
        fprintf(fp, "%s %.2lf %s\n", list->items[i]->store,
                                     list->items[i]->price,
                                     list->items[i]->name);
      }
    }
    free(remainingInput);
    fclose(fp);
  }
}

/**
  This function will print the report command, whether it is a full
  report, just a report by store, or a report based on price.
  @param cmd, command passed in by user (whole line of input)
  @param chars_Parsed, this is an int that tells us how much input has been parsed
  @param list, current shopping list
*/
static void printReport(char *cmd, int chars_Parsed, ShoppingList *list) {
  char *restOfString = restOfInput(cmd + chars_Parsed);
  if (restOfString == NULL) {
    shoppingListReport(list, testReport, NULL);
  } else {
    char reportCommand[STORE_MAX] = {'\0'};
    char storeMatch[STORE_MAX] = {'\0'};
    sscanf(restOfString, "%s%n", reportCommand, &chars_Parsed);
    if (strcmp(reportCommand, "store") == 0) {
      if (sscanf((restOfString + chars_Parsed), "%s", storeMatch) == 1) {
        shoppingListReport(list, testStore, storeMatch);
      } else {
        printf("Invalid command\n");
      }
    } else if (strcmp(reportCommand, "greater") == 0) {
      double *pricePointer = NULL;
      double price = 0.0;
      if (sscanf((restOfString + chars_Parsed), "%lf", &price) == 1) {
        pricePointer = &price;
        shoppingListReport(list, testGreater, pricePointer);
      } else {
        printf("Invalid command\n");
      }
    }  else if (strcmp(reportCommand, "less") == 0) {
      double *pricePointer = NULL;
      double price = 0.0;
      if (sscanf((restOfString + chars_Parsed), "%lf", &price) == 1) {
        pricePointer = &price;
        shoppingListReport(list, testLess, pricePointer);
      } else {
        printf("Invalid command\n");
      }
    } else {
      printf("Invalid command\n");
    }
  }
  free(restOfString);
}

/**
  This function will print all the valid commands for the program, when
  the "help" command is entered.
*/
static void printHelp() {
  char commands[][CMD_LENGTH] = {"load <file>", "save <file>",
                                  "add <store> <price> <name>",
                                  "remove <id>", "report",
                                  "report store <store>",
                                  "report less <price>",
                                  "report greater <price>",
                                  "help", "quit"};
  for (int i = 0; i < NUM_VALID_COMMANDS; i++) {
    printf("%s\n", commands[i]);
  }
}

/**
  This is the main function for the program, which runs the program.
  @return int of whether or not the program was successful
*/
int main()
{
  int cmd_counter = 1;
  printf("%d> ", cmd_counter);
  char *cmd = getLine(stdin);
  ShoppingList *list = makeShoppingList();
  while (cmd != NULL) {
    printf("\n");
    char command[INIT_CHAR_LENGTH * 2];
    int chars_Parsed = 0;
    if (sscanf(cmd, "%s %n", command, &chars_Parsed) == 1) {
      if (strcmp(command, "help") == 0) {
        printHelp();
      } else if (strcmp(command, "quit") == 0) {
        break;
      } else if (strcmp(command, "add") == 0) {
        Item *it = readItem(cmd + chars_Parsed);
        shoppingListAdd(list, it);
      } else if (strcmp(command, "report") == 0) {
        printReport(cmd, chars_Parsed, list);
      } else if (strcmp(command, "load") == 0) {
        load(cmd, chars_Parsed, list);
      } else if (strcmp(command, "save") == 0) {
        save(cmd, chars_Parsed, list);
      } else if (strcmp(command, "remove") == 0) {
        removeItem(cmd, chars_Parsed, list);
      } else {
        printf("Invalid command\n");
      }
    }
    cmd_counter++;
    printf("%d> ", cmd_counter);
    free(cmd);
    cmd = getLine(stdin);
  }
  free(cmd);
  freeShoppingList(list);
  return 0;
}
