#include <string>
#include <unordered_map>

/************************************************************************
 * \brief This class is used to pass/get arguments to/from a component
 * \ingroup Components
 * ## Setting
 * you first nead to set some arguments using the Set function
 * args = ComponentArguments { }
 * 	.Set ( "first", 1 )
 * 	.Set ( "second", "number 2" )
 * 	...
 * Then you can pass it to a component
 * - NewComponent ( args ): this is used while constucting
 * - NewComponent { }. Set ( args ): this is used after for modifications
 * 
 * ## Getting
 * simply use
 * auto args = NewComponent.Get ( );
 ***********************************************************************/
class ComponentArguments {
private:
	struct _Argument
	{
		char * data = nullptr;
		std::string name = "";
		size_t data_size = 0;
	};

	std::unordered_map < std::string, _Argument > _arguments;
public:	
	/*************************************************
	 * \brief Stores the named parameter of value
	 * 	for later use
	 * 
	 * \param name a string naming the argument to set
	 * \param value the typed value of the parameter
	 ************************************************/
	template < typename T >
		ComponentArguments& Set ( const char* name, T value );

	/***************************************************
	 * \brief Retrives the parameter
	 * 
	 * If the named parameter does not exists it returns
	 * the default_value
	 * 
	 * If the named parameter is not of the right type
	 * it will DEBUG ( ERROR );
	 * 
	 * \param name a string naming the argument to get
	 * \param default_value fallback return value
	 **************************************************/
	template < typename T >
		T Get ( const char* name, T default_value ) const;
};