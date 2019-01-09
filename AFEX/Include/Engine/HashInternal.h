#pragma once
// street cred: 
// http://web.archive.org/web/20080303102530/http://c.snippets.org/snip_lister.php?fname=crc_32.c
// http://web.archive.org/web/20080303093609/http://c.snippets.org/snip_lister.php?fname=crc.h

#include <Engine/Types.h>

namespace afex {
namespace hash {
namespace internal {
constexpr u32 table[] = { /* CRC polynomial 0xedb88320 */ 0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d };
constexpr hash32 GetCrc32(char const* str, u32 len, hash32 hashVal = 0xffffffff) {
	return len > 0 ? GetCrc32(str + 1, len - 1, table[((hashVal) ^ ((unsigned char)*str)) & 0xff] ^ ((hashVal) >> 8)) : hashVal;
}

constexpr usize StrLen(char const* str) {
	return *str ? StrLen(str + 1) + 1 : 0;
}
}
}
}


//	xxhash_cx
//	xxHash 32/64 - C++14 constexpr version.
//	https://github.com/masyos/xxhash_cx
//
//	Copyright(c) 2018, YOSHIDA Masahiro
//	MIT License
//
// xxHash document: https://github.com/Cyan4973/xxHash/wiki/xxHash-specification-(draft)
//
// Modifications by xoorath for AFEX
// - Changed styling to be consistant with AFEX


#include <array>
#include <string>
#include <limits>

#define	XXHASH_CX_XXH32_SEED	(20180101UL)
#define	XXHASH_CX_XXH64_SEED	(201801011200ULL)

namespace afex {
namespace hash {
namespace internal {

namespace details {

	template <class T>
	constexpr T rotl(T n, usize shift)
	{
		static_assert(std::numeric_limits<T>::is_integer, "integer only.");
		return ((n << shift) | (n >> (std::numeric_limits<T>::digits - shift)));
	}
	template <class T>
	constexpr T read(const char* p)
	{
		return static_cast<T>(*p);
	}
	template <>
	constexpr u8 read(const char* p)
	{
		return u8(p[0]);
	}
	template <>
	constexpr std::uint16_t read(const char* p)
	{
		return ((static_cast<std::uint16_t>(u8(p[0])) << 0)
			| (static_cast<std::uint16_t>(u8(p[1])) << 8));
	}
	template <>
	constexpr u32 read(const char* p)
	{
		return ((static_cast<u32>(u8(p[0])) << 0)
			| (static_cast<u32>(u8(p[1])) << 8)
			| (static_cast<u32>(u8(p[2])) << 16)
			| (static_cast<u32>(u8(p[3])) << 24));
	}
	template <>
	constexpr u64 read(const char* p)
	{
		return ((static_cast<u64>(u8(p[0])) << 0)
			| (static_cast<u64>(u8(p[1])) << 8)
			| (static_cast<u64>(u8(p[2])) << 16)
			| (static_cast<u64>(u8(p[3])) << 24)
			| (static_cast<u64>(u8(p[4])) << 32)
			| (static_cast<u64>(u8(p[5])) << 40)
			| (static_cast<u64>(u8(p[6])) << 48)
			| (static_cast<u64>(u8(p[7])) << 56));
	}
}


///	@brief	xxHash32
class xxHash32{
public:
	///	@brief	xxHash32 data.
	using hash_type = hash32;
	using long_type = u64;

	///	@brief	xxHash32.
	///	@param	buf		data.
	///	@param	len		data length(byte).
	///	@param	seed	seed.
	///	@return	xxHash32
	constexpr static hash_type xxh(const char* input, usize len, hash_type seed) {
		hash_type acc = 0;
		if (input == nullptr || len == 0) {
			len = 0;
			// step 1.
			acc = static_cast<hash_type>(seed + static_cast<long_type>(prime(4)));
			// step 4.
			acc = static_cast<hash_type>(static_cast<long_type>(acc) + static_cast<long_type>(len));
			// step 6.
			acc = final_mix(acc);
			// step 7.
			return acc;
		}
		const char* src{ input };
		const char* end{ src + len };
		// step 1.
		if (len >= 16) {
			const char* limit{ end - 16 };
			hash_type acc0 = static_cast<hash_type>(seed + static_cast<long_type>(prime(0)) + static_cast<long_type>(prime(1)));
			hash_type acc1 = static_cast<hash_type>(seed + static_cast<long_type>(prime(1)));
			hash_type acc2 = seed + 0;
			hash_type acc3 = seed - prime(0);
			// step 2.
			do {
				acc0 = round(acc0, details::read<u32>(src));
				src += sizeof(u32);
				acc1 = round(acc1, details::read<u32>(src));
				src += sizeof(u32);
				acc2 = round(acc2, details::read<u32>(src));
				src += sizeof(u32);
				acc3 = round(acc3, details::read<u32>(src));
				src += sizeof(u32);
			} while (src <= limit);
			// step 3.
			acc = convergence(acc0, acc1, acc2, acc3);
		}
		else {
			acc = static_cast<hash_type>(seed + static_cast<long_type>(prime(4)));
		}
		// step 4.
		acc = static_cast<hash_type>(static_cast<long_type>(acc) + static_cast<long_type>(len));
		// step 5.
		if (len >= 4) {
			const char* limit{ end - 4 };
			while (src <= limit) {
				acc = static_cast<hash_type>(
					details::rotl<hash_type>(
						static_cast<hash_type>(
							static_cast<long_type>(acc)
							+ static_cast<long_type>((details::read<u32>(src))
								* static_cast<long_type>(prime(2))))
						, 17)
					* static_cast<long_type>(prime(3))
					);
				src += sizeof(u32);
			}
		}
		if (input != nullptr) {
			while (src < end) {
				acc = static_cast<hash_type>(
					details::rotl<hash_type>(
						static_cast<long_type>(acc)
						+ static_cast<long_type>(details::read<u8>(src))
						* static_cast<long_type>(prime(4))
						, 11)
					* static_cast<long_type>(prime(0))
					);
				++src;
			}
		}
		// step 6.
		acc = final_mix(acc);
		// step 7.
		return acc;
	}

private:
	constexpr static std::array<hash_type, 5> primes{ {
		2654435761U, 2246822519U, 3266489917U, 668265263U, 374761393U
	} };
	constexpr static hash_type prime(int i) { return primes[i]; }

	constexpr static hash_type round(hash_type acc, hash_type input)
	{
		return static_cast<hash_type>(
			details::rotl<hash_type>(
				static_cast<hash_type>(static_cast<long_type>(static_cast<long_type>(acc) + (static_cast<long_type>(input) * static_cast<long_type>(prime(1)))))
				, 13)
			* static_cast<long_type>(prime(0))
			);
	}

	constexpr static hash_type convergence(hash_type acc0, hash_type acc1, hash_type acc2, hash_type acc3)
	{
		return static_cast<hash_type>(
			static_cast<long_type>(details::rotl(acc0, 1))
			+ static_cast<long_type>(details::rotl(acc1, 7))
			+ static_cast<long_type>(details::rotl(acc2, 12))
			+ static_cast<long_type>(details::rotl(acc3, 18))
			);
	}

	constexpr static hash_type final_mix(hash_type acc)
	{
		acc = static_cast<hash_type>((acc ^ (acc >> 15)) * static_cast<long_type>(prime(1)));
		acc = static_cast<hash_type>((acc ^ (acc >> 13)) * static_cast<long_type>(prime(2)));
		acc = acc ^ (acc >> 16);
		return acc;
	}
};


///	@brief	xxHash64.
class xxHash64 {
public:
	///	@brief	xxHash64 data.
	using hash_type = hash64;
	using long_type = u64;

	///	@brief	xxHash64
	///	@param	buf		data.
	///	@param	len		data length(byte).
	///	@param	seed	seed.
	///	@return	xxHash64
	constexpr static hash_type xxh(const char* input, usize len, hash_type seed)
	{
		hash_type acc = 0;
		if (input == nullptr) {
			len = 0;
			// step 1.
			acc = static_cast<hash_type>(seed + static_cast<long_type>(prime(4)));
			// step 4.
			acc += len;
			// step 6.
			acc = final_mix(acc);
			// step 7.
			return acc;
		}
		const char* src{ input };
		const char* end{ src + len };
		// step 1.
		if (len >= 32) {
			const char* limit{ end - 32 };
			hash_type acc0 = static_cast<hash_type>(seed + static_cast<long_type>(prime(0)) + static_cast<long_type>(prime(1)));
			hash_type acc1 = static_cast<hash_type>(seed + static_cast<long_type>(prime(1)));
			hash_type acc2 = seed + 0;
			hash_type acc3 = seed - prime(0);
			// step 2.
			do {
				acc0 = round(acc0, details::read<u64>(src));
				src += sizeof(u64);
				acc1 = round(acc1, details::read<u64>(src));
				src += sizeof(u64);
				acc2 = round(acc2, details::read<u64>(src));
				src += sizeof(u64);
				acc3 = round(acc3, details::read<u64>(src));
				src += sizeof(u64);
			} while (src <= limit);
			// step 3.
			acc = convergence(acc0, acc1, acc2, acc3);
		}
		else {
			acc = static_cast<hash_type>(seed + static_cast<long_type>(prime(4)));
		}
		// step 4.
		acc += len;
		// step 5.
		if (len >= 8) {
			const char* limit{ end - 8 };
			while (src <= limit) {
				acc = static_cast<hash_type>(
					details::rotl<u64>(acc ^ round(0, details::read<u64>(src)), 27)
					* static_cast<long_type>(prime(0)) + static_cast<long_type>(prime(3)));
				src += sizeof(u64);
			}
		}
		if (len >= 4) {
			const char* limit{ end - 4 };
			while (src <= limit) {
				acc = static_cast<hash_type>(
					details::rotl<hash_type>(acc ^ static_cast<hash_type>((details::read<u32>(src) * static_cast<long_type>(prime(0)))), 23)
					* static_cast<long_type>(prime(1)) + static_cast<long_type>(prime(2)));
				src += sizeof(u32);
			}
		}
		if (input != nullptr) {
			while (src < end) {
				acc = static_cast<hash_type>(
					details::rotl<hash_type>(acc ^ (details::read<u8>(src) * static_cast<long_type>(prime(4))), 11)
					* static_cast<long_type>(prime(0)));
				++src;
			}
		}
		// step 6.
		acc = final_mix(acc);
		// step 7.
		return acc;
	}

private:
	constexpr static std::array<hash_type, 5> primes{ {
		11400714785074694791ULL, 14029467366897019727ULL, 1609587929392839161ULL, 9650029242287828579ULL, 2870177450012600261ULL
	} };
	constexpr static hash_type prime(int i) { return primes[i]; }

	constexpr static hash_type round(hash_type acc, hash_type input)
	{
		acc = static_cast<hash_type>(acc + (static_cast<long_type>(input) * static_cast<long_type>(prime(1))));
		acc = details::rotl(acc, 31);
		return static_cast<hash_type>(acc * static_cast<long_type>(prime(0)));
	}

	constexpr static hash_type mearge_accumulator(hash_type acc, hash_type acc_n)
	{
		return static_cast<hash_type>(
			((acc ^ round(0, acc_n)) * static_cast<long_type>(prime(0)))
			+ static_cast<long_type>(prime(3))
			);
	}

	constexpr static hash_type convergence(hash_type acc0, hash_type acc1, hash_type acc2, hash_type acc3)
	{
		hash_type acc = static_cast<hash_type>(
			static_cast<long_type>(details::rotl(acc0, 1))
			+ static_cast<long_type>(details::rotl(acc1, 7))
			+ static_cast<long_type>(details::rotl(acc2, 12))
			+ static_cast<long_type>(details::rotl(acc3, 18))
			);
		acc = mearge_accumulator(acc, acc0);
		acc = mearge_accumulator(acc, acc1);
		acc = mearge_accumulator(acc, acc2);
		acc = mearge_accumulator(acc, acc3);
		return acc;
	}

	constexpr static hash_type final_mix(hash_type acc) {
		acc = static_cast<hash_type>((acc ^ (acc >> 33)) * static_cast<long_type>(prime(1)));
		acc = static_cast<hash_type>((acc ^ (acc >> 29)) * static_cast<long_type>(prime(2)));
		acc = (acc ^ (acc >> 32));
		return acc;
	}

};


///	@brief	xxHash32.
///	@param	buf		data.
///	@param	len		data length(byte).
///	@param	seed	seed.
///	@return	xxHash32
constexpr xxHash32::hash_type xxh32(const char* buf, usize len, xxHash32::hash_type seed) {
	return xxHash32::xxh(buf, len, seed);
}

///	@brief	xxHash64.
///	@param	buf		data.
///	@param	len		data length(byte).
///	@param	seed	seed.
///	@return	xxHash64
constexpr xxHash64::hash_type xxh64(const char* buf, usize len, xxHash64::hash_type seed) {
	return xxHash64::xxh(buf, len, seed);
}

}
}
}