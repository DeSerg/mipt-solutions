template<class T>
class CNewCreatorStrategy {
public:
static T* create() { return new T; }
};

template<class T>
class CMallocCreatorStrategy {
public:
static T* create() {
void* buf = malloc(sizeof(T));
if (!buf) return 0;
return new(buf) T;
}
};

template<class T>
class CPrototypeCreatorStrategy {
private:
T* m_prototype;
public:
CPrototypeCreatorStrategy(...) { ... }
static T* create() {
return m_prototype ?
m_prototype->clone() :
0;

}
// ... get/set methods
};


// код библиотеки
template<template <class Created> class CreationPolicy>
class CWidgetManager : public CreationPolicy<Widget> {
public:
void func(...) {
Gadget* g = CreationPolicy<Gadget>().create();
}
};

// код приложения
typedef CWidgetManager<CNewCreatorStrategy> MyWidgetManager;

CNewCreatorStrategy *mngr = new MyWidgetManager();

Чтобы избежать undefinde behavour, вызванного отсутствием вызова деструктора MyWidgetManager, следует добавить виртуальные деструкторы в классы стратегий.

