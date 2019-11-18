#include "hash.h"
#include <assert.h>


/**
* @input - A number of buckets, the size to make the hash table.
* Should assign space for the hash_struct pointer, all buckets, and should
* initialize all entries as KEY = -1, VALUE = -1 to begin.
* 
*/

hash_struct* initTable(int num_of_buckets){
    int i = 0;
    data_entry* key_value_pair = NULL;
    hash_struct* table = malloc(sizeof(hash_struct));
    table->buckets = malloc(sizeof(data_entry*)*num_of_buckets);
    table->num_of_buckets = num_of_buckets;
    assert(table->buckets != NULL);
    for(i = 0; i < num_of_buckets; i ++) {
        table->buckets[i] = (data_entry*)malloc(sizeof(data_entry));
        assert(table->buckets[i] != NULL);
        table->buckets[i]->key = -1;
        table->buckets[i]->value = -1;
    }
    return table;
}


/*
* @input : Some key value.
* @returns : The key value modulated by the size of the hash table.
*/

int hashcode(hash_struct* table, int key){
        int code = 0;
        if (key > 0 ){
          code = key % table->num_of_buckets;
        } else if ( key < 0 ) {
          code = key + table->num_of_buckets;
        }
        return code;

}


/*
* @input : Some key value, table pointer.
* @returns : The data entry if some matching (key, value) pair exists, otherwise returns NULL.
*/

data_entry* get(hash_struct* table, int key){

    int current_index = hashcode(table, key);
	
    int i;
    if((table->buckets[current_index]->key == -1) && (table->buckets[current_index]->value == -1)){
        return NULL;
    }
  
    for(i=current_index; i<table->num_of_buckets; i++){
        if((table->buckets[i]->key) == key){
        return table->buckets[i];
        }
    }
    for(i=0; i<current_index; i++){
        if((table->buckets[i]->key) == key){
            return table->buckets[i];
        }
    }
    return NULL;
}


/*
* @input : Some key value, table pointer.
* @returns : True only if a valid key exists that maps to this value.
*/

bool contains(hash_struct* table, int key){
    for(int i = 0; i < table->num_of_buckets; i ++) {
        if (table->buckets[i]->key == key) {
            return true;
        }
    }
    return false;
}


/*
* @input : Some key integer.
* @input : Some value,.
* @returns : void. Places (key, value) pairing into map.
* Replaces value if another pairing with this key already exists.
* Do nothing if the table is full!
*/

void put(hash_struct* table, int key, int value){
    int i = 0;
    int j = hashcode(table,key);
    if(isFull(table)) {
        return;
    }
    while(j < table->num_of_buckets) {
        if (table->buckets[j]->key == -1) {
            table->buckets[j]->key = key;
            table->buckets[j]->value = value;
            return;
        } else {
          j ++;
        }
    }
    if(j == table->num_of_buckets) {
        while(i < hashcode(table,key)) {
            if (table->buckets[i]->key == -1) {
                table->buckets[i]->key = key;
                table->buckets[i]->value = value;
            return;
            }
        }
    }
}

/*
* @returns : The number of valid (key, value) pairings in the table.
*/
int size(hash_struct* table){
    int n = 0;
    for(int i = 0; i < table->num_of_buckets; i ++) {
        if(table->buckets[i]->key != -1) {
            n++;
        }
    }
    return n;
}

/*
* Iterates through the table and resets all entries.
*/
void clear(hash_struct* table){
    int num_of_buckets = table->num_of_buckets;
    for(int i = 0; i < num_of_buckets; i ++) {
        if(table->buckets[i]->key != -1) {
            table->buckets[i]->key = -1;
        }
        if(table->buckets[i]->value != -1) {
            table->buckets[i]->value = -1;
        }
    }
}

/*
* @returns : true, only if the table contains 0 valid (key, value) pairings.
*/
bool isEmpty(hash_struct* table){
    if(size(table) == 0){
      return true;
    } else {
      return false;
    }
}


/*
* @returns : true, only when the table is filled entirely with VALID values.
*/
bool isFull(hash_struct* table){
    if(size(table) == table->num_of_buckets){
      return true;
    } else {
      return false;
    }
}


/*
* @input : Some key value.
* @returns : void. if a pair exists for this key, reinitialize this entry.
*/
void removeEntry(hash_struct* table, int key){
    if(table->buckets[hashcode(table,key)]->key == key) {
        table->buckets[hashcode(table,key)]->key = -1;
        table->buckets[hashcode(table,key)]->value = -1;
    }
}


/*
* Debugging function.
* Iterates through the hashTable and prints all NON-NULL (key, value) pairings.
* Print statement should be of the form "(key1, value1), (key2, value2), ...."
*/
void printEntries(hash_struct* table){
    for(int i = 0; i < table->num_of_buckets; i ++) {
        if(table->buckets[i]->key != -1) {
            printf("(%d, %d)", table->buckets[i]->key, table->buckets[i]->value);
            if(i != table->num_of_buckets - 1 ) {
                printf(", ");
            }
        }
    }
    printf("\n");
}

/*
* Debugging function.
* Iterates though the hashTable and prints ALL entries in order.
* If a value is not valid, you will print "EMPTY" for the value.
* Entry print Format: "(INDEX: tableIndex, KEY: key, VALUE: value)"
* Example :  "(INDEX: 0, KEY: 0, VALUE: 3), (INDEX: 1, KEY: EMPTY, VALUE: EMPTY), (INDEX: 2, KEY: 2, VALUE: 49), ..."
*/
void printFullTable(hash_struct* table){
    for(int i = 0; i < table->num_of_buckets; i ++) {
        if(table->buckets[i]->key != -1) {
            printf("(INDEX: %d, KEY: %d, VALUE: %d)", i, table->buckets[i]->key, table->buckets[i]->value);
        } else {
            printf("(INDEX: %d, KEY: EMPTY, VALUE: EMPTY)", i);
        }
        if(i != table->num_of_buckets - 1 ) {
          printf(", ");
        }
    }
    printf("\n");
}


/**
* Should free all space consumed by the hash table.
*/
void done(hash_struct* table){
    for(int i = 0; i < table->num_of_buckets; i ++) {
      free(table->buckets[i]);
    }
    free(table->buckets);
    free(table);
}