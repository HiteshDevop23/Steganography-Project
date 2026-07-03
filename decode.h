#ifndef DECODE_H
#define DECODE_H
#include "types.h" // Contains user defined types
#include <stdio.h>
/* 
 * Structure to store information required for
 * decoding secret file from source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILENAME_SIZE 100

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *decode_stego_image_fname;
    FILE *decode_fptr_stego_image;

    /* Secret File Info */
    char extracted_secret_fname[MAX_FILENAME_SIZE];
    FILE *extracted_fptr_secret;
    int extn_size;
    char extn_secret_file[MAX_IMAGE_BUF_SIZE];
    long size_secret_file;
} DecodeInfo;

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status open_decode_files(DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
