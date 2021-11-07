//
// Created by Burak on 14.07.2021.
//

#include "k8deployment.h"

void
initialize_route(struct mg_context *ctx){
    mg_set_request_handler(ctx, "/js/**.js", FileHandler, 0);
    mg_set_request_handler(ctx, "/images/**.png", FileHandler, 0);
    mg_set_request_handler(ctx, "/css/**.css", FileHandler, 0);

    mg_set_request_handler(ctx, "/", FileHandler, 0);

    mg_set_request_handler(ctx, "/version", ExampleHandler, 0);
    mg_set_request_handler(ctx, "/shutdown", ExitHandler, 0);
}

int
ExampleHandler(struct mg_connection *conn, void *cbdata)
{
    cJSON *obj = cJSON_CreateObject();
    (void)cbdata; /* currently unused */

    if (!obj) {
        mg_send_http_error(conn, 500, "Server error");
        return 500;
    }

    cJSON_AddStringToObject(obj, "version", CIVETWEB_VERSION);
    cJSON_AddNumberToObject(obj, "status", 200);
    SendJSON(conn, obj);
    cJSON_Delete(obj);

    return 200;
}

int
ExitHandler(struct mg_connection *conn, void *cbdata)
{
    (void)cbdata;
    mg_printf(conn,"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n");
    mg_printf(conn, "Server will shut down.");
    exitNow = 1;
    return 1;
}

int
FileHandler(struct mg_connection *conn, void *cbdata)
{
    const struct mg_request_info *req_info = mg_get_request_info(conn);
    int returnCode = 200;
    const char* requestedFile = req_info -> local_uri;

    if (strcmp(requestedFile,"/") == 0) {
        requestedFile = "/index.html";
    }

    char* buffer;
    if ( (buffer = _getcwd( NULL, 0 )) == NULL )
        log_error("Getting current directory error" );

    char *s = malloc(strlen(buffer)+strlen(requestedFile) + 1 );

    strcpy(s,buffer);
    strcat(s,requestedFile);

    if(checkIfFileExists(s)){
        mg_send_file(conn, s);
    }else{
        mg_send_http_error(conn,404,"Content-Type: text/plain");
    }

    free(buffer);
    free(s);

    return returnCode;
}