#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h" 
#include "encode.h"
#include "statistics.h"

char *read_long_line_from_stdin(char);
char *read_long_line_from_file(char, char *);
void view_or_delete_history(const char *);
void write_to_history(char *, char *);
FILE* open_file(const char *, const char *);
int check_choice(int);


int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        printf("An error encountered! Follow the next instructions!\n");
        printf("Usage: %s <switch> <filename>\n", argv[0]);
        printf("----------Switches:\n");
        printf("-c for encoding\n");
        printf("-d for decoding\n");
        printf("-h for history\n");
        printf("----------Filenames:\n");
        printf("-> 'encode.txt' for encoding personalized input from stdin\n");
        printf("-> <filename> for encoding an already existing file\n");
        printf("-> 'decode.txt' for decoding personalized input from stdin\n");
        printf("-> <filename> for decoding an already existing file\n");
        printf("-> e2m for viewing Enlgish text to Morse code translation\n");
        printf("-> m2e for viewing Morse code to English text translation\n");
        return 1;
    }

    char *switchArg = argv[1]; //we assign to arguments variables, in case we would like in the features to increase/ decrease the number of arguments 
    char *filename = argv[2];

    if (strlen(switchArg) != 2 || (switchArg[0] != '-' || (switchArg[1] != 'c' && switchArg[1] != 'd' && switchArg[1] != 'h'))) 
    {
        printf("Invalid switch argument. Use -c for encoding, -d for decoding or -h for history.\n");
        return 1;
    }

    node* root = NULL;

    if (switchArg[1] == 'c') 
    {
        char *eng2morse = "eng2morse.txt";

        if (strcmp(filename, "encode.txt") == 0)
        {
            printf("-------------------- Welcome to EnglishText - MorseText converter! --------------------\n");
            printf("Enter the EnglishText you would like to encode. When you're done, press Enter, Ctrl+Z, Enter:\n");
            
            char *str = read_long_line_from_stdin(switchArg[0]);
            FILE *encodeptr = open_file("encode.txt", "w");
            fprintf(encodeptr, "%s", str);
            fclose(encodeptr);

            write_to_history(eng2morse, str);

            encodeptr = open_file("encode.txt", "r");
            fscanf(encodeptr, "%[^\n]", str);

            statistics(str);

            printf("\nThe MorseCode text is: ");
            encode(str);

            write_to_history(eng2morse, str);

            fclose(encodeptr);

            FILE *outputptr = open_file("output.txt", "r");
            fscanf(outputptr, "%[^\n]", str);

            write_to_history(eng2morse, str);

            statistics(str);

            fclose(outputptr);

            free(str);
        }
        else
        {

            char *str = read_long_line_from_file(switchArg[0], filename);
            FILE *fptr = open_file(filename, "r");
            fscanf(fptr, "%[^\n]", str);

            write_to_history(eng2morse, str);

            printf("\nThe content of the file is: %s\n", str);
            statistics(str);
            encode(str);

            write_to_history(eng2morse, str);
            fclose(fptr);
            FILE *outputptr = open_file("output.txt", "r");
            fscanf(outputptr, "%[^\n]", str);

            write_to_history(eng2morse, str);
            
            statistics(str);
            fclose(outputptr);
            free(str);
        }
    } 

    else if (switchArg[1] == 'd') 
    {
        char *morse2eng = "morse2eng.txt";
        if (strcmp(filename, "decode.txt") == 0)
        {
        printf("-------------------- Welcome to MorseText - EnglishText converter! --------------------\n");
        printf("Enter the MorseText you would like to decode. When you're done, press Enter, Ctrl+Z, Enter:\n");
        char *str = read_long_line_from_stdin(switchArg[0]);


        FILE *decodeptr = open_file("decode.txt", "w");
        fprintf(decodeptr, "%s", str);
        fclose(decodeptr);

        write_to_history(morse2eng, str);

        decodeptr = open_file("decode.txt", "r");

        fscanf(decodeptr, "%[^\n]", str);
        statistics(str);

        fclose(decodeptr);

        create_tree(&root);
        decode(str, root);
        FILE *outputptr = open_file("output.txt", "r");

        fscanf(outputptr, "%[^\n]", str);
        write_to_history(morse2eng, str);
        printf("The decoded text is: %s\n", str);
        statistics(str);
        
        drop_tree(root);
        fclose(outputptr);

        free(str);
        }
        else
        {
            char *str = read_long_line_from_file(switchArg[0], filename);
            FILE *fptr = open_file(filename, "r");
            fscanf(fptr, "%[^\n]", str);

            write_to_history(morse2eng, str);

            printf("The content of the file is: %s", str);
            statistics(str);

            create_tree(&root);
            decode(str, root);
            fclose(fptr);
            FILE *outputptr = open_file("output.txt", "r");

           fscanf(outputptr, "%[^\n]", str);
        write_to_history(morse2eng, str);
        printf("The decoded text is: %s\n", str);
        statistics(str);
        
        drop_tree(root);
        fclose(outputptr);

        free(str);
        }
    }   
    else if (switchArg[1] == 'h')
    {
        if (strcmp(filename, "e2m") == 0)
        {
            view_or_delete_history("eng2morse.txt");
        }
        if (strcmp(filename, "m2e") == 0)
        {
            view_or_delete_history("morse2eng.txt");
        }
    }
    return 0;
}

char *read_long_line_from_stdin(char mode) //this function reads continously text from standard input till EOF is encountered, dynamically allocating memory for the string (i.e. it doesn't have fixed size mem.)
{
    int i = 0;
    char c;
    int MAX_BUFFER_LENGTH = 1;
    char *str = (char*) malloc(MAX_BUFFER_LENGTH * sizeof(char));

    if (str == NULL) 
    {
    printf("Memory allocation failed.\n");
    return NULL;
    }
    while(scanf("%c", &c) != EOF)
        {
            if (c == '\n') {
            break;  // Break the loop when newline character is encountered
        }
            if (mode == 'c')
          {
            if ((c == '.' || c == '_') || (c >= '0' && c <= '9'))
                {
                    printf("Wrong format entered! Enter a valid English text.\n");
                    return NULL;
                }
            }
        else if (mode == 'd')
        {
            if ((toupper(c) >= 'A' && toupper(c) <= 'Z') || (c >= '0' && c <= '9'))
            {
                printf("Wrong format entered! Enter a valid Morse code.\n");
                return NULL;
            }
        }
            
        str[i] = c;
        i++;
        
        if(i == MAX_BUFFER_LENGTH)
        {
            MAX_BUFFER_LENGTH ++;
        }

        char *str2 = (char*) malloc(MAX_BUFFER_LENGTH * sizeof(char));
        if (str2 == NULL) {
            printf("Memory allocation failed.\n");
            free(str);
            return NULL;
        }

        for (int j = 0; j < i; j++)
        {
            str2[j] = str[j];
        }
        free(str);
        str = str2;
    }
    str[i] = '\0';
    return str;
}

char *read_long_line_from_file(char mode, char *filename) //this function is an analog of the last one, but for reading text from files
{
    int MAX_BUFFER_LENGTH = 1;
    FILE *file = fopen(filename, "r");
    if (file == NULL) 
    {
        perror("Error opening file");
        return NULL;
    }

    int i = 0;
    char c;
    char *str = (char *)malloc(MAX_BUFFER_LENGTH * sizeof(char));

    if (str == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    while ((c = fgetc(file)) != EOF) {
        if (mode == 'c') {
            if ((c == '.' || c == '_') || (c >= '0' && c <= '9')) {
                printf("Wrong format entered! Enter a valid English text.\n");
                fclose(file);
                free(str);
                return NULL;
            }
        } else if (mode == 'd') {
            if ((toupper(c) >= 'A' && toupper(c) <= 'Z') || (c >= '0' && c <= '9')) {
                printf("Wrong format entered! Enter a valid Morse code.\n");
                fclose(file);
                free(str);
                return NULL;
            }
        }

        str[i] = c;
        i++;

        if (i == MAX_BUFFER_LENGTH) 
        {
            MAX_BUFFER_LENGTH ++;
            char *str2 = (char*) malloc(MAX_BUFFER_LENGTH * sizeof(char));
            if (str2 == NULL) {
                perror("Memory reallocation failed");
                fclose(file);
                free(str);
                return NULL;
            }
            str = str2;
        }
    }

    str[i] = '\0';

    fclose(file);
    return str;
}

void view_or_delete_history(const char *filename) //this function is used when the user wants to see the translation history. It also has the menu whic asks the user whether he wants to delete the history or no
{
    long size;
    FILE *fptr = fopen(filename, "rb");

    if (fptr == NULL) {
        printf("Failed to open '%s' for reading.\n", filename);
        return;
    }
    
    // Determine the size of the file
    fseek(fptr, 0, SEEK_END);
    size = ftell(fptr);
    fclose(fptr);
    
    // Check if the file is empty
    if (size == 0) {
        printf("\nNo history registered. Do some translations, and come back! :3\n\n");
        return;
    }

    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("Failed to open '%s' for reading.\n", filename);
        return;
    }

    printf("Here is the history:\n");

    int choice;
    char c = fgetc(fptr);

    while (c != EOF) {
        printf("%c", c);
        c = fgetc(fptr);
    }

    printf("If you want to delete the history, press 0.\n");
    printf("If you want to exit, press 1.\n");
    printf("Your choice: ");
    
    scanf("%d", &choice);

    while (choice != 0 && choice != 1) {
        printf("You can choose only between 0 and 1! Try again: ");
        scanf("%d", &choice);
    }

    if (choice == 0) {
        fptr = fopen(filename, "w");

        if (fptr == NULL) {
            printf("Failed to open '%s' for writing.\n", filename);
            return;
        }

        fclose(fptr);
        printf("The history was successfully deleted! :3\n");
    }

    fclose(fptr);
}

void write_to_history(char *filename, char *str) // it is used during actual translation, helping to write the history of translations
{
      FILE *fptr = fopen(filename, "a");
        if (fptr == NULL)
        {
            {
            printf("Failed to open 'eng2morse.txt' for writing.\n");
            free(str);
            return;
            }
        }
        //fprintf(fptr, "\nThe original text:\n");

        fprintf(fptr, "%s", str);

        fprintf(fptr, "\n\n_________________________________________________________________\n\n");
        fclose(fptr);
}

FILE* open_file(const char *filename, const char *mode) 
//since this program is using lots of file handling, this function is designed to open the file in the desired mode, and to check whether the file was opened succesfully.
//it is used for shrinking in size the number of lines of code
{
    // Check if mode is valid
    if (strcmp(mode, "r") != 0 && strcmp(mode, "w") != 0 && strcmp(mode, "a") != 0) {
        printf("Invalid file mode: %s\n", mode);
        return NULL;
    }

    FILE *file = fopen(filename, mode);

    if (file == NULL) {
        perror("Failed to open file");
        return NULL;  // or handle the error in the calling code
    }

    return file;
}

