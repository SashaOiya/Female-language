#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

char *s = nullptr;
int p = 0;

int GetFileSize ( FILE * f );
int GetN ();
int GetG ( char * str );
int GetE ();
int GetT ();
int GetP ();

int main ()
{
    FILE *f = fopen ( "start.txt", "r" );
    assert ( f != nullptr );

    int f_size = GetFileSize ( f );

    char *buffer = ( char *)calloc ( f_size + 1, sizeof ( char ) );
    assert ( buffer != nullptr );

    fread ( buffer, sizeof( buffer[0] ), f_size, f );   // error

    int val = GetG ( buffer );

    printf ( "%d\n", val );

    return 0;
}

int GetG ( char * str )
{
    s = str;
    p = 0;

    //assert ( s[p] == '\0' ); //my_assert

    return GetE();
}

int GetN ()
{
    int val = 0;
    int prev_p = p;

    while ( s[p] >= '0' && s[p] <= '9' ) {
        val = val * 10 + s[p] - '0';
        ++p;
    }
    assert ( p > prev_p );

    return val;
}

int GetFileSize ( FILE * f )
{
    int prev = ftell ( f );

    fseek ( f, 0, SEEK_END );
    size_t size_not_blue = ftell ( f );
    fseek ( f, prev, SEEK_SET );

    return size_not_blue;
}

int GetE ()
{
    int val = GetT ();
    while ( s[p] == '+' || s[p] == '-' )
    {
        int op = s[p];
        p++;
        int val_2 = GetT ();

        switch ( op ) {
            case '+' : {
                val += val_2;
                break;
            }
            case '-' : {
                val -= val_2;
                break;
            }
            default : {
                printf ( "Error" );
                break;
            }
        }
    }
    return val;
}

int GetT ()
{
    int val = GetP ();

    while ( s[p] == '*' || s[p] == '/' ) {
        char op = s[p];
        p++;

        int val_2 = GetP ();

        switch ( op ) {
            case '*' : {
                val *= val_2;
                break;
            }
            case '/' : {
                val /= val_2;
                break;
            }
            default : {
                printf ( "Error" );
                break;
            }
        }
    }
    return val;
}

int GetP ()
{
    if ( s[p] == '(' ) {
        int val = 0;
        p++;
        val = GetE ();

        assert ( s[p] == ')' );
        p++;

        return val;
    }
    else {

        return GetN ();
    }
}



