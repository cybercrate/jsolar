# Jsolar

### About
Jsolar is a lightweight library for serializing/deserializing the Jsolar format in C++.
Taking advantage of templates and operator overloading on the backend,
you can immediately create and work with Jsolar objects, as you would expect from a language like JavaScript.

### Examples
```cpp
#include <jsolar/jsolar.h>
#include <iostream>

using namespace wingmann::serializing::json;

int main() {
    Jsolar obj;

    // Create a new array as a field of an object.
    obj["array"] = array(true, "two", 3, 4.9213798);

    // Create a new object as a field of another object.
    obj["object"] = object();
    // Assign to one of the inner object's fields.
    obj["object"]["inner"] = "inside";
    
    // We don't need to specify the type of the json object.
    obj["new"]["some"]["deep"]["key"] = "value";
    obj["another_array"].append(false, "three");
    
    // We can also parse a string into a json object.
    obj["parsed"] = Jsolar::load(R"([{"key": "value"}, false])");
    
    std::cout << obj << '\n';
    return 0;
}
```

```cpp
#include <jsolar/jsolar.h>
#include <iostream>

using namespace wingmann::serializing::json;

int main() {
    auto obj = Jsolar::load(R"({
        "array": [true, "two", 3, 4.9213798],
        "another_array": [false, "three"],
        "new": {
            "some": {
                "deep": {
                    "key": "value"
                }
            }
        },
        "object": {
            "inner": "inside"
        },
        "parsed": [{ "key": "value" }, false]
    })");

    std::cout << obj << '\n';
    return 0;
}
```

Output:
```json
{
    "array": [true, "two", 3, 4.921380],
    "another_array": [false, "three"],
    "new": {
        "some": {
            "deep": {
                "key": "value"
            }
        }
    },
    "object": {
        "inner": "inside"
    },
    "parsed": [{ "key": "value" }, false]
}
```

[//]: # (### Additionally)

[//]: # (We do not have access to the colon &#40;:&#41; character in C++, so we cannot use that to seperate)

[//]: # (key-value pairs, but by using commas, we can achieve a similar effect.)

[//]: # (The other point you might notice, is that we have to explicitly create arrays.)

[//]: # (This is a limitation of C++'s operator overloading rules,)

[//]: # (so we cannot use the [] operator to define the array.)
