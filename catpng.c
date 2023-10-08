#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>   /* for printf().  man 3 printf */
#include <stdlib.h>  /* for exit().    man 3 exit   */
#include <string.h>  /* for strcat().  man strcat   */
#include "lab_png.h"
#include "zutil.h"
#include <errno.h>

<<<<<<< HEAD
=======
//function to initialize data
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
void init_data(U8 *buf, int len)
{
    int i;
    for ( i = 0; i < len; i++) {
        buf[i] = i%256;
    }
}


int main(int argc, char *argv[]) {
    //ensure that at least one png file was given as an argument  
    if (argc == 1) {
        fprintf(stderr, "Usage: %s <directory name>\n", argv[0]);
        exit(1);
    }

    //create/open all.png
    FILE *all_file = fopen("all.png", "wb+");

    //create an array to hold the first 33 bytes to be passed in
    char first_bytes[33];

    //pass in the png signature and IHDR from the first file, write to all, and close it 
    FILE *file_1 = fopen(argv[1], "rb");
    fread(first_bytes, 1, 33, file_1);

<<<<<<< HEAD
    // do i wanna do this??
=======
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
    fclose(file_1);

    fwrite(first_bytes, 1, 33, all_file);

    //define...
    struct data_IHDR IHDR_obj1;
    struct data_IHDR *IHDR_obj_ptr1 = &IHDR_obj1;
    U32 all_width = 0;
    U32 all_height = 0;
    U32 all_height_input = 0;

    //for each file passed in...
    for (int i = 1; i < argc; ++i) {
        //open the file and get the IHDR to find all.png's height 
        FILE *current_png_file = fopen(argv[i], "rb");
        get_png_data_IHDR (IHDR_obj_ptr1, current_png_file, 16, SEEK_SET);

        if(i == 1){
            all_width = IHDR_obj_ptr1 -> width;
        }else if(IHDR_obj_ptr1-> width != all_width){
            printf("WARNING: Widths are different ");
        }

        all_height += IHDR_obj_ptr1 -> height;
        fclose(current_png_file);
    }

    all_height_input = htonl(all_height);
<<<<<<< HEAD

    U32 *ptr1 = &all_height_input;
=======
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe

    U32 *ptr1 = &all_height_input;
    fseek(all_file, 20, 0);
    fwrite(ptr1, 1, 4 ,all_file);

<<<<<<< HEAD
    //make smthn to keep the total data 

    U8 *p_buffer = NULL;  //a buffer that contains some data to play with 
    U8 *p_buffer_def = NULL;  //a buffer that contains some data to play with 
    //U32 crc_val = 0;      //CRC value                                     
=======
    U8 *p_buffer = NULL;  //a buffer that contains some data to play with 
    U8 *p_buffer_def = NULL;  //a buffer that contains some data to play with 
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
    int ret = 0;          //return value for various routines           
    U64 len = 0;
    U64 len2 = 0;                             
    U64 *len_inf = &len;      //uncompressed data length
    U64 *len_def = &len2;
    U64 len_tot = 0;          //total

   U32 buffer_size = all_height * (all_width*4 + 1);
   p_buffer = malloc(buffer_size * sizeof(unsigned char));  
    if (p_buffer == NULL) {
        perror("malloc");
	return errno;
    }

<<<<<<< HEAD
    //printf("%d\n",(all_height * (all_width + 1)));

=======
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
    init_data(p_buffer, buffer_size * sizeof(unsigned char));

    for (int i = 1; i < argc; ++i) {
        //get first chunk 
        //get data from it 
        //put it into the data of the all.png
        //make sure the pointer moves (SEEK_CUR)
        //keep track of length field 
        //do it again for the next chunk in the next data location 

        struct chunk IDAT;
        struct chunk *IDAT_p = &IDAT;
        FILE *curr = fopen(argv[i], "rb");

        struct data_IHDR IHDR_object;
        struct data_IHDR *IHDR_object_ptr = &IHDR_object;
        get_png_data_IHDR (IHDR_object_ptr, curr, 16, SEEK_SET);
<<<<<<< HEAD

        U32 curr_height = IHDR_object_ptr -> height ;
        U32 curr_width = IHDR_object_ptr -> width;
        U32 curr_size = curr_height * (curr_width*4 + 1);

        U8 *p_data_buff = malloc(curr_size * sizeof(unsigned char));

=======

        U32 curr_height = IHDR_object_ptr -> height ;
        U32 curr_width = IHDR_object_ptr -> width;
        U32 curr_size = curr_height * (curr_width*4 + 1);

        U8 *p_data_buff = malloc(curr_size * sizeof(unsigned char));

>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
        get_chunk(IDAT_p, curr, 25);

        int data_len_ID = IDAT_p->length;

        U8 d_IDAT[data_len_ID];

        for(int i = 0; i < data_len_ID; ++i){
            d_IDAT[i] = get_chunk_data(curr, 25, i);
<<<<<<< HEAD
           //printf("%x\n", d_IDAT[i]);
=======
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
        }

        IDAT_p->p_data = d_IDAT;

        U64 data_len_ID2 = IDAT_p->length;
<<<<<<< HEAD

        //printf("%d\n", IDAT_p->length);

        ret = mem_inf(p_data_buff, len_inf,  IDAT_p->p_data , data_len_ID2);

        //printf("%d\n", *len_inf);

        memcpy(p_buffer + len_tot, p_data_buff, *len_inf);
        //printf("%d\n", p_buffer);
        len_tot += *len_inf;

        //printf("%d", len_tot);
        free(p_data_buff);
        //inflate
=======

        ret = mem_inf(p_data_buff, len_inf,  IDAT_p->p_data , data_len_ID2);

        memcpy(p_buffer + len_tot, p_data_buff, *len_inf);
        len_tot += *len_inf;

        free(p_data_buff);
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
        fclose(curr);
    }

    p_buffer_def = malloc(buffer_size * sizeof(unsigned char));  
<<<<<<< HEAD
    if (p_buffer_def == NULL) {
        perror("malloc");
	return errno;
    }

    init_data(p_buffer_def, buffer_size * sizeof(unsigned char));

    ret = mem_def(p_buffer_def, len_def, p_buffer, len_tot, Z_DEFAULT_COMPRESSION);

    U8 *p_final_buf = malloc( (*len_def) * sizeof(unsigned char));

    memcpy(p_final_buf, p_buffer_def, *len_def);
    
    //might not have to do ntohl 
    fseek(all_file, 33, SEEK_SET);
    //printf("%d\n", *len_def);
    U64 buffer = 0;
    U64 *len_def_reverse = &buffer;
    *len_def_reverse = ntohl(*len_def);
    //printf("%d\n", *len_def_reverse);
=======
        if (p_buffer_def == NULL) {
            perror("malloc");
	    return errno;
    }

    init_data(p_buffer_def, buffer_size * sizeof(unsigned char));

    ret = mem_def(p_buffer_def, len_def, p_buffer, len_tot, Z_DEFAULT_COMPRESSION);

    U8 *p_final_buf = malloc( (*len_def) * sizeof(unsigned char));

    memcpy(p_final_buf, p_buffer_def, *len_def);
    
    fseek(all_file, 33, SEEK_SET);
    U64 buffer = 0;
    U64 *len_def_reverse = &buffer;
    *len_def_reverse = ntohl(*len_def);
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
    fwrite(len_def_reverse, 1, 4 ,all_file);

    fseek(all_file, 37, SEEK_SET);
    char type_ptr[4] = {0x49, 0x44, 0x41, 0x54};
    fwrite(type_ptr, 1, 4 ,all_file);

    fseek(all_file, 41, SEEK_SET);
<<<<<<< HEAD

    fwrite(p_final_buf, 1, *len_def, all_file);

    U8 td[*len_def + 4];

   for(int i = 0; i < *len_def + 4; ++i){
	if(i < 4){
	 td[i] = type_ptr[i];
   	}else{
	 td[i] = p_final_buf[i-4];
	}
   }

    U32 calculated_crc = crc(td,*len_def + 4);
=======
    fwrite(p_final_buf, 1, *len_def, all_file);

    U8 td[*len_def + 4];
    for(int i = 0; i < *len_def + 4; ++i) {
	    if (i < 4) {
	        td[i] = type_ptr[i];
   	    } else {
	        td[i] = p_final_buf[i-4];
	    }
    }
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe

    //set the crc for the IDAT chunk
    U32 calculated_crc = ntohl(crc(td,*len_def + 4));
    U32 *ptr_crc = &calculated_crc;
<<<<<<< HEAD

    //printf("%x\n", calculated_crc);

    fseek(all_file, 41 + *len_def, SEEK_SET);

    fwrite(ptr_crc, 1, 4, all_file); 

=======
    fseek(all_file, 41 + *len_def, SEEK_SET);
    fwrite(ptr_crc, 1, 4, all_file); 


    //set the length for the IEND chunk
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
    fseek(all_file, 45 + *len_def, SEEK_SET);
    char length_IEND[4] = {0x0, 0x0, 0x0, 0x0};
    fwrite(length_IEND, 1, 4 ,all_file);

<<<<<<< HEAD
=======
    //set the type for the IEND chunk
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
    fseek(all_file, 49 + *len_def, SEEK_SET);
    char type_IEND[4] = {0x49, 0x45, 0x4e, 0x44};
    fwrite(type_IEND, 1, 4 ,all_file);

    //set the crc for the IEND chunk
    U32 calculated_crc_end = ntohl(crc(type_IEND, 4));
    U32 *ptr_crc_end = &calculated_crc_end;
<<<<<<< HEAD

    fwrite(ptr_crc_end, 1, 4, all_file);
    

    fclose(all_file);
=======
    fwrite(ptr_crc_end, 1, 4, all_file);


    //set the CRC for the IHDR chunk
    char IHDR_data[13];
    fseek(all_file, 16, SEEK_SET);
    fread(IHDR_data, 1, 13, all_file);
    char type_IHDR[4] = {0x49, 0x48, 0x44, 0x52};
    U8 td2[17];
    for(int i = 0; i < 17; ++i){
	    if(i < 4){
	        td2[i] = type_IHDR[i];
   	    } else {
	        td2[i] = IHDR_data[i-4];
	}
   }
    U32 calculated_crc_ihdr = ntohl(crc(td2,17));
    U32 *ptr_crc_ihdr = &calculated_crc_ihdr;
    fseek(all_file, 29, SEEK_SET);
    fwrite(ptr_crc_ihdr, 1, 4, all_file); 

    //free memory 
    free(p_buffer);
    free(p_buffer_def);
    free(p_final_buf);

    fclose(all_file); 
>>>>>>> 44ddbce434204feb7995c9262acfda5e2fc4e8fe
}