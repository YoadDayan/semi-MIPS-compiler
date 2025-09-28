#include "header.h"

extern keyed_int_node *error_list;

extern instruction instruction_table[16];

/*
	The function handles symbol's errors, if found an error it is added to the errors_list.
*/
int symbol_error_handler(char *symbol_key, int line_counter, typed_keyed_node *symbol_list){
	char error[80] = "";
	int error_exist = 0;
	if(is_letter(symbol_key[0]) == 0){
		error_exist = 1;

		sprintf(error, "The symbol %s does not start with a letter.", symbol_key);
		add_keyed_int_node(line_counter, error, &error_list);
	}

	if(strlen(symbol_key) > 31){
		error_exist = 1;
		sprintf(error, "The symbol %s is longer then the maximum size of 31.", symbol_key);
		add_keyed_int_node(line_counter, error, &error_list);
	}

	if(is_saved_assembly_word(symbol_key) == 1){
		error_exist = 1;
		sprintf(error, "The word %s is an assembly key word and can not be used as a symbol. ", symbol_key);
		add_keyed_int_node(line_counter, error, &error_list);
	}

	if(key_exists_in_typed_keyed_list(symbol_key, symbol_list)){
		error_exist = 1;
		sprintf(error, "The symbol %s is already defined.", symbol_key);
		add_keyed_int_node(line_counter, error, &error_list);
	}

	return error_exist;

}

int instruction_compilation_error_handler_pass2(char *line, int index, char *word, int line_counter, typed_keyed_node *symbol_list){
	char error[60];
	int error_exist = 0;
	char current_word[31] = "";

	current_word[0] = '\0';
	get_next_word(line, current_word, &index);

	if(current_word[0] != '\0'){
		if(current_word[0] != '@' && is_number(current_word[0]) == 0){
			if(key_exists_in_typed_keyed_list(current_word, symbol_list) == 0){
				error_exist = 1;
				sprintf(error, "The symbol %s is not defined.", current_word);
				add_keyed_int_node(line_counter, error, &error_list);
			}
		}

	}


	current_word[0] = '\0';
	get_next_word(line, current_word, &index);

	if(current_word[0] != '\0'){ 
		if(current_word[0] != '@' && is_number(current_word[0]) == 0){
			if(key_exists_in_typed_keyed_list(current_word, symbol_list) == 0){
				error_exist = 1;
				sprintf(error, "The symbol %s is not defined.", current_word);
				add_keyed_int_node(line_counter, error, &error_list);
			}
		}
	}

	return error_exist;
}

/*
	The function handles instruction's errors, if found an error it is added to the errors_list.
*/
int instruction_compilation_error_handler(char *line, int index, char *word, int line_counter){
	char error[60];
	int error_exist = 0;
	int first_operand_type = 0;
	int second_operand_type = 0;
	char current_word[31] = "";
	int instruction_key;
	instruction current_instruction;

	strcpy(current_word, word);

	instruction_key = search_in_instruction_table(current_word);

	if(instruction_key == -1){
		error_exist = 1;
		sprintf(error, "the instruction %s does not exist.", current_word);
		add_keyed_int_node(line_counter, error, &error_list);
		return error_exist;
	}

	current_instruction = instruction_table[instruction_key];

	current_word[0] = '\0';
	get_next_word(line, current_word, &index);

	if(current_word[0] != '\0'){
		if(is_letter(current_word[0]) == 0 && current_word[0] != '@' && is_number(current_word[0]) == 0){
			error_exist = 1;
			add_keyed_int_node(line_counter, "Illegal operand.", &error_list);
		}
		if(current_word[0] == '@'){
			if(current_word[2] < '0' || current_word[2] > '7'){
				error_exist = 1;			
				add_keyed_int_node(line_counter, "Operand that is not a legal register cannot begin with @.", &error_list);
			}else{
				first_operand_type = 5;
			}
		}

		else if(is_number(current_word[0])){
			first_operand_type = 1;
		}
		
		else {
			first_operand_type = 3;
		}

	}


	current_word[0] = '\0';
	get_next_word(line, current_word, &index);

	if(current_word[0] != '\0'){ 
		if(current_word[0] == '@'){
			if(current_word[2] < '0' || current_word[2] > '7'){
				error_exist = 1;			
				add_keyed_int_node(line_counter, "Operand that is not a legal register cannot begin with @.", &error_list);
			}else{
				second_operand_type = 5;
			}
		}

		else if(is_number(current_word[0])){
			second_operand_type = 1;
		}
		
		else{
			second_operand_type = 3;
		}

	}


	if(current_instruction.group == 3){
		if(first_operand_type > 0 || second_operand_type > 0){
			error_exist = 1;
			add_keyed_int_node(line_counter, "too many operands.", &error_list);
		}
	}

	

	if(current_instruction.group == 2){
		if(first_operand_type == 0){
			error_exist = 1;
			add_keyed_int_node(line_counter, "missing operands.", &error_list);
		}
		
		else if(second_operand_type > 0){
			error_exist = 1;
			add_keyed_int_node(line_counter, "too many operands.", &error_list);
		}
	
	}

	

	if(first_operand_type > 0 && second_operand_type == 0){
		
		if(num_exist_in_array(current_instruction.destination_operand, first_operand_type, 3) == 0){
			error_exist = 1;
			add_keyed_int_node(line_counter, "the operand is incompatible with the instruction type", &error_list);
		}
	}

	if(first_operand_type > 0 && second_operand_type > 0){
		if(num_exist_in_array(current_instruction.source_operand, first_operand_type, 3) == 0){
			error_exist = 1;
			add_keyed_int_node(line_counter, "the operand is incompatible with the instruction type", &error_list);
		}
		if(num_exist_in_array(current_instruction.destination_operand, second_operand_type, 3) == 0){
			error_exist = 1;
			add_keyed_int_node(line_counter, "the operand is incompatible with the instruction type", &error_list);
		}
	}
	
	return error_exist;
}

/*
	The function handles data's errors, if found an error it is added to the errors_list.
*/
int data_compilation_error_handler(char *line, int index, int line_counter){
	int error_exist = 0;

	while(is_space(line[index])) index++;

	if(line[index] == ','){
		error_exist = 1;
		add_keyed_int_node(line_counter, "there are too many commas.", &error_list);
	}

	return error_exist;
}

/*
	The function handles string's errors, if found an error it is added to the errors_list.
*/
int string_compilation_error_handler(char *line, int index, int line_counter){
	int error_exist = 0;

	while(is_space_or_comma(line[index]) == 1) index++;

	

	if(line[index] != '"'){
		error_exist = 1;
		add_keyed_int_node(line_counter, "the string is missing \" at the start", &error_list);
	} else {
		index++;
	}

	

	while(index != strlen(line) && line[index] != '"'){
		index++;
	}

	if(index >= strlen(line) && line[index] != '"'){
		error_exist = 1;
		add_keyed_int_node(line_counter, "the string is missing \" at the end", &error_list);
	}

	return error_exist; 
}
