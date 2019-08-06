#pragma once
#include <vector>
#include <string>
#include <unordered_map>

std::vector<std::string> SplitString(const std::string &source, char delim = ' ');
//-----------------------------------------------
std::string GetPrefix(std::string &source, char delim = ' ');
//-----------------------------------------------
void ClearSufix(std::string &str, char ch);
//-----------------------------------------------
void ClearPrefix(std::string &str, char ch);
//-----------------------------------------------
std::unordered_map<std::string, int> GetStopesMap(std::vector<std::string> &vec);
// Utilites tests ===============================
#ifdef _DEBUG
void SpitString_test();
void GetPrefix_test();
void ClearPrefixSuffix_test();
void GetStopesMap_test();
#endif // _DEBUG