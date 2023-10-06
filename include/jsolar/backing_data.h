#ifndef JSOLAR_JSOLAR_BACKING_DATA_H
#define JSOLAR_JSOLAR_BACKING_DATA_H

#include <string>
#include <deque>
#include <map>
#include <utility>

namespace wingmann::serializing::json {

class Jsolar;

union BackingData {
    BackingData(double d)      : number(d) {}
    BackingData(long l)        : number(l) {}
    BackingData(bool b)        : boolean(b) {}
    BackingData(std::string s) : string{new std::string{std::move(s)}} {}
    BackingData()              : number(0) {}

    std::deque<Jsolar>* list;
    std::map<std::string, Jsolar>* map;
    std::string* string;
    double number;
    bool boolean;
};

} // namespace wingmann::serializing::json

#endif // JSOLAR_JSOLAR_BACKING_DATA_H
