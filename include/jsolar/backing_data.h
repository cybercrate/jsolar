#ifndef JSOLAR_JSOLAR_BACKING_DATA_H
#define JSOLAR_JSOLAR_BACKING_DATA_H

#include <string>
#include <deque>
#include <map>
#include <utility>

namespace wingmann::ser::json {

class Json;

union BackingData {
    BackingData(double d)      : number(d) {}
    BackingData(long l)        : number(l) {}
    BackingData(bool b)        : boolean(b) {}
    BackingData(std::string s) : string{new std::string{std::move(s)}} {}
    BackingData()              : number(0) {}

    std::deque<Json>* list;
    std::map<std::string, Json>* map;
    std::string* string;
    double number;
    bool boolean;
};

} // namespace wingmann::ser::json

#endif // JSOLAR_JSOLAR_BACKING_DATA_H
