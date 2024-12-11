#include <stdio.h>
#include "l_9.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "tree.c"


int main(int argc, char* argv[]){

    if (argc < 3){
        fprintf(stderr, "Error: Invalid number of arguments\n");
        return OPT_ERROR_ARGUMENTS;
    }

    const char* filename = argv[1];
    int start = 2; 

    char* separators = collectSeparators(argc, argv, start);
    if (!separators){
        fprintf(stderr, "Error: Memory allocation error\n");
        return OPT_ERROR_MEMORY;
    }
    Node *root = NULL;
    errorCode opt = readFile(filename, separators, &root);

    if(opt == OPT_ERROR_MEMORY){
        fprintf(stderr,"Error: Memory allocation error\n");
        return OPT_ERROR_MEMORY;
    }else if(opt == OPT_ERROR_OPEN_FILE){
        fprintf(stderr, "Error: Error open input file\n");
        return OPT_ERROR_OPEN_FILE;
    }

    while(true){
        printMainMenu();
        printf("The selected option >>> ");
        char command[256];
        char option;
        char command_r[256];
        char option_r;
        scanf(" %255[^\n]", command);
        sscanf(command, "%c", &option);
        if (option == 'r'){
            printf("Enter the word >> ");
            char* word = readString(stdin);
            if (word == NULL){
                fprintf(stderr, "Error: Memory allocation error\n");
                break;
            }

            int found = countingWords(root, word);
            printf("%s: %d\n", word, found);
            free(word);
            continue;
        }else if(option == 'n'){
            unsigned int n;
            printf("Enter n >> ");
            char* in = readString(stdin);
            if (in == NULL){
                fprintf(stderr, "Error: Memory allocation error\n");
                break;
            }
            if(!idValidation(in)){
                printf("Error: Invalid format\n");
                free(in);
                continue;
            }
            char *end;
            n = strtoul(in, &end, 10);
            if (*end != '\0'){
                printf("Error: Invalid format\n");
                free(in);
                break;
            }  

            Node** sort_words = (Node**)malloc(20 * sizeof(Node*));  
            int size = 0;
            int capacity = 20;

            get_all_words(root, &sort_words, &size, &capacity);
            if (n >= size){
                printf("Error: n there should be less than the number of all words\n");
                continue;
            }
            qsort(sort_words, size, sizeof(Node*), compare_count);
            printf("Top %d most frequent words:\n", n);
            int count = 0;
            for (int i = 0; i < n; i++) {
                printf("%d) %s: %d\n", ++count, sort_words[i]->word, sort_words[i]->count);
            }
            free(in);
            free(sort_words);
            continue;

        }else if(option == 'f'){

            printf("The longest word: %s\n", findLongestWord(root));
            printf("The shortest word: %s\n", findShortestWord(root));
            continue;

        }else if(option == 'd'){

            printf("Depth of the tree: %d\n", tree_depth(root));
            continue;

        }else if(option == 's'){
            FILE* output = fopen("output.txt", "w");
            if(!output){
                fprintf(stderr, "Error: Error open input file\n");
                free(separators);
                free_tree(root);
                return OPT_ERROR_OPEN_FILE;
            }
            save_tree(root, output);
            fclose(output);
            printf("The tree has been successfully saved to a file\n");
            continue;

        }else if(option == 'l'){
            FILE* output = fopen("output.txt", "r");
            if(!output){
                fprintf(stderr, "Error: Error open input file\n");
                free(separators);
                free_tree(root);
                return OPT_ERROR_OPEN_FILE;
            }

            free_tree(root);
            root = load_tree(output);
            if(!root){
                fprintf(stderr, "Error: Error open input file\n");
                free(separators);
                free_tree(root);
                fclose(output);
                return OPT_ERROR_OPEN_FILE;
            }
            fclose(output);
            printf("The tree was successfully loaded from the file\n");
            continue;

        }else if(option == 'p'){
            printf("\n");
            print_tree(root, 0);
            continue;
        }else if(option == 'e'){
            printf("Exiting is completed\n");
            break;
        }else{
            printf("Error: Invalid option\n");
            continue;
        }
        
    }
    free(separators);
    free_tree(root);
    return OPT_SUCCESS;
}