UML is by Jeff Dike 
 >Copyright (C) 2000 - 2007 Jeff Dike (jdike@{linux.intel,addtoit}.com)
 >
 >Licensed under the GPL
 >
 >(all initially committed is work of Jeff Dike)
 
Configured via https://xeiaso.net/blog/howto-usermode-linux-2019-07-07
 
**Project goal: to record lines of text entered in the UML terminal and saving to an external file**

Located in /linux-5.1.16/arch/um/drivers is user_chan.c

**c_out** is the key pressed
```
int generic_read(int fd, char *c_out, void *unused)
{
        int n;
        n = read(fd, c_out, sizeof(*c_out));
        printk("fd: %d\n",fd);
        printk("c_out: %s\n",c_out); <--prints key pressed to terminal
        if (n > 0)
                return n;
        else if (errno == EAGAIN)
                return 0;
        else if (n == 0)
                return -EIO;
        return -errno;
}
```

