//extern "C" {
//#include <linux/i2c-dev.h>
//#include <i2c/smbus.h>
//}
//#include <linux/i2c-dev.h>

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <sys/ioctl.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h> 
//#include <iostream>
//using namespace std;

int file;
int adapter_nr = 1;
const char* filename = "/dev/i2c-4";
int initialize_mpu(int file);

signed int high;
signed int low;
signed int value;

#define I2C_SLAVE	0x0703	/* Use this slave address */
int read_raw_data(int file, unsigned char addr);

int main()
{
	file = open(filename, O_RDWR);
	if (file < 0) {
		exit(1);
	}
	int addr = 0x18;

	if (ioctl(file, I2C_SLAVE, addr) < 0) {
		exit(1);
	}

	unsigned char res;

	//     signed int whoami;
	signed int accel_x2;
	signed int accel_x;
	signed int accel_y;
	signed int accel_z;

	signed int accel_x_register_high = 0x29;
	signed int accel_y_register_high = 0x2B;
	signed int accel_z_register_high = 0x2D;

	res = i2c_smbus_write_byte_data(file, addr, 0);

	if (res < 0) {
		/* ERROR HANDLING: i2c transaction failed */
	} else {
		/* res contains the read word */
	}



	initialize_mpu(file);




	while (1) {

		accel_x = read_raw_data(file, accel_x_register_high) / 133.0;
		accel_y = read_raw_data(file, accel_y_register_high) / 133.0;
		accel_z = read_raw_data(file, accel_z_register_high) / 133.0;
		printf("[x y z]: %d %d %d\n", accel_x, accel_y, accel_z);
		//cout << accel_x <<" " << accel_y << " " << accel_z << " " << endl;

		usleep(150000);
	}
}


int initialize_mpu(int file) {
	i2c_smbus_write_byte_data(file, 0x20, 0x57);
	i2c_smbus_write_byte_data(file, 0x21, 0x00);
	i2c_smbus_write_byte_data(file, 0x22, 0x40);
	i2c_smbus_write_byte_data(file, 0x23, 0x00);
	i2c_smbus_write_byte_data(file, 0x24, 0x08);
	i2c_smbus_write_byte_data(file, 0x32, 0x10);
	i2c_smbus_write_byte_data(file, 0x33, 0x00);
	i2c_smbus_write_byte_data(file, 0x30, 0x0A);
}

// Read the data of two 8-bit registers and compile into one 16-bit value
// register_address is the first (high) register, register_address-1 is the low register
// E.g., if the two registers contain the 8-bit values 0x01 and 0x02, this
// function returns the value 0x0102
int read_raw_data(int file, unsigned char register_address) {
	high = i2c_smbus_read_byte_data(file, register_address);
	low = i2c_smbus_read_byte_data(file, register_address-1);

	value = (high << 8 | low);

	//     This converts it from an unsigned 0-63355 value
	//     to a signed value between -32769 and 32768
	if (value > 32768)
		value = value - 65536;

	return value;
}
