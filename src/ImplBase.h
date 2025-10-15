#pragma once
#include <GeneralDefinitions.h>
#include <DynamicPatch.h>
#include <unordered_map>

/*

实现类的定义。
所有实现类都不可构造。
Impl Base Classes Definition.
All Impl Classes are Non-constructible.

*/
class NonConstructible {
public:
	NonConstructible() = delete;
	NonConstructible(const NonConstructible&) = delete;
	NonConstructible(NonConstructible&&) = delete;

	template<typename... Args>
	NonConstructible(Args...) = delete;

	void* operator new(std::size_t) = delete;
	void* operator new[](std::size_t) = delete;
};

//For Non-virtual Classes
template <typename RealType>
class NOVTABLE ImplBase : NonConstructible
{
protected:
	void DeleteThis() const
	{
		YRMemory::Deallocate(this);
	}

	template<typename BaseType>
	void DestroyBase()
	{
		reinterpret_cast<BaseType*>(this)->DTOR();
	}
public:
	using real_type = RealType;

	real_type* _This() noexcept
	{
		return reinterpret_cast<real_type*>(this);
	}
	const real_type* _This() const noexcept
	{
		return reinterpret_cast<const real_type*>(this);
	}
};

//For Virtual Classes
using vtable_t = size_t;
template <typename RealType, vtable_t VTable>
class NOVTABLE VirtualImplBase : public ImplBase<RealType>
{
	vtable_t VTablePadding;
public:
	static const constexpr vtable_t vtable_addr = VTable;

	vtable_t& vtable() noexcept
	{
		return VTablePadding;
	}

	const vtable_t& vtable() const noexcept
	{
		return VTablePadding;
	}

	void reset_vtable() noexcept
	{
		VTablePadding = vtable_addr;
	}
};

/*

在实现类的成员函数当中，使用This宏访问实际类型。
For Member Functions in Impl Classes,
Use MACRO This to have access to the actual class.

*/
#define This (this->_This())

/*

重置虚表指针到T类型的虚表。
Reset VTable pointer to VTable of type T.

*/
template<typename T, typename V>
void reset_vtable(V* ptr) noexcept
{
	reinterpret_cast<T*>(ptr)->reset_vtable();
}

/*

转换成员函数等指针到void*等
不受任何约束限制。
Cast Member Function Pointers to void* etc.
This function is unrestricted.

*/
template<typename dst_type, typename src_type>
dst_type constexpr union_cast(src_type src)
{
	union {
		src_type s;
		dst_type d;
	}u;
	u.s = src;
	return u.d;
}

/*
辅助类型，不直接使用
Helper Class, NO Direct Usage.
*/
struct FunctionImplHelper
{
	FunctionImplHelper() = delete;
	FunctionImplHelper(size_t Addr, void* Func)
	{
		CodeModifier::InsertFarJump(Addr, Func);
	}
	~FunctionImplHelper() = default;
};

/*
辅助宏，不直接使用
Helper Macro, NO Direct Usage.
*/
#define IMPL_TYPE_DEFINE(_ClassName) \
namespace _ClassName##_Impl_Namespace { \
    inline struct _ClassName##_Implement_Caller { \
        _ClassName##_Implement_Caller() { \
            _ClassName::Implement(); \
        } \
    } _ClassName##_implement_caller; \
} 

/*

定义实现类的实现替换函数。
Defining Implementation Replacing Functions.

示例/Example：

(in class definition)
DEFINE_IMPLEMENT
{
	IMPLEMENT(Addr1, Method1)
	...
}

详见示例 FileClass_Impl.h
See details in FileClass_Impl.h

*/
#define IMPLEMENT(Addr, Method) \
	CodeModifier::InsertFarJump(Addr, union_cast<void*>(&Method));
#define DEFINE_IMPLEMENT \
	inline static void Implement()


/*

定义实现类的开头与结尾。
Defining Implementation Class Begin/End.

用法/Usage：

T是被实现的类。
T is the class to be implemented.

非虚类/Non-Virtual:

DEFINE_IMPL(T)
{
	...
};
DEFINE_IMPL_END(T)

非虚类/Virtual:

DEFINE_VIRTUAL_IMPL(T, VTableAddress)
{
	...
};
DEFINE_IMPL_END(T)

详见示例 FileClass_Impl.h
See details in FileClass_Impl.h

*/
#define DEFINE_VIRTUAL_IMPL(_ClassName, _VTable) \
	class NOVTABLE _ClassName ## _Impl : public VirtualImplBase<_ClassName, _VTable>, public _ClassName ## _Data
#define DEFINE_IMPL(_ClassName) \
	class NOVTABLE _ClassName ## _Impl : public ImplBase<_ClassName> 
#define DEFINE_IMPL_END(_ClassName) \
	IMPL_TYPE_DEFINE(_ClassName ## _Impl) \
	static_assert(sizeof(_ClassName ## _Impl) == sizeof(_ClassName), #_ClassName " is different from its Impl in size!");

/*

为实现类配置成员变量。
Defining Member Variables for Impl Classes.

用法/Usage：

若被实现的类是T，其基类为B，则放置下列代码于
实现类定义的DEFINE_IMPL/DEFINE_VIRTUAL_IMPL前面：
If the class being implemented is T, whose base class is B,
it is advised to write the following code before 
DEFINE_IMPL/DEFINE_VIRTUAL_IMPL of Impl Classes.

DEFINE_DATA_TYPE(T) : BASE_DATA(B)
{
	...
};

详见示例 FileClass_Impl.h
See details in FileClass_Impl.h

*/
#define DEFINE_DATA_TYPE(_ClassName) \
	struct _ClassName ## _Data
#define BASE_DATA(_Type) \
	public _Type ## _Data

/*

自动生成静态与动态析构函数。
要求需要类似析构函数的"void DTOR();"函数
Auto Generate Static & Dynamic Destructors.
Require Destructor-like function "void DTOR();"

详见示例 FileClass_Impl.h
See details in FileClass_Impl.h

*/
#define MAKE_DTOR \
	void StaticDTOR() { this->DTOR(); }\
	real_type* DynamicDTOR(bool _Delete) \
	{ \
		if((_Delete & 1) != 0) \
			DeleteThis(); \
		return _This(); \
	}


/*

调用基类的析构函数，用在继承链上逐个析构。
Calling base class's destructor.
For destroying on the inheritance chain.

若被实现的类的基类是B，则在实现类的DTOR()中可以使用BASE_CLASS_DTOR(T)
if B is the base of the class being implemented,
BASE_CLASS_DTOR(T) is available in DTOR() of the impl class.

详见示例 FileClass_Impl.h
See details in FileClass_Impl.h

*/
#define BASE_CLASS_DTOR(_Type) \
	(DestroyBase<_Type ## _Impl>())

/*

为普通函数生成实现补丁，放在函数的实现后面。
Generate Implementation Patch for Non-member Functions.
For use after the function's implementation.

*/
#define FUNCTION_IMPLEMENT(Addr, Func) \
	FunctionImplHelper _ImplHelper_ ## Addr ## Func {Addr, union_cast<void*>(Func)};

/*

为尚未实现的函数生成跳转到原函数的补丁，放在函数的声明后面。
Generate Jump-to-Original Patch for Not-Yet-Implemented Functions.
For use after the function's declaration.

*/
#define FUNCTION_NOT_YET_IMPLEMENTED(Addr) \
	{ JMP_STD((Addr)) }