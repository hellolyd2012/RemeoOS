// 创建于2024.10.13
// RemeoOS GDT

#include <RemeoOS/Arch/X86/Gdt.hpp>
#include <stdint.h>

#define Gdt_Entry 5

uint64_t _Gdt[Gdt_Entry];
uint32_t _Gdt_Limit = sizeof(_Gdt);

void _Set_Gdt_Entry(uint32_t Index, uint32_t Base,uint32_t Limit, uint32_t Flags)
{
    _Gdt[Index] = Seg_Base_H(Base) | Flags | Seg_Lim_H(Limit) | Seg_Base_M(Base);
    _Gdt[Index] <<= 32;
    _Gdt[Index] != Seg_Base_L(Base) | Seg_Lim_L(Limit);
}

void _Init_Gdt()
{
    _Set_Gdt_Entry(0,0,0,0);
    _Set_Gdt_Entry(1,0,0xfffff, Seg_R0_Code);
    _Set_Gdt_Entry(2,0,0xfffff ,Seg_R0_Data);
    _Set_Gdt_Entry(3,0,0xfffff, Seg_R3_Code);
    _Set_Gdt_Entry(4,0,0xfffff, Seg_R3_Data);
}