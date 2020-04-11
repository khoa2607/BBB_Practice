/*
 ============================================================================
 Name        : TeensyTalkative.c
 Author      : Khoa Le
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //OPEN
#include <stdint.h> //FOR UNSIGNED INT

#include <linux/i2c-dev.h> //for slave in i2c interface in terminal
#include <sys/ioctl.h> //ioctl to configure I/O in kernel

#include <unistd.h>

//define some of the variable
#define PATH "/dev/i2c-2"

//slave address b110100X ->  b 0110 1000 -> 68
#define SLAVE_ADDR 0x44
#define DEVICE_ADDR 0x00
#define ADDR_WRITE 0x0C

int fd; //open descriptor

int write_data(uint8_t base_addr, uint16_t buffer){

	int ret;
	char buf[2];
	buf[0] = base_addr;
	buf[1] = buffer;

	//support sent ADDR + 0 to confirm the first one
	ret = write(fd,buf,2);
	if(ret <=0){
		perror("write fail!!-Write Function");
		return -1;
	}
	return 0;
}

//read from what address,
int read_data(uint8_t base_addr,char *buffer, uint32_t len){

	int ret;
	char buf[2];
	buf[0] = base_addr;

	//start to write first
	ret = write(fd,buf,1);
	if(ret<=0){
		perror("Write fail!!-Read Function");
		return -1;
	}

	//then read
	ret = read(fd,buffer,len); //expect to read 1 byte
	if (ret<=0){
		perror("read fail");
		return -1;
	}
	return 0;
}

//void read_Teensy(short int *buffer){
void read_Teensy(char *buffer){
	char buf[6];

	read_data(DEVICE_ADDR,buf,6);

	for (int i=0;i<6;i++){
		buffer[i] = (int)(buf[i]);
	}
/*
	buffer[0] = (int)(buf[0]<<8 | buf[1]);
	buffer[1] = (int)(buf[2]<<8 | buf[3]);
	buffer[2] = (int)(buf[4]<<8 | buf[5]);
*/
}
/*
void write_Teensy(char *buffer){
//	char buf[4]; //Already know the bytes

	write_data(ADDR_WRITE,buffer);

}
*/

int main(void) {

//	short read_value [3];
	char read_value[6];
//	char write_value [4] = {0x0D,0x0E,0x0F,0x10};
//	uint8_t write_value  = 0x10;

	//access i2c folder
	fd = open(PATH,O_RDWR);
	if(fd <0){
		perror("open path to i2c wrong-please check path /dev/i2c-c\n");
		return -1;
	}

	//specify the address that wants to communicate
	if(ioctl(fd,I2C_SLAVE,SLAVE_ADDR)<0){
		perror("Communicate between slave and master(controller)! - could be I2C_SLAVE address is wrong! \n");
		close(fd);
		return -1;
	}

//	int ret;
	uint8_t device_address[4] = {0x0B,0x0C,0x0D,0x0E};
	uint8_t wrvalue [4] = {0x0E,0x0F,0x10,0x11};

	while(1){
		//sending 1 byte to Teensy

		//read data from Teensy
		read_Teensy(read_value);
		sleep(1);
		printf("Device ID: %d%d, Year.Date.Month: %d%d%d%d \n",read_value[0],read_value[1],read_value[2],read_value[3],read_value[4],read_value[5],read_value[6]);
//		ret = write(fd,buf1,2);
//		write_Teensy(write_value);
		for (int i=0;i<4;i++){
			write_data(device_address[i],wrvalue[i]);
			printf("Address: %d Data: %d\n", device_address[i],wrvalue[i]);
			sleep(2);
		}

//		printf("read_value[0]: %d,read_value[1]: %d,read_value[2]: %d",read_value[0],read_value[1],read_value[2]),

//		printf("Device ID: %d, Year.Date.Month: %d %d \n",read_value[0],read_value[1],read_value[2]);



	}

	return 0;
}
