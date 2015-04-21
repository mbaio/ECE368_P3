// Project #3 --- Submission by: Michael Baio --- PUID: 0026281964 //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define bufflen 1000

typedef struct linky
{
  int ID;
  int friendship_dist;
  struct linky * next;
} List_network;

typedef struct users
{
  int ID;
  int age;
  int gender;
  int martial;
  int race;
  int birth;
  int lang;
  int occ;
  int income;
} User;



void min_path_1(List_network **, int);
void dist_alpha_2(List_network **, int, float);
void direct_conn_3(List_network **,int);
void hop_2_3(List_network **, int);
void avg_deg_4(List_network **, int);
void avg_2nd_5(List_network **, int);
char * * explode(const char *, const char *, int *);
List_network * create_network(int, float);


int main(int argc, char ** argv)
{
  int ind;
  FILE * fptr = fopen(argv[1],"r");
  if (fptr == NULL)
    return EXIT_FAILURE;
  int arr_len;
  char * line = malloc(sizeof(char) * bufflen);
  char * delims = ",";
  char ** exploded_line;
  
  // Getting Starting info
  fgets(line,bufflen,fptr);
  exploded_line = explode(line,delims,&arr_len);
  int users = atoi(exploded_line[0]);
  float S1 = atof(exploded_line[1]);
  float S2 = atof(exploded_line[2]);
  int node_query = atoi(exploded_line[3]);
  float alpha = atof(exploded_line[4]);
  for (ind = 0; ind < 5; ind++)
  {
    free(exploded_line[ind]);
  }
  free(exploded_line);
  
  int user_index = 0;
  User * user_arr = malloc(sizeof(User) * users);
  List_network ** = malloc(sizeof(List_network *) * users);
  
  // Getting User ID's
  while (fgets(line,bufflen,fptr) != NULL)
  {
    exploded_line = explode(line,delims,&arr_len);
    int ID = atoi(exploded_line[0]);
    int age = atoi(exploded_line[1]);
    int gender = atoi(exploded_line[2]);
    int martial = atoi(exploded_line[3]);
    int race = atoi(exploded_line[4]);
    int birth = atoi(exploded_line[5]);
    int lang = atoi(exploded_line[6]);
    int occ = atoi(exploded_line[7]);
    int income = atoi(exploded_line[8]);
    
  
  
  
  
  
  
  return 0;
}


char * * explode(const char * str, const char * delims, int * arrLen)
{// it mallocs for you
  // it gives you back arrLength
  int ind = 0; 
  int numdelim = 0;
  int letters = 0;
  const char * first; 
  int tempind = 0;
  while (str[ind] != '\0')
    {//count delimeters
      if ((strchr(delims,str[ind])) != NULL)
	{
	numdelim++; //add another delimeter to the count
	}
      ind++;
    }//end counting of delimeters
  char ** dest = malloc((numdelim + 1) * sizeof(char*));
  first = str;
  *arrLen = numdelim + 1;
  for(ind = 0; ind <= numdelim; ind ++){
     
    while ((strchr(delims,str[tempind])) == NULL)
	{
	  letters++;
	  tempind++;
	}
    dest[ind] = malloc((letters + 1) * sizeof(char));
    memcpy(dest[ind],first,(sizeof(char)*letters));
    dest[ind][letters] = '\0';
    tempind++;
    first +=  (sizeof(char)*(1 + letters));
    letters = 0;
  }
return dest;
}