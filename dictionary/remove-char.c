#include <stdio.h>
#include <ctype.h>
#include <string.h>

void clean_string(const char *input, char *output);

int main() {
    FILE *input_file = fopen("dictionary_long.txt", "r");
    FILE *output_file = fopen("dictionary_long_tolower.txt", "w");

    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }

    char line[256];
    char cleaned_line[256];
    while (fgets(line, sizeof(line), input_file)) {
        clean_string(line, cleaned_line);
        if (strlen(cleaned_line) <= 16) {
            fprintf(output_file, "%s\n", cleaned_line);
        }
    }

    fclose(input_file);
    fclose(output_file);

    printf("Special characters removed, all letters converted to lowercase, and words longer than 16 characters skipped. Check 'dictionary_long_tolower.txt' for the output.\n");
    return 0;
}

void clean_string(const char *input, char *output) {
    while (*input) {
        if (isalpha((unsigned char)*input)) {
            *output = tolower((unsigned char)*input);
            output++;
        }
        input++;
    }
    *output = '\0'; // Null-terminate the string
}
