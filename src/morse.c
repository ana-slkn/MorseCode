#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/decode.h" 
#include "../include/encode.h"
#include "../include/statistics.h"

char *read_long_line_from_stdin(char);
char *read_long_line_from_file(char, char *);
void view_or_delete_history(const char *);
void write_to_history(const char *, const char *, const char *);
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

    if (switchArg[1] == 'c' || switchArg[1] == 'd')
    {
        int encoding = switchArg[1] == 'c';
        const char *stdin_filename = encoding ? "encode.txt" : "decode.txt";
        const char *history_filename = encoding ? "eng2morse.txt" : "morse2eng.txt";
        int from_stdin = strcmp(filename, stdin_filename) == 0;
        char *str;

        if (from_stdin) {
            printf("Enter the %s text and press Enter:\n", encoding ? "English" : "Morse");
            str = read_long_line_from_stdin('-');
            if (str == NULL)
                return EXIT_FAILURE;

            FILE *snapshot = open_file(stdin_filename, "w");
            if (snapshot == NULL) {
                free(str);
                return EXIT_FAILURE;
            }
            int written = fputs(str, snapshot);
            int closed = fclose(snapshot);
            if (written == EOF || closed == EOF) {
                fprintf(stderr, "Could not write '%s'.\n", stdin_filename);
                free(str);
                return EXIT_FAILURE;
            }
        } else {
            str = read_long_line_from_file('-', filename);
            if (str == NULL)
                return EXIT_FAILURE;
            printf("\nThe content of the file is: %s\n", str);
        }

        statistics(str, !encoding);
        int converted;
        if (encoding) {
            printf("\nThe MorseCode text is: ");
            converted = encode(str);
        } else {
            node *root = NULL;
            create_tree(&root);
            converted = decode(str, root);
            drop_tree(root);
        }
        if (!converted) {
            free(str);
            return EXIT_FAILURE;
        }

        char *result = read_long_line_from_file('-', "output.txt");
        if (result == NULL) {
            free(str);
            return EXIT_FAILURE;
        }

        write_to_history(history_filename, str, result);
        if (!encoding)
            printf("The decoded text is: %s\n", result);
        statistics(result, encoding);

        free(result);
        free(str);
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
    size_t MAX_BUFFER_LENGTH = 1;
    FILE *file = fopen(filename, "r");
    if (file == NULL) 
    {
        perror("Error opening file");
        return NULL;
    }

    size_t i = 0;
    int c;
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

        str[i++] = (char)c;

        if (i == MAX_BUFFER_LENGTH) 
        {
            if (MAX_BUFFER_LENGTH == SIZE_MAX)
            {
                fprintf(stderr, "Input file is too large.\n");
                free(str);
                fclose(file);
                return NULL;
            }
            char *str2 = (char*) malloc((MAX_BUFFER_LENGTH + 1) * sizeof(char));
            if (str2 == NULL) {
                perror("Memory reallocation failed");
                fclose(file);
                free(str);
                return NULL;
            }
            memcpy(str2, str, i);
            free(str);
            str = str2;
            ++MAX_BUFFER_LENGTH;
        }
    }

    if (ferror(file)) {
        perror(filename);
        free(str);
        fclose(file);
        return NULL;
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

    int c;
    while ((c = fgetc(fptr)) != EOF)
        putchar(c);
    int read_failed = ferror(fptr);
    fclose(fptr);
    if (read_failed) {
        fprintf(stderr, "Could not read '%s'.\n", filename);
        return;
    }

    printf("If you want to delete the history, press 0.\n");
    printf("If you want to exit, press 1.\n");

    char choice[32];
    for (;;) {
        printf("Your choice: ");
        fflush(stdout);
        if (fgets(choice, sizeof choice, stdin) == NULL)
            return;
        if (strcmp(choice, "1\n") == 0 || strcmp(choice, "1") == 0)
            return;
        if (strcmp(choice, "0\n") == 0 || strcmp(choice, "0") == 0)
            break;
        if (strchr(choice, '\n') == NULL) {
            while ((c = getchar()) != '\n' && c != EOF) {
            }
        }
        printf("You can choose only between 0 and 1! Try again.\n");
    }

    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        perror(filename);
        return;
    }
    if (fclose(fptr) == EOF) {
        perror(filename);
        return;
    }
    printf("The history was successfully deleted! :3\n");
}

void write_to_history(const char *filename, const char *input, const char *result)
{
    FILE *fptr = fopen(filename, "a");
    if (fptr == NULL) {
        perror(filename);
        return;
    }

    int written = fprintf(fptr, "%s\n\n%s\n\n"
                          "_________________________________________________________________\n\n",
                          input, result);
    int closed = fclose(fptr);
    if (written < 0 || closed == EOF)
        fprintf(stderr, "Could not write '%s'.\n", filename);
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

