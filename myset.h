#ifndef MYSET_H
#define MYSET_H
#include <set>
#include <string>

template <typename T>
class myset : public std::set<T> {
public:
	myset();
	myset<T> set_intersection(const myset<T>& other);
	myset<T> set_union(const myset<T>& other);
};

template <typename T>
myset<T>::myset() : std::set<T>() {}

template <typename T>
myset<T> myset<T>::set_intersection(const myset<T>& other){
	myset<T> result;
	typename myset<T>::iterator itfirst;
	typename myset<T>::iterator itsecond;
	for(itfirst = this->begin(); itfirst != this->end(); ++itfirst) {
        for(itsecond = other.begin(); itsecond != other.end(); ++itsecond){
        	if(*itsecond==*itfirst){
        		result.insert(*itfirst);
        	}
        }
    }
    return result;
}

template <typename T>
myset<T> myset<T>::set_union(const myset<T>& other){
	myset<T> result;
	typename myset<T>::iterator itfirst;
	typename myset<T>::iterator itsecond;
    for(itfirst = this->begin(); itfirst != this->end(); ++itfirst) {
        result.insert(*itfirst);
    }
    for(itsecond = other.begin(); itsecond != other.end(); ++itsecond){
        result.insert(*itsecond);
    }
    return result;
}
#endif

