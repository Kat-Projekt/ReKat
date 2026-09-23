#include "debugger.h"

template <typename T>
struct ArrayView
{
	const T* _data = nullptr;
	size_t _size = 0;

	constexpr const T* begin ( ) const noexcept
	{ return _data; }

	constexpr const T* end ( ) const noexcept
	{ return _data + _size; }

	constexpr size_t size ( ) const noexcept
	{ return _size; }

	constexpr const T& operator [ ] ( size_t i ) const
	{
		if ( i < _size )
		{ return _data [i]; }
		else
		{ 
			DEBUG ( DebugLevel::ERROR, "out of bounds" );
			return T {};
		}
	}
};