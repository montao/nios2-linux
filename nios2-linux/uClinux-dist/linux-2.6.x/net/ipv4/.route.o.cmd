cmd_net/ipv4/route.o := nios2-linux-gnu-gcc -Wp,-MD,net/ipv4/.route.o.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h  -I/home/developer/altera/nios2-linux/linux-2.6/net/ipv4 -Inet/ipv4 -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -pipe -D__linux__ -D__ELF__ -mhw-mul -mno-hw-mulx -mno-hw-div -fno-optimize-sibling-calls -DUTS_SYSNAME=\"Linux\" -fno-builtin -G 0 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(route)"  -D"KBUILD_MODNAME=KBUILD_STR(route)" -c -o net/ipv4/route.o /home/developer/altera/nios2-linux/linux-2.6/net/ipv4/route.c

source_net/ipv4/route.o := /home/developer/altera/nios2-linux/linux-2.6/net/ipv4/route.c

deps_net/ipv4/route.o := \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/ip/route/classid.h) \
    $(wildcard include/config/ip/route/verbose.h) \
    $(wildcard include/config/ip/multiple/tables.h) \
    $(wildcard include/config/ip/mroute.h) \
    $(wildcard include/config/ip/route/multipath.h) \
    $(wildcard include/config/xfrm.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/module.h \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
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
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/const.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/stat.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/stat.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sysinfo.h \
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
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/init.h \
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
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/cache.h \
    $(wildcard include/config/nios2/dcache/size.h) \
    $(wildcard include/config/nios2/icache/size.h) \
    $(wildcard include/config/nios2/dcache/line/size.h) \
    $(wildcard include/config/nios2/icache/line/size.h) \
    $(wildcard include/config/nios2/icache/line/shift.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/seqlock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/spinlock.h \
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
  arch/nios2/include/generated/asm/bug.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/thread_info.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/stringify.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/bottom_half.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/barrier.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/cmpxchg.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/cmpxchg.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/cmpxchg-local.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/spinlock_types.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/spinlock_types_up.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/lockdep.h \
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
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/math64.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/uidgid.h \
    $(wildcard include/config/uidgid/strict/type/checks.h) \
    $(wildcard include/config/user/ns.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/highuid.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kmod.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/cma.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/have/memblock/node.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/wait.h \
  arch/nios2/include/generated/asm/current.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/current.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
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
    $(wildcard include/config/sparsemem/vmemmap.h) \
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
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sysctl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rbtree.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/elf.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/elf-em.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/elf.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kobject.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sysfs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kobject_ns.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kref.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/tracepoint.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/static_key.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/export.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
  arch/nios2/include/generated/asm/module.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/module.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/uaccess.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/mm.h \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
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
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/bootmem.h \
    $(wildcard include/config/have/arch/bootmem/node.h) \
    $(wildcard include/config/have/arch/alloc/remap.h) \
  arch/nios2/include/generated/asm/dma.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/dma.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/socket.h \
  arch/nios2/include/generated/asm/socket.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/socket.h \
  arch/nios2/include/generated/asm/sockios.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/sockios.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sockios.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/uio.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/in.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/inet.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/netdevice.h \
    $(wildcard include/config/dcb.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/ax25.h) \
    $(wildcard include/config/mac80211/mesh.h) \
    $(wildcard include/config/tr.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/rps.h) \
    $(wildcard include/config/netpoll.h) \
    $(wildcard include/config/xps.h) \
    $(wildcard include/config/bql.h) \
    $(wildcard include/config/rfs/accel.h) \
    $(wildcard include/config/fcoe.h) \
    $(wildcard include/config/net/poll/controller.h) \
    $(wildcard include/config/libfcoe.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/vlan/8021q.h) \
    $(wildcard include/config/net/dsa.h) \
    $(wildcard include/config/net/ns.h) \
    $(wildcard include/config/netprio/cgroup.h) \
    $(wildcard include/config/net/dsa/tag/dsa.h) \
    $(wildcard include/config/net/dsa/tag/trailer.h) \
    $(wildcard include/config/netpoll/trap.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/if.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/hdlc/ioctl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/if_ether.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/skbuff.h \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/nf/defrag/ipv4.h) \
    $(wildcard include/config/nf/defrag/ipv6.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/net/dma.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/network/phy/timestamping.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kmemcheck.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/net.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/random.h \
    $(wildcard include/config/arch/random.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ioctl.h \
  arch/nios2/include/generated/asm/ioctl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/ioctl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irqnr.h \
    $(wildcard include/config/generic/hardirqs.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/fcntl.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/fcntl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/fcntl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/textsearch.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/err.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/slab.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/slab_def.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kmalloc_sizes.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/checksum.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/checksum.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dmaengine.h \
    $(wildcard include/config/async/tx/enable/channel/switch.h) \
    $(wildcard include/config/rapidio/dma/engine.h) \
    $(wildcard include/config/dma/engine.h) \
    $(wildcard include/config/async/tx/dma.h) \
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
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  arch/nios2/include/generated/asm/scatterlist.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/timerqueue.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/arch/has/dma/set/coherent/mask.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dma-attrs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dma-direction.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/dma-mapping.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/netdev_features.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/if_packet.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/if_link.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/netlink.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/capability.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pm_qos.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/miscdevice.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/major.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/delay.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/delay.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rculist.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dynamic_queue_limits.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ethtool.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compat.h \
    $(wildcard include/config/arch/want/old/compat/ipc.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/net_namespace.h \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/ip/dccp.h) \
    $(wildcard include/config/netfilter.h) \
    $(wildcard include/config/wext/core.h) \
    $(wildcard include/config/net.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/core.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/mib.h \
    $(wildcard include/config/xfrm/statistics.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/snmp.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/snmp.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/u64_stats_sync.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/unix.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/packet.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/ipv4.h \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/ip/mroute/multiple/tables.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/inet_frag.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/ipv6.h \
    $(wildcard include/config/ipv6/multiple/tables.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/ipv6/mroute/multiple/tables.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/dst_ops.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/percpu_counter.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/dccp.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/x_tables.h \
    $(wildcard include/config/bridge/nf/ebtables.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/netfilter.h \
    $(wildcard include/config/nf/nat/needed.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/in6.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/xfrm.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/xfrm.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/seq_file_net.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/seq_file.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/dsa.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netprio_cgroup.h \
    $(wildcard include/config/cgroups.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cgroup.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/epoll.h) \
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
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/block.h) \
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
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pid.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/proportions.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/seccomp/filter.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/resource.h \
  arch/nios2/include/generated/asm/resource.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/resource.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/latencytop.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/key.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/aio.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/aio_abi.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cgroupstats.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/taskstats.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/prio_heap.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/idr.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/hardirq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  arch/nios2/include/generated/asm/hardirq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/hardirq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irq_cpustat.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irq.h \
    $(wildcard include/config/generic/pending/irq.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irqreturn.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/irq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/irq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irqdomain.h \
    $(wildcard include/config/irq/domain.h) \
    $(wildcard include/config/of/irq.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/radix-tree.h \
  arch/nios2/include/generated/asm/irq_regs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/irq_regs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/irqdesc.h \
    $(wildcard include/config/irq/preflow/fasteoi.h) \
    $(wildcard include/config/sparse/irq.h) \
  arch/nios2/include/generated/asm/hw_irq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/hw_irq.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/neighbour.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/proc_fs.h \
    $(wildcard include/config/proc/devicetree.h) \
    $(wildcard include/config/proc/kcore.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/fs.h \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/limits.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kdev_t.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dcache.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rculist_bl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/list_bl.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/path.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/semaphore.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/fiemap.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/migrate_mode.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dqblk_xfs.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dqblk_v1.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dqblk_v2.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/dqblk_qtree.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/nfs_fs_i.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/magic.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/inetdevice.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rtnetlink.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/if_addr.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/igmp.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pkt_sched.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/mroute.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pim.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/sock.h \
    $(wildcard include/config/cgroup/mem/res/ctlr/kmem.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/list_nulls.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/security.h \
    $(wildcard include/config/security/path.h) \
    $(wildcard include/config/security/network.h) \
    $(wildcard include/config/security/network/xfrm.h) \
    $(wildcard include/config/securityfs.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/uaccess.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/memcontrol.h \
    $(wildcard include/config/cgroup/mem/res/ctlr/swap.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/res_counter.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/filter.h \
    $(wildcard include/config/bpf/jit.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/rculist_nulls.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/poll.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/poll.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/poll.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/dst.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/neighbour.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/rtnetlink.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netlink.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/netfilter_ipv4.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/jhash.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/unaligned/packed_struct.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/times.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/prefetch.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/protocol.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/ip.h \
    $(wildcard include/config/inet.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ip.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/inet_sock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/flow.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/request_sock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netns/hash.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/route.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/inetpeer.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/ipv6.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ipv6.h \
    $(wildcard include/config/ipv6/privacy.h) \
    $(wildcard include/config/ipv6/router/pref.h) \
    $(wildcard include/config/ipv6/route/info.h) \
    $(wildcard include/config/ipv6/optimistic/dad.h) \
    $(wildcard include/config/ipv6/mip6.h) \
    $(wildcard include/config/ipv6/subtrees.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/icmpv6.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/tcp.h \
    $(wildcard include/config/tcp/md5sig.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/inet_connection_sock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/inet_timewait_sock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/tcp_states.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/timewait_sock.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/udp.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/if_inet6.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/ndisc.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/in_route.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/route.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/ip_fib.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/fib_rules.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/fib_rules.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/arp.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/if_arp.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/tcp.h \
    $(wildcard include/config/syn/cookies.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/crypto.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/cryptohash.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/inet_hashtables.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/interrupt.h \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/vmalloc.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/inet_ecn.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/dsfield.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/icmp.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/icmp.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/xfrm.h \
    $(wildcard include/config/xfrm/sub/policy.h) \
    $(wildcard include/config/xfrm/migrate.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pfkeyv2.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ipsec.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/audit.h \
    $(wildcard include/config/change.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ptrace.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/ip6_fib.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/ipv6_route.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/netevent.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/kmemleak.h \
    $(wildcard include/config/debug/kmemleak.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/net/secure_seq.h \

net/ipv4/route.o: $(deps_net/ipv4/route.o)

$(deps_net/ipv4/route.o):
