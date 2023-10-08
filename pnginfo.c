#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "lab_png.h"
#include "crc.h"

U32 check_crc(U32 non_calculated_crc, U8 *td, int data_len);

int main (int argc, char * argv[]){ 
/*read in the files from the path*/
// allocate memory
U32 sig_buf[2];

// open the file from the given path
FILE *f = fopen(argv[1], "rb");

// read to sig_buf
fread(sig_buf, 8, 1, f );

if(is_png(sig_buf, PNG_SIG_SIZE) == 1){
    /* if file is png, check for CRC error*/

    struct data_IHDR IHDR_obj; 
    struct data_IHDR *IHDR_obj_ptr = &IHDR_obj;

    // get the IHDR chunk 
    get_png_data_IHDR (IHDR_obj_ptr, f, PNG_SIG_SIZE, SEEK_CUR);

    printf("%s: Is a PNG file: %d x %d \n", argv[1], IHDR_obj_ptr -> width, IHDR_obj_ptr -> height);

    /*Check for CRC Error*/

    //use get_chunk to get each chunk -> check crc errors in all chunks, IHDR, IDAT, IEND

    struct simple_PNG png_obj;
    
    struct chunk png_1;
    struct chunk png_2;
    struct chunk png_3;

    png_obj.p_IDAT = &png_1;
    png_obj.p_IHDR = &png_2;
    png_obj.p_IEND = &png_3;

//IHDR
    get_chunk(png_obj.p_IHDR, f, 0);

    int data_len_IH = (int) (png_obj.p_IHDR) -> length;

    // create an array to store the data hex
    U8 d_IHDR[data_len_IH];

    for(int i = 0; i < data_len_IH; ++i){
        d_IHDR[i] = get_chunk_data(f, 0, i);
    }

    png_obj.p_IHDR->p_data = d_IHDR;

    // calculate the offset needed to get IDAT chunk
    int offset_IH = 8 + data_len_IH  + 4; 

// IDAT
    get_chunk(png_obj.p_IDAT, f, offset_IH);

    int data_len_ID = (int) (png_obj.p_IDAT) -> length;

    U8 d_IDAT[data_len_ID];

    for(int i = 0; i < data_len_ID; ++i){
        d_IDAT[i] = get_chunk_data(f, offset_IH,  i);
    }

    png_obj.p_IDAT->p_data = d_IDAT;

    //calculate the offset needed to get IEND chunk

    int offset_ID = offset_IH + 8 + data_len_ID + 4; 

//IEND
    get_chunk(png_obj.p_IEND, f, offset_ID);

    /* compare CRC values */

   //type and data
    U8 td_IH[data_len_IH + 4];
    U32 crc_IH = png_obj.p_IHDR -> crc;

   for(int i = 0; i < data_len_IH + 4; ++i){
	if(i < 4){
	 td_IH[i] = (png_obj.p_IHDR -> type)[i];
   	}else{
	 td_IH[i] = (png_obj.p_IHDR -> p_data)[i-4];
	}
   }

   check_crc(crc_IH, td_IH, data_len_IH);

   if (check_crc(crc_IH, td_IH, data_len_IH) != 0) {
   //CRC error in IHDR chunk
   printf("IHDR chunk CRC error: computed %x, expected %x\n", check_crc(crc_IH, td_IH, data_len_IH), crc_IH);
   }

    U8 td_ID[data_len_ID + 4];
    U32 crc_ID = png_obj.p_IDAT -> crc;

    for(int i = 0; i < data_len_ID + 4; ++i){
	    if (i < 4) {
	    td_ID[i] = (png_obj.p_IDAT -> type)[i];
   	    } else {
        td_ID[i] = (png_obj.p_IDAT -> p_data)[i-4];
        }
    }


    if (check_crc(crc_ID, td_ID, data_len_ID) != 0) {
        //CRC error in IHDR chunk
        printf("IDAT chunk CRC error: computed %x, expected %x\n",check_crc(crc_ID, td_ID, data_len_ID), crc_ID);
    }



    U8 td_IE[4];
    U32 crc_IE = png_obj.p_IEND -> crc;

    for(int i = 0; i < 4; ++i){
        td_IE[i] = (png_obj.p_IEND -> type)[i];
    }

    if (check_crc(crc_IE, td_IE, 0) != 0 ) {
   //CRC error in IHDR chunk
        printf("IEND chunk CRC error: computed %x, expected %x\n", check_crc(crc_IE, td_IE, 0), crc_IE);
    }

    fclose(f);

} else {
    /*file is not a png*/
    printf("%s: Not a PNG file\n", argv[1]);

    fclose(f);
}

}
//check file signature instead of just extension (first 8 bytes)
/* Function Definitions*/

U32 check_crc(U32 non_calculated_crc, U8 *td ,int data_len) {
    /*return 0 for no CRC error otherwise, return calculated crc value*/

    U32 calculated_crc = crc(td, data_len + 4);

    if (calculated_crc != non_calculated_crc) {
        return calculated_crc;
    } else {
        return 0;
    }

    return 0; 

}