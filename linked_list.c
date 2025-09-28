#include "header.h"

/* The function adds a new node to the end of the list */
void add_typed_keyed_node(int data,char type, char *key, typed_keyed_node **head){
	
	/*create a link*/
	typed_keyed_node *lk = (typed_keyed_node*) malloc(sizeof(typed_keyed_node));
	typed_keyed_node *linkedlist = *head;


	lk->data = data;

	lk->key = (char *) malloc(80);
	strcpy(lk->key, key);
	lk->type = type;

	if(linkedlist == NULL){
		
		lk->next = NULL;
		*head = lk;
		
	} else {
		/* point it to old first node */
		while(linkedlist->next != NULL)
		linkedlist = linkedlist->next;

		/* point first to new first node */
		linkedlist->next = lk;
	} 
	
}

/* The function returns 1 if one of the nodes'key is the same of the key and 0 otherwise  */
int key_exists_in_typed_keyed_list(char *key, typed_keyed_node* head){
	int exist = 0;
	typed_keyed_node *current_node =  head;

	while(current_node != NULL){
		if(strcmp(current_node->key, key) == 0){
			exist = 1;
		}
		current_node = current_node->next;
	}

	return exist;

}

/* The function searches the key in the list and returns the node that matches the key, and NULL if there is no node that matches the key */
typed_keyed_node* search_typed_keyed_node(char *key, typed_keyed_node* head){

	typed_keyed_node *temp = head;

   while(temp != NULL) {
      if (strcmp(key, temp->key) == 0) {
	return temp;
      }
      temp=temp->next;
   }
   return NULL;

}

/* The function adds a new node to the end of the list */
void add_keyed_node(node *data, char *key, keyed_node **head){

   /*create a link*/
   keyed_node *lk = (keyed_node*) malloc(sizeof(keyed_node));
   keyed_node *linkedlist = *head;


	lk->data = data;

	lk->key = (char *) malloc(80);
	strcpy(lk->key, key);

	

	if(linkedlist == NULL){
		
		lk->next = NULL;
		*head = lk;
		
	} else {
		/* point it to old first node */
		while(linkedlist->next != NULL)
		linkedlist = linkedlist->next;

		/* point first to new first node */
		linkedlist->next = lk;
	}
}

/* The function adds a new node to the end of the list */
void add_keyed_int_node(int data, char *key, keyed_int_node **head){
	
	/*create a link*/
	keyed_int_node *lk = (keyed_int_node*) malloc(sizeof(keyed_int_node));
	keyed_int_node *linkedlist = *head;


	lk->data = data;

	lk->key = (char *) malloc(80);
	strcpy(lk->key, key);



	if(linkedlist == NULL){
	
		lk->next = NULL;
		*head = lk;
	
	} else {
		/* point it to old first node */
		while(linkedlist->next != NULL)
		linkedlist = linkedlist->next;

		/* point first to new first node */
		linkedlist->next = lk;
	}

}

/* The function adds a new node to the end of the list */
void add_node(char *data, node **head){
	node *lk = (node*) malloc(sizeof(node));
	node *linked_list = *head;

	lk->data = (char *) malloc(80);
	strcpy(lk->data, data);
	
	if(linked_list == NULL){
		lk->next = NULL;
		*head = lk;
	} else {
		/* point it to old first node */
		while(linked_list->next != NULL){
			linked_list = linked_list->next;
		}

		/* point first to new first node */
		lk->next = NULL;
		linked_list->next = lk;
	}

	
}

/* The function adds a new node to the end of the list */
void add_int_node(int data, int_node **head){
	int_node *new_node = (int_node*) malloc(sizeof(int_node));
	int_node *linked_list = *head;

	new_node->data = data;

	if(linked_list == NULL){
		new_node->next = NULL;
		*head = new_node;
	} else {
		/* point it to the last node */
		while(linked_list->next != NULL)
		linked_list = linked_list->next;

		/* point the last node to the new node */
		linked_list->next = new_node;
	}

}


/* The function searches the key in the list and returns the node that matches the key, and NULL if there is no node that matches the key */
keyed_node *search_key_in_list(char *key, keyed_node *head){
	keyed_node *temp = head;

	while(temp != NULL) {
		if (strcmp(key, temp->key) == 0) {
		return temp;
	}
		temp=temp->next;
	}
	return NULL;
}


