#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct instruction_table{
	char *instruction_name;
	int key;
	int group;
	int source_operand[3];
	int destination_operand[3];
	
} instruction;

typedef struct int_node_item {
	int data;
	struct int_node_item *next;
} int_node;

typedef struct node_item {
	char *data;
	struct node_item *next;
} node;

typedef struct keyed_int_node_item {
	char *key;
	int data;
	struct keyed_int_node_item *next;
} keyed_int_node;

typedef struct keyed_node_item {
	char *key;
	node *data;
	struct keyed_node_item *next;
} keyed_node;

typedef struct typed_keyed_node_item {
	char type;	
	char *key;
	int data;
	struct typed_keyed_node_item *next;
} typed_keyed_node;



/* Pre assembler */

int pre_assembler_main(char *file_name);
void read_line_pre_assembler(char *original_line, keyed_node **mcro_list, char **current_mcro_key, int *in_mcro, FILE *output_file);

/* Assembler */

void assembler(char *file_name);

/* Assembler  pass 1 */

void assembler_pass_1(char *file_name, typed_keyed_node **symbol_list, node **instructions_list, node **data_list, int *IC, int *DC);

void read_line_pass_1(char *original_line, int *IC, int *DC, typed_keyed_node **symbol_list, node **instructions_list, node **data_list, int line_counter);

int compile_instruction_pass_1(char *line, int index, node **instructions_list, typed_keyed_node **symbol_list, char *instruction, int line_counter);
int compile_data_int_pass_1(char *line, int index, node **data_list, int line_counter);
int compile_data_string_pass_1(char *line, int index, node **data_list, int line_counter);

/* Assembler pass 2 */

void assembler_pass_2(typed_keyed_node **symbol_list, node *instructions_list, node *data_list, keyed_int_node **extern_list, char *file_name, int *IC);

void read_line_pass_2(char *original_line, typed_keyed_node **symbol_list, keyed_int_node **extern_list, node **current_instruction, int line_counter, int *IC, FILE *output);
int compile_instruction_pass_2(char *line, int index, char *word, int *IC, char *instruction_in_binary, typed_keyed_node **symbol_list, keyed_int_node **extern_list, int line_counter, FILE *output);
/* Linked list */

void add_int_node(int data, int_node **head);

void add_node(char *data, node **head);
void add_keyed_int_node(int data, char *key, keyed_int_node **head);

void add_keyed_node(node *data,char *key, keyed_node **head);
void add_typed_keyed_node(int data,char type, char *key, typed_keyed_node **head);

keyed_node *search_key_in_list(char *key, keyed_node *head);
int key_exists_in_typed_keyed_list(char *key, typed_keyed_node* head);
typed_keyed_node* search_typed_keyed_node(char *key, typed_keyed_node* head); 

/* Utils */

void get_next_word(char *line, char *word, int *index);

int search_in_instruction_table(char *instruction_name);

char *dec_to_binary(int number, int length);

char *binary_to_base64(char *binary);

int binary_to_int(char *binary);

int string_to_int(char *string);

int num_exist_in_array(int arr[], int num, int arr_length);

int is_saved_assembly_word(char *string);

void concat_strings(char *string1, char *string2);

int is_string_empty(char *string); 

int is_space(char c);
int is_space_or_comma(char c);
int is_space_or_newline(char c);
int is_space_or_comma_or_newline(char c);
int is_number(char c);

int is_letter(char c);

/* Errors */

int symbol_error_handler(char *symbol_key, int line_counter, typed_keyed_node *symbol_list);
int instruction_compilation_error_handler_pass2(char *line, int index, char *word, int line_counter, typed_keyed_node *symbol_list);
int instruction_compilation_error_handler(char *line, int index, char *word, int line_counter);
int data_compilation_error_handler(char *line, int index, int line_counter);
int string_compilation_error_handler(char *line, int index, int line_counter);

