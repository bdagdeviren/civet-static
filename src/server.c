#define NO_SSL

#include "k8deployment.h"

#ifdef NO_SSL
#define PORT "8089"
#define HOST_INFO "http://localhost:8089"
#else
#define PORT "8843"
#define HOST_INFO "https://localhost:8843"
#endif

int exitNow = 0;

int
RunServer(){

    signal(SIGINT,sigHandler);
    signal(SIGTERM,sigHandler);

    const char *options[] = {"listening_ports",
                             PORT,
                             "request_timeout_ms",
                             "10000",
                             "error_log_file",
                             "error.log",
#ifndef NO_SSL
                             "ssl_certificate",
                             "../../resources/cert/server.pem",
                             "ssl_protocol_version",
                             "3",
                             "ssl_cipher_list",
                             "DES-CBC3-SHA:AES128-SHA:AES128-GCM-SHA256",
#endif
                             "enable_auth_domain_check",
                             "no",
                             "num_threads",
                             "2",
                             0};

    struct mg_callbacks callbacks;
    struct mg_context *ctx;
    int err = 0;

/* Check if libcivetweb has been built with all required features. */
#ifndef NO_SSL
    if (!mg_check_feature(2)) {
        fprintf(stderr,"Error: Embedded example built with SSL support, but civetweb library build without.");
        err = 1;
    }
    mg_init_library(MG_FEATURES_SSL);
#else
    mg_init_library(0);
#endif

    if (err) {
        fprintf(stderr, "Cannot start CivetWeb - inconsistent build.");
        return EXIT_FAILURE;
    }

    /* Callback will print error messages to console */
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.log_message = log_message;

    /* Start CivetWeb web server */
    ctx = mg_start(&callbacks, 0, options);

    /* Check return value: */
    if (ctx == NULL) {
        fprintf(stderr, "Cannot start CivetWeb - mg_start failed.");
        return EXIT_FAILURE;
    }

    /* Add handler EXAMPLE_URI, to explain the example */
    initialize_route(ctx);

    /* Shows server info */
    log_info("Server Listening Address: %s", HOST_INFO);
    log_info("Shutdown Server:  %s%s", HOST_INFO,"/shutdown");

    /* Wait until the server should be closed */
    while (!exitNow) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    mg_stop(ctx);

    log_info("Server shutdown.");

    return EXIT_SUCCESS;
}

