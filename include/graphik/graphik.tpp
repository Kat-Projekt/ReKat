template < typename T >
extern bool ReKat::Graphik::Key_Down ( T key ) {
	if ( _current_window == nullptr ) { return false; }
	return _current_window->input.Key_Down ( key ); }
template < typename T >
extern bool ReKat::Graphik::Key_Pressed ( T key ) {
	if ( _current_window == nullptr ) { return false; }
	return _current_window->input.Key_Pressed ( key ); }
template < typename T >
extern bool ReKat::Graphik::Key_Up ( T key ) {
	if ( _current_window == nullptr ) { return false; }
	return _current_window->input.Key_Up ( key ); }