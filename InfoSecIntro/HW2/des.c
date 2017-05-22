#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** BMP things **/

typedef struct __attribute__((__packed__)) {
	uint8_t  magic[2];
	uint32_t bfSize;
	uint16_t unused1;
	uint16_t unused2;
	uint32_t imageDataOffset;
} BMP_FILE_HEADER;

typedef struct __attribute__((__packed__)) {
	uint32_t biSize;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitPix;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} DIB_HEADER;

typedef struct {
	BMP_FILE_HEADER file_header;
	DIB_HEADER      dib_header;
	size_t          gap_size;
	uint8_t         *gap_data;
	size_t          data_size;
	uint8_t         data[0];
} BMP;

BMP* bmp_parse_from_file (FILE *f)
{
	BMP local_hdr, *hdr = NULL;
	if (fread(&local_hdr.file_header, sizeof(BMP_FILE_HEADER), 1, f) != 1 ||
		fread(&local_hdr.dib_header, sizeof(DIB_HEADER), 1, f) != 1) {
		goto failed;
	}

	if (memcmp(local_hdr.file_header.magic, "BM", 2) != 0) {
		printf("Invalid BMP file header\n");
		goto failed;
	}

	size_t headers_size = sizeof(BMP_FILE_HEADER) + sizeof(DIB_HEADER);
	size_t file_size = local_hdr.file_header.bfSize;
	local_hdr.data_size = file_size - local_hdr.file_header.imageDataOffset;

	size_t gap_size = local_hdr.file_header.imageDataOffset - headers_size;
	uint8_t *gap = NULL;

	if (gap_size > 0) {
		gap = malloc(gap_size);
		if (fread(gap, gap_size, 1, f) != 1) {
			goto failed;
		}
	}
	local_hdr.gap_size = gap_size;
	local_hdr.gap_data = gap;

	hdr = malloc(sizeof(BMP) + local_hdr.data_size);
	*hdr = local_hdr;
	if (fread(&hdr->data, local_hdr.data_size, 1, f) != 1) {
		goto failed;
	}

	return hdr;

failed:
	free(hdr);
	free(gap);
	return NULL;
}

void bmp_destory (BMP *bmp)
{
	free(bmp->gap_data);
	free(bmp);
}

bool bmp_save (BMP *bmp, FILE *fout)
{
	if (fwrite(&bmp->file_header, sizeof(BMP_FILE_HEADER), 1, fout) != 1 ||
		fwrite(&bmp->dib_header, sizeof(DIB_HEADER), 1, fout) != 1 ||
		(bmp->gap_size > 0 && fwrite(bmp->gap_data, bmp->gap_size, 1, fout) != 1) ||
		fwrite(&bmp->data, bmp->data_size, 1, fout) != 1) {
		return false;
	}
	return true;
}

void bmp_stat (BMP *bmp)
{
	printf("Stat of bitmap file:\n");
	printf("Filesize:       %u\n", bmp->file_header.bfSize);
	printf("Datasize:       %zu\n", bmp->data_size);
	printf("Width:          %u\n", bmp->dib_header.width);
	printf("Height:         %u\n", bmp->dib_header.height);
	printf("Bits per pixel: %u\n", bmp->dib_header.bitPix);
}

/** DES things **/

enum BLOCK_CIPHER_MODE { ECB, CBC, CTR, OFB };

typedef struct {
	uint8_t k[8];
	uint8_t c[4];
	uint8_t d[4];
} key_set;

const int initial_key_permutaion[] = {
	57, 49,  41, 33,  25,  17,  9,
	1, 58,  50, 42,  34,  26, 18,
	10,  2,  59, 51,  43,  35, 27,
	19, 11,   3, 60,  52,  44, 36,
	63, 55,  47, 39,  31,  23, 15,
	7, 62,  54, 46,  38,  30, 22,
	14,  6,  61, 53,  45,  37, 29,
	21, 13,   5, 28,  20,  12,  4
};

const int initial_message_permutation[] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

const int key_shift_sizes[] = {-1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

const int sub_key_permutation[] = {
	14, 17, 11, 24,  1,  5,
	3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

const int message_expansion[] = {
	32,  1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

const int S1[] = {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
			 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
			 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
			15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13};

const int S2[] = {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
			 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
			 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
			13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

const int S3[] = {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
			13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
			13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
			 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

const int S4[] = { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
			13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
			10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
			 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

const int S5[] = { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
			14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
			 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
			11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

const int S6[] = {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
			10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
			 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
			 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};

const int S7[] = { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
			13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
			 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
			 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

const int S8[] = {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
			 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
			 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
			 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11};

const int right_sub_message_permutation[] = {
	16,  7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25
};

const int final_message_permutation[] = {
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};


void* des_create(const uint8_t* key) {
	int i, j;
	int shift_size;
	uint8_t shift_byte, first_shift_bits, second_shift_bits, third_shift_bits, fourth_shift_bits;
	key_set* key_sets = malloc(17 * sizeof(key_set));
	memset(key_sets, 0, 17 * sizeof(key_set));

	for (i=0; i<8; i++) {
		key_sets[0].k[i] = 0;
	}

	for (i=0; i<56; i++) {
		shift_size = initial_key_permutaion[i];
		shift_byte = 0x80 >> ((shift_size - 1)%8);
		shift_byte &= key[(shift_size - 1)/8];
		shift_byte <<= ((shift_size - 1)%8);

		key_sets[0].k[i/8] |= (shift_byte >> i%8);
	}

	for (i=0; i<3; i++) {
		key_sets[0].c[i] = key_sets[0].k[i];
	}

	key_sets[0].c[3] = key_sets[0].k[3] & 0xF0;

	for (i=0; i<3; i++) {
		key_sets[0].d[i] = (key_sets[0].k[i+3] & 0x0F) << 4;
		key_sets[0].d[i] |= (key_sets[0].k[i+4] & 0xF0) >> 4;
	}

	key_sets[0].d[3] = (key_sets[0].k[6] & 0x0F) << 4;


	for (i=1; i<17; i++) {
		for (j=0; j<4; j++) {
			key_sets[i].c[j] = key_sets[i-1].c[j];
			key_sets[i].d[j] = key_sets[i-1].d[j];
		}

		shift_size = key_shift_sizes[i];
		if (shift_size == 1){
			shift_byte = 0x80;
		} else {
			shift_byte = 0xC0;
		}

		// Process C
		first_shift_bits = shift_byte & key_sets[i].c[0];
		second_shift_bits = shift_byte & key_sets[i].c[1];
		third_shift_bits = shift_byte & key_sets[i].c[2];
		fourth_shift_bits = shift_byte & key_sets[i].c[3];

		key_sets[i].c[0] <<= shift_size;
		key_sets[i].c[0] |= (second_shift_bits >> (8 - shift_size));

		key_sets[i].c[1] <<= shift_size;
		key_sets[i].c[1] |= (third_shift_bits >> (8 - shift_size));

		key_sets[i].c[2] <<= shift_size;
		key_sets[i].c[2] |= (fourth_shift_bits >> (8 - shift_size));

		key_sets[i].c[3] <<= shift_size;
		key_sets[i].c[3] |= (first_shift_bits >> (4 - shift_size));

		// Process D
		first_shift_bits = shift_byte & key_sets[i].d[0];
		second_shift_bits = shift_byte & key_sets[i].d[1];
		third_shift_bits = shift_byte & key_sets[i].d[2];
		fourth_shift_bits = shift_byte & key_sets[i].d[3];

		key_sets[i].d[0] <<= shift_size;
		key_sets[i].d[0] |= (second_shift_bits >> (8 - shift_size));

		key_sets[i].d[1] <<= shift_size;
		key_sets[i].d[1] |= (third_shift_bits >> (8 - shift_size));

		key_sets[i].d[2] <<= shift_size;
		key_sets[i].d[2] |= (fourth_shift_bits >> (8 - shift_size));

		key_sets[i].d[3] <<= shift_size;
		key_sets[i].d[3] |= (first_shift_bits >> (4 - shift_size));

		for (j=0; j<48; j++) {
			shift_size = sub_key_permutation[j];
			if (shift_size <= 28) {
				shift_byte = 0x80 >> ((shift_size - 1)%8);
				shift_byte &= key_sets[i].c[(shift_size - 1)/8];
				shift_byte <<= ((shift_size - 1)%8);
			} else {
				shift_byte = 0x80 >> ((shift_size - 29)%8);
				shift_byte &= key_sets[i].d[(shift_size - 29)/8];
				shift_byte <<= ((shift_size - 29)%8);
			}

			key_sets[i].k[j/8] |= (shift_byte >> j%8);
		}
	}

	return key_sets;
}

void des_cipher(uint8_t* data, uint8_t* out_buffer, void* key_context, int is_encrypt) {
	int i, k;
	int shift_size;
	uint8_t shift_byte;
	key_set *key_sets = (key_set*)key_context;

	uint8_t initial_permutation[8];
	memset(initial_permutation, 0, 8);
	memset(out_buffer, 0, 8);

	for (i=0; i<64; i++) {
		shift_size = initial_message_permutation[i];
		shift_byte = 0x80 >> ((shift_size - 1)%8);
		shift_byte &= data[(shift_size - 1)/8];
		shift_byte <<= ((shift_size - 1)%8);

		initial_permutation[i/8] |= (shift_byte >> i%8);
	}

	uint8_t l[4], r[4];
	for (i=0; i<4; i++) {
		l[i] = initial_permutation[i];
		r[i] = initial_permutation[i+4];
	}

	uint8_t ln[4], rn[4], er[6], ser[4];

	int key_index;
	for (k=1; k<=16; k++) {
		memcpy(ln, r, 4);

		memset(er, 0, 6);

		for (i=0; i<48; i++) {
			shift_size = message_expansion[i];
			shift_byte = 0x80 >> ((shift_size - 1)%8);
			shift_byte &= r[(shift_size - 1)/8];
			shift_byte <<= ((shift_size - 1)%8);

			er[i/8] |= (shift_byte >> i%8);
		}

		key_index = is_encrypt ? k : 17 - k;

		for (i=0; i<6; i++) {
			er[i] ^= key_sets[key_index].k[i];
		}

		uint8_t row, column;

		for (i=0; i<4; i++) {
			ser[i] = 0;
		}

		// 0000 0000 0000 0000 0000 0000
		// rccc crrc cccr rccc crrc cccr

		// Byte 1
		row = 0;
		row |= ((er[0] & 0x80) >> 6);
		row |= ((er[0] & 0x04) >> 2);

		column = 0;
		column |= ((er[0] & 0x78) >> 3);

		ser[0] |= ((uint8_t)S1[row*16+column] << 4);

		row = 0;
		row |= (er[0] & 0x02);
		row |= ((er[1] & 0x10) >> 4);

		column = 0;
		column |= ((er[0] & 0x01) << 3);
		column |= ((er[1] & 0xE0) >> 5);

		ser[0] |= (uint8_t)S2[row*16+column];

		// Byte 2
		row = 0;
		row |= ((er[1] & 0x08) >> 2);
		row |= ((er[2] & 0x40) >> 6);

		column = 0;
		column |= ((er[1] & 0x07) << 1);
		column |= ((er[2] & 0x80) >> 7);

		ser[1] |= ((uint8_t)S3[row*16+column] << 4);

		row = 0;
		row |= ((er[2] & 0x20) >> 4);
		row |= (er[2] & 0x01);

		column = 0;
		column |= ((er[2] & 0x1E) >> 1);

		ser[1] |= (uint8_t)S4[row*16+column];

		// Byte 3
		row = 0;
		row |= ((er[3] & 0x80) >> 6);
		row |= ((er[3] & 0x04) >> 2);

		column = 0;
		column |= ((er[3] & 0x78) >> 3);

		ser[2] |= ((uint8_t)S5[row*16+column] << 4);

		row = 0;
		row |= (er[3] & 0x02);
		row |= ((er[4] & 0x10) >> 4);

		column = 0;
		column |= ((er[3] & 0x01) << 3);
		column |= ((er[4] & 0xE0) >> 5);

		ser[2] |= (uint8_t)S6[row*16+column];

		// Byte 4
		row = 0;
		row |= ((er[4] & 0x08) >> 2);
		row |= ((er[5] & 0x40) >> 6);

		column = 0;
		column |= ((er[4] & 0x07) << 1);
		column |= ((er[5] & 0x80) >> 7);

		ser[3] |= ((uint8_t)S7[row*16+column] << 4);

		row = 0;
		row |= ((er[5] & 0x20) >> 4);
		row |= (er[5] & 0x01);

		column = 0;
		column |= ((er[5] & 0x1E) >> 1);

		ser[3] |= (uint8_t)S8[row*16+column];

		for (i=0; i<4; i++) {
			rn[i] = 0;
		}

		for (i=0; i<32; i++) {
			shift_size = right_sub_message_permutation[i];
			shift_byte = 0x80 >> ((shift_size - 1)%8);
			shift_byte &= ser[(shift_size - 1)/8];
			shift_byte <<= ((shift_size - 1)%8);

			rn[i/8] |= (shift_byte >> i%8);
		}

		for (i=0; i<4; i++) {
			rn[i] ^= l[i];
		}

		for (i=0; i<4; i++) {
			l[i] = ln[i];
			r[i] = rn[i];
		}
	}

	uint8_t pre_end_permutation[8];
	for (i=0; i<4; i++) {
		pre_end_permutation[i] = r[i];
		pre_end_permutation[4+i] = l[i];
	}

	for (i=0; i<64; i++) {
		shift_size = final_message_permutation[i];
		shift_byte = 0x80 >> ((shift_size - 1)%8);
		shift_byte &= pre_end_permutation[(shift_size - 1)/8];
		shift_byte <<= ((shift_size - 1)%8);

		out_buffer[i/8] |= (shift_byte >> i%8);
	}
}

void des_destory(void * key_context)
{
	free(key_context);
}

void hexdump(const void *data, size_t len) { for(int i = 0; i < len; i++) { printf("%.2x ", ((uint8_t*)data)[i]); } putchar(0xa); }

void xor (uint8_t *data, uint8_t *key, size_t len)
{
	do {
		len--;
		data[len] ^= key[len];
	} while (len > 0);
}

void inc (uint8_t *data, size_t len)
{
	do {
		if (++data[--len]) {
			return;
		}
	} while (len > 0);
}

void des_encrypt (const uint8_t *key, const uint8_t *iv, enum BLOCK_CIPHER_MODE mode, uint8_t *data, size_t len)
{
	assert(len % 8 == 0);
	void *ctx = des_create(key);
	uint8_t buff[8], vec[8];

	if (mode == ECB) {
		for(int i = 0; i < len; i += 8) {
			des_cipher(data + i, buff, ctx, 1);
			memcpy(data + i, buff, 8);
		}
	} else if (mode == CBC) {
		memcpy(vec, iv, 8);

		for(int i = 0; i < len; i += 8) {
			des_cipher(data + i, buff, ctx, 1);
			xor(vec, buff, 8);
			memcpy(data + i, vec, 8);
		}
	} else if (mode == CTR) {
		memcpy(vec, iv, 8);

		uint8_t stream_key[8];

		for(int i = 0; i < len; i += 8) {
			des_cipher(vec, stream_key, ctx, 1);
			xor(data + i, stream_key, 8);
			inc(vec, 8);
		}
	} else if (mode == OFB) {
		memcpy(vec, iv, 8);

		for(int i = 0; i < len; i += 8) {
			des_cipher(vec, buff, ctx, 1);
			memcpy(vec, buff, 8);
			xor(data + i, vec, 8);
		}
	}

	des_destory(ctx);
}

void des_decrypt (const uint8_t *key, const uint8_t *iv, enum BLOCK_CIPHER_MODE mode, uint8_t *data, size_t len)
{
	if (mode == CTR || mode == OFB) {
		des_encrypt(key, iv, mode, data, len);
		return;
	}
	assert(len % 8 == 0);
	void *ctx = des_create(key);
	uint8_t buff[8], vec[8];

	if (mode == ECB) {
		for(int i = 0; i < len; i += 8) {
			des_cipher(data + i, buff, ctx, 0);
			memcpy(data + i, buff, 8);
		}
	} else if (mode == CBC) {
		memcpy(vec, iv, 8);

		for(int i = 0; i < len; i += 8) {
			xor(vec, data + i, 8);
			des_cipher(vec, buff, ctx, 0);
			memcpy(vec, data + i, 8);
			memcpy(data + i, buff, 8);
		}
	}

	des_destory(ctx);
}

size_t parse_hex(const char *input, uint8_t *output, size_t len)
{
	const static uint8_t hex_decode_table[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
		0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i = 0;
	uint8_t v, t;
	while(input[i] && len > 0) {
		if(i % 2 == 0) {
			v = hex_decode_table[input[i]];
			if (v == 0 && input[i] != '0') {
				return i / 2;
			}
		} else {
			t = hex_decode_table[input[i]];
			if (t == 0 && input[i] != '0') {
				return i / 2;
			}
			output[i / 2] = (v << 4) | t;
			len--;
		}
		i++;
	}
	return i / 2;
}

/** Main program **/

int main (int argc, const char *argv[])
{
	if (argc <= 6) {
		printf("Usage: %s infile outfile enc|dec ECB|CBC|CTR|OFB key-in-hex iv-in-hex\n", argv[0]);
		return 1;
	}

	int is_encrypt;
	if (strcmp(argv[3], "enc") == 0) {
		is_encrypt = 1;
	} else if (strcmp(argv[3], "dec") == 0) {
		is_encrypt = 0;
	} else {
		printf("Unknow operation %s\n", argv[3]);
		return 1;
	}

	enum BLOCK_CIPHER_MODE mode;
	if (strcmp(argv[4], "ECB") == 0) {
		mode = ECB;
	} else if (strcmp(argv[4], "CBC") == 0) {
		mode = CBC;
	} else if (strcmp(argv[4], "CTR") == 0) {
		mode = CTR;
	} else if (strcmp(argv[4], "OFB") == 0) {
		mode = OFB;
	} else {
		printf("Unknow block operation mode %s\n", argv[4]);
		return 1;
	}

	uint8_t key[8], iv[8];

	if (parse_hex(argv[5], key, 8) != 8) {
		printf("Invalid key: %s\n", argv[5]);
		return 1;
	}

	if (parse_hex(argv[6], iv, 8) != 8) {
		printf("Invalid iv: %s\n", argv[6]);
		return 1;
	}

	FILE *f = fopen(argv[1], "rb");
	if (f == NULL) {
		printf("Can not open file %s\n", argv[1]);
		return 1;
	}
	BMP *bmp = bmp_parse_from_file(f);
	fclose(f);

	if (bmp == NULL) {
		printf("Can not parse BMP file\n");
		return 1;
	}

	// bmp_stat(bmp);
	//printf("key: ");hexdump(key, 8);
	//printf("iv:  ");hexdump(iv, 8);

	if (is_encrypt) {
		des_encrypt(key, iv, mode, bmp->data, bmp->data_size);
	} else {
		des_decrypt(key, iv, mode, bmp->data, bmp->data_size);
	}

	f = fopen(argv[2], "wb");
	if (f == NULL) {
		printf("Can not open file %s\n", argv[2]);
	} else {
		if (!bmp_save(bmp, f))
			printf("Can not save file\n");
		fclose(f);
	}
	bmp_destory(bmp);
}
