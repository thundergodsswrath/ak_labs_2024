#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_AUTHOR("IhorSS <igorskor0912@gmail.com>");
MODULE_DESCRIPTION("Hello, World in Linux Kernel Module");
MODULE_LICENSE("Dual BSD/GPL");

static uint print_count = 1;
module_param(print_count, uint, 0444);
MODULE_PARM_DESC(print_count, "Number of 'Hello, world!' messages to print");

struct hello_entry {
	struct list_head node;
	ktime_t timestamp;
};

static LIST_HEAD(hello_entries);

static void cleanup_list(void)
{
	struct hello_entry *entry, *next;

	list_for_each_entry_safe(entry, next, &hello_entries, node) {
		pr_info("Releasing entry with timestamp: %lld ns\n",
			ktime_to_ns(entry->timestamp));
		list_del(&entry->node);
		kfree(entry);
	}
}

static int __init hello_module_init(void)
{
	int i;
	struct hello_entry *entry;

	if (print_count == 0) {
		pr_warn("print_count is set to 0. Skipping message printing.\n");
		return 0;
	}

	if (print_count > 10) {
		pr_err("print_count exceeds 10. Aborting module load.\n");
		return -EINVAL;
	}

	if (print_count >= 5)
		pr_warn("print_count is %u. Consider lowering this value.\n",
			print_count);

	for (i = 0; i < print_count; i++) {
		entry = kmalloc(sizeof(*entry), GFP_KERNEL);
		if (!entry) {
			cleanup_list();
			return -ENOMEM;
		}

		entry->timestamp = ktime_get();
		list_add_tail(&entry->node, &hello_entries);
		pr_info("Hello, world! (%d/%u)\n", i + 1, print_count);
	}

	pr_info("Module initialized successfully with %u messages.\n",
		print_count);
	return 0;
}

static void __exit hello_module_exit(void)
{
	pr_info("Cleaning up and unloading module...\n");
	cleanup_list();
	pr_info("Module unloaded cleanly.\n");
}

module_init(hello_module_init);
module_exit(hello_module_exit);
