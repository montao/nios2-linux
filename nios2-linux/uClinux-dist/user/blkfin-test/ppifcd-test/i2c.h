#ifndef I2C_H
#define I2C_H
int i2c_write_register(const char *, unsigned char, unsigned char, unsigned short);
int i2c_read_register(const char *, unsigned char, unsigned char);
void i2c_dump_register(const char *, unsigned char, unsigned short, unsigned short);
void i2c_scan_bus(const char *);
#endif
