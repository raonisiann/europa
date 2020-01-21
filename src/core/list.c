#include <stdio.h>
#include "list.h"
#include "memm.h"


struct list *list_factory(){
    return (struct list *)memm_alloc(sizeof(struct list));
}

struct list_item *list_item_factory(){
    struct list_item *item = (struct list_item *)memm_alloc(sizeof(struct list_item));
    item->data = NULL;
    item->next = NULL;
    return item;
}

struct list *list_create(){
    struct list *newlist = list_factory();
    newlist->size = 0;
    newlist->first = NULL;
    newlist->last = NULL;
    return newlist;    
}

// Duplicate the list struct returning a pointer 
// to the new list struct
struct list *list_dup(struct list *l){    
    struct list *newlist = list_create();

    if(l->first != NULL){
       struct list_item *i = l->first;      
       while(i != NULL){
            struct list_item *newi = list_item_factory();            
            newi->data = i->data;            
            if(newlist->first == NULL){                
                newlist->first = newi;    
                newlist->last = newi;
            }else{             
                newlist->last->next = newi;
                newlist->last = newi;
            }
            newlist->size++;
            // go to next item of the list
            i = i->next; 
       }
    }
    newlist->size = l->size;  
    return newlist; 
}

// Add an item do the end of the list 
void list_add_item(struct list *l, void *data){
    struct list_item *item = list_item_factory(); 
    item->data = data;             
    if(l->first == NULL && l->last == NULL){        
        l->first = item; 
        l->last = item; 
    }else{
        l->last->next = item;
        l->last = item;
    }
    l->size++; 
}

// Free the list 
void list_destroy(struct list *l){   
    if(l->first != NULL){
        struct list_item *i = l->first;  
        struct list_item *freeme;
        while(i != NULL){            
            freeme = i;
            i = i->next;
            memm_free(freeme);            
        }
    }  
    l->first = NULL;
    l->last = NULL;
    l->size = 0;   
}

// Dump commmand list
void list_dump(struct list *l){
    printf(">> DUMPING COMMAND LIST >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    struct list_item *i = l->first; 
    while(i != NULL){
        printf(" : ");
        printf("\n");
        i = i->next;    
    }
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}
