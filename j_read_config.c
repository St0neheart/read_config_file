#include <stdio.h>
#include <stdlib.h>

#include "cjson.h"
#include "j_read_config.h"
#include "j_linked_list.h"

char domain[DOMAIN_MAX_LEN] = "default";
j_linked_list *header = NULL;
j_linked_list *end = NULL;

void str_trim_crlf(char *str)
{
    char *p = &str[strlen(str)-1];
    while(*p == '\r' || *p == '\n')
        *p-- = '\0';
}
char *rtspace(char *str)
{
    char *it=NULL;
    str_trim_crlf(str);
    while( *str == ' ' && *str)
        ++str;
    it=str;
    while(*str)
        str++;
    while(*(--str)==' ')
        ;
    *(++str)='\0';
    return it;
}

int get_domain(const char *str, int rows)
{
    int len = strlen(str);
    int i;

    if(len - 2 > DOMAIN_MAX_LEN - 1)
    {
        sprintf(stderr, "[%d] the domain must less than %s bytes",rows,DOMAIN_MAX_LEN);
        return -1;
    }

    memset(domain, 0, DOMAIN_MAX_LEN);
    for(i = 1; i <= len; i++)
    {
        if(str[i] != ']')
            domain[i - 1] = str[i];
        else
            break;
    }
    if(i != len)
    {
        sprintf(stderr, "[%d] cannot recongize the domain.", rows);
        memset(domain, 0, DOMAIN_MAX_LEN);
        strcpy(domain, "default");
        return -1;
    }
    return 0;
}
int get_config_setting(const char *str, int rows)
{
    char *token;
    char *tmp = strdup(str);

    token = strsep(&tmp, "=");
    if(strlen(token) + 1 > KEY_MAX_LEN)
    {
        sprintf(stderr, "[%d] the key must be less than %s bytes", rows, KEY_MAX_LEN);
        return -1;
    }
    if(strlen(tmp) + 1 > VALUE_MAX_LEN)
    {
        sprintf(stderr, "[%d] the value must be less than %s bytes", rows, VALUE_MAX_LEN);
        return -1;
    }

    j_read_config_s *data = malloc(sizeof(j_read_config_s));;
    strcpy(data->domain, domain);
    strcpy(data->key, token);
    strcpy(data->value, tmp);

    if(header == NULL)
        header = end = create_list(data);
    else
        end = add_node(data, header);

    return 0;
}
int load_config_file(const char *file_path)
{
    FILE        *fp = NULL;
    char        *line = NULL;
    size_t      len = 0;
    int         rows = 1;
    int         ret = 0;

    fp = fopen(file_path, "r");
    if(fp == NULL)
        return -1;

    while((read = getline(&line, &len, fp)) != 1)
    {
        char *no_space_line = rtspace(line);
        if(no_space_line[0] == ANNOTATION || no_space_line[0] == '\0')
            goto next;
        else if(no_space_line[0] == '[')
            ret = get_domain(no_space_line, rows);
        else
            ret = get_config_setting(no_space_line, rows);
next:
        if(line)
            free(line);
        if(ret != 0)
            break;
        rows++;
    }
    fclose(fp);
    return ret;
}

char *get_variable(const char *domain, const char *key)
{
    j_read_config_s *data = (j_read_config_s *)malloc(sizeof(j_read_config_s));
    char *value = NULL;
    strcpy(data->domain, domain);
    strcpy(data->key, key);

    j_linked_list_node *t = NULL;

    t = search_config(data, header);
    if(t != NULL)
    {
        value = malloc(strlen(t->value) + 1);
        strcpy(value, t->value);
    }
    return value;
}

char *get_json_struct()
{
}
