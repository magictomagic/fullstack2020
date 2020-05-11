#include<stdio.h>

int main(int argc, char *argv[]){
int signal;
	//while(1){
int i = 0;
	char* storeargv = argv[1];    	

		while(1){
		scanf("%d", &signal);
printf("your choice: %d", signal);
			switch(signal){
				case 1: 
				
					 printf("%s", storeargv);
				
					break;
				case 2: printf("2\n");
					break;
				case 3: printf("3\n");
					break;
				default: printf("test\n");	
					break;
			}


		}
	




	//}




return 0;

}
/*
    int signal;
    printf("*****************");
    printf("1: receive file\n");
    printf("2: send    file\n");
    printf("3: start   chart\n");
    printf("*****************");*/
