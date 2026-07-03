#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->decode_stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
    if(argv[3] != NULL)
    {
        // Copy the provided filename into the allocated array space
        strcpy(decInfo->extracted_secret_fname, argv[3]); //
    }
    else
    {
        // Copy the default name into the allocated array space
        strcpy(decInfo->extracted_secret_fname, "decoded");
    }

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->decode_fptr_stego_image = fopen(decInfo->decode_stego_image_fname, "r");
    // Do Error handling
    if (decInfo->decode_fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decode_stego_image_fname);

    	return e_failure;
    }

    decInfo->extracted_fptr_secret = fopen(decInfo->extracted_secret_fname, "w");
    // Do Error handling
    if (decInfo->extracted_fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->extracted_secret_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo->decode_fptr_stego_image, 54, SEEK_SET);
    int len = strlen(magic_string);
    char check_magic_string[len+1];
    for (int i = 0; i < strlen(magic_string); i++)
    {
        char ch = 0;
        for (int j = 0; j < 8; j++)
        {
            char image_byte;
            fread(&image_byte, 1, 1, decInfo->decode_fptr_stego_image);
            ch |= (image_byte & 0x01) << j;
        }
        check_magic_string[i] = ch;
    }
    check_magic_string[len] = '\0';
    if (strcmp(magic_string, check_magic_string) == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status decode_secret_file_extn_size(DecodeInfo *decinfo)
{
    decinfo->extn_size = 0;
    for(int i=0; i<32; i++)
    {
        decinfo->extn_size |= (fgetc(decinfo->decode_fptr_stego_image) & 0x01) << i;
    }
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    for(int i=0; i<decInfo->extn_size; i++)
    {
        char ch = 0;
        for(int j=0; j<8; j++)
        {
            char image_byte;
            fread(&image_byte, 1, 1, decInfo->decode_fptr_stego_image);
            ch |= (image_byte & 0x01) << j;
        }
        decInfo->extn_secret_file[i] = ch;
    }
    decInfo->extn_secret_file[decInfo->extn_size] = '\0';
    strcat(decInfo->extracted_secret_fname, decInfo->extn_secret_file);
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decInfo->size_secret_file = 0;
    for(int i=0; i<32; i++)
    {
        decInfo->size_secret_file |= (fgetc(decInfo->decode_fptr_stego_image) & 0x01) << i;
    }
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    for(long i=0; i<decInfo->size_secret_file; i++)
    {
        char ch = 0;
        for(int j=0; j<8; j++)
        {
            char image_byte;
            fread(&image_byte, 1, 1, decInfo->decode_fptr_stego_image);
            ch |= (image_byte & 0x01) << j;
        }
        fwrite(&ch, 1, 1, decInfo->extracted_fptr_secret);
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo) == e_success)
    {
        printf("INFO : Fopen is success\n");
        if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
        {
            printf("INFO : Decoded magic string successfully\n");
            if(decode_secret_file_extn_size(decInfo) == e_success)
            {
                printf("INFO : Decoded secret file extn size successfully\n");
                if(decode_secret_file_extn(decInfo) == e_success)
                {
                    printf("INFO : Decoded secret file extn successfully\n");
                    if (decode_secret_file_size(decInfo) == e_success)
                    {
                        printf("INFO : Decoded secret file size successfully\n");
                        if(decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("INFO : Decoded secret file data successfully\n");
                            fclose(decInfo->decode_fptr_stego_image);
                            fclose(decInfo->extracted_fptr_secret);
                        }
                        else
                        {
                            printf("ERROR : Failed to decode secret file data\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR : Failed to decode secret file size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR : Failed to decode secret file extn\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR : Failed to decode secret file extn size\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR : Failed to decode magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR : Failed to open files\n");
        return e_failure;
    }
    return e_success;
}
