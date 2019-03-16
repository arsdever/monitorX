#define IMPL_BEGIN(__CLASS, __INTERFACE) \
private: class __INTERFACE##_impl : public __INTERFACE { \
    private: __CLASS *__this; \
    public: __INTERFACE##_impl() { __this = (__CLASS*)((unsigned __int64)this - (unsigned __int64)(&((__CLASS*)0)->__INTERFACE##_obj)); }

#define IMPL_END(__INTERFACE) \
} __INTERFACE##_obj; \
public: __INTERFACE * GetInterface_##__INTERFACE() { return (__INTERFACE *)&__INTERFACE##_obj; } \
private: