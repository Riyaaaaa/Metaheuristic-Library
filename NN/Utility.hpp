//
//  Utility.h
//  MTL_Development
//
//  Created by Riya.Liel on 2015/07/14.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef MTL_Development_Utility_h
#define MTL_Development_Utility_h

#include<utility>
#include<tuple>

namespace mtl{
    
    template<size_t begin, size_t end, bool terminate = begin + 1 == end>
    struct surfaceExecutePart;
    
    template<size_t begin, size_t end>
    struct surfaceExecutePart<begin, end, true>
    {
        template<typename Tuple, typename Function>
        static void Execute(Tuple && tuple, Function && function)
        {
            function(std::get<begin>(tuple),std::get<begin+1>(tuple));
        }
    };
    
    template<size_t begin, size_t end>
    struct surfaceExecutePart<begin, end, false>
    {
        template<typename Tuple, typename Function>
        static void Execute(Tuple && tuple, Function && function)
        {
            // execute first half
            surfaceExecutePart<begin, (begin + end) / 2>::Execute
            (std::forward<Tuple>(tuple), std::forward<Function>(function));
            
            // execute latter half
            surfaceExecutePart<(begin + end) / 2, end>::Execute
            (std::forward<Tuple>(tuple), std::forward<Function>(function));
        }
    };
    
    // pass all element of tuple to function
    template<typename Tuple, typename Function>
    void surfaceExecuteAll(Tuple && tuple, Function && function)
    {
        using namespace std;
        
        static const size_t end =
        tuple_size<typename remove_reference<Tuple>::type>::value-1;
        
        surfaceExecutePart<0, end>::Execute(
                                     std::forward<Tuple>(tuple),
                                     std::forward<Function>(function));
    }
    
    
    template <class Seq1, class Seq2>
    struct concat;
    
    template <class... Seq1, class... Seq2>
    struct concat<std::tuple<Seq1...>, std::tuple<Seq2...>> {
        typedef std::tuple<Seq1..., Seq2...> type;
    };
    
    template<class T, class Tuple,std::size_t... Dims>
    struct make_tuple_array{
        typedef Tuple type;
    };
    
    template<class T, class Tuple, std::size_t First, std::size_t... Dims>
    struct make_tuple_array<T,Tuple,First,Dims...>{
        typedef typename make_tuple_array< T, typename mtl::concat < Tuple, std::tuple<std::array<T,First>> >::type  , Dims... >::type type;
    };
    /*
    template<template<class T,std::size_t __Size> class Array, std::size_t _Size = __Size>
    struct array_size{
        static constexpr std::size_t Size = _Size;
    };
     */
}

#endif
