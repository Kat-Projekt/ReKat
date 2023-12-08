#pragma once

#include "crypt_math.hpp"
#include <iostream>
#include <time.h>
#include <stdlib.h>

namespace RSA {
	typedef struct {
		num e;
		num n;
	} encrypt_info;

	class crypt
	{
		private:
			num e_key;
			num d_key;
			
			num n_prod;
			num tozinente;

			void find_Keys( num p, num q );

		public:
			 crypt ( ) { }
			 crypt ( num p, num q );
			~crypt ( );

			encrypt_info get_encrypt_info ( );

			char* encrypt ( char* _buf, size_t *_n, encrypt_info *key = nullptr );
			char* decrypt ( char* _buf, size_t *_n );
			template < typename N >
			N encrypt ( N n, encrypt_info *_key = nullptr );
			template < typename N >
			N decrypt ( N n ); 


	};
	
	void crypt::find_Keys 
	( num p, num q ) {
		// set a seed for genereting a random e
		srand ( time ( 0 ) );

		// find a fitting e
		e_key = rand ( ) % tozinente;
		while ( gcd ( e_key, tozinente ) != 1 ) { e_key = rand ( ) % tozinente; }

		// find d
		d_key = inverso_molti ( e_key, tozinente );
	}

	crypt::crypt
	( num p, num q ) {
		// controlla gli input
		if ( p == q ) 
		{ throw ("the primes must not be the same\n"); }
		if ( ! ( is_prime ( p ) && is_prime ( q ) ) ) 
		{ throw ("one of the inserted values is not prime \n"); }

		// calcola le componenti fondamentali
		n_prod = p * q;
		tozinente = ( p - 1 ) * ( q - 1 );

		// torva le soluzioni al problema ( e & d )
		find_Keys ( p, q );
	}
	crypt::~crypt
	( ) { }

	encrypt_info crypt::get_encrypt_info 
	( ) {
		encrypt_info r;
		r.n = n_prod;
		r.e = e_key;
		return r;
	}

	char* crypt::encrypt 
	( char* _buf, size_t *_n, encrypt_info *_key ) {
		// getting encr infos
		num key = e_key;
		num prod = n_prod;

		// verify if using foreign key
		if ( _key != nullptr ) 
		{ key = _key->e; prod = _key->n; }
		std::cout << "e: "<< key << "n: " << prod << std::endl;

		// dividing the _buf in 6 bytes chunks
		size_t _n_subuffers = ( *_n - *_n % 6 ) / 6 + 1;
		num *_int_buf = (num*) calloc ( _n_subuffers, sizeof(num) );

		// copy buffer
		for 
		(size_t i = 0; i < _n_subuffers; i++) {
			num _current_value = 0;
			char* _current_buf = (char*)&_current_value;
			char _values_to_copy = 6;
			for 
			(size_t y = 2; y < _values_to_copy; y++) {
				_current_buf[ 2 + y ] = _buf[ 6 * i + y ]; 
			}
			_int_buf[i] = _current_value;
		}

		// encrypt
		for 
		(size_t i = 0; i < _n_subuffers; i++) {
			std::cout << "pre-encrt: " << i << " " << _int_buf[i] << '\n';
			if 
			( _int_buf[i] > prod ) { 
				std::cerr << "n is more than prod cannot decrtpt\n";
				throw ("n is less than prod cannot encrtpt\n"); 
			}
			_int_buf[i] = pow ( _int_buf[i], key, prod ); 
			std::cout << "post-encrt: " << i << " " << _int_buf[i] << '\n';
		}

		*_n = _n_subuffers * sizeof(num);
		return (char*) _int_buf;
	}
	char* crypt::decrypt 
	( char* _buf, size_t *_n ) {
		std::cout << "d: " << d_key << '\n';

		// decrypt
		for 
		(size_t i = 0; i < *_n >> 3; i++) { 	
			std::cout << "pre-decrt: " << i << " " << ((num*)_buf)[ i ] << '\n';
			if 
			( ((num*)_buf)[ i ] > n_prod ) { 
				std::cerr << "n is more than prod cannot decrtpt\n";
				throw ("n is more than prod cannot decrtpt\n"); 
			}
			((num*)_buf)[ i ] = pow ( ((num*)_buf)[ i ], d_key, n_prod ); 
			std::cout << "post-decrt: " << i << " " << ((num*)_buf)[ i ] << '\n';
		}

		// ripuslisci _buf dagli zeri iniziali

		size_t _n_subuffers = *_n >> 3 + 1;
		num *_int_buf = (num*) calloc ( _n_subuffers, sizeof(num) );

		// copy buffer
		for 
		(size_t i = 0; i < _n_subuffers; i++) {
			num _current_value = 0;
			char* _current_buf = (char*)&_current_value;
			char _values_to_copy = 6;
			for 
			(size_t y = 2; y < _values_to_copy; y++) {
				_current_buf[ 2 + y ] = _buf[ 6 * i + y ]; 
			}
			_int_buf[i] = _current_value;
		}

		return _buf;
	}

	template < typename N >
	N crypt::encrypt ( N n, encrypt_info *_key ) {
		// verify if using foreign key
		if ( _key != nullptr ) 
		{ return pow ( n, _key->e, _key->n ); }
		return pow ( n, e_key, n_prod );
	}
	
	template < typename N >
	N crypt::decrypt ( N n ) {
		return pow ( n, d_key, n_prod );
	}
}

#include <iostream>
std::ostream& operator << ( std::ostream& os, const RSA::encrypt_info& key ) {
	os << "n:  " << key.n;
	os << ", e: " << key.e;
	
	return os;
}

#include <fstream>
RSA::crypt get_crt 
( std::string path = "primes.txt" ) {
	// carica file with primes
	// choose a random number
	return RSA::crypt ( 440581159, 204869779 );
}