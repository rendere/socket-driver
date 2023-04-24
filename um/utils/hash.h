#pragma once
#include <cstdint>
#include <concepts>
typedef std::uint32_t Hash_t;


// Base keys
constexpr inline std::uint32_t m_uBasis = 0x811C9DC5;
constexpr inline std::uint32_t m_uPrime = 0x1000193;

// Internal funcs
template<std::integral ch = char>
inline constexpr Hash_t Hash(const ch* szString, const Hash_t uValue = m_uBasis) noexcept
{
	return ( szString[ 0 ] == '\0' ) ? uValue : Hash(&szString[ 1 ], ( uValue ^ static_cast<Hash_t>( szString[ 0 ] ) ) * m_uPrime);
}

// Operators (compile time)
inline consteval Hash_t operator""_fnv(const char* szString, size_t uLen) noexcept
{
	return Hash(szString);
}

inline consteval Hash_t operator""_fnv(const wchar_t* szString, size_t uLen) noexcept
{
	return Hash(szString);
}
