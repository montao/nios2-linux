cmd_kernel/printk.o := nios2-linux-gnu-gcc -Wp,-MD,kernel/.printk.o.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h  -I/home/developer/altera/nios2-linux/linux-2.6/kernel -Ikernel -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -pipe -D__linux__ -D__ELF__ -mhw-mul -mno-hw-mulx -mno-hw-div -fno-optimize-sibling-calls -DUTS_SYSNAME=\"Linux\" -fno-builtin -G 0 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(printk)"  -D"KBUILD_MODNAME=KBUILD_STR(printk)" -c -o kernel/printk.o /home/developer/altera/nios2-linux/linux-2.6/kernel/printk.c

source_kernel/printk.o := /home/developer/altera/nios2-linux/linux-2.6/kernel/printk.c

deps_kernel/printk.o := \
    $(wildcard include/config/default/message/loglevel.h) \
    $(wildcard include/config/a11y/braille/console.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/have/efficient/unaligned/access.h) \
    $(wildcard include/config/log/buf/shift.h) \
    $(wildcard include/config/kexec.h) \
    $(wildcard include/config/boot/printk/delay.h) \
    $(wildcard include/config/security/dmesg/restrict.h) \
    $(wildcard include/config/printk/time.h) \
    $(wildcard include/config/kgdb/kdb.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sysinfo.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  arch/nios2/include/generated/asm/types.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/types.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/int-ll64.h \
  arch/nios2/include/generated/asm/bitsperlong.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitsperlong.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/posix_types.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/stddef.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler-gcc4.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/posix_types.h \
    $(wildcard include/config/mmu.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/posix_types.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include/stdarg.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/bitops.h \
  arch/nios2/include/generated/asm/bitops.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/typecheck.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/irqflags.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/registers.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/__ffs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/ffz.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/fls.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/__fls.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/fls64.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/sched.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/ffs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/hweight.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/arch_hweight.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/const_hweight.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/lock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/atomic.h \
    $(wildcard include/config/smp.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/non-atomic.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/le.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/byteorder.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/byteorder/little_endian.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/swab.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/swab.h \
    $(wildcard include/config/nios2/ci/swab/support.h) \
    $(wildcard include/config/nios2/ci/swab/no.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/swab.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/byteorder/generic.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitops/ext2-atomic.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/printk.h \
    $(wildcard include/config/dynamic/debug.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dynamic_debug.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/string.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/errno.h \
  arch/nios2/include/generated/asm/errno.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/errno.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/errno-base.h \
  arch/nios2/include/generated/asm/div64.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/div64.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/mm.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/cma.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/have/memblock/node.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  arch/nios2/include/generated/asm/bug.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/thread_info.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/const.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/stringify.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/bottom_half.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/barrier.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/cmpxchg.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/cmpxchg.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/cmpxchg-local.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/spinlock_types.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/spinlock_types_up.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rwlock_types.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/spinlock_up.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/processor.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/ptrace.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rwlock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/spinlock_api_up.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  arch/nios2/include/generated/asm/atomic.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/atomic.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/atomic-long.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/atomic64.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/wait.h \
  arch/nios2/include/generated/asm/current.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/current.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/cache.h \
    $(wildcard include/config/nios2/dcache/size.h) \
    $(wildcard include/config/nios2/icache/size.h) \
    $(wildcard include/config/nios2/dcache/line/size.h) \
    $(wildcard include/config/nios2/icache/line/size.h) \
    $(wildcard include/config/nios2/icache/line/shift.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/seqlock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/nodemask.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/bitmap.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/generated/bounds.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/page.h \
    $(wildcard include/config/mem/base.h) \
    $(wildcard include/config/kernel/region/base.h) \
    $(wildcard include/config/io/region/base.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pfn.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/memory_model.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/getorder.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/notifier.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rwsem-spinlock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/srcu.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/preempt/rt.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/completion.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rcutiny.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/math64.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/jiffies.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/timex.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/param.h \
  arch/nios2/include/generated/asm/param.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/timex.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/timex.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/sched/book.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  arch/nios2/include/generated/asm/percpu.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/percpu.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  arch/nios2/include/generated/asm/topology.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/topology.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rbtree.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/prio_tree.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/mmu/notifier.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/auxvec.h \
  arch/nios2/include/generated/asm/auxvec.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/auxvec.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/guard.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/uprobes.h \
    $(wildcard include/config/arch/supports/uprobes.h) \
    $(wildcard include/config/uprobes.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/mmu.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/range.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/bit_spinlock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/shrinker.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/pgtable.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/pgtable_mm.h \
    $(wildcard include/config/kernel/mmu/region/base.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/io.h \
    $(wildcard include/config/cc/optimize/for/size.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/pgtable-bits.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/cacheflush.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/tlbflush.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/pgtable-nopmd.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/pgtable-nopud.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/pgtable.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/s390.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/huge_mm.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/vm_event_item.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/tty.h \
    $(wildcard include/config/audit.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/fs.h \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/limits.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ioctl.h \
  arch/nios2/include/generated/asm/ioctl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/ioctl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kdev_t.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dcache.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rculist.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rculist_bl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/list_bl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/path.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/stat.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/stat.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/uidgid.h \
    $(wildcard include/config/uidgid/strict/type/checks.h) \
    $(wildcard include/config/user/ns.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/highuid.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/radix-tree.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pid.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/capability.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/semaphore.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/fiemap.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/migrate_mode.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/percpu_counter.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dqblk_xfs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dqblk_v1.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dqblk_v2.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dqblk_qtree.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/nfs_fs_i.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/fcntl.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/fcntl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/fcntl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/err.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/major.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/termios.h \
  arch/nios2/include/generated/asm/termios.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/termios.h \
  arch/nios2/include/generated/asm/termbits.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/termbits.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/ioctls.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/ioctls.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/uaccess.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/tty_driver.h \
    $(wildcard include/config/console/poll.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/export.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cdev.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kobject.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sysfs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kobject_ns.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kref.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/tty_ldisc.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pps_kernel.h \
    $(wildcard include/config/ntp/pps.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pps.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ioport.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/klist.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pm.h \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ratelimit.h \
  arch/nios2/include/generated/asm/device.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/device.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pm_wakeup.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/console.h \
    $(wildcard include/config/hw/console.h) \
    $(wildcard include/config/vga/console.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/nmi.h \
    $(wildcard include/config/have/nmi/watchdog.h) \
    $(wildcard include/config/hardlockup/detector.h) \
    $(wildcard include/config/lockup/detector.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/cfs/bandwidth.h) \
    $(wildcard include/config/cgroup/sched.h) \
  arch/nios2/include/generated/asm/cputime.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/cputime.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sem.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ipc.h \
  arch/nios2/include/generated/asm/ipcbuf.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/ipcbuf.h \
  arch/nios2/include/generated/asm/sembuf.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/sembuf.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/signal.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/signal.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/signal-defs.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/sigcontext.h \
  arch/nios2/include/generated/asm/siginfo.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/siginfo.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/proportions.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/seccomp/filter.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/resource.h \
  arch/nios2/include/generated/asm/resource.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/resource.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/timerqueue.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/latencytop.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/key.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sysctl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/aio.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/aio_abi.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/uio.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/irq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/irq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irqdomain.h \
    $(wildcard include/config/irq/domain.h) \
    $(wildcard include/config/of/irq.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/module.h \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kmod.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/elf.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/elf-em.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/elf.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ppc64.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/tracepoint.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/static_key.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  arch/nios2/include/generated/asm/module.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/module.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/interrupt.h \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irqreturn.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irqnr.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/hardirq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  arch/nios2/include/generated/asm/hardirq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/hardirq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irq_cpustat.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irq.h \
    $(wildcard include/config/generic/pending/irq.h) \
  arch/nios2/include/generated/asm/irq_regs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/irq_regs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irqdesc.h \
    $(wildcard include/config/irq/preflow/fasteoi.h) \
    $(wildcard include/config/sparse/irq.h) \
  arch/nios2/include/generated/asm/hw_irq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/hw_irq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/delay.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/delay.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/security.h \
    $(wildcard include/config/security/path.h) \
    $(wildcard include/config/security/network.h) \
    $(wildcard include/config/security/network/xfrm.h) \
    $(wildcard include/config/securityfs.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/slab.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/slab_def.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kmalloc_sizes.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/bootmem.h \
    $(wildcard include/config/have/arch/bootmem/node.h) \
    $(wildcard include/config/have/arch/alloc/remap.h) \
  arch/nios2/include/generated/asm/dma.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/dma.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/memblock.h \
    $(wildcard include/config/have/memblock.h) \
    $(wildcard include/config/arch/discard/memblock.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/syscalls.h \
    $(wildcard include/config/ftrace/syscalls.h) \
    $(wildcard include/config/mips.h) \
    $(wildcard include/config/have/syscall/wrappers.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/unistd.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/unistd.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/trace/syscall.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ftrace_event.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ring_buffer.h \
    $(wildcard include/config/ring/buffer/allow/swap.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kmemcheck.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/seq_file.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/trace_seq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/perf_event.h \
    $(wildcard include/config/cgroup/perf.h) \
    $(wildcard include/config/function/tracer.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pid_namespace.h \
    $(wildcard include/config/pid/ns.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/nsproxy.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ftrace.h \
    $(wildcard include/config/dynamic/ftrace.h) \
    $(wildcard include/config/stack/tracer.h) \
    $(wildcard include/config/frame/pointer.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/trace_clock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kallsyms.h \
  arch/nios2/include/generated/asm/ftrace.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/ftrace.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cpu.h \
    $(wildcard include/config/arch/has/cpu/autoprobe.h) \
    $(wildcard include/config/arch/cpu/probe/release.h) \
    $(wildcard include/config/pm/sleep/smp.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/node.h \
    $(wildcard include/config/memory/hotplug/sparse.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irq_work.h \
  arch/nios2/include/generated/asm/local.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/local.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kexec.h \
    $(wildcard include/config/.h) \
    $(wildcard include/config/kexec/jump.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kdb.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kmsg_dump.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/syslog.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/poll.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/poll.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/poll.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/trace/events/printk.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/trace/define_trace.h \

kernel/printk.o: $(deps_kernel/printk.o)

$(deps_kernel/printk.o):
