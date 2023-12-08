#pragma once
#include <math.h>
#include <vector>
#include <iostream>

double F_period ( double x ) {
    // return ((sin( x ) + 2* sin ( 2*x )) / 3) ;
    return sin(x);
}

std::vector < double > get_sample ( size_t duration, double Freq, double Inte, size_t crescita ) {
    std::vector < double > samples ;
    double Derivata_C;
    for (size_t i = 0; i < duration; i++) {
        
        if ( i >= crescita && i <= duration - crescita ) {
            Derivata_C = 1;
        } else if (i < crescita ) {
            Derivata_C = (double)i / (double)crescita;
        } else {
            Derivata_C = (double)( crescita - ( i - (duration - crescita) ) ) / (double)crescita;
        }
        
        samples.push_back ( Derivata_C * Inte * F_period ( Freq * (double) ( i ) ) );
    }
    
    return samples;   
}

std::vector <double> Isinfxx ( short *Frequenze,
                               unsigned short *Intensita,
                               unsigned short *Lunghezze,
                               size_t lenth_f,
                               double bpm = 120, int sample_rate = 44100 ) 
{
    std::vector <double> Samples;
    // std::cout << "#### generating samples\n";
    // std::cout << "     bpm: " << bpm << std::endl;
    // std::cout << "     smp: " << sample_rate << std::endl;

    // std::cout << "     Fn: " << lenth_f << std::endl;
    // std::cout << "     In: " << lenth_f << std::endl;
    // std::cout << "     Ln: " << lenth_f*2-1 << std::endl;
    // std::cout << "\n";

    // somma delle Lungezze * sample rate
    size_t lungezza_totale_note = 0;
    for ( size_t i = 0; i < lenth_f*2-1; i++ ) { lungezza_totale_note += Lunghezze[i]; }

    // nota lungezza 1/4 (16) = 1 bpm
    size_t sample_x_beat = ( ( sample_rate * 60 ) / bpm ) / 16; 
    // size_t sample_totali = sample_x_beat * lungezza_totale_note;

    // trovo i sample per L
    size_t *sample_per_L = (size_t*) calloc (lenth_f*2, sizeof(size_t));
    for ( size_t i = 0; i < lenth_f*2; i++ ) {
        // std::cout << "Lt_: "<< (size_t) Lunghezze[i] * sample_x_beat << " " << Lunghezze[i] << "\n";
        sample_per_L[i] = ( (size_t) Lunghezze[i] * sample_x_beat );
    }
    // std::cout << "\n";

    // normalizzo frequenze
    double *Frequenze_M = (double*) calloc (lenth_f, sizeof(double));
    for ( size_t i = 0; i < lenth_f; i++ ) {
        // std::cout << "Ft_: "<< 440 * pow ( 2, ((double)Frequenze[i]) / 12.0 ) << " " << Frequenze[i] << "\n";
        Frequenze_M [i] = ( 440.0 * pow ( 2, ((double)Frequenze[i]) / 12.0 ));
    }
    // std::cout << "\n";

    double *Frequenze_N = (double*) calloc (lenth_f, sizeof(double));
    for ( size_t i = 0; i < lenth_f; i++ ) {
        Frequenze_N [i] = ( Frequenze_M[i] * 2 * M_PI / sample_rate );
    }

    // normalizzo intesinta
    std::vector <double> Intensita_M;
    for ( size_t i = 0; i < lenth_f; i++ ) {
        // std::cout << "It_: "<< (double) Intensita[i] / 65536 << " " << Intensita[i] << "\n";
        Intensita_M.push_back ( (double) Intensita[i] / 65536);
    }
    // std::cout << "\n";

    // print 
    // for ( auto F : Frequenze_M )  { std::cout << "F: " << F << "\n";}
    // for ( auto L : sample_per_L ) { std::cout << "L: " << L << "\n";}
    // for ( auto I : Intensita_M )    { std::cout << "I: " << I << "\n";}
    // std::cout << "generating samples\n";
    size_t comulative_x = 0;
    for (size_t i = 0; i < lenth_f*2-1; i++)
    {
        // std::cout << "generating sample: "<< i+1 << std::endl;
        size_t current_L = sample_per_L[i];
        
        if ( (i + 1) % 2 == 0) {
            // std::cout << "\tgenerating pause: "<< i+1 << std::endl;

            for (size_t x = 0; x < current_L; x++) {
                Samples.push_back ( 0 );
            }
        } else {    
            // std::cout << "\tgenerating sample: "<< i+1 << std::endl;

            std::vector <double> sample_L = get_sample ( current_L, Frequenze_N[i/2],Intensita_M[i/2], (size_t)sample_per_L[0] / 8 );
            Samples.insert(Samples.end(), sample_L.begin(), sample_L.end());
        }
        comulative_x += current_L;
    }
    return Samples;
}
