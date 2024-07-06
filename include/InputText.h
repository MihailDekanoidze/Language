#ifndef INPUT_TEXT_H
#define INPUT_TEXT_H

#include "./CommonIncludes.h"

struct text_info {
                    void* buffer;
                    size_t elemcount;
                 };


text_info*  text_info_ctor(void);
size_t      fsize(FILE* file);
void        charprint(struct text_info* text);
Errors      input_text(FILE* file, size_t elem_size, text_info* text);
void        text_info_dtor(text_info* text);

#endif //INPUT_TEXT_H
