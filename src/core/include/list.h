#ifndef I_LIST_H
#define I_LIST_H

// Linked List 
struct list {
    int size;
    struct list_item *first;        
    struct list_item *last; 
};

// Linked list item  
struct list_item {
    struct list_item *next;
    void *data;     
};

struct list *list_factory();
struct list_item *list_item_factory();
struct list *list_create();
void list_dump(struct list *list);
void list_add_item(struct list *list, void *data);
void list_remove_item(struct list *list, void *data);
void list_destroy(struct list *list);
struct list *list_dup(struct list *list);

#endif