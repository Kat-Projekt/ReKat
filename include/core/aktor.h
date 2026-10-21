#include "maestro.h"
#include <list>

class Aktor {
	std::list < Maestro::uniqueBehaviour > _behaviours;
	std::list < Maestro::uniqueBehaviour > _pending_behaviours;
public:	
	Behaviour * Add_Behaviour (
		std::string type,
		bool stable = true,
		uint64_t major = 0,
		uint64_t minor = 0,
		uint64_t pathc = 0,
		bool start_behaviour = true
	);


	Behaviour * Get_Behaviour (
		std::string type,
		bool stable = true,
		uint64_t major = 0,
		uint64_t minor = 0,
		uint64_t pathc = 0
	);


	void Rem_Behaviour (
		std::string type,
		bool stable = true,
		uint64_t major = 0,
		uint64_t minor = 0,
		uint64_t pathc = 0
	);


	void Has_Behaviour (
		std::string type,
		bool stable = true,
		uint64_t major = 0,
		uint64_t minor = 0,
		uint64_t pathc = 0
	);

	void Start ( );
	void Frame_Start ( );
	void Update ( );
	void Frame_End ( );
};
