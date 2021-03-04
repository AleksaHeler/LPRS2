
///////////////////////////////////////////////////////////////////////////////

// Product of Serbia.
// RA22-2018 Aleksa Heler

///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////

#if DEBUG
#define dbg_printf printf
#else
#define dbg_printf
#endif

///////////////////////////////////////////////////////////////////////////////

// Ispis cele memorije (sva 4 uint32_t)
#define PRINT_FOO dbg_printf("foo_mem[0] = 0x%x\n", foo_mem[0]);\
	dbg_printf("foo_mem[1] = 0x%x\n", foo_mem[1]);\
	dbg_printf("foo_mem[2] = 0x%x\n", foo_mem[2]);\
	dbg_printf("foo_mem[3] = 0x%x\n\n", foo_mem[3]);

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


//foo_bf_regs
typedef struct {
	uint32_t W;
	uint32_t X;
	uint32_t Z;
	uint32_t Y;
} foo_bf_regs;

//foo_bf_arrays
typedef struct {
	struct { // W
		uint32_t L;
	} W;
	struct { // x
		unsigned D  :  15;
		unsigned C  :  6;
		unsigned B  :  7;
		unsigned A  :  4;
	} X;
	struct { // Z
		uint16_t N[2];
	} Z;
	struct { // Y
		uint8_t M[4];
	} Y;
} foo_bf_arrays;

// foo_bf_unions
typedef struct {
	union { // W
		uint32_t r;
		struct {
			uint32_t L;
		} b;
	} W;
	union { // X
		uint32_t r;
		struct {
			unsigned D  :  15;
			unsigned C  :  6;
			unsigned B  :  7;
			unsigned A  :  4;
		} b;
	} X;
	union { // Z
		uint32_t r;
		struct {
			uint16_t N[2];
		} b;
	} Z;
	union { // Y
		uint32_t r;
		struct {
			uint8_t M[4];
		} b;
	} Y;
} foo_bf_unions;

uint32_t foo_mem[4];

///////////////////////////////////////////////////////////////////////////////

int main(void) {
	printf("-------------------\n\n");

	PRINT_FOO;
	
	volatile uint8_t*  p8  = (volatile uint8_t*) foo_mem;
	volatile uint32_t* p32 = (volatile uint32_t*)foo_mem;

	volatile foo_bf_regs*   pr = (volatile foo_bf_regs*)  foo_mem;
	volatile foo_bf_arrays* pa = (volatile foo_bf_arrays*)foo_mem;
	volatile foo_bf_unions* pu = (volatile foo_bf_unions*)foo_mem;
	
	// ----------- Zadatak 3 ---------------------
	printf("----------- Zadatak 3 -----------\n");
	// Dodeliti 0xbabadeda registru W preko pr
	pr->W = 0xbabadeda;
	// Dodeliti 0xfeedbeef registru Y preko p32
	p32[3] = 0xfeedbeef;
	// Dodeliti 0xdeadbeef registru X preko p8
	p8[4] = 0xef;
	p8[5] = 0xbe;
	p8[6] = 0xad;
	p8[7] = 0xde;
	// Dodeliti 0x76543210 registru Z preko pu
	pu->Z.r = 0x76543210;
	// Ispis zadatka 3.
	printf("foo_mem[0] = 0x%08x\n", foo_mem[0]);
	printf("foo_mem[1] = 0x%08x\n", foo_mem[1]);
	printf("foo_mem[2] = 0x%08x\n", foo_mem[2]);
	printf("foo_mem[3] = 0x%08x\n\n", foo_mem[3]);
	
	// ----------- Zadatak 4 ---------------------
	// Ispisati __ preko __
	printf("----------- Zadatak 4 -----------\n");
	printf("Y = 0x%02x%02x%02x%02x (preko p8)\n", p8[15], p8[14], p8[13], p8[12]);
	printf("Z = 0x%08x (preko pu)\n", pu->Z.r);
	printf("W = 0x%08x (preko p32)\n", p32[0]);
	printf("X = 0x%08x (preko pr)\n\n", pr->X);
	
	// ----------- Zadatak 5 ---------------------
	// Dodeliti __ registru M[__] preko __
	printf("----------- Zadatak 5 -----------\n");
	// prvo resetujemo ciljane bite na 0, pa ih or-ujemo na 1
	pr->Y = (pr->Y & 0xffffff00) | 0x76;
	pr->Y = (pr->Y & 0xffff00ff) | 0x32<<8;
	pr->Y = (pr->Y & 0xff00ffff) | 0x10<<16;
	pr->Y = (pr->Y & 0x00ffffff) | 0x54<<24;
	printf("Y = 0x%08x (preko pr)\n\n", pr->Y);
	
	// ----------- Zadatak 6 ---------------------
	// Ispisati __ preko __
	printf("----------- Zadatak 6 -----------\n");
	printf("W = 0x%08x (preko p32)\n", p32[0]);
	printf("X = 0x%02x%02x%02x%02x (preko p8)\n", p8[7], p8[6], p8[5], p8[4]);
	printf("Y = 0x%08x (preko pr)\n", pr->Y);
	printf("Z = 0x%08x (preko pu)\n\n", pu->Z.r);
	
	// ----------- Zadatak 7 ---------------------
	// Ispisati __ preko __
	printf("----------- Zadatak 7 -----------\n");
	printf("M[2] = 0x%02x   (preko p8)\n", p8[14]);
	printf("N[0] = 0x%02x%02x (preko p8)\n", p8[9], p8[8]);
	printf("N[1] = 0x%04x (preko pr)\n", pr->Z >> 16);
	printf("M[3] = 0x%02x   (preko pr)\n", pr->Y >> 24);
	printf("M[3] = 0x%02x   (preko p32)\n", p32[3] >> 24);
	printf("N[0] = 0x%04x (preko p32)\n", p32[2] & 0xffff);
	printf("M[2] = 0x%02x   (preko pu)\n", pu->Y.b.M[2]);
	printf("N[0] = 0x%02x (preko pu)\n\n", pu->Z.b.N[0]);
	
	// ----------- Zadatak 8 ---------------------
	// Proveriti
	printf("----------- Zadatak 8 -----------\n");
	printf("   ┌A ┐┌  B  ┐┌ C  ┐┌      D      ┐\n");
	printf("X: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(p8[7]));
	printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(p8[6]));
	printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(p8[5]));
	printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(p8[4]));
	printf("bit 5 reg C = %01x (preko p8)\n",  (p8[6] >> 4) & 1);
	printf("bit 4 reg D = %01x (preko pu)\n",  (pu->X.b.D >> 3) & 1);
	printf("bit 6 reg D = %01x (preko pr)\n",  (pr->X >> 6) & 1);
	printf("bit 1 reg A = %01x (preko pr)\n",  (pr->X >> 29) & 1);
	printf("bit 3 reg C = %01x (preko p32)\n", (p32[1] >> 18) & 1);
	printf("bit 5 reg B = %01x (preko p32)\n", (p32[1] >> 26) & 1);
	printf("bit 3 reg A = %01x (preko pu)\n",  (pu->X.b.A >> 3) & 1);
	printf("bit 0 reg B = %01x (preko p8)\n\n",  (p8[6] >> 5) & 1);
	
	// ----------- Zadatak 9 ---------------------
	// Ispisati __ preko __
	printf("----------- Zadatak 8 -----------\n");
	printf("D = 0x%02x%02x (preko p8)\n", p8[5] & 0b1111111, p8[4]);
	printf("A = 0x%02x   (preko pr)\n", (pr->X >> 28) & 0b1111);
	printf("B = 0x%02x   (preko p32)\n", (p32[1] >> 21) & 0b1111111);
	printf("C = 0x%02x   (preko pu)\n\n", pu->X.b.C);

	printf("\n-------------------\n\n");
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
