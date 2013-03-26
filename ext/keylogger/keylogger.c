#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <ruby.h>
#include <ruby/io.h>

VALUE rb_mKeylogger = Qnil;

static
VALUE kl_close_fd(VALUE vfd)
{
    close(FIX2INT(vfd));
    return Qnil;
}

static
VALUE kl_read_loop(VALUE vfd)
{
    int fd = FIX2INT(vfd);

    int ret, done;
    struct input_event ev;

    int brek = 0;
    while(!brek) {
        
        done = 0;
        rb_thread_wait_fd(fd);

        while(!done) {

            ret = read(fd, &ev, sizeof(struct input_event));

            if(ret == -1) {

                if (rb_io_wait_readable(fd) == Qfalse)
                    rb_raise(rb_eStandardError, "read error");

            } else {

                if(ev.type == 1) {
                    if(rb_yield_values(2, INT2FIX(ev.code), INT2FIX(ev.value)) != Qtrue)
                        brek = 1;
                }
                done = 1;

            }
        }
    }

    return Qnil;
}

static
VALUE kl_listen(VALUE self)
{
    if(!rb_block_given_p())
        rb_raise(rb_eStandardError, "missing block");

    char dev[512];
    int kbd_found = 0;

    static const char* p = "/dev/input/by-id/";
    
    DIR *dp;
    struct dirent *dent;

    dp = opendir(p);
    if(dp == 0)
        rb_raise(rb_eStandardError, "failed to read /dev/input/by-id");

    strncpy(dev, p, 17);

    while((dent = readdir(dp)) != 0) {
        if(strstr(dent->d_name, "kbd") != NULL) {
            strncpy(dev + 17, dent->d_name, 512 - 17);
            kbd_found = 1;
            break;
        }
    }

    closedir(dp);

    if(!kbd_found)
        rb_raise(rb_eStandardError, "no keyboard found");
    
    int fd;
    fd = open(dev, O_RDONLY);
    if(!fd)
        rb_raise(rb_eStandardError, "failed to open device");
   
    rb_ensure(kl_read_loop, INT2FIX(fd), kl_close_fd, INT2FIX(fd));

    return Qnil;
}

void Init_keylogger()
{
    rb_mKeylogger  = rb_define_module("Keylogger");
    rb_define_singleton_method(rb_mKeylogger, "listen", kl_listen, 0);
}
