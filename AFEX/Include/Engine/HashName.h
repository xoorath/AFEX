#pragma once
#include <Engine/Hash.h>

#define HASHNAME_NO_DETAILS 0

namespace afex {
namespace hash {

class HashName32 {
public:
	HashName32()
		: m_HashValue(InvalidHash32)
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
		, m_Name("INVALID")
#endif
	{
	}

	HashName32(HashName32 const&) = default;
	HashName32& operator=(HashName32 const&) = default;

	HashName32(char const* name)
		: m_HashValue(GetHash32(name))
	{
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
		m_Name = m_HashValue != InvalidHash32 ? name : "INVALID";
#endif
	}

	HashName32& operator=(char const* name) {
		m_HashValue = GetHash32(name);
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
		m_Name = m_HashValue != InvalidHash32 ? name : "INVALID";
#endif
	}

	inline bool operator == (HashName32 const& other) const {
		return GetHashValue() == other.GetHashValue();
	}

	inline bool operator == (hash32 numericHash) const {
		return GetHashValue() == numericHash;
	}

	inline bool operator == (char const* text) const {
		return GetHashValue() == GetHash32(text);
	}

	inline bool operator != (HashName32 const& other) const {
		return GetHashValue() != other.GetHashValue();
	}

	inline bool operator != (hash32 numericHash) const {
		return GetHashValue() != numericHash;
	}

	inline bool operator != (char const* text) const {
		return GetHashValue() != GetHash32(text);
	}

	inline bool IsValid() const {
		return GetHashValue() != InvalidHash32;
	}

#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
	string const& GetName() const {
		return m_Name;
	}
#endif

	hash32 GetHashValue() const {
		return m_HashValue;
	}

private:
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
	string m_Name;
#endif
	hash32 m_HashValue;

};

class HashName64 {
public:

	HashName64()
		: m_HashValue(InvalidHash64)
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
		, m_Name("INVALID")
#endif
	{
	}

	HashName64(HashName64 const&) = default;
	HashName64& operator=(HashName64 const&) = default;

	HashName64(char const* name)
		: m_HashValue(GetHash64(name))
	{
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
		m_Name = m_HashValue != InvalidHash64 ? name : "INVALID";
#endif
	}

	HashName64& operator=(char const* name) {
		m_HashValue = GetHash64(name);
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
		m_Name = m_HashValue != InvalidHash64 ? name : "INVALID";
#endif
	}

	inline bool operator == (HashName64 const& other) const {
		return GetHashValue() == other.GetHashValue();
	}

	inline bool operator == (hash64 numericHash) const {
		return GetHashValue() == numericHash;
	}

	inline bool operator == (char const* text) const {
		return GetHashValue() == GetHash64(text);
	}

	inline bool operator != (HashName64 const& other) const {
		return GetHashValue() != other.GetHashValue();
	}

	inline bool operator != (hash64 numericHash) const {
		return GetHashValue() != numericHash;
	}

	inline bool operator != (char const* text) const {
		return GetHashValue() != GetHash64(text);
	}

	inline bool IsValid() const {
		return GetHashValue() != InvalidHash64;
	}

#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
	string const& GetName() const {
		return m_Name;
	}
#endif

	hash64 GetHashValue() const {
		return m_HashValue;
	}

private:
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
	string m_Name;
#endif
	hash64 m_HashValue;

};

}
}

namespace std {
template <>
struct hash<afex::hash::HashName32> {
	afex::usize operator()(const afex::hash::HashName32& k) const {
		return afex::numeric_cast<afex::usize>(k.GetHashValue());
	}
};

template <>
struct hash<afex::hash::HashName64> {
	afex::usize operator()(const afex::hash::HashName64& k) const {
		return afex::numeric_cast<afex::usize>(k.GetHashValue());
	}
};
}