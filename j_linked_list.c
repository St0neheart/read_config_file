#include <string.h>
#include <stdlib.h>

#include "j_linked_list.h"

j_linked_list_node *create_list(j_read_config_s *data)
{
    j_linked_list_node *header = (j_linked_list_node)malloc(sizeof(j_linked_list_node));

    header->data = data;
    header->next = NULL;
    return header;
}

j_linked_list_node *add_node(j_read_config_s *data, j_linked_list_node *end)
{
    j_linked_list_node *add = (j_linked_list_node)malloc(sizeof(j_linked_list_node));

    add->data = data;
    add->next = NULL;
    end->next = add;

    return add;
}

j_linked_list_node *search_config(j_read_config_s *data_in, j_linked_list_node *header)
{
    j_linked_list_node *tmp = header;
    j_linked_list_node *ret = NULL;

    for(;tmp != NULL; tmp=tmp->next)
    {
        if(!strcmp(tmp->data->domain, data_in->domain) && !strcmp(tmp->data->key, data_in->key))
            ret = tmp;
    }
    return ret;
}
