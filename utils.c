#include "header.h"

char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'
				};


extern instruction instruction_table[16];

/* 	
	The function gets:
	line (to read from), word(to read into)
	and an index the it changes to the next white space.
*/
void get_next_word(char *line, char *word, int *index){

	char current_char;
	int i = *index;
	int length = 0;
	
	while(is_space_or_comma(current_char = line[i]) == 1 && line[i] != '\n'){
		i++;
	}
	
	while(is_space_or_comma(current_char = line[i]) == 0 && line[i] != '\n' && i < strlen(line)){
		length++;
		strncat(word, &(line[i]), 1);
		i++;
	}

	word[length] = '\0';

	
	*index = i;

}

/* The function encodes an integer to binary string in the size of the length argument*/
char *dec_to_binary(int number, int length){
	char *binary;
	int i, sign = 1;

	if(number < 0){
		sign = -1;
	}

	binary = malloc((length + 1) * sizeof(char));
	for(i = 0; i < length + 1; i++){
		binary[i] = '0';
	}

	binary[length] = '\0';
	i = length-1;


	if(sign == -1){
		number *= -1;
	}
	while(number > 0){
		binary[i] = (number % 2) + '0';
		number = number / 2;
		i--;
	}

	if(sign == -1){
		for(i = 0; i < length; i++){
			binary[i] = (binary[i] == '0') ? '1' : '0';
		}
		binary[length-1] += 1;
	}

	return binary;
		
}

/* The function return 1 if string is a saved assembly word and 0 otherwise*/
int is_saved_assembly_word(char *string){

	if(search_in_instruction_table(string) != -1){
		return 1;
	}

	if(strcmp(string, "data") == 0 || strcmp(string, "string") == 0 || strcmp(string, "extern") == 0 || strcmp(string, "entry") == 0){
		return 1;
	}

	return 0;

}
/* The function returns the number inside the string as an int */
int string_to_int(char *string){
	int index = 0;
	int returned_value = 0;	
	
	while(string[index] != '\0'){
		returned_value = (10 * returned_value) + string[index] - '0';
		index++;
	}

	return returned_value;

}


/* The function encodes binary string in a length of 12 into a base64 string in a length of 2 */
char *binary_to_base64(char *binary){
	int first_half_in_int, second_half_in_int;
	char *result = malloc(3 * sizeof(char));
	char *first_half, *second_half;

	second_half = binary + 6;
	
	second_half_in_int = binary_to_int(second_half);

	
	
	first_half = binary;
	first_half[6] = '\0';
	first_half_in_int = binary_to_int(first_half);

	result[0] = encoding_table[first_half_in_int];
	result[1] = encoding_table[second_half_in_int];
	result[2] = '\0';

	return result;
}

/* The function decodes binary string into a natural number */
int binary_to_int(char *binary){
	int result = 0;
	int index = 0;
	int length = strlen(binary);

	while(index < length){
		if(binary[length-(index+1)] == '1'){
			result += pow(2, index);
		}
		index++;
	}

	return result;
}

/* The function searches the number in the arr and returns 1 if it is found and 0 otherwise */
int num_exist_in_array(int arr[], int num, int arr_length){
	int index = 0;
	int exist = 0;

	while(index < arr_length){
		if(arr[index] == num){
			exist = 1;
		}
		index++;
	}
		
	return exist;
}

/* The function searches the string in the instruction table and returns it index if found and -1 otherwise */
int search_in_instruction_table(char *string){
	int i = 0;

	for (i = 0; i < 16; ++i){
		if(strcmp(instruction_table[i].instruction_name, string) == 0){
			return i;
		}
	}
	
	return -1;
	
}

/* The function return 1 if the string is completely contains whitespace and 0 otherwise */
int is_string_empty(char *string){
	int index = 0;	
	
	while(is_space_or_newline(string[index])) index++;
	
	if(index == strlen(string)) return 1;

	return 0;
}

/* The function return 1 if the char is a space or a tab or a comma and 0 otherwise */
int is_space_or_comma(char c){
	if(c == ' ' || c == '\t' || c == ','){
		return 1;
	}

	return 0;
}

/* The function return 1 if the char is a space or a tab or a newline or a comma and 0 otherwise */
int is_space_or_comma_or_newline(char c){
	if(c == ' ' || c == '\t' || c == '\n' || c == ','){
		return 1;
	}

	return 0;
}

/* The function return 1 if the char is a space or a tab or a newline and 0 otherwise */
int is_space_or_newline(char c){
	if(c == ' ' || c == '\t' || c == '\n'){
		return 1;
	}

	return 0;
}


/* The function return 1 if the char is a space or a tab and 0 otherwise */
int is_space(char c){
	if(c == ' ' || c == '\t'){
		return 1;
	}

	return 0;
}

/* The function return 1 if the char c is a number (including the '-' or '+' at the beginning) and 0 otherwise*/
int is_number(char c){

	if(('0' <= c && c <= '9') || c == '-' || c == '+'){
		return 1;
	}
	
	return 0;

}

/* The function return 1 if the char c is a letter and 0 otherwise*/
int is_letter(char c){

	if((c < 'A' || c > 'Z') && (c < 'a' || c > 'z')){
		return 0;
	}

	return 1;
}
