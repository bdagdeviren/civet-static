//
// Created by Burak on 14.07.2021.
//

#include "k8deployment.h"

int
SendJSON(struct mg_connection *conn, cJSON *json_obj)
{
    char *json_str = cJSON_PrintUnformatted(json_obj);
    size_t json_str_len = strlen(json_str);
    /* Send HTTP message header */
    mg_send_http_ok(conn, "application/json; charset=utf-8", json_str_len);
    /* Send HTTP message content */
    mg_write(conn, json_str, json_str_len);
    /* Free string allocated by cJSON_Print* */
    cJSON_free(json_str);

    return (int)json_str_len;
}

int
log_message(const struct mg_connection *conn, const char *message)
{
    (void)conn;
    log_info(message);
    return 1;
}

void
sigHandler (){
    log_info("Exiting programs!!!");
    exit(0);
}

int
checkIfFileExists(const char* filename){
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0 && S_ISREG(buffer.st_mode))
        return 1;
    else
        return 0;
}

