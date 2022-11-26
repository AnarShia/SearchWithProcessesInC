#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


// Write result to file
void writeToFile(char *fileName, char *result) {
    FILE *fp = fopen(fileName, "a");
    if (fp == NULL) {
               printf(" Reading Error opening file! %s\n",fileName);

        exit(1);
    }
    fprintf(fp, "%s", result);
    fclose(fp);
}
// read files
char *readFile(char *fileName,int a) {
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf(" Reading Error opening file! %s\n",fileName);

        exit(1);
    }

    char *result = (char*)malloc(8*100);
    char *line = (char*)malloc(100);
    while (fgets(line, 100, fp) != NULL) {
        strcat(result,line);
    }
    fclose(fp);
    return result;
}

int search(char *word, char *filename,int outputNumber,int pipe [] )
{
    FILE *fp;
    char line[200];
    int found = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Cannot open file %s\n", filename);
        exit(1);
    }
    int lineNum = 0;
    char * resultFinal = (char*)malloc(8000);
    while (fgets(line, 200, fp) != NULL)
    {
    
       char* temp = strdup(line);
       char* tempfile = strdup(filename);
        char *token = strtok(line, " ");
        
        while (token != NULL)
        {   
            if (strcmp(token, word) == 0)
            {
                found++;
                char* outputfile;
                outputfile = (char*)malloc(100);
                sprintf(outputfile, "output%d.txt", outputNumber);
                char* result;
                result = (char*)malloc(8000);
                sprintf(result, "%s,%d,%s", tempfile,lineNum,temp);
                strcat(resultFinal,result);
                
            }
            token = strtok(NULL, " ");
        }
        lineNum++;
    }
    close(pipe[0]);
    write(pipe[1],resultFinal,strlen(resultFinal));
    close(pipe[1]);
    fclose(fp);
    return found;
}

int main(int argc, char *argv[])
{   
    clock_t start = clock();
    int PROCCESSNUMBER = atoi(argv[1]);
    int pids[PROCCESSNUMBER];
    int i;
    int pipes[PROCCESSNUMBER][2];
    for (i = 0; i < PROCCESSNUMBER; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < PROCCESSNUMBER; i++)
    {
        pids[i] = fork();
        if(pids[i]==-1){
            printf("fork error");
            return 2;
        }
        if(pids[i]==0){
            
            char *filename = (char*)malloc(100);
            sprintf(filename,"%s%d.txt", argv[3],i);
            search(argv[2],filename,i,pipes[i]);
            
            return 0;
        }

    }
    if(pids[0]>0){
        wait(NULL);
    }
    if(pids[0]>0){
        int status;
        waitpid(pids[0],&status,0);
        waitpid(pids[1],&status,0);
        waitpid(pids[2],&status,0);
        char *result = (char*)malloc(16000);
        for (i = 0; i < PROCCESSNUMBER; i++)
        {
            
            char *filename = (char*)malloc(100);
            sprintf(filename, "%s%d.txt", "output",i);
            close(pipes[i][1]);
            char *line = (char*)malloc(8000);
            read(pipes[i][0],line,8000);
            strcat(result, line);
            close(pipes[i][0]);
            printf("%s",line);
        }
        writeToFile((char*)"result.txt", result);
    }
  
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent: %f",time_spent);
    return 0;
}



