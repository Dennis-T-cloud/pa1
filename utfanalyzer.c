#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//Functions - Milestone 1
/*int32_t is_ascii(char str[])
Takes a UTF-8 encoded string and returns if it is valid 
ASCII (e.g. all bytes are 127 or less). */
int32_t is_ascii(char str[]){
    for(int i = 0; str[i] !='\0'; i++) {
        if (str[i]<0 || str[i] > 127 ) {
            return false;
        } 
    }
    return true;
}

/* int32_t capitalize_ascii(char str[])
Takes a UTF-8 encoded string and changes it in-place so that any ASCII 
lowercase characters a-z are changed to their uppercase versions. 
Leaves all other characters unchanged. It returns the number of characters 
updated from lowercase to uppercase.*/
int32_t capitalize_ascii(char str[]) {
    int32_t count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if ((unsigned char)str[i] <= 127 && islower(str[i])) {
            str[i] = toupper(str[i]);
            count++;
        }
    }
    return count;
}

//Functions - Milestone 2
/*int32_t width_from_start_byte(char start_byte)
Given the start byte of a UTF-8 sequence, return how many bytes it indicates 
the sequence will take (start byte + continuation bytes).
Returns 1 for ASCII characters, and -1 if byte is not a valid start byte.*/
int32_t width_from_start_byte(char start_byte){
    char temp = start_byte;
    if ((temp & 0x80) == 0x00) {
        return 1;
    }
    else if ((temp & 0xE0) == 0xC0){ //0b110
        return 2;
    }
    else if ((temp & 0xF0) == 0xE0){ //0b1110
        return 3;
    }    
    else if ((temp & 0xF8) == 0xF0){ //0b11110
        return 4;
    }
    return -1;
}

/* int32_t utf8_strlen(char str[])
Takes a UTF-8 encoded string and returns the number of UTF-8 codepoints it represents.
Returns -1 if there are any errors encountered in processing the UTF-8 string.*/
int32_t utf8_strlen(char str[]){
    int count = 0;
    int index = 0;
    while( str[index] != '\0' ){
        int length = width_from_start_byte(str[index]);
        if (length != -1 ){    
            index += length;     
            count ++ ;        
        }
        else return -1;
    }
    return count;
}

/*int32_t codepoint_index_to_byte_index(char str[], int32_t cpi)
Given a UTF-8 encoded string, and a codepoint index, return the byte index in the string 
where the Unicode character at the given codepoint index starts.
Returns -1 if there are any errors encountered in processing the UTF-8 string. */
int32_t codepoint_index_to_byte_index(char str[], int32_t cpi){
    if (cpi > utf8_strlen(str)){
        return -1;
    }
    int count = 0;
    for (int i = 0; i< cpi; i++){
        count += width_from_start_byte(str[i]);
    }
    return count;
}

/*void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[])
    Takes a UTF-8 encoded string and start(inclusive) and end(exclusive) codepoint indices, 
        and writes the substring between those indices to result, with a null terminator. 
        Assumes that result has sufficient bytes of space available. (Hint: result will be 
        created beforehand with a given size and passed as input here. Can any of the above 
        functions be used to determine what the size of result should be?)
    If cpi_start is greater than cpi_end or either is negative, the function should have no effect.
*/


void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    int total_codepoints = utf8_strlen(str);
    // check and modif the cpi_end if it exceeds
    if (cpi_end > total_codepoints) {
        cpi_end = total_codepoints;
    }
    // Check invalid input ranges
    if (cpi_start < 0 || cpi_end < 0 || cpi_start >= cpi_end) {
        result[0] = '\0';
        return;
    }

    int byte_start = codepoint_index_to_byte_index(str, cpi_start);
    int current_cpi = cpi_start;
    int byte_index = byte_start;

    while (current_cpi < cpi_end && str[byte_index] != '\0') {
        int length = width_from_start_byte(str[byte_index]);
        if (length == -1) {
            result[0] = '\0'; //add end symbol
            return;
        }

        // Copy the current character to the result
        strncpy(&result[byte_index - byte_start], &str[byte_index], length);
        byte_index += length;
        current_cpi++;
    }

    result[byte_index - byte_start] = '\0';
}

//unctions - Milestone 3
/* int32_t codepoint_at(char str[], int32_t cpi)
Takes a UTF-8 encoded string and a codepoint index, and returns a decimal representing 
the codepoint at that index.
*/

int32_t codepoint_at(char str[], int32_t cpi) {
    int index = 0;
    int current_cpi = 0;
    
    while (str[index] != '\0') {
        int length = width_from_start_byte(str[index]);
        if (length == -1) {
            return -1; 
        }
        
        if (current_cpi == cpi) {
            int32_t codepoint = 0;
            if (length == 1) {
                codepoint = (unsigned char)str[index];
            } else if (length == 2) {
                codepoint = ((unsigned char)str[index] & 0x1F) << 6; //* 2^6 and so on
                codepoint |= ((unsigned char)str[index + 1] & 0x3F);
            } else if (length == 3) {
                codepoint = ((unsigned char)str[index] & 0x0F) << 12;
                codepoint |= ((unsigned char)str[index + 1] & 0x3F) << 6;
                codepoint |= ((unsigned char)str[index + 2] & 0x3F);
            } else if (length == 4) {
                codepoint = ((unsigned char)str[index] & 0x07) << 18;
                codepoint |= ((unsigned char)str[index + 1] & 0x3F) << 12;
                codepoint |= ((unsigned char)str[index + 2] & 0x3F) << 6;
                codepoint |= ((unsigned char)str[index + 3] & 0x3F);
            }
            return codepoint;
        }
        
        index += length;
        current_cpi++;
    }
    
    return -1; // Codepoint index out of range
}
/*char is_animal_emoji_at(char str[], int32_t cpi)
Takes a UTF-8 encoded string and an codepoint index, and returns if the code point 
at that index is an animal emoji.
For simplicity for this question, we will define that that the “animal emojii” are 
in two ranges: from 🐀 to 🐿️ and from 🦀 to 🦮. (Yes, this technically includes things 
like 🐽 which are only related to or part of an animal, and excludes a few things like 🙊, 😸, 
which are animal faces.). You may find the wikipedia page on Unicode emoji helpful here. */
char is_animal_emoji_at(char str[], int32_t cpi) {
    int32_t codepoint = codepoint_at(str, cpi);
    return (codepoint >= 0x1F400 && codepoint <= 0x1F43E) ||
           (codepoint >= 0x1F980 && codepoint <= 0x1F997) ||
           (codepoint >= 0x1F9A0 && codepoint <= 0x1F9A7);
}

// a help function, findout utf8 to codepoint 
int32_t utf8_to_codepoint(const char *str, int *length) {
    unsigned char byte = (unsigned char)str[0];
    int32_t codepoint = 0;
    *length = width_from_start_byte(str[0]);
    
    if (*length == 1) {
        codepoint = byte;
    } else if (*length == 2) {
        codepoint = (byte & 0x1F) << 6; //*2^6 and so on
        codepoint |= (str[1] & 0x3F);
    } else if (*length == 3) {
        codepoint = (byte & 0x0F) << 12;
        codepoint |= (str[1] & 0x3F) << 6;
        codepoint |= (str[2] & 0x3F);
    } else if (*length == 4) {
        codepoint = (byte & 0x07) << 18;
        codepoint |= (str[1] & 0x3F) << 12;
        codepoint |= (str[2] & 0x3F) << 6;
        codepoint |= (str[3] & 0x3F);
    }
    
    return codepoint;
}
/*PA1 Resubmission:
Takes a UTF-8 encoded string and a codepoint index. Calculates the codepoint 
at that index. Then, calculates the code point with value one higher (so e.g. 
for ”é“ U+00E9 that would be “ê” (U+00EA), and for “🐩” (U+1F429) that would be 
“🐪” (U+1F42A)). Saves the encoding of that code point in the result array starting
 at index 0.*/

//helper function for next char: 
void encode_codepoint_to_utf8(int32_t codepoint, char result[]) {
    if (codepoint <= 0x7F) {
        result[0] = codepoint;
        result[1] = '\0';
    } else if (codepoint <= 0x7FF) {
        result[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
        result[1] = 0x80 | (codepoint & 0x3F);
        result[2] = '\0';
    } else if (codepoint <= 0xFFFF) {
        result[0] = 0xE0 | ((codepoint >> 12) & 0x0F);
        result[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        result[2] = 0x80 | (codepoint & 0x3F);
        result[3] = '\0';
    } else if (codepoint <= 0x10FFFF) {
        result[0] = 0xF0 | ((codepoint >> 18) & 0x07);
        result[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        result[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        result[3] = 0x80 | (codepoint & 0x3F);
        result[4] = '\0';
    }
}
void next_utf8_char(char str[], int32_t cpi, char result[]) {
    int32_t codepoint = codepoint_at(str, cpi);
    if (codepoint == -1) {
        result[0] = '\0';
        return;
    }
    int32_t next_codepoint = codepoint + 1;
    //call the helper function
    encode_codepoint_to_utf8(next_codepoint, result);
}

int main() {
    char input[50];
    printf("Enter a UTF-8 encoded string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 
    
    // Check if the string is ASCII
    printf("\nValid ASCII:%s\n", is_ascii(input)? "true" : "false");

    // Check Capitalize ASCII characters
    char uppercased[50];
    strcpy(uppercased, input);
    int32_t changes = capitalize_ascii(uppercased);
    printf("Uppercased ASCII: \"%s\"\nCharacters updated: %d\n", uppercased, changes);

    //Check Length in bytes
    int byte_length = strlen(input);
    printf("Length in bytes: %d\n", byte_length);

   //Check Number of code points
    int codepoint_count = 0;
    int index = 0;
    while (input[index] != '\0') {
        int length;
        utf8_to_codepoint(&input[index], &length);
        index += length;
        codepoint_count++;
    }
    printf("Number of code points: %d\n", codepoint_count);

    //Check Bytes per code point
    index = 0;
    printf("Bytes per code point: ");
    while (input[index] != '\0') {
        int length;
        utf8_to_codepoint(&input[index], &length);
        printf("%d ", length);
        index += length;
    }
    printf("\n");


    //Cehck Substring of the first 6 code points
    char substring[50];
    utf8_substring(input, 0, 6, substring);
    printf("Substring of the first 6 code points: \"%s\"\n", substring);

    //Cehck Code points as decimal numbers
    printf("Code points as decimal numbers: ");
    index = 0;
    while (input[index] != '\0') {
        int length;
        int32_t codepoint = utf8_to_codepoint(&input[index], &length);
        printf("%d ", codepoint);
        index += length;
    }
    printf("\n");
    
    //Check Identify animal emojis
    int cpi = 0;
    index = 0;
    printf("Animal emojis: ");
    while (input[index] != '\0') {
        int length = width_from_start_byte(input[index]);
        if (is_animal_emoji_at(input, cpi)) {
            printf("%.*s ", length, &input[index]);
        }
        index += length;
        cpi++;
    }
    printf("\n");

    // Check next 3 UTF-8 character
    char next_char[10];
    next_utf8_char(input, 3, next_char);
    printf("Next Character of Codepoint at Index 3: %s\n", next_char);

    return 0;
    return 0;
}