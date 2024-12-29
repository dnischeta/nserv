#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "char_buf.h"

CharBuf *create_char_buf(size_t cap)
{
    CharBuf *cv = (CharBuf *)malloc(sizeof(CharBuf));

    if (cv == NULL)
    {
        perror("Failed to allocate CharBuf: ");
        exit(1); // TODO: shutdown server properly
    }

    char *buf = (char *)malloc(cap);

    if (cv == NULL)
    {
        perror("Failed to allocate CharBuf buffer: ");
        exit(1); // TODO: shutdown server properly
    }

    cv->buf = buf;
    cv->cap = cap;
    cv->len = 0;

    return cv;
}

void clear_char_buf(CharBuf *cv)
{
    cv->len = 0;
}

int append_str_char_buf(CharBuf *cv, char *s)
{
    if (strlen(s) > (cv->cap - cv->len))
        return -1;

    while (*s)
        cv->buf[cv->len++] = *(s++);

    return 0;
}

int append_bytes_char_buf(CharBuf *cv, char *b, size_t n)
{
    if (n > (cv->cap - cv->len))
        return -1;

    int idx = 0;
    while (idx < n)
        cv->buf[cv->len++] = *(b + idx++);

    return 0;
}
