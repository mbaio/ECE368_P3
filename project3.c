// Project #3 --- Submission by: Michael Baio --- PUID: 0026281964 //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define bufflen 1000

typedef struct linky
{
  int ID;
  float friendship_dist;
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
void List_push(List_network * head, int id, float UL);


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
  List_network ** list = malloc(sizeof(List_network *) * users);
  
  // Getting User ID's
  while (fgets(line,bufflen,fptr) != NULL)
  {
    exploded_line = explode(line,delims,&arr_len);
    user_arr[user_index].ID = atoi(exploded_line[0]);
    user_arr[user_index].age = atoi(exploded_line[1]);
    user_arr[user_index].gender = atoi(exploded_line[2]);
    user_arr[user_index].martial = atoi(exploded_line[3]);
    user_arr[user_index].race = atoi(exploded_line[4]);
    user_arr[user_index].birth = atoi(exploded_line[5]);
    user_arr[user_index].lang = atoi(exploded_line[6]);
    user_arr[user_index].occ = atoi(exploded_line[7]);
    user_arr[user_index].income = atoi(exploded_line[8]);
    for (ind = 0; ind < 9; ind++)
    {
      free(exploded_line[ind]);
    }
    free(exploded_line); 
    user_index++;
  }
  for(ind = 0; ind < users; ind++)
  {
    printf("ID: %d, age: %d, gender: %d, martial: %d, race: %d, birth: %d, lang: %d, occ: %d, income: %d\n", user_arr[ind].ID,user_arr[ind].age,user_arr[ind].gender,user_arr[ind].martial,user_arr[ind].race,user_arr[ind].birth,user_arr[ind].lang,user_arr[ind].occ,user_arr[ind].income);
  }
  printf("\n");
  
  // Build Network
  int sum = 0;
  int ind2;
  float UL;
  float Lmax = 0;
  float friendship_dist;
  for (ind = 0; ind < users; ind++)
  {
    List_network * node = create_network(user_arr[ind].ID,0);
    list[ind] = node;
    
    for (ind2 = 0; ind2 < users; ind2++)
    {
       sum = 0;
       sum += pow(user_arr[ind].age - user_arr[ind2].age,2);
       sum += pow(user_arr[ind].gender - user_arr[ind2].gender,2);
       sum += pow(user_arr[ind].martial - user_arr[ind2].martial,2);
       sum += pow(user_arr[ind].race - user_arr[ind2].race,2);
       sum += pow(user_arr[ind].birth - user_arr[ind2].birth,2);
       sum += pow(user_arr[ind].lang - user_arr[ind2].lang,2);
       sum += pow(user_arr[ind].occ - user_arr[ind2].occ,2);
       sum += pow(user_arr[ind].income - user_arr[ind2].income,2);
       UL = sqrt(sum);
       if (Lmax < UL)
	 Lmax = UL;
       List_push(list[ind],user_arr[ind2].ID,UL);
    }
  }
  List_network * list_ptr;
  for(ind = 0; ind < users; ind++)
  {
    list_ptr = list[ind];
    printf("\nUser: %d",list[ind]->ID);
    list_ptr = list_ptr -> next;
    while (list_ptr != NULL)
    { 
      printf("\n\tNeighbor: %d\tDistance: %f",list_ptr->ID,list_ptr->friendship_dist);
      list_ptr = list_ptr -> next;
    }
  }
  
  
  
  return 0;
}

void List_push(List_network * head, int id, float UL)
{
        List_network * addnode = create_network(id,UL);
        List_network * finder = head;
	while (1) {
	  if (finder -> next == NULL)
	  {
	    finder -> next = addnode;
	    break;
	  }
	  finder = finder -> next;
	}
}

List_network * create_network(int ID, float dist)
{
  List_network * node = malloc(sizeof(List_network));
  node -> ID = ID;
  node -> friendship_dist = dist;
  node -> next = NULL;
  return node;
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