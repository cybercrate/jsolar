#ifndef WINGMANN_JSOLAR_JSOLAR_H
#define WINGMANN_JSOLAR_JSOLAR_H

#include "backing_data.h"
#include "class.h"
#include "utility.h"
#include "wrapper.h"

#include <cctype>
#include <cmath>
#include <cstdint>
#include <deque>
#include <initializer_list>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <type_traits>

namespace wingmann::ser::json {

class Jsolar {
    BackingData internal_;
    JsonClass type_ = JsonClass::Null;

public:
    Jsolar() : internal_{}, type_{JsonClass::Null} {}

    Jsolar(std::initializer_list<Jsolar> list) : Jsolar{} {
        setType_(JsonClass::Object);

        for (auto i = list.begin(), e = list.end(); i != e; i += 2) {
            operator[](i->toString()) = *std::next(i);
        }
    }

    Jsolar(Jsolar&& other) noexcept : internal_{other.internal_}, type_{other.type_} {
        other.type_ = JsonClass::Null;
        other.internal_.map = nullptr;
    }

    Jsolar& operator=(Jsolar&& other) noexcept {
        clearInternal_();

        internal_ = other.internal_;
        type_ = other.type_;

        other.internal_.map = nullptr;
        other.type_ = JsonClass::Null;

        return *this;
    }

    Jsolar(const Jsolar& other) {
        switch (other.type_) {
        case JsonClass::Object:
            internal_.map = new std::map<std::string, Jsolar>(
                other.internal_.map->begin(),
                other.internal_.map->end());
            break;
        case JsonClass::Array:
            internal_.list = new std::deque<Jsolar>(
                other.internal_.list->begin(),
                other.internal_.list->end());
            break;
        case JsonClass::String:
            internal_.string = new std::string{*other.internal_.string};
            break;
        default:
            internal_ = other.internal_;
        }
        type_ = other.type_;
    }

    Jsolar& operator=(const Jsolar& other) {
        clearInternal_();
        switch (other.type_) {
        case JsonClass::Object:
            internal_.map = new std::map<std::string, Jsolar>(
                other.internal_.map->begin(),
                other.internal_.map->end());
            break;
        case JsonClass::Array:
            internal_.list = new std::deque<Jsolar>(
                other.internal_.list->begin(),
                other.internal_.list->end());
            break;
        case JsonClass::String:
            internal_.string = new std::string(*other.internal_.string);
            break;
        default:
            internal_ = other.internal_;
        }
        type_ = other.type_;
        return *this;
    }

    ~Jsolar() {
        switch (type_) {
        case JsonClass::Array:
            delete internal_.list;
            break;
        case JsonClass::Object:
            delete internal_.map;
            break;
        case JsonClass::String:
            delete internal_.string;
            break;
        default:;
        }
    }

    template<typename T>
    explicit Jsolar(T b, typename std::enable_if<std::is_same<T, bool>::value>::type* = 0)
        : internal_(b), type_(JsonClass::Boolean) {}

    template<typename T>
    explicit Jsolar(
        T i,
        typename std::enable_if<
            std::is_integral<T>::value && !std::is_same<T, bool>::value>::type* = 0)
        : internal_{static_cast<long>(i)}, type_{JsonClass::Number} {}

    template<typename T>
    explicit Jsolar(T f, typename std::enable_if<std::is_floating_point<T>::value>::type* = 0)
        : internal_{static_cast<double>(f)}, type_{JsonClass::Number} {}

    template<typename T>
    explicit Jsolar(T s, typename std::enable_if<std::is_convertible<T, std::string>::value>::type* = 0)
        : internal_{std::string{s}}, type_{JsonClass::String} {}

    explicit Jsolar(std::nullptr_t) : internal_{}, type_{JsonClass::Null} {}

    static Jsolar Make(JsonClass type) {
        Jsolar ret;
        ret.setType_(type);
        return ret;
    }

    static Jsolar load(const std::string &str);

    template<typename T>
    void append(T arg) {
        setType_(JsonClass::Array);
        internal_.list->emplace_back(arg);
    }

    template<typename T, typename... U>
    void append(T arg, U... args) {
        append(arg);
        append(args...);
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, bool>::value, Jsolar&>::type operator=(T b) {
        setType_(JsonClass::Boolean);
        internal_.boolean = b;
        return *this;
    }

    template<typename T>
    typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, Jsolar&>::type operator=(T i) {
        setType_(JsonClass::Number);
        internal_.number = i;
        return *this;
    }

    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value, Jsolar&>::type operator=(T f) {
        setType_(JsonClass::Number);
        internal_.number = f;
        return *this;
    }

    template<typename T>
    typename std::enable_if<std::is_convertible<T, std::string>::value, Jsolar&>::type
    operator=(T s) {
        setType_(JsonClass::String);
        *internal_.string = std::string(s);
        return *this;
    }

    Jsolar& operator[](const std::string& key) {
        setType_(JsonClass::Object);
        return internal_.map->operator[](key);
    }

    Jsolar& operator[](unsigned index) {
        setType_(JsonClass::Array);

        if (index >= internal_.list->size()) {
            internal_.list->resize(index + 1);
        }
        return internal_.list->operator[](index);
    }

    Jsolar& at(const std::string& key) {
        return operator[](key);
    }

    [[nodiscard]]
    const Jsolar& at(const std::string& key) const {
        return internal_.map->at(key);
    }

    Jsolar& at(unsigned index) {
        return operator[](index);
    }

    [[nodiscard]]
    const Jsolar& at(unsigned index) const {
        return internal_.list->at(index);
    }

    [[nodiscard]]
    int length() const {
        if (type_ == JsonClass::Array) {
            return internal_.list->size();
        } else {
            return -1;
        }
    }

    [[nodiscard]]
    bool hasKey(const std::string& key) const {
        if (type_ == JsonClass::Object) {
            return internal_.map->find(key) != internal_.map->end();
        }
        return false;
    }

    [[nodiscard]]
    int size() const {
        if (type_ == JsonClass::Object) {
            return internal_.map->size();
        } else if (type_ == JsonClass::Array) {
            return internal_.list->size();
        } else {
            return -1;
        }
    }

    [[nodiscard]]
    JsonClass getType() const {
        return type_;
    }

    // Functions for getting primitives from the Jsolar object.
    [[nodiscard]]
    bool IsNull() const {
        return type_ == JsonClass::Null;
    }

    [[nodiscard]]
    std::string toString() const {
        bool b;
        return std::move(toString(b));
    }

    std::string toString(bool& ok) const {
        ok = (type_ == JsonClass::String);
        return ok ? std::move(json_escape(*internal_.string)) : std::string("");
    }

    [[nodiscard]]
    double toFloat() const {
        bool b;
        return toFloat(b);
    }
    double toFloat(bool& ok) const {
        ok = (type_ == JsonClass::Number);
        return ok ? internal_.number : 0.0;
    }

    [[nodiscard]]
    long toInt() const {
        bool b;
        return toInt(b);
    }

    long toInt(bool& ok) const {
        ok = (type_ == JsonClass::Number);
        return ok ? internal_.number : 0;
    }

    [[nodiscard]]
    bool toBool() const {
        bool b;
        return toBool(b);
    }

    bool toBool(bool& ok) const {
        ok = (type_ == JsonClass::Boolean);
        return ok && internal_.boolean;
    }

    JsonWrapper<std::map<std::string, Jsolar>> objectRange() {
        if (type_ == JsonClass::Object) {
            return {internal_.map};
        }
        return {nullptr};
    }

    JsonWrapper<std::deque<Jsolar>> arrayRange() {
        if (type_ == JsonClass::Array) {
            return {internal_.list};
        }
        return {nullptr};
    }

    [[nodiscard]]
    JsonConstWrapper<std::map<std::string, Jsolar>> objectRange() const {
        if (type_ == JsonClass::Object) {
            return {internal_.map};
        }
        return {nullptr};
    }

    [[nodiscard]]
    JsonConstWrapper<std::deque<Jsolar>> ArrayRange() const {
        if (type_ == JsonClass::Array) {
            return {internal_.list};
        }
        return {nullptr};
    }

    [[nodiscard]]
    std::string dump(int depth = 1, const std::string& tab = "  ") const {
        std::string pad;
        for (int i = 0; i < depth; ++i, pad += tab) {}

        switch (type_) {
        case JsonClass::Null:
            return "null";
        case JsonClass::Object: {
            std::string s = "{\n";
            bool skip = true;

            for (auto& p : *internal_.map) {
                if (!skip) s += ",\n";
                s += (pad + "\"" + p.first + "\" : " + p.second.dump(depth + 1, tab));
                skip = false;
            }
            s += ("\n" + pad.erase(0, 2) + "}");
            return s;
        }
        case JsonClass::Array: {
            std::string s = "[";
            bool skip = true;
            for (auto& p : *internal_.list) {
                if (!skip) s += ", ";
                s += p.dump(depth + 1, tab);
                skip = false;
            }
            s += "]";
            return s;
        }
        case JsonClass::String:
            return "\"" + json_escape(*internal_.string) + "\"";
        case JsonClass::Number:
            return std::to_string(internal_.number);
        case JsonClass::Boolean:
            return internal_.boolean ? "true" : "false";
        default:
            return "";
        }
    }

    friend std::ostream& operator<<(std::ostream&, const Jsolar&);

private:
    void setType_(JsonClass type) {
        if (type == type_) {
            return;
        }
        clearInternal_();

        switch (type) {
        case JsonClass::Null:
            internal_.map = nullptr;
            break;
        case JsonClass::Object:
            internal_.map = new std::map<std::string, Jsolar>();
            break;
        case JsonClass::Array:
            internal_.list = new std::deque<Jsolar>();
            break;
        case JsonClass::String:
            internal_.string = new std::string();
            break;
        case JsonClass::Number:
            internal_.number = 0.0;
            break;
        case JsonClass::Boolean:
            internal_.boolean = false;
            break;
        }

        type_ = type;
    }

    void clearInternal_() {
        switch (type_) {
        case JsonClass::Object:
            delete internal_.map;
            break;
        case JsonClass::Array:
            delete internal_.list;
            break;
        case JsonClass::String:
            delete internal_.string;
            break;
        default:;
        }
    }
};

Jsolar array() {
    return std::move(Jsolar::Make(JsonClass::Array));
}

template<typename... T>
Jsolar array(T... args) {
    Jsolar arr = Jsolar::Make(JsonClass::Array);
    arr.append(args...);
    return std::move(arr);
}

Jsolar object() {
    return std::move(Jsolar::Make(JsonClass::Object));
}

std::ostream& operator<<(std::ostream& os, const Jsolar& json) {
    os << json.dump();
    return os;
}

namespace {
Jsolar parse_next(const std::string&, size_t&);

void consume_ws(const std::string& str, size_t& offset) {
    while (isspace(str[offset])) ++offset;
}

Jsolar parse_object(const std::string& str, size_t& offset) {
    Jsolar Object = Jsolar::Make(JsonClass::Object);

    ++offset;
    consume_ws(str, offset);
    if (str[offset] == '}') {
        ++offset;
        return std::move(Object);
    }

    while (true) {
        Jsolar Key = parse_next(str, offset);
        consume_ws(str, offset);
        if (str[offset] != ':') {
            std::cerr << "error[object] expected colon, found '" << str[offset] << "'\n";
            break;
        }
        consume_ws(str, ++offset);
        Jsolar Value = parse_next(str, offset);
        Object[Key.toString()] = Value;

        consume_ws(str, offset);
        if (str[offset] == ',') {
            ++offset;
            continue;
        } else if (str[offset] == '}') {
            ++offset;
            break;
        } else {
            std::cerr << "error[object] expected comma, found '" << str[offset] << "'\n";
            break;
        }
    }

    return std::move(Object);
}

Jsolar parse_array(const std::string& str, size_t& offset) {
    Jsolar Array = Jsolar::Make(JsonClass::Array);
    unsigned index = 0;

    ++offset;
    consume_ws(str, offset);
    if (str[offset] == ']') {
        ++offset;
        return std::move(Array);
    }

    while (true) {
        Array[index++] = parse_next(str, offset);
        consume_ws(str, offset);

        if (str[offset] == ',') {
            ++offset;
            continue;
        } else if (str[offset] == ']') {
            ++offset;
            break;
        } else {
            std::cerr << "error[array] expected ',' or ']', found '" << str[offset] << "'\n";
            return std::move(Jsolar::Make(JsonClass::Array));
        }
    }

    return std::move(Array);
}

Jsolar parse_string(const std::string& str, size_t& offset) {
    Jsolar String;
    std::string val;
    for (char c = str[++offset]; c != '\"'; c = str[++offset]) {
        if (c == '\\') {
            switch (str[++offset]) {
            case '\"':
                val += '\"';
                break;
            case '\\':
                val += '\\';
                break;
            case '/':
                val += '/';
                break;
            case 'b':
                val += '\b';
                break;
            case 'f':
                val += '\f';
                break;
            case 'n':
                val += '\n';
                break;
            case 'r':
                val += '\r';
                break;
            case 't':
                val += '\t';
                break;
            case 'u': {
                val += "\\u";
                for (unsigned i = 1; i <= 4; ++i) {
                    c = str[offset + i];
                    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                        val += c;
                    else {
                        std::cerr
                            << "error[string] expected hex character in unicode escape, found '"
                            << c << "'\n";
                        return std::move(Jsolar::Make(JsonClass::String));
                    }
                }
                offset += 4;
            } break;
            default:
                val += '\\';
                break;
            }
        } else
            val += c;
    }
    ++offset;
    String = val;
    return std::move(String);
}

Jsolar parse_number(const std::string& str, size_t& offset) {
    Jsolar Number;
    std::string val, exp_str;
    char c;
    bool isDouble = false;
    long exp = 0;
    while (true) {
        c = str[offset++];
        if ((c == '-') || (c >= '0' && c <= '9'))
            val += c;
        else if (c == '.') {
            val += c;
            isDouble = true;
        } else
            break;
    }
    if (c == 'E' || c == 'e') {
        c = str[offset++];
        if (c == '-') {
            ++offset;
            exp_str += '-';
        }
        while (true) {
            c = str[offset++];
            if (c >= '0' && c <= '9')
                exp_str += c;
            else if (!isspace(c) && c != ',' && c != ']' && c != '}') {
                std::cerr << "error[number] expected a number for exponent, found '" << c << "'\n";
                return std::move(Jsolar::Make(JsonClass::Null));
            } else
                break;
        }
        exp = std::stol(exp_str);
    } else if (!isspace(c) && c != ',' && c != ']' && c != '}') {
        std::cerr << "error[number] unexpected character '" << c << "'\n";
        return std::move(Jsolar::Make(JsonClass::Null));
    }
    --offset;

    if (isDouble)
        Number = std::stod(val) * std::pow(10, exp);
    else {
        if (!exp_str.empty())
            Number = std::stol(val) * std::pow(10, exp);
        else
            Number = std::stol(val);
    }
    return std::move(Number);
}

Jsolar parse_bool(const std::string& str, size_t& offset) {
    Jsolar Bool;
    if (str.substr(offset, 4) == "true")
        Bool = true;
    else if (str.substr(offset, 5) == "false")
        Bool = false;
    else {
        std::cerr << "error[boolean] expected 'true' or 'false', found '" << str.substr(offset, 5)
                  << "'\n";
        return std::move(Jsolar::Make(JsonClass::Null));
    }
    offset += (Bool.toBool() ? 4 : 5);
    return std::move(Bool);
}

Jsolar parse_null(const std::string& str, size_t& offset) {
    Jsolar Null;
    if (str.substr(offset, 4) != "null") {
        std::cerr << "error[null]: expected 'null', found '" << str.substr(offset, 4) << "'\n";
        return std::move(Jsolar::Make(JsonClass::Null));
    }
    offset += 4;
    return std::move(Null);
}

Jsolar parse_next(const std::string& str, size_t& offset) {
    char value;
    consume_ws(str, offset);
    value = str[offset];

    switch (value) {
    case '[':
        return std::move(parse_array(str, offset));
    case '{':
        return std::move(parse_object(str, offset));
    case '\"':
        return std::move(parse_string(str, offset));
    case 't':
    case 'f':
        return std::move(parse_bool(str, offset));
    case 'n':
        return std::move(parse_null(str, offset));
    default:
        if ((value <= '9' && value >= '0') || value == '-')
            return std::move(parse_number(str, offset));
    }
    std::cerr << "error[parse]: unknown starting character '" << value << "'\n";
    return {};
}
} // namespace

Jsolar Jsolar::load(const std::string& str) {
    size_t offset = 0;
    return std::move(parse_next(str, offset));
}

} // namespace wingmann::ser::json

#endif // WINGMANN_JSOLAR_JSOLAR_H
