# Context switch
#
#   void swtch(struct context *old, struct context *new);
# 
# Save current registers in old. Load from new.	
#
# LAB ch1.1:
# TODO: 补全上下文切换时需要保存和恢复的寄存器。
# a0 指向 old context，a1 指向 new context。
# struct context 在 kernel/proc.h 中定义，每个字段是 8 字节。
# 下面已经给出 ra/sp/s0/s1 作为示例；请继续补全 s2-s11。


.globl swtch
swtch:
        sd ra, 0(a0)
        sd sp, 8(a0)
        sd s0, 16(a0)
        sd s1, 24(a0)

        # TODO ch1.1:
        # 1. 按 struct context 的字段顺序保存 s2-s11 到 old context。
        # 2. 偏移量从 32(a0) 开始，每个寄存器占 8 字节。
        # 3. 格式示例：sd s2, 32(a0)

        ld ra, 0(a1)
        ld sp, 8(a1)
        ld s0, 16(a1)
        ld s1, 24(a1)

        # TODO ch1.1:
        # 1. 按同样的偏移量，从 new context 恢复 s2-s11。
        # 2. 注意这里使用 a1，不是 a0。
        # 3. 格式示例：ld s2, 32(a1)
        
        ret

	
