#ifndef H_CHAR_BUF
#define H_CHAR_BUF

#include <ctype.h>

typedef struct CharBuf
{
    char *buf;
    size_t cap;
    size_t len;
} CharBuf;

CharBuf *create_char_buf(size_t cap);
void clear_char_buf(CharBuf *);
int append_str_char_buf(CharBuf *, char *);
int append_bytes_char_buf(CharBuf *, char *, size_t);

#endif // H_CHAR_BUF