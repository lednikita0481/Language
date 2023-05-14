#include "include/File_work.h"

const char* PROG_FOLDER = "./programs/";
const char* STANDARD_FILE_NAME = "code.prog";
const int PROG_FOLDER_NAME_LEN = 12;
const int FILE_NAME_LEN = 100;

const int SIGNATURE_LEN = 8;
const char* NEEDED_SIGNATURE = "ASS.300";

void Get_Text(Text* text, int argc, char* argv[])
{
    char file_name[FILE_NAME_LEN] = {};
    strcpy(file_name, PROG_FOLDER);
    if (argc > 1) strncat(file_name, argv[1], FILE_NAME_LEN-PROG_FOLDER_NAME_LEN-1);
    else strcat(file_name, STANDARD_FILE_NAME);

    FILE* file = fopen(file_name, "r");
    if (file == NULL) Error_Occures(FILE_INPUT_ERROR);

    Check_Signature(file);

    char c = 0;
    fscanf(file, "%c", &c); //leave \n
    fscanf(file, "%d", &text->size);
    fscanf(file, "%c", &c); //leave \n

    text->buffer = (char*) calloc(text->size, sizeof(char));

    printf("kuku ok\n");

    int size = fread(text->buffer, sizeof(char), text->size, file);

    if (size != text->size) Error_Occures(WRONG_SIZE);
}

void Check_Signature(FILE* code_file)
{
    char signature[SIGNATURE_LEN] = {0};
    fread(signature, sizeof(char), SIGNATURE_LEN-1, code_file);
    if (strcmp(NEEDED_SIGNATURE, signature) != 0)
    {
        Error_Occures(WRONG_SIGNATURE);
    }
}