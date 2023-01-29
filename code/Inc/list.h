#ifndef LIST_H
#define LIST_H

typedef int list_el_t;

typedef struct 
{
    list_el_t* mem;
    size_t mem_len;
    size_t list_size;
    size_t free_elem;
    size_t* next_mem;
    size_t* prev_mem;
} list;


enum LIST_MEM_ADR_STATUS
{
    FREE = 0,
    POISON = 13,
};

enum LIST_ERRORS
{
    OK                  = 0,
    MEM_ERROR           = 1 << 0,
    END_OF_COPASITY     = 1 << 1,
    BED_MEM_LOOP        = 1 << 2,
    BED_FREE_LOOP       = 1 << 3,
    LOST_MEM_ELEMENT    = 1 << 4,
    LOST_FREE_ELEMENT   = 1 << 5,
};

//standart func================================================
int list_ctor   (list* list, size_t start_size);

int list_dtor   (list* list);

int list_add    (list* list, size_t num, list_el_t elem);

int list_dell    (list* list, size_t num);

int list_take   (list* list, size_t num, list_el_t* elem);

//extra func===================================================
int list_resize (list* list, size_t new_size);

int list_norm   (list* list);

//lite debug func==============================================

int list_print (list* list);

void grafic_damp (list* list);

#endif