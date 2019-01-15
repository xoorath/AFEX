// Used my xo-alloc as a starting point:
// https://github.com/xoorath/xo-alloc/blob/master/xo-alloc.h
#pragma once


#include <new>
#include <Engine/Types.h>

namespace afex {

struct BlockAllocatorInfo {
	u32 size;
	u32 bytesFree;
	u32 bytesUsed;
	u32 bytesWasted;
	u32 blockCount;
};

template<u32 SIZE>
class BlockAllocator {
	static_assert(SIZE < (1 << 31), "BlockAllocator doesn't support being larger than 1^31");
public:

	////////////////////////////////////////////////////////////////////// BlockAllocator API

	template<typename T, typename...Args>
	T* New(Args...args) {
		void* mem = static_cast<void*>(InternalMallocT<sizeof(T)>());
		return mem ? new(mem) T(args...) : nullptr;
	}

	template<typename T>
	void Delete(T* m) {
		if (m) {
			m->~T();
			InternalFree(static_cast<void*>(m));
		}
	}

	void* Malloc(size_t size) {
		return InternalMalloc(size);
	}

	void Free(void* m) {
		if (m) {
			InternalFree(m);
		}
	}

	BlockAllocator() {
		Block* b = reinterpret_cast<Block*>(m_Buffer);
		b->Free = true;
		b->Size = SIZE - sizeof(Block);
	}

	BlockAllocatorInfo ReportInfo() {
		BlockAllocatorInfo info;
		info.size = SIZE;
		info.bytesFree = 0;
		info.bytesUsed = 0;
		info.bytesWasted = 0;
		info.blockCount = 0;

		Block* i = static_cast<Block*>(Begin());
		Block* e = static_cast<Block*>(End());
		for (; i < e; i = i->Next()) {
			info.blockCount++;
			if (i->Free) {
				info.bytesFree += i->Size;
			}
			else {
				info.bytesUsed += i->Size;
			}
		}
		info.bytesWasted = info.size - (info.bytesFree + info.bytesUsed);
		return info;
	}

private:
	////////////////////////////////////////////////////////////////////// BlockAllocator Internal

	char m_Buffer[SIZE];

	void* Begin() { return static_cast<void*>(m_Buffer); }
	void* End() { return static_cast<void*>(static_cast<char*>(m_Buffer) + SIZE); }

	struct Block {
		u32 Free : 1;
		u32 Size : 31;

		Block* Next() const {
			return reinterpret_cast<Block*>((char*)(this) + Size + sizeof(Block));
		}

		Block* Previous(Block* s) const {
			while ((char*)s + s->Size + sizeof(Block) < (char*)this) {
				s = s->Next();
			}
			return s;
		}
	};

	static_assert(sizeof(BlockAllocator<SIZE>::Block) == 4, "Block isn't 32 bits. Why not?");
	
	static void JoinBlocks(Block* b, Block* e, Block* m) {
		if (m->Free) {
			Block* n = m->Next();
			Block* p = m->Previous(b);
			// consume the next block, if it's free.
			if (n < e && n->Free) {
				m->Size += n->Size + sizeof(Block);
				// n is now invalid.
			}

			// consume the previous block.
			if (p != m && p->Free) {
				p->Size += m->Size + sizeof(Block);
				// m is now invalid.
			}
		}
	}

	void* InternalMalloc(u32 size) {
		Block* i = static_cast<Block*>(Begin());
		Block* e = static_cast<Block*>(End());
		for (; i < e; i = i->Next()) {
			if (i->Free && i->Size >= size) {
				i->Free = false;
				intptr_t oldSize = i->Size;
				i->Size = size;
				Block* n = i->Next();
				intptr_t nextSize = (oldSize - size - sizeof(Block));
				// if there's not enough space for the next block (meaning n is invalid)
				if (nextSize <= sizeof(Block)) {
					i->Size += nextSize + sizeof(Block);
					n = i;
				}
				// otherwise, break our block in half, creating a new next block. 
				else {
					n->Free = true;
					n->Size = nextSize;
				}
				return reinterpret_cast<char*>(i + 1);
			}
		}
		return nullptr;
	}

	template<u32 size>
	void* InternalMallocT() {
		static_assert(size < SIZE - sizeof(Block), "Allocation requested is larger than the allocator.");
		return InternalMalloc(size);
	}

	void InternalFree(void* mem) {
		Block* m = reinterpret_cast<Block*>(mem) - 1;
		Block* i = static_cast<Block*>(Begin());
		Block* e = static_cast<Block*>(End());

		if (m < i || m > e) {
			return;
		}
		m->Free = true;
		JoinBlocks(i, e, m);
	}
};

}