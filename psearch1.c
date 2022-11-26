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
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(fp, "%s", result);
    fclose(fp);
}
// read files
char *readFile(char *fileName,int a) {
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    char *result = (char*)malloc(8000);
    char *line = (char*)malloc(8000);
    while (fgets(line, 100, fp) != NULL) {
        strcat(result,line);
    }
    fclose(fp);
    return result;
}

int search(char *word, char *filename,int outputNumber)
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
                printf("input file name %s, ", filename);
                printf("matched line number %d, ", lineNum);
                printf("line: %s", temp);
                char* outputfile;
                outputfile = (char*)malloc(100);
                sprintf(outputfile, "output%d.txt", outputNumber);
                char* result;
                result = (char*)malloc(8000);
                sprintf(result, "%s,%d,%s", tempfile,lineNum,temp);
                writeToFile(outputfile, result);
            
            }
            token = strtok(NULL, " ");
        }
        lineNum++;
    }

    fclose(fp);
    return found;
}

int main(int argc, char *argv[])
{   
    clock_t begin = clock();

    int PROCCESSNUMBER= atoi(argv[1]);
    int pids[PROCCESSNUMBER];
    int i;
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
            search(argv[2],filename,i);
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
            strcat(result, readFile(filename,1));
            
        }
        writeToFile((char*)"result.txt", result);
    }
  
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("time spent: %f",time_spent);

    return 0;
}



