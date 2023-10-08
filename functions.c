#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "lab_png.h"
#include "crc.h"
#include <arpa/inet.h>

int is_png(U32 *buf, size_t n){
/*return 1 for yes, return 0 for no*/
    /*store the PNG signature header example in a char */
    U32 png_sig[2] = {0x89504E47, 0x0D0A1A0A};
    /* verify that the given file is a valid png */
    return ntohl(buf[0]) == png_sig[0] && ntohl(buf[1]) == png_sig[1];

}

/* extract header chunk*/
int get_png_data_IHDR(struct data_IHDR *out, FILE *fp, long offset, int whence){
    U32 height_width_buf[2];
    //U8 data_buf[5];

    // sets the file position of the stream to the given offset, starting from whence
    fseek(fp, offset, whence);

    //reads data from the given stream into the ptr
    fread(height_width_buf, 8, 1 ,fp);

    //correct byte order (endianness) while assigning to the IHDR ptr
    out -> width = ntohl(height_width_buf[0]);
    out -> height = ntohl(height_width_buf[1]);

    return 0;
}

int get_chunk(struct chunk *out, FILE *fp, long offset){
    //store type and data in size 50 char array
    U32 length_a[1];
    U8 type_a[4];
    U32 crc_a[1];

    //variable to store the length, in bytes, of the data layer
    int data_len;

    //store length
    fseek(fp, 8 + offset, SEEK_SET);
    fread(length_a, 4, 1, fp);

    out -> length = ntohl(length_a[0]);

    data_len = (int) out -> length;
    //U8 data_a[data_len];

    //store type
    fseek(fp, 12 + offset, SEEK_SET);
    fread(type_a, 4, 1, fp);

    for(int i = 0; i <= 3; ++i){
        out -> type[i] = type_a[i];
    }

    //store CRC
    fseek(fp, 16 + data_len + offset, SEEK_SET);
    fread(crc_a, 4, 1, fp);

    out -> crc = ntohl(crc_a[0]);

    return 0;
}

U8 get_chunk_data(FILE *fp, long offset, long offset2){
    U8 data[1];
    fseek(fp, 16 + offset + offset2, SEEK_SET);
    fread(data, 1, 1, fp);

    return data[0];

};