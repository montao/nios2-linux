#include <assert.h>
#include <string.h>

#include "base64.h"
#include "ncat.h"
#include "http.h"

/* Limit the size of in-memory data structures to avoid certain denial of
   service attacks (those trying to consume all available memory). */
static const int MAX_REQUEST_LINE_LENGTH = 1024;
static const int MAX_STATUS_LINE_LENGTH = 1024;
static const int MAX_HEADER_LENGTH = 1024 * 10;

void socket_buffer_init(struct socket_buffer *buf, int sd)
{
    buf->sd = sd;
    buf->p = buf->buffer;
    buf->end = buf->p;
}

/* Read from a stateful socket buffer. If there is any data in the buffer it is
   returned, otherwise data is read with recv. Return value is as for recv. */
int socket_buffer_read(struct socket_buffer *buf, char *out, size_t size)
{
    int i;

    /* Refill the buffer if necessary. */
    if (buf->p >= buf->end) {
        buf->p = buf->buffer;
        do {
            errno = 0;
            i = recv(buf->sd, buf->buffer, sizeof(buf->buffer), 0);
        } while (i == -1 && errno == EINTR);
        if (i <= 0)
            return i;
        buf->end = buf->buffer + i;
    }
    i = buf->end - buf->p;
    if (i > size)
        i = size;
    memcpy(out, buf->p, i);
    buf->p += i;

    return i;
}

/* Read a line thorough a stateful socket buffer. The line, including its '\n',
   is returned in a dynamically allocated buffer. The length of the line is
   returned in n. If the length of the line exceeds maxlen, then NULL is
   returned and *n is greater than or equal to maxlen. On error, NULL is
   returned and *n is less than maxlen. The returned buffer is always
   null-terminated if the return value is not NULL. */
char *socket_buffer_readline(struct socket_buffer *buf, size_t *n, size_t maxlen)
{
    char *line;
    char *newline;
    size_t count;

    line = NULL;
    *n = 0;

    do {
        /* Refill the buffer if necessary. */
        if (buf->p >= buf->end) {
            int i;

            buf->p = buf->buffer;
            do {
                errno = 0;
                i = recv(buf->sd, buf->buffer, sizeof(buf->buffer), 0);
            } while (i == -1 && errno == EINTR);
            if (i <= 0) {
                free(line);
                return NULL;
            }
            buf->end = buf->buffer + i;
        }

        newline = (char *) memchr(buf->p, '\n', buf->end - buf->p);
        if (newline == NULL)
            count = buf->end - buf->p;
        else
            count = newline + 1 - buf->p;

        if (*n + count >= maxlen) {
            /* Line exceeds our maximum length. */
            free(line);
            *n += count;
            return NULL;
        }

        line = (char *) safe_realloc(line, *n + count + 1);
        memcpy(line + *n, buf->p, count);
        *n += count;
        buf->p += count;
    } while (newline == NULL);

    line[*n] = '\0';

    return line;
}

/* Get whatever is left in the buffer. */
char *socket_buffer_remainder(struct socket_buffer *buf, size_t *len)
{
    if (len != NULL)
        *len = buf->end - buf->p;

    return buf->p;
}

/* The URI functions have a test program in test/test-uri.c. Run the test after
   making any changes and add tests for any new functions. */

void uri_init(struct uri *uri)
{
    uri->scheme = NULL;
    uri->host = NULL;
    uri->port = -1;
    uri->path = NULL;
}

void uri_free(struct uri *uri)
{
    free(uri->scheme);
    free(uri->host);
    free(uri->path);
}

static int hex_digit_value(int digit)
{
    const char *DIGITS = "0123456789abcdef";
    const char *p;

    if (digit == '\0')
        return -1;
    p = strchr(DIGITS, tolower(digit));
    if (p == NULL)
        return -1;

    return p - DIGITS;
}

static int lowercase(char *s)
{
    char *p;

    for (p = s; *p != '\0'; p++)
        *p = tolower(*p);

    return p - s;
}

/* In-place percent decoding. */
static int percent_decode(char *s)
{
    char *p, *q;

    /* Skip to the first '%'. If there are no percent escapes, this lets us
       return without doing any copying. */
    q = s;
    while (*q != '\0' && *q != '%')
        q++;

    p = q;
    while (*q != '\0') {
        if (*q == '%') {
            int c, d;

            q++;
            c = hex_digit_value(*q);
            if (c == -1)
                return -1;
            q++;
            d = hex_digit_value(*q);
            if (d == -1)
                return -1;

            *p++ = c * 16 + d;
            q++;
        } else {
            *p++ = *q++;
        }
    }
    *p = '\0';

    return p - s;
}

/* Use these functions because isalpha and isdigit can change their meaning
   based on the locale. */
static int is_alpha_char(int c)
{
    return c != '\0' && strchr("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", c) != NULL;
}

static int is_digit_char(int c)
{
    return c != '\0' && strchr("0123456789", c) != NULL;
}

/* Get the default port for the given URI scheme, or -1 if unrecognized. */
static int scheme_default_port(const char *scheme)
{
    if (strcmp(scheme, "http") == 0)
        return 80;

    return -1;
}

/* Parse a URI string into a struct URI. Any parts of the URI that are absent
   will become NULL entries in the structure, except for the port which will be
   -1. Returns NULL on error. See RFC 3986, section 3 for syntax. */
struct uri *uri_parse(struct uri *uri, const char *uri_s)
{
    const char *p, *q;

    uri_init(uri);

    /* Scheme, section 3.1. */
    p = uri_s;
    if (!is_alpha_char(*p))
        goto fail;
    for (q = p; is_alpha_char(*q) || is_digit_char(*q) || *q == '+' || *q == '-' || *q == '.'; q++)
        ;
    if (*q != ':')
        goto fail;
    uri->scheme = mkstr(p, q);
    /* "An implementation should accept uppercase letters as equivalent to
       lowercase in scheme names (e.g., allow "HTTP" as well as "http") for the
       sake of robustness..." */
    lowercase(uri->scheme);

    /* Authority, section 3.2. */
    p = q + 1;
    if (*p == '/' && *(p + 1) == '/') {
        char *authority = NULL;

        p += 2;
        for (q = p; !(*q == '/' || *q == '?' || *q == '#' || *q == '\0'); q++)
            ;
        authority = mkstr(p, q);
        if (uri_parse_authority(uri, authority) == NULL) {
            free(authority);
            goto fail;
        }
        free(authority);

        p = q;
    }
    if (uri->port == -1)
        uri->port = scheme_default_port(uri->scheme);

    /* Path, section 3.3. We include the query and fragment in the path. The
       path is also not percent-decoded because we just pass it on to the origin
       server. */
    q = strchr(p, '\0');
    uri->path = mkstr(p, q);

    return uri;

fail:
    uri_free(uri);
    return NULL;
}

/* Parse the authority part of a URI. userinfo (user name and password) are not
   supported and will cause an error if present. See RFC 3986, section 3.2.
   Returns NULL on error. */
struct uri *uri_parse_authority(struct uri *uri, const char *authority)
{
    const char *portsep;
    const char *host_start, *host_end;
    char *tail;

    /* We do not support "user:pass@" userinfo. The proxy has no use for it. */
    if (strchr(authority, '@') != NULL)
        return NULL;

    /* Find the beginning and end of the host. */
    host_start = authority;
    if (*host_start == '[') {
        /* IPv6 address in brackets. */
        host_start++;
        host_end = strchr(host_start, ']');
        if (host_end == NULL)
            return NULL;
        portsep = host_end + 1;
        if (!(*portsep == ':' || *portsep == '\0'))
            return NULL;
    } else {
        portsep = strrchr(authority, ':');
        if (portsep == NULL)
            portsep = strchr(authority, '\0');
        host_end = portsep;
    }

    /* Get the port number. */
    if (*portsep == ':') {
        long n;

        errno = 0;
        n = parse_long(portsep + 1, &tail);
        if (errno != 0 || *tail != '\0' || tail == portsep + 1 || n < 1 || n > 65535)
            return NULL;
        uri->port = n;
    } else {
        uri->port = -1;
    }

    /* Get the host. */
    uri->host = mkstr(host_start, host_end);
    if (percent_decode(uri->host) < 0) {
        free(uri->host);
        uri->host = NULL;
        return NULL;
    }

    return uri;
}

static void http_header_node_free(struct http_header *node) 
{
    free(node->name);
    free(node->value);
    free(node);
}

void http_header_free(struct http_header *header)
{
    struct http_header *p, *next;

    for (p = header; p != NULL; p = next) {
        next = p->next;
        http_header_node_free(p);
    }
}

/* RFC 2616, section 2.2; see LWS. */
static int is_space_char(int c)
{
    return c == ' ' || c == '\t';
}

/* RFC 2616, section 2.2. */
static int is_sep_char(int c)
{
    return c != '\0' && strchr("()<>@,;:\\\"/[]?={} \t", c) != NULL;
}

/* RFC 2616, section 2.2. */
static int is_token_char(int c)
{
    return !iscntrl(c) && !is_sep_char(c);
}

static int is_crlf(const char *s)
{
    return *s == '\n' || (*s == '\r' && *(s + 1) == '\n');
}

static const char *skip_crlf(const char *s)
{
    if (*s == '\n')
        return s + 1;
    else if (*s == '\r' && *(s + 1) == '\n')
        return s + 2;

    assert(0);
    return NULL;
}

static int field_name_equal(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0') {
        if (tolower(*a) != tolower(*b))
            return 0;
        a++;
        b++;
    }

    return *a == '\0' && *b == '\0';
}

/* Get the value of every header with the given name, separated by commas. If
   you only want the first value for header fields that should not be
   concatentated in this way, use http_header_get_first. The returned string
   must be freed. */
char *http_header_get(const struct http_header *header, const char *name)
{
    const struct http_header *p;
    char *buf = NULL;
    size_t size = 0, offset = 0;
    int count;

    count = 0;
    for (p = header; p != NULL; p = p->next) {
        /* RFC 2616, section 4.2: "Multiple message-header fields with the same
           field-name MAY be present in a message if and only if the entire
           field-value for that header field is defined as a comma-separated
           list [i.e., #(values)]. It MUST be possible to combine the multiple
           header fields into one "field-name: field-value" pair, without
           changing the semantics of the message, by appending each subsequent
           field-value to the first, each separated by a comma." */
        if (field_name_equal(p->name, name)) {
            if (count > 0)
                strbuf_append_str(&buf, &size, &offset, ", ");
            strbuf_append_str(&buf, &size, &offset, p->value);
            count++;
        }
    }

    return buf;
}

/* Get the value of the first header with the given name. The returned string
   must be freed. */
char *http_header_get_first(const struct http_header *header, const char *name)
{
    const struct http_header *p;

    for (p = header; p != NULL; p = p->next) {
        if (field_name_equal(p->name, name))
            return Strdup(p->value);
    }

    return NULL;
}

struct http_header *http_header_set(struct http_header *header, const char *name, const char *value)
{
    struct http_header *node, **prev;

    header = http_header_remove(header, name);

    node = (struct http_header *) safe_malloc(sizeof(*node));
    node->name = Strdup(name);
    node->value = Strdup(value);
    node->next = NULL;

    /* Link it to the end of the list. */
    for (prev = &header; *prev != NULL; prev = &(*prev)->next)
        ;
    *prev = node;

    return header;
}

/* Read a token from a space-separated string. This only recognizes space as a
   separator, so the string must already have had LWS normalized.
   http_header_parse does this normalization. */
static const char *read_token(const char *s, char **token)
{
    const char *t;

    while (*s == ' ')
        s++;
    t = s;
    while (is_token_char(*t))
        t++;
    if (s == t)
        return NULL;

    *token = mkstr(s, t);

    return t;
}

static const char *read_token_list(const char *s, char **tokens[], size_t *n)
{
    char *token;

    *tokens = NULL;
    *n = 0;

    for (;;) {
        s = read_token(s, &token);
        if (s == NULL) {
            int i;

            for (i = 0; i < *n; i++)
                free(*tokens);
            free(*tokens);

            return NULL;
        }

        *tokens = (char **) safe_realloc(*tokens, (*n + 1) * sizeof((*tokens)[0]));
        (*tokens)[(*n)++] = token;
        if (*s != ',')
            break;
        s++;
    }

    return s;
}

struct http_header *http_header_remove(struct http_header *header, const char *name)
{
    struct http_header *p, *next, **prev;

    prev = &header;
    for (p = header; p != NULL; p = next) {
        next = p->next;
        if (field_name_equal(p->name, name)) {
            *prev = next;
            http_header_node_free(p);
            continue;
        }
        prev = &p->next;
    }

    return header;
}

/* Removes hop-by-hop headers listed in section 13.5.1 of RFC 2616, and
   additionally removes any headers listed in the Connection header as described
   in section 14.10. */
int http_header_remove_hop_by_hop(struct http_header **header)
{
    static const char *HOP_BY_HOP_HEADERS[] = {
        "Connection",
        "Keep-Alive",
        "Proxy-Authenticate",
        "Proxy-Authorization",
        "TE",
        "Trailers",
        "Transfer-Encoding",
        "Upgrade",
    };
    char *connection;
    char **connection_tokens;
    size_t num_connection_tokens;
    unsigned int i;

    connection = http_header_get(*header, "Connection");
    if (connection != NULL) {
        const char *p;

        p = read_token_list(connection, &connection_tokens, &num_connection_tokens);
        if (p == NULL) {
            free(connection);
            return 400;
        }
        if (*p != '\0') {
            free(connection);
            for (i = 0; i < num_connection_tokens; i++)
                free(connection_tokens[i]);
            free(connection_tokens);
            return 400;
        }
        free(connection);
    } else {
        connection_tokens = NULL;
        num_connection_tokens = 0;
    }

    for (i = 0; i < sizeof(HOP_BY_HOP_HEADERS) / sizeof(HOP_BY_HOP_HEADERS[0]); i++)
        *header = http_header_remove(*header, HOP_BY_HOP_HEADERS[i]);
    for (i = 0; i < num_connection_tokens; i++)
        *header = http_header_remove(*header, connection_tokens[i]);

    for (i = 0; i < num_connection_tokens; i++)
        free(connection_tokens[i]);
    free(connection_tokens);

    return 0;
}

char *http_header_to_string(const struct http_header *header, size_t *n)
{
    const struct http_header *p;
    char *buf = NULL;
    size_t size = 0, offset = 0;

    strbuf_append_str(&buf, &size, &offset, "");

    for (p = header; p != NULL; p = p->next)
        strbuf_sprintf(&buf, &size, &offset, "%s: %s\r\n", p->name, p->value);

    if (n != NULL)
        *n = offset;

    return buf;
}

void http_request_init(struct http_request *request)
{
    request->method = NULL;
    uri_init(&request->uri);
    request->version = HTTP_UNKNOWN;
    request->header = NULL;
    request->content_length = 0;
    request->bytes_transferred = 0;
}

void http_request_free(struct http_request *request)
{
    free(request->method);
    uri_free(&request->uri);
    http_header_free(request->header);
}

char *http_request_to_string(const struct http_request *request, size_t *n)
{
    const char *path;
    char *buf = NULL;
    size_t size = 0, offset = 0;

    /* RFC 2616, section 5.1.2: "the absolute path cannot be empty; if none is
       present in the original URI, it MUST be given as "/" (the server
       root)." */
    path = request->uri.path;
    if (path[0] == '\0')
        path = "/";

    if (request->version == HTTP_09) {
        /* HTTP/0.9 doesn't have headers. See
           http://www.w3.org/Protocols/HTTP/AsImplemented.html. */
        strbuf_sprintf(&buf, &size, &offset, "%s %s\r\n", request->method, path);
    } else {
        const char *version;
        char *header_str;

        if (request->version == HTTP_10)
            version = " HTTP/1.0";
        else
            version = " HTTP/1.1";

        header_str = http_header_to_string(request->header, NULL);
        strbuf_sprintf(&buf, &size, &offset, "%s %s%s\r\n%s\r\n",
            request->method, path, version, header_str);
        free(header_str);
    }

    if (n != NULL)
        *n = offset;

    return buf;
}

void http_response_init(struct http_response *response)
{
    response->version = HTTP_UNKNOWN;
    response->code = 0;
    response->phrase = NULL;
    response->header = NULL;
    response->content_length = 0;
    response->bytes_transferred = 0;
}

void http_response_free(struct http_response *response)
{
    free(response->phrase);
    http_header_free(response->header);
}

char *http_response_to_string(const struct http_response *response, size_t *n)
{
    char *buf = NULL;
    size_t size = 0, offset = 0;

    if (response->version == HTTP_09) {
        /* HTTP/0.9 doesn't have a Status-Line or headers. See
           http://www.w3.org/Protocols/HTTP/AsImplemented.html. */
        return Strdup("");
    } else {
        const char *version;
        char *header_str;

        if (response->version == HTTP_10)
            version = "HTTP/1.0";
        else
            version = "HTTP/1.1";

        header_str = http_header_to_string(response->header, NULL);
        strbuf_sprintf(&buf, &size, &offset, "%s %d %s\r\n%s\r\n",
            version, response->code, response->phrase, header_str);
        free(header_str);
    }

    if (n != NULL)
        *n = offset;

    return buf;
}

int http_read_header(struct socket_buffer *buf, char **result)
{
    char *line = NULL;
    size_t n = 0;
    size_t count;
    int blank;

    *result = NULL;

    do {
        line = socket_buffer_readline(buf, &count, MAX_HEADER_LENGTH);
        if (line == NULL) {
            free(*result);
            if (n >= MAX_HEADER_LENGTH)
                /* Request Entity Too Large. */
                return 413;
            else
                return 400;
        }
        blank = is_crlf(line);

        if (n + count >= MAX_HEADER_LENGTH) {
            free(line);
            free(*result);
            /* Request Entity Too Large. */
            return 413;
        }

        *result = (char *) safe_realloc(*result, n + count + 1);
        memcpy(*result + n, line, count);
        n += count;
        free(line);
    } while (!blank);
    (*result)[n] = '\0';

    return 0;
}

static const char *skip_lws(const char *s)
{
    for (;;) {
        while (is_space_char(*s))
            s++;

        if (*s == '\n' && is_space_char(*(s + 1)))
            s += 1;
        else if (*s == '\r' && *(s + 1) == '\n' && is_space_char(*(s + 2)))
            s += 2;
        else
            break;
    }

    return s;
}

/* See section 4.2 of RFC 2616 for header format. */
int http_parse_header(struct http_header **result, const char *header)
{
    const char *p, *q;
    size_t value_len, value_offset;
    struct http_header *node, **prev;

    *result = NULL;
    prev = result;

    p = header;
    while (*p != '\0' && !is_crlf(p)) {
        /* Get the field name. */
        q = p;
        while (*q != '\0' && is_token_char(*q))
            q++;
        if (*q != ':') {
            http_header_free(*result);
            return 400;
        }

        node = (struct http_header *) safe_malloc(sizeof(*node));
        node->name = mkstr(p, q);
        node->value = NULL;
        node->next = NULL;
        value_len = 0;
        value_offset = 0;

        /* Copy the header field value until we hit a CRLF. */
        p = q + 1;
        p = skip_lws(p);
        for (;;) {
            q = p;
            while (*q != '\0' && !is_space_char(*q) && !is_crlf(q)) {
                /* Section 2.2 of RFC 2616 disallows control characters. */
                if (iscntrl(*q)) {
                    http_header_node_free(node);
                    return 400;
                }
                q++;
            }
            strbuf_append(&node->value, &value_len, &value_offset, p, q - p);
            p = skip_lws(q);
            if (is_crlf(p))
                break;
            /* Replace LWS with a single space. */
            strbuf_append_str(&node->value, &value_len, &value_offset, " ");
        }
        *prev = node;
        prev = &node->next;

        p = skip_crlf(p);
    }

    return 0;
}

static int http_header_get_content_length(const struct http_header *header, unsigned long *content_length)
{
    char *content_length_s;
    char *tail;
    int code;

    content_length_s = http_header_get_first(header, "Content-Length");
    if (content_length_s == NULL) {
        *content_length = 0;
        return 0;
    }

    code = 0;

    errno = 0;
    *content_length = parse_long(content_length_s, (char **) &tail);
    if (errno != 0 || *tail != '\0' || tail == content_length_s)
        code = 400;
    free(content_length_s);

    return code;
}

/* Parse a header and fill in any relevant fields in the request structure. */
int http_request_parse_header(struct http_request *request, const char *header)
{
    int code;

    code = http_parse_header(&request->header, header);
    if (code != 0)
        return code;
    code = http_header_get_content_length(request->header, &request->content_length);
    if (code != 0)
        return code;

    return 0;
}

/* Parse a header and fill in any relevant fields in the response structure. */
int http_response_parse_header(struct http_response *response, const char *header)
{
    int code;

    code = http_parse_header(&response->header, header);
    if (code != 0)
        return code;
    code = http_header_get_content_length(response->header, &response->content_length);
    if (code != 0)
        return code;

    return 0;
}

int http_read_request_line(struct socket_buffer *buf, char **line)
{
    size_t n;

    *line = NULL;

    /* Section 4.1 of RFC 2616 says "servers SHOULD ignore any empty line(s)
       received where a Request-Line is expected." */
    do {
        free(*line);
        *line = socket_buffer_readline(buf, &n, MAX_REQUEST_LINE_LENGTH);
        if (*line == NULL) {
            if (n >= MAX_REQUEST_LINE_LENGTH)
                /* Request Entity Too Large. */
                return 413;
            else
                return 400;
        }
    } while (is_crlf(*line));

    return 0;
}

/* Returns the character pointer after the HTTP version, or s if there was a
   parse error. */
static const char *parse_http_version(const char *s, enum http_version *version)
{
    const char *PREFIX = "HTTP/";
    const char *p, *q;
    long major, minor;

    *version = HTTP_UNKNOWN;

    p = s;
    if (memcmp(p, PREFIX, strlen(PREFIX)) != 0)
        return s;
    p += strlen(PREFIX);

    /* Major version. */
    errno = 0;
    major = parse_long(p, (char **) &q);
    if (errno != 0 || q == p)
        return s;

    p = q;
    if (*p != '.')
        return s;
    p++;

    /* Minor version. */
    errno = 0;
    minor = parse_long(p, (char **) &q);
    if (errno != 0 || q == p)
        return s;

    if (major == 1 && minor == 0)
        *version = HTTP_10;
    else if (major == 1 && minor == 1)
        *version = HTTP_11;

    return q;
}

int http_parse_request_line(const char *line, struct http_request *request)
{
    const char *p, *q;
    struct uri *uri;
    char *uri_s;

    http_request_init(request);

    p = line;
    while (*p == ' ')
        p++;

    /* Method (CONNECT, GET, etc.). */
    q = p;
    while (is_token_char(*q))
        q++;
    if (p == q)
        goto badreq;
    request->method = mkstr(p, q);

    /* URI. */
    p = q;
    while (*p == ' ')
        p++;
    q = p;
    while (*q != '\0' && *q != ' ')
        q++;
    if (p == q)
        goto badreq;
    uri_s = mkstr(p, q);

    /* RFC 2616, section 5.1.2:
         Request-URI    = "*" | absoluteURI | abs_path | authority
       The absoluteURI form is REQUIRED when the request is being made to a
       proxy... The authority form is only used by the CONNECT method. */
    if (strcmp(request->method, "CONNECT") == 0) {
        uri = uri_parse_authority(&request->uri, uri_s);
    } else {
        uri = uri_parse(&request->uri, uri_s);
    }
    free(uri_s);
    if (uri == NULL)
        /* The URI parsing failed. */
        goto badreq;

    /* Version number. */
    p = q;
    while (*p == ' ')
        p++;
    if (*p == '\0') {
        /* No HTTP/X.X version number indicates version 0.9. */
        request->version = HTTP_09;
    } else {
        q = parse_http_version(p, &request->version);
        if (p == q)
            goto badreq;
    }

    return 0;

badreq:
    http_request_free(request);
    return 400;
}

int http_read_status_line(struct socket_buffer *buf, char **line)
{
    size_t n;

    /* RFC 2616, section 6.1: "The first line of a Response message is the
       Status-Line... No CR or LF is allowed except in the final CRLF sequence."
       Contrast that with Request-Line, which allows leading blank lines. */
    *line = socket_buffer_readline(buf, &n, MAX_STATUS_LINE_LENGTH);
    if (*line == NULL) {
        if (n >= MAX_STATUS_LINE_LENGTH)
            /* Request Entity Too Large. */
            return 413;
        else
            return 400;
    }

    return 0;
}

/* Returns 0 on success and nonzero on failure. */
int http_parse_status_line(const char *line, struct http_response *response)
{
    const char *p, *q;

    http_response_init(response);

    /* Version. */
    p = parse_http_version(line, &response->version);
    if (p == line)
        return -1;
    while (*p == ' ')
        p++;

    /* Status code. */
    errno = 0;
    response->code = parse_long(p, (char **) &q);
    if (errno != 0 || q == p)
        return -1;
    p = q;

    /* Reason phrase. */
    while (*p == ' ')
        p++;
    q = p;
    while (!is_crlf(q))
        q++;
    /* We expect that the CRLF ends the string. */
    if (*skip_crlf(q) != '\0')
        return -1;
    response->phrase = mkstr(p, q);

    return 0;
}

/* userpass is a user:pass string (the argument to --proxy-auth). value is the
   value of the Proxy-Authorization header field. Returns 0 on authentication
   failure and nonzero on success. */
int http_check_auth_basic(const char *userpass, const char *value)
{
    const char *p, *q;
    char *scheme;
    char *base64_expected;
    char *base64_received;
    int cmp;

    p = value;
    p = read_token(value, &scheme);
    if (p == NULL)
        return 0;
    if (strcmp(scheme, "Basic") != 0) {
        if (o.debug > 1)
            logdebug("Got authentication scheme \"%s\", expected \"Basic\".\n", scheme);
        free(scheme);
        return 0;
    }
    free(scheme);

    /* Grab the received base64-encoded user name and password. */
    while (*p == ' ')
        p++;
    q = p;
    while (*q != '\0' && *q != ' ')
        q++;
    /* Should be at the end of the string now. */
    if (*q != '\0')
        return 0;
    base64_received = mkstr(p, q);

    base64_expected = b64enc((unsigned char *) userpass, strlen(userpass));

    /* We don't decode the received password, we encode the expected password
       and compare the encoded strings. */
    cmp = strcmp(base64_expected, base64_received);
    free(base64_expected);
    free(base64_received);

    return cmp == 0;
}
