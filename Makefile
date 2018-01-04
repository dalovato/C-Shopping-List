# This is the makefile for Project 4.
# @author David Lovato, dalovato
CC = gcc
CFLAGS = -g -Wall -std=c99
shopping: item.o list.o
list.o: item.o
clean:
				rm -f shopping shopping.o
				rm -f list.o list
				rm -f item item.o
				rm -f output.txt
				rm -f outlist.txt
