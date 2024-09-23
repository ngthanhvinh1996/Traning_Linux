#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

#define DEV_MEM_SIZE 512U

/* pseudo device's memory */
char device_buffer[DEV_MEM_SIZE];

/* This holds the device number */
static dev_t device_number;

/* Cdev variable */
struct cdev pcd_cdev;

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence);
ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);
int pcd_open(struct inode *inode, struct file *filp);
int pcd_release(struct inode *inode, struct file *filp);

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
    loff_t temp;

    pr_info("lseek requested\n");
    pr_info("current value of file position == %lld\n", filp->f_pos);

    switch (whence)
    {
        case SEEK_SET:
            if((DEV_MEM_SIZE < offset) || (0 > offset))
            {
                return -EINVAL;
            }
            filp->f_pos = offset;
            break;
        case SEEK_CUR:
            temp = filp->f_pos + offset;
            if((DEV_MEM_SIZE < temp) || (0 > temp))
            {
                return -EINVAL;
            }
            filp->f_pos = temp;
            break;
        case SEEK_END:
            temp = DEV_MEM_SIZE + offset;
            if((DEV_MEM_SIZE < temp) || (0 > temp))
            {
                return -EINVAL;
            }
            filp->f_pos = temp;
            break;
        default:
            return - EINVAL;
            break;
    }

    pr_info("new value of the file position = %lld\n", filp->f_pos);
    return filp->f_pos;
}

ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("Read requested for %zu bytes\n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    /* 1. Adjust the 'count' */
    if(DEV_MEM_SIZE < (*f_pos + count))
    {
        count = DEV_MEM_SIZE - *f_pos;
    }

    /* 2. Copy to user */
    if(0U != copy_to_user(buff, &device_buffer[*f_pos], count))
    {
        return -EFAULT;
    }

    /* 3. Update the current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully read = %zu\n", count);
    pr_info("Update file position = %lld\n", *f_pos);

    /* 4. Return number of bytes which have been successfully read */
    return count;
}

ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("Write requested for %zu bytes\n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    /* 1. Adjust the 'count' */
    if(DEV_MEM_SIZE < (*f_pos + count))
    {
        count = DEV_MEM_SIZE - *f_pos;
    }

    if(0 == count)
    {
        return -ENOMEM;
    }

    /* 2. Copy from user */
    if(0U != copy_from_user(&device_buffer[*f_pos], buff, count))
    {
        return -EFAULT;
    }

    /* 3. Update the current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully write = %zu\n", count);
    pr_info("Update file position = %lld\n", *f_pos);

    /* 4. Return number of bytes which have been successfully write */
    return count;
}

int pcd_open(struct inode *inode, struct file *filp)
{
    pr_info("open was successful\n");
    return 0;
}

int pcd_release(struct inode *inode, struct file *filp)
{
    pr_info("close was successful\n");
    return 0;
}

/* File operations of the driver */
struct file_operations pcd_fops = {
    .open = pcd_open,
    .write = pcd_write,
    .read = pcd_read,
    .llseek = pcd_lseek,
    .release = pcd_release,
    .owner = THIS_MODULE,
};

struct class *class_pcd;

struct device *device_pcd;

static int __init pcd_driver_init(void)
{
    int ret;

    /* 1. Dynamically allocate a device number */
    ret = alloc_chrdev_region(&device_number, 0, 1, "pcd_devices");
    if(0U > ret)
    {
        pr_err("Alloc chrdev failed\n");
        goto __error;
    }
    pr_info("Device number <major>:<minor> = %d:%d\n", MAJOR(device_number), MINOR(device_number));

    /* 2. Initialize the cdev structure with fops */
    cdev_init(&pcd_cdev, &pcd_fops);

    /* 3. Register a device (cdev structure) with VFS */
    pcd_cdev.owner = THIS_MODULE;
    ret = cdev_add(&pcd_cdev, device_number, 1);
    if(0 > ret)
    {
        pr_err("Cdev add failed\n");
        goto __unreg_chrdev;
    }

    /* 4. Create device class under /sys/class/ */
    // class_pcd = class_create(THIS_MODULE, "pcd_class");
    class_pcd = class_create("pcd_class");
    if(IS_ERR(class_pcd))
    {
        pr_err("Class creation failed\n");
        ret = PTR_ERR(class_pcd);
        goto __cdev_del;
    }

    /* 5. Populate the sysfs with device information */
    device_pcd = device_create(class_pcd, NULL, device_number, NULL, "pcd");
    if(IS_ERR(device_pcd))
    {
        pr_err("Device creation failed\n");
        ret = PTR_ERR(device_pcd);
        goto __class_del;
    }
    pr_info("Module init was successful\n");

    return 0;

__class_del:
    class_destroy(class_pcd);

__cdev_del:
    cdev_del(&pcd_cdev);

__unreg_chrdev:
    unregister_chrdev_region(device_number, 1U);

__error:
    pr_info("Module insertion failed\n");
    return ret;
}

static void __exit pcd_driver_cleanup(void)
{
    device_destroy(class_pcd, device_number);

    class_destroy(class_pcd);

    cdev_del(&pcd_cdev);

    unregister_chrdev_region(device_number, 1);

    pr_info("module unloaded\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VINHNT");
MODULE_DESCRIPTION("A simple pseudo char driver");
MODULE_INFO(board, "Beagle Bone Black Rev C");
