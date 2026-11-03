// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

namespace NMib::NIntrusive
{
	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Public																							|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::TCIterator()
	{
		// Reset iterator
		mp_iStack = -1;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_GetIterator() const -> TCIterator
	{
		return *this;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_medium TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::TCIterator(TCIterator const &_iOther)
	{
		mp_iStack = _iOther.mp_iStack;

		for (aint iEntry = 0; iEntry <= mp_iStack; ++iEntry)
			mp_Stack[iEntry] = _iOther.mp_Stack[iEntry];
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::TCIterator(TCAVLTreeAggregate const &_Tree)
	{
		f_StartForward(&_Tree);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::TCIterator(TCAVLTreeAggregate const *_pTree)
	{
		f_StartForward(_pTree);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_medium umint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_GetLen() const
	{
		TCIterator iNode(*this);
		umint Len = 0;
		while (iNode)
		{
			++Len;
			++iNode;
		}
		return Len;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_Clear()
	{
		mp_iStack = -1;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_StartForward(TCAVLTreeAggregate const &_Tree)
	{
		f_StartForward(&_Tree);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_StartForward(TCAVLTreeAggregate const *_pTree)
	{
		// Find the smallest item in tree, and build stack
		CLink const *pCurrent = CLink::fs_GetPtr(_pTree->m_Root);
		aint iStack = -1;

		while (pCurrent)
		{
			mp_Stack[++iStack] = pCurrent;
			pCurrent = pCurrent->f_GetLeftP();
		}

		mp_iStack = iStack;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_StartBackward(TCAVLTreeAggregate const *_pTree)
	{
		// Find the smallest item in tree, and build stack
		CLink const *pCurrent = CLink::fs_GetPtr(_pTree->m_Root);
		aint iStack = -1;

		while (pCurrent)
		{
			mp_Stack[++iStack] = pCurrent;
			pCurrent = pCurrent->f_GetRightP();
		}

		mp_iStack = iStack;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_Next()
	{
		if (mp_iStack < 0)
		{
			// We are already done
			return;
		}

		CLink const *pCurrent = mp_Stack[mp_iStack];

		pCurrent = pCurrent->f_GetRightP();
		// Decrease stack so we overwrite the current stack
		--mp_iStack;
		while (pCurrent)
		{
			mp_Stack[++mp_iStack] = pCurrent;
			pCurrent = pCurrent->f_GetLeftP();
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_Prev()
	{
		if (mp_iStack < 0)
		{
			// We are already done

			return;
		}

		CLink const *pCurrent = mp_Stack[mp_iStack];

		pCurrent = pCurrent->f_GetLeftP();
		// Decrease stack so we overwrite the current stack
		--mp_iStack;
		while (pCurrent)
		{
			mp_Stack[++mp_iStack] = pCurrent;
			pCurrent = pCurrent->f_GetRightP();
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_Debug_CheckStack()
	{
		for (smint iEntry = 0; iEntry < mp_iStack; ++iEntry)
		{
			CLink const *pCurrent = mp_Stack[iEntry];
			CLink const *pNext = mp_Stack[iEntry + 1];

			DMibFastCheck(pCurrent->f_GetRightP() == pNext || pCurrent->f_GetLeftP() == pNext);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_NextBidirectional()
	{
		if (mp_iStack < 0)
		{
			// We are already done
			return;
		}

		CLink const *pCurrent = mp_Stack[mp_iStack];

		CLink const *pNext = pCurrent->f_GetRightP();
		if (pNext)
		{
			while (pNext)
			{
				mp_Stack[++mp_iStack] = pNext;
				pNext = pNext->f_GetLeftP();
			}
		}
		else
		{
			--mp_iStack;
			if (mp_iStack < 0)
				return;
			CLink const *pParent = mp_Stack[mp_iStack];
			while (pCurrent == pParent->f_GetRightP())
			{
				pCurrent = pParent;
				if (--mp_iStack >= 0)
					pParent = mp_Stack[mp_iStack];
				else
					break;
			}
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_PrevBidirectional()
	{
		if (mp_iStack < 0)
		{
			// We are already done

			return;
		}

		CLink const *pCurrent = mp_Stack[mp_iStack];

		CLink const *pNext = pCurrent->f_GetLeftP();
		if (pNext)
		{
			while (pNext)
			{
				mp_Stack[++mp_iStack] = pNext;
				pNext = pNext->f_GetRightP();
			}
		}
		else
		{
			--mp_iStack;
			if (mp_iStack < 0)
				return;
			CLink const *pParent = mp_Stack[mp_iStack];
			while (pCurrent == pParent->f_GetLeftP())
			{
				pCurrent = pParent;
				if (--mp_iStack >= 0)
					pParent = mp_Stack[mp_iStack];
				else
					break;
			}
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_InitForSearch(TCAVLTreeAggregate const &_Tree)
	{
		f_InitForSearch(&_Tree);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_InitForSearch(TCAVLTreeAggregate const *_pTree)
	{
		mp_Stack[0] = CLink::fs_GetPtr(_pTree->m_Root);
		mp_iStack = 0;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_CKey>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_FindEqualForward(tf_CKey const &_Key)
	{
		return f_FindEqualForward(_Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_CKey, typename tf_FCompare>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_FindEqualForward(tf_CKey const &_Key, tf_FCompare &&_fCompare)
	{
		aint iStack = mp_iStack;

		CLink const *pCurrentSearch = mp_Stack[iStack];

		while (pCurrentSearch)
		{
			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLinkConst(pCurrentSearch), _Key);
			static_assert(cIsOrderType<decltype(CompareResult)>);
			if (CompareResult < 0)
				pCurrentSearch = pCurrentSearch->f_GetRightP();
			else if (CompareResult > 0)
			{
				mp_Stack[iStack++] = pCurrentSearch;
				pCurrentSearch = pCurrentSearch->f_GetLeftP();
			}
			else
			{
				mp_Stack[iStack] = pCurrentSearch;
				// We found something set stack
				mp_iStack = iStack;
				return true;
			}
		}
		mp_iStack = -1;
		return false;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_CKey>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_FindSmallestGreaterThanEqualForward(tf_CKey const &_Key)
	{
		return f_FindSmallestGreaterThanEqualForward(_Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_CKey, typename tf_FCompare>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_FindSmallestGreaterThanEqualForward
		(
			tf_CKey const &_Key
			, tf_FCompare &&_fCompare
		)
		-> bool
	{
		CLink const *pBestFit = nullptr;

		aint iStack = mp_iStack;

		CLink const *pCurrentSearch = mp_Stack[iStack];

		while (pCurrentSearch)
		{
			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLinkConst(pCurrentSearch), _Key);
			static_assert(cIsOrderType<decltype(CompareResult)>);
			if (CompareResult < 0)
				pCurrentSearch = pCurrentSearch->f_GetRightP();
			else if (CompareResult > 0)
			{
				DMibFastCheck(!pBestFit || (fsp_Compare(_fCompare, *fsp_MemberFromLinkConst(pCurrentSearch), *fsp_MemberFromLinkConst(pBestFit)) < 0)); // Tree is damaged
				pBestFit = pCurrentSearch;
				mp_Stack[iStack++] = pCurrentSearch;
				pCurrentSearch = pCurrentSearch->f_GetLeftP();
			}
			else
			{
				mp_Stack[iStack] = pCurrentSearch;
				mp_iStack = iStack;
				// Equal match
				return true;
			}
		}

		if (pBestFit)
		{
			mp_iStack = iStack - 1;
			return true;
		}
		else
		{
			mp_iStack = -1;
			return false;
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_CKey>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_FindLargestLessThanEqualForward(tf_CKey const &_Key)
	{
		return f_FindLargestLessThanEqualForward(_Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_CKey, typename tf_FCompare>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_FindLargestLessThanEqualForward
		(
			tf_CKey const &_Key
			, tf_FCompare &&_fCompare
		)
		-> bool
	{
		CLink const *pBestFit = nullptr;

		aint iStack = mp_iStack;
		aint iBestFitStack = -1;

		CLink const *pCurrentSearch = mp_Stack[iStack];

		while (pCurrentSearch)
		{
			auto CompareResult = fsp_Compare(_fCompare, _Key, *fsp_MemberFromLinkConst(pCurrentSearch));
			static_assert(cIsOrderType<decltype(CompareResult)>);
			if (CompareResult < 0)
			{
				mp_Stack[iStack++] = pCurrentSearch;
				pCurrentSearch = pCurrentSearch->f_GetLeftP();
			}
			else if (CompareResult > 0)
			{
				DMibFastCheck(!pBestFit || (fsp_Compare(_fCompare, *fsp_MemberFromLinkConst(pBestFit), *fsp_MemberFromLinkConst(pCurrentSearch)) < 0)); // Tree is damaged
				pBestFit = pCurrentSearch;
				iBestFitStack = iStack;
				pCurrentSearch = pCurrentSearch->f_GetRightP();
			}
			else
			{
				mp_Stack[iStack] = pCurrentSearch;
				mp_iStack = iStack;
				// Equal match
				return true;
			}
		}

		if (pBestFit)
		{
			mp_Stack[iBestFitStack] = pBestFit;
			mp_iStack = iBestFitStack;
			return true;
		}
		else
		{
			mp_iStack = -1;
			return false;
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_StartBackward(TCAVLTreeAggregate const &_Tree)
	{
		f_StartBackward(&_Tree);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_FCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_Delete(TCAVLTreeAggregate &_Tree, tf_FCompare &&_fCompare)
	{
		CNode *pToDelete = f_GetCurrent();
		f_Next();
		CNode *pToFind = f_GetCurrent();
		_Tree.f_Remove(pToDelete, _fCompare);
		fg_DeleteObject(CAllocator(), pToDelete);
		if (pToFind)
		{
			f_InitForSearch(&_Tree);
			f_FindEqualForward(*pToFind, _fCompare);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_Delete(TCAVLTreeAggregate &_Tree)
	{
		f_Delete(_Tree, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_FCompare, typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_DeleteAllocator
		(
			TCAVLTreeAggregate &_Tree
			, tf_FCompare &&_fCompare
			, tf_CAllocator &_Allocator
		)
	{
		CNode *pToDelete = f_GetCurrent();
		f_Next();
		CNode *pToFind = f_GetCurrent();
		_Tree.f_Remove(pToDelete, _fCompare);
		fg_DeleteObject(_Allocator, pToDelete);
		if (pToFind)
		{
			f_InitForSearch(&_Tree);
			f_FindEqualForward(*pToFind, _fCompare);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_DeleteAllocator
		(
			TCAVLTreeAggregate &_Tree
			, tf_CAllocator &_Allocator
		)
	{
		f_DeleteAllocator(_Tree, t_CCompare(), _Allocator);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_FCompare, typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_DeleteAllocatorDefiniteType
		(
			TCAVLTreeAggregate &_Tree
			, tf_FCompare &&_fCompare
			, tf_CAllocator &_Allocator
		)
	{
		CNode *pToDelete = f_GetCurrent();
		f_Next();
		CNode *pToFind = f_GetCurrent();
		_Tree.f_Remove(pToDelete, _fCompare);
		fg_DeleteObjectDefiniteType(_Allocator, pToDelete);
		if (pToFind)
		{
			f_InitForSearch(&_Tree);
			f_FindEqualForward(*pToFind, _fCompare);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_DeleteAllocatorDefiniteType
		(
			TCAVLTreeAggregate &_Tree
			, tf_CAllocator &_Allocator
		)
	{
		f_DeleteAllocatorDefiniteType(_Tree, t_CCompare(), _Allocator);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <typename tf_FCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_Remove(TCAVLTreeAggregate &_Tree, tf_FCompare &&_fCompare)
	{
		CNode *pToDelete = f_GetCurrent();
		f_Next();
		CNode *pToFind = f_GetCurrent();
		_Tree.f_Remove(pToDelete, _fCompare);
		if (pToFind)
		{
			f_InitForSearch(&_Tree);
			f_FindEqualForward(*pToFind, _fCompare);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_Remove(TCAVLTreeAggregate &_Tree)
	{
		f_Remove(_Tree, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	template <bool tf_bReverse>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_ExtractCurrentNoRebalance(TCAVLTreeAggregate &_Tree)
		-> typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	{
		DMibFastCheck(mp_iStack >= 0);

		// Get current node to extract and compute parent link pointer
		CLink *pToExtract = const_cast<CLink *>(mp_Stack[mp_iStack]);

		CLinkPointer *pParentLink;
		if (mp_iStack == 0)
			pParentLink = &_Tree.m_Root;
		else
		{
			CLink *pParent = const_cast<CLink *>(mp_Stack[mp_iStack - 1]);
			if (pParent->f_GetLeftP() == pToExtract)
				pParentLink = pParent->f_GetLeft();
			else
				pParentLink = pParent->f_GetRight();
		}

		// Save position before advancing
		aint iExtractedPos = mp_iStack;

		// Advance to next element using bidirectional algorithm (maintains full ancestor path)
		if constexpr (tf_bReverse)
			f_PrevBidirectional();
		else
			f_NextBidirectional();

		// Do the removal, get the replacement
		CLink *pReplacement = fsp_RemovedNoRebalance(pParentLink, pToExtract);

		// Patch stack if extracted node is still in it (we went down into its subtree)
		if (iExtractedPos <= mp_iStack)
		{
			if (pReplacement)
			{
				// Check if replacement is also in stack (between iExtractedPos+1 and mp_iStack)
				// This happens when successor replaces the extracted node and successor was an ancestor of current
				aint iReplacementPos = -1;
				for (aint iPos = iExtractedPos + 1; iPos <= mp_iStack; ++iPos)
				{
					if (mp_Stack[iPos] == pReplacement)
					{
						iReplacementPos = iPos;
						break;
					}
				}

				if (iReplacementPos >= 0)
				{
					// Collapse: replacement moved from iReplacementPos to iExtractedPos
					// Remove entries from iExtractedPos+1 to iReplacementPos (the path between them is now gone)
					aint nToRemove = iReplacementPos - iExtractedPos;
					mp_Stack[iExtractedPos] = pReplacement;
					for (aint iPos = iExtractedPos + 1; iPos <= mp_iStack - nToRemove; ++iPos)
						mp_Stack[iPos] = mp_Stack[iPos + nToRemove];
					mp_iStack -= nToRemove;
				}
				else
				{
					// Replacement was not in stack - just update the entry
					mp_Stack[iExtractedPos] = pReplacement;
				}
			}
			else
			{
				// Leaf node removed - shift stack entries down
				for (aint iPos = iExtractedPos; iPos < mp_iStack; ++iPos)
					mp_Stack[iPos] = mp_Stack[iPos + 1];
				--mp_iStack;
			}
		}
		// Else: we went up, extracted node was popped from stack, nothing to patch

		return fsp_MemberFromLink(pToExtract);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::f_GetCurrent() const
		-> typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	{
		if (mp_iStack >= 0)
			return fsp_MemberFromLinkConst(mp_Stack[mp_iStack]);
		else
			return nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::operator CNode * () const
	{
		return f_GetCurrent();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::operator -> () const
		-> typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	{
		return f_GetCurrent();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::operator ++ ()
	{
		f_Next();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint t_RecursionDepth>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCIterator<t_RecursionDepth>::operator -- ()
	{
		f_Prev();
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Aggregate public																					|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_GetIterator()
		-> typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CIterator
	{
		return *this;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_GetIterator() const
		-> typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CIteratorConst
	{
		return *this;
	}
}
