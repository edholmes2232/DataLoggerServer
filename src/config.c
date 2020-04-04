#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h> //link instead?


int DEBUG, NUM_SLAVES = 1;

void ConfigRead(void) {
    
    config_t cfg;
    //config_setting_t *setting;
    config_init(&cfg);

/* Read the file. If there is an error, report it and exit. */
    if(! config_read_file(&cfg, "Settings.cfg")) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        //return(EXIT_FAILURE);
    }

    if (config_lookup_int(&cfg, "NUM_SLAVES", &NUM_SLAVES))
        printf("NODES: %d\n",NUM_SLAVES);
    else 
        printf("NUM_SLAVES NOT FOUND");
 
    if (config_lookup_int(&cfg, "DEBUG", &DEBUG))
        printf("DEBUG FLAG: %d\n", DEBUG);
    else
        printf("DEBUG FLAG NOT SET");
    
}