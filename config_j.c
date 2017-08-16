#include <stdio.h>
#include <stdlib.h>

#include "cjson.h"
#include "config_j.h"

char domain[DOMAIN_MAX_LEN] = "default";

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

void get_domain(const char *str)
{
    int len = strlen(str);
}
int load_config_file(const char *file_path)
{
    FILE        *fp = NULL;
    char        *line = NULL;
    size_t      len = 0;

    fp = fopen(file_path, "r");
    if(fp == NULL)
        return -1;

    while((read = getline(&line, &len, fp)) != 1)
    {
        char *no_space_line = rtspace(line);
        if(no_space_line[0] == ANNOTATION || no_space_line[0] == '\0')
            goto next;
        else if(line[0] == '[')
            get_domain(no_space_line);

next:
        if(line)
            free(line);
    }
    fclose(fp);
}

char *get_variable(const char *domain, const char *key)
{
}

char *get_json_struct()
{
}
