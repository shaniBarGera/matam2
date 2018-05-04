#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char* RemoveFirstAppearance(const char* string, const char* word);
bool FindWord(const char * string, const char * word, int i);
char * CopyEndOfString(char * new_string, const char * string, int i_dest, int i_src);

int main() {
    char * new_string = RemoveFirstAppearance("1c2caca3catc4catac", "cat"),
    printf("%s",new_string);
    free(new_string);
    return 0;
}

char* RemoveFirstAppearance(const char* string, const char* word){
    if(string == NULL || word == NULL) return NULL;

    //Define variables
    int string_len = strlen(string), word_len = strlen(word), new_len = 0;
    if( string_len < word_len) return NULL;

    //Define size for the new string
    unsigned int new_size =
            (unsigned int)string_len - (unsigned int)word_len + 1;

    char * new_string = malloc(new_size);
    if(new_string == NULL) return NULL;

    /*
     * Go through each character in the given string and check if the word is
     * there, then copy the character if the word isn't there
     * */
    for(int i = 0; i < new_size; i++) {
        if(FindWord(string, word, i))
            return CopyEndOfString(new_string, string, new_len, i + word_len);
        else{
            new_string[new_len] = string[i];
            new_len++;
        }
    }

    free(new_string);

    return NULL;

}

//Check if the word starts at a certain point in the string
bool FindWord(const char * string, const char * word, int i){
    int count = 0;
    for (int j = i; j < i + strlen(word); j++) {
        if (string[j] != word[count]) return false;
        count++;
    }

    return true;
}

char * CopyEndOfString(char * new_string,const char * string, int i_dest, int i_src){
    while(i_src < strlen(string)){
        new_string[i_dest] = string[i_src];
        i_dest++;
        i_src++;
    }
    new_string[i_dest] = '\0';
    return new_string;
}