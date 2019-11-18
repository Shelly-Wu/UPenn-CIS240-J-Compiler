#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct {

  int key;
  int value;

} data_entry;

typedef struct {

  int num_of_buckets ;
  data_entry** buckets ;

} hash_struct ;


hash_struct* initTable();

int hashcode(hash_struct* table, int key);


data_entry* get(hash_struct* table, int key);

bool contains(hash_struct* table, int key);

void put(hash_struct* table, int key, int value);

int size(hash_struct* table);

void clear(hash_struct* table);

bool isEmpty(hash_struct* table );

bool isFull(hash_struct* table );

void removeEntry(hash_struct* table, int key);

void printFullTable(hash_struct* table);

void printEntries(hash_struct* table );


void done(hash_struct* table);


#endif
