#include <stdio.h>
#include "l_9.h"
#include <string.h>
#include <stdlib.h>

Node* createNode(const char* word) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->word = strdup(word); 
    if (!node->word) {
        return NULL;
    }
    node->count = 1;
    node->length = strlen(word); 
    node->left = node->right = NULL;
    return node;
}

Node* insert(Node *root, const char *word) {
    if (!root) return createNode(word); 
    int cmp = strcmp(word, root->word);
    if (cmp == 0) {
        root->count++; 
    } else if (cmp < 0) {
        root->left = insert(root->left, word); 
    } else {
        root->right = insert(root->right, word); 
    }
    return root;
}

void print_tree(Node* node, int depth) {
	if (node == NULL) return;

	print_tree(node->right, depth + 1);
	for (int i = 0; i < depth; i++) {
		printf("\t");
	}
	printf("%s (%d)\n", node->word, node->count);
	print_tree(node->left, depth + 1);
}

int tree_depth(Node* root) {
    if (root == NULL) {
        return 0;
    }
    int left_depth = tree_depth(root->left);
    int right_depth = tree_depth(root->right);
    return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

void free_tree(Node* root) {
    if (root == NULL) {
        return; 
    }

    free_tree(root->left);
    free_tree(root->right);

    free(root->word);
    free(root);
}

void save_tree(Node* root, FILE* file) {
    if (root == NULL) {
        fprintf(file, "NULL\n");
        return;
    }
    fprintf(file, "%s %d %d\n", root->word, root->count, root->length);
    save_tree(root->left, file);
    save_tree(root->right, file);
}

void printMainMenu(){
    printf("\nSelect the option:\n");
    printf("<r> - how many times is a given word repeated in a file\n");
    printf("<n> - print the n most frequently repeated words in the file\n");
    printf("<f> - find and print the longest and shortest word\n");
    printf("<d> - depth of the tree\n");
    printf("<s> - save the tree to a file\n");
    printf("<l> - load a tree from a file\n");
    printf("<p> - print a tree\n");
    printf("<e> - exit\n");
}

char* readString(FILE *file) {
	char c = fgetc(file);
	while (isspace(c)) {
		c = fgetc(file);
	}
	ungetc(c, file);
	char *str = NULL;
	int length = 0;
	int capacity = 20;
	str = (char *) malloc(capacity * sizeof(char));
	if (!str) {
		return NULL;
	}
	char ch;
	while (fscanf(file, "%c", &ch) == 1 && ch != ' ' && ch != '\n') {
		if (length + 1 >= capacity) {
			capacity *= 2;
			char *tmp = (char*)realloc(str, capacity * sizeof(char));
			if (!tmp) {
				free(str);
				return NULL;
			}
			str = tmp;
		}
		str[length++] = ch;
	}
	str[length] = '\0';

	return str;
}

int idValidation(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0; 
        }
    }
    return 1; 
}

void get_all_words(Node* root, Node*** words, int* size, int* capacity) {
    if (root == NULL) {
        return;
    }
    get_all_words(root->left, words, size, capacity);

    if (*size == *capacity) {
        *capacity *= 2;  
        *words = realloc(*words, (*capacity) * sizeof(Node*));
    }

    (*words)[*size] = root;
    (*size)++;

    get_all_words(root->right, words, size, capacity);
}

Node* load_tree(FILE *file) {
	char buffer[1024];
	if (fscanf(file, "%1023s", buffer) != 1 || strcmp(buffer, "NULL") == 0) return NULL;

	int count;
	fscanf(file, "%d", &count);
    int len;
	fscanf(file, "%d", &len);
	Node* node = createNode(buffer);
	if (!node) return NULL;

	node->count = count;
    node->length = len;
	node->left = load_tree(file);
	node->right = load_tree(file);
	return node;
}

char* collectSeparators(int argc, char *argv[], int start) {
    
    int total_len = 0;
    for (int i = start; i < argc; i++) {
        total_len += strlen(argv[i]);
    }

    char *delimiters = (char *)malloc(total_len + 1);
    if (!delimiters) {
        return NULL;
    }

    delimiters[0] = '\0'; 
    for (int i = start; i < argc; i++) {
        strcat(delimiters, argv[i]); 
    }

    return delimiters;
}

int isSeparator(char c, const char *delimiters) {
    return strchr(delimiters, c) != NULL; 
}

errorCode readFile(const char *filename, const char *delimiters, Node **root) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return OPT_ERROR_OPEN_FILE;
    }

    char* word = NULL; 
    size_t capacity = 0; 
    size_t size = 0;   
    char c;

    while ((c = fgetc(file)) != EOF) {
        if (isSeparator(c, delimiters)) {
            if (size > 0) {
                word[size] = '\0';         
                *root = insert(*root, word);
                if (*root == NULL){
                    return OPT_ERROR_MEMORY;
                }
                size = 0;                  
            }
        } else {
            if (size == capacity) { 
                capacity = (capacity == 0) ? 20 : capacity * 2;
                char* new_word = realloc(word, capacity);
                if (!new_word) {
                    free(word);
                    return OPT_ERROR_MEMORY;
                }
                word = new_word;
            }
            word[size++] = c; 
        }
    }

    if (size > 0) { 
        word[size] = '\0';
        *root = insert(*root, word);
        if (*root == NULL){
            return OPT_ERROR_MEMORY;
        }
    }

    free(word); 
    fclose(file);
    return OPT_SUCCESS;
}

int countingWords(Node* root, const char* word){
    if (!root) return 0;

    int cmp = strcmp(word, root->word);
    if (cmp == 0){
        return root->count;
    }
    else if (cmp < 0){
        return countingWords(root->left, word);
    }else{
        return countingWords(root->right, word);
    }
}

char* findLongestWord(Node* root) {
    if (root == NULL) {
        return NULL;
    }

    char* longest = root->word;
    int max_length = root->length;

    if (root->left != NULL) {
        char* left_longest = findLongestWord(root->left);
        int left_length = strlen(left_longest);
        if (left_length > max_length) {
            longest = left_longest;
            max_length = left_length;
        }
    }

    if (root->right != NULL) {
        char* right_longest = findLongestWord(root->right);
        int right_length = strlen(right_longest);
        if (right_length > max_length) {
            longest = right_longest;
            max_length = right_length;
        }
    }

    return longest;
}

char* findShortestWord(Node* root) {
    if (root == NULL) {
        return NULL;
    }

    char* shortest = root->word;
    int min_length = root->length;

    if (root->left != NULL) {
        char* left_shortest = findShortestWord(root->left);
        int left_length = strlen(left_shortest);
        if (left_length < min_length) {
            shortest = left_shortest;
            min_length = left_length;
        }
    }

    if (root->right != NULL) {
        char* right_shortest = findShortestWord(root->right);
        int right_length = strlen(right_shortest);
        if (right_length < min_length) {
            shortest = right_shortest;
            min_length = right_length;
        }
    }

    return shortest;
}

int compare_count(const void* a, const void* b) {
    Node* n_A = *(Node**)a;
    Node* n_B = *(Node**)b;
    return n_B->count - n_A->count;  
}

