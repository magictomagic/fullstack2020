#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include<string.h>
#define BUFFER_SIZE 1024
char buf[BUFFER_SIZE+1];
void exitOut(){
strcpy(buf, "12345678901234567890123456789012345678901234567890\
12345678901234567890123456789012345678901234567890\
12345678901234567890123456789012345678901234567890\
12345678901234567890123456789012345678901234567890");
printf("ssss%ld\n",strlen(buf)-1);

}

int main(){

    int a;
while(1){
    signal(SIGTSTP, exitOut);
}

return 0;
}
