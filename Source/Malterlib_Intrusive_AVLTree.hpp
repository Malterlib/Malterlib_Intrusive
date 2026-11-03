// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

namespace NMib::NIntrusive::NPrivate
{
	template <typename t_CInner>
	TCImplicitConvert<t_CInner>::TCImplicitConvert(t_CInner &_Inner)
		: m_Inner(_Inner)
	{
	}

	template <typename t_CInner>
	TCImplicitConvert<t_CInner>::operator t_CInner & ()
	{
		return m_Inner;
	}
}

namespace NMib::NIntrusive
{
	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	constexpr TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCAVLTreeAggregate(EAggregateInitialization _Init)
		: m_Root{nullptr}
	{
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FCompare, typename tf_CNode>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_GetKey(tf_FCompare &&_fCompare, tf_CNode &&_Node)
		-> NTraits::TCCallableReturnTypeFor<NTraits::TCRemoveReference<tf_FCompare>, void (NPrivate::TCImplicitConvert<tf_CNode &&> _Node)>
		requires (NTraits::cIsCallableWith<NTraits::TCRemoveReference<tf_FCompare>, void (NPrivate::TCImplicitConvert<tf_CNode &&> _Node)>)
	{
		return _fCompare(fg_Forward<tf_CNode>(NPrivate::TCImplicitConvert<tf_CNode>(_Node)));
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FCompare, typename tf_CNode>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_GetKey(tf_FCompare &&_fCompare, tf_CNode &&_Node)
		-> NTraits::TCRemoveRValueReference<tf_CNode>
		requires (!NTraits::cIsCallableWith<NTraits::TCRemoveReference<tf_FCompare>, void (NPrivate::TCImplicitConvert<tf_CNode &&> _Node)>)
	{
		return _Node;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FCompare, typename tf_CLeft, typename tf_CRight>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_DoCompare(tf_FCompare &&_fCompare, tf_CLeft &&_Left, tf_CRight &&_Right)
		requires (NTraits::cIsCallableWith<NTraits::TCRemoveReference<tf_FCompare>, void (tf_CLeft &&_Left, tf_CRight &&_Right)>)
	{
		return _fCompare(_Left, _Right);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FCompare, typename tf_CLeft, typename tf_CRight>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_DoCompare(tf_FCompare &&_fCompare, tf_CLeft &&_Left, tf_CRight &&_Right)
		requires (!NTraits::cIsCallableWith<NTraits::TCRemoveReference<tf_FCompare>, void (tf_CLeft &&_Left, tf_CRight &&_Right)>)
	{
		return _Left <=> _Right;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FCompare, typename tf_CLeft, typename tf_CRight>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_Compare(tf_FCompare &&_fCompare, tf_CLeft &&_Left, tf_CRight &&_Right)
	{
		return fsp_DoCompare(fg_Forward<tf_FCompare>(_fCompare), fsp_GetKey(_fCompare, fg_Forward<tf_CLeft>(_Left)), fsp_GetKey(_fCompare, fg_Forward<tf_CRight>(_Right)));
	}

#ifdef DCompiler_clang
	template <EAVLLinkType t_LinkType, typename t_CLinkData>
	constexpr TCAVLLinkAggregate<t_LinkType, t_CLinkData>::TCAVLLinkAggregate()
		: m_Link()
	{
	}
#endif
}
