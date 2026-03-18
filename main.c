#include <stdio.h>   
#include <stdlib.h>  
#include <string.h> 

#define MAX_INPUT 1024  

int main(void) {
    char input[MAX_INPUT];  

    while (1) {  

        printf("mysh$ ");       
        fflush(stdout);    
         if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n");       
            break;          
         }

        input[strcspn(input, "\n")] = '\0'; 
        if (input[0] == '\0') continue;     
        printf("You typed: [%s]\n", input);
    }

    return 0;
}