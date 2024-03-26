#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int Collatz(int n)
{
    printf("%d\n",n);
    if(n==1){
        return 1;
    }
    if(n%2 == 1)
    {   
        return Collatz(n*3+1);
    }
    else if(n%2 == 0)
    {
        return Collatz(n/2);
    }

}
int main()
{
    int num=0;
    printf("Enter number:\n");
    scanf("%d",&num);
    
    pid_t pid = fork();
    if (pid == -1){
        perror("Error");
    }
    if (pid == 0) 
    {
        printf("Answer:\n");
        Collatz(num);
    }
    else if(pid > 0)
    {
        wait(NULL);
        printf("finish");
    }
    else 
    {
        return 1;
    } 
    return 0;

}