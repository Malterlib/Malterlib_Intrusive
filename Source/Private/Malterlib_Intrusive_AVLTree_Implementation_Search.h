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
	template <typename tf_CKey>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindEqual(const tf_CKey &_Key) const
	{
		return f_FindEqual(_Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey, typename tf_CCompare>
	inline_medium typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindEqual(const tf_CKey &_Key, tf_CCompare &&_fCompare) const
	{
		CLink* pCurrentSearch = CLink::fs_GetPtr(m_Root);

		while (pCurrentSearch)
		{
			auto Compare = fsp_Compare(_fCompare, *fsp_MemberFromLink(pCurrentSearch), _Key);
			static_assert(cIsOrderType<decltype(Compare)>);
#if !defined(DCompiler_MSVC)
			if constexpr (NTraits::cIsSame<decltype(Compare), COrdering_Strong>)
			{
				if (Compare == 0)// [[unlikely]]
					return fsp_MemberFromLink(pCurrentSearch);
				else
					pCurrentSearch = pCurrentSearch->f_GetNextP((Compare > 0 ? 0 : 1));
			}
			else
#endif
			{
				if (Compare < 0)
					pCurrentSearch = pCurrentSearch->f_GetNextP(1);
				else if (Compare > 0)
					pCurrentSearch = pCurrentSearch->f_GetNextP(0);
				else
					return fsp_MemberFromLink(pCurrentSearch);
			}
		}

		return nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindSmallestGreaterThanEqual(const tf_CKey &_Key) const
	{
		return f_FindSmallestGreaterThanEqual(_Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey, typename tf_CCompare>
	inline_large typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindSmallestGreaterThanEqual(const tf_CKey &_Key, tf_CCompare &&_fCompare) const
	{
		CLink *pBestFit = nullptr;

		CLink* pCurrentSearch = CLink::fs_GetPtr(m_Root);

		while (pCurrentSearch)
		{
			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(pCurrentSearch), _Key);
			static_assert(cIsOrderType<decltype(CompareResult)>);
#if !defined(DCompiler_MSVC)
			if constexpr (NTraits::cIsSame<decltype(CompareResult), COrdering_Strong>)
			{
				if (CompareResult == 0)// [[unlikely]]
					return fsp_MemberFromLink(pCurrentSearch);
				else if (CompareResult < 0)
					pCurrentSearch = pCurrentSearch->f_GetNextP(1);
				else
				{
					DMibFastCheck(!pBestFit || (fsp_Compare(_fCompare, *fsp_MemberFromLink(pCurrentSearch), *fsp_MemberFromLink(pBestFit)) < 0)); // Tree is damaged
					pBestFit = pCurrentSearch;
					pCurrentSearch = pCurrentSearch->f_GetNextP(0);
				}
			}
			else
#endif
			{
				if (CompareResult < 0)
					pCurrentSearch = pCurrentSearch->f_GetNextP(1);
				else if (CompareResult > 0)
				{
					DMibFastCheck(!pBestFit || (fsp_Compare(_fCompare, *fsp_MemberFromLink(pCurrentSearch), *fsp_MemberFromLink(pBestFit)) < 0)); // Tree is damaged
					pBestFit = pCurrentSearch;
					pCurrentSearch = pCurrentSearch->f_GetNextP(0);
				}
				else
					return fsp_MemberFromLink(pCurrentSearch);
			}
		}

		return pBestFit ? fsp_MemberFromLink(pBestFit) : nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindSmallestGreaterThanEqualAndPrev(const tf_CKey &_Key, CNode* &_pPrev) const
	{
		return f_FindSmallestGreaterThanEqualAndPrev(_Key, _pPrev, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey, typename tf_CCompare>
	inline_large typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindSmallestGreaterThanEqualAndPrev
		(
			const tf_CKey &_Key
			, CNode* &_pPrev
			, tf_CCompare &&_fCompare
		) const
	{
		CLink *pBestFit = nullptr;
		CLink *pPrev = nullptr;

		CLink* pCurrentSearch = CLink::fs_GetPtr(m_Root);

		while (pCurrentSearch)
		{
			auto CompareResult = fsp_Compare(_fCompare, _Key, *fsp_MemberFromLink(pCurrentSearch));
			static_assert(cIsOrderType<decltype(CompareResult)>);
			if (CompareResult < 0)
			{
				DMibFastCheck(!pBestFit || (fsp_Compare(_fCompare, *fsp_MemberFromLink(pCurrentSearch), *fsp_MemberFromLink(pBestFit)) < 0)); // Tree is damaged
				pBestFit = pCurrentSearch;
				pCurrentSearch = pCurrentSearch->f_GetNextP(0);
			}
			else if (CompareResult > 0)
			{
				pPrev = pCurrentSearch;
				pCurrentSearch = pCurrentSearch->f_GetNextP(1);
			}
			else
			{
				CLink *pLeft = pCurrentSearch->f_GetLeftP();
				if (pLeft)
				{
					CLink *pRight = pLeft->f_GetRightP();
					while (pRight)
					{
						pLeft = pRight;
						pRight = pLeft->f_GetRightP();
					}
					_pPrev = fsp_MemberFromLink(pLeft);
				}
				else if (pPrev)
					_pPrev = fsp_MemberFromLink(pPrev);
				return fsp_MemberFromLink(pCurrentSearch);
			}
		}

		if (pPrev)
			_pPrev = fsp_MemberFromLink(pPrev);
		return pBestFit ? fsp_MemberFromLink(pBestFit) : nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindLargestLessThanEqual(const tf_CKey &_Key) const
	{
		return f_FindLargestLessThanEqual(_Key, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CKey, typename tf_CCompare>
	inline_large typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindLargestLessThanEqual(const tf_CKey &_Key, tf_CCompare &&_fCompare) const
	{
		CLink *pBestFit = nullptr;

		CLink* pCurrentSearch = CLink::fs_GetPtr(m_Root);

		while (pCurrentSearch)
		{
			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(pCurrentSearch), _Key);
			static_assert(cIsOrderType<decltype(CompareResult)>);
#if !defined(DCompiler_MSVC)
			if constexpr (NTraits::cIsSame<decltype(CompareResult), COrdering_Strong>)
			{
				if (CompareResult == 0)// [[unlikely]]
					return fsp_MemberFromLink(pCurrentSearch);
				else if (CompareResult < 0)
				{
					DMibFastCheck(!pBestFit || (fsp_Compare(_fCompare, *fsp_MemberFromLink(pBestFit), *fsp_MemberFromLink(pCurrentSearch)) < 0)); // Tree is damaged
					pBestFit = pCurrentSearch;
					pCurrentSearch = pCurrentSearch->f_GetNextP(1);
				}
				else
					pCurrentSearch = pCurrentSearch->f_GetNextP(0);
			}
			else
#endif
			{
				if (CompareResult < 0)
				{
					DMibFastCheck(!pBestFit || (fsp_Compare(_fCompare, *fsp_MemberFromLink(pBestFit), *fsp_MemberFromLink(pCurrentSearch)) < 0)); // Tree is damaged
					pBestFit = pCurrentSearch;
					pCurrentSearch = pCurrentSearch->f_GetNextP(1);
				}
				else if (CompareResult > 0)
					pCurrentSearch = pCurrentSearch->f_GetNextP(0);
				else
					return fsp_MemberFromLink(pCurrentSearch);
			}
		}

		return pBestFit ? fsp_MemberFromLink(pBestFit) : nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindSmallest() const
	{
		CLink *pBestFit = CLink::fs_GetPtr(m_Root);

		if (pBestFit)
		{
			while (1)
			{
				CLink *pNext = pBestFit->f_GetLeftP();

				if (!pNext)
					return fsp_MemberFromLink(pBestFit);

				pBestFit = pNext;
			}
		}

		return nullptr;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode *TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_FindLargest() const
	{
		CLink *pBestFit = CLink::fs_GetPtr(m_Root);

		if (pBestFit)
		{
			while (1)
			{
				CLink *pNext = pBestFit->f_GetRightP();

				if (!pNext)
					return fsp_MemberFromLink(pBestFit);

				pBestFit = pNext;
			}
		}

		return nullptr;
	}
}
