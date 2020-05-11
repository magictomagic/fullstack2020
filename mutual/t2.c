#include<stdio.h>


void rec(){
int signal;
int i = 0;
while(1){
		scanf("%d", &signal);
			switch(signal){
				case 1: printf("%d", i);
					break;
				case 2: printf("2\n");
					break;
				case 3: printf("3\n");
					break;
				default: printf("test\n");	
					break;
			}


		}

}


int main(int argc, char *argv[]){
	rec();

return 0;

}
