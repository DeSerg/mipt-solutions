#include <iostream>
#include <typeinfo>

class NullType {};

template<typename T=NullType, typename... U>
struct TypeList {
    using head = T;
    using tail = TypeList<U...>;
};

template<typename T>
struct TypeList<T> {
    using head = T;
    using tail = NullType;
};

using EmptyList = TypeList<>;


// Length
template <typename TypeList>
struct Length;

template<>
struct Length<EmptyList> {
    enum { value = 0 };
};

template <typename T, typename... U>
struct Length<TypeList<T, U...>> {
    enum { value = Length<TypeList<U...>>::value + 1 };
};


// TypeAt
template<unsigned int i, class TypeList>
struct TypeAt;

template<class T, class... U>
struct TypeAt<0, TypeList<T, U...>> {
    using Result = T;
};

template<unsigned int i, class T, class... U>
struct TypeAt<i, TypeList<T, U...>> {
    using Result = typename TypeAt<i - 1, TypeList<U...>>::Result;
};


// Erase
template<class Type, class TypeList>
struct Erase;

template<class Type>
struct Erase<Type, EmptyList> {
    using Result = NullType;
};

template<class Type, class... U>
struct Erase<Type, TypeList<Type, U...>> {
    using Result = TypeList<U...>;
};

template<class Type, class T, class... U>
struct Erase<Type, TypeList<T, U...>> {
    using Result = typename Erase<Type, TypeList<U...>>::Result;
};


using List1 = TypeList<bool>;
using List2 = TypeList<double, float>;
using List3 = TypeList<char, int, long>;


int main() {
    
    // length
    std::cout << "EmptyList length: " <<  Length<EmptyList>::value << std::endl;
    std::cout << "List1 length: " <<  Length<List1>::value << std::endl;
    std::cout << "List2 length: " <<  Length<List2>::value << std::endl;
    std::cout << "List3 length: " <<  Length<List3>::value << std::endl;

    // type at
    std::cout << "List1:" << std::endl;
    std::cout << typeid(TypeAt<0, List1>::Result).name() << std::endl;

    std::cout << "List2:" << std::endl;
    std::cout << typeid(TypeAt<0, List2>::Result).name() << std::endl;
    std::cout << typeid(TypeAt<1, List2>::Result).name() << std::endl;

    std::cout << "List3:" << std::endl;
    std::cout << typeid(TypeAt<0, List3>::Result).name() << std::endl;
    std::cout << typeid(TypeAt<1, List3>::Result).name() << std::endl;
    std::cout << typeid(TypeAt<2, List3>::Result).name() << std::endl;

    using List1Erased = Erase<bool, List1>::Result;
    std::cout << "List1Erased length: " <<  Length<List1Erased>::value << std::endl;

    using List2Erased = Erase<float, List2>::Result;
    std::cout << "List2Erased length: " <<  Length<List2Erased>::value << std::endl;
    std::cout << "List2Erased at 0: " << typeid(TypeAt<0, List2Erased>::Result).name() << std::endl;
}


