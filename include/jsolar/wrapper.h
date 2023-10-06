#ifndef WINGMANN_JSOLAR_WRAPPER_H
#define WINGMANN_JSOLAR_WRAPPER_H

namespace wingmann::ser::json {

template<typename Container>
class JsonWrapper {
    Container* object;

public:
    JsonWrapper(Container* val) : object(val) {}
    JsonWrapper(std::nullptr_t) : object(nullptr) {}

    typename Container::iterator begin() {
        return object ? object->begin() : typename Container::iterator();
    }

    typename Container::iterator end() {
        return object ? object->end() : typename Container::iterator();
    }

    typename Container::const_iterator begin() const {
        return object ? object->begin() : typename Container::iterator();
    }

    typename Container::const_iterator end() const {
        return object ? object->end() : typename Container::iterator();
    }
};

template<typename Container>
class JsonConstWrapper {
    const Container* object;

public:
    JsonConstWrapper(const Container* val) : object(val) {}
    JsonConstWrapper(std::nullptr_t) : object(nullptr) {}

    typename Container::const_iterator begin() const {
        return object ? object->begin() : typename Container::const_iterator();
    }

    typename Container::const_iterator end() const {
        return object ? object->end() : typename Container::const_iterator();
    }
};

} // namespace wingmann::ser::json

#endif // WINGMANN_JSOLAR_WRAPPER_H
