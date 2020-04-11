/*
 ============================================================================
 Name        : A.c
 Author      : Khoa Le
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#include <string.h>
#include <errno.h>

#define USER_OPTION  3
#define PATH "/sys/class/leds"


int write_trigger_value(uint8_t led_option,char *mode){

	int fd,ret=0;
	char buf[100];

	//concatenate 2 strings and storing that into buf variable
	snprintf(buf,sizeof(buf),PATH"//beaglebone:green:usr%d/trigger",led_option );

	//open file in write mode
	fd = open(buf,O_WRONLY); //buf is the path and O_WRONLY is for writing
	if(fd<=0){
		perror("write triggert error");
		return -1;
	}
	//write the value to the folder
	ret = write(fd,mode,strlen(mode));
	if(ret <=0){
		printf("trigger value write error");
		return -1;
	}
	return 0;
}

//process trigger
void process_trigger( char  *value){

	if(!(strcmp(value,"timer") && strcmp(value,"heartbeat") && strcmp(value,"none") && strcmp(value,"default-on"))){

		write_trigger_value(USER_OPTION, value);
	}
	else{
		printf("Invalid value for Trigger, Please choose again");
	}
}

//pass argument to folder brightness or trigger
int main(int args, char *arg_Array[]) {

	printf("Control the mode of the LED onboard \n");
	if(args != 3){ //should 3 arguments: file, led mode, led brightness
		printf("usage: %s <trigger> <value> \n ", arg_Array[0]);
		printf("valid trigger are: heartbeat, timer, none, oneshot, default-on\n");
	}
	else{
		//check each of the argument
		if(strcmp(arg_Array[1],"trigger")==0){
			process_trigger(arg_Array[2]);
		}
		else{
			printf("Invalid control");
			printf("Valid control are: trigger");
		}
	}
	return 0;
}
