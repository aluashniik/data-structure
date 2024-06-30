#include <stdio.h>
#include <string.h>

#define max_string_size 31
#define max_pattern_size 31

void fail(char *pat, int *failure);
void pmatch_all(char *string, char *pat);

int main() {
    char string[max_string_size];
    char pat[max_pattern_size];
    
    fgets(string, max_string_size, stdin);
    fgets(pat, max_pattern_size, stdin);

    string[strcspn(string, "\n")] = 0;
    pat[strcspn(pat, "\n")] = 0;

    pmatch_all(string, pat);

    return 0;
}


void pmatch_all(char *string, char *pat) {
    int failure[max_pattern_size];
    int lens = strlen(string);
    int lenp = strlen(pat);

    fail(pat, failure); 

    int i = 0, j = 0;
    while (i < lens) {
        if (string[i] == pat[j]) {
            if (j == lenp - 1) { 
                printf("%d\n", i - j);
                j = failure[j]; 
            }
            i++;
            j++;
        }
        else if (j > 0)
            j = failure[j - 1] + 1;
        else
            i++;
    }
}

void fail(char *pat, int *failure) {
    int len = strlen(pat);
    failure[0] = -1;
    for (int j = 1; j < len; j++) {
        int i = failure[j - 1];
        while ((pat[j] != pat[i + 1]) && (i >= 0))
            i = failure[i];
        if (pat[j] == pat[i + 1])
            failure[j] = i + 1;
        else
            failure[j] = -1;
    }
}

