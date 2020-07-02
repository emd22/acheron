#include <f3d/engine/image/bmp.h>

#include <f3d/engine/core/cr_log.h>
#include <f3d/engine/core/memory/mm_memory.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static int            read_long(FILE *fp);
static unsigned       read_dword(FILE *fp);
static unsigned short read_word(FILE *fp);

image_bmp_t bmp_load(const char *filename) {
    int x, y;        
    int length;      
    int bitsize;
    int infosize;
    bmp_file_header_t file_header;
    
    image_bmp_t bmp;
    bmp.data = NULL;
    
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
        return bmp;
    uint16_t type = read_word(fp);
    file_header.type[0] = (uint8_t)(type);
    file_header.type[1] = (uint8_t)(type >> 8);
    file_header.file_size = read_dword(fp);
    file_header.resv0     = read_word(fp);
    file_header.resv1     = read_word(fp);
    file_header.offset    = read_dword(fp);

    if (file_header.type[0] != 'B' || file_header.type[1] != 'M') {
        ar_log(AR_LOG_ERROR, "malformed BMP signature (0x%02X,0x%02X != 0x42,0x4D)", file_header.type[0], file_header.type[1]);
        goto error;
    }

    infosize = file_header.offset - 18;
    bmp_info_header_t *header = &(bmp.info.header);

    header->size            = read_dword(fp);
    header->image_width     = read_long(fp);
    header->image_height    = read_long(fp);
    header->colour_planes   = read_word(fp);
    header->pixel_bit_count = read_word(fp);
    header->image_compression  = read_dword(fp);
    header->image_data_size    = read_dword(fp);
    header->pixels_per_meter_x = read_long(fp);
    header->pixels_per_meter_y = read_long(fp);
    header->colours_used       = read_dword(fp);
    header->important_colours  = read_dword(fp);

    if (infosize > 40)
	    if (fread(bmp.info.colours, infosize - 40, 1, fp) < 1) {
            goto error;
        }
        
    if ((bitsize = header->image_data_size) == 0)
        bitsize = (header->image_width *
                   header->pixel_bit_count+7)/8 *
  	           abs(header->image_height);

    if ((bmp.data = ar_memory_alloc(bitsize)) == NULL) {
        goto error;
    }

    if (fread(bmp.data, 1, bitsize, fp) < (unsigned long)bitsize) {
        goto error;
    }

    /* Swap red and blue */
    length = (header->image_width*3+3) & ~3;
    uint8_t *ptr;
    uint8_t temp;
    for (y = 0; y < header->image_height; y++) {
        ptr = (bmp.data)+y*length;
        x = header->image_width;
        for (; x > 0; x--, ptr += 3) {
	        temp   = ptr[0];
	        ptr[0] = ptr[2];
	        ptr[2] = temp;
	    }
    }

    /* OK, everything went fine - return the allocated bitmap... */
    //log_msg(LOG_INFO, "Done\n", 0);
    fclose(fp);
    return bmp;
error:;
    if (bmp.data != NULL)
        ar_memory_free(bmp.data);
    bmp.data = NULL;
    fclose(fp);
    return bmp;
}


/*

int bmp_save(const char *filename, BITMAPINFO *info, unsigned char *bits) {
    FILE *fp;
    int  size,
         infosize,
         bitsize;

    if ((fp = fopen(filename, "wb")) == NULL)
        return (-1);

    if (info->bmiHeader.biSizeImage == 0)
	bitsize = (info->bmiHeader.biWidth *
        	   info->bmiHeader.biBitCount + 7) / 8 *
		  abs(info->bmiHeader.biHeight);
    else
	bitsize = info->bmiHeader.biSizeImage;

    infosize = sizeof(BITMAPINFOHEADER);
    switch (info->bmiHeader.biCompression)
	{
	case BI_BITFIELDS :
            infosize += 12;
            if (info->bmiHeader.biClrUsed == 0)
	      break;
	case BI_RGB :
            if (info->bmiHeader.biBitCount > 8 &&
        	info->bmiHeader.biClrUsed == 0)
	      break;
	case BI_RLE8 :
	case BI_RLE4 :
            if (info->bmiHeader.biClrUsed == 0)
              infosize += (1 << info->bmiHeader.biBitCount) * 4;
	    else
              infosize += info->bmiHeader.biClrUsed * 4;
	    break;
	}

    size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;
    
    write_word(fp, BF_TYPE);        
    write_dword(fp, size);          
    write_word(fp, 0);              
    write_word(fp, 0);              
    write_dword(fp, 18 + infosize); 

    write_dword(fp, info->bmiHeader.biSize);
    write_long(fp, info->bmiHeader.biWidth);
    write_long(fp, info->bmiHeader.biHeight);
    write_word(fp, info->bmiHeader.biPlanes);
    write_word(fp, info->bmiHeader.biBitCount);
    write_dword(fp, info->bmiHeader.biCompression);
    write_dword(fp, info->bmiHeader.biSizeImage);
    write_long(fp, info->bmiHeader.biXPelsPerMeter);
    write_long(fp, info->bmiHeader.biYPelsPerMeter);
    write_dword(fp, info->bmiHeader.biClrUsed);
    write_dword(fp, info->bmiHeader.biClrImportant);

    if (infosize > 40)
	if (fwrite(info->bmiColors, infosize - 40, 1, fp) < 1) {
            fclose(fp);
            return (-1);
            }

    if (fwrite(bits, 1, bitsize, fp) < (unsigned long)bitsize)
        {
        fclose(fp);
        return (-1);
        }
    fclose(fp);
    return (0);
    }
*/

static unsigned short     
read_word(FILE *fp)       
    {
    unsigned char b0, b1; 

    b0 = getc(fp);
    b1 = getc(fp);

    return ((b1 << 8) | b0);
    }


static unsigned int read_dword(FILE *fp) {
    unsigned char b0, b1, b2, b3; 

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
    }

static int                        
read_long(FILE *fp) {
    unsigned char b0, b1, b2, b3; 

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}
/*
static int                     
write_word(FILE           *fp, 
           unsigned short w)
    {
    putc(w, fp);
    return (putc(w >> 8, fp));
    }

static int
write_dword(FILE *fp,
            unsigned int dw) 
    {
    putc(dw, fp);
    putc(dw >> 8, fp);
    putc(dw >> 16, fp);
    return (putc(dw >> 24, fp));
    }

static int
write_long(FILE *fp,
           int  l)
    {
    putc(l, fp);
    putc(l >> 8, fp);
    putc(l >> 16, fp);
    return (putc(l >> 24, fp));
    }
*/

