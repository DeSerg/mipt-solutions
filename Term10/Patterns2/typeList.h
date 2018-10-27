
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
