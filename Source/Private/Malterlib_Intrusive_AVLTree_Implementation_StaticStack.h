// Copyright © 2015 Hansoft AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NIntrusive
{
	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_BalanceLowest
		(
			CLink* &_pLowestObject
			, CLinkPointer &_pObject
			, CLinkPointer **_pStack
		)
	{
		auto *pStack = _pStack;

		CLinkPointer *pObject = &_pObject;

		CLink *pObj = CLink::fs_GetPtr(_pObject);
		while (pObj->f_GetLeftP())
		{
			*pStack = pObject;
			++pStack;
			pObject = pObj->f_GetLeft();
			pObj = CLink::fs_GetPtr(*pObject);
		}

		// Save the object that we want at targets place
		_pLowestObject = pObj;
		// Remove pObject from the tree
		CLink::f_Assign(pObject, pObj->f_GetRight());
		//_pStack = pStack;

		while (pStack != _pStack)
		{
			--pStack;
			if (!fsp_LeftShrunk(**pStack))
				return false;
		}

		return true;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_BalanceHighest
		(
			CLink* &_pHighestObject
			, CLinkPointer &_pObject
			, CLinkPointer **_pStack
		)
	{
		auto *pStack = _pStack;

		CLinkPointer *pObject = &_pObject;

		CLink *pObj = CLink::fs_GetPtr(_pObject);
		while (pObj->f_GetRightP())
		{
			*pStack = pObject;
			++pStack;
			pObject = pObj->f_GetRight();
			pObj = CLink::fs_GetPtr(*pObject);
		}

		// Save the object that we want at targets place
		_pHighestObject = pObj;
		// Remove pObject from the tree
		CLink::f_Assign(pObject, pObj->f_GetLeft());

		while (pStack != _pStack)
		{
			--pStack;
			if (!fsp_RightShrunk(**pStack))
				return false;
		}

		return true;

	}

	// No-rebalance version: finds and detaches the highest node in the subtree
	// Returns the detached node. Does NOT rebalance.
	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_DetachHighest(CLinkPointer &_pObject)
		-> CLink *
	{
		CLinkPointer *pObject = &_pObject;
		CLink *pObj = CLink::fs_GetPtr(_pObject);

		// Find rightmost node
		while (pObj->f_GetRightP())
		{
			pObject = pObj->f_GetRight();
			pObj = CLink::fs_GetPtr(*pObject);
		}

		// Detach: parent now points to our left child (if any)
		CLink::f_Assign(pObject, pObj->f_GetLeft());

		return pObj;
	}

	// No-rebalance version: finds and detaches the lowest node in the subtree
	// Returns the detached node. Does NOT rebalance.
	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_DetachLowest(CLinkPointer &_pObject)
		-> CLink *
	{
		CLinkPointer *pObject = &_pObject;
		CLink *pObj = CLink::fs_GetPtr(_pObject);

		// Find leftmost node
		while (pObj->f_GetLeftP())
		{
			pObject = pObj->f_GetLeft();
			pObj = CLink::fs_GetPtr(*pObject);
		}

		// Detach: parent now points to our right child (if any)
		CLink::f_Assign(pObject, pObj->f_GetRight());

		return pObj;
	}

	// No-rebalance version of fp_Removed - performs BST removal without rebalancing
	// Returns the replacement node (or nullptr if removed node was a leaf)
	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RemovedNoRebalance
		(
			CLinkPointer *_pObject
			, CLink *_pObj
		)
		-> CLink *
	{
		if (_pObj->f_GetLeftP())
		{
			// Has left child - find predecessor (highest in left subtree)
			CLink *pReplacement = fsp_DetachHighest(*_pObj->f_GetLeft());

			// Remove target from tree
			pReplacement->f_SetSkew(_pObj->f_GetSkew());
			_pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
			// Link predecessor in target's place
			pReplacement->f_SetLeft(_pObj->f_GetLeft());
			pReplacement->f_SetRight(_pObj->f_GetRight());
			CLink::f_Assign(_pObject, pReplacement);
			return pReplacement;
		}
		else if (_pObj->f_GetRightP())
		{
			// Has only right child - find successor (lowest in right subtree)
			CLink *pReplacement = fsp_DetachLowest(*_pObj->f_GetRight());

			// Remove target from tree
			pReplacement->f_SetSkew(_pObj->f_GetSkew());
			_pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
			// Link successor in target's place
			pReplacement->f_SetLeft(_pObj->f_GetLeft());
			pReplacement->f_SetRight(_pObj->f_GetRight());
			CLink::f_Assign(_pObject, pReplacement);
			return pReplacement;
		}
		else
		{
			// Leaf node - just remove
			_pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
			CLink::f_Assign(_pObject, (CLink *)nullptr);
			return nullptr;
		}
		// No rebalancing - intentionally skip the balancing loop
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_Removed
		(
			CLinkPointer *_pObject
			, CLink *_pObj
			, CTemporaryStack &_Stack
		)
	{
		CLinkPointer *pObject = _pObject;
		CLink *pObj = _pObj;

		auto *pStack = _Stack.m_pStack;
		auto *pStartStack = _Stack.m_Stack;
		auto *pLarger = _Stack.m_pLarger;

		if (pObj->f_GetLeftP())
		{
			CLink* pHighestObject;
			bool bLeftShrunk = fp_BalanceHighest(pHighestObject, *pObj->f_GetLeft(), pStack);

			// Remove target from tree
			pHighestObject->f_SetSkew(pObj->f_GetSkew());
			pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
			// Remove from last place
			// Link in on targets place
			pHighestObject->f_SetLeft(pObj->f_GetLeft());
			pHighestObject->f_SetRight(pObj->f_GetRight());
			CLink::f_Assign(pObject, pHighestObject);

			if (!bLeftShrunk)
				return;

			*pStack = pObject;
			*pLarger = false;
			++pStack;
			++pLarger;
		}
		else if (pObj->f_GetRightP())
		{
			CLink* pLowestObject;
			bool bRightShrunk = fp_BalanceLowest(pLowestObject, *pObj->f_GetRight(), pStack);

			// Remove target from tree
			pLowestObject->f_SetSkew(pObj->f_GetSkew());
			pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
			// Link in on targets place
			pLowestObject->f_SetLeft(pObj->f_GetLeft());
			pLowestObject->f_SetRight(pObj->f_GetRight());
			CLink::f_Assign(pObject, pLowestObject);

			if (!bRightShrunk)
				return;

			*pStack = pObject;
			*pLarger = true;
			++pStack;
			++pLarger;
		}
		else
		{
			pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
			CLink::f_Assign(pObject, (CLink *)nullptr);
		}

		while (pStack != pStartStack)
		{
			--pStack;
			--pLarger;
			if (*pLarger)
			{
				if (!fsp_RightShrunk(**pStack))
					break;
			}
			else
			{
				if (!fsp_LeftShrunk(**pStack))
					break;
			}
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_Remove(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_CCompare &&_fCompare)
	{
		DMibFastCheck(_pObjectToRemove);

		CTemporaryStack Stack;

		auto *pStack = Stack.m_Stack;
		auto *pLarger = Stack.m_Larger;

		CLinkPointer *pObject = &_pObject;

		CLink *pObj = CLink::fs_GetPtr(*pObject);
		while (pObj != _pObjectToRemove)
		{
			DMibFastCheck(pObj); // Object not found

			*pStack = pObject;
			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToRemove));
			static_assert(cIsOrderType<decltype(CompareResult)>);
#if !defined(DCompiler_MSVC) && false
			if constexpr (NTraits::cIsSame<decltype(CompareResult), COrdering_Strong>)
			{
				auto Direction = CompareResult < 0 ? 1 : 0;
				*pLarger = Direction;
				++pStack;
				++pLarger;
				pObject = pObj->f_GetNext(Direction);
			}
			else
#endif
			{
				if (CompareResult < 0)
				{
					*pLarger = true;
					++pStack;
					++pLarger;
					pObject = pObj->f_GetNext(1);

				}
				else
				{
					*pLarger = false;
					++pStack;
					++pLarger;
					pObject = pObj->f_GetNext(0);
				}
			}
			pObj = CLink::fs_GetPtr(*pObject);
		}

		Stack.m_pStack = pStack;
		Stack.m_pLarger = pLarger;
		fp_Removed(pObject, pObj, Stack);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey, typename tf_CCompare>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_FindEqualAndRemove(CLinkPointer &_pObject, tf_CKey const &_Key, tf_CCompare &&_fCompare)
		-> CNode *
	{
		CTemporaryStack Stack;

		auto *pStack = Stack.m_Stack;
		auto *pLarger = Stack.m_Larger;

		CLinkPointer *pObject = &_pObject;

		CLink *pObj = CLink::fs_GetPtr(*pObject);
		while (pObj)
		{
			DMibFastCheck(pObj); // Object not found

			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), _Key);
			static_assert(cIsOrderType<decltype(CompareResult)>);
			*pStack = pObject;
#if !defined(DCompiler_MSVC) && false
			if constexpr (NTraits::cIsSame<decltype(CompareResult), COrdering_Strong>)
			{
				if (CompareResult == 0)// [[unlikely]]
				{
					Stack.m_pStack = pStack;
					Stack.m_pLarger = pLarger;
					fp_Removed(pObject, pObj, Stack);
					return fsp_MemberFromLink(pObj);
				}

				auto Direction = CompareResult > 0 ? 0 : 1;

				*pLarger = Direction;
				++pStack;
				++pLarger;
				pObject = pObj->f_GetNext(Direction);
			}
			else
#endif
			{
				if (CompareResult < 0)
				{
					*pLarger = true;
					++pStack;
					++pLarger;
					pObject = pObj->f_GetNext(1);
				}
				else if (CompareResult > 0)
				{
					*pLarger = false;
					++pStack;
					++pLarger;
					pObject = pObj->f_GetNext(0);
				}
				else
				{
					Stack.m_pStack = pStack;
					Stack.m_pLarger = pLarger;
					fp_Removed(pObject, pObj, Stack);
					return fsp_MemberFromLink(pObj);
				}
			}

			pObj = CLink::fs_GetPtr(*pObject);
		}

		return nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <int tf_Direction>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RemoveRotate3(CLinkPointer *_pObject)
	{
		CLink *pD;
		CLink *pB;
		CLink *pC;
		CLink *pE;
		CLink *pF;

		pF = CLink::fs_GetPtr(*_pObject);
		pB = pF->f_GetNextP(1-tf_Direction);
		pD = pB->f_GetNextP(tf_Direction);
		pC = pD->f_GetNextP(1-tf_Direction);
		pE = pD->f_GetNextP(tf_Direction);

		CLink::f_Assign(_pObject, pD);

		pD->fp_SetNext(1-tf_Direction, pB);
		pD->fp_SetNext(tf_Direction, pF);
		pB->fp_SetNext(tf_Direction, pC);
		pF->fp_SetNext(1-tf_Direction, pE);

		pB->f_SetSkew(CLink::EAVLTreeSkew_None);
		pF->f_SetSkew(CLink::EAVLTreeSkew_None);
		int Skew = pD->f_GetSkew();
		if (Skew == tf_Direction)
			pB->f_SetSkew(1-tf_Direction);
		else if (Skew == 1-tf_Direction)
			pF->f_SetSkew(tf_Direction);
		pD->f_SetSkew(CLink::EAVLTreeSkew_None);

	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <int tf_Direction>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RemoveRotate2(CLinkPointer *_pObject)
	{
		CLink *pD;
		CLink *pB;
		CLink *pC;

		pD = CLink::fs_GetPtr(*_pObject);
		pB = pD->f_GetNextP(1-tf_Direction);
		pC = pB->f_GetNextP(tf_Direction);

		CLink::f_Assign(_pObject, pB);
		pB->fp_SetNext(tf_Direction, pD);
		pD->fp_SetNext(1-tf_Direction, pC);

		if (pB->f_IsBalanced())
		{
			pB->f_SetSkew(tf_Direction);
			pD->f_SetSkew(1-tf_Direction);
		}
		else
		{
			pB->f_SetSkew(CLink::EAVLTreeSkew_None);
			pD->f_SetSkew(CLink::EAVLTreeSkew_None);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_Insert(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_CCompare &&_fCompare)
	{
		CTemporaryStack Stack;

		auto *pStack = Stack.m_Stack;
		auto *pStartStack = Stack.m_Stack;
		auto *pLarger = Stack.m_Larger;

		CLinkPointer *pObject = &_pObject;
//				int iStack = 0;
		CLink *pObj = CLink::fs_GetPtr(*pObject);

		while (pObj)
		{
			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert));
			static_assert(cIsOrderType<decltype(CompareResult)>);
			*pStack = pObject;
#if !defined(DCompiler_MSVC) && false
			if constexpr (NTraits::cIsSame<decltype(CompareResult), COrdering_Strong>)
			{
				if (CompareResult == 0)// [[unlikely]]
				{
					DMibFastCheck(false); // Tree does not support inserting two equal objects
					return false;
				}

				auto Direction = CompareResult < 0 ? 1 : 0;
				*pLarger = Direction;
				++pStack;
				++pLarger;
				pObject = pObj->f_GetNext(Direction);
			}
			else
#endif
			{
				if (CompareResult < 0)
				{
					*pLarger = true;
					++pStack;
					++pLarger;
					pObject = pObj->f_GetNext(1);
				}
				else if (CompareResult > 0)
				{
					*pLarger = false;
					++pStack;
					++pLarger;
					pObject = pObj->f_GetNext(0);
				}
				else
				{
					DMibFastCheck(false); // Tree does not support inserting two equal objects
					return false;
				}
			}
			pObj = CLink::fs_GetPtr(*pObject);
		}

		_pObjectToInsert->f_Clear();

		CLink::f_Assign(pObject, _pObjectToInsert);
		while (pStack != pStartStack)
		{
			--pStack;
			--pLarger;
			if (*pLarger)
			{
				if (!fsp_RightGrown_Inl(**pStack))
					break;
			}
			else
			{
				if (!fsp_LeftGrown_Inl(**pStack))
					break;
			}
		}
		return true;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey, typename tf_FOnInsert, typename tf_CCompare>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_FindEqualOrInsert
		(
			CLinkPointer &_pObject
			, tf_CKey const &_Key
			, tf_FOnInsert &&_fOnInsert
			, tf_CCompare &&_fCompare
		)
		-> CNode *
	{
		CTemporaryStack Stack;

		auto *pStack = Stack.m_Stack;
		auto *pStartStack = Stack.m_Stack;
		auto *pLarger = Stack.m_Larger;

		CLinkPointer *pObject = &_pObject;
		CLink *pObj = CLink::fs_GetPtr(*pObject);

		while (pObj)
		{
			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), _Key);
			static_assert(cIsOrderType<decltype(CompareResult)>);
			*pStack = pObject;
#if !defined(DCompiler_MSVC) && false
			if constexpr (NTraits::cIsSame<decltype(CompareResult), COrdering_Strong>)
			{
				if (CompareResult == 0)// [[unlikely]]
					return fsp_MemberFromLink(pObj);

				auto Direction = CompareResult < 0 ? 1 : 0;

				*pLarger = Direction;
				++pStack;
				++pLarger;
				pObject = pObj->f_GetNext(Direction);
			}
			else
#endif
			{
				if (CompareResult < 0)
				{
					*pLarger = true;
					++pStack;
					++pLarger;
					pObject = pObj->f_GetNext(1);
				}
				else if (CompareResult > 0)
				{
					*pLarger = false;
					++pStack;
					++pLarger;
					pObject = pObj->f_GetNext(0);
				}
				else
					return fsp_MemberFromLink(pObj);
			}
			pObj = CLink::fs_GetPtr(*pObject);
		}

		auto pCreatedObject = _fOnInsert();
		if (!pCreatedObject)
			return nullptr;

		CLink *pObjectToInsert = fsp_LinkFromMember(pCreatedObject);

		if constexpr (CLinkContainer::mc_bNeedSetTree)
			((CLinkContainer *)pObjectToInsert)->f_SetTree(this, &TCAVLTreeAggregate::fsp_Remove);
		DMibFastCheck(pObjectToInsert->f_GetSkew() == CLink::EAVLTreeSkew_NotInTree); // Must not be in tree already

		pObjectToInsert->f_Clear();
		CLink::f_Assign(pObject, pObjectToInsert);
		while (pStack != pStartStack)
		{
			--pStack;
			--pLarger;
			if (*pLarger)
			{
				if (!fsp_RightGrown_Inl(**pStack))
					break;
			}
			else
			{
				if (!fsp_LeftGrown_Inl(**pStack))
					break;
			}
		}

		return pCreatedObject;
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Public																							|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Insert(CNode *_pToInsert, tf_CCompare &&_fCompare)
	{
		CLink * pToInsert = fsp_LinkFromMember(_pToInsert);
		if constexpr (CLinkContainer::mc_bNeedSetTree)
			((CLinkContainer *)pToInsert)->f_SetTree(this, &TCAVLTreeAggregate::fsp_Remove);
		DMibFastCheck(pToInsert->f_GetSkew() == CLink::EAVLTreeSkew_NotInTree); // Must not be in tree already

		return fp_Insert(m_Root, pToInsert, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Insert(CNode &_ToInsert)
	{
		return f_Insert(_ToInsert, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Insert(CNode &_ToInsert, tf_CCompare &&_fCompare)
	{
		return f_Insert(&_ToInsert, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Insert(CNode *_pToInsert)
	{
		return f_Insert(_pToInsert, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey, typename tf_FOnInsert, typename tf_CCompare>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindEqualOrInsert
		(
			tf_CKey const &_Key
			, tf_FOnInsert &&_fOnInsert
			, tf_CCompare &&_fCompare
		)
		-> CNode *
	{
		return fp_FindEqualOrInsert(m_Root, _Key, _fOnInsert, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey, typename tf_FOnInsert>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindEqualOrInsert(tf_CKey const &_Key, tf_FOnInsert &&_fOnInsert)
		-> CNode *
	{
		return fp_FindEqualOrInsert(m_Root, _Key, _fOnInsert, t_CCompare());
	}


	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey, typename tf_CCompare>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindEqualAndRemove(tf_CKey const &_Key, tf_CCompare &&_fCompare) -> CNode *
	{
		return fp_FindEqualAndRemove(m_Root, _Key, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindEqualAndRemove(tf_CKey const &_Key) -> CNode *
	{
		return fp_FindEqualAndRemove(m_Root, _Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Remove(CNode *_pToRemove, tf_CCompare &&_fCompare)
	{
		CLink * pToRemove = fsp_LinkFromMember(_pToRemove);

		fp_Remove(m_Root, pToRemove, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Remove(CNode &_ToRemove, tf_CCompare &&_fCompare)
	{
		f_Remove(&_ToRemove, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Remove(CNode &_ToRemove)
	{
		f_Remove(_ToRemove, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Remove(CNode *_pToRemove)
	{
		f_Remove(_pToRemove, t_CCompare());
	}
}
