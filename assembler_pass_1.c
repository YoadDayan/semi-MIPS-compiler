#include "header.h"

extern keyed_int_node *error_list;

extern instruction instruction_table[16];


/* 	
	The function represents the first pass over the am file.
	It creates several linked lists which the second pass uses to finish the task.

	The lists are: symbol_list(the symbol's table), instructions_list(the instruction of any instruction-line encoded to binary)
	and data_list(contains the data in the file encoded fully to binary).
	In addition, the function finds all of the errors in the code.
*/
void assembler_pass_1(char *file_name, typed_keyed_node **symbol_list, node **instructions_list, node **data_list, int *IC, int *DC){

	FILE *input_file = fopen(file_name, "r");

	char line [81];
	int line_counter = 1; /* counts the lines in the file*/

	keyed_int_node *current_error = NULL;
	typed_keyed_node *current_symbol = NULL;

	file_name[strlen(file_name) - 3] = '\0';

	/* loops over all of the lines in the .am file */
	while(fgets(line,sizeof line,input_file)!= NULL) {
      		read_line_pass_1(line, IC, DC, symbol_list, instructions_list, data_list, line_counter);
		line_counter++;
    	}

	current_error = error_list;

	/* If an errors has been detected the program stops */
	if(current_error != NULL){
		
		return;
	}
	
	current_symbol = *symbol_list;
	
	/* Adds to the symbols of type data, IC */
	while(current_symbol != NULL){
		if(current_symbol->type == 'd') current_symbol->data += *IC;
		current_symbol = current_symbol->next;
	}

	

}

/*

	The function gets a line and compiles it.
	if there is a symbol declartion it adds it to the symbol_list

*/
void read_line_pass_1(char *original_line, int *IC, int *DC, typed_keyed_node **symbol_list, node **instructions_list, node **data_list, int line_counter){
	
	char *line;
	char word[80] = "";
	int index = 0; /* index of the current char that is read from the line (used for get_next_word) */
	char current_symbol_key[80] = "";
	int in_symbol = 0; /* a falg tells wether we are in a symbol declaration*/
	
	line = original_line;

	if(is_string_empty(line) == 1) return;
	if(line[0] == ';') return;

	get_next_word(line, word, &index);
	
	/* The line is a symbol declaration*/
	if(word[strlen(word) - 1] == ':'){
		in_symbol = 1;
		strcpy(current_symbol_key, word);
		current_symbol_key[strlen(current_symbol_key) - 1] = '\0'; /* gets read of the : at the end */

		/* calls the symbol's errors handler */
		if(symbol_error_handler(current_symbol_key, line_counter, *symbol_list) == 1){
			return;
		}
	}else{
		index = 0;
	}
	
	/* Getting the next word */
	word[0] = '\0';
	get_next_word(line, word, &index);


	/* The line is for data storage */
	if(strcmp(word, ".string") == 0 || strcmp(word, ".data") == 0){
	
		/* adds the symbol to symbol_list */
		if(in_symbol == 1){
			add_typed_keyed_node(*DC, 'd', current_symbol_key, symbol_list);
		}

		if(strcmp(word, ".data") == 0){
			*DC += compile_data_int_pass_1(line, index, data_list, line_counter);
		}
		
		else if(strcmp(word, ".string") == 0){
			*DC += compile_data_string_pass_1(line, index, data_list, line_counter);
		}
		
	}
	
	/* The line defines/declares an extern symbol with type 'n' */
	else if(strcmp(word, ".extern") == 0 || strcmp(word, ".entry") == 0){

		if(strcmp(word, ".extern") == 0){

			word[0] = '\0';
			get_next_word(line, word, &index);
			add_typed_keyed_node(0, 'n', word, symbol_list);

		}
	
	}

	/* The line is an instruction */
	else {
		/* adds the symbol to symbol_list */
		if(in_symbol == 1){
			add_typed_keyed_node(*IC, 'i', current_symbol_key, symbol_list);
		}

		*IC += compile_instruction_pass_1(line, index, instructions_list, symbol_list, word, line_counter);

	}

}

/*
	the function gets an instruction-line and compiles only the instruction into a binary code.
	At the end it adds it to the instructions_list.
	The function also return the amount of words that the current instruction needs (it used to update the IC).
*/
int compile_instruction_pass_1(char *line, int index, node **instructions_list, typed_keyed_node **symbol_list, char *word, int line_counter){

	int source_operand_type, destination_operand_type;	
	int instruction_key;
	char binary[12] = "";

	destination_operand_type = source_operand_type = 0;

	instruction_key = search_in_instruction_table(word);

	/* Calls the instruction's errors handler */
	if(instruction_compilation_error_handler(line, index, word, line_counter) == 1){
		return 0;
	}
	
	/* The instruction does not have any operands */
	if(instruction_table[instruction_key].group == 3){
		strcat(binary, "000");
		strcat(binary, dec_to_binary(instruction_key, 4));
		strcat(binary, "00000");
		
		add_node(binary, instructions_list);
		
		return 1;
	}

	word[0] = '\0';
	get_next_word(line, word, &index);

	/* Determinating the first operand's type: 1/3/5 */
	if(word[0] == '@'){
		destination_operand_type = 5;
	} else if (is_number(word[0])){
		destination_operand_type = 1;
	} else {
		destination_operand_type = 3;
	}

	word[0] = '\0';
	get_next_word(line, word, &index);

	/* There is 2 operands */
	/* Determinating the second operand's type: 1/3/5 */
	if(word[0] != '\0'){
		source_operand_type = destination_operand_type; /* as we now know there is two operands */

		if(word[0] == '@'){
			destination_operand_type = 5;
		} else if (is_number(word[0])){
			destination_operand_type = 1;
		} else {
			destination_operand_type = 3;
		}

		/* adding the compiled instruction to instructions-list */
		strcat(binary, dec_to_binary(source_operand_type, 3));
		strcat(binary, dec_to_binary(instruction_key, 4));
		strcat(binary, dec_to_binary(destination_operand_type, 3));
		strcat(binary, "00");
		add_node(binary, instructions_list);

		if(source_operand_type == 5 && destination_operand_type == 5){
			return 2;
		}
		
		return 3;

	} else {
		
		/* adding the compiled instruction to instructions-list */
		strcat(binary, "000");
		strcat(binary, dec_to_binary(instruction_key, 4));
		strcat(binary, dec_to_binary(destination_operand_type, 3));
		strcat(binary, "00");
		add_node(binary, instructions_list);

		return 2;
	}

	return 0;

}

/*
	The function gets a data-int-line and compiles it into a binary and adds it to data_list.
	The function also return the amount of words that the current data type needs (it used to update the DC).
*/
int compile_data_int_pass_1(char *line, int index, node **data_list, int line_counter){
	
	int current_number, sign, word_counter;

	word_counter = 0;

	/* Calls the data's errors handler */
	if(data_compilation_error_handler(line, index, line_counter) == 1){
		return 0;
	}

	/* Loops over all of the numbers, encodes each one to binary and adds to data_list */
	while(line[index] != '\n'){
		current_number = 0;
		sign = 1;

		while(is_space_or_comma(line[index]) == 1){
			index++;
		}

		if(line[index] == '-'){
			sign = -1;
			index++;
		} else if(line[index] == '+'){
			index++;
		}

		while(is_number(line[index]) == 1){
			current_number = current_number*10 + (line[index] - '0');
			index++;
		}

		current_number *= sign;
		
		word_counter++;

		add_node(dec_to_binary(current_number, 12), data_list);

		while(is_space_or_comma(line[index]) == 1){
			index++;
		}

	}

	return word_counter;
}

/*
	the function gets a data-string-line and compiles it into a binary and adds it to data_list.
	The function also return the amount of words that the current data type needs (it used to update the DC).
*/
int compile_data_string_pass_1(char *line, int index, node **data_list, int line_counter){
	int word_counter = 0;
	char current_char = 0;

	while(is_space(line[index]) == 1) index++;

	/* Calls the data's errors handler */
	if(string_compilation_error_handler(line, index, line_counter) == 1){
		return 0;
	}

	index++;

	/* Loops over all of the charchters, encodes each one to binary and adds to data_list */
	while(line[index] != '"'){
		current_char = line[index];
		add_node(dec_to_binary(current_char, 12), data_list);
		word_counter++;
		index++;
	}

	/* adds the '\0' char at the end of the string */
	add_node("000000000000", data_list);
	word_counter++;
	

	return word_counter;
}
