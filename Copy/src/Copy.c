#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


#define SPI_PATH "/dev/spidev2.0"
int transfer(int fd, unsigned char send[], unsigned char receive[], int length){
struct spi_ioc_transfer transfer; // the transfer structure
transfer.tx_buf = (unsigned long) send; // the buffer for sending data
transfer.rx_buf = (unsigned long) receive; // the buffer for receiving data
transfer.len = length; // the length of buffer
transfer.speed_hz = 1000000; // the speed in Hz
transfer.bits_per_word = 8; // bits per word
transfer.delay_usecs = 0; // delay in us
// send the SPI message (all of the above fields, inc. buffers)
int status = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
if (status < 0) {
perror("SPI: SPI_IOC_MESSAGE Failed");
return -1;
}

return status;
}
int main(){
unsigned int fd, i=0,b,a=112; // file handle and loop counter

unsigned char value, null=0x00; // sending only a single char
uint8_t bits = 8, mode = 0; // 8-bits per word, SPI mode 3
uint32_t speed = 10000; // Speed is 1 MHz
// The following calls set up the SPI bus properties
fd = open(SPI_PATH, O_RDWR);
ioctl(fd, SPI_IOC_WR_MODE, &mode);
ioctl(fd, SPI_IOC_RD_MODE, &mode);
ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
for (i=33; i<124; i++)
{
transfer(fd, (unsigned char*) &i,(unsigned char*) &b, 1);
printf(" %c ",b);
fflush(stdout); // need to flush the output, as no

usleep(100000); // sleep for 100ms each loop
}
close(fd); // close the file
return 0;
}
