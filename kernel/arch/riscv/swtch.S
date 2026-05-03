# Context switch
#
#   void swtch(struct context *old, struct context *new);
#
# Save current registers in old. Load from new.
#
# LAB ch1.1: struct context 见 kernel/proc.h，每字段 8 字节。
# a0 指向 old，a1 指向 new。ra/sp/s0/s1 已给出。

.globl swtch
swtch:
        sd ra, 0(a0)
        sd sp, 8(a0)
        sd s0, 16(a0)
        sd s1, 24(a0)

        # LAB ch1.1: 保存 s2~s11 到 old，偏移量 32~104，步进 8。

        ld ra, 0(a1)
        ld sp, 8(a1)
        ld s0, 16(a1)
        ld s1, 24(a1)

        # LAB ch1.1: 从 new 恢复 s2~s11（同偏移，用 a1）。

        ret
