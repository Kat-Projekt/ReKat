#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include <math.h>
#include <stdexcept>
using std::filesystem::exists;

// this module allows tho load and store the STATE of the kat audio
// l'estensione del file è .xdg (expand dong)

class STATE
{
private:  
    // the path of th file
    std::string file_path;

    // file basic informations
    size_t sample_rate;
    double bpm;
    size_t Lenth_F;

    // buffers
    short          *Frequenze;
    unsigned short *Intensita;
    unsigned short *Lunghezze;
    char           *Frase;

public:
    /**
     * @brief Construct a new STATE object
     * 
     */
     STATE ( );
    /**
     * @brief Construct a new STATE object
     * 
     * @param file_path 
     * @param dimension 
     */
     STATE ( std::string file_path, size_t dimension = 1 );
    /**
     * @brief Destroy the STATE object
     * 
     */
    ~STATE ( );

    /**
     * @brief resize the vocals buffer
     * 
     * @param dimension 
     * @param set_up 
     * @return int 
     */
    int resize_buff ( size_t dimension, bool set_up = false );

    /**
     * @brief load the buffers and basic infos from the specified file
     * 
     * @param path 
     * @return int 
     */
    int load ( std::string path = "");
    /**
     * @brief saves the buffers and basic infos in the specified file
     * 
     * @param path 
     * @param forze if true : save even if the file exists
     * @return int 
     */
    int save ( std::string path = "", bool forze = false );

    /**
     * @brief Get the lenth
     * 
     * @return size_t 
     */
    size_t get_lenth ( );
    /**
     * @brief Get the sample_rate
     * 
     * @return size_t 
     */
    size_t get_sampl ( );
    /**
     * @brief Get the bpm 
     * 
     * @return double 
     */
    double get_bpm ( );

    /**
     * @brief Set the sample_rate
     * 
     * @param sample_rate 
     * @return size_t 
     */
    size_t set_sampl ( size_t sample_rate );
    /**
     * @brief Set the bpm
     * 
     * @param bpm 
     * @return double 
     */
    double set_bpm ( double bpm );

    // vaious get/set methods
    short get_frequ ( uint frame );
    short get_inten ( uint frame );
    short get_frlen ( uint frame );
    char* get_chars ( );

    short set_frequ ( uint frame, short value );
    short set_inten ( uint frame, short value );
    short set_frlen ( uint frame, short value );        
    char* set_chars ( char* caracters );

    short          *get_all_frequ ();
    unsigned short *get_all_inten ();
    unsigned short *get_all_lenth ();

    /**
     * @brief adds an empty frame
     * 
     */
    void add_frame ( );
    /**
     * @brief adds a frame with the specified parameters
     * 
     * @param short 
     * @param short 
     */
    void add_frame ( short, unsigned short, 
                    unsigned short, unsigned short );
    /**
     * @brief modifyes a frame based on the given paramenters
     * 
     * @param short 
     * @param short 
     * @return int 
     */
    int frame ( size_t, short, unsigned short, 
                unsigned short, unsigned short );
};

int STATE::frame ( size_t P, short F, unsigned short I, 
             unsigned short P1, unsigned short L1 ) {
    if ( P >= Lenth_F ) { throw std::invalid_argument ("value out of borders"); return 1; }
    set_frequ ( P, F );
    set_inten ( P, I );
    set_frlen ( (P)*2, L1 );
    set_frlen ( (P)*2+1, P1 );
    return 0;
}
void STATE::add_frame ( ) { resize_buff ( Lenth_F + 1 ); }
void STATE::add_frame ( short F, unsigned short I, 
                       unsigned short P1, unsigned short L1 ) {
    resize_buff ( Lenth_F + 1 );
    set_frequ ( Lenth_F-1, F );
    set_inten ( Lenth_F-1, I );
    set_frlen ( (Lenth_F-1)*2, L1 );
    set_frlen ( (Lenth_F-1)*2+1, P1 );
}

STATE::STATE () {
    // std::cout << "CONSTUCTOR\n";
    sample_rate = 44100;
    bpm = 120.0;
    resize_buff(0,true);
}
STATE::STATE ( std::string file_path, size_t dimension ) {
    sample_rate = 44100;
    bpm = 120.0;
    resize_buff (dimension,true);

    this->file_path = file_path;
    if ( exists ( file_path ) ) {
        load ( ); 
    }
}
STATE::~STATE () {
    free ( Frequenze );
    free ( Intensita );
    free ( Lunghezze );
}

int STATE::resize_buff ( size_t dimension, bool set_up ) {
    // std::cout << "START_RESIZE: " << dimension << " MODE: " << (set_up?"set_up":"resize")<< "\n";
    size_t old_lenth = Lenth_F;
    Lenth_F = (dimension);
    if ( Lenth_F == old_lenth ) { return 0; }

    // std::cout << "reallocating: " << Lenth_F << "\n";
    
    if ( set_up ) {
        Frequenze = (short*) calloc (Lenth_F, sizeof(short));
        Intensita = (unsigned short*) calloc (Lenth_F,sizeof(unsigned short));
        Lunghezze = (unsigned short*) calloc (Lenth_F*2,sizeof(unsigned short));

        for (size_t i = 0; i < Lenth_F; i++) {
            Frequenze[i] = 0;
            Intensita[i] = 0;
            Lunghezze[i*2] = 0;
            Lunghezze[i*2+1] = 0;
        }
    } else {
        Frequenze = (short*) realloc (Frequenze, Lenth_F * 2);
        Intensita = (unsigned short*) realloc (Intensita, Lenth_F * 2);
        Lunghezze = (unsigned short*) realloc (Lunghezze, Lenth_F * 4);
        
        for (size_t i = old_lenth; i < Lenth_F; i++) {
            Frequenze[i] = 0;
            Intensita[i] = 0;
            Lunghezze[i*2] = 0;
            Lunghezze[i*2+1] = 0;
        }   
    }

    // std::cout << "MALLOC\n";
    return 0;
}

int STATE::load ( std::string path ) {
    if ( path != "" ) { file_path = path; }
    if ( file_path == "" ) { return -1; }
    // verify that the file exist
    if ( !exists ( file_path ) ) {

        return 1;
    }
    // load file
    std::ifstream file ( file_path );

    file.read ((char*) & Lenth_F, 8);
    file.ignore ();
    file.read ((char*) & bpm, 8);
    file.ignore ();
    file.read ((char*) & sample_rate, 8);
    file.ignore ();
    file >> Frase;
    file.ignore ();
    // std::cout << "get_basic\n";
    resize_buff ( Lenth_F,true );
    // std::cout << "set_buff\n";

    char* F = (char*) malloc (Lenth_F * 2);
    file.read( F, Lenth_F*2 );
    for (size_t i = 0; i < Lenth_F; i++) {
        short t = 0;
        char* t1 = (char*) & t;
        t1[0] = F[i*2];
        t1[1] = F[i*2+1];
        // std::cout << t << std::endl;
        Frequenze[i] = t;        
    }
    file.ignore();

    char* I = (char*) malloc (Lenth_F * 2);
    file.read( I, Lenth_F*2 );
    for (size_t i = 0; i < Lenth_F; i++) {
        short t = 0;
        char* t1 = (char*) & t;
        t1[0] = I[i*2];
        t1[1] = I[i*2+1];
        // std::cout << t << std::endl;
        Intensita[i] = t;        
    }
    file.ignore();

    char* L = (char*) malloc (Lenth_F * 4 - 2);
    file.read( L, Lenth_F*4 -2 );
    for (size_t i = 0; i < Lenth_F*2; i++) {
        short t = 0;
        char* t1 = (char*) & t;
        t1[0] = L[i*2];
        t1[1] = L[i*2+1];
        // std::cout << t << std::endl;
        Lunghezze[i] = t;        
    }

    return 0;
}
int STATE::save ( std::string path , bool forze ) {
    if ( path != "" ) { file_path = path; }
    if ( file_path == "" ) { return -1; }
    // verify that the file exist
    if ( !forze ) { if ( exists ( file_path ) ) { return 1; } }
    

    // load file
    std::ofstream file ( file_path );

    char* lenF = (char*) & Lenth_F;
    for (char i = 0; i < 8; i++) { file.put(lenF[i]); }
    file << std::endl;

    char* bpmC = (char*) & bpm;
    for (char i = 0; i < 8; i++) { file.put(bpmC[i]); }
    file << std::endl;

    char* samp = (char*) & sample_rate;
    for (char i = 0; i < 8; i++) { file.put(samp[i]); }
    file << std::endl;

    file << Frase << std::endl;

    for ( size_t i = 0; i < Lenth_F; i++ ) {
        char* f = (char*) & Frequenze[i];
        for (char i = 0; i < 2; i++) { file.put(f[i]); }
    }

    file << std::endl;

    for ( size_t i = 0; i < Lenth_F; i++ ) {
        char* f = (char*) & Intensita[i];
        for (char i = 0; i < 2; i++) { file.put(f[i]); } 
    }
    file << std::endl;

    for ( size_t i = 0; i < Lenth_F*2; i++ ) {
        char* f = (char*) & Lunghezze[i];
        for (char i = 0; i < 2; i++) { file.put(f[i]); }
    }

    return 0;
}

size_t STATE::get_lenth ( ) { return Lenth_F; }
size_t STATE::get_sampl ( ) { return sample_rate; }
double STATE::get_bpm   ( ) { return bpm; }

size_t STATE::set_sampl ( size_t sample_rate ) {
    this->sample_rate = sample_rate;
    return this->sample_rate;
}
double STATE::set_bpm ( double bpm ) {
    this->bpm = bpm;
    return this->bpm;
}

short STATE::get_frequ ( uint frame ) { return Frequenze [frame]; }
short STATE::get_inten ( uint frame ) { return Intensita [frame]; }
short STATE::get_frlen ( uint frame ) { return Lunghezze [frame]; }

short STATE::set_frequ ( uint frame, short value ) {
    if ( frame + 1  > Lenth_F ) { return -1; }

    Frequenze [frame] = value;
    return Frequenze [frame];
}
short STATE::set_inten ( uint frame, short value ) {
    if ( frame + 1  > Lenth_F ) { return -1; }

    Intensita [frame] = value;
    return Intensita [frame];
}
short STATE::set_frlen ( uint frame, short value ) {
    if ( frame + 1  > Lenth_F*2 ) { return -1; }

    Lunghezze [frame] = value;
    return Lunghezze [frame];
}

short          *STATE::get_all_frequ () { return Frequenze; }
unsigned short *STATE::get_all_inten () { return Intensita; }
unsigned short *STATE::get_all_lenth () { return Lunghezze; }

std::ostream& operator << (std::ostream& os, STATE& p) {
    os << "\tvalues:" << std::endl;
    os << "\tbpm: " << p.get_bpm() << std::endl;
    os << "\tsmp: " << p.get_sampl() << std::endl;
    os << "\tlen: " << p.get_lenth() << std::endl;

    os << std::endl;
    for (short i = 0; i < p.get_lenth(); i++)
    {
        os << "\tF: " << p.get_frequ ( i );
        os << "\tI: " << p.get_inten ( i );
        os << "\tL1:" << p.get_frlen ( i*2 );
        os << "\tL2:" << p.get_frlen ( i*2+1 );
        os << std::endl;
    }
    return os;
}
