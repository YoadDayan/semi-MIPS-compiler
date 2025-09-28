#include "header.h"


/* 
  The function takes as parameter an .as file in it there is the assembly code.
  Also creates an .am file that implements the macros in the declared in the .as file.
  If the file does not exist the pre_assember prints an error and returns -1
*/
int pre_assembler_main(char *file_name){

	FILE *input_file, *output_file;
	char line [81];
	char *current_mcro_key = NULL;
	keyed_node *mcro_list = NULL;
	int in_mcro = 0;

	strcat(file_name, ".as");
	
	input_file = fopen(file_name, "r");

	/*Checks if the file exists*/
	if(input_file == NULL){
		printf("Error: The file %s does not exist\n", file_name);
		return -1;
	}

	file_name[strlen(file_name) - 3] = '\0'; /* deletes .as ending */

	strcat(file_name, ".am");

	output_file = fopen(file_name, "w");

	/*file_name[strlen(file_name) - 3] = '\0';*/

	while(fgets(line,sizeof line,input_file)!= NULL) /* read a line from a file */ {
      		read_line_pre_assembler(line, &mcro_list, &current_mcro_key, &in_mcro, output_file); /* print the file contents on stdout. */
    	}

	fclose(input_file);
	fclose(output_file);
	
	return 0;

}

/*
  The function takes as parameters:
	char *: the current line
	keyed_node **: a pointer to the mcro's list
	char **: the key of the macro that is currently written
	int *: a flag determines wether we define a macro or not
	node **: the head of the linked list to which the implemeted lines are inserted
	FILE *: the file to which the implemeted lines are inserted
*/
void read_line_pre_assembler(char *original_line, keyed_node **mcro_list, char **current_mcro_key, int *in_mcro, FILE *output_file){
	char *line;
	char word[80] = "";
	int index = 0;
	keyed_node *current_mcro = NULL;
	node *current_mcro_commands_list = NULL;
	
	line = original_line;

	get_next_word(line, word, &index);

	
	
	current_mcro = search_key_in_list(word, *mcro_list);
	
	/* the line uses a macro */
	if(current_mcro != NULL){

		current_mcro_commands_list = current_mcro->data;

		while(current_mcro_commands_list != NULL){
			fprintf(output_file, "%s", current_mcro_commands_list->data);
			
			current_mcro_commands_list = current_mcro_commands_list->next;
		}
	

	}
	/* the line is end of macro */
	else if(strcmp(word, "endmcro") == 0){
		*in_mcro = 0;
	
		/*current_mcro = search_key_in_list(*current_mcro_key, *mcro_list);*/
	}

	/* the line is part of a macro */
	else if(*in_mcro == 1){
		current_mcro = search_key_in_list(*current_mcro_key, *mcro_list);
		add_node(line, &(current_mcro->data));
	}

	/* the line begins with macro */
	else if(strcmp(word, "mcro") == 0){
		*in_mcro = 1;
		word[0] = '\0';
		get_next_word(line, word, &index);

		add_keyed_node(NULL, word, mcro_list);

		*current_mcro_key = (char *) malloc(80);
		strcpy(*current_mcro_key, word);
	/* the line is a regular line */
	}else{
		fprintf(output_file, "%s", original_line);
	}
}
