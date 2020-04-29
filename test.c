#include <stdio.h>

#define NOTENUM 256
struct note
{
    long inuse;
    long size;
    char *content;
};

struct note_list
{
    long total;
    long inuse;
    struct note notes[0];
};

struct note_list *list;
void init_env(){
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	alarm(0x3C);
}
void init_notes()
{
    list = (struct note_list*)malloc(16 + NOTENUM * sizeof(struct note));
    list->total = NOTENUM;
    list->inuse = 0;
    for (int i = 0; i < NOTENUM; ++i)
    {
        list->notes[i].inuse = 0;
        list->notes[i].size = 0;
        list->notes[i].content = NULL;
    }
}

int read_len(char *dst,int num){
    int i;
    if(num<=0)
	    return 0;
    i = read(0,dst,num);
    *(dst+i-1) = 0;
    return i;
}

int read_number(){
    char num[32];
    read_len(num,32);
    return atoi(num);    
}
int menu()
{
    puts("== Chaitin Free Note ==");
    puts("1. List Note");
    puts("2. New Note");
    puts("3. Edit Note");
    puts("4. Delete Note");
    puts("5. Exit");
    puts("====================");
    printf("Your choice: ");
    return read_number();
}

void new_note()
{
    if (list->inuse >= list->total)
    {
        puts("Unable to create new note.");
        return;
    }
    for (int i = 0; i < list->total; ++i)
        if (list->notes[i].inuse == 0)
        {
            printf("Length of new note: ");
            int len = read_number();
            if (len <= 0)
            {
                puts("Invalid length!");
                return;
            }
            if (len > 4096) len = 4096;
            char *content = (char*)malloc(len);
            printf("Enter your note: ");
            read_len(content, len);
            list->notes[i].inuse = 1;
            list->notes[i].size = len;
            list->notes[i].content = content;
            list->inuse++;
            puts("Done.");
            return;
        }
}

void edit_note()
{
    printf("Note number: ");
    int n = read_number();
    if (n < 0 || n >= list->total || list->notes[n].inuse != 1)
    {
        puts("Invalid number!");
        return;
    }
    printf("Length of note: ");
    int len = read_number();
    if (len <= 0)
    {
        puts("Invalid length!");
        return;
    }
    if (len > 4096) len = 4096;
    if (len != list->notes[n].size)
    {
        //int bsize = len + (128 - (len % 128)) % 128;
        list->notes[n].content = (char*)realloc(list->notes[n].content, len);
        list->notes[n].size = len;
    }
    printf("Enter your note: ");
    read_len(list->notes[n].content, len);
    puts("Done.");
}

void delete_note()
{
    if (list->inuse > 0)
    {
        printf("Note number: ");
        int n = read_number();
        if (n < 0 || n >= list->total)
        {
            puts("Invalid number!");
            return;
        }
        list->inuse--;
        list->notes[n].inuse = 0;
        list->notes[n].size = 0;
        free(list->notes[n].content);
        puts("Done.");
    }
    else
    {
        puts("No notes yet.");
    }
}

void list_note()
{
    if (list->inuse > 0)
    {
        for (int i = 0; i < list->total; ++i)
            if (list->notes[i].inuse == 1)
            {
                printf("%d. %s\n", i, list->notes[i].content);
            }
    }
    else
    {
        puts("You need to create some new notes first.");
    }
}

void main(){
    init_env();
    init_notes();
    while(1){
    	switch(menu()){
	    case 1:
		list_note();
		break;
	    case 2:
		new_note();
		break;
	    case 3:
		edit_note();
		break;
	    case 4:
		delete_note();
		break;
	    case 5:
		puts("Bye");
		return 0;
	    default:
		puts("Invalid!");
		break;
	}
    
    }
    
}
