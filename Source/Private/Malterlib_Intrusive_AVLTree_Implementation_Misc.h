// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NIntrusive
{
	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Aggregate private																					|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#ifdef DMibDebuggerHelpers
	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fs_Debug_GetNode()
		-> typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	{
		return nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>*
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::fs_Debug_GetTree()
	{
		return nullptr;
	}
#endif

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	return_not_aliased inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CLink *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_LinkFromMember(CNode *_pMember)
	{
		return &((_pMember->*t_pLinkMember).m_Link);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	return_not_aliased inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CLink *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_LinkFromMemberConst(const CNode *_pMember)
	{
		return &((_pMember->*t_pLinkMember).m_Link);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	return_not_aliased inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_MemberFromLink(CLink *_pLink)
	{
#ifdef DCompiler_MSVC_Workaround
		mint Offset = DMibRelaxConstexpr((smint)(&(((CNode *)nullptr)->*t_pLinkMember).m_Link));
#else
		static constexpr mint Offset = DMibRelaxConstexpr((smint)(&(((CNode *)nullptr)->*t_pLinkMember).m_Link));
#endif
		return ((CNode *)(((uint8 *)_pLink) - Offset));
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	return_not_aliased inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_MemberFromLinkConst(const CLink *_pLink)
	{
#ifdef DCompiler_MSVC_Workaround
		mint Offset = DMibRelaxConstexpr((smint)(&(((CNode *)nullptr)->*t_pLinkMember).m_Link));
#else
		static constexpr mint Offset = DMibRelaxConstexpr((smint)(&(((CNode *)nullptr)->*t_pLinkMember).m_Link));
#endif
		return ((CNode *)(((uint8 *)_pLink) - Offset));
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_Remove(void *_pTree, CLink &_Link)
	{
		TCAVLTreeAggregate *pTree = static_cast<TCAVLTreeAggregate *>(_pTree);
		pTree->f_Remove(fsp_MemberFromLink(&_Link));
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Aggregate public																					|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fs_MemberFromLink(CLink *_pLink)
	{
		return fsp_MemberFromLink(_pLink);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_IsEmpty() const
	{
		return !(CLink::fs_GetPtr(m_Root));
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_HasOneElement() const
	{
		CLink * pObject = CLink::fs_GetPtr(m_Root);
		if (pObject)
		{
			if (!pObject->f_GetLeftP() && !pObject->f_GetRightP())
				return true;
		}
		return false;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_GetRoot() const
	{
		if (CLink::fs_GetPtr(m_Root))
			return fsp_MemberFromLink(CLink::fs_GetPtr(m_Root));
		else
			return nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fs_GetRight(CNode *_pObject)
	{
		CLink * pObject = fsp_LinkFromMember(_pObject);

		if (pObject->f_GetRightP())
			return fsp_MemberFromLink(pObject->f_GetRightP());
		else
			return nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fs_GetLeft(CNode *_pObject)
	{
		CLink * pObject = fsp_LinkFromMember(_pObject);

		if (pObject->f_GetLeftP())
			return fsp_MemberFromLink(pObject->f_GetLeftP());
		else
			return nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium mint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_GetLen() const
	{
		TIterator<> Iter(this);
		mint Len = 0;
		while (Iter)
		{
			++Len;
			++Iter;
		}
		return Len;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	mint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_GetDepth() const
	{
		auto pRoot = CLink::fs_GetPtr(m_Root);
		if (!pRoot)
			return 0;

		return fp_GetDepthRecursive(pRoot);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::operator == (const TCAVLTreeAggregate &_Other) const
	{
		TIterator<> Iter0 = *this;
		TIterator<> Iter1 = _Other;

		while (Iter0 && Iter1)
		{
			if (!(*Iter0 == *Iter1))
				return false;
			++Iter0;
			++Iter1;
		}

		if (!!Iter0 != !!Iter1)
			return false;

		return true;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::operator <=> (const TCAVLTreeAggregate &_Other) const
	{
		TIterator<> Iter0 = *this;
		TIterator<> Iter1 = _Other;

		using COrdering = decltype(*Iter0 <=> *Iter1);

		while (Iter0 && Iter1)
		{
			if (auto Result = *Iter0 <=> *Iter1; Result != 0)
				return Result;

			++Iter0;
			++Iter1;
		}

		if (Iter0 && !Iter1)
			return COrdering::greater;
		if (!Iter0 && Iter1)
			return COrdering::less;

		return COrdering::equivalent;
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Public																							|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	bool TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::operator == (const TCAVLTree &_Other) const
	{
		return (const CSuper &)(*this) == (const CSuper &)_Other;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	auto TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::operator <=> (const TCAVLTree &_Other) const
	{
		return (const CSuper &)(*this) <=> (const CSuper &)_Other;
	}
}
