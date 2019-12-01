#include "../inc/cool.h"

int call_api(FILE *pf)
{
  CURL        *curl;
  CURLcode    res;

  curl = curl_easy_init();
  if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, "http://fr.openfoodfacts.org/api/v0/product/5010677850100.json");
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)pf);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}

char *parse_str(char *str, const char *handle)
{
  char *new = NULL;
  char *add_del;
  long  del;
  int  i;

  i = 0;
  while (str[i])
  {
    if (str[i] == ',')
    {
      if (ft_strnstr(&str[i], handle, strlen(handle) + 1))
      {
        str = strchr(str + i, ':');
        add_del = strchr(str + 2, '"');
        del = (add_del - str) - 2;
        new = calloc(1, del);
        strncpy(new, str + 2, del);
        *(new + del) = 0;
        return (new);
      }
    }
    i++;
  }
  return new;
}

void read_file(FILE *pf, char **str)
{
	char  buff[1024];

  *str = calloc(1, 1);
  if (ftell(pf))
    fseek(pf, 0, SEEK_SET);
  while (fread(buff, 1023, 1, pf), !feof(pf))
  {
    buff[1023] = 0;
    *str = strjoin(*str, buff);
  }
}
