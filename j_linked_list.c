#include <string.h>
#include <stdlib.h>

#include "j_linked_list.h"

struct j_linked_list_node *search_config(struct j_read_config_s *data_in, struct j_linked_list_node *header)
{
    struct j_linked_list_node *tmp = header;
    struct j_linked_list_node *ret = NULL;

    for(;tmp != NULL; tmp=tmp->next)
    {
        if(!strcmp(tmp->data.domain, data_in->domain) && !strcmp(tmp->data.key, data_in->key))
            ret = tmp;
    }
    return ret;
}

int free_list(struct j_linked_list_node *header)
{
    struct j_linked_list_node *t1 = header;
    struct j_linked_list_node *t2 = header;

    free(header->domain_list);
    while(t1 != NULL)
    {
        t2 = t1->next;
        free(t1);
        t1 = t2;
    }
    header = NULL;
    return 0;
}
