/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_string.h
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015年10月30日 09时45分24秒
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s (), peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

int dtcurl_strstart(const char *str, const char *pfx, const char **ptr);
int dtcurl_stristart(const char *str, const char *pfx, const char **ptr);
char *dtcurl_stristr(const char *s1, const char *s2);
char *dtcurl_strnstr(const char *haystack, const char *needle, size_t hay_length);
size_t dtcurl_strlcpy(char *dst, const char *src, size_t size);
size_t dtcurl_strlcat(char *dst, const char *src, size_t size);
size_t dtcurl_strlcatf(char *dst, size_t size, const char *fmt, ...);
char *dtcurl_asprintf(const char *fmt, ...);
char *dtcurl_d2str(double d);
char *dtcurl_get_token(const char **buf, const char *term);
char *dtcurl_strtok(char *s, const char *delim, char **saveptr);
int dtcurl_strcasecmp(const char *a, const char *b);
int dtcurl_strncasecmp(const char *a, const char *b, size_t n);
