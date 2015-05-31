#include <iostream>
#include <map>
#include <list>
#include <vector>

#include "utils.h"
#include "parser.h"
#include "combinators.h"
#include "concrete_combinators.h"

enum value_kind_t
{
    Null,
    Object,
    Array,
    String,
    Number,
    Boolean
};

class value_t;
using object_t = std::map<std::string, value_t>;
using key_val_t= std::pair<std::string, value_t>;
using array_t = std::list<value_t>;

class value_t
{
public:
    value_t()
        : kind_(Null)
    {
        ptr_ = nullptr;
    }

    value_t(object_t const& object)
        : kind_(Object)
    {
        ptr_ = new object_t(object);
    }

    value_t(array_t const& array)
        : kind_(Array)
    {
        ptr_ = new array_t(array);
    }

    value_t(std::string const& string)
        : kind_(String)
    {
        ptr_ = new std::string(string);
    }

    value_t(int number)
        : kind_(Number)
    {
        number_ = number;
    }

    value_t(bool boolean)
        : kind_(Boolean)
    {
        boolean_ = boolean;
    }

    bool isNull() const
    {
        return kind_ == Null;
    }

    bool isObject() const
    {
        return kind_ == Object;
    }

    object_t& getObject() const
    {
        return *static_cast<object_t*>(ptr_);
    }

    bool isArray() const
    {
        return kind_ == Array;
    }

    array_t& getArray() const
    {
        return *static_cast<array_t*>(ptr_);
    }

    bool isString() const
    {
        return kind_ == String;
    }

    std::string& getString() const
    {
        return *static_cast<std::string*>(ptr_);
    }

    bool isBoolean() const
    {
        return kind_ == Boolean;
    }

    bool getBoolean() const
    {
        return boolean_;
    }

    bool isNumber() const
    {
        return kind_ == Number;
    }

    int getNumber() const
    {
        return number_;
    }

    ~value_t()
    {
//        if (kind_ == Object)
//        {
//            delete static_cast<object_t*>(ptr_);
//        }
//        else if (kind_ == Array)
//        {
//            delete static_cast<array_t*>(ptr_);
//        }
//        else if (kind_ == String)
//        {
//            delete static_cast<std::string*>(ptr_);
//        }
    }

private:
    value_kind_t kind_;
    union
    {
        void* ptr_;
        int number_;
        bool boolean_;
    };
};

std::ostream& operator << (std::ostream& out, value_t const& value)
{
    if (value.isArray())
    {
        out << value.getArray();
    }
    else if (value.isBoolean())
    {
        out << value.getBoolean();
    }
    else if (value.isNull())
    {
        out << "null";
    }
    else if (value.isNumber())
    {
        out << value.getNumber();
    }
    else if (value.isObject())
    {
        out << value.getObject();
    }
    else if (value.isString())
    {
        out << "\"" << value.getString() << "\"";
    }
    return out;
}

Parser<std::string> string_p()
{
    return between(symbol('"'), word(), symbol('"'));
}

Parser<value_t> json_p;

Parser<key_val_t> key_val_p()
{
    return string_p() >= [](std::string const& key){
    return indentation() > symbol(':') > indentation() > json_p >= [key](value_t const& value) {
    return result(std::make_pair(key, value));
    };};
}

Parser<object_t> object_p()
{
    return between(indentation() > symbol('{'),
                   sepby(indentation() > key_val_p(), indentation() > symbol(',')),
                   indentation() > symbol('}')) >= [](std::list<key_val_t> const& kv)
    {
        return result(object_t(kv.begin(), kv.end()));
    };
}

Parser<array_t> array_p()
{
    return between(indentation() > symbol('['),
                   sepby(indentation() > json_p, indentation() > symbol(',')),
                   indentation() > symbol(']'));
}

Parser<bool> bool_p()
{
    return (string("true") > result(true)) || (string("false") > result(false));
}

Parser<value_t> value_p()
{
    return integer() >= [](int num) { return result(value_t(num));}                  ||
           bool_p()  >= [](bool boolean) {return result(value_t(boolean)); }         ||
           string("null") > result(value_t())                                        ||
           string_p() >= [](std::string const& str) { return result(value_t(str)); } ||
           object_p() >= [](object_t const& obj) { return result(value_t(obj)); }    ||
           array_p()  >= [](array_t const& array) { return result(value_t(array)); };
}

int main()
{
    json_p.set_context(value_p());
    while (std::cin)
    {
        std::string str;
        std::getline(std::cin, str);
        std::cout << json_p.apply(str) << std::endl;
    }
    return 0;
}
