#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

struct proc_dir_entry *dirent;
DEFINE_SPINLOCK(test_lock);

static int nmi_wdt_write_proc(struct file *file, const char __user *buffer,
					unsigned long count, void *data)
{
	int flags;
	spin_lock_irqsave(&test_lock, flags);
	spin_lock(&test_lock);
	return count;
}


static int test_init(void)
{
	dirent = create_proc_entry("nmi_wdt", 0222, NULL);
	if (dirent == NULL)
		return -ENOMEM;

	dirent->write_proc = &nmi_wdt_write_proc;

	return 0;
}

static void test_exit(void)
{
	remove_proc_entry("nmi_wdt", NULL);
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");
