#pragma once
#include <GeneralDefinitions.h>
#include <DynamicPatch.h>
#include <unordered_map>
#include <Memory.h>

//For Non-virtual Classes
template <typename RealType>
class NOVTABLE ImplBase
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
public:
	static const constexpr vtable_t vtable_addr = VTable;

	vtable_t& vtable() noexcept
	{
		return *reinterpret_cast<vtable_t*>(this);
	}

	const vtable_t& vtable() const noexcept
	{
		return *reinterpret_cast<const vtable_t*>(this);
	}

	void reset_vtable() noexcept
	{
		this->vtable() = vtable_addr;
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
    struct _ClassName##_Implement_Caller { \
        _ClassName##_Implement_Caller() { \
            _ClassName::Implement(); \
        } \
    } _ClassName##_implement_caller; \
} 

#define IMPLEMENT(Addr, Method) \
	CodeModifier::InsertFarJump(Addr, union_cast<void*>(&Method));
#define DEFINE_IMPLEMENT \
	static void Implement()

#define DEFINE_VIRTUAL_IMPL(_ClassName, _VTable) \
	class NOVTABLE _ClassName ## _Impl : public VirtualImplBase<_ClassName, _VTable>

#define DEFINE_IMPL(_ClassName) \
	class NOVTABLE _ClassName ## _Impl : public ImplBase<_ClassName> 

#define DEFINE_IMPL_END(_ClassName) \
	IMPL_TYPE_DEFINE(_ClassName ## _Impl)

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