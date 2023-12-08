#pragma once

#include <string>
#include <filesystem>

#define root_path std::string(std::filesystem::current_path().c_str())
#define data_path root_path + "/data"
#define audio_libs_path data_path + "/audio_libs"
#define save_files_path data_path + "/save_files"

#define punteggiatura "-_ ,.;:!?\0"
#define vocali "aeiouAEIOU"
#define alphabet_min "abcdefghijklmnopqrstuvwxyz"
#define alphabet_cap "ABCDEFGHIJKLMNOPQRTSUVWXYZ"
