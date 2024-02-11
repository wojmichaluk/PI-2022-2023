#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define TEST 1  // 1 - dla testowania,  0 - dla automatycznej oceny

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int if_found(char word [],int in_tab){
    int size=sizeof(keywords)/sizeof(keywords[0]);
    for (int i=0; i<size; i++){
        if (strcmp(word,keywords[i])==0) return 0;
    }
    for (int i=0; i<in_tab; i++){
        if (strcmp(word,tab[i])==0) return 0;
    }
    return 1;
}

int find_idents(FILE *stream)
{
    int i,counter=0,ind=0;
    char d,c=fgetc(stream);
    while (c!=EOF){
        if (c=='"') {
            c=fgetc(stream);
            while (c!='"'){
                if (c=='\\') fgetc(stream);
                c=fgetc(stream);
            }
        }
        else if (c=='\'') {
            c=fgetc(stream);
            while (c!='\''){
                if (c=='\\') fgetc(stream);
                c=fgetc(stream);
            }
        }
        else if (c=='/'){
            d=fgetc(stream);
            if (d=='/') while (c!='\r' && c!='\n') c=fgetc(stream);
            else if (d=='*'){
                while (!(c=='*' && d=='/')){
                    c=d;
                    d=fgetc(stream);
                }
            }
        }
        else if (c=='_' || isalpha(c)){
            i=0;
            while (c=='_' || isalnum(c)){
                tab[ind][i]=c;
                i++;
                c=fgetc(stream);
            }
            counter+=if_found(tab[ind],ind);
            ind++;
        }
        c=fgetc(stream);
    }
    return counter;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void) {
	char file_name[40];
	FILE *stream;

    if(TEST)  stream = stdin;
    else {
        scanf("%s",file_name);
        stream = fopen(file_name,"r");
        if(stream == NULL) {
            printf("fopen failed\n");
            return -1;
        }
    }
    printf("%d\n", find_idents(stream));
	return 0;
}
