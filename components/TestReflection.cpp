#include <engine.hpp>

class TestReflection : public Behaviour
{
	std::string	string_parameter = "";
	int		int_parameter = 0;
	double		double_parameter = 0.0;
	float		float_parameter = 0.0f;

	std::string	string_prop = "";
	int		int_prop = 1;
	double		double_prop = 1.0;
	float		float_prop = 1.0f;

	void void_fun_no_args ( ) { }
	void void_fun_no_args_throws_on_call ( ) { DEBUG ( DebugLevel::ERROR, "this should throw" ); }
	void void_fun_1_arg ( int ) { }
	void void_fun_2_args ( int, float ) { }
	void void_fun_1_arg_with_default ( int ) { }
	void void_fun_2_args_with_1_default ( int, float ) { }
	void void_fun_2_args_with_2_defaults ( int, float ) { }

	int val_fun_no_args_returns_10 ( ) { return 10; }
	int val_fun_no_args_throws_on_call ( ) { DEBUG ( DebugLevel::ERROR, "this should throw" ); return 0; }
	int val_fun_1_arg_returns_arg1 ( int arg1 ) { return arg1; }
	float val_fun_2_args_return_sum_arg1_arg2 ( int arg1, float arg2 ) { return arg2 + arg1; }
	int val_fun_1_arg_with_default_returs_arg1 ( int arg1 ) { return arg1; }
	float val_fun_2_args_with_1_default_returns_sum_arg1_arg2 ( int arg1, float arg2 ) { return arg1 + arg2; }
	float val_fun_2_args_with_2_defaults_return_sum_arg1_arg2 ( int arg1, float arg2 ) { return arg1 + arg2; }

	void string_setter ( std::string _v ) { string_prop += _v; }
	std::string string_getter ( ) const { return "getted: " + string_prop; }

	void int_setter ( int _v ) { int_prop += _v; }
	int int_getter ( ) const { return - int_prop; }

	void double_setter ( double _v ) { double_prop *= _v; }
	double double_getter ( ) const { return double_prop * 2; }

	void float_setter ( float _v ) { float_prop /= _v; }
	float float_getter ( ) const { return float_prop / 2; }
public:
	METADATA ( TestReflection, "Comprehensive reflection system test", 2,1,0, true )

	METHODS (
		VOID_FUNCTION ( void_fun_no_args ),
		VOID_FUNCTION ( void_fun_no_args_throws_on_call ),
		VOID_FUNCTION ( void_fun_1_arg, arg1 ),
		VOID_FUNCTION ( void_fun_2_args, arg1, arg2 ),
		VOID_FUNCTION ( void_fun_1_arg_with_default, DEFAULT ( arg1, 10 ) ),	
		VOID_FUNCTION ( void_fun_2_args_with_1_default, DEFAULT ( arg1, 10 ), arg2 ),
		VOID_FUNCTION ( void_fun_2_args_with_2_defaults, DEFAULT ( arg1, 10 ), DEFAULT ( arg2, 1.0f ) ),

		VALUE_FUNCTION ( val_fun_no_args_returns_10 ),
		VALUE_FUNCTION ( val_fun_no_args_throws_on_call ),
		VALUE_FUNCTION ( val_fun_1_arg_returns_arg1, arg1 ),
		VALUE_FUNCTION ( val_fun_2_args_return_sum_arg1_arg2, arg1, arg2 ),
		VALUE_FUNCTION ( val_fun_1_arg_with_default_returs_arg1, DEFAULT ( arg1, 10 ) ),	
		VALUE_FUNCTION ( val_fun_2_args_with_1_default_returns_sum_arg1_arg2, DEFAULT ( arg1, 10 ), arg2 ),
		VALUE_FUNCTION ( val_fun_2_args_with_2_defaults_return_sum_arg1_arg2, DEFAULT ( arg1, 10 ), DEFAULT ( arg2, 1.0f ) )
	)

	PARAMETERS (
		PARAMETER ( string_parameter ),
		PARAMETER ( int_parameter ),
		PARAMETER ( double_parameter ),
		PARAMETER ( float_parameter ),
		
		PROPERTY ( string_prop, string_setter, string_getter ),
		PROPERTY ( int_prop, int_setter, int_getter ),
		PROPERTY ( double_prop, double_setter, double_getter ),
		PROPERTY ( float_prop, float_setter, float_getter )
	)
};

