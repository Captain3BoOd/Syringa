#pragma once

#include <initializer_list>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

template<typename element>
inline bool in(const element& Elem, const std::initializer_list<element>& list)
{
	for (const element& elem : list)
		if (Elem == elem) return true;

	return false;
}

template<typename element, typename element2>
inline bool in(const element& Elem, const std::initializer_list<element2>& list)
{
	for (const element2& elem : list)
		if (Elem == elem) return true;

	return false;
}

template<typename element, typename elements>
inline bool in(const element& Elem, const elements& list)
{
	for (const element& elem : list)
		if (Elem == elem) return true;

	return false;
}

template<typename element, size_t size>
inline bool in(const element& Elem, const element list[])
{
	for (size_t i = 0; i < size; i++)
		if (Elem == list[i]) return true;

	return false;
}
