#ifndef WINGMANN_JSOLAR_CLASS_H
#define WINGMANN_JSOLAR_CLASS_H

namespace wingmann::ser::json {

enum class JsonClass {
    Null,
    Object,
    Array,
    String,
    Number,
    Boolean
};

} // namespace wingmann::ser::json

#endif // WINGMANN_JSOLAR_CLASS_H
