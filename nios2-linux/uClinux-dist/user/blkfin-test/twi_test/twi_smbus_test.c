#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>

/* AD5280 vcomm */
static unsigned char vcomm_value = 150;

#define AD5280_NAME "bf537-lq035-ad5280"

static int __devinit ad5280_probe(struct i2c_client *client,
					const struct i2c_device_id *id)
{
	int ret;
	unsigned char new_vcomm=0;

	if (!i2c_check_functionality(client->adapter,
					I2C_FUNC_SMBUS_BYTE_DATA)) {
		dev_err(&client->dev, "SMBUS Byte Data not Supported\n");
		return -EIO;
	}

	ret = i2c_smbus_write_byte_data(client, 0x00, vcomm_value);
	if(ret) {
		printk("TWI_SMBUS_TEST: i2c_smbus_write_byte_data fail: %d\n", ret);
		return ret;
	}

	new_vcomm = i2c_smbus_read_byte_data(client, 0x00);
	if(new_vcomm != vcomm_value) {
		printk("TWI_SMBUS_TEST: i2c_smbus_read_byte_data fails: %d\n", new_vcomm);
		return -1;
	}

	printk("TWI_SMBUS_TEST.....[PASS]\n");
	return 0;
}

static const struct i2c_device_id ad5280_id[] = {
	{AD5280_NAME, 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, ad5280_id);

static struct i2c_driver ad5280_driver = {
	.driver		= {
		.name	= AD5280_NAME,
		.owner	= THIS_MODULE,
	},
	.probe = ad5280_probe,
	.id_table = ad5280_id,
};

static int __init twi_test_init(void)
{

	printk("TWI smbus api test\n");
	i2c_add_driver(&ad5280_driver);

	return 0;
}

static void __exit twi_test_exit(void)
{
	i2c_del_driver(&ad5280_driver);
}

MODULE_LICENSE("GPL");

module_init(twi_test_init);
module_exit(twi_test_exit);
