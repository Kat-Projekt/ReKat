#include "extensions/graphik/resources/instance.h"	
#include <cstring>

void Instance::Add_Data
( void * data, unsigned int elements )
{
	if ( !glIsBuffer(_buffer) || _buffer_size < _instances + elements )
	{
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
void Instance::Rem_Data
( unsigned int index )
{
	// switch last and index then _instaces --;
	if ( index >= _buffer_size )
	{ DEBUG (1, "out of bounds"); return; }

	glBindBuffer(GL_ARRAY_BUFFER, _buffer); GL_CHECK_ERROR;
	char *ptr = (char*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); GL_CHECK_ERROR;

	// copy last instance to index
	_instances --;
	memcpy ( ptr + index * _data_size, ptr +  _instances * _data_size, _buffer_size );
	
	glUnmapBuffer(GL_ARRAY_BUFFER); GL_CHECK_ERROR;
}
void Instance::Update_Data
( unsigned int index, void * data )
{
	if ( index >= _buffer_size )
	{ DEBUG (1, "out of bounds"); return; }

	glBindBuffer(GL_ARRAY_BUFFER, _buffer); GL_CHECK_ERROR;
	glBufferSubData(GL_ARRAY_BUFFER, index * _data_size, _data_size, data); GL_CHECK_ERROR;
	glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERROR;
}
void Instance::Update_Data
( unsigned int start, unsigned int size, void * data )
{
	assert ( size > 0 && start + size < _buffer_size );

	glBindBuffer(GL_ARRAY_BUFFER, _buffer); GL_CHECK_ERROR;
	char *ptr = (char*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); GL_CHECK_ERROR;
	// now copy data into memory
	memcpy(ptr + start*_data_size, data, _data_size * ( size ));
	// make sure to tell OpenGL we're done with the pointer
	glUnmapBuffer(GL_ARRAY_BUFFER); GL_CHECK_ERROR;
}
void Instance::Configure_Atributes
( std::vector < Attribute > attributes, unsigned int index )
{
	_attributes = attributes;
	_index = index;
	_changed = true;
}
// first thing to call to confingure data dimensions
// then call Add_Data
// then call Configure_Attributes
// then you can use it
int Instance::Make
( unsigned int data_size, unsigned int chunk_size )
{
	_data_size = data_size;
	_memory_chunk = chunk_size;
	_instances = 0;
	return 0;
}
// to end the resource
void Instance::End ( ) 
{
	if ( glIsBuffer(_buffer) )
	{ glDeleteBuffers(1,&_buffer); }
}

// used to trigger vertex attriv reconfiguration
void Instance::Use ( )
{
	if ( _changed ) {
		glBindBuffer(GL_ARRAY_BUFFER, _buffer); GL_CHECK_ERROR;
		int* pointer = 0;
		unsigned int index = _index;
		for ( auto Att : _attributes )
		{
			glEnableVertexAttribArray(index); GL_CHECK_ERROR;
			// confiugre memory layout
			glVertexAttribPointer(index, Att.size, Att.type, Att.normalized, _data_size, (void*)pointer); GL_CHECK_ERROR;
			glVertexAttribDivisor(index, 1); GL_CHECK_ERROR; // say it is an instance buffer
			DEBUG ( 5, "attrubte set: ", index, " size ", Att.size, " point ", pointer );
			pointer += Att.stride;
			index++;
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		_changed = false;
	}
}

unsigned int Instance::Instances
( )
{ return _instances; }

std::ostream& operator << ( std::ostream& os, const Instance& ele ) {
	for ( size_t i = 0; i < ele._attributes.size ( ); i++ )
	{
		auto Att = ele._attributes [ i ];
		os << "Att: " << i << " of size " << Att.size << " type ";

		switch ( Att.type ) {
			case GL_INT: os << "INT"; break;
			case GL_FLOAT: os << "FLOAT"; break;
			case GL_UNSIGNED_INT: os << "UNSIGNED_INT"; break;
			case GL_DOUBLE: os << "DOUBLE"; break;
			
			default: os << Att.type; break;
		}
		
		os << " stride " << Att.stride << '\n';
	}
	os << "instances: " << ele._instances << '\n';
	os << "data size: " << ele._data_size << '\n';
	os << "buff size: " << ele._buffer_size << '\n';

	os << "data: \n";
	glBindBuffer(GL_ARRAY_BUFFER, ele._buffer); GL_CHECK_ERROR;
	char *ptr = (char*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY); GL_CHECK_ERROR;
	
	for ( int i = 0; i < ele._instances; i++ )
	{
		os << "I: " << i;

		for ( size_t x = 0; x < ele._attributes.size ( ); x++ )
		{
			auto Att = ele._attributes [ x ];
			os << " Atr: " << x << ": ";
			for ( int i = 0; i < Att.size; i++ )
			{
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
