//
// Distributed under the ITensor Library License, Version 1.2
//    (See accompanying LICENSE file.)
//
#ifndef __ITENSOR_CALL_REWRITE_H
#define __ITENSOR_CALL_REWRITE_H

#include <type_traits>
#include <utility>
#include <stdexcept>
#include "itensor/util/stdx.h"

namespace itensor {
namespace detail {

//
// The call(obj,v) function uses substitution-failure-is-not-an-error (sfinae)
// to either "plug" v into obj's operator() method if it has one defined
// or else to throw a runtime error if not.
//
// Also, if obj's operator() method exists but returns void instead of type "Ret",
// rewrites the call to return a default-constructed Ret().
//
// Use call(obj,v) to convert the absence of a specific operator() method
// to be a run-time error instead of a compile-time error.
//
template <typename Ret, class T, typename V>
Ret
call(T&& obj, V&& v);

//Implementation:

template <typename Ret, class T, typename V>
auto 
call_impl(T&& obj, V&& v, int) -> decltype(obj(std::forward<V>(v)))
    {
    return obj(std::forward<V>(v));
    }
template <typename Ret, class T, typename V>
Ret
call_impl(T&& obj, V&& v, long) 
    {
    throw std::runtime_error("Object does not support operator(v)->Ret for specified type.");
    return Ret();
    }
template <typename Ret, class T, typename V>
stdx::enable_if_t<std::is_same<typename std::result_of<T(V)>::type,void>::value,Ret>
fixret(T&& obj, V&& v, int)
    {
    call_impl<void,T,V>(std::forward<T>(obj),std::forward<V>(v),0);
    return Ret();
    }
template <typename Ret, class T, typename V>
Ret
fixret(T&& obj, V&& v, long)
    {
    return call_impl<Ret,T,V>(std::forward<T>(obj),std::forward<V>(v),0);
    }
template <typename Ret, class T, typename V>
Ret
call(T&& obj, V&& v)
    {
    return fixret<Ret,T,V>(std::forward<decltype(obj)>(obj),std::forward<V>(v),0);
    }

/////////////////////////

//
// The call(obj,v) function uses substitution-failure-is-not-an-error (sfinae)
// to either "plug" v into obj's operator() method if it has one defined
// or else to throw a runtime error if not.
// Use call(obj,v) to convert the absence of a specific operator() method
// to be a run-time error instead of a compile-time error.
//
template <class T, typename V>
void
call(T&& obj, V&& v);

//Implementation:

template <class T, typename V>
auto 
call_impl(T&& obj, V&& v, int)
    -> stdx::conditional_t<std::is_same<decltype(obj(v)),void>::value,int,int>
    {
    obj(std::forward<V>(v));
    return 0;
    }
template <class T, typename V>
void
call_impl(T&& obj, V&& v, long) 
    {
    throw std::runtime_error("Object does not support operator(v) for specified type.");
    }
template <class T, typename V>
void
call(T&& obj, V&& v)
    {
    call_impl<T,V>(std::forward<T>(obj),std::forward<V>(v),0);
    }

/////////////////////////

//
// The call(obj,v1,v2) function uses substitution-failure-is-not-an-error (sfinae)
// to either "plug" v1,v2 into obj's operator() method if it has one defined
// or else to throw a runtime error if not.
//
// Use call(obj,v1,v2) to convert the absence of a specific operator() method
// to be a run-time error instead of a compile-time error.
//
template <typename Ret, class FType, typename T1, typename T2>
Ret
call(FType&& func, T1&& a1, T2&& a2);

//Implementation:

template <typename Ret, class FType, typename T1, typename T2>
auto 
call_impl(FType&& func, T1&& a1, T2&& a2, int) -> decltype(func(std::forward<T1>(a1),std::forward<T2>(a2)))
    {
    return func(std::forward<T1>(a1),std::forward<T2>(a2));
    }
template <typename Ret, class FType, typename T1, typename T2>
Ret
call_impl(FType&& func, T1&& a1, T2&& a2, long) 
    {
    throw std::runtime_error("Object does not support operator(v1,v2)->Ret for specified type.");
    return Ret();
    }
template <typename Ret, class FType, typename T1, typename T2>
stdx::enable_if_t<std::is_same<typename std::result_of<FType(T1,T2)>::type,void>::value,Ret>
fixret(FType&& func, T1&& a1, T2&& a2, int)
    {
    call_impl<void,FType,T1,T2>(std::forward<FType>(func),
                                std::forward<T1>(a1),
                                std::forward<T2>(a2),0);
    return Ret();
    }
template <typename Ret, class FType, typename T1, typename T2>
Ret
fixret(FType&& func, T1&& a1, T2&& a2, long)
    {
    return call_impl<Ret,FType,T1,T2>(std::forward<FType>(func),
                                      std::forward<T1>(a1),
                                      std::forward<T2>(a2),0);
    }
template <typename Ret, class FType, typename T1, typename T2>
Ret
call(FType&& func, T1&& a1, T2&& a2)
    {
    return fixret<Ret,FType,T1,T2>(std::forward<FType>(func),
                                   std::forward<T1>(a1),
                                   std::forward<T2>(a2),0);
    //return call_impl<Ret,FType,T1,T2>(std::forward<FType>(func),
    //                                  std::forward<T1>(a1),
    //                                  std::forward<T2>(a2),0);
    }

/////////////////////////

//
// The call(obj,v1,v2) function uses substitution-failure-is-not-an-error (sfinae)
// to either "plug" v1,v2 into obj's operator() method if it has one defined
// or else to throw a runtime error if not.
// Use call(obj,v1,v2) to convert the absence of a specific operator() method
// to be a run-time error instead of a compile-time error.
//
template <class T, typename V1, typename V2>
void
call(T&& obj, V1&& v1, V2&& v2);

//Implementation:

template <class T, typename V1, typename V2>
auto 
call_impl(T&& obj, V1&& v1, V2&& v2, int)
    -> stdx::conditional_t<std::is_same<decltype(obj(std::forward<V1>(v1),std::forward<V2>(v2))),void>::value,int,int>
    {
    obj(std::forward<V1>(v1),std::forward<V2>(v2));
    return 0;
    }
template <class T, typename V1, typename V2>
void
call_impl(T&& obj, V1&& v1, V2&& v2, long) 
    {
    throw std::runtime_error("Object does not support operator(v1,v2) for specified type.");
    }
template <class T, typename V1, typename V2>
void
call(T&& obj, V1&& v1, V2&& v2)
    {
    call_impl<T,V1,V2>(std::forward<T>(obj),
                       std::forward<V1>(v1),
                       std::forward<V2>(v2),0);
    }


} //namespace detail
} //namespace itensor

#endif

