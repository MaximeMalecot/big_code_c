#include "../inc/cool.h"


int add_product()
{
  if(!begin)
    return 0;

  t_prod *tmp = begin;

  while(tmp)
  {
    insert_bdd(tmp);
    tmp = tmp->next;
  }

  return 0;
}

char * get_date(char * date)
{
  int day, month, year;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  day = tm.tm_mday;
  month = tm.tm_mon + 1;
  year = tm.tm_year + 1900;
  sprintf(date,"%d-%d-%d", day, month, year);

  return date;

}
char * lowercase(char * string){
  int i = 0;

  while (string[i] != '\0'){
    if (string[i] >= 65 && string[i] <= 90){
      string[i] += 32;
    }
    i++;
  }
  return string;
}

char * delete_space(char * string){
  while (strchr(string, ' ') != NULL){
    strcpy(strchr(string, ' '), (strchr(string, ' ') + 1));
  }

  return string;
}

char * uniform_unit(char * quantity, char * unity){
  char liquidunity[] = "cl";
  char solidunity[] = "g";
  char * string;
  double nbquantity;

  string = malloc(sizeof(char) * 50);
  if (strstr(unity, "ml") != NULL){
    sscanf(quantity, "%lf", &nbquantity);
    nbquantity /= 10;
    sprintf(string, "%.2lf %s", nbquantity, liquidunity);
    return string;
  }
  else if( strstr(unity, "l") != NULL){
    sscanf(quantity, "%lf", &nbquantity);
    nbquantity *= 100;
    sprintf(string, "%.2lf %s", nbquantity, liquidunity);
    return string;
  }
  else if (strstr(unity, "kg") != NULL){
    sscanf(quantity, "%lf", &nbquantity);
    nbquantity *= 1000;
    sprintf(string, "%.2lf %s", nbquantity, solidunity);
    return string;
  }
  else if(strstr(unity, "mg") != NULL){
    sscanf(quantity, "%lf", &nbquantity);
    nbquantity /= 1000;
    sprintf(string, "%.2lf %s", nbquantity, solidunity);
    return string;
  }

}


char * uniform_quantity(char * string){
  char quantite[10];
  int pt;

  lowercase(string);
  delete_space(string);
  if( strstr(string, "ml") != NULL){
    pt = strstr(string, "ml") - string;
    strncpy(quantite, string, pt);
    quantite[pt] = '\0';
    strcpy(string,uniform_unit(quantite, string));
    return string;
  }
  else if( strstr(string, "cl") != NULL){
    return string;
  }
  else if( strstr(string, "l") != NULL){
    pt = strstr(string, "l") - string;
    strncpy(quantite, string, pt);
    quantite[pt] = '\0';
    strcpy(string,uniform_unit(quantite, string));
    return string;
  }
  else if(strstr(string, "kg") != NULL){
    pt = strstr(string, "kg") - string;
    strncpy(quantite, string, pt);
    quantite[pt] = '\0';
    strcpy(string,uniform_unit(quantite, string));
    return string;
  }
  else if(strstr(string, "mg") != NULL){
    pt = strstr(string, "mg") - string;
    strncpy(quantite, string, pt);
    quantite[pt] = '\0';
    strcpy(string,uniform_unit(quantite, string));
    return string;
  }
  else
    return string;
}


char * total_quantity(char * quantity, int nb){
  char quantite[10];
  char unity[5];
  double nbquantite;
  int pt;

  if(strstr(quantity, "cl") != NULL){
    pt = strstr(quantity, "cl") - quantity;
    strncpy(quantite, quantity, pt);
    quantite[pt] = '\0';
    strcpy(unity, strstr(quantity, "cl"));
    sscanf(quantite, "%lf", &nbquantite);
    nbquantite *= nb;
    sprintf(quantity, "%.2lf %s", nbquantite, unity);
    return quantity;
  }
  else if(strstr(quantity, "g") != NULL){
    pt = strstr(quantity, "g") - quantity;
    strncpy(quantite, quantity, pt);
    quantite[pt] = '\0';
    strcpy(unity, strstr(quantity, "g"));
    sscanf(quantite, "%lf", &nbquantite);
    nbquantite *= nb;
    sprintf(quantity, "%.2lf %s", nbquantite, unity);
    return quantity;

  }
  else{
    sscanf(quantity, "%lf", &nbquantite);
    nbquantite *= nb;
    sprintf(quantity, "%.2lf", nbquantite);
    return quantity;
  }
}



char * get_peremption(char * date, char * tmp){
  int final;
  int day, month, year;
  char dayc[3], monthc[3], yearc[5];
  char month30 []= "04,06,09,11";
  char month31 []= "01,03,05,07,08,10,12";
  sscanf(tmp, "%d", &final);

  sscanf(date,"%d-%d-%d", &day, &month, &year);
  while (final != 0){
    day++;
    if(day > 28 && strcmp(monthc, "02")==0 && (year%4)!=0){
      month++;
      day = 01;
    }
    else if(day > 29 && strcmp(monthc, "02")==0 && (year%4) ==0){
      month++;
      day = 01;
    }
    if ((day > 30) && (strstr(month30, monthc) != NULL)){
      month++;
      day = 01;
    }
    if(day > 31 && (strstr(month31, monthc)!= NULL)){
      month++;
      day = 01;
    }
    if( month > 12){
      month = 01;
      year ++;
    }
    if (day < 10)
      sprintf(dayc, "0%d", day);
    else
      sprintf(dayc, "%d", day);
    if (month < 10)
      sprintf(monthc, "0%d", month);
    else
      sprintf(monthc, "%d", month);
    sprintf(yearc, "%d", year);
    final--;
  }
  sprintf(date, "%s-%s-%s", dayc, monthc, yearc);
  return date;
}

int insert_bdd(t_prod *tmp){
//////////////////////Convertion des données////////////////////////////
  char * date;
  char * peremption;
  char res_per[15];
  char id_ing[5];
  char id_stock[5];
  char request[255];
  MYSQL * con;
  MYSQL_RES * result;
  MYSQL_ROW  row;

  con = NULL;

  connection_bdd(con);
  date = malloc(sizeof(char) * 255);
  peremption = malloc(sizeof(char) * 255);

  get_date (date);

  sprintf(request, "SELECT id, peremption FROM ingredient WHERE nom = %s;", tmp->name);
  if(mysql_query(con, request))
  {
    finish_with_error(con);
  }

  result = mysql_store_result(con);

  if (result == NULL)
  {
    finish_with_error(con);
  }

  row = mysql_fetch_row(result);

  strcpy(id_ing, row[0]);
  strcpy(res_per, row[1]);

  if (res_per == NULL){
    peremption = NULL;
    get_date(date);
  }
  else{
    get_date(date);
    get_peremption(strcpy(peremption, date), res_per);

  }
  uniform_quantity(tmp->quantity);

  total_quantity(tmp->quantity, tmp->nb);

  sprintf(request, "INSERT INTO stock (nom, marque, quantite, date_ajout, date_expire) VALUES ('%s', '%s', '%s', '%s', '%s');", tmp->name, tmp->brand, tmp->quantity, date, peremption);

  mysql_query(con, request);

  sprintf(request, "SELECT id FROM stock WHERE nom = %s AND marque = %s AND date_ajout = %s ;", tmp->name, tmp->brand, date);
  if(mysql_query(con, request))
  {
    finish_with_error(con);
  }
  result = mysql_store_result(con);

  if(result == NULL){
    finish_with_error(con);
  }
  row = mysql_fetch_row(result);

  strcpy(id_stock, row[0]);

  sprintf(request, "INSERT INTO contenant ( id_stock, id_ingredient) VALUES ('%s', '%s');", id_stock, id_ing);







  return 0;
}
