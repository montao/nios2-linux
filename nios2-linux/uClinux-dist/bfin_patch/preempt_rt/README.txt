This patch enables preempt-rt for blackfin - this patch will be sent to upstream preempt-rt tree, once the blackfin SMP fix patch(svn revision #8034, #8039) is in upstream.

To use the patch:

1. Checkout Blackfin Linux kernel svn r8602 (linux-2.6.33.2)

2. Apply preempt-rt patch: patch-2.6.33.1-rt11 from http://www.kernel.org/pub/linux/kernel/projects/rt/patch-2.6.33.1-rt11.bz2.
You will need to fix some trivial patch failure for linux-kernel/Makefile, linux-kernel/arch/blackfin/kernel/time.c, linux-kernel/kernel/irq/manager.c

3. Apply bfin_preempt-rt.patch

This patch has been tested on BF537-STAMP, BF561-ezkit(with SMP).

The cyclictest shows there is huge schedule latency even PREEMPT_RT is enabled. Please refer to bug [#5814].
