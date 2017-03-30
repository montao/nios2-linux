#ifndef _HTTP_H
#define _HTTP_H

#include <stdio.h>
#include <stdlib.h>

/* This is an abstraction over a socket that provides rudimentary buffering. It
   is useful for the line-oriented parts of HTTP. */
struct socket_buffer {
    int sd;
    char buffer[BUFSIZ];
    char *p;
    char *end;
};

void socket_buffer_init(struct socket_buffer *buf, int sd);

int socket_buffer_read(struct socket_buffer *buf, char *out, size_t size);

char *socket_buffer_readline(struct socket_buffer *buf, size_t *n, size_t maxlen);

char *socket_buffer_remainder(struct socket_buffer *buf, size_t *len);

/* A broken-down URI as defined in RFC 3986, except that the quiery and fragment
   parts are included in the path. */
struct uri {
    char *scheme;
    char *host;
    int port;
    char *path;
};

void uri_init(struct uri *uri);

void uri_free(struct uri *uri);

struct uri *uri_parse(struct uri *uri, const char *uri_s);

struct uri *uri_parse_authority(struct uri *uri, const char *authority);

enum http_version {
    HTTP_09,
    HTTP_10,
    HTTP_11,
    HTTP_UNKNOWN,
};

struct http_header {
    char *name;
    char *value;
    struct http_header *next;
};

struct http_request {
    char *method;
    struct uri uri;
    enum http_version version;
    struct http_header *header;
    unsigned long content_length;
    unsigned long bytes_transferred;
};

struct http_response {
    enum http_version version;
    int code;
    char *phrase;
    struct http_header *header;
    unsigned long content_length;
    unsigned long bytes_transferred;
};

void http_header_free(struct http_header *header);
char *http_header_get(const struct http_header *header, const char *name);
char *http_header_get_first(const struct http_header *header, const char *name);
struct http_header *http_header_set(struct http_header *header, const char *name, const char *value);
struct http_header *http_header_remove(struct http_header *header, const char *name);
int http_header_remove_hop_by_hop(struct http_header **header);
char *http_header_to_string(const struct http_header *header, size_t *n);

void http_request_init(struct http_request *request);
void http_request_free(struct http_request *request);
char *http_request_to_string(const struct http_request *request, size_t *n);

void http_response_init(struct http_response *response);
void http_response_free(struct http_response *response);
char *http_response_to_string(const struct http_response *response, size_t *n);

int http_read_header(struct socket_buffer *buf, char **result);
int http_parse_header(struct http_header **result, const char *header);
int http_request_parse_header(struct http_request *request, const char *header);
int http_response_parse_header(struct http_response *response, const char *header);

int http_read_request_line(struct socket_buffer *buf, char **line);
int http_parse_request_line(const char *line, struct http_request *request);

int http_read_status_line(struct socket_buffer *buf, char **line);
int http_parse_status_line(const char *line, struct http_response *response);

int http_check_auth_basic(const char *userpass, const char *value);

#endif
