#include "../inc/cool.h"

char *make_path_screen(char *path, char *color, int c)
{
  int len;
  char *res = NULL;

  if (c)
  {
    strcpy(path, "img/background/");
    strcat(path, color);
    strcat(path, "_home.png");
  }
  else
  {
    len = strlen(color) + 27;
    if (!(res = malloc(1 * len)))
      return (NULL);
    strcpy(res, "img/background/");
    strcat(res, color);
    strcat(res, "_screen.png");
  }
  return (res);
}

int change_path_screen(char *path_screen)
{
  FILE *fd;
  FILE *new_fd;
  char buff[1024];
  char id[25];
  char c[2] = "0";

  if (!(fd = fopen("gladeproject.glade", "rb")))
    return (0);
  if (!(new_fd = fopen("tmp6576576", "wb+")))
    return (0);
  while (fgets(buff, 1023, fd))
  {
    strcpy(id, "id=\"screen_img");
    strcat(id, c);
    if (strstr(buff, id))
    {
      fprintf(new_fd, "%s", buff);
      while (fgets(buff, 1023, fd))
      {
        if (strstr(buff, "name=\"pixbuf"))
        {
          fprintf(new_fd, "\t\t\t%s%s%s\n", "<property name=\"pixbuf\">", path_screen, "</property>");
          break ;
        }
        else
          fprintf(new_fd, "%s", buff);
      }
      c[0]++;
    }
    else
      fprintf(new_fd, "%s", buff);
  }
  fclose(new_fd);
  fclose(fd);
  remove("gladeproject.glade");
  rename("tmp6576576", "gladeproject.glade");
  return (1);
}

int change_path_home(char *path_home)
{
  FILE *fd;
  FILE *new_fd;
  char buff[1024];

  if (!(fd = fopen("gladeproject.glade", "rb")))
    return (0);
  if (!(new_fd = fopen("tmp6576576", "wb+")))
    return (0);
  while (fgets(buff, 1023, fd))
  {
    if (strstr(buff, "id=\"home_img"))
    {
      fprintf(new_fd, "%s", buff);
      while (fgets(buff, 1023, fd))
      {
        if (strstr(buff, "name=\"pixbuf"))
        {
          fprintf(new_fd, "%s%s%s\n", "\t\t\t<property name=\"pixbuf\">", path_home, "</property>");
          break ;
        }
        else
          fprintf(new_fd, "%s", buff);
      }
    }
    else
      fprintf(new_fd, "%s", buff);
  }
  fclose(new_fd);
  fclose(fd);
  remove("gladeproject.glade");
  rename("tmp6576576", "gladeproject.glade");
  return (1);
}

const gchar *load_conf(void)
{
  FILE          *fd;
  char          buff[1024];
  char          color[10];
  char          path_home[30];
  char          *path_screen = NULL;


  if (!(fd = fopen("conf.con" , "rb")))
    return ("red");
  while (fgets(buff, 1023, fd))
  {
    if (strlen(buff))
      if ((buff[strlen(buff) - 1] = '\n'))
        buff[strlen(buff) - 1] = '\0';
    if (buff[0] != '#' && buff[0])
      strcpy(color, buff);
  }
  make_path_screen(path_home, color, 1);
  path_screen = make_path_screen(path_screen, color, 0);
  change_path_home(path_home);
  change_path_screen(path_screen);
  fclose(fd);
  page->img = path_screen;
  return ("red");
}
