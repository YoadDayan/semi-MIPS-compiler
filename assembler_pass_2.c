#include "header.h"

extern instruction instruction_table[16];

extern keyed_int_node *error_list;

/* 	
	The function represents the second passof the assembler.
	It gets as parameters the lists which the assembler_pass_1 has created.

	The lists are: symbol_list(the symbol's table), instructions_list(the instruction of any instruction-line encoded to binary)
	and data_list(contains the data in the file encoded fully to binary).

	The function finishes all of the encoding that has not been done in the first pass and encodes them.
	To finish, the function prints the encoded binary compiled code in a base 64 format to a .ob file.
*/
void assembler_pass_2(typed_keyed_node **symbol_list, node *instructions_list, node *data_list, keyed_int_node **extern_list, char *file_name, int *IC){
	
	FILE *output = NULL;
	FILE *input = NULL;

	char line [81];
	int line_counter = 1;

	node *current_data_node = NULL;
	node *current_instruction = NULL;
	keyed_int_node *current_error = NULL;

	current_instruction = instructions_list;


	strcat(file_name, ".am");
	input = fopen(file_name, "r");
	file_name[strlen(file_name) - 3] = '\0';
	

	strcat(file_name, ".ob");
	output = fopen(file_name, "w");
	file_name[strlen(file_name) - 3] = '\0';
	
	
	/* loops over all of the lines in the .am file */
	while(fgets(line,sizeof line,input)!= NULL) {
      		read_line_pass_2(line, symbol_list, extern_list, &current_instruction, line_counter, IC, output);
		line_counter++;
    	}

	
	current_error = error_list;
	/* checks is eny errors exist and if so it prints them */	
	if(current_error != NULL){
		printf("\nCompilation errors in the file %s:\n\n", file_name);
		while(current_error != NULL){
			printf("Error in line %d: %s\n", current_error->data, current_error->key);
			current_error = current_error->next;
		}
		return;
	}

	/* prints to the output file the data list without changing it */
	current_data_node = data_list;
	while(current_data_node != NULL){
		fprintf(output, "%s\n", binary_to_base64(current_data_node->data));
		current_data_node = current_data_node->next;
	}

	
	fclose(output);
}

/*

	The function gets a line and compiles it.
	If the line is a directive-line the function ignores it. 

*/
void read_line_pass_2(char *original_line, typed_keyed_node **symbol_list, keyed_int_node **extern_list, node **current_instruction, int line_counter, int *IC, FILE *output){
	char *line;
	char word[80] = "";
	char error_message[80] = "";
	int index = 0;
	typed_keyed_node *current_symbol = NULL;

	line = original_line;

	if(is_string_empty(line) == 1) return;
	if(line[0] == ';') return;
	
	get_next_word(line, word, &index);

	/* The first field is a symbol */
	if(word[strlen(word) - 1] == ':'){
		word[0] = '\0';
		get_next_word(line, word, &index);
	}

	/* The line is a data or extern */
	if(strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0 || strcmp(word, ".extern") == 0){
		return;
	}

	/* The line is an entry decleration*/
	else if(strcmp(word, ".entry") == 0){
		word[0] = '\0';
		get_next_word(line, word, &index);
		if(key_exists_in_typed_keyed_list(word, *symbol_list) == 0){
			sprintf(error_message, "The symbol %s is not defined.", word);
			add_keyed_int_node(line_counter, error_message, &error_list);
		} else {
			current_symbol = search_typed_keyed_node(word, *symbol_list);
			current_symbol->type = 'y';
		}
	}

	else {
		if((*current_instruction) != NULL){
			*IC += compile_instruction_pass_2(line, index, word, IC, (*current_instruction)->data, symbol_list, extern_list, line_counter, output);
			*current_instruction = (*current_instruction)->next;
		}	
	}

}

/* 
	The function gets an instruction-line, compiles it to it's binary representation and prints it to the output file in a base64 format.
	Also the line returns the ammount of words the current instruction requires.
 */
int compile_instruction_pass_2(char *line, int index, char *word, int *IC, char *instruction_in_binary, typed_keyed_node **symbol_list, keyed_int_node **extern_list, int line_counter, FILE *output){
	int source_operand_type, destination_operand_type; 	
	int source_operand_value, destination_operand_value;	
	int instruction_key;
	int sign;
	char binary[12] = "";
	typed_keyed_node *current_symbol = NULL;

	destination_operand_type = source_operand_type = 0;
	destination_operand_value = source_operand_value = 0;

	if(instruction_compilation_error_handler_pass2(line, index, word, line_counter, *symbol_list) == 1){
		return 0;
	}
	

	instruction_key = search_in_instruction_table(word);

	
	
	
	fprintf(output, "%s\n", binary_to_base64(instruction_in_binary)); /* prints the first word of the compilation that was compiled in the first pass */
	
	/* The instruction does not have any operands */
	if(instruction_table[instruction_key].group == 3){
		return 1;
	}


	word[0] = '\0';
	get_next_word(line, word, &index);

	

	/* Determinating the first operand's type: 1/3/5 */
	if(word[0] == '@'){
		destination_operand_value = word[2] - '0';
		destination_operand_type = 5;
	}
	
	else if (is_number(word[0])){
		destination_operand_type = 1;
		if(word[0] == '-'){
			sign = -1;
			word++;
		}
		else if (line[index] == '+'){
			sign = 1;
			word++;
		}
		else {
			sign = 1;
		}

		destination_operand_value = string_to_int(word);
		
		destination_operand_value *= sign;
		sprintf(binary, "%s00", dec_to_binary(destination_operand_value, 10));
		fprintf(output, "%s\n", binary_to_base64(binary));
		
	} else {
		destination_operand_type = 3;
		current_symbol = search_typed_keyed_node(word, *symbol_list);
		destination_operand_value = current_symbol->data;
		sprintf(binary, "%s", dec_to_binary(destination_operand_value, 10));

		if(current_symbol->type == 'n'){
			strcat(binary, "01");
			fprintf(output, "%s\n", binary_to_base64(binary));
			add_keyed_int_node(*IC + 1, current_symbol->key, extern_list);
		} else {
			strcat(binary, "10");
			fprintf(output, "%s\n", binary_to_base64(binary));
		}		
	}

	

	word[0] = '\0';
	get_next_word(line, word, &index);

	/* There is 2 operands prints the decoded code to the output file*/
	if(word[0] != '\0'){
		source_operand_type = destination_operand_type;
		source_operand_value = destination_operand_value;

		if(word[0] == '@'){
			destination_operand_value = word[2] - '0';
			destination_operand_type = 5;
		}
	
		else if (is_number(word[0])){
			destination_operand_type = 1;
			if(word[0] == '-'){
				sign = -1;
				word++;
			}
			else if (line[index] == '+'){
				sign = 1;
				word++;
			}
			else {
				sign = 1;
			}

			destination_operand_value = string_to_int(word);
		
			destination_operand_value *= sign;

			if(source_operand_type == 5){
				binary[0] = '\0';
				sprintf(binary, "%s0000000", dec_to_binary(source_operand_value, 5));
				fprintf(output, "%s\n", binary_to_base64(binary));
			}

			sprintf(binary, "%s00", dec_to_binary(destination_operand_value, 10));
			fprintf(output, "%s\n", binary_to_base64(binary));
			return 3;
		
		} else {
			
			destination_operand_type = 3;
			current_symbol = search_typed_keyed_node(word, *symbol_list);

			destination_operand_value = current_symbol->data;
			
			if(source_operand_type == 5){
				sprintf(binary, "%s0000000", dec_to_binary(source_operand_value, 5));
				fprintf(output, "%s\n", binary_to_base64(binary));
			}
			sprintf(binary, "%s", dec_to_binary(destination_operand_value, 10));
			if(current_symbol->type == 'n'){
				strcat(binary, "01");
				fprintf(output, "%s\n", binary_to_base64(binary));
				add_keyed_int_node(*IC + 2, current_symbol->key, extern_list);
			} else {
				strcat(binary, "10");
				fprintf(output, "%s\n", binary_to_base64(binary));
			}
			return 3;	
		}
		
		
		if(destination_operand_type == 5 && source_operand_type == 5){
			sprintf(binary, "%s%s00", dec_to_binary(source_operand_value, 5), dec_to_binary(destination_operand_value, 5));
			fprintf(output, "%s\n", binary_to_base64(binary));
			return 2;
		}

	

	/* There is only a single operand prints the decoded code to the output file */
	} else {
		if(destination_operand_type == 5){
			sprintf(binary, "00000%s00", dec_to_binary(destination_operand_value, 5));
			fprintf(output, "%s\n", binary_to_base64(binary));
		}
		return 2;
	}

	

	return 0;

}
