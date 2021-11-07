//
// Created by Burak on 6.11.2021.
//

#ifndef K8DEPLOYMENT_UI_K8DEPLOYMENT_H
#define K8DEPLOYMENT_UI_K8DEPLOYMENT_H


#include <cjson/cJSON.h>
#include <civetweb.h>
#include <log.h>


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>

extern int exitNow;

void initialize_route(struct mg_context *ctx);

int ExampleHandler(struct mg_connection *conn, void *cbdata);
int ExitHandler(struct mg_connection *conn, void *cbdata);
int FileHandler(struct mg_connection *conn, void *cbdata);

int SendJSON(struct mg_connection *conn, cJSON *json_obj);
int log_message(const struct mg_connection *conn, const char *message);
void sigHandler ();
int checkIfFileExists(const char* filename);

int RunServer();

#endif //K8DEPLOYMENT_UI_K8DEPLOYMENT_H
