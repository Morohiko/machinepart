#include <linux/fs.h>     // file_opetaions, register_chrdev
#include <linux/kernel.h> // printk
#include <linux/module.h> // module_init, module_exit
#include <linux/slab.h>   // kmalloc, kfree

#define DEVICE_NAME "test"
#define DEVICE_MAJOR 77
#define DEVICE_MINOR 0
#define DEVICE_COUNT 1

static int device_major_number = -1;

static ssize_t test_read(struct file *f, char __user *user, size_t size,
                         loff_t *loff);
static ssize_t test_write(struct file *f, const char __user *user, size_t size,
                          loff_t *loff);

static char *buffer = NULL;
static size_t buffer_size = -1;

static struct file_operations operations = {.read = test_read,
                                            .write = test_write};

static ssize_t test_read(struct file *f, char __user *user, size_t size,
                         loff_t *loff) {
  size_t current_buffer_size = -1;
  int retval;

  printk("test_read");

  if (buffer == NULL || buffer_size <= 0) {
    printk("test_read: buffer is empty");
    return 0;
  }

  if (user == NULL || size < buffer_size) {
    printk("test_read: not enough space for read, or buffer is empty");
    return -EFAULT;
  }

  retval = copy_to_user(user, buffer, buffer_size);

  if (retval < 0) {
    printk("cannot copy to user");
    return -EFAULT;
  }

  current_buffer_size = buffer_size;
  buffer_size = 0;
  kfree(buffer);

  if (current_buffer_size < size) {
    current_buffer_size = size;
  }

  return current_buffer_size;
}

static ssize_t test_write(struct file *f, const char __user *user, size_t size,
                          loff_t *loff) {
  int retval;
  printk("test_write");

  if (buffer != NULL) {
    kfree(buffer);
  }

  buffer = kmalloc(size, GFP_USER);

  if (buffer == NULL) {
    printk("Cannot allocate memory");
    return -ENOMEM;
  }

  retval = copy_from_user(buffer, user, size);

  if (retval < 0) {
    printk("cannot copy to user");
    return -EFAULT;
  }
  buffer_size = size;
  printk("buffer_size = %zu, buffer = %s", buffer_size, buffer);

  return buffer_size;
}

static int test_init_module(void) {
  int retval = -1;
  printk("test_init_module");
  retval = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &operations);
  if (retval < 0) {
    printk(KERN_WARNING "cannot register device, error_code = %d", retval);
    return -retval;
  }
  device_major_number = retval;
  printk("device major number %d", device_major_number);
  return 0;
}

static void test_exit_module(void) {
  printk("test_exit_module");
  if (device_major_number > 0) {
    unregister_chrdev(device_major_number, DEVICE_NAME);
  }
}

module_init(test_init_module);
module_exit(test_exit_module);
