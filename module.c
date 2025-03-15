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

static void create_children(struct task_info *parent, int level) {
    int i ;
    for (i = 0; i < MAX_CHILDREN; i++) {
        struct task_struct *task = kthread_run(child_function, NULL, "module_child");
        if (IS_ERR(task)) {
            printk(KERN_ERR "Failed to create child process\n");
            continue;
        }
        struct task_info *task_info = kmalloc(sizeof(*task_info), GFP_KERNEL);
        if (!task_info) {
            printk(KERN_ERR "Failed to allocate memory for task info\n");
            kthread_stop(task);
            continue;
        }
        task_info->pid = task->pid;
        task_info->task = task;
        task_info->parent = parent;
        INIT_LIST_HEAD(&task_info->list);
        list_add_tail(&task_info->list, &task_list);
        printk(KERN_INFO "Created child process with PID: %d (Parent PID: %d, Level: %d)\n", 
            task->pid, parent ? parent->pid : -1, level);
        if (level < MAX_CHILDREN - 1) {
            create_children(task_info, level + 1);
        }
    }
}

static void log_memory_maps(struct task_info *task_info, int level) {
    struct task_struct *task_struct;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    if (!task_info) {
        return;
    }
    task_struct = task_info->task;
    if (!task_struct) {
        printk(KERN_ERR "Failed to find task_struct for PID: %d\n", task_info->pid);
        return;
    }
    mm = get_task_mm(task_struct);
    if (!mm) {
        printk(KERN_ERR "No memory map for PID: %d\n", task_info->pid);
        return;
    }
    printk(KERN_INFO "Level %d: Memory map for PID: %d (Parent ID: %d)\n", level, 
        task_info->pid, task_info->parent ? task_info->parent->pid : -1);
    mmap_read_lock(mm);
    VMA_ITERATOR(vmi, mm, 0);
    for_each_vma(vmi, vma) {
        printk(KERN_INFO "  Level %d: Range: %lx-%lx, Permissions: %c%c%c\n", level, 
            vma->vm_start, vma->vm_end, 
            (vma->vm_flags & VM_READ) ? 'r' : '-', 
            (vma->vm_flags & VM_WRITE) ? 'w' : '-', 
            (vma->vm_flags & VM_EXEC) ? 'x' : '-');
    }
    mmap_read_unlock(mm);
    mmput(mm);
    struct task_info *child;
    list_for_each_entry(child, &task_list, list) {
        if (child->parent == task_info) {
            log_memory_maps(child, level + 1);
        }
    }
}

static void create_birthday_list(void) {
    int i;
    for (i = 0; i < 5; i++) {
        struct birthday *person = kmalloc(sizeof(*person), GFP_KERNEL);
        if (!person) {
            printk(KERN_ERR "Failed to allocate memory for birthday struct\n");
            continue;
        }
        person->day = i + 1;
        person->month = i + 1;
        person->year = 1990 + i;
        INIT_LIST_HEAD(&person->list);
        list_add_tail(&person->list, &birthday_list);
        printk(KERN_INFO "Added birthday: %d/%d/%d\n", person->day, person->month, person->year);
    }
}

static void log_birthday_list(void) {
    struct birthday *person;
    list_for_each_entry(person, &birthday_list, list) {
        printk(KERN_INFO "Birthday: %d/%d/%d\n", person->day, person->month, person->year);
    }
}

static void free_birthday_list(void) {
    struct birthday *person, *next;
    list_for_each_entry_safe(person, next, &birthday_list, list) {
        list_del(&person->list);
        kfree(person);
        printk(KERN_INFO "Removed birthday: %d/%d/%d\n", person->day, person->month, person->year);
    }
}

static int __init mod_init(void) {
    printk(KERN_INFO "Loading Module\n");
    struct task_info *root = kmalloc(sizeof(*root), GFP_KERNEL);
    if (!root) {
        printk(KERN_ERR "Failed to allocate memory for root process\n");
        return -ENOMEM;
    }
    root->pid = current->pid;
    root->task = current;
    root->parent = NULL;
    INIT_LIST_HEAD(&root->list);
    list_add_tail(&root->list, &task_list);
    printk(KERN_INFO "Created root process with PID: %d (Level: 0)\n", root->pid);
    create_children(root, 1);
    log_memory_maps(root, 0);
    create_birthday_list();
    log_birthday_list();
    printk(KERN_INFO "Module successfully loaded\n");
    return 0;
}

static void __exit mod_exit(void) {
    struct task_info *task_info, *next;
    list_for_each_entry_safe(task_info, next, &task_list, list) {
        if (task_info->task) {
            kthread_stop(task_info->task);
        }
        list_del(&task_info->list);
        kfree(task_info);
        printk(KERN_INFO "Removed child process with PID: %d\n", task_info->pid);
    }
    free_birthday_list();
    printk(KERN_INFO "Removing Module\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LINUX KERNEL MODULE");
MODULE_AUTHOR("LALITH REDDY");
