#ifndef PROPERTYSERIALIZER_H
#define PROPERTYSERIALIZER_H
#include "properties.h"
#include "messagecontext.h"
#include "box.h"
#include <functional>
namespace editor {
namespace property {

struct Exception {
    std::string mMessage;
};
template <typename T>
struct Cast {
    static std::string to_string(const T &t) {
        return t;
    }
    static T from_string(const std::string &value) {
        return value;
    }
};

template <typename T>
struct Cast<Box<T>> {
    static std::string to_string(const Box<T> &b) {
        return b.toString();
    }
    static Box<T> from_string(const std::string &value) {
        try {
            return Box<T>::fromString(value);
        }
        catch (const SerializationException &e) {
            throw Exception{e.what()};
        }
    }
};

template <>
struct Cast<int> {
    static std::string to_string(int i) {
        return std::to_string(i);
    }
    static int from_string(const std::string &value) {
        try {
            return boost::lexical_cast<int>(value);
        }
        catch (const boost::bad_lexical_cast &e) {
            throw Exception{'"' + value + "\" can't be converted to an integer."};
        }
    }
};

template <>
struct Cast<float> {
    static std::string to_string(float i) {
        return std::to_string(i);
    }
    static float from_string(const std::string &value) {
        try {
            return boost::lexical_cast<float>(value);
        }
        catch (const boost::bad_lexical_cast &e) {
            throw Exception{'"' + value + "\" can't be converted to a float."};
        }
    }
};

template <>
struct Cast<double> {
    static std::string to_string(double i) {
        return std::to_string(i);
    }
    static double from_string(const std::string &value) {
        try {
            return boost::lexical_cast<double>(value);
        }
        catch (const boost::bad_lexical_cast &e) {
            throw Exception{'"' + value + "\" can't be converted to a double."};
        }
    }
};

template <>
struct Cast<bool> {
    static std::string to_string(bool i) {
        return i ? "true" : "false";
    }
    static bool from_string(std::string value) {
        std::string l = text::lowered(value);
        if (l.size() == 1) {
            switch (l[0]) {
            case 'T':
            case 'Y':
            case '1':
                return true;
            case 'N':
            case 'F':
            case '0':
                return false;
            default:
                break;
            }
        }
        else {
            if (l == "true" || l == "yes") {
                return true;
            } else if (l == "false" || l == "no") {
                return false;
            }
        }
        throw Exception{'"' + value + "\" can't be converted to a boolean."};
    }
};


template <>
struct Cast<Name> {
    static std::string to_string(const Name &n) {
        return n.toParseString();
    }
    static Name from_string(const std::string &value) {
        return Name::fromParseString(value);
    }
};

template <typename T>
std::string to_string(const T &v) {
    return Cast<T>::to_string(v);
}

template <typename T>
T from_string(const std::string &str) {
    return Cast<T>::from_string(str);
}


}
}

#endif // PROPERTYSERIALIZER_H
