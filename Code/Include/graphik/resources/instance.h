#ifndef INSTANCE_H
#define INSTANCE_H

#include "../graphik_debugger.hpp"
#include "../../resource.hpp"

// instances a series of objekts with Data as the instance data
// in partivular it manages a buffer of type Data from where you can instance objekts
// by default it renders Quads
class Instance : public Resource {
private:
    bool changed = true;
	int _instances = 0;
    unsigned int _buffer = 0;
    unsigned int _index = 0;

    int _data_size = 0;
    int _buffer_size = 0;
    int _memory_chunk = 16;

    struct Attribute {
        GLint size; // dimension of the attribute
        GLenum type; // type
        GLboolean normalized = false; // to clamp [0,1]
        GLsizei stride; // stride normaly is equal to size * sizeof(type)
    };

    std::vector < Attribute > _attributes;
public:
    void Add_Data ( void * data, unsigned int elements ) {
        if ( !glIsBuffer(_buffer) || _buffer_size < _instances + elements ) {
            // create a new buffer
            unsigned int new_buffer = 0; // temporary
            // add a chunk over the cheunks neaded for allocation
            unsigned int new_size = _instances + ( ( elements * _data_size ) / _memory_chunk + 1 ) * _memory_chunk;
            glGenBuffers(1, &new_buffer); GL_CHECK_ERROR;
            glBindBuffer(GL_ARRAY_BUFFER, new_buffer); GL_CHECK_ERROR;
            glBufferData(GL_ARRAY_BUFFER, new_size, NULL, GL_DYNAMIC_DRAW ); GL_CHECK_ERROR;

            // copy eventual old buffer
            if ( glIsBuffer(_buffer) ) {
                glBindBuffer(GL_COPY_READ_BUFFER, _buffer); GL_CHECK_ERROR;
                glBindBuffer(GL_COPY_WRITE_BUFFER, new_buffer); GL_CHECK_ERROR;
                glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, _instances * _data_size); GL_CHECK_ERROR;

                // delete old buffer
                glDeleteBuffers(1, &_buffer); GL_CHECK_ERROR;
            }

            // update memory
            _buffer = new_buffer;
            _buffer_size = new_size;
        }

        // add new data
        glBindBuffer(GL_ARRAY_BUFFER, _buffer); GL_CHECK_ERROR;
        DEBUG ( 5, "size: ", _buffer_size, " begin: ", _instances * _data_size, " length: ",  _data_size * elements );
        glBufferSubData(GL_ARRAY_BUFFER, _instances * _data_size, _data_size * elements, data); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERROR;
		_instances += elements;
        DEBUG (5, "data replaced");
    }
    void Rem_Data ( unsigned int index ) {
        // switch last and index then _instaces --;
        if ( index >= _buffer_size ) { DEBUG (1, "out of bounds"); return; }
        glBindBuffer(GL_ARRAY_BUFFER, _buffer); GL_CHECK_ERROR;
        char *ptr = (char*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); GL_CHECK_ERROR;

        // copy last instance to index
        _instances --;
        memcpy ( ptr + index * _data_size, ptr +  _instances * _data_size, _buffer_size );
        
        glUnmapBuffer(GL_ARRAY_BUFFER); GL_CHECK_ERROR;
    }
    void Update_Data ( unsigned int index, void * data ) {
        if ( index >= _buffer_size ) { DEBUG (1, "out of bounds"); return; }
        glBindBuffer(GL_ARRAY_BUFFER, _buffer); GL_CHECK_ERROR;
        glBufferSubData(GL_ARRAY_BUFFER, index * _data_size, _data_size, data); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERROR;
    }
    void Update_Data ( unsigned int start, unsigned int size, void * data ) {
        assert ( size > 0 && end < _buffer_size );
        glBindBuffer(GL_ARRAY_BUFFER, _buffer); GL_CHECK_ERROR;
        char *ptr = (char*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); GL_CHECK_ERROR;
        // now copy data into memory
        memcpy(ptr + start*_data_size, data, _data_size * ( size ));
        // make sure to tell OpenGL we're done with the pointer
        glUnmapBuffer(GL_ARRAY_BUFFER); GL_CHECK_ERROR;
    }
    void Configure_Atributes ( std::vector < Attribute > attributes, unsigned int index = 1 ) {
        _attributes = attributes;
        _index = index;
    }
    int Make ( unsigned int data_size, unsigned int chunk_size ) {
        _data_size = data_size;
        _memory_chunk = chunk_size;
        _instances = 0;
        return 0;
    }
    void End ( ) 
    { if ( glIsBuffer(_buffer) ) { glDeleteBuffers(1,&_buffer); } }
    
    void Use ( ) { if ( changed ) {
        glBindBuffer(GL_ARRAY_BUFFER, _buffer); GL_CHECK_ERROR;
        unsigned int pointer = 0;
        unsigned int index = _index;
        for ( auto Att : _attributes ) {
            glEnableVertexAttribArray(index); GL_CHECK_ERROR;
            // confiugre memory layout
            glVertexAttribPointer(index, Att.size, Att.type, Att.normalized, _data_size, (void*)pointer); GL_CHECK_ERROR;
            glVertexAttribDivisor(index, 1); GL_CHECK_ERROR; // say it is an instance buffer
            DEBUG ( 5, "attrubte set: ", index, " size ", Att.size, " point ", pointer );
            pointer += Att.stride;
            index++;
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    } }

    unsigned int Instances ( ) { return _instances; }

    friend std::ostream& operator << ( std::ostream& os, const Instance& ele ) {
        for ( int i = 0; i < ele._attributes.size ( ); i++ ) {
            auto Att = ele._attributes [ i ];
            os << "Att: " << i << " of size " << Att.size << " type ";

            switch ( Att.type ) {
            case GL_INT: os << "INT"; break;
            case GL_FLOAT: os << "FLOAT"; break;
            case GL_UNSIGNED_INT: os << "UNSIGNED_INT"; break;
            case GL_DOUBLE: os << "DOUBLE"; break;
            
            default:
                os << Att.type;
                break;
            }
               
            os << " stride " << Att.stride << '\n';
        }
        os << "instances: " << ele._instances << '\n';
        os << "data size: " << ele._data_size << '\n';
        os << "buff size: " << ele._buffer_size << '\n';

        os << "data: \n";
        glBindBuffer(GL_ARRAY_BUFFER, ele._buffer); GL_CHECK_ERROR;
        char *ptr = (char*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY); GL_CHECK_ERROR;
        for (size_t i = 0; i < ele._instances; i++) {
            os << "I: " << i;

            for ( int x = 0; x < ele._attributes.size ( ); x++ ) {
                auto Att = ele._attributes [ x ];
                os << " Atr: " << x << ": ";
                for (size_t i = 0; i < Att.size; i++) {
                    switch ( Att.type ) {
                    case GL_INT: os << *(int*)(ptr + i*sizeof(int)); break;
                    case GL_FLOAT: os << *(float*)(ptr + i*sizeof(float)); break;
                    case GL_UNSIGNED_INT: os << "UNSIGNED_INT"; break;
                    case GL_DOUBLE: os << "DOUBLE"; break;
                    }
                    os << " ";
                }
                os << '\n';
                ptr += Att.stride;
            }
            
        }
        
        
        // make sure to tell OpenGL we're done with the pointer
        glUnmapBuffer(GL_ARRAY_BUFFER); GL_CHECK_ERROR;
        

        return os;
    }
};
#endif