#include "hash.h"
#include "token.h"
#include <stdbool.h>
#include <assert.h>

/**
 * This file is to check list.h and list.c for part I of the assignment only
 *
 * Do not include it in the submission of part II
 *
 * You are expected to add additional test cases to this file
 * and to run valgrind on the program once you have it working
 */

//Tests that the hashcode function works
bool basicHashcode(hash_struct* table){


    if (hashcode(table, 7) == 7 && hashcode(table, 107) == 7){
        return true;
    } else {
        return false;
    }

}


//tests that an empty list has size == 0
bool testBasicEmpty(hash_struct* table){

    clear(table);

    if (isEmpty(table) == true){
        return true;
    } return false;

}





int main()
{
  /*
    printf ("Creating table\n") ;

        hash_struct* my_table = initTable(10) ;


        if (basicHashcode(my_table) == true){
         printf("%s\n", "Passed Test 1 : basicHashcode()");
    } else {
        printf("%s\n", "Failed Test 1 : basicHashcode()");
    }

    if (testBasicEmpty(my_table) == true){
        printf("%s\n", "Passed Test 2 : testBasicEmpty()");
    } else {
        printf("%s\n", "Failed Test 2 : testBasicEmpty()");
    }
*/
    //-------------------YOU CAN ADD YOUR OWN TESTS HERE---------
/*
    put(my_table,10,12);
    put(my_table,20,34);
    put(my_table,30,56);
    put(my_table,42,78);
    put(my_table,54,910);
    put(my_table,65,1112);
    put(my_table,75,1314);
    put(my_table,87,1516);
    put(my_table,98,1718);
    put(my_table,109,1920);
  
    //printEntries(my_table);
    //printFullTable(my_table);
    data_entry* entry = get(my_table,75);
    int value = entry->value;
    printf("The value for key=75 is %d\n",value);
    if(contains(my_table, 87)){
      printf("The key=87 it contains \n");
    }
    printf("size = %d",size(my_table));
  
  if(isFull(my_table)){
      printf("The table is full\n");
    }
  removeEntry(my_table, 109);
  if(isFull(my_table)){
      printf("The table is full\n");
    }*/

    //-------------------------------------------------

/*
    printf ("freeing table\n") ;
    done(my_table) ;*/
  
    token* my_token = init_token();
    delete_token(my_token);


    return 0;

}
