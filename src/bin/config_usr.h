
#ifndef CONFIG_USR_H
#define CONFIG_USR_H

//retrieves user configuration from config file.
//returned config must be deleted using delete_user_config().
config_t* get_user_config(void);

void delete_user_config(config_t* config);

#endif