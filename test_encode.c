#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"

int main(int argc, char *argv[])
{
    EncodeInfo E1;
    DecodeInfo D1;
    
    int ret = check_operation_type(argv);

    // Validate CLA
    if (ret == e_encode)
    {
        if(argc < 4)
        {
            printf("ERROR : Enter valid no. of CLA\n");
            return 1;
        }
    }
    if (ret == e_decode)
    {
        if(argc < 3)
        {
            printf("ERROR : Enter valid no. of CLA\n");
            return 1;
        }
    }
    
    if( ret == e_encode)
    {
        printf("INFO : Encoding is selected\n");
        if(read_and_validate_encode_args(argv,&E1) == e_success)
        {
            printf("INFO : Read and Validate encode args is success\n");
            if(do_encoding(&E1) == e_success)
            {
                printf("INFO : Encoding is success\n");
            }
            else
            {
                printf("ERROR : Failed to encode\n");
            }
        }
        else
        {
            printf("ERROR : Failed to Read and Validate encode args\n");
        }
    }
    else if( ret == e_decode)
    {
        printf("INFO : Decoding is selected\n");
        if(read_and_validate_decode_args(argv,&D1) == e_success)
        {
            printf("INFO : Read and Validate decode args is success\n");
            if(do_decoding(&D1) == e_success)
            {
                printf("INFO : decoding is success\n");
            }
            else
            {
                printf("ERROR : Failed to decode\n");
            }
        }
        else
        {
            printf("ERROR : Failed to Read and Validate decode args\n");
        }
    }
    else
    {
        printf("Invalid option\n");  
    }
    
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if( strcmp(argv[1], "-e")==0)
        return e_encode;
    else if( strcmp(argv[1], "-d")==0)
        return e_decode;
    else
        return e_unsupported;
}

