// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NIntrusive
{
	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Public																							|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::TIterator()
	{
		// Reset iterator
		m_iStack = -1;
#ifdef DMibDebuggerHelpers
		static_assert(TCInstantiateValue<&fs_Debug_GetTree>::mc_Value);
#endif
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_medium TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::TIterator(const TIterator &_Tree)
	{
		m_iStack = _Tree.m_iStack;
#ifdef DMibDebuggerHelpers
		static_assert(TCInstantiateValue<&fs_Debug_GetTree>::mc_Value);
#endif

		for (aint i = 0; i <= m_iStack; ++i)
			m_pStack[i] = _Tree.m_pStack[i];
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::TIterator(const TCAVLTreeAggregate &_Tree)
	{
		f_StartForward(&_Tree);
#ifdef DMibDebuggerHelpers
		static_assert(TCInstantiateValue<&fs_Debug_GetTree>::mc_Value);
#endif
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::TIterator(const TCAVLTreeAggregate *_pTree)
	{
		f_StartForward(_pTree);
#ifdef DMibDebuggerHelpers
		static_assert(TCInstantiateValue<&fs_Debug_GetTree>::mc_Value);
#endif
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_medium mint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_GetLen() const
	{
		TIterator Iter(*this);
		mint Len = 0;
		while (Iter)
		{
			++Len;
			++Iter;
		}
		return Len;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_Clear()
	{
		m_iStack = -1;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_StartForward(const TCAVLTreeAggregate &_Tree)
	{
		f_StartForward(&_Tree);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_StartForward(const TCAVLTreeAggregate *_pTree)
	{
		// Find the smallest item in tree, and build stack
		const CLink *pCurrent = CLink::fs_GetPtr(_pTree->m_Root);
		aint iStack = -1;

		while (pCurrent)
		{
			m_pStack[++iStack] = pCurrent;
			pCurrent = pCurrent->f_GetLeftP();
		}

		m_iStack = iStack;
	}


	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_StartBackward(const TCAVLTreeAggregate *_pTree)
	{
		// Find the smallest item in tree, and build stack
		const CLink *pCurrent = CLink::fs_GetPtr(_pTree->m_Root);
		aint iStack = -1;

		while (pCurrent)
		{
			m_pStack[++iStack] = pCurrent;
			pCurrent = pCurrent->f_GetRightP();
		}

		m_iStack = iStack;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_Next()
	{
		if (m_iStack < 0)
		{
			// We are already done
			return;
		}

		const CLink *pCurrent = m_pStack[m_iStack];

		pCurrent = pCurrent->f_GetRightP();
		// Decrease stack so we overwrite the current stack
		--m_iStack;
		while (pCurrent)
		{
			m_pStack[++m_iStack] = pCurrent;
			pCurrent = pCurrent->f_GetLeftP();
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_Prev()
	{
		if (m_iStack < 0)
		{
			// We are already done

			return;
		}

		const CLink *pCurrent = m_pStack[m_iStack];

		pCurrent = pCurrent->f_GetLeftP();
		// Decrease stack so we overwrite the current stack
		--m_iStack;
		while (pCurrent)
		{
			m_pStack[++m_iStack] = pCurrent;
			pCurrent = pCurrent->f_GetRightP();
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_Debug_CheckStack()
	{
		for (smint i = 0; i < m_iStack; ++i)
		{
			const CLink *pCurrent = m_pStack[i];
			const CLink *pNext = m_pStack[i+1];

			DMibFastCheck(pCurrent->f_GetRightP() == pNext || pCurrent->f_GetLeftP() == pNext);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_NextBidirectional()
	{
		if (m_iStack < 0)
		{
			// We are already done
			return;
		}

		const CLink *pCurrent = m_pStack[m_iStack];

		const CLink *pNext = pCurrent->f_GetRightP();
		if (pNext)
		{
			while (pNext)
			{
				m_pStack[++m_iStack] = pNext;
				pNext = pNext->f_GetLeftP();
			}
		}
		else
		{
			--m_iStack;
			if (m_iStack < 0)
				return;
			const CLink *pParent = m_pStack[m_iStack];
			while (pCurrent == pParent->f_GetRightP())
			{
				pCurrent = pParent;
				if (--m_iStack >= 0)
					pParent = m_pStack[m_iStack];
				else
					break;
			}
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_PrevBidirectional()
	{
		if (m_iStack < 0)
		{
			// We are already done

			return;
		}

		const CLink *pCurrent = m_pStack[m_iStack];

		const CLink *pNext = pCurrent->f_GetLeftP();
		if (pNext)
		{
			while (pNext)
			{
				m_pStack[++m_iStack] = pNext;
				pNext = pNext->f_GetRightP();
			}
		}
		else
		{
			--m_iStack;
			if (m_iStack < 0)
				return;
			const CLink *pParent = m_pStack[m_iStack];
			while (pCurrent == pParent->f_GetLeftP())
			{
				pCurrent = pParent;
				if (--m_iStack >= 0)
					pParent = m_pStack[m_iStack];
				else
					break;
			}
		}
	}


	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_InitForSearch(const TCAVLTreeAggregate &_Tree)
	{
		f_InitForSearch(&_Tree);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_InitForSearch(const TCAVLTreeAggregate *_pTree)
	{
		m_pStack[0] = CLink::fs_GetPtr(_pTree->m_Root);
		m_iStack = 0;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CKey>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_FindEqualForward(const tf_CKey &_Key)
	{
		return f_FindEqualForward(_Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CKey, typename tf_CCompare>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_FindEqualForward(const tf_CKey &_Key, tf_CCompare &&_Compare)
	{
		aint iStack = m_iStack;

		const CLink* pCurrentSearch = m_pStack[iStack];

		while (pCurrentSearch)
		{
			if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLinkConst(pCurrentSearch), _Key))
			{
				pCurrentSearch = pCurrentSearch->f_GetRightP();
			}
			else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), _Key, *fsp_MemberFromLinkConst(pCurrentSearch)))
			{
				m_pStack[iStack++] = pCurrentSearch;
				pCurrentSearch = pCurrentSearch->f_GetLeftP();
			}
			else
			{
				m_pStack[iStack] = pCurrentSearch;
				// We found something set stack
				m_iStack = iStack;
				return true;
			}
		}
		m_iStack = -1;
		return false;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CKey>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_FindSmallestGreaterThanEqualForward(const tf_CKey &_Key)
	{
		return f_FindSmallestGreaterThanEqualForward(_Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CKey, typename tf_CCompare>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_FindSmallestGreaterThanEqualForward(const tf_CKey &_Key, tf_CCompare &&_Compare)
	{
		const CLink* pBestFit = nullptr;

		aint iStack = m_iStack;

		const CLink* pCurrentSearch = m_pStack[iStack];

		while (pCurrentSearch)
		{
			if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLinkConst(pCurrentSearch), _Key))
			{
				pCurrentSearch = pCurrentSearch->f_GetRightP();
			}
			else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), _Key, *fsp_MemberFromLinkConst(pCurrentSearch)))
			{
				DMibFastCheck(!pBestFit || (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLinkConst(pCurrentSearch), *fsp_MemberFromLinkConst(pBestFit)))); // Tree is damaged
				pBestFit = pCurrentSearch;
				m_pStack[iStack++] = pCurrentSearch;
				pCurrentSearch = pCurrentSearch->f_GetLeftP();
			}
			else
			{
				m_pStack[iStack] = pCurrentSearch;
				m_iStack = iStack;
				// Equal match
				return true;
			}
		}

		if (pBestFit)
		{
			m_iStack = iStack-1;
			return true;
		}
		else
		{
			m_iStack = -1;
			return false;
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CKey>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_FindLargestLessThanEqualForward(const tf_CKey &_Key)
	{
		return f_FindLargestLessThanEqualForward(_Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CKey, typename tf_CCompare>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_FindLargestLessThanEqualForward(const tf_CKey &_Key, tf_CCompare &&_Compare)
	{
		const CLink *pBestFit = nullptr;

		aint iStack = m_iStack;
		aint iBestFitStack = -1;

		const CLink* pCurrentSearch = m_pStack[iStack];

		while (pCurrentSearch)
		{
			if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), _Key, *fsp_MemberFromLinkConst(pCurrentSearch)))
			{
				m_pStack[iStack++] = pCurrentSearch;
				pCurrentSearch = pCurrentSearch->f_GetLeftP();
			}
			else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLinkConst(pCurrentSearch), _Key))
			{
				DMibFastCheck(!pBestFit || (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLinkConst(pBestFit), *fsp_MemberFromLinkConst(pCurrentSearch)))); // Tree is damaged
				pBestFit = pCurrentSearch;
				iBestFitStack = iStack;
				pCurrentSearch = pCurrentSearch->f_GetRightP();
			}
			else
			{
				m_pStack[iStack] = pCurrentSearch;
				m_iStack = iStack;
				// Equal match
				return true;
			}
		}

		if (pBestFit)
		{
			m_pStack[iBestFitStack] = pBestFit;
			m_iStack = iBestFitStack;
			return true;
		}
		else
		{
			m_iStack = -1;
			return false;
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_StartBackward(const TCAVLTreeAggregate &_Tree)
	{
		f_StartBackward(&_Tree);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_Delete(TCAVLTreeAggregate &_Tree, tf_CCompare &&_Compare)
	{
		CNode *pToDelete = f_GetCurrent();
		f_Next();
		CNode *pToFind = f_GetCurrent();
		_Tree.f_Remove(pToDelete, fg_Forward<tf_CCompare>(_Compare));
		fg_DeleteObject(CAllocator(), pToDelete);
		if (pToFind)
		{
			f_InitForSearch(&_Tree);
			f_FindEqualForward(*pToFind);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_Delete(TCAVLTreeAggregate &_Tree)
	{
		f_Delete(_Tree, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CCompare, typename tf_CAllocator>
	void
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>
	::TIterator<_RecursionDepth>
	::f_DeleteAllocator(TCAVLTreeAggregate &_Tree, tf_CCompare &&_Compare, tf_CAllocator &_Allocator)
	{
		CNode *pToDelete = f_GetCurrent();
		f_Next();
		CNode *pToFind = f_GetCurrent();
		_Tree.f_Remove(pToDelete, fg_Forward<tf_CCompare>(_Compare));
		fg_DeleteObject(_Allocator, pToDelete);
		if (pToFind)
		{
			f_InitForSearch(&_Tree);
			f_FindEqualForward(*pToFind);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_DeleteAllocator(TCAVLTreeAggregate &_Tree, tf_CAllocator &_Allocator)
	{
		f_DeleteAllocator(_Tree, t_CCompare(), _Allocator);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CCompare, typename tf_CAllocator>
	void
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>
	::TIterator<_RecursionDepth>
	::f_DeleteAllocatorDefiniteType(TCAVLTreeAggregate &_Tree, tf_CCompare &&_Compare, tf_CAllocator &_Allocator)
	{
		CNode *pToDelete = f_GetCurrent();
		f_Next();
		CNode *pToFind = f_GetCurrent();
		_Tree.f_Remove(pToDelete, fg_Forward<tf_CCompare>(_Compare));
		fg_DeleteObjectDefiniteType(_Allocator, pToDelete);
		if (pToFind)
		{
			f_InitForSearch(&_Tree);
			f_FindEqualForward(*pToFind);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_DeleteAllocatorDefiniteType(TCAVLTreeAggregate &_Tree, tf_CAllocator &_Allocator)
	{
		f_DeleteAllocatorDefiniteType(_Tree, t_CCompare(), _Allocator);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_Remove(TCAVLTreeAggregate &_Tree, tf_CCompare &&_Compare)
	{
		CNode *pToDelete = f_GetCurrent();
		f_Next();
		CNode *pToFind = f_GetCurrent();
		_Tree.f_Remove(pToDelete, fg_Forward<tf_CCompare>(_Compare));
		if (pToFind)
		{
			f_InitForSearch(&_Tree);
			f_FindEqualForward(*pToFind);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_Remove(TCAVLTreeAggregate &_Tree)
	{
		f_Remove(_Tree, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::f_GetCurrent() const
	{
		if (m_iStack >= 0)
			return fsp_MemberFromLinkConst(m_pStack[m_iStack]);
		else
			return nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::operator CNode *() const
	{
		return f_GetCurrent();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::operator ->() const
	{
		return f_GetCurrent();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::operator ++()
	{
		f_Next();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <aint _RecursionDepth>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TIterator<_RecursionDepth>::operator --()
	{
		f_Prev();
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Aggregate public																					|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/


	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CIterator TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_GetIterator()
	{
		return CIterator(*this);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CIteratorConst TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_GetIterator() const
	{
		return CIteratorConst(*this);
	}
}
