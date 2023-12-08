#pragma once

// return  X % m
#define num unsigned long long
#define mod(x,m) x >= m ? x % m : x

inline void swap 
( num &a, num &b ) {
	num c = a;
	a = b;
	b = c;
}

num add
( num a, num b, num c ) {
	if 
	( ! ( c > 0 && 0 <= a && a < c && 0 <= b && b < c ) ) {
		throw ( "numbers not corrected \n" );
	}
    num room = (c - 1) - a;
    if (b <= room)
        a += b;
    else
        a = b - room - 1;
    return a;
}

num mult
( num a, num b, num c ) {
    num result = 0;
    a = mod ( a, c );
    b = mod ( b, c );

    if ( b > a ) { swap(a, b); }
        
    while 
	( b ) {
        if (b & 0x1) { result = add(result, a, c); }
        a = add ( a, a, c );
        b >>= 1;
    }

    return result;
}

/// @brief This function calculates a ^ b % m, all values are positive integers.
/// This program uses Binary Exponentiation
/// @param a The base of the exponent
/// @param b The exponent 
/// @param m The modulus
/// @return a ^ b % m
num pow
( num a, num b, num m ) {
	a = mod ( a, m );
	num res = 1;
	while 
	(b > 0) {
		if (b & 1) { res = mod( mult ( res, a, m), m ); }
		a = mod ( mult ( a, a, m), m );
		b >>= 1;
	}
	return res;
}

/// @brief Verify if n is a prime number
/// @return True if the number is prime
bool is_prime 
( num n ) {
	if ( n == 2 || n == 3 )
		return true;

	if ( n <= 1 || n % 2 == 0 || n % 3 == 0 )
		return false;

	for ( int i = 5; i * i <= n; i += 6 ) {
		if (n % i == 0 || n % (i + 2) == 0)
			return false;
	}

	return true;
}

/// @brief Calculate and returns the great commun diviser of a and b
num gcd 
( num a, num b ) {
	while (b != 0) {
		num c = a;

		a = b;
		b = c % b;
	}

	return a;
}

/// @brief Calulate the inverse multiplicative using the Euclid methos
num inverso_molti 
( num e, num t ) {
	num d = 0;
	num x1 = 0;
	num x2 = 1;
	num y1 = 1;
	num temp_phi = t;

	while (e > 0) {
		num temp1 = temp_phi / e;
		num temp2 = temp_phi - temp1 * e;
		temp_phi = e;
		e = temp2;

		num x = x2 - temp1 * x1;
		num y = d - temp1 * y1;

		x2 = x1;
		x1 = x;
		d = y1;
		y1 = y;
	}

	return d + t;
}