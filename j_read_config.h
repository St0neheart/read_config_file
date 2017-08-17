#ifndef _J_READ_CONFIG_H_
#define _J_READ_CONFIG_H_

#define DOMAIN_MAX_LEN      32
#define KEY_MAX_LEN         64
#define VALUE_MAX_LEN       192
#define ANNOTATION          '#'

#define DOMAIN_NUM          10

struct j_domain_s{
    char domain[DOMAIN_MAX_LEN];
};
struct j_read_config_s{
    char domain[DOMAIN_MAX_LEN];
    char key[KEY_MAX_LEN];
    char value[VALUE_MAX_LEN];
};

struct j_linked_list_node *start_read_config();
char *get_variable(struct j_linked_list_node *header, const char *domain, const char *key);
int finish_read_config(struct j_linked_list_node *header);
int load_config_file(struct j_linked_list_node *header, const char *file_path);
char *get_json_struct(struct j_linked_list_node *header);
#endif
