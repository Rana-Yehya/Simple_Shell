#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_WORD 10
#define MAX_CHAR 100

int READ (int* j ,char* arg[] , char command[] )

{
        //read the command and return it to separate commands to be working with
        read_command(command);
        separate(j,arg,command);
        return 1;
}

void read_command (char command[])

{
    //read command from user
    char* ret = fgets(command , MAX_CHAR , stdin);
    remove_end_of_command(command);
    if (strcmp(command,"exit") ==0 || ret == NULL)
    exit(0);

}

void remove_end_of_command(char command[])

{
    //remove the /n from the end of the command
    int i=0;
    while(command[i] != '\n')
    { i++;}
    command[i] = '\0';

}

int separate(int* j ,char* arg[],char command[])
{
    //separate the command into separate commands in an array arg[]
    int c;
    int i=0;
    arg[i]= strtok(command," ");

    if(arg[i] == NULL)
    {
        return 1;
    }

    while(arg[i] != NULL)
    {
        i++;
        arg[i]= strtok(NULL," ");
    }
    *j =i;
    return 1;
}

int main()
{
    //define the variables
    char* arg[MAX_WORD];
    char line[MAX_CHAR];
    char s[100];
    int* j;
    int c;
    FILE *k;
    k=fopen("simple shell.log","a+");

    // loop when input line is read
    while  (READ(j,arg ,line))
    {

    c = *j;
    //make a child
    int child_1=fork();
    k=fopen("simple shell.log","a+");
    fprintf(k,"The child 1 is executed\n");
    fclose(k);
    if(child_1 == 0) //in child
        {
            execvp(arg[0],arg);
            if (strcmp(arg[0],"cd") ==0 )
                    {
                        chdir(arg[c-1]);
                        printf("%s\n",getcwd(s,100));
                    }
            else
                        printf("command not found\n");

        }
    else
        {
            //when there is "&" in the end of the command , make a new child

            if(strcmp(arg[c-1],"&") ==0 )
            {
                while  (READ(j,arg ,line))
                    {

                        int child_2=fork();
                        k=fopen("simple shell.log","a+");
                        fprintf(k,"The child 2 is executed\n");
                        fclose(k);
                        if(child_2 == 0)
                            {
                            execvp(arg[0],arg);
                            if (strcmp(arg[0],"cd") ==0 )
                                    {chdir(arg[c-1]);
                                    printf("%s\n",getcwd(s,100));}
                            else
                                    printf("command not found\n");
                            }
                        else
                                  {  waitpid(child_2 ,0);
                                    k=fopen("simple shell.log","a+");
                                    fprintf(k,"The child 2 is terminated\n");
                                    fclose(k);   }
                    }
            }

            else
            { waitpid(child_1 ,0); }

        waitpid(child_1 ,0);
        k=fopen("simple shell.log","a+");
        fprintf(k,"The child 1 is terminated\n");
        fclose(k);

    }
    }
    return 0;
}
