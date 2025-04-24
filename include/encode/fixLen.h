#ifndef FIXLEN_H
#define FIXLEN_H

template <typename sub, typename T>
class fixLen {
protected:
    static const bool fix = true;
    T* p;
public:
    fixLen& operator++() {
        p ++;
        return *this;
    }
    fixLen operator++(int) {
        fixLen tmp = *this;
        ++(*this);
        return tmp;
    }

    fixLen& operator--() {
        p --;
        return *this;
    }
    fixLen operator--(int){
        fixLen tmp = *this;
        --(*this);
        return tmp;
    }

    char32_t operator*() const {
        return sub::decode(p);
    }
    char32_t operator[](size_t index) const {
        T* cursor = p;
        cursor += index;
        return decode(cursor);
    }

    bool operator==(sub other) const{
        return (this->p) == other.p;
    }

    size_t len(){
        size_t len = 0;
        while(*(p + len) != 0){
            len ++;
        } return len;
    }
    size_t size(){
        return len()*sizeof(T);
    }
};

#endif