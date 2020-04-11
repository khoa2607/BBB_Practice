
#include <stdint.h>
#include <unistd.h> //
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <stdlib.h> //wait

#include <linux/types.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

//define some constant variables
#define PATH "/dev/spidev2.0"

static	uint16_t delay =0;
static	uint32_t speed = 10000;
static uint8_t bits = 8;
static uint8_t mode = 0;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

 void write_data(int fd){

	int ret;

	char tx[2];


	char rx[ARRAY_SIZE(tx)] = {0, };

	struct spi_ioc_transfer tr ={
		.tx_buf = (unsigned long) tx,
		.rx_buf = (unsigned long) rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	tx[0] = 0x01;
	//send one bye at a time.
	ret = ioctl(fd,SPI_IOC_MESSAGE(1),&tr);
	if (ret<0){
		perror("SPI_IOC_MESSAGE");
	}

	tx[1] = 0x02;
	//send one bye at a time.
	ret = ioctl(fd,SPI_IOC_MESSAGE(1),&tr);
	if (ret<0){
		perror("SPI_IOC_MESSAGE");
	}

}

int main(void) {


	int ret,fd;


	//access to spidev location
	fd = open(PATH,O_RDWR);
	if (fd<0){
		perror("Bad fd open - check PATH");
		return -1;
	}
	//setup SPI mode
	ret = ioctl(fd,SPI_IOC_WR_MODE,&mode);
	if(ret<0){
		perror("SPI_IOC_WR_MODE");
	}
	ret = ioctl(fd,SPI_IOC_RD_MODE,&mode);
	if(ret<0){
		perror("SPI_IOC_RD_MODE");
	}
	ret = ioctl(fd,SPI_IOC_WR_BITS_PER_WORD,&bits);
	if(ret<0){
		perror("SPI_IOC_WR_BITS_PER_WORD");
	}
	ret = ioctl(fd,SPI_IOC_RD_BITS_PER_WORD,&bits);
	if(ret<0){
		perror("SPI_IOC_RD_BITS_PER_WORD");
	}
	ret = ioctl(fd,SPI_IOC_WR_MAX_SPEED_HZ,&speed);
	if(ret<0){
		perror("SPI_IOC_WR_MAX_SPEED_HZ");
	}
	ret = ioctl(fd,SPI_IOC_RD_MAX_SPEED_HZ,&speed);
	if(ret<0){
		perror("SPI_IOC_RD_MAX_SPEED_HZ");
	}

	write_data(fd);

//	fflush(stdout);

	usleep(100*1000);

	close(fd);

	return 0;


}
