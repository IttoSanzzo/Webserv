#ifndef STPLIB
# define STPLIB

size_t						stp_stringArrayLength(const std::string* str);
int							stp_countOccurrences(const std::string& input, char delimiter);
std::string*				stp_split(const std::string& inputString, char delimiter);
std::vector<std::string>	stp_split(const std::string& str, const std::string& delimiter);
bool						stp_stringIsNumeric(const std::string& str);
bool						stp_stringIsNumericOrSpaced(const std::string& str);
bool						stp_checkSufix(const std::string& str, const std::string& sufix);
std::string					stp_itoa(const int& number);
std::string					stp_btoa(const bool& value);
void*						stp_memset(void* target, const int& value, const size_t& size);
bool						stp_isFolder(const std::string& path);
void						stp_sortInsert(std::vector<std::string>& vector, const std::string& newEntry);
char*						stp_strdup(const std::string& src);

#endif