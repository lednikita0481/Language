#include "include/headers/File_work.h"
const char* USER_FILE_NAME = "./lang_code/code.txt";
const char* LANG_FOLDER = "./lang_code";
const int LANG_FOLDER_NAME_LEN = 10;

void File_Read(Text* text, const int argc, const char* argv[])
{
    FILE* file = NULL;
    if (argc == 1)
    {
        file = fopen(USER_FILE_NAME, "r");
        if (file == NULL)
        {
            printf("Impossible to open file with default file name %s\n", USER_FILE_NAME);
            exit(666);
        }
    }
    else
    {
        char name[LANG_FOLDER_NAME_LEN + 5 + strlen(argv[1])] = {};
        strcpy(name, LANG_FOLDER);
        strcat(name, "/");
        strcat(name, argv[1]);
        file = fopen(name, "r");
        if (file == NULL)
        {
            printf("Impossible to open file with given file name\n");
            exit(666);
        }
    }
    struct stat buff = {};
    fstat(fileno(file), &buff);
    int size = buff.st_size;

    text->buff = (char*) calloc(size, sizeof(char));

    text->symbols = fread(text->buff, sizeof(char), size, file);

    fclose(file);
}