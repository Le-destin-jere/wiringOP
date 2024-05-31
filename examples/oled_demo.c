/*
 * Copyright (c) 2015, Vladimir Komendantskiy
 * MIT License
 *
 * SSD1306 demo of block and font drawing.
 */

//
// fixed for OrangePiZero by HypHop
//

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#include "oled.h"
#include "font.h"

int oled_show_list(struct display_info *disp);

void animation(float *a, float *a_trg, uint8_t n)
{
  if (*a != *a_trg){
    if (fabs(*a - *a_trg) < 0.15f) *a = *a_trg;
    else *a += (*a_trg - *a) / (n / 10.0f);
  }
}
#define LINE_OFFSET 9

int oled_get_arg_from_file(char* arg_flie_name, char* input_key, char* out_value, uint16_t out_len)
{
	char line[64];
	FILE *file = fopen(arg_flie_name, "r");
    if (file == NULL) {
        printf("opening file: errno:%d [%s]\n",errno,strerror(errno));
        return -1;
    }

    while (fgets(line, 64, file)) 
	{
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");
        if (key != NULL && value != NULL) {
            key = strtok(key, " \t\r\n");
            value = strtok(value, " \t\r\n");
            printf("Key: %s, Value: %s\n", key, value);
			//here find key value
			if(memcmp(key, input_key, strlen(input_key)) == 0){
				strncpy(out_value, value, out_len);
				return 0;
			}
        } else {
            printf("Invalid line: %s\n", line);
        }
    }

    fclose(file);

    return -2; // no find target key

}

int oled_show_hello(struct display_info *disp, char* arg_flie_name) {
	int ret = 0;
	float x;
	float x_target;
	char ip_str[32] = {0};
	char buff[64]={0};
	x = 128;
	x_target = 10;
	disp->font = font1;

	ret = oled_get_arg_from_file(arg_flie_name, "IP", buff, sizeof(buff));
	if(ret == 0){
		snprintf(ip_str, sizeof(ip_str), "IP:%s", buff);
	}
	while(1)
	{
		animation(&x, &x_target, 20);
		oled_clear_buffer(disp);
		oled_putstrto(disp, (uint8_t)x, 1*LINE_OFFSET+1, "+ Destin linux Pi +");
		oled_putstrto(disp, (uint8_t)x, 4*LINE_OFFSET+1, ip_str);
		oled_send_buffer(disp);
		if(x == x_target){
			break;
		}
	}

	return 0;
}

void show_error(int err, int add) {
	printf("\nERROR: %i, %i\n\n", err, add);
}

void show_usage(char *progname) {
	printf("\nUsage:\n%s <I2C bus device node >\n", progname);
}

int main(int argc, char **argv) {
	int e;
	char filename[32];
	struct display_info disp;

	if (argc < 2) {
		show_usage(argv[0]);
		return -1;
	}

	memset(&disp, 0, sizeof(disp));
	sprintf(filename, "%s", argv[1]);
	disp.address = OLED_I2C_ADDR;
	disp.font = font3;

	e = oled_open(&disp, filename);
	if (e < 0) {
		show_error(1, e);
		goto exit;
	} 

	e = oled_init(&disp);
	if (e < 0) {
		show_error(2, e);
		goto exit;
	}
	sprintf(filename, "%s", argv[2]);
	printf("---------start--------\n");
	if (oled_show_hello(&disp, filename) < 0){
		show_error(3, 777);
	}
	float x = 128;
	float x_target = 0;
	float y = 64;
	float y_target = 0;
#if 1
	while(1)
	{
		animation(&x, &x_target, 45);
		animation(&y, &y_target, 20);
		oled_clear_buffer(&disp);
		oled_draw_img(&disp, x, y, 32, 32, icon_img[0]);
		oled_draw_img(&disp, x+32, y, 32, 32, icon_img[1]);
		oled_draw_img(&disp, x+64, y, 32, 32, icon_img[2]);
		oled_draw_img(&disp, x+96, y, 32, 32, icon_img[3]);
		oled_send_buffer(&disp);
		if(y == y_target){
			break;
		}

	}
#endif
	x = 32;
	x_target = 0;
	while(1)
	{
		animation(&x, &x_target, 30);
		oled_clear_buffer(&disp);
		oled_draw_img(&disp, x-32, y, 32, 32, icon_img[0]);
		oled_draw_img(&disp, x, y, 32, 32, icon_img[1]);
		oled_draw_img(&disp, x+32, y, 32, 32, icon_img[2]);
		oled_draw_img(&disp, x+64, y, 32, 32, icon_img[3]);
		oled_draw_img(&disp, x+96, y, 32, 32, icon_img[4]);
		oled_send_buffer(&disp);
		if(x == x_target){
			break;
		}

	}

	x = 32;
	x_target = 0;
	disp.font = font5;
	while(1)
	{
		animation(&x, &x_target, 30);
		oled_clear_buffer(&disp);
		oled_draw_img(&disp, x-32, y, 32, 32, icon_img[1]);
		oled_draw_img(&disp, x, y, 32, 32, icon_img[2]);
		oled_draw_img(&disp, x+32, y, 32, 32, icon_img[3]);
		oled_draw_img(&disp, x+64, y, 32, 32, icon_img[4]);
		oled_draw_img(&disp, x+96, y, 32, 32, icon_img[6]);
		oled_putstrto2(&disp, x+24, 40, "Setting");
		oled_send_buffer(&disp);
		if(x == x_target){
			break;
		}
	}
	//oled_show_list(&disp);
	printf("----------end---------\n");

exit:

	return 0;
}

#define LIST_MAX_STR_LEN 32
char test_list[][LIST_MAX_STR_LEN] =
{   
	"= Setting",
	"+ network",
	"- ethnet",
	"- wlan",
	"+ power",
	"- sleep",
	"+ disp",
	"+ power",
};

int oled_show_list(struct display_info *disp) 
{
	float y;
	float y_target;
	uint8_t i;
	char* plist = NULL;
	
	disp->font = font6;
	y = 16;
	y_target = 20;
	plist = test_list[0];

	oled_clear_buffer(disp);
	oled_putstrto2(disp, 0, 0, plist);
	oled_send_buffer(disp);

	while(1)
	{
		animation(&y, &y_target, 100);
		oled_clear_buffer(disp);
		for(i = 1; i < 4; i++){
			plist = test_list[i];
			oled_putstrto2(disp, 0, y+i*(disp->font.height)+2, plist);
		}
		oled_send_buffer(disp);
		if(y == y_target){
			break;
		}
	}

	return 0;
}