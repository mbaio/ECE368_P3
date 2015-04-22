// Project #3 --- Submission by: Michael Baio --- PUID: 0026281964 //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define bufflen 1000

typedef struct linky
{
  int ID;
  double friendship_dist;
} User_el;

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



char * * explode(const char *, const char *, int *);
//List_network * create_network(int, float);
//void List_push(List_network * head, int id, float UL);
void display_queries(User_el **,double, double,int,int,double);


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
  double S1 = atof(exploded_line[1]);
  double S2 = atof(exploded_line[2]);
  int node_query = atoi(exploded_line[3]);
  double alpha = atof(exploded_line[4]);
 
  for (ind = 0; ind < 5; ind++)
  {
    free(exploded_line[ind]);
  }
  free(exploded_line);
  
  int user_index = 0;
  User * user_arr = malloc(sizeof(User) * users);
  User_el ** graph_grid = malloc(sizeof(User_el *) * users);
  
  // Getting User ID's
  while (fgets(line,bufflen,fptr))
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
  double UL;
  double Lmax = 0;
  for (ind = 0; ind < users; ind++)
  {
    graph_grid[ind] = malloc(sizeof(User_el) * (users + 1));
    graph_grid[ind][0].ID = user_arr[ind].ID;
    graph_grid[ind][0].friendship_dist = 0;
    
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
       graph_grid[ind][ind2 + 1].ID = user_arr[ind2].ID;
       graph_grid[ind][ind2 + 1].friendship_dist = UL;
    }
  }
  double s_dense;
  double s_sparse;
  if (S1 <= S2) {
    s_dense = S1;
    s_sparse = S2; }
  else {
    s_dense = S2;
    s_sparse = S1; }
    
  for(ind = 0; ind < users; ind++)
  {
    printf("\nUser #%d\n",graph_grid[ind][0].ID);
    for (ind2 = 1; ind2 < users + 1; ind2++)
    {
      printf("\tNeighbor: %d\tDistance: %lf\n",graph_grid[ind][ind2].ID,graph_grid[ind][ind2].friendship_dist);
    }
  }
  printf("Lmax = %lf\n",Lmax);
    
   printf("\nDisplaying Queries 1-6 for a Dense representation of this Social Network...\n");
   display_queries(graph_grid,s_dense, alpha, users, node_query,Lmax);
//   printf("\nDisplaying Queries 1-6 for a Sparse representation of this Social Network...\n");
//   display_queries(list,s_sparse, alpha, users, node_query);
  
  return 0;
}

void display_queries(User_el ** graph_grid,double S, double alpha, int users, int node_query, double Lmax)
{
  printf("\nFirst displaying Friendship edges with S = %lf\n\n",S);
  int ind;
  int ind2;
  double L;
  for(ind = 0; ind < users; ind++)
  {
    printf("User: %d\n",graph_grid[ind][0].ID);
    for (ind2 = 0; ind2 < users + 1; ind2++)
    {
      L = (1 - (graph_grid[ind][ind2].friendship_dist / Lmax));
      
      if (L > S && graph_grid[ind][ind2].ID != graph_grid[ind][0].ID)
	graph_grid[ind][ind2].friendship_dist = (float)((int)(L * 100)) / 100;
      else
	graph_grid[ind][ind2].friendship_dist = -1;
      printf("\tNeighbor: %d\tL = %lf\tDistance: %lf\n",graph_grid[ind][ind2].ID,L,graph_grid[ind][ind2].friendship_dist);
    }
  }
  printf("\n\n");
  return;
}

// void List_push(List_network * head, int id, float UL)
// {
//         List_network * addnode = create_network(id,UL);
//         List_network * finder = head;
// 	while (1) {
// 	  if (finder -> next == NULL)
// 	  {
// 	    finder -> next = addnode;
// 	    break;
// 	  }
// 	  finder = finder -> next;
// 	}
// }
// 
// List_network * create_network(int ID, float dist)
// {
//   List_network * node = malloc(sizeof(List_network));
//   node -> ID = ID;
//   node -> friendship_dist_1 = dist;
//   node -> friendship_dist_2 = 0;
//   node -> next = NULL;
//   return node;
// }


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