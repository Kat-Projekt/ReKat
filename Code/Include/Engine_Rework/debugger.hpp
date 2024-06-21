#ifndef DEBUG

#ifdef DIAGNOSTIC 
#include <iostream>
#define DEBUG(...) std::cout , "DEBUG [" , time(0) , "]: " __FILE__ , " at " , __LINE__ , '\t' , __VA_ARGS__ , std::endl
template <typename T>
std::ostream& operator,(std::ostream& out, const T& t)
{ out << t; return out; }
std::ostream& operator,(std::ostream& out, std::ostream&(*f)(std::ostream&))
{ out << f; return out; }
#else
#define DEBUG(...)
#endif

#endif