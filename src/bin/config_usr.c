#include "libconfig.h"
#include <stdio.h>

config_t get_user_config(void) {
    //initiate a new config
    config_t user_config;
    config_init(&user_config);

    FILE* config_file = fopen(CONF_DIR "/WordConscious.conf", "r");

    //make sure that the configuration file exists.
    if (config_file == NULL) {
        fprintf(stderr, "Error. Unable to read configuration file from disk.\n");
        config_clear(&user_config);
        return user_config;
    }

    int result = config_read(&user_config, config_file);

    //make sure that the configuration was able to be read successfully.
    if (result == CONFIG_FALSE) {
        fprintf(stderr, "Error. Unable to read the file \"WordConscious.conf\" as a "
                        "configuration file.\n");
        config_clear(&user_config);
        return user_config;
    }

    //return the now parsed configuration
    return user_config;
}
