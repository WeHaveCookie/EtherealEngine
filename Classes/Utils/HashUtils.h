#pragma once
#include <cstdint>

inline uint32_t HashU32(const uint8_t* bytes, size_t count, uint32_t basis = 2166136261U)
{
	// FNV-1a hash function
	const uint32_t FNV_offset_basis = basis;
	const uint32_t FNV_prime = 16777619U;

	uint32_t hash = FNV_offset_basis;
	for (uint32_t i = 0; i < count; ++i)
	{
		hash ^= (uint32_t)bytes[i];
		hash *= FNV_prime;
	}

	return hash;
}

template <class T>
inline uint32_t HashU32(const T& val)
{
	return HashU32((const uint8_t*)&val, sizeof(val));
}

template <class T>
inline uint32_t HashU32Combine(const T& val, uint32_t baseHash)
{
	return HashU32((const uint8_t*)&val, sizeof(val), baseHash);
}

inline uint64_t HashU64(const uint8_t* bytes, size_t count, uint64_t basis = 14695981039346656037ULL)
{
	// FNV-1a hash function
	const uint64_t FNV_offset_basis = basis;
	const uint64_t FNV_prime = 1099511628211ULL;

	uint64_t hash = FNV_offset_basis;
	for (uint64_t i = 0; i < count; ++i)
	{
		hash ^= (uint64_t)bytes[i];
		hash *= FNV_prime;
	}

	return hash;
}

template <class T>
inline uint64_t HashU64(const T& val)
{
	return HashU64((const uint8_t*)&val, sizeof(val));
}

template <class T>
inline uint64_t HashU64Combine(const T& val, uint64_t baseHash)
{
	return HashU64((const uint8_t*)&val, sizeof(val), baseHash);
}
