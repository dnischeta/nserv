#ifndef HTTP_DEF
#define HTTP_DEF

#include "char_buf.h"

int parse_http(char *req, size_t req_len, CharBuf *res);

#endif // #ifndef HTTP_DEF