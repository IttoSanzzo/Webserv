#include "webserv.hpp"

bool			stp_stringIsNumeric(const std::string& str) {
	for (size_t i = 0; i < str.size(); ++i)
		if (std::isdigit(str[i]) == false)
			return (false);
	return (true);
}
bool			stp_stringIsNumericOrSpaced(const std::string& str) {
	for (size_t i = 0; i < str.size(); ++i)
		if (std::isdigit(str[i]) == false && str[i] != ' ')
			return (false);
	return (true);
}
size_t			stp_stringArrayLength(const std::string* str) {
	size_t	lenght = 0;
	while (str[lenght] != "")
		lenght++;
	return (lenght);
}
int				stp_countOccurrences(const std::string& input, char delimiter) {
    int		count = 0;
	size_t	i = 0;
	while (i < (size_t)input.size() && input[i] == delimiter)
		i++;
	while (i < (size_t)input.size()) {
		if (input[i] != delimiter) {
			count++;
			while (++i < (size_t)input.size() && input[i] != delimiter)
				continue ;
			--i;
		}
		i++;
    }
    return (count);
}
std::string*	stp_split(const std::string& inputString, char delimiter) {
    int partsCount = stp_countOccurrences(inputString, delimiter) + 1;
    std::string* parts = new std::string[partsCount];
	if (partsCount == 1) {
		parts[0] = std::string("");
		return (parts);
	}
    size_t	partIndex = 0;
    size_t	start = 0;
	while (inputString[start] == delimiter)
		start++;
    size_t	end = inputString.find(delimiter, start);
    while (start < inputString.size() && end != std::string::npos) {
        parts[partIndex++] = inputString.substr(start, end - start);
        start = end;
		while (start < inputString.size() && inputString[start] == delimiter)
			start++;
        end = inputString.find(delimiter, start);
    }
	if (inputString[inputString.size() - 1] != delimiter)
    	parts[partIndex++] = inputString.substr(start, inputString.size() - start);
    parts[partIndex] = "";
    return (parts);
}
bool			stp_checkSufix(const std::string& str, const std::string& sufix) {
	size_t sufixPosition = str.find(sufix);
	if (sufixPosition != str.size() - sufix.size())
		return (false);
	return (true);
}
std::string		stp_itoa(const int& number) {
	std::ostringstream	oss;
	oss << number;
	return (oss.str());
}
std::string		stp_btoa(const bool& value) {
	switch (value) {
		case (true):
			return ("true");
		break;
		case (false):
			return ("false");
		break;
	}
}
void*			stp_memset(void* target, const int& value, const size_t& size) {
	for (size_t i = 0; i < size; ++i)
		((unsigned char *)target)[i] = value;
	return (target);
}