#ifndef _J_LINKED_LIST_H
#define _J_LINKED_LIST_H

#include "j_read_config.h"

struct j_linked_list_node
{
    j_read_config_s     data;
    j_linked_list_node  *nextp;
}


#endif
