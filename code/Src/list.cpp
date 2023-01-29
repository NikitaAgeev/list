#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <list.h>

int main()
{
    list list = {};

    //printf("yes??\n");

    list_ctor(&list, 10);
    
    //printf("yes?\n");

    //grafic_damp(&list);

    list_add(&list, list.list_size, list.list_size);
    list_add(&list, list.list_size, list.list_size);
    list_add(&list, list.list_size, list.list_size);
    list_add(&list, list.list_size, list.list_size);

    //grafic_damp(&list);

    list_dell(&list, 2);

    grafic_damp(&list);

    list_resize(&list, 3);

    grafic_damp(&list);

    list_dtor(&list);

    return 0;
}

static void loop_dell_elem (list* list, size_t pos);

static void loop_add_elem_after (list* list, size_t pos, size_t targ_to_add);

static void loop_add_elem_befor (list* list, size_t pos, size_t targ_to_add);

static void loop_swap (list* list, size_t pos_a, size_t pos_b);

static void loop_compres (list* list, size_t pos_point);

//standart func================================================
int list_ctor   (list* list, size_t start_size)
{
    assert(list != nullptr);
    assert(start_size != 0);

    list->mem = (list_el_t*)calloc(start_size + 1, sizeof(list_el_t));
    list->next_mem = (size_t*)calloc(start_size + 1, sizeof(size_t));
    list->prev_mem = (size_t*)calloc(start_size + 1, sizeof(size_t));
    
    if((list->mem == nullptr) || (list->next_mem == nullptr) || (list->prev_mem == nullptr))
    {
        list->mem = (list_el_t*)POISON;
        list->next_mem = (size_t*)POISON;
        list->prev_mem = (size_t*)POISON;

        list->free_elem = 0;
        list->list_size = 0;
        list->mem_len = 0;

        return MEM_ERROR;
    }

    list->free_elem = 1;
    list->list_size = 0;
    list->mem_len = start_size;

    //free loop filing
    size_t itter = 1;
    list->prev_mem[itter] = start_size;

    if(start_size == 1)
    {
        list->next_mem[itter] = 1;
        return OK;
    }

    list->next_mem[itter] = itter + 1;

    for(itter = 2; itter < start_size; itter++)
    {
        list->prev_mem[itter] = itter - 1;
        list->next_mem[itter] = itter + 1;
    }

    list->prev_mem[itter] = itter - 1;
    list->next_mem[itter] = 1;

    return OK;
}

int list_dtor   (list* list)
{
    assert(list != nullptr);
    
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);

    free(list->mem);
    free(list->next_mem);
    free(list->prev_mem);

    list->free_elem = 0;
    list->list_size = 0;
    list->mem_len = 0;

    return OK;
}

int list_add    (list* list, size_t num, list_el_t elem)
{
    assert(list != nullptr);
    
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);

    assert(num < list->mem_len);

    if(list->mem_len == list->list_size)
        return END_OF_COPASITY;
    
    size_t itter = 0;
    size_t pos = list->next_mem[0];

    for(itter = 0; itter < num; itter++)
        pos = list->next_mem[pos];
    

    size_t new_free = 0;
    if(list->next_mem[list->free_elem] == list->free_elem)
        new_free = 0;
    else
        new_free = list->next_mem[list->free_elem];

    loop_dell_elem(list, list->free_elem);

    list->mem[list->free_elem] = elem;

    loop_add_elem_befor(list, list->free_elem, pos);

    list->free_elem = new_free;
    list->list_size ++;

    return OK;
}

int list_dell    (list* list, size_t num)
{
    assert(list != nullptr);
    
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);

    assert(0 < list->list_size);
    
    size_t itter = 0;
    size_t pos = list->next_mem[0];

    for(itter = 0; itter < num; itter++)
        pos = list->next_mem[pos];

    loop_dell_elem(list, pos);

    loop_add_elem_befor(list, pos, list->free_elem);

    list->list_size --;
    
    return OK;
}

int list_take   (list* list, size_t num, list_el_t* elem)
{
    assert(list != nullptr);
    
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);

    assert(num < list->list_size);
 
    size_t itter = 0;
    size_t pos = list->next_mem[0];

    for(itter = 0; itter < num; itter++)
        pos = list->next_mem[pos];
    
    *elem = list->mem[pos];

    return OK;
}

//extra func===================================================
int list_resize (list* list, size_t new_size)
{
    assert(list != nullptr);
    
    assert(new_size >= list->list_size);

    if(new_size == list->mem_len)
        return OK;
    
    if(new_size < list->mem_len)
        list_norm(list);
    

    list->mem = (list_el_t*)realloc(list->mem, sizeof(list_el_t)*(new_size + 1));
    list->next_mem = (size_t*)realloc(list->next_mem, sizeof(size_t)*(new_size + 1));
    list->prev_mem = (size_t*)realloc(list->prev_mem, sizeof(size_t)*(new_size + 1));


    if((list->mem == nullptr) || (list->next_mem == nullptr) || (list->prev_mem == nullptr))
    {
        list->mem = (list_el_t*)POISON;
        list->next_mem = (size_t*)POISON;
        list->prev_mem = (size_t*)POISON;

        list->free_elem = 0;
        list->list_size = 0;
        list->mem_len = 0;

        return MEM_ERROR;
    }

    if(new_size < list->mem_len)
    {
        if(new_size != list->list_size)
        {
            list->next_mem[new_size] = list->free_elem;
            list->prev_mem[list->free_elem] = new_size;
        }
        else
        {
            list->free_elem = 0;
        }

        list->mem_len = new_size;

        return OK;
    }

    size_t itter = list->mem_len + 1;
    for(itter = list->mem_len + 1; itter <= new_size; itter ++)
    {
        list->next_mem[itter] = itter + 1;
        list->prev_mem[itter] = itter - 1;
    }

    if(list->list_size == list->mem_len)
    {
        list->free_elem = list->mem_len + 1;
        list->prev_mem[list->mem_len + 1] = new_size;
        list->next_mem[new_size] = list->mem_len + 1;
    
        list->mem_len = new_size;

        return OK;
    }

    list->next_mem[list->prev_mem[list->free_elem]] = list->mem_len + 1;
    list->prev_mem[list->mem_len + 1] = list->prev_mem[list->free_elem];

    list->next_mem[new_size] = list->free_elem;
    list->prev_mem[list->free_elem] = new_size;

    list->mem_len = new_size;

    return OK;

}

int list_norm   (list* list)
{
    assert(list != nullptr);
    
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);

    loop_compres(list, 0);
    if(list->mem_len == list->list_size)
    {
        return OK;
    }

    list->free_elem = list->list_size + 1;
    loop_compres(list, list->free_elem);

    return OK;
}

//debug========================================================
int list_print (list* list)
{
    size_t pos = list->next_mem[0];
    
    while(pos != 0)
    {
        printf("\'%d\'", list->mem[pos]);
        pos = list->next_mem[pos];
    }
    printf("\n");
    
    return OK;
}

int list_verify (list*  list)
{
    if(list->list_size > list->mem_len)
        return END_OF_COPASITY;

    if((list->mem == nullptr) || (list->next_mem == nullptr) || (list->prev_mem == nullptr) || \
        (list->mem == (list_el_t*)POISON) || (list->next_mem == (size_t*)POISON) || (list->prev_mem == (size_t*)POISON))
        return MEM_ERROR;

    size_t elem_counter = 0;

    size_t pos = 0;
    if((list->next_mem[list->prev_mem[pos]] != pos) || (list->prev_mem[list->next_mem[pos]] != pos))
        return BED_MEM_LOOP;
    pos = list->next_mem[pos];

    while(pos != 0)
    {
        if((list->next_mem[list->prev_mem[pos]] != pos) || (list->prev_mem[list->next_mem[pos]] != pos))
            return BED_MEM_LOOP;
        pos = list->next_mem[pos];
        elem_counter ++;
    }

    if(list->list_size != elem_counter)
        return LOST_MEM_ELEMENT;
    
    if(list->free_elem != 0)
    {
        pos = list->free_elem;

        if((list->next_mem[list->prev_mem[pos]] != pos) || (list->prev_mem[list->next_mem[pos]] != pos))
            return BED_FREE_LOOP;
        pos = list->next_mem[pos];
        elem_counter ++;

        while(pos != list->free_elem)
        {
            if((list->next_mem[list->prev_mem[pos]] != pos) || (list->prev_mem[list->next_mem[pos]] != pos))
                return BED_FREE_LOOP;
            pos = list->next_mem[pos];
            elem_counter ++;
        }

        if(list->mem_len != elem_counter)
            return LOST_FREE_ELEMENT;
    }

    return OK;
}

//grafic=======================================================
static void print_struct (list* list, size_t pos, FILE* output_file)
{
    fprintf(output_file, "s%lu[label = \"№ %lu|e: %d|n: %lu |p: %lu\";];\n", pos, pos, list->mem[pos], list->next_mem[pos], list->prev_mem[pos]);
}

static void print_arrow (list* list, size_t pos, FILE* output_file)
{
    fprintf(output_file, "s%lu -> s%lu [constraint = false;];\n", pos, list->next_mem[pos]);
}

static void print_hed (list* list, FILE* output_file)
{
    fprintf(output_file, "hed[label = \"list|m: %p|n_m: %p|p_m: %p|m_len: %lu|l_len: %lu| <free> f_elem: %lu\";];\n",\
            (void*)list->mem, (void*)list->next_mem, (void*)list->prev_mem, list->mem_len, list->list_size, list->free_elem);
}

static void color_loop (list* list, const char* color, size_t pos, FILE* output_file)
{
    size_t my_pos = pos;
    fprintf(output_file, "s%lu[color = \"%s\";];\n",  my_pos, color);
    my_pos = list->next_mem[my_pos]; 

    while(my_pos != pos)
    {
        fprintf(output_file, "s%lu[color = \"%s\";];\n",  my_pos, color);
        my_pos = list->next_mem[my_pos];
    }
}

static void print_grafic_damp (list* list, FILE* output_file)
{
    fprintf(output_file, "digraph {\n");
    fprintf(output_file, "rankdir=LR;\n");
    fprintf(output_file, "pack=true;\n");

    fprintf(output_file, "splines=ortho;\n");
  
    fprintf(output_file, "node [ shape=record ];\n");

    size_t itter = 0;

    for (itter = 0; itter <= list->mem_len; itter++)
    {
        print_struct(list, itter, output_file);
    }


    if(list_verify(list) == OK)
    {
        color_loop(list, "green", 0, output_file);
        if(list->free_elem != 0)
            color_loop(list, "red", list->free_elem, output_file);
    }
    else
        printf("bed ver: %d\n", list_verify(list));

    print_hed(list, output_file);

    fprintf(output_file, "hed");

    for (itter = 0; itter <= list->mem_len; itter++)
    {
        fprintf(output_file, "-> s%ld", itter);
    }

    fprintf(output_file, "[color= \"white\";];\n");

    fprintf(output_file, "hed -> s%lu [constraint = false;];\n", list->free_elem);
    fprintf(output_file, "hed -> s0 [constraint = false;];\n");

    for (itter = 0; itter <= list->mem_len; itter++)
    {
        print_arrow(list, itter, output_file);
    }

    fprintf(output_file, "}\n");

}

void grafic_damp (list* list)
{
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);
    
    FILE* dump_file = fopen("dump_file.dot", "w");

    print_grafic_damp(list, dump_file);

    fclose(dump_file);

    system("dot -T png dump_file.dot -o outfile.png\n");
    system("eog outfile.png\n");
    
}

//loop funk====================================================

static void loop_dell_elem (list* list, size_t pos)
{
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);

    assert(list->prev_mem[pos] != pos);

    list->next_mem[list->prev_mem[pos]] = list->next_mem[pos];
    list->prev_mem[list->next_mem[pos]] = list->prev_mem[pos];
}

static void loop_add_elem_after (list* list, size_t pos, size_t targ_to_add)
{
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);

    list->prev_mem[list->next_mem[targ_to_add]] = pos;

    list->next_mem[pos] = list->next_mem[targ_to_add];
    list->prev_mem[pos] = targ_to_add;

    list->next_mem[targ_to_add] = pos;
}

static void loop_add_elem_befor (list* list, size_t pos, size_t targ_to_add)
{
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);

    assert(pos < list->mem_len + 1);

    list->next_mem[list->prev_mem[targ_to_add]] = pos;

    list->prev_mem[pos] = list->prev_mem[targ_to_add];
    list->next_mem[pos] = targ_to_add;

    list->prev_mem[targ_to_add] = pos;
}

static void loop_swap (list* list, size_t pos_a, size_t pos_b)
{
    assert(list->mem != (list_el_t*)POISON);
    assert(list->next_mem != (size_t*)POISON);
    assert(list->prev_mem != (size_t*)POISON);

    assert(pos_a < list->mem_len + 1);
    assert(pos_b < list->mem_len + 1);

    if(pos_a == pos_b)
        return;
    
    list_el_t elem = list->mem[pos_b];
    size_t next = list->next_mem[pos_b];
    size_t prev = list->prev_mem[pos_b];

    list->mem[pos_b] = list->mem[pos_a];
    list->next_mem[pos_b] = list->next_mem[pos_a];
    list->prev_mem[pos_b] = list->prev_mem[pos_a];

    list->mem[pos_a] = elem;
    list->next_mem[pos_a] = next;
    list->prev_mem[pos_a] = prev;

    list->prev_mem[list->next_mem[pos_a]] = pos_a;
    list->prev_mem[list->next_mem[pos_b]] = pos_b;

    list->next_mem[list->prev_mem[pos_a]] = pos_a;
    list->next_mem[list->prev_mem[pos_b]] = pos_b;    
}

static void loop_compres (list* list, size_t pos_point)
{
    size_t pos = list->next_mem[pos_point];
    size_t itter = pos_point + 1;

    while (pos_point != pos)
    {
        loop_swap(list, pos, itter);

        pos = list->next_mem[itter];
        itter ++;
    }

}