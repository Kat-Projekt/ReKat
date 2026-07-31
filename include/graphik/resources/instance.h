#ifndef INSTANCE_H
#define INSTANCE_H

#include "../graphik_debugger.hpp"
#include "../../resource.hpp"

// instances a series of objekts with Data as the instance data
// in partivular it manages a buffer of type Data from where you can instance objekts
class Instance : public Resource {
private:
	int _instances = 0;
	unsigned int _buffer = 0;
	unsigned int _index = 0;
	unsigned int _data_size = 0;
	unsigned int _buffer_size = 0;
	unsigned int _memory_chunk = 16;

	bool _changed = true;

	struct Attribute {
		GLint size; // dimension of the attribute
		GLenum type; // type
		GLboolean normalized = false; // to clamp [0,1]
		GLsizei stride; // stride normaly is equal to size * sizeof(type)
	};

	std::vector < Attribute > _attributes;
public:
	Instance ( );

	void Add_Data (
		void * data,
		unsigned int elements
	);
	void Rem_Data (
		unsigned int index
	);
	void Update_Data (
		unsigned int index,
		void * data
	);
	void Update_Data (
		unsigned int start,
		unsigned int size,
		void * data
	);

	void Configure_Atributes (
		std::vector < Attribute > attributes,
		unsigned int index = 1
	);
	unsigned int Instances ( );

	int Make ( );
	int Make (
		unsigned int data_size,
		unsigned int chunk_size
	);
	void End ( );
	void Use ( );

	friend std::ostream& operator << ( std::ostream& os, const Instance& ele );
};
#endif