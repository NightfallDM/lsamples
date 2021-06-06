#include<linux/module.h>
#include<linux/debugfs.h>
#include<linux/types.h>
#include<linux/dcache.h>
#include<linux/fs.h>

static char night_message[32] = "test the debugfs\n";

const char *test_root_name = "test_debugfs";
static char *filename = "hello_night";
struct dentry *test_root, *test_file;

static int nightmg_open(struct inode *inode, struct file *filep){
	filep->private_data = inode->i_private;
	return 0;
}

ssize_t nightmg_read(struct file *filep,  char __user *userbuf, size_t count, loff_t *ppos){
	if (*ppos == 32)
		return 0;
	if (*ppos + count >= 32)
		count = 32 - *ppos;
	if(copy_to_user(userbuf, night_message + *ppos, count))
		return -EFAULT;
	*ppos += count;
	return count;
}

ssize_t nightmg_write(struct file *filep, const char __user *userbuf, size_t count, loff_t *ppos){
	if (*ppos >= 32)
		return 0;
	if (*ppos + count > 32)
		count = 32 - *ppos;
	if(copy_from_user(night_message + *ppos, userbuf, count))
		return -EFAULT;
	*ppos += count;
	return count;
}

struct file_operations nightmg_fops = {
	.owner = THIS_MODULE,
	.open = nightmg_open,
	.read = nightmg_read,
	.write = nightmg_write,
};

static void setup_debugfs(void){
	test_root = debugfs_create_dir(test_root_name, NULL);
	test_file = debugfs_create_file(filename, 0644, test_root, night_message, &nightmg_fops);
}

static int __init test_debugfs_init(void){
	setup_debugfs();
	return 0;
}

static void __exit test_debugfs_exit(void){
	debugfs_remove_recursive(test_root);
}

module_init(test_debugfs_init);
module_exit(test_debugfs_exit);
MODULE_LICENSE("GPL");
