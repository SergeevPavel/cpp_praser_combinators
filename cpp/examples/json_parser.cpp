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

    bool isNull()
    {
        return kind_ == Null;
    }

    bool isObject()
    {
        return kind_ == Object;
    }

    object_t* getObject()
    {
        return static_cast<object_t*>(ptr_);
    }

    bool isArray()
    {
        return kind_ == Array;
    }

    array_t* getArray()
    {
        return static_cast<array_t*>(ptr_);
    }

    bool isString()
    {
        return kind_ == String;
    }

    std::string* getString()
    {
        return static_cast<std::string*>(ptr_);
    }

    bool isBoolean()
    {
        return kind_ == Boolean;
    }

    bool getBoolean()
    {
        return boolean_;
    }

    bool isNumber()
    {
        return kind_ == Number;
    }

    int getNumber()
    {
        return number_;
    }

    ~value_t()
    {
        if (kind_ == Object)
        {
            delete static_cast<object_t*>(ptr_);
        }
        else if (kind_ == Array)
        {
            delete static_cast<array_t*>(ptr_);
        }
        else if (kind_ == String)
        {
            delete static_cast<std::string*>(ptr_);
        }
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

Parser<std::string> string_p()
{
    return between(symbol('"'), word(), symbol('"'));
}

Parser<value_t> value_p();

Parser<key_val_t> key_val_p()
{
    return string_p() >= [](std::string const& key){
    return indentation() > symbol(':') > indentation() > value_p() >= [key](value_t const& value) {
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
    return between(indentation() > symbol('['), sepby(value_p(), symbol(',')), indentation() > symbol(']'));
}

Parser<value_t> value_p()
{
    return integer() >= [](int num)
    {
        return result(value_t(num));
    };
}

int main()
{
    while (true)
    {
        std::string str;
        std::getline(std::cin, str);
        std::cout << object_p().apply(str) << std::endl;
    }
    return 0;
}
