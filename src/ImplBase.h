#pragma once
#include <GeneralDefinitions.h>
#include <DynamicPatch.h>
#include <unordered_map>

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
#define This (this->_This())

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

template<typename T, typename V>
void reset_vtable(V* ptr) noexcept
{
	reinterpret_cast<T*>(ptr)->reset_vtable();
}

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

#define IMPL_TYPE_DEFINE(_ClassName) \
namespace _ClassName##_Impl_Namespace { \
    inline struct _ClassName##_Implement_Caller { \
        _ClassName##_Implement_Caller() { \
            _ClassName::Implement(); \
        } \
    } _ClassName##_implement_caller; \
} 

#define IMPLEMENT(Addr, Method) \
	CodeModifier::InsertFarJump(Addr, union_cast<void*>(&Method));
#define DEFINE_IMPLEMENT \
	inline static void Implement()

#define DEFINE_VIRTUAL_IMPL(_ClassName, _VTable) \
	class NOVTABLE _ClassName ## _Impl : public VirtualImplBase<_ClassName, _VTable>, public _ClassName ## _Data

#define DEFINE_IMPL(_ClassName) \
	class NOVTABLE _ClassName ## _Impl : public ImplBase<_ClassName> 

#define DEFINE_IMPL_END(_ClassName) \
	IMPL_TYPE_DEFINE(_ClassName ## _Impl) \
	static_assert(sizeof(_ClassName ## _Impl) == sizeof(_ClassName), #_ClassName " is different from its Impl in size!");

#define MAKE_DTOR \
	void StaticDTOR() { this->DTOR(); }\
	real_type* DynamicDTOR(bool _Delete) \
	{ \
		if((_Delete & 1) != 0) \
			DeleteThis(); \
		return _This(); \
	}

#define BASE_CLASS_DTOR(_Type) \
	(DestroyBase<_Type ## _Impl>())

#define DEFINE_DATA_TYPE(_ClassName) \
	struct _ClassName ## _Data
#define BASE_DATA(_Type) \
	public _Type ## _Data