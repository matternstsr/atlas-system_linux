#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "http_request_parser.c"
#include "todos.c"

#define GET_URIS {"/todos"}
#define HEAD_URIS {NULL}
#define POST_URIS {"/todos"}
#define PUT_URIS {NULL}
#define DELETE_URIS {"/todos"}
#define CONNECT_URIS {NULL}
#define OPTIONS_URIS {NULL}
#define TRACE_URIS {NULL}

bool is_known_uri(http_request_t *request) {
    const char *uris_by_method[][NUM_HTTP_METHODS] = {
        GET_URIS,
        HEAD_URIS,
        POST_URIS,
        PUT_URIS,
        DELETE_URIS,
        CONNECT_URIS,
        OPTIONS_URIS,
        TRACE_URIS
    };
    
    char **uris = uris_by_method[request->method];

    for (size_t i = 0; uris[i]; ++i) {
        if (strcmp(request->uri, uris[i]) == 0) {
            return true;
        }
    }
    return false;
}

size_t handle_get_request(http_request_t *request, char **body, todo_t *todos, int sum_repr_lens, int id) {
    char *param_id = get_param(request->query_params, "id");
    
    if (param_id) {
        int i = atoi(param_id);
        if (i >= id || !todos[i].repr) return 0;

        *body = strdup(todos[i].repr);
        return todos[i].repr_len;
    } 

    *body = malloc(sum_repr_lens + 3);
    (*body)[0] = '[';
    
    size_t length = 1;
    char *delimiter = "";

    for (int i = 0; i < id; ++i) {
        if (todos[i].repr) {
            length += sprintf(*body + length, "%s%s", delimiter, todos[i].repr);
            delimiter = ",";
        }
    }
    
    (*body)[length++] = ']';
    (*body)[length] = '\0';
    
    return length;
}

char *handle_delete_request(http_request_t *request, todo_t *todos, int id, int *sum_repr_lens) {
    char *param_id = get_param(request->query_params, "id");
    
    if (!param_id) return NULL;

    int i = atoi(param_id);
    if (i >= id || !todos[i].repr) return NULL;

    *sum_repr_lens -= todos[i].repr_len;
    memset(&todos[i], 0, sizeof(todo_t));
    return strdup("\r\n");
}

char *process_request(http_request_t *request) {
    char *body = NULL;
    static todo_t todos[100];
    static int current_id = 0, total_repr_length = 0;
    size_t response_length = 0;

    if (request->method == GET) {
        response_length = handle_get_request(request, &body, todos, total_repr_length, current_id);
    } else {
        char *title = get_param(request->body_params, "title");
        char *description = get_param(request->body_params, "description");

        if (!title || !description) return NULL;

        add_todo(todos, current_id, title, description);
        total_repr_length += todos[current_id].repr_len;
        body = strdup(todos[current_id].repr);
        response_length = todos[current_id].repr_len;
        current_id++;
    }

    if (request->method == DELETE) {
        return handle_delete_request(request, todos, current_id, &total_repr_length);
    }

    if (!body) return NULL;

    #define RESPONSE_FORMAT "Content-Length: %lu\r\nContent-Type: application/json\r\n\r\n%s"
    
    char *response = calloc(response_length + sizeof(RESPONSE_FORMAT) + 10, sizeof(char));
    sprintf(response, RESPONSE_FORMAT, response_length, body);
    free(body);
    return response;
}

char *make_response(char *client_address, char *buffer) {
    struct http_request_s *request;
    char *status = NULL, *response = NULL;

    (void)client_address;

    if (!(request = http_request_init(buffer))) {
        status = "400 Bad Request";
    } else if (!is_known_uri(request)) {
        status = "404 Not Found";
    } else if (request->method == POST && !get_header(request->headers, "Content-Length")) {
        status = "411 Length Required";
    } else {
        response = process_request(request);
        if (!response) {
            status = (request->method == POST) ? "422 Unprocessable Entity" : "404 Not Found";
        } else {
            status = (request->method == POST) ? "201 Created" : (request->method == DELETE) ? "204 No Content" : "200 OK";
        }
    }

    char *method = request ? request->method_str : strtok(buffer, " ");
    char *uri = request ? request->uri : strtok(NULL, "? ");
    printf("%s %s -> %s\n", method, uri, status);

    char *res = calloc(1024, sizeof(char));
    sprintf(res, "HTTP/1.1 %s\r\n%s", status, response ? response : "\r\n");
    
    free_http_request(request);
    free(response);
    return res;
}
