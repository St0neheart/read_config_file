#ifndef _J_READ_CONFIG_H_
#define _J_READ_CONFIG_H_

#define DOMAIN_MAX_LEN      32
#define KEY_MAX_LEN         64
#define VALUE_MAX_LEN       192

struct j_read_config_s{
    char domain[DOMAIN_MAX_LEN];
    char key[KEY_MAX_LEN];
    char value[VALUE_MAX_LEN];
};

#endif
