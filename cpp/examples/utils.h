#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <list>
#include <map>
#include <utility>

std::ostream& operator << (std::ostream& out, const std::nullptr_t _)
{
    out << "()";
    return out;
}

template <class T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vec);

template <class T>
std::ostream& operator << (std::ostream& out, const std::list<T>& lst);

template <class A, class B>
std::ostream& operator << (std::ostream& out, const std::pair<A, B>& p)
{
    out << "(" << p.first << "," << p.second << ")";
    return out;
}

template <class T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vec)
{
    out << "{";
    for (auto x : vec)
    {
        out << x << ",";
    }
    out << "}";
    return out;
}

template <class T>
std::ostream& operator << (std::ostream& out, const std::list<T>& lst)
{
    out << "[";
    for (auto x : lst)
    {
        out << x << ",";
    }
    out << "]";
    return out;
}

template <class K, class V>
std::ostream& operator << (std::ostream& out, const std::map<K, V>& map)
{
    out << "{" << std::endl;
    for (auto p : map)
    {
        out << p.first << " : " << p.second << "," << std::endl;
    }
    out << "}";
    return out;
}


#endif // UTILS_H

