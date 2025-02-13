#ifndef SAVE_SYSTEM
#define SAVE_SYSTEM

#include <string>
#include <unordered_map>
#include "../../debugger.hpp"
#include <fstream>

namespace Manager {
    struct data {
        // start of the data to log
        unsigned char * point = nullptr;
        // legth of the data to log
        unsigned int len = 0;
    };
    static std::unordered_map < std::string, data > _logged_data;

    void Free_Save ( ) 
    { _logged_data.clear ( ); }

    // save a pointer to a value that will be saved
    template < typename T > 
    int Log ( std::string name, T value, unsigned int size ) {
        if ( name.find ( '"' ) != name.npos )
        { DEBUG ( 2, "name cannot contain \" char"); return 1; }
        data new_data;
        new_data.point = (unsigned char*) value;
        new_data.len = size;
        _logged_data.insert ( { name, new_data } );
        return 0;
    }

    // removes a logged item and it wont be logged anymore
    void Rem ( std::string name )
    {  _logged_data.erase ( name ); }

    // save all logged value
    int Save ( std::string path ) {
        // create file
        auto file = std::ofstream ( path.c_str ( ), std::ios::out | std::ios::binary );
        if ( ! file ) { DEBUG(1, "cannot open save file"); return 1; }

        for ( auto d : _logged_data ) {
            DEBUG ( 4, "Saving: \"", d.first, "\" ", d.second.len, '\n' );
            file << '"' << d.first << "\" ";
            file.write ( (char*) d.second.point, d.second.len );
            file << '\n';
        }

        // close file
        file.close ( );
        return 0;
    }

    // checks if the save exists
    int Check ( std::string path ) {
        if (FILE *file = fopen(path.c_str(), "r")) {
        	fclose(file);
			return true;
		} else {
			return false;
		} 
    }

    // replaces all loged values with previouly saved ones
    int Load ( std::string path ) {
        auto file = std::ifstream ( path.c_str ( ), std::ios::in | std::ios::binary );
        if ( ! file ) { DEBUG(1, "cannot open save file"); return 1; }

        std::string data_point;
        while ( std::getline ( file, data_point ) ) {
            // parse : std::string int binary_data
            std::string name = "";
            data _data;

            bool Quotation_Mark = false;
		    char current;
            int i = 0;
            for ( i; i < data_point.size ( ); i++) {
                current = data_point[i];

                if ( current == '\"' )
                { Quotation_Mark = !Quotation_Mark; continue; }

                if ( Quotation_Mark ) { name += current; }
                else if ( name != "" ) { break; }
            }
            DEBUG ( 4, "Loading variable: ", name );
            DEBUG ( 5, "point: ", data_point.size ( ), " i: ", i );
            _data.len = data_point.size ( ) - i - 1;
            _data.point = ( unsigned char * ) calloc ( _data.len, 1 );

            DEBUG ( 5, "Data len: ", _data.len, " : '" );
            for ( int x = 0; x < _data.len; x++ ) {
                _data.point [ x ] = data_point [ i + x + 1];
                // DEBUG ( 6, _data.point [x] );
            }

            // insert
            auto where = _logged_data[name].point;

            for ( int i = 0; i < _data.len; i++ ) {
                where[i] = _data.point[i];
                /* code */
            }
        }

        file.close ( );
        return 0;
    }

    // reads a saved value
    data Read ( std::string name ) {
        return _logged_data[name];
    }
}


#endif