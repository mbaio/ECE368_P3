/* Project #3 --- Submission by: Michael Baio --- PUID: 0026281964 //

  This project tests my ability to apply Vertex and Edge graphing 
  to the C language by modeling a social network.  6 querys are then
  done to analyze the social network based on two different threshold
  values to determine if two users are considered friends or not. 
  
  -Has minor valgrind errors- structs not freed at exit */ 
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFFLEN 1050

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

// User Functions
char * * explode(const char *, const char *, int *);
void display_queries(double **,double, double,int,int,double);
void dijkstras(int, int, double **,int,int,int *,int);

  int main(int argc, char ** argv)
{
  int ind;
  FILE * fptr = fopen(argv[1],"r");
  if (fptr == NULL)
    return EXIT_FAILURE;
  int arr_len;
  char * line = malloc(sizeof(char) * BUFFLEN);
  char * delims = ",";
  char ** exploded_line;
  
  // Getting Starting info
  fgets(line,BUFFLEN,fptr);
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
  double ** graph_grid = malloc(sizeof(double *) * users);
  double ** graph_grid2 = malloc(sizeof(double *) * users);
 
  // Getting User ID's
  while (fgets(line,BUFFLEN,fptr))
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
  
  // Build Network
  int sum = 0;
  int ind2;
  double UL;
  double Lmax = 0;
  for (ind = 0; ind < users; ind++)
  {
    graph_grid[ind] = malloc(sizeof(double) * (users));
    graph_grid2[ind] = malloc(sizeof(double) * (users));
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
       graph_grid[ind][ind2] = UL;
       graph_grid2[ind][ind2] = UL;
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
    
   display_queries(graph_grid,s_dense, alpha, users, node_query,Lmax);
   printf("\n\n");
   display_queries(graph_grid2,s_sparse, alpha, users, node_query,Lmax);
   fclose(fptr);
   return 0;
}

void display_queries(double ** graph_grid,double S, double alpha, int users, int node_query, double Lmax)
{
  int ind;
  int ind2;
  double L;
  int int_s = (int) (S * 100);
  for(ind = 0; ind < users; ind++)
  {
    for (ind2 = 0; ind2 < users; ind2++)
    {
      L = (1 - (graph_grid[ind][ind2] / Lmax));
      if ((int) (L * 100) > int_s && ind != ind2)
	graph_grid[ind][ind2] = (int)(L * 100);
      else
	graph_grid[ind][ind2] = -1;
    }
  }
  
  //Query #1
  int shortest_dist = 65535;
  int id_ind = node_query - 1;
  for(ind = 0; ind < users; ind++)
  {
    if (graph_grid[id_ind][ind] != -1 && shortest_dist > graph_grid[id_ind][ind])
    {
      shortest_dist = (int) graph_grid[id_ind][ind];
    }
  }
  printf("%.2lf",(double) shortest_dist / 100);
  for(ind = 0; ind < users; ind++)
  {
    if ((int)graph_grid[id_ind][ind] == shortest_dist)
    {
      printf(",%d",ind + 1);
    }
  }
  printf("\n");
  
  //Query #2 
  int count = 0;
  int * id_list = malloc(sizeof(int) * users);
  memset(id_list,0,sizeof(int) * users); 
  dijkstras(users, 0, graph_grid,(int)(alpha * 100),id_ind,id_list,-1);
  for(ind = 0; ind < users; ind++)
  {
    if (id_list[ind] == 1 && ind != id_ind) {
      //printf("\nnode path less than alpha: %d",ind + 1);
      count++; }
  }
  printf("%d\n",count);
  
  // Query 3
  count = 0;
  memset(id_list,0,sizeof(int) * users); 
  for (ind = 0; ind < users; ind++)
  {
    if (graph_grid[id_ind][ind] != -1)
    {
      count++;
    }
  }
  printf("%d",count);
  for (ind = 0; ind < users; ind++)
  {
    if (graph_grid[id_ind][ind] != -1)
    {
      printf(",%d",ind + 1);
      id_list[ind] = 1;
    }
  }  

  //Query #4   
  int * id_list_2 = malloc(sizeof(int) * users);
  memset(id_list_2,0,sizeof(int) * users);
  count = 0;
  for(ind = 0; ind < users; ind++)
  {
    if (id_list[ind] == 1)
    {
      for(ind2 = 0; ind2 < users; ind2++)
      {
	if (graph_grid[ind][ind2] != -1)
	  id_list_2[ind2] = 1;
      }
    }
  }
  for(ind = 0; ind < users; ind++)
  {
    if (id_list_2[ind] == 1 && ind != id_ind)
      count++;
  }
  printf("\n%d",count);
  for(ind = 0; ind < users; ind++)
  {
    if (id_list_2[ind] == 1 && ind != id_ind)
      printf(",%d",ind + 1);
  }
  
 //Query #5
  int sum = 0;
  for(ind = 0; ind < users; ind++)
  {
    count = 0;
    for(ind2 = 0; ind2 < users; ind2++)
    {
      if (graph_grid[ind][ind2] != -1)
	count++;
    }
    sum += count;
  }
  printf("\n%.2lf",(double) (int)((sum * 100) / users) / 100);
  
  // Query 6
  double sum_total = 0;
  int current_sum = 0;
  int ind3;
  for(ind = 0; ind < users; ind++)
  {
    current_sum = 0;
    memset(id_list_2,0,sizeof(int) * users);
    for(ind2 = 0; ind2 < users; ind2++)
    {  
      if(graph_grid[ind][ind2] != -1)
      {
	for(ind3 = 0; ind3 < users; ind3++)
	{
	  if(graph_grid[ind2][ind3] != -1 && ind3 != ind)
	  {
	    id_list_2[ind3] = 1;
	  }
	}
      }
    }
    for(ind3 = 0; ind3 < users; ind3++)
    {
      if (id_list_2[ind3] == 1)
      {
	current_sum++;
      }
    }
    sum_total += current_sum;
  }
  printf("\n%.2lf",(double) (int)((sum_total * 100) / (users)) / 100);
  return;
}

void dijkstras(int users, int start, double ** graph_grid, int alpha, int id_ind, int * id_list, int before_id)
{
  int ind;
  int count = 0;
  
  for (ind = 0; ind < users; ind++)
  {
    if (start + (int)(graph_grid[id_ind][ind]) < alpha && graph_grid[id_ind][ind] != -1 && id_ind != before_id ) {
      id_list[ind] = 1;
      count++;
      dijkstras(users,start + (int)(graph_grid[id_ind][ind]),graph_grid,alpha,ind,id_list,id_ind);
    }
  }
  if (count == 0)
    return;
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