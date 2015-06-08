//
// Distributed under the ITensor Library License, Version 1.2
//    (See accompanying LICENSE file.)
//
#ifndef __ITENSOR_INFARRAY_H
#define __ITENSOR_INFARRAY_H

#include <array>
#include <vector>
#include <iterator> 
#include "itensor/util/error.h"
#include "itensor/util/safe_ptr.h"

#ifdef DEBUG
#define CHECK_IND(X) check_ind(X);
#else
#define CHECK_IND(X)
#endif

#ifdef DEBUG
#define CHECK_EMPTY check_empty();
#else
#define CHECK_EMPTY
#endif

namespace itensor {

template<typename T, size_t ArrSize>
class InfArray
    {
    public:
    using array_type = std::array<T,ArrSize>;
    using value_type = typename array_type::value_type;
    using size_type = typename array_type::size_type;
    using difference_type = typename array_type::difference_type;
    using reference = typename array_type::reference;
    using const_reference = typename array_type::const_reference;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    private:
    pointer data_ = nullptr;
    size_t size_ = 0;
    array_type arr_;
    std::vector<T> vec_;
    public:

    InfArray() 
      : size_(0) 
        { 
        data_ = &(arr_[0]);
#ifdef DEBUG
        if(size_ <= ArrSize) assert(data_==&(arr_[0]));
#endif
        }


    InfArray(size_t size)
        {
        if(size <= ArrSize)
            {
            data_ = &(arr_[0]);
            size_ = size;
            }
        else
            {
            //println("Resizing vec_ in InfArray");
            vec_.resize(size);
            data_ = vec_.data();
            size_ = vec_.size();
            }
#ifdef DEBUG
        if(size_ <= ArrSize) assert(data_==&(arr_[0]));
#endif
        }

    InfArray(size_t size,
             const_reference value) 
      : InfArray(size)
        { 
        std::fill(data_,data_+size_,value);
#ifdef DEBUG
        if(size_ <= ArrSize) assert(data_==&(arr_[0]));
#endif
        }

    InfArray(std::initializer_list<T> init) 
      : InfArray(init.size())
        { 
        auto p = data_;
        for(auto& el : init)
            {
            *p = el;
            ++p;
            }
        }

    InfArray(const InfArray& o) 
      : size_(o.size_),
        arr_(o.arr_),
        vec_(o.vec_)
        { 
        if(size_ <= ArrSize) data_ = &(arr_[0]);
        else                 data_ = vec_.data();
        }

    InfArray&
    operator=(const InfArray& o) 
        { 
        size_ = o.size_;
        arr_ = o.arr_;
        vec_ = o.vec_;
        if(size_ <= ArrSize) data_ = &(arr_[0]);
        else                 data_ = vec_.data();
        return *this;
        }

    InfArray(InfArray&& o) 
      : size_(o.size_),
        arr_(std::move(o.arr_)),
        vec_(std::move(o.vec_))
        { 
        if(size_ <= ArrSize) data_ = &(arr_[0]);
        else                 data_ = vec_.data();
        }

    InfArray&
    operator=(InfArray&& o) 
        { 
        size_ = o.size_;
        arr_ = std::move(o.arr_);
        vec_ = std::move(o.vec_);
        if(size_ <= ArrSize) data_ = &(arr_[0]);
        else                 data_ = vec_.data();
        return *this;
        }

    void
    resize(size_t new_size) 
        { 
        if(new_size > ArrSize)
            {
            //println("Resizing vec_ in InfArray");
            vec_.resize(new_size);
            if(size_ <= ArrSize)
                {
                auto pv = MAKE_SAFE_PTR(vec_.data(),vec_.size());
                std::copy(arr_.begin(),arr_.begin()+size_,pv);
                }
            data_ = vec_.data();
            }
        else //new_size <= ArrSize and not zero
            {
            if(size_ > ArrSize)
                {
                auto pa = MAKE_SAFE_PTR(&(arr_[0]),ArrSize);
                std::copy(vec_.begin(),vec_.begin()+new_size,pa);
                }
            vec_.clear();
            data_ = &(arr_[0]);
            }
        size_ = new_size; 
        }

    void
    assign(size_t new_size,
           const_reference val)
        {
        resize(new_size);
        std::fill(begin(),end(),val);
        }

    size_t
    size() const { return size_; }

    size_t constexpr
    arr_size() const { return ArrSize; }

    size_t
    vec_size() const { return vec_.size(); }

    void
    clear() 
        { 
        data_ = &(arr_[0]);
        size_ = 0; 
        vec_.clear();
        }

    explicit operator bool() const { return size_!=0; }

    reference
    operator[](size_t i) 
        { 
        CHECK_IND(i) 
#ifdef DEBUG
        if(size_ <= ArrSize) assert(data_==&(arr_[0]));
#endif
        return data_[i];
        }

    const_reference
    operator[](size_t i) const 
        { 
        CHECK_IND(i) 
#ifdef DEBUG
        if(size_ <= ArrSize) assert(data_==&(arr_[0]));
#endif
        return data_[i];
        }

    reference
    at(size_t i) { check_ind(i); return data_[i]; }

    const_reference
    at(size_t i) const { check_ind(i); return data_[i]; }

    pointer
    data() { return data_; }

    const_pointer
    data() const { return data_; }

    bool
    empty() const { return size_==0; }

    iterator
    begin() { return data_; }

    iterator
    end() { return data_+size_; }

    const_iterator
    begin() const { return data_; }

    const_iterator
    end() const { return data_+size_; }

    const_iterator
    cbegin() const { return data_; }

    const_iterator
    cend() const { return data_+size_; }

    private:
    void
    check_ind(size_t i) const
        {
        if(i >= size_) Error("index out of range in InfArray");
        }
    void
    check_empty() const
        {
        if(size_==0) Error("InfArray is empty");
        }
    };

//template<typename T, size_t ArrSize>
//class InfArray
//    {
//    //InfArray(std::initializer_list<T> init) 
//    //    { 
//    //    resize(init.size());
//    //    auto p = data_;
//    //    for(auto& el : init)
//    //        {
//    //        *p = el;
//    //        ++p;
//    //        }
//    //    }
//
//    size_t
//    size() const { return size_; }
//
//    size_t constexpr
//    arr_size() const { return ArrSize; }
//
//    size_t
//    vec_size() const { return vec_.size(); }
//
//    //void
//    //resize(size_t new_size) 
//    //    { 
//    //    if(new_size > ArrSize)
//    //        {
//    //        vec_.resize(new_size);
//    //        if(size_ <= ArrSize)
//    //            {
//    //            auto pv = MAKE_SAFE_PTR(vec_.data(),vec_.size());
//    //            std::copy(arr_.begin(),arr_.begin()+size_,pv);
//    //            }
//    //        data_ = vec_.data();
//    //        }
//    //    else //new_size <= ArrSize and not zero
//    //        {
//    //        if(size_ > ArrSize)
//    //            {
//    //            auto pa = MAKE_SAFE_PTR(&(arr_[0]),ArrSize);
//    //            std::copy(vec_.begin(),vec_.begin()+new_size,pa);
//    //            }
//    //        vec_.clear();
//    //        data_ = &(arr_[0]);
//    //        }
//    //    size_ = new_size; 
//    //    }
//
//    void
//    clear() 
//        { 
//        data_ = &(arr_[0]);
//        size_ = 0; 
//        vec_.clear();
//        }
//
//    //void
//    //push_back(const_reference val) 
//    //    { 
//    //    if(size_ < ArrSize) 
//    //        {
//    //        arr_[size_] = val; 
//    //        ++size_; 
//    //        }
//    //    else if(size_ == ArrSize)
//    //        {
//    //        resize(size_+1);
//    //        back() = val;
//    //        }
//    //    else                
//    //        {
//    //        vec_.push_back(val);
//    //        data_ = vec_.data();
//    //        ++size_;
//    //        }
//    //    }
//
//    //void
//    //push_back(value_type&& val) 
//    //    { 
//    //    if(size_ < ArrSize) 
//    //        {
//    //        arr_[size_] = std::move(val); 
//    //        ++size_; 
//    //        }
//    //    else if(size_ == ArrSize)
//    //        {
//    //        resize(size_+1);
//    //        back() = std::move(val);
//    //        }
//    //    else                
//    //        {
//    //        vec_.emplace_back(std::move(val));
//    //        data_ = vec_.data();
//    //        ++size_;
//    //        }
//    //    }
//
//    //void
//    //assign(size_t count, 
//    //       const_reference val) 
//    //    { 
//    //    resize(count);
//    //    fill(val);
//    //    }
//
//    explicit operator bool() const { return size_!=0; }
//
//    //reference
//    //operator[](size_t i) { CHECK_IND(i) return *(data_+i); }
//
//    //const_reference
//    //operator[](size_t i) const { CHECK_IND(i) return *(data_+i); }
//
//    //TODO DEBUG
//    reference
//    operator[](size_t i) 
//        { 
//        CHECK_IND(i) 
//        //return arr_[i]; 
//        //data_ = &(arr_[0]);
//#ifdef DEBUG
//        if(size_ <= ArrSize) assert(data_==&(arr_[0]));
//#endif
//        return data_[i];
//        }
//
//    //TODO DEBUG
//    const_reference
//    operator[](size_t i) const 
//        { 
//        CHECK_IND(i) 
//        //return arr_[i]; 
//        //auto cdata_ = &(arr_[0]);
//#ifdef DEBUG
//        if(size_ <= ArrSize) assert(data_==&(arr_[0]));
//#endif
//        return data_[i];
//        }
//
//    reference
//    at(size_t i) { check_ind(i); return data_[i]; }
//
//    const_reference
//    at(size_t i) const { check_ind(i); return data_[i]; }
//
//    reference
//    front() { CHECK_EMPTY return *data_; }
//
//    const_reference
//    front() const { CHECK_EMPTY return *data_; }
//
//    //reference
//    //back() { CHECK_EMPTY return data_[size_-1]; }
//
//    //const_reference
//    //back() const { CHECK_EMPTY return data_[size_-1]; }
//
//    pointer
//    data() { return data_; }
//
//    const_pointer
//    data() const { return data_; }
//
//    bool
//    empty() const { return size_==0; }
//
//    void
//    fill(const_reference val) 
//        { 
//        if(size_==0) return;
//        std::fill(data_,data_+size_,val);
//        }
//
//    void
//    swap(InfArray& other) 
//        { 
//        std::swap(data_,other.data_); 
//        std::swap(size_,other.size_); 
//        arr_.swap(other.arr_); 
//        vec_.swap(other.vec_);
//#ifdef DEBUG
//        if(size_ <= ArrSize) assert(data_==&(arr_[0]));
//#endif
//        }
//
//    iterator
//    begin() { return data_; }
//
//    iterator
//    end() { return data_+size_; }
//
//    const_iterator
//    begin() const { return data_; }
//
//    const_iterator
//    end() const { return data_+size_; }
//
//    const_iterator
//    cbegin() const { return data_; }
//
//    const_iterator
//    cend() const { return data_+size_; }
//
//    private:
//    void
//    check_ind(size_t i) const
//        {
//        if(i >= size_) Error("index out of range in InfArray");
//        }
//    void
//    check_empty() const
//        {
//        if(size_==0) Error("InfArray is empty");
//        }
//    };

} //namespace itensor

#undef CHECK_EMPTY
#undef CHECK_IND

#endif
