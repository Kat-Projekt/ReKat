#pragma once

struct fonema {
    unsigned int indice_fonema;
    double* forma;
    double* tono;
};


struct libreria {
    char* nome;
    char* artista;
    unsigned long fonemi;
    fonema* fonemi;
};

int main () {

}