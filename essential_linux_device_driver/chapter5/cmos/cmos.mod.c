#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xfd5f6819, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xbd96fb78, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x7c61340c, __VMLINUX_SYMBOL_STR(__release_region) },
	{ 0xae6ea92, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xed720d5f, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xccf508a3, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xcc47ae, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xcdfdd5c2, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x1fedf0f4, __VMLINUX_SYMBOL_STR(__request_region) },
	{ 0xff7559e4, __VMLINUX_SYMBOL_STR(ioport_resource) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xe344f121, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x9b29ea68, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x28b3b2f3, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "DD343BB64586EA398358AAC");
