#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "ledgpio"
#define LED_GPIO    17  // BCM17 (pin físico 11)

static dev_t dev_num;
static struct cdev led_cdev;

static int led_open(struct inode *i, struct file *f) { return 0; }
static int led_release(struct inode *i, struct file *f) { return 0; }

static ssize_t led_read(struct file *f, char __user *buf, size_t len, loff_t *off) {
    char val = gpio_get_value(LED_GPIO) ? '1' : '0';
    if (*off) return 0;
    if (copy_to_user(buf, &val, 1)) return -EFAULT;
    *off = 1;
    return 1;
}

static ssize_t led_write(struct file *f, const char __user *ubuf, size_t len, loff_t *off) {
    char buf[4] = {0};
    if (len > 3) len = 3;
    if (copy_from_user(buf, ubuf, len)) return -EFAULT;
    if      (strncmp(buf,"ON",2)==0)  gpio_set_value(LED_GPIO,1);
    else if (strncmp(buf,"OFF",3)==0) gpio_set_value(LED_GPIO,0);
    else    return -EINVAL;
    return len;
}

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = led_open,
    .release = led_release,
    .read    = led_read,
    .write   = led_write,
};

static int __init led_init(void) {
    int ret;
    ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
    if (ret<0) return ret;
    cdev_init(&led_cdev,&fops);
    ret = cdev_add(&led_cdev,dev_num,1);
    if (ret) { unregister_chrdev_region(dev_num,1); return ret; }
    ret = gpio_request(LED_GPIO,DEVICE_NAME);
    if (ret) { cdev_del(&led_cdev); unregister_chrdev_region(dev_num,1); return ret; }
    gpio_direction_output(LED_GPIO,0);
    pr_info("ledgpio: cargado major=%d\n",MAJOR(dev_num));
    return 0;
}

static void __exit led_exit(void) {
    gpio_set_value(LED_GPIO,0);
    gpio_free(LED_GPIO);
    cdev_del(&led_cdev);
    unregister_chrdev_region(dev_num,1);
    pr_info("ledgpio: descargado\n");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tu Nombre");
MODULE_DESCRIPTION("Driver carácter para LED GPIO");

