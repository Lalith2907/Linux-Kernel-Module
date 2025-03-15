#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x34b27607, "kmalloc_trace" },
	{ 0x122c3a7e, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x282cf8bd, "kthread_stop" },
	{ 0x1000e51, "schedule" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x37a0cba, "kfree" },
	{ 0x7520f13b, "get_task_mm" },
	{ 0xb018650a, "__tracepoint_mmap_lock_start_locking" },
	{ 0x668b19a1, "down_read" },
	{ 0x26fa1b, "__tracepoint_mmap_lock_acquire_returned" },
	{ 0x87b30034, "mas_find" },
	{ 0x4e5993d2, "__tracepoint_mmap_lock_released" },
	{ 0x53b954a2, "up_read" },
	{ 0xc5d19c39, "mmput" },
	{ 0x637e4831, "__mmap_lock_do_trace_released" },
	{ 0x50f3d674, "__mmap_lock_do_trace_acquire_returned" },
	{ 0x17370caf, "__mmap_lock_do_trace_start_locking" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xff9343cd, "const_pcpu_hot" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xb53470a3, "kthread_create_on_node" },
	{ 0x32ab0f12, "wake_up_process" },
	{ 0x4c03a563, "random_kmalloc_seed" },
	{ 0x558548ea, "kmalloc_caches" },
	{ 0xd28dd47a, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "DA0B3AF61DC6C63606A0700");
