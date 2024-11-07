#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include "l_10.h"
#include "tree10.c"

errorCode validationExpression(const char* expression){
    int a = 0, b = 0;
    bool flag = false;
    while (*expression != '\0'){
        if (*expression == '('){
            if (!flag){
                return OPT_ERROR_FORMAT;
            }
            a++;
        }else if(*expression == ')'){
            b++;
        }
        expression++;
        flag = true;
    }
    if (a != b){
        return OPT_ERROR_FORMAT;
    }

    return OPT_SUCCESS;
}

errorCode validationInput(int argc, char* argv[]){
    if (argc != 3) {return OPT_ERROR_ARGUMENTS;}
 
    const char *inpath = argv[1];
    char abs_inpath[MAX_PATH];
    const char *outpath = argv[2];
    char abs_outpath[MAX_PATH];

    if (realpath(inpath, abs_inpath) == NULL || realpath(outpath, abs_outpath) == NULL) {
        return OPT_ERROR_FILE;
    }

    if (strcmp(abs_inpath, abs_outpath) == 0){
        return OPT_ERROR_FILE_NAME;
    }
    return OPT_SUCCESS;

}

Node* parseExpression(char** expr) {
    if (**expr == '\0' || **expr == ')') return NULL;
	Node* root = create_node(**expr);
	if (!root) return NULL;
	(*expr)++;
	if (**expr == '(') {
		(*expr)++;
		while (**expr != '\0' && **expr != ')') {
			Node* child = parseExpression(expr);
			if (child) {
				if (add_child(root, child) != OPT_SUCCESS){
					free_tree(root);
					return NULL;
				}
			}else{
				free_tree(root);
				return NULL;
			}
			if (**expr == ','){
                (*expr)++;
            }
		}
		(*expr)++;
	}
	return root;
}

int main(int argc, char* argv[]){
    errorCode opt = validationInput(argc, argv);
    if (opt == OPT_ERROR_ARGUMENTS){
        fprintf(stderr, "Error: Invalid number of arguments\n");
        return OPT_ERROR_ARGUMENTS;
    }else if (opt == OPT_ERROR_FILE){
        fprintf(stderr, "Error: Error when using the function realpath\n");
        return OPT_ERROR_FILE;
    }else if (opt == OPT_ERROR_FILE_NAME){
        fprintf(stderr, "Error: The files must have different names\n");
        return OPT_ERROR_FILE_NAME;
    }

    FILE *input = fopen(argv[1], "r");
	if (input == NULL){
        fprintf(stderr, "Error: Error open input file\n");
		return OPT_ERROR_FILE;
	}

    FILE *output = fopen(argv[2], "w");
	if (output == NULL){
        fprintf(stderr, "Error: Error open input file\n");
		return OPT_ERROR_FILE;
	}

    int num = 0;
    char *line = NULL;
    size_t len = 0;
    while ((getline(&line, &len, input)) != -1){
        opt = validationExpression(line);
        if (opt != OPT_SUCCESS){
            fclose(input);
            fclose(output);
            free(line);
            fprintf(stderr, "Error: Invalid format\n");
            return OPT_ERROR_FORMAT;
        }
        char* exp = line;
        Node* tree = parseExpression(&exp);
        if (tree == NULL){
            fclose(input);
            fclose(output);
            free(line);
            fprintf(stderr, "Error: Memory allocation error\n");
            return OPT_ERROR_MEMORY;
        }
        fprintf(output, "tree %d:\n", ++num);
        print_tree(output, tree, 0);
        fprintf(output, "\n");
        free_tree(tree);

    }
    free(line);
    fclose(input);
    fclose(output);
}