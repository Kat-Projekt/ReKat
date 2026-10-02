#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <algorithm> // for reflection names
#include <sstream>   // for metadata

using namespace Catch::Matchers;

#include <iostream>

#include <core/maestro.h>

class SectionLogger : public Catch::EventListenerBase {
public:
	using Catch::EventListenerBase::EventListenerBase;

	void sectionStarting( Catch::SectionInfo const& sectionInfo) override 
	{
		std::cout << "[";
		ReKat::Debug::_print_colored ( sectionInfo.name.c_str ( ), FOREGROUND_GREEN );
		std::cout << "]\n";
		// std::cout << "-------------------------------------------------------------------------------\n";
	}
};

CATCH_REGISTER_LISTENER(SectionLogger)


SCENARIO ( "a maestro can register components", "[maestro]" )
{
	GIVEN( "a maestro" )
	{
		Maestro m;
		int r = m.Register ( "Empty_v1.dylib" );

		THEN ( "the maestro has 1 registered component" )
		{
			REQUIRE ( r == 1 ); // registration success
			REQUIRE ( m.Get_Registered_Components ( ).size ( ) == 1 );
		}

		WHEN ( "a new component is registered" )
		{
			r = m.Register ( "Empty_v2.dylib" );
			THEN ( "the maestro has 2 registered components" )
			{
				REQUIRE ( r == 1 ); // registration success
				REQUIRE ( m.Get_Registered_Components ( ).size ( ) == 2 );
			}
		}

		WHEN ( "a component is re-registered" )
		{
			r = m.Register ( "Empty_v1.dylib" );
			THEN ( "the maestro has 1 registered components" )
			{
				REQUIRE ( r == 0 ); // registration failed
				REQUIRE ( m.Get_Registered_Components ( ).size ( ) == 1 );
			}
		}

		WHEN ( "loading the directory" )
		{
			r = m.Register_Directory ( "." );
			THEN ( "the maestro registered all components" )
			{
				REQUIRE ( r == 4 ); // registration of Emtpty_1 failed
				REQUIRE ( m.Get_Registered_Components ( ).size ( ) == 5 );
			}
		}
	}
}

SCENARIO ( "a maestro can create a component", "[maestro]" )
{
	GIVEN ( "a maestro and a component" )
	{
		Maestro m;
		int r = m.Register_Directory ( "." );

		THEN ( "the maestro registered all components" )
		{
			REQUIRE ( r == 5 );
			REQUIRE ( m.Get_Registered_Components ( ).size ( ) == 5 );
		}

		WHEN ( "a component is constructed" )
		{
			REQUIRE_NOTHROW ( m.Construct ( "TestReflection" ) );
		}

		WHEN ( "a specific component is constructed" )
		{
			REQUIRE_NOTHROW ( m.Construct ( "TestReflection", true, 2,1,0 ) );
		}

		WHEN ( "a non existing component is constructed" )
		{
			REQUIRE_THROWS ( m.Construct ( "TestReflection_EEE" ) );
		}

		WHEN ( "constructing last stable component" )
		{
			REQUIRE_NOTHROW ( m.Construct ( "Empty", true ) );
		}

		WHEN ( "constructing throw on construction component / last empty" )
		{
			REQUIRE_THROWS ( m.Construct ( "Empty", false ) );
		}
	}
}

SCENARIO ( "a TestReflection component can set and get every parameter" )
{
	GIVEN ( "a TestReflecion component created by a maestro" )
	{
		Maestro m;
		int r = m.Register ( "TestReflection.dylib" );
		auto b = m.Construct ( "TestReflection" );

		THEN ( "the component is loaded correctly" )
		{
			REQUIRE ( r == 1 );
			REQUIRE_FALSE ( b.get ( ) == nullptr );
		}

		WHEN ( "setting parameters" )
		{
			b->Configure ( arg(string_parameter) = "ciao123" );
			b->Configure ( arg(int_parameter) = 421 );
			b->Configure ( arg(double_parameter) = 20.21 );
		b->Configure ( arg(float_parameter) = 23.1f );

		THEN ( "check if values are setted correctly" )
		{
			REQUIRE ( static_cast < std::string > ( b->Query ( "string_parameter" ) ) == "ciao123" );
			REQUIRE ( static_cast < int > ( b->Query ( "int_parameter" ) ) == 421 );
			REQUIRE_THAT ( static_cast < double > ( b->Query ( "double_parameter" ) ), WithinRel (20.21) );
			REQUIRE_THAT ( static_cast < float > ( b->Query ( "float_parameter" ) ), WithinRel (23.1f) );
		}
	}

	WHEN ( "setting properties" )
	{
		b->Configure ( arg(string_prop) = "ciao123" );
		b->Configure ( arg(int_prop) = 421 );
		b->Configure ( arg(double_prop) = 20.21 );
		b->Configure ( arg(float_prop) = 23.1f );

		THEN ( "check if values are setted correctly" )
		{
			REQUIRE ( static_cast < std::string > ( b->Query ( "string_prop" ) ) == "getted: ciao123" );
			REQUIRE ( static_cast < int > ( b->Query ( "int_prop" ) ) == -422 );
			REQUIRE_THAT ( static_cast < double > ( b->Query ( "double_prop" ) ), WithinRel (2*20.21) );
			REQUIRE_THAT ( static_cast < float > ( b->Query ( "float_prop" ) ), WithinRel (0.5f/23.1f) );
			}
	
		}
	}
}

SCENARIO ( "a TestReflection component can execute every function" )
{
	GIVEN ( "a TestReflecion component created by a maestro" )
	{
		Maestro m;
		int r = m.Register ( "TestReflection.dylib" );
		auto b = m.Construct ( "TestReflection" );

		THEN ( "the component is loaded correctly" )
		{
			REQUIRE ( r == 1 );
			REQUIRE_FALSE ( b.get ( ) == nullptr );
		}

		WHEN ( "Calling non existing functions" )
		{
			REQUIRE_THROWS ( b->Perform ( "non_existing_function" ) );
			REQUIRE_THROWS ( b->Perform ( "non_existing_function_with_args", arg(pino) = 10 ) );
		}

		WHEN ( "Calling existing void functions" )
		{
			REQUIRE_NOTHROW ( b->Perform ( "void_fun_no_args" ) );
			REQUIRE_THROWS ( b->Perform ( "void_fun_no_args_throws_on_call" ) );
			REQUIRE_NOTHROW ( b->Perform ( "void_fun_1_arg", arg(arg1) = 10 ) );
			REQUIRE_NOTHROW ( b->Perform ( "void_fun_2_args", arg(arg1) = 10, arg(arg2) = 1.0f ) );
			REQUIRE_NOTHROW ( b->Perform ( "void_fun_1_arg_with_default" ) );
			REQUIRE_NOTHROW ( b->Perform ( "void_fun_2_args_with_1_default", arg(arg2) = 1.0f ) );
			REQUIRE_NOTHROW ( b->Perform ( "void_fun_2_args_with_2_defaults" ) );
		}

		WHEN ( "Calling existing value functions" )
		{
			int val_1 = b->Perform ( "val_fun_no_args_returns_10" );
			REQUIRE_THROWS ( b->Perform ( "val_fun_no_args_throws_on_call" ) );
			int val_3 = b->Perform ( "val_fun_1_arg_returns_arg1", arg(arg1) = 42 );
			float val_4 = b->Perform ( "val_fun_2_args_return_sum_arg1_arg2", arg(arg1) = 6, arg(arg2) = 36.0f );
			int val_5 = b->Perform ( "val_fun_1_arg_with_default_returs_arg1" );	
			float val_6 = b->Perform ( "val_fun_2_args_with_1_default_returns_sum_arg1_arg2", arg(arg2) = 32.0f );
			float val_7 = b->Perform ( "val_fun_2_args_with_2_defaults_return_sum_arg1_arg2" );

			THEN ( "make sure output is correct" )
			{
				REQUIRE ( val_1 == 10 );
				REQUIRE ( val_3 == 42 );
				REQUIRE_THAT ( val_4, WithinRel (42.0f) );
				REQUIRE ( val_5 == 10 );
				REQUIRE_THAT ( val_6, WithinRel (42.0f) );	
				REQUIRE_THAT ( val_7, WithinRel (11.0f) );
			}
		}
	}
}

SCENARIO ( "a TestReflection component can express reflection" )
{
	GIVEN ( "a TestReflecion component created by a maestro" )
	{
		Maestro m;
		int r = m.Register ( "TestReflection.dylib" );
		auto b = m.Construct ( "TestReflection" );

		THEN ( "the component is loaded correctly" )
		{
			REQUIRE ( r == 1 );
			REQUIRE_FALSE ( b.get ( ) == nullptr );
		}
		
		WHEN ( "Requesting metadata" )
		{
			auto meta = b->Metadata ( );

			std::string meta_expected = "TestReflection 2.1.0[v] ( Comprehensive reflection system test )";

			THEN ( "Metadata is correct" )
			{
				std::stringstream ss;
				ss << meta;
				std::string formatted_meta = ss.str ( );
				
				REQUIRE ( meta_expected == formatted_meta );
			}
		}

		WHEN ( "Requesting methods" )
		{
			auto methods = b->Reflected_Methods ( );

			std::vector < std::string > methods_expected = {
				"void_fun_no_args",
				"void_fun_no_args_throws_on_call",
				"void_fun_1_arg",
				"void_fun_2_args",
				"void_fun_1_arg_with_default",
				"void_fun_2_args_with_1_default",
				"void_fun_2_args_with_2_defaults",
				"val_fun_no_args_returns_10",
				"val_fun_no_args_throws_on_call",
				"val_fun_1_arg_returns_arg1",
				"val_fun_2_args_return_sum_arg1_arg2",
				"val_fun_1_arg_with_default_returs_arg1",
				"val_fun_2_args_with_1_default_returns_sum_arg1_arg2",
				"val_fun_2_args_with_2_defaults_return_sum_arg1_arg2"
			};

			THEN ( "Expected methods are matched" )
			{
				REQUIRE ( std::is_permutation (
						methods.begin ( ), methods.end ( ),
						methods_expected.begin ( ), methods_expected.end ( )
					) );
			}
		}

		WHEN ( "Requesting parameters" )
		{
			auto parameters = b->Reflected_Parameters ( );

			std::vector < std::string > parameters_expected = {
				"string_parameter",
				"int_parameter",
				"double_parameter",
				"float_parameter",
				"string_prop",
				"int_prop",
				"double_prop",
				"float_prop"
			};

			THEN ( "Expected parameters are matched" )
			{
				REQUIRE ( std::is_permutation (
						parameters.begin ( ), parameters.end ( ),
						parameters_expected.begin ( ), parameters_expected.end ( )
					) );
			}

		}
	}
}
