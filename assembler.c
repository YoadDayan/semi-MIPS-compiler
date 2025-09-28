#include <stdio.h>
#include <string.h>
#include "header.h"

keyed_int_node *error_list = NULL; /* all of the errors is gathered in this list */

/* an array contains of the instructions, each one's index, how many operand it takes and their type */
instruction instruction_table[16] =
	{
		{"mov", 0, 1, {1, 3, 5}, {0, 3, 5}},
		{"cmp", 1, 1, {1, 3, 5}, {1, 3, 5}},
		{"add", 2, 1, {1, 3, 5}, {0, 3, 5}},
		{"sub", 3, 1, {1, 3, 5}, {0, 3, 5}},
		{"not", 4, 2, {0, 0, 0}, {0, 3, 5}},
		{"clr", 5, 2, {0, 0, 0}, {0, 3, 5}},
		{"lea", 6, 1, {0, 3, 0}, {0, 3, 5}},
		{"inc", 7, 2, {0, 0, 0}, {0, 3, 5}},
		{"dec", 8, 2, {0, 0, 0}, {0, 3, 5}},
		{"jmp", 9, 2, {0, 0, 0}, {0, 3, 5}},
		{"bne", 10, 2, {0, 0, 0}, {0, 3, 5}},
		{"red", 11, 2, {0, 0, 0}, {0, 3, 5}},
		{"prn", 12, 2, {0, 0, 0}, {1, 3, 5}},	
		{"jsr", 13, 2, {0, 0, 0}, {0, 3, 5}},
		{"rts", 14, 3, {0, 0, 0}, {0, 0, 0}},
		{"stop", 15, 3, {0, 0, 0}, {0, 0, 0}},
	};

/* 
	This function gets an assembly language file and creates several file as an output:
	
	.ob file: The file contains all of the data declarations encoded in addition to all of the instruction and their operands encoded to base64),
	.ent file: An optional file that is created only if there is a symbol that is declared as an entry.
	.ext file: An optional file that is created only if there is an external declartion of a symbol.

	if found any error in the first pass of the assembly it does not create any file and simply returns.
 */
void assembler(char *file_name){

	FILE *extern_file = NULL;
	FILE *entry_file = NULL;

	keyed_int_node *extern_list = NULL;

	node *instructions_list = NULL; /* here the encoded first line of any instruction-line is stored */
	node *data_list = NULL; /* here the encoded data is stored (after the first pass all of the data is fully encoded to binary) */

	typed_keyed_node *symbol_list = NULL; /* contains all of the symbols in the file including entry/extern */

	typed_keyed_node *current_symbol = NULL;
	keyed_int_node *extern_entry_pointer = NULL;
	
	int exist_entry = 0;
	int IC, DC;

	error_list = NULL; /* an initialization of the error list */

	/* an initialization of IC and DC */
	IC = 100;
	DC = 0;

	assembler_pass_1(file_name, &symbol_list, &instructions_list, &data_list, &IC, &DC);

	IC = 100;


	assembler_pass_2(&symbol_list, instructions_list, data_list, &extern_list, file_name, &IC);
	
	/* a check if any error was found */
	if(error_list != NULL){
		return;
	}	

	extern_entry_pointer = extern_list;
	
	/* checks if there were any extern declartions and creates the .ext file*/
	if(extern_entry_pointer != NULL){
		strcat(file_name, ".ext");
		extern_file = fopen(file_name, "w");
		file_name[strlen(file_name) - 4] = '\0'; /* deleting the .ext from the file_name */
	}

	/* filling the .ext file with the extern uses that were in the .am file*/
	while(extern_entry_pointer != NULL){
		fprintf(extern_file, "%s %d\n", extern_entry_pointer->key, extern_entry_pointer->data);
		extern_entry_pointer = extern_entry_pointer->next;
	}

	

	current_symbol = symbol_list;

	/* check if entry exist */
	while(current_symbol != NULL){
		if(current_symbol->type == 'y'){
			exist_entry = 1;
		}
		current_symbol = current_symbol->next;
	}

	/* creates .ent file */
	if(exist_entry == 1){
		strcat(file_name, ".ent");
		entry_file = fopen(file_name, "w");
		file_name[strlen(file_name) - 4] = '\0'; /* deleting the .ent from the file_name */
	}

	current_symbol = symbol_list;

	/* filling the .ent file with the extern uses that were in the .am file*/
	while(current_symbol != NULL){
		if(current_symbol->type == 'y'){
			fprintf(entry_file, "%s %d\n", current_symbol->key, current_symbol->data);
		}

		current_symbol = current_symbol->next;
	}

	

	printf("\nThe file %s compiled successfully!\n", file_name);
}


