#include "header.h"

int main(int argc, char *argv[]){

	char current_file_name[50] = "";
	int index = 1; 


	while(index < argc){
		
		strcpy(current_file_name, argv[index]);

		if(pre_assembler_main(current_file_name) == 0){
			assembler(current_file_name);
		}
		

		current_file_name[0] = '\0';

		index++;

		
	}

	return 0;
}
