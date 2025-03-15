#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/kthread.h>
#include <linux/sched/mm.h>

#define MAX_CHILDREN 5

struct task_info {
    pid_t pid;
    struct task_struct *task;
    struct task_info *parent;
    struct list_head list;
};

struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
};

static LIST_HEAD(task_list);
static LIST_HEAD(birthday_list);

static int child_function(void *data) {
    while (!kthread_should_stop()) {
        schedule();
    }
    return 0;
}