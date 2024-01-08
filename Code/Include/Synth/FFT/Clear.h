#ifndef CLEAR
#define CLEAR
#include "FastFourierTransform.h"

void clear_signal ( complex <double> *t, int size, double base ) {
	for ( int i = 0; i < size; i++ ) {
		if ( abs(t[i]) < base ) 
		{ t[i] = 0; }
	}
}

void clamp_freq ( complex <double> *t, int size, int Min, int Max ) {
	for ( int i = 0; i < size; i++ ) { 
		if ( i < Min ) { t[i] = 0; }
		if ( i > Max ) { t[i] = 0; }		
	}
}

auto Derviate_Main ( complex <double> *t, int size ) {
	auto _buf = ( complex <double> * ) calloc ( size, sizeof( complex <double> ) );
	double last = abs(t[0]);
	double curr = abs(t[1]);
	double next;
	for ( size_t i = 1; i < size-1; i++ ) {
		next = abs(t[i+1]);
		// abs derivate
		if ( last <= curr && curr <= next ) { _buf[i] = curr; }
		last = curr;
		curr = next;
	}

	return _buf;
}

#endif