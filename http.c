#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "http.h"
#include "char_buf.h"
#include "hash_table.h"


int parse_http(char *raw, size_t req_len, CharBuf *res)
{
    HashTable *http_headers = ht_create(100);
    
    int left = 0, right = 0;
    FILE *fp;
    char *line;

    while (isspace(raw[left]))
    {
        left++;
    }

    right = left;

    while (!isspace(raw[right + 1]))
    {
        right++;
    }

    if (strncmp("GET", raw + left, right - left + 1) != 0)
    {
        printf("Unsupported method: %.4s\n", raw + left);
        return -1;
    }

    left = right + 2;
    right = right + 2;

    while (raw[right + 1] != 0 && !isspace(raw[right + 1]))
    {
        right++;
    }

    raw[right + 1] = 0;
    fp = fopen(raw + left + 1, "r");

    if (fp == NULL)
    {
        append_str_char_buf(res, "HTTP/1.1 404 Not Found\r\n\r\nFile not found");
        return 0;
    }

    size_t line_len;

    append_str_char_buf(res, "HTTP/1.1 200 OK\r\n\r\n");

    while ((line = fgetln(fp, &line_len)) != NULL)
    {
        append_bytes_char_buf(res, line, line_len);
    }

    fclose(fp);
    return 0;
}