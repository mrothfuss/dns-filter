#include <stdint.h>

typedef struct {
	void *buffer;
	int size;
	int cursor;
} bf_data;

// 8bit reads
uint8_t bf_r8(bf_data *data);
#define        bf_r8le(a) bf_r8(a)
#define        bf_r8be(a) bf_r8(a)
// 8bit writes
void bf_w8(bf_data *data, uint8_t val);
#define     bf_w8le(a, b) bf_w8(a, b)
#define     bf_w8be(a, b) bf_w8(a, b)

// 16bit reads
#define         bf_r16(a) bf_r16le(a)
uint16_t bf_r16le(bf_data *data);
uint16_t bf_r16be(bf_data *data);
// 16bit writes
#define     bf_w16(a, b) bf_w16le(a, b)
void bf_w16le(bf_data *data, uint16_t val);
void bf_w16be(bf_data *data, uint16_t val);

// 32bit reads
#define         bf_r32(a) bf_r32le(a)
uint32_t bf_r32le(bf_data *data);
uint32_t bf_r32be(bf_data *data);
// 32bit writes
#define     bf_w32(a, b) bf_w32le(a, b)
void bf_w32le(bf_data *data, uint32_t val);
void bf_w32be(bf_data *data, uint32_t val);

// 64bit reads
#define         bf_r64(a) bf_r64le(a)
uint64_t bf_r64le(bf_data *data);
uint64_t bf_r64be(bf_data *data);
// 64bit writes
#define     bf_w64(a, b) bf_w64le(a, b)
void bf_w64le(bf_data *data, uint64_t val);
void bf_w64be(bf_data *data, uint64_t val);

// string reads
void bf_rstr(bf_data *data, void *dest, int size);
// string writes
void bf_wstr(bf_data *data, void *source, int size);
