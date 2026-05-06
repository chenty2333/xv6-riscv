# Context switch
#
#   void swtch(struct context *old, struct context *new);
#
# Save current registers in old. Load from new.
#
# a0 指向 old，a1 指向 new。ra/sp/s0/s1 已给出。

.globl swtch
swtch:
        sd ra, 0(a0)
        sd sp, 8(a0)
        sd s0, 16(a0)
        sd s1, 24(a0)


        ld ra, 0(a1)
        ld sp, 8(a1)
        ld s0, 16(a1)
        ld s1, 24(a1)


        ret
