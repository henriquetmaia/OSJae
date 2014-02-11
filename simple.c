#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

typedef struct { 
    int day;
    int month;
    int year;
    struct list_head list;
} birthday;

static LIST_HEAD(birthday_list);

void birthday_init_tail(birthday *ptr, int d, int m, int y);

void birthday_init_tail(birthday *ptr, int d, int m, int y){
    ptr = (birthday*) kmalloc(sizeof(*ptr), GFP_KERNEL);
    ptr->day = d;
    ptr->month = m;
    ptr->year = y;
    INIT_LIST_HEAD(&ptr->list);
    list_add_tail(&ptr->list, &birthday_list);
}

/* This function is called when the module is loaded. */
int simple_init(void){
    birthday *zeroth, *first, *second, *third, *fourth;
    birthday *ptr;

    printk(KERN_INFO "Loading Module, creating birthday elements\n");

    birthday_init_tail(zeroth, 20, 2, 1988);
    birthday_init_tail(first, 19, 3, 1992);
    birthday_init_tail(second, 16, 4, 1990);
    birthday_init_tail(third, 10, 5, 1957);
    birthday_init_tail(fourth, 20, 9, 1991);

    list_for_each_entry(ptr, &birthday_list, list){
        printk(KERN_INFO "Birth-Month: %d\n", ptr->month);
    }

    printk(KERN_INFO "Finished Loading Module\n");

   return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
    birthday *ptr, *next;
    printk(KERN_INFO "Removing Module, freeing birthday elements\n");

    list_for_each_entry_safe(ptr, next, &birthday_list, list) { 
        list_del(&ptr->list);
        kfree(ptr);
    }

    list_for_each_entry(ptr, &birthday_list, list){
        printk(KERN_INFO "Birth-Month: %d\n", ptr->month);
    }
    printk(KERN_INFO "Finished Removing Module\n");

}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
