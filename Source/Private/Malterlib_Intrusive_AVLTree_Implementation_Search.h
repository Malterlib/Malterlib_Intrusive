// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib
{
	namespace NIntrusive
	{
		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Public																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CKey>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindEqual(const tf_CKey &_Key) const
		{
			return f_FindEqual(_Key, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CKey, typename tf_CCompare>
		inline_medium typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindEqual(const tf_CKey &_Key, tf_CCompare &&_Compare) const
		{
			CLink* pCurrentSearch = CLink::fs_GetPtr(m_Root);
			while (pCurrentSearch)
			{
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pCurrentSearch), _Key))
					pCurrentSearch = pCurrentSearch->f_GetRightP();
				else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), _Key, *fsp_MemberFromLink(pCurrentSearch)))
					pCurrentSearch = pCurrentSearch->f_GetLeftP();
				else
					return fsp_MemberFromLink(pCurrentSearch);
			}
				
			return nullptr;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CKey>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindSmallestGreaterThanEqual(const tf_CKey &_Key) const
		{
			return f_FindSmallestGreaterThanEqual(_Key, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CKey, typename tf_CCompare>
		inline_large typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindSmallestGreaterThanEqual(const tf_CKey &_Key, tf_CCompare &&_Compare) const
		{
			CLink *pBestFit = nullptr;

			CLink* pCurrentSearch = CLink::fs_GetPtr(m_Root);

			while (pCurrentSearch)
			{
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pCurrentSearch), _Key))
					pCurrentSearch = pCurrentSearch->f_GetNextP(1);
				else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), _Key, *fsp_MemberFromLink(pCurrentSearch)))
				{
					DMibFastCheck(!pBestFit || (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pCurrentSearch), *fsp_MemberFromLink(pBestFit)))); // Tree is damaged
					pBestFit = pCurrentSearch;
					pCurrentSearch = pCurrentSearch->f_GetNextP(0);
				}
				else
					return fsp_MemberFromLink(pCurrentSearch);
			}

			return pBestFit ? fsp_MemberFromLink(pBestFit) : nullptr;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CKey>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindSmallestGreaterThanEqualAndPrev(const tf_CKey &_Key, CNode* &_pPrev) const
		{
			return f_FindSmallestGreaterThanEqualAndPrev(_Key, _pPrev, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CKey, typename tf_CCompare>
		inline_large typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindSmallestGreaterThanEqualAndPrev(const tf_CKey &_Key, CNode* &_pPrev, tf_CCompare &&_Compare) const
		{
			CLink *pBestFit = nullptr;
			CLink *pPrev = nullptr;

			CLink* pCurrentSearch = CLink::fs_GetPtr(m_Root);

			while (pCurrentSearch)
			{
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), _Key, *fsp_MemberFromLink(pCurrentSearch)))
				{
					DMibFastCheck(!pBestFit || (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pCurrentSearch), *fsp_MemberFromLink(pBestFit)))); // Tree is damaged
					pBestFit = pCurrentSearch;
					pCurrentSearch = pCurrentSearch->f_GetNextP(0);
				}
				else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pCurrentSearch), _Key))
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

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CKey>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindLargestLessThanEqual(const tf_CKey &_Key) const
		{
			return f_FindLargestLessThanEqual(_Key, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CKey, typename tf_CCompare>
		inline_large typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindLargestLessThanEqual(const tf_CKey &_Key, tf_CCompare &&_Compare) const
		{
			CLink *pBestFit = nullptr;

			CLink* pCurrentSearch = CLink::fs_GetPtr(m_Root);

			while (pCurrentSearch)
			{
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pCurrentSearch), _Key)) 
				{
					DMibFastCheck(!pBestFit || (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pBestFit), *fsp_MemberFromLink(pCurrentSearch)))); // Tree is damaged
					pBestFit = pCurrentSearch;
					pCurrentSearch = pCurrentSearch->f_GetNextP(1);
				}
				else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), _Key, *fsp_MemberFromLink(pCurrentSearch)))
				{
					pCurrentSearch = pCurrentSearch->f_GetNextP(0);
				}
				else
					return fsp_MemberFromLink(pCurrentSearch);
			}

			return pBestFit ? fsp_MemberFromLink(pBestFit) : nullptr;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindSmallest() const
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

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_FindLargest() const
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
}

