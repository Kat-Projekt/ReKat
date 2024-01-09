#include <iostream>
#include <complex>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

int log2 ( int N ) {
	int k = N , i = 0 ;
	while ( k ) { k >>= 1 ; i++; }
	return i -1;
}
int potenza2 ( int n ) {
	int h = n ; 
	if ( ! ( n > 0 && ( n & ( n - 1 ) ) ) ) { return n ; }
	else {
		while ( h & ( h -1) ) 
		{ h = h & ( h -1) ; }
		h = h << 1 ;
	}
	return h ;
}

int invertibit ( int N , int n ) {
	int j , r = 0 ; 
	for ( j = 1 ; j <= log2 ( N ) ; j++ ) {
		if ( n & ( 1 << ( log2 ( N ) - j ) ) )
		{ r |= 1 << ( j - 1 ) ; }
	}
	return r;
}

template < typename T >
void ordina ( std::vector < complex < T > > &vec ) { 
	int n = vec.size ( );
	std::vector < complex < T > > vec2;
	vec2.resize ( n );
	for ( int i = 0 ; i < n ; i++ )
	{ vec2 [ i ] = vec [ invertibit ( n , i ) ] ; }
	for ( int j = 0 ; j < n ; j++ )
	{ vec [ j ] = vec2 [ j ] ; }
}
/*
template < typename T >
void FFT ( std::vector < complex < T > > &vec1, T t ) {
	// resise vec1 so it will have size as a power of 2
	vec1.resize ( pow ( 2, log2( (int)vec1.size() ) + 1 ) );
	int N = vec1.size ( );
	std::vector < complex < T > > W;
	W.resize ( N / 2 );
	W [ 1 ] = polar ( 1. , 2. * M_PI / N ) ;
	W [ 0 ] = 1 ;
	for ( int i = 2 ; i < N / 2 ; i++ ) 
	{ W [ i ] = pow ( W [ 1 ] , i ) ; }
	int n = 1 ;
	int a = N / 2 ;
	std::cout << " samples " << N << " log " << log2 ( N ) << " pow " << pow ( 2, log2(N) ) << "\n";
	for ( int j = 0 ; j < log2 ( N ) ; j++ ) {
		for ( int i = 0 ; i < N ; i++ ) {
			if ( ! ( i & n ) ) {
				complex < T > temp = vec1 [ i ] ;
				complex < T > Temp = W [ ( i * a ) % ( n * a ) ] * vec1 [ i + n ] ;
				vec1 [ i ] = temp + Temp ;
				vec1 [ i + n ] = temp - Temp ;
			}
		}
		n *= 2 ;
		a = a / 2 ;
	}
	for ( int k =0; k < N ; k++ ) { vec1 [ k ]*=( t * sqrt ( N ) ) ; }
}

template < typename T >
std::vector < complex < T > > IFFT ( std::vector < complex < T > > &vec1, T t ) {
	std::vector < complex < T > >::iterator it;
	std::vector < complex < T > > test;
	for (it = vec1.begin ( ); it != vec1.end ( ); ++it) {
		*it = std::conj ( *it );
	}
	
	ordina < T > ( vec1 );
	FFT < T > (vec1, t);
	test = vec1;

	for (it = test.begin ( ); it != test.end ( ); ++it) {
		*it = std::conj ( *it );
	}

	T rev = 1 / test.size ( );
	for (it = test.begin ( ); it != test.end ( ); ++it) {
		*it *= rev;
	}

	return test;
}

template < typename T >
struct Freq {
	int freq;
	T Int;
};

template < typename T >
std::vector < Freq < T > > Reformat ( std::vector < complex < T > > &vec1, T threashold ) {
	std::vector < Freq < T > > R;
	int c = vec1.size( );
	T scale = 2 / sqrt ( vec1.size ( ) );
	// find the mid point
	int M = ( c + 1 ) /2;
	for ( int i =0; i < ( ( c / 2 ) ) ; i++) {
		T p = abs ( vec1 [ c/2+i ] ) * scale;
		if ( p > threashold ) 
		{ R.push_back ( { M - i, p } ); }
	}
	return R;
}

template < typename T >
std::vector < Freq < T > > Get_Freq ( std::vector < T > _vec, T _sample_rate, T threashold ) {
	std::cout << "called\n";
	std::vector < complex < T > > vec;
	vec.resize ( _vec.size ( ) );
	// copy vector
	std::cout << "copy\n";
	for (size_t i = 0; i < _vec.size ( ); i++) 
	{ vec [i] = _vec [i]; }
	// apply transform
	std::cout << "ordina\n";
	ordina < T > ( vec );
	std::cout << "FFT " << vec.size() << " " << 1 / _sample_rate << '\n';
	FFT < T > ( vec, 1 / _sample_rate );
	std::cout << "reformat\n";
	return Reformat < T > ( vec, threashold );
}

template < typename T >
std::vector < complex < T > > Get_Tranform ( std::vector < T > &_vec, T _sample_rate ) {
	std::cout << "called\n";
	std::vector < complex < T > > vec;
	vec.resize ( _vec.size ( ) );
	// copy vector
	std::cout << "copy\n";
	for (size_t i = 0; i < _vec.size ( ); i++) 
	{ vec [i] = _vec [i]; }
	// apply transform
	std::cout << "ordina\n";
	ordina < T > ( vec );
	std::cout << "FFT\n";
	FFT < T > ( vec, 1 / _sample_rate );
	return vec;
}

/*int _main ( ) {
	ofstream output ;
	output.open ( "file.csv " );

	double c = 4096;
	double j = 0;
	std::vector < double > vec1;
	vec1.resize ( c );
	output << "Normal\n";
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; output << vec1[i] << '\t'; }

	auto v = Get_Tranform < double > ( vec1, c );
	IFFT < double > ( v, c );

	output << "Re trasformed\n";
	for ( auto p : v ) 
	{ output << p.real() << '\t'; }
	return 0;
}


int _main ( ) {
	ofstream output ;
	output.open ( "file.csv " );

	double c = 4096;
	double j = 0;
	std::vector < double > vec1;
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	auto r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	c = 2048;
	j = 0;
	vec1.clear ( );
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	c = 1024;
	j = 0;
	vec1.clear ( );
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	c = 512;
	j = 0;
	vec1.clear ( );
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	c = 256;
	j = 0;
	vec1.clear ( );
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	c = 128;
	j = 0;
	vec1.clear ( );
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	c = 64;
	j = 0;
	vec1.clear ( );
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	c = 32;
	j = 0;
	vec1.clear ( );
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	c = 16;
	j = 0;
	vec1.clear ( );
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	c = 8;
	j = 0;
	vec1.clear ( );
	vec1.resize ( c );
	for ( int i =0; i < c ; i++) 
	{ vec1 [ i ] = sin ( 6 * M_PI * j ) + 2 * sin ( 8 * M_PI * j ); j += 1 / c; }

	r = Get_Freq < double > ( vec1, c, 0.1 );
	output << "Bals " << log2 ( c ) << '\n';

	for ( auto f : r ) { output << "Freq: " << f.freq << " Int: " << f.Int << '\n'; }

	output . close ( ) ;
	return 0 ; 
}*/