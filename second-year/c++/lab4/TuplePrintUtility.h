#pragma once

#include <ostream>
#include <tuple>

template<std::size_t>
struct stupidCrutch {};

template<typename CharT, typename Traits, typename Tuple>
auto &putTupleInOstream(std::basic_ostream<CharT, Traits> &basicOstream, const Tuple &t, stupidCrutch<1>) {
    return basicOstream << std::get<std::tuple_size<Tuple>::value - 1>(t);
}

template<typename CharT, typename Traits, typename Tuple, size_t PosFromEnd>
auto &putTupleInOstream(std::basic_ostream<CharT, Traits> &basicOstream, const Tuple &tuple, stupidCrutch<PosFromEnd>){
    basicOstream << std::get<std::tuple_size<Tuple>::value - PosFromEnd>(tuple) << ", ";
    return putTupleInOstream(basicOstream, tuple, stupidCrutch<PosFromEnd - 1>());
}

template<typename CharT, typename Traits, typename ... Args>
auto &operator<<(std::basic_ostream<CharT, Traits> &basicOstream, const std::tuple<Args...> &t) {
    basicOstream << '(';
    putTupleInOstream(basicOstream, t, stupidCrutch<sizeof...(Args)>());
    basicOstream << ')';
    return basicOstream;
}
