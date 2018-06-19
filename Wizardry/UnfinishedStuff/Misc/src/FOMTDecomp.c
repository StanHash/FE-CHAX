// rewrite in C of the Harvest Moon : FOMT in-game decompressor
// The original decompression "routine" is located at D102C and was probably handwritten in ASM

#include "stdint.h"

struct DecompState {
	unsigned* pSource;    // r0 in original decompressor
	void*     pTarget;    // r1 in original decompressor

	unsigned  queuedRead; // r2 in original decompressor
	int       queuedBits; // r3 in original decompressor

	unsigned(*readByteFunc)(struct DecompState*);

	unsigned  prevByte; // r10 in original decompressor // buffered "written" byte in case the current target address is odd (not aligned)

	short     huffTree[0x200];

	struct {
		uint16_t offset;
		uint8_t  bits;
	} lzReadLookup[0x10];
};

unsigned decomp_read(struct DecompState* state, unsigned bitCount);
void decomp_build_huffman_tree(struct DecompState* state, unsigned chunkBits);
unsigned decomp_readbyte_huff4(struct DecompState* state);
unsigned decomp_readbyte_huff8(struct DecompState* state);
unsigned decomp_readbyte_raw(struct DecompState* state);
unsigned decomp_read(struct DecompState* state, unsigned bitCount);
unsigned decomp_read_huff(struct DecompState* state);

unsigned decomp_main(unsigned* pSource, uint16_t* pTarget) {
	struct DecompState state;

	unsigned size = (*pSource++) >> 8;
	// r8 = pTarget + size - 2

	state.pSource = pSource;
	state.pTarget = (void*)(pTarget) - 2;

	state.queuedRead = 0;
	state.queuedBits = 0;

	unsigned compType = decomp_read(&state, 8);

	unsigned huffType  = (compType << 27) >> 30; // bits 3-4 (2 bits)
	unsigned pass1Type = (compType << 29) >> 29; // bits 0-2 (3 bits)
	unsigned pass2Type = (compType << 24) >> 29; // bits 5-7 (3 bits)

	switch (huffType) {
	case 1:
		decomp_build_huffman_tree(&state, 4);
		state.readByteFunc = decomp_readbyte_huff4;
		break;

	case 2:
		decomp_build_huffman_tree(&state, 8);
		state.readByteFunc = decomp_readbyte_huff8;
		break;

	default:
		state.readByteFunc = decomp_readbyte_raw;
		break;
	}

	return size; // temp
}

void decomp_build_huffman_tree(struct DecompState* state, unsigned chunkBits) {
	short* nBuf = state->huffTree;

	// Initializing tree with -1
	for (unsigned fillCount = (1 << chunkBits); fillCount > 0; fillCount -= 2) {
		*nBuf++ = -1;
		*nBuf++ = -1;
		*nBuf++ = -1;
		*nBuf++ = -1;
	}

	unsigned nodePrevAllocIndex = 0;
	unsigned currentNodePath    = 0;
	unsigned i                  = 0;
	unsigned count              = chunkBits * 2;

	do {
		currentNodePath <<= 1;

		unsigned readCount = decomp_read(state, chunkBits);

		while (readCount) {
			short nodeItOff = 0;
			unsigned j;

			// Finding path in tree
			for (j = i; j; j--) {
				unsigned nodeIndex = nodeItOff + (currentNodePath >> j) % 2;

				nodeItOff = state->huffTree[nodeIndex];

				if (nodeItOff <= 0) // <= 0 means unallocated (or data)
					nodeItOff = state->huffTree[nodeIndex] = (nodePrevAllocIndex += 2);
			}

			// Writing leaf data
			unsigned nodeIndex = (currentNodePath % 2) + nodeItOff;
			state->huffTree[nodeIndex] = ~decomp_read(state, chunkBits);

			currentNodePath++;
			readCount--;
		}
	} while (i++, (--count != 0));
}

void decomp_pass1_default(struct DecompState* state) {

}

unsigned decomp_readbyte_huff4(struct DecompState* state) {
	unsigned result = 0;

	result |= decomp_read_huff(state) << 4;
	result |= decomp_read_huff(state);

	return result;
}

unsigned decomp_readbyte_huff8(struct DecompState* state) {
	return decomp_read_huff(state);
}

unsigned decomp_readbyte_raw(struct DecompState* state) {
	return decomp_read(state, 8);
}

unsigned decomp_read(struct DecompState* state, unsigned bitCount) {
	int      queueShift = 32 - bitCount;                   // r5 in original decompressor
	unsigned data       = state->queuedRead >> queueShift; // r6 in original decompressor

	state->queuedBits -= bitCount;

	if (state->queuedBits >= 0) {
		state->queuedRead <<= bitCount;
		return data;
	} else {
		unsigned newRead  = *(state->pSource)++;

		state->queuedRead = newRead << (-state->queuedBits);
		state->queuedBits += 32;

		return data | (newRead >> state->queuedBits);
	}
}

unsigned decomp_read_huff(struct DecompState* state) {
	short node = 0;

	do {
		if ((--state->queuedBits) < 0) {
			state->queuedRead = *state->pSource++;
			state->queuedBits = 32;
		}

		node = state->huffTree[node + (state->queuedRead >> 31)];
		state->queuedRead <<= 1;
	} while (node > 0);

	return ~node;
}

// loc_80D145C
void decomp_lz(struct DecompState* state, unsigned distance, int length) {
	if (distance == 1) {
		unsigned data;

		if ((uintptr_t)(state->pTarget) % 2) {
			data = state->prevByte | (state->prevByte << 8);

			state->pTarget -= 1;
			length += 1;
		} else {
			data = *(uint16_t*)(state->pTarget);
			data >>= 8; // data = high byte of word at target

			state->prevByte = data << 8;
			data = data | state->prevByte;
		}

		do {
			*(short*)(state->pTarget += 2) = data;
			length -= 2;
		} while (length > 0);

		state->prevByte = data >> 8;
		state->pTarget += length;
	} else if (distance % 2) {
		// distance is odd

		void* viewIt = state->pTarget - distance;

		if ((uintptr_t)(viewIt) % 2) {
			// target is aligned here
			// reading first byte individually to sync alignment

			viewIt += 1; // align view

			// Reading byte
			state->prevByte = *(uint16_t*)(viewIt);
			state->prevByte >>= 8;

			state->pTarget += 1;
			// target is misaligned

			// We read a byte
			length -= 1;

			// if only one byte was to be read, return
			if (length == 0)
				return;
		}

		// target is not aligned here

		state->pTarget -= 1; // align

		// target is aligned here

		unsigned data;

		do {
			data = *(uint16_t*)(viewIt += 2);
			data = data << 8;
			data = data | state->prevByte;

			*(uint16_t*)(state->pTarget += 2) = data;

			state->prevByte = data >> 16;
			
			length -= 2;
		} while(length > 0);

		length += 1;
		state->pTarget -= -length;
	} else {
		// distance is even

		void* viewIt = state->pTarget - distance;

		if ((uintptr_t)(viewIt) % 2) {
			// target is misaligned
			unsigned nextShort;

			nextShort = *(uint16_t*)(viewIt + 1);
			nextShort = nextShort & 0xFF00;
			nextShort = nextShort | state->prevByte;

			state->pTarget++;

			*(uint16_t*)(state->pTarget) = nextShort;

			length--;

			if (length == 0)
				return;
		}

		unsigned data;

		do {
			data = *(uint16_t*)(state->pTarget += 2) = *(uint16_t*)(viewIt += 2);
			length -= 2;
		} while (length > 0);

		state->prevByte = data & 0xFF;
		state->pTarget -= -length;
	}
}

// I'll just assume this is equivalent
// The original is that much more complex because it reads/writes 16bit chunks all the time
// Because it needs to be able to write to VRAM I guess
// It's also optimized to work that way (doesn't use any kind of wrapper to extract bytes or something)
void decomp_lz_simple(struct DecompState* state, unsigned distance, unsigned length) {
	uint8_t* view = state->pTarget - distance;

	for (unsigned i = 0; i < length; ++i)
		*(uint8_t*)(state->pTarget++ + 2) = *view++;
}

void decomp_read_lzref(struct DecompState* state, unsigned count) {
	unsigned offset = 1;

	for (unsigned i = 0; i < count; ++i) {
		unsigned bits = decomp_read(state, 4) + 1;

		state->lzReadLookup[i].bits = bits;
		state->lzReadLookup[i].offset = offset;

		offset += (1 << bits);
	}
}
