#ifndef _J_LINKED_LIST_H
#define _J_LINKED_LIST_H

#include "j_read_config.h"

struct j_linked_list_node
{
    struct j_read_config_s      data;
    struct j_linked_list_node   *next;
    struct j_domain_s           *domain_list;
};

struct j_linked_list_node *search_config(struct j_read_config_s *data_in, struct j_linked_list_node *header);

int free_list(struct j_linked_list_node *header);

#endif
