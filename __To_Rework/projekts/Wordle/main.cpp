#include <engine.hpp>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

int main ( ) {
	srand ( time ( 0 ) );
	std::string command = "python3 Data/Esercizio8.py " + std::to_string ( rand ( ) );

	auto v = exec ( ( command + " aeiou ").c_str ( ) );

	ReKat::grapik::Start ( "Wordle", 800, 600, false, false, false, "Wordle.png" );

	std::cout << "respose:" << v << " l: " << v.size ( ) << '\n';

	while ( ReKat::grapik::IsEnd ( ) ) {
		ReKat::grapik::Update ( );
	}
	
	ReKat::grapik::Terminate ( );
}