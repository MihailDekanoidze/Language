#include <stdio.h>
#include <malloc.h>
#include "../include/InputText.h"


text_info* text_info_ctor(void)
{
    text_info* text = (text_info*)calloc(1, sizeof(text_info));
    return text;
}

void text_info_dtor(text_info* text)
{
    if (!text)  
    {
        printf("Null ptr was given by %s\n", __PRETTY_FUNCTION__);
        return;
    }
    
    free(text->buffer);
    text->elemcount = 0;
    free(text);
}

size_t fsize(FILE* file)
{
    size_t begin = 0;
    size_t end = 0;

    begin = (size_t)ftell(file);
    fseek(file, 0, SEEK_END);
    end = (size_t)ftell(file);

    rewind(file);

    return end - begin;
}

Errors input_text(FILE* file, size_t elem_size, text_info* text)
{
    size_t size = 0;
    size = fsize(file);
    rewind(file);

    char* buffer = (char*) calloc(size + 1, elem_size);
    if (!buffer) return CALLOC;

    size_t elemcount = 0;
    elemcount = fread(buffer, elem_size, size, file);
    if (!elemcount) return CALLOC;

    buffer[elemcount * elem_size] = '\0';

    text->buffer = buffer;
    text->elemcount = elemcount;

    return NO_ERROR;
}

void charprint(struct text_info* text)
{
    size_t size = text->elemcount;
    char* ptr = (char*) text->buffer;

    for (size_t i = 0; i < size; i++)
    {
        printf("%c (%d)\n", ptr[i], ptr[i]);
    }

}





