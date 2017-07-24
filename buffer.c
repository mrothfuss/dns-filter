#include <stdint.h>
#include <string.h>

#include "buffer.h"

inline uint8_t
bf_r8(bf_data *data) {
	int last;

	last = data->cursor;
	data->cursor += 1;
	if(data->cursor > data->size) return 0;
	return ((uint8_t*)data->buffer)[last];
}

inline uint16_t
bf_r16le(bf_data *data) {
	int last;
	uint16_t res = 0;

	last = data->cursor;
	data->cursor += 2;
	if(data->cursor > data->size) return 0;
	res |= (uint16_t)(((uint8_t*)data->buffer)[last++]);
	res |= (uint16_t)(((uint8_t*)data->buffer)[last++]) << 8;
	return res;
}

inline uint16_t
bf_r16be(bf_data *data) {
	int last;
	uint16_t res = 0;

	last = data->cursor;
	data->cursor += 2;
	if(data->cursor > data->size) return 0;
	res |= (uint16_t)(((uint8_t*)data->buffer)[last++]) << 8;
	res |= (uint16_t)(((uint8_t*)data->buffer)[last++]);
	return res;
}

inline uint32_t
bf_r32le(bf_data *data) {
	int last;
	uint32_t res = 0;

	last = data->cursor;
	data->cursor += 4;
	if(data->cursor > data->size) return 0;
	res |= (uint32_t)(((uint8_t*)data->buffer)[last++]);
	res |= (uint32_t)(((uint8_t*)data->buffer)[last++]) << 8;
	res |= (uint32_t)(((uint8_t*)data->buffer)[last++]) << 16;
	res |= (uint32_t)(((uint8_t*)data->buffer)[last++]) << 24;
	return res;
}

inline uint32_t
bf_r32be(bf_data *data) {
	int last;
	uint32_t res = 0;

	last = data->cursor;
	data->cursor += 4;
	if(data->cursor > data->size) return 0;
	res |= (uint32_t)(((uint8_t*)data->buffer)[last++]) << 24;
	res |= (uint32_t)(((uint8_t*)data->buffer)[last++]) << 16;
	res |= (uint32_t)(((uint8_t*)data->buffer)[last++]) << 8;
	res |= (uint32_t)(((uint8_t*)data->buffer)[last++]);
	return res;
}

inline uint64_t
bf_r64le(bf_data *data) {
	int last;
	uint64_t res = 0;

	last = data->cursor;
	data->cursor += 8;
	if(data->cursor > data->size) return 0;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]);
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 8;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 16;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 24;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 32;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 40;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 48;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 56;
	return res;
}

inline uint64_t
bf_r64be(bf_data *data) {
	int last;
	uint64_t res = 0;

	last = data->cursor;
	data->cursor += 8;
	if(data->cursor > data->size) return 0;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 56;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 48;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 40;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 32;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 24;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 16;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]) << 8;
	res |= (uint64_t)(((uint8_t*)data->buffer)[last++]);
	return res;
}

inline void
bf_w8(bf_data *data, uint8_t value) {
	int last;

	last = data->cursor;
	data->cursor += 1;
	if(data->cursor > data->size) return;
	((uint8_t*)data->buffer)[last] = value;
}

inline void
bf_w16le(bf_data *data, uint16_t value) {
	int last;

	last = data->cursor;
	data->cursor += 2;
	if(data->cursor > data->size) return;
	((uint8_t*)data->buffer)[last++] = value & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 8) & 0xFF;
}

inline void
bf_w16be(bf_data *data, uint16_t value) {
	int last;

	last = data->cursor;
	data->cursor += 2;
	if(data->cursor > data->size) return;
	((uint8_t*)data->buffer)[last++] = (value >> 8) & 0xFF;
	((uint8_t*)data->buffer)[last++] = value & 0xFF;
}

inline void
bf_w32le(bf_data *data, uint32_t value) {
	int last;

	last = data->cursor;
	data->cursor += 4;
	if(data->cursor > data->size) return;
	((uint8_t*)data->buffer)[last++] = value & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 8) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 16) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 24) & 0xFF;
}

inline void
bf_w32be(bf_data *data, uint32_t value) {
	int last;

	last = data->cursor;
	data->cursor += 4;
	if(data->cursor > data->size) return;
	((uint8_t*)data->buffer)[last++] = (value >> 24) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 16) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 8) & 0xFF;
	((uint8_t*)data->buffer)[last++] = value & 0xFF;
}

inline void
bf_w64le(bf_data *data, uint64_t value) {
	int last;

	last = data->cursor;
	data->cursor += 8;
	if(data->cursor > data->size) return;
	((uint8_t*)data->buffer)[last++] = value & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 8) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 16) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 24) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 32) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 40) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 48) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 56) & 0xFF;
}

inline void
bf_w64be(bf_data *data, uint64_t value) {
	int last;

	last = data->cursor;
	data->cursor += 8;
	if(data->cursor > data->size) return;
	((uint8_t*)data->buffer)[last++] = (value >> 56) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 48) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 40) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 32) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 24) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 16) & 0xFF;
	((uint8_t*)data->buffer)[last++] = (value >> 8) & 0xFF;
	((uint8_t*)data->buffer)[last++] = value & 0xFF;
}

inline void
bf_rstr(bf_data *data, void *dest, int size) {
	int last;

	last = data->cursor;
	data->cursor += size;
	if(data->cursor > data->size) return;
	memcpy(dest, (data->buffer + last), size);
}

inline void
bf_wstr(bf_data *data, void *src, int size) {
	int last;

	last = data->cursor;
	data->cursor += size;
	if(data->cursor > data->size) return;
	memcpy((data->buffer + last), src, size);
}
