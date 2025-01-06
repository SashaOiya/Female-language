#include "my_program.h"

int Get_File_Size ( FILE * file )
{
    assert ( file != nullptr );

    int prev = ftell ( file );

    fseek ( file, 0, SEEK_END );
    size_t size_not_blue = ftell ( file );
    fseek ( file, prev, SEEK_SET );

    return size_not_blue;
}

Error_t Text_Remove_Comments ( Text_t *text )
{
    assert ( text != nullptr );

    if ( !text ) {

        return INPUT_VAL_ERR;
    }

    char *new_buffer = (char *)calloc ( text->file_size, sizeof ( char ) );
    if ( !new_buffer ) {

        return MEMMORY_ERR;
    }
    int new_buffer_size = 0;

    for ( int j = 0; j < text->file_size; ) {
        while ( isspace ( text->data[j] ) ) {
$           ++j;
        }

        while ( *( text->data + j ) != ';' && j < text->file_size ) {
            new_buffer[new_buffer_size++] = text->data[j++];
        }
        text->data[j] = '\0';
        ++(text->n_lines);
        new_buffer[new_buffer_size++] = text->data[j++];

        while ( text->data[j++] != '\n' ) {
$           ;
        }
    }
    text->file_size = new_buffer_size;
    free(text->data);
    text->data = new_buffer;

    return NO_ERR;
}
