#include <linux/bitmap.h>
#include "mempool.h"
#include <debug.h>

#define BITOP_WORD(nr)          ((nr) / BITS_PER_LONG)
/**
 * gen_pool_create - create a new special memory pool
 * @min_alloc_order: log base 2 of number of bytes each bitmap bit represents
 * @nid: node id of the node the pool structure should be allocated on, or -1
 *
 * Create a new special memory pool that can be used to manage special purpose
 * memory not managed by the regular kmalloc/kfree interface.
 */

#define MAX_POOL 4
#define MAX_CHUNK 32
struct gen_pool gmempool[MAX_POOL];
struct gen_pool_chunk gchunk[MAX_POOL * MAX_CHUNK];

#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) % BITS_PER_LONG))
void bitmap_set(unsigned long *map, int start, int nr)
{
	unsigned long *p = map + BIT_WORD(start);
	const int size = start + nr;
	int bits_to_set = BITS_PER_LONG - (start % BITS_PER_LONG);
	unsigned long mask_to_set = BITMAP_FIRST_WORD_MASK(start);

	while (nr - bits_to_set >= 0) {
		*p |= mask_to_set;
		nr -= bits_to_set;
		bits_to_set = BITS_PER_LONG;
		mask_to_set = ~0UL;
		p++;
	}
	if (nr) {
		mask_to_set &= BITMAP_LAST_WORD_MASK(size);
		*p |= mask_to_set;
	}
}

unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
		unsigned long offset)
{
	const unsigned long *p = addr + BITOP_WORD(offset);
	unsigned long result = offset & ~(BITS_PER_LONG-1);
	unsigned long tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset %= BITS_PER_LONG;
	if (offset) {
		tmp = *(p++);
		tmp &= (~0UL << offset);
		if (size < BITS_PER_LONG)
			goto found_first;
		if (tmp)
			goto found_middle;
		size -= BITS_PER_LONG;
		result += BITS_PER_LONG;
	}
	while (size & ~(BITS_PER_LONG-1)) {
		if ((tmp = *(p++)))
			goto found_middle;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;
	tmp = *p;

found_first:
	tmp &= (~0UL >> (BITS_PER_LONG - size));
	if (tmp == 0UL)         /* Are any bits set? */
		return result + size;   /* Nope. */
found_middle:
	return result + __ffs(tmp);
}

unsigned long find_next_zero_bit(const unsigned long *addr, unsigned long size,
		unsigned long offset)
{
	const unsigned long *p = addr + BITOP_WORD(offset);
	unsigned long result = offset & ~(BITS_PER_LONG-1);
	unsigned long tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset %= BITS_PER_LONG;
	if (offset) {
		tmp = *(p++);
		tmp |= ~0UL >> (BITS_PER_LONG - offset);
		if (size < BITS_PER_LONG)
			goto found_first;
		if (~tmp)
			goto found_middle;
		size -= BITS_PER_LONG;
		result += BITS_PER_LONG;
	}
	while (size & ~(BITS_PER_LONG-1)) {
		if (~(tmp = *(p++)))
			goto found_middle;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;
	tmp = *p;

found_first:
	tmp |= ~0UL << size;
	if (tmp == ~0UL)        /* Are any bits zero? */
		return result + size;   /* Nope. */
found_middle:
	return result + ffz(tmp);
}

unsigned long bitmap_find_next_zero_area(unsigned long *map,
		unsigned long size,
		unsigned long start,
		unsigned int nr,
		unsigned long align_mask)
{
	unsigned long index, end, i;
again:
	index = find_next_zero_bit(map, size, start);

	/* Align allocation */
	index = __ALIGN_MASK(index, align_mask);

	end = index + nr;
	if (end > size)
		return end;
	i = find_next_bit(map, end, index);
	if (i < end) {
		start = i + 1;
		goto again;
	}
	return index;
}

struct gen_pool *gen_pool_create(int min_alloc_order)
{
	struct gen_pool *pool;
	int i;

	for (i = 0; i < MAX_POOL; i++) {
		if (gmempool[i].min_alloc_order == 0)
			break;
	}

	if (i == MAX_POOL)
		return NULL;
	else
		pool = &gmempool[i];

	coreb_msg("@@@ gen pool create%d \n", i);
	pool->chunks = &gchunk[i * MAX_CHUNK];
	pool->min_alloc_order = min_alloc_order;
	return pool;
}

int gen_pool_add(struct gen_pool *pool, unsigned long addr, unsigned long size)
{
	struct gen_pool_chunk *chunk;
	int nbits = size >> pool->min_alloc_order;
	int nbytes = (nbits + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
	int chunkid = find_next_zero_bit(pool->chunk_bits, BITS_PER_LONG, 0);

//	coreb_msg("@@@ gen pool nbits%d \n", nbits);
	if (nbits > 256)
		return -1;

	chunk = &pool->chunks[chunkid];
	if (unlikely(chunk == NULL))
		return -1;

	chunk->start_addr = addr;
	chunk->end_addr = addr + size;


//	coreb_msg("@@@ gen pool start %0x end %0x\n",
//		chunk->start_addr, chunk->end_addr);

	bitmap_set(pool->chunk_bits, chunkid, 1);

	return 0;
}

void gen_pool_destroy(struct gen_pool *pool)
{
	int _chunk;
	struct gen_pool_chunk *chunk;
	int order = pool->min_alloc_order;
	int bit, end_bit;


	for_each_set_bit(_chunk, pool->chunk_bits, BITS_PER_LONG) {
		chunk = &pool->chunks[_chunk];
		memset(chunk, 0, sizeof(*chunk));
	}

	memset(pool, 0, sizeof(*pool));
	return;
}

unsigned long gen_pool_alloc(struct gen_pool *pool, unsigned long size)
{
	int _chunk;
	struct gen_pool_chunk *chunk;
	unsigned long addr, flags;
	int order = pool->min_alloc_order;
	int nbits, start_bit, end_bit;

	if (size == 0)
		return 0;

	nbits = (size + (1UL << order) - 1) >> order;


//	coreb_msg("@@@ gen pool nbits %0x\n", nbits);

	for_each_set_bit(_chunk, pool->chunk_bits, BITS_PER_LONG) {

//		coreb_msg("@@@ gen pool chunkid %0x\n", _chunk);
		chunk = &pool->chunks[_chunk];

		end_bit = (chunk->end_addr - chunk->start_addr) >> order;

		start_bit = bitmap_find_next_zero_area(chunk->bits,
			end_bit, 0, nbits, 0);

//		coreb_msg("@@@ gen pool startbit %0x,endbit %0x\n",
//			start_bit, end_bit);
		if (start_bit >= end_bit) {
			continue;
		}

		addr = chunk->start_addr + ((unsigned long)start_bit << order);

		memset((void *)addr, 0, (nbits << order));
		bitmap_set(chunk->bits, start_bit, nbits);
		return addr;
	}
	coreb_msg("@@@bug gen pool full\n");
	coreb_dump_stack(0,0);
	return 0;
}

/**
 * gen_pool_free - free allocated special memory back to the pool
 * @pool: pool to free to
 * @addr: starting address of memory to free back to pool
 * @size: size in bytes of memory to free
 *
 * Free previously allocated special memory back to the specified pool.
 */
void gen_pool_free(struct gen_pool *pool, unsigned long addr, unsigned long size)
{
	unsigned long _chunk;
	struct gen_pool_chunk *chunk;
	unsigned long flags;
	int order = pool->min_alloc_order;
	int bit, nbits;

	nbits = (size + (1UL << order) - 1) >> order;

	for_each_set_bit(_chunk, pool->chunk_bits, BITS_PER_LONG) {
		chunk = &pool->chunks[_chunk];

		if (addr >= chunk->start_addr && addr < chunk->end_addr) {
			bit = (addr - chunk->start_addr) >> order;
			while (nbits--)
				__clear_bit(bit++, chunk->bits);
			break;
		}
	}
}

int gen_pool_check(struct gen_pool *pool, unsigned long addr, unsigned long size)
{
	unsigned long _chunk;
	struct gen_pool_chunk *chunk;
	unsigned long flags;
	int order = pool->min_alloc_order;
	int bit, nbits;

	nbits = (size + (1UL << order) - 1) >> order;

	for_each_set_bit(_chunk, &pool->chunk_bits, 32) {
		chunk = &pool->chunks[_chunk];

		if (addr >= chunk->start_addr && addr < chunk->end_addr) {
			bit = (addr - chunk->start_addr) >> order;
			while (nbits--) {
				if (!test_bit(bit++, chunk->bits))
					return 0;
			}
			return 1;
		}
	}
	return 0;
}
