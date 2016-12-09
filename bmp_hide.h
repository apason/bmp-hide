#ifndef bmp_hide
#define bmp_hide

#include <stdint.h>
#include <stdio.h>

#define READ 1
#define WRITE 0
#define UNDEFINED -1
#define BUFFERSIZE 1024
#define PIXEL_OFFSET 0xA

typedef struct options{
	int direction;
	char *message;
	FILE *picture;
	FILE *message_file;
}OPTIONS;

size_t file_size(FILE *fp);
int check_options_integrity(OPTIONS *options);
void free_options(OPTIONS *options);
void file_to_message(OPTIONS *options);
void read_user_message(OPTIONS *options);
int write_message(OPTIONS *options);
int write_char(FILE *fp,char c);
int write_bit(FILE *fp,uint8_t byte);
int write_byte(FILE *fp,uint8_t byte);
void print_help(void);
void reset_options(OPTIONS *options);
OPTIONS *interactive_options(void);
OPTIONS *check_options(int argc,char *argv[]);
uint8_t read_lsb(uint8_t byte);
uint8_t read_byte(FILE *fp);
char read_char(FILE *fp);
uint32_t read_word(FILE *fp);
uint32_t read_pixel_offset(FILE *fp);
void read_message(OPTIONS *options);

#endif
