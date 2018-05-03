#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char* RemoveFirstAppearance(const char* string, const char* word);
bool FindWord(const char * string, const char * word, int i);
char * CopyEndOfString(char * new_string, const char * string, int i_dest, int i_src);

int main() {
    //Strings to check
    printf("1.%s\n2.%s\n3.%s\n4.%s\n5.%s\n6.%s\n7.%s\n8.%s\n9.%s\n10.%s\n"
                   "11.%s\n12.%s\n13.%s\n14.%s\n",
           RemoveFirstAppearance("1c2caca3catc4catac", "cat"),
           RemoveFirstAppearance("abcdcaccabbabc", "abc"),
           RemoveFirstAppearance("cat", "cat"),
           RemoveFirstAppearance("ca", "cat"),
           RemoveFirstAppearance("caccabbcatdcadcatb", "catb"),
           RemoveFirstAppearance("abcdcaccabbcatdcadcatb", NULL),
           RemoveFirstAppearance( NULL , "cat"),
           RemoveFirstAppearance(NULL,NULL),
           RemoveFirstAppearance("tcacabcbabbbcacaca", "cat"),
           RemoveFirstAppearance("cccabc", "abc"),
           RemoveFirstAppearance("CccAbC", "abc"),
           RemoveFirstAppearance("Abcabc","abc"),
           RemoveFirstAppearance("ggghhhi", ""),
           RemoveFirstAppearance("cabcde", "ABC"));
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