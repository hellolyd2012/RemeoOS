// 创建于2024.10.13
// RemeoOS X86 GDT头文件

#define SD_Type(x)  (x << 8)
#define Sd_Code_Data(x) (x << 12)
#define Sd_DPL(x) (x << 13)
#define Sd_Present(x) (x << 15)
#define SdAvl(x)    (x << 20)
#define Sd_64Bits(x) (x << 21)
#define Sd_32Bits(x) (x << 22)
#define Sd_4k_Gran(x) (x << 23)

#define Seg_Lim_L(x)    (x & 0x0ffff)
#define Seg_Lim_H(x)    (x & 0xf0000)
#define Seg_Base_L(x)   ((x & 0x0000ffff) << 16)
#define Seg_Base_M(x)   ((x & 0x00ff0000) >> 16)
#define Seg_Base_H(x)    (x & 0xff000000)

#define Seg_Data_Rd 0x00
#define Seg_Data_Rda 0x01
#define Seg_Data_Rdwr 0x02
#define Seg_Data_Rdwra 0x03
#define Seg_Data_Rdexpd 0x04
#define Seg_Data_Rdexpda 0x05
#define Seg_Data_Rdwrexpd 0x06
#define Seg_Data_Rdwrexpda 0x07
#define Seg_Code_Ex 0x08
#define Seg_Code_Exa 0x09
#define Seg_Code_Exrd 0x0A
#define Seg_Code_Exrda 0x0B
#define Seg_Code_Exc 0x0C
#define Seg_Code_Exca 0x0D
#define Seg_Code_Exrdc 0x0E
#define Seg_Code_Exrdca 0x0F

#define Seg_R0_Code     SD_Type(Seg_Code_Exrd) | Sd_Code_Data(1) | Sd_DPL(0) | \
                        Sd_Present(1) | SdAvl(0) | Sd_64Bits(0) | Sd_32Bits(1) | \
                        Sd_4k_Gran(1)  

#define Seg_R0_Data     SD_Type(Seg_Code_Exrd) | Sd_Code_Data(1) | Sd_DPL(0) | \
                        Sd_Present(1) | SdAvl(0) | Sd_64Bits(0) | Sd_32Bits(1) | \
                        Sd_4k_Gran(1)

#define Seg_R3_Code     SD_Type(Seg_Code_Exrd) | Sd_Code_Data(1) | Sd_DPL(0) | \
                        Sd_Present(1) | SdAvl(0) | Sd_64Bits(0) | Sd_32Bits(1) | \
                        Sd_4k_Gran(1)

#define Seg_R3_Data     SD_Type(Seg_Code_Exrd) | Sd_Code_Data(1) | Sd_DPL(0) | \
                        Sd_Present(1) | SdAvl(0) | Sd_64Bits(0) | Sd_32Bits(1) | \
                        Sd_4k_Gran(1)



void _Init_Gdt();