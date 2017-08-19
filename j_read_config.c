#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "j_read_config.h"
#include "j_linked_list.h"
#include "cjson.h"

char domain[DOMAIN_MAX_LEN] = "default";
static int domain_num = 0;
static int realloc_times = 0;

// replace line feeds with '\0'
void str_trim_crlf(char *str)
{
    char *p = &str[strlen(str)-1];
    while(*p == '\r' || *p == '\n')
        *p-- = '\0';
}
// remove redundant spaces and tabs
char *rtspace(char *str)
{
    char *it=NULL;
    str_trim_crlf(str);
    while( (*str == ' ' || *str == '\t') && *str)
        ++str;
    it=str;
    while(*str)
        str++;
    str--;
    while(*(str)==' ' || *str == '\t')
        str--;
    *(++str)='\0';
    return it;
}

int get_domain(struct j_linked_list_node *header, const char *str, int rows)
{
    int len = strlen(str);
    int i;

    if(len - 2 > DOMAIN_MAX_LEN - 1)
    {
        printf("[%d] the domain must less than %d bytes",rows,DOMAIN_MAX_LEN);
        return -1;
    }

    memset(domain, 0, DOMAIN_MAX_LEN);
    for(i = 1; i < len; i++)
    {
        if(str[i] != ']')
            domain[i - 1] = str[i];
        else
            break;
    }
    if(i != len - 1)
    {
        printf("[%d] cannot recongize the domain.", rows);
        memset(domain, 0, DOMAIN_MAX_LEN);
        strcpy(domain, "default");
        return -1;
    }
    if(domain_num < DOMAIN_NUM * (realloc_times + 1))
        strcpy(header->domain_list[domain_num].domain , domain);
    else
    {
        realloc_times++;
        header->domain_list = realloc(header->domain_list, (DOMAIN_NUM * (realloc_times + 1)) * sizeof(struct j_domain_s));
        strcpy(header->domain_list[domain_num].domain, domain);
    }
    domain_num++;
    return 0;
}
int get_config_setting(struct j_linked_list_node *header, char *str, int rows)
{
    char *token = NULL;
    char *tmp = str;

    token = strsep(&tmp, "=");
    if(token == NULL)
    {
        printf("strsep error\n");
        return -1;
    }
    if(strlen(token) + 1 > KEY_MAX_LEN)
    {
        printf("[%d] the key must be less than %d bytes", rows, KEY_MAX_LEN);
        return -1;
    }
    if(strlen(tmp) + 1 > VALUE_MAX_LEN)
    {
        printf("[%d] the value must be less than %d bytes", rows, VALUE_MAX_LEN);
        return -1;
    }

    struct j_linked_list_node *node = (struct j_linked_list_node*)malloc(sizeof(struct j_linked_list_node));
    strcpy(node->data.domain, domain);
    token = rtspace(token);
    strcpy(node->data.key, token);
    tmp = rtspace(tmp);
    strcpy(node->data.value, tmp);

    node->next = header->next;
    node->domain_list = NULL;
    header->next = node;
    return 0;
}
int load_config_file(struct j_linked_list_node *header, const char *file_path)
{
    FILE        *fp = NULL;
    char        *line = NULL;
    size_t      len = 0;
    ssize_t     read;
    int         rows = 1;
    int         ret = 0;

    fp = fopen(file_path, "r");
    if(fp == NULL)
        return -1;

    while((read = getline(&line, &len, fp)) != -1)
    {
        char *no_space_line = rtspace(line);
        if(no_space_line[0] == '#' || no_space_line[0] == '\0')
        {
            rows++;
            continue;
        }
        if(no_space_line[0] == '[')
            ret = get_domain(header, no_space_line, rows);
        else
            ret = get_config_setting(header, no_space_line, rows);
        if(ret != 0)
            break;
        rows++;
    }
    if(line)
        free(line);
    line = NULL;
    fclose(fp);
    return ret;
}

struct j_linked_list_node *start_read_config()
{
    struct j_linked_list_node *header;

    header = (struct j_linked_list_node *)malloc(sizeof(struct j_linked_list_node));
    header->next = NULL;
    header->domain_list = (struct j_domain_s *)malloc(sizeof(struct j_domain_s) * DOMAIN_NUM);;
    return header;
}

int finish_read_config(struct j_linked_list_node *header)
{
    int ret = 0;

    ret = free_list(header);
    return ret;
}

char *get_variable(struct j_linked_list_node *header, const char *domain_in, const char *key)
{
    struct j_read_config_s *data = (struct j_read_config_s *)malloc(sizeof(struct j_read_config_s));
    char *value = NULL;
    if(domain_in == NULL)
        strcpy(data->domain, "default");
    else
        strcpy(data->domain, domain_in);
    strcpy(data->key, key);

    struct j_linked_list_node *t = NULL;

    t = search_config(data, header);
    if(t != NULL)
        value = t->data.value;

    free(data);
    return value;
}

char *get_json_struct(struct j_linked_list_node *header)
{
    int i;
    cJSON **domain_js = (cJSON**) malloc(sizeof(cJSON *) * domain_num);
    cJSON *out_js = NULL;

    out_js = cJSON_CreateObject();
    struct j_linked_list_node *t1 = header->next;

    for(i = 0; i < domain_num; i++)
        domain_js[i] = cJSON_CreateObject();

    while(t1 != NULL)
    {
        for(i = 0; i < domain_num; i++)
        {
            if(!strcmp(t1->data.domain, header->domain_list[i].domain))
            {
                cJSON_AddStringToObject(domain_js[i], t1->data.key, t1->data.value);
                break;
            }
        }
        t1 = t1->next;
    }
    for(i = 0; i < domain_num; i++)
    {
        if(domain_js[i]->child != NULL)
            cJSON_AddItemToObject(out_js, header->domain_list[i].domain, domain_js[i]);
    }
    char *out = cJSON_Print(out_js);
    cJSON_Delete(out_js);
    return out;
}
