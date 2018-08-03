#include "libconfig.h"
#include <stdio.h>
#include <stdlib.h>

void delete_user_config(config_t* config) {
    config_destroy(config);
    free(config);
}

config_t* get_user_config(void) {
    //initiate a new config
    config_t* user_config = malloc(sizeof(config_t));
    config_init(user_config);

    FILE* config_file = fopen(CONF_DIR "/WordConscious.conf", "r");

    //make sure that the configuration file exists.
    if (config_file == NULL) {
        fprintf(stderr, "Error. Unable to read configuration file from disk.\n");
        
        delete_user_config(user_config);
        return NULL;
    }

    int result = config_read(user_config, config_file);

    //make sure that the configuration was able to be read successfully.
    if (result == CONFIG_FALSE) {
        fprintf(stderr, "Error. Unable to read the file \"WordConscious.conf\" as a "
                        "configuration file.\n");
        
        delete_user_config(user_config);
        return NULL;
    }

    //return the now parsed configuration
    return user_config;
}
