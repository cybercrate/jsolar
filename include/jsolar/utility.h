#ifndef WINGMANN_JSOLAR_UTILITY_H
#define WINGMANN_JSOLAR_UTILITY_H

#include <string>

namespace wingmann::serializing::json {

std::string json_escape(const std::string &str) {
    std::string output;

    for (char i : str) {
        switch (i) {
            case '\"':
                output += "\\\"";
                break;
            case '\\':
                output += "\\\\";
                break;
            case '\b':
                output += "\\b";
                break;
            case '\f':
                output += "\\f";
                break;
            case '\n':
                output += "\\n";
                break;
            case '\r':
                output += "\\r";
                break;
            case '\t':
                output += "\\t";
                break;
            default:
                output += i;
                break;
        }
    }
    return std::move(output);
}

} // namespace wingmann::serializing::json

#endif // WINGMANN_JSOLAR_UTILITY_H
