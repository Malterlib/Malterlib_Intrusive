// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Mib/Core/Core>

namespace NMib::NIntrusive
{
	/************************************************************************************************\
	||¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯||
	|| Single linked list
	||______________________________________________________________________________________________||
	\************************************************************************************************/

	// Had to make this one a template so the functions won't get compiled at header time
	// 	template <aint t_Dummy = 0>

	struct CSLinkAggr
	{
		constexpr CSLinkAggr(EAggregateInitialization _Init)
			: m_pNext(nullptr)
		{
		}

		inline_always CSLinkAggr()
		{
		}

		CSLinkAggr *m_pNext;

		inline_small void f_Construct()
		{
#ifdef DMibEnableSafeCheck
			m_pNext = nullptr;
#endif
		}

		inline_small void f_Destruct()
		{
			DMibSafeCheck(!m_pNext, "You have to take care of it. No auto delete");
		}

		inline_small void f_Link(CSLinkAggr *_pLinkAfter)
		{
			DMibSafeCheck(!m_pNext, "Singly linked list cannot be automatically unlinked, you have to remove it from the list manually");
			m_pNext = _pLinkAfter->m_pNext;
			_pLinkAfter->m_pNext = this;
		}

		inline_small void f_Unlink(CSLinkAggr *_pPrev)
		{
			DMibSafeCheck(_pPrev, "You better know what you are doing");
			_pPrev->m_pNext = m_pNext;
			m_pNext = nullptr;
		}

		inline_small void f_UnsafeLink(CSLinkAggr *_pLinkAfter)
		{
			m_pNext = _pLinkAfter->m_pNext;
			_pLinkAfter->m_pNext = this;
		}

		inline_small void f_UnsafeUnlink(CSLinkAggr *_pPrev)
		{
			_pPrev->m_pNext = m_pNext;
#ifdef DMibEnableSafeCheck
			m_pNext = nullptr;
#endif
		}

		inline_small CSLinkAggr *f_GetNext() const
		{
			return m_pNext;
		}

		inline_small void f_SetNext(CSLinkAggr *_pNext)
		{
			m_pNext = _pNext;
		}

		inline_small bool f_IsInList() const
		{
			return m_pNext != nullptr;
		}

	private:
		using CThis = CSLinkAggr;
	};

	struct CSLink : public CSLinkAggr
	{
		inline_small CSLink()
		{
			CSLinkAggr::f_Construct();
		}

		inline_small ~CSLink()
		{
			CSLinkAggr::f_Destruct();
		}

	private:
		CSLink(CSLink const &) = delete;
		CSLink &operator = (CSLink const &) = delete;
	};

	struct CSLinkListData_Last
	{
		constexpr CSLinkListData_Last(EAggregateInitialization _Init)
			: m_First(_Init)
			, m_pLast(nullptr)
		{
		}

		inline_always CSLinkListData_Last()
		{
		}

		CSLinkAggr m_First;
		CSLinkAggr *m_pLast;
		inline_small CSLinkAggr &f_GetFirst()
		{
			return m_First;
		}

		inline_small CSLinkAggr const &f_GetFirst() const
		{
			return m_First;
		}

		inline_small CSLinkAggr *f_GetLast()
		{
			return m_pLast;
		}

		inline_small CSLinkAggr const *f_GetLast() const
		{
			return m_pLast;
		}

		inline_small void f_SetLast(CSLinkAggr *_pLast)
		{
			m_pLast = _pLast;
		}

		static inline_small bool fs_HasLast()
		{
			return true;
		}
	};

	struct CSLinkListData
	{
		constexpr CSLinkListData(EAggregateInitialization _Init)
			: m_First(_Init)
		{
		}

		inline_always CSLinkListData()
		{
		}

		CSLinkAggr m_First;
		inline_small CSLinkAggr &f_GetFirst()
		{
			return m_First;
		}

		inline_small CSLinkAggr *f_GetLast()
		{
			return nullptr;
		}

		inline_small CSLinkAggr const &f_GetFirst() const
		{
			return m_First;
		}

		inline_small CSLinkAggr const *f_GetLast() const
		{
			return nullptr;
		}

		inline_small void f_SetLast(CSLinkAggr *_pLast)
		{
		}

		static inline_small bool fs_HasLast()
		{
			return false;
		}
	};

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	struct TCSLinkListAggregate
	{
	private:
		using COffset = typename t_CTranslator::template TCOffset<t_CData>;

	public:
		constexpr TCSLinkListAggregate(EAggregateInitialization _Init)
			: m_Data(_Init)
		{
		}

		inline_always TCSLinkListAggregate() = default;

		t_CListData m_Data;

		void f_Construct()
		{
			m_Data.f_GetFirst().f_SetNext(nullptr);
			if (m_Data.fs_HasLast())
				m_Data.f_SetLast(&m_Data.f_GetFirst());
		}

		void f_Destruct()
		{
			if constexpr (t_bAutoDelete)
				fp_AutoDelete<t_bAutoDelete>();
			else
				f_Clear();
		}

		void f_ClearFast() // Fast clear that does not set the linked members to the unlinked state.
		{
			m_Data.f_GetFirst().f_SetNext(nullptr);
			if (m_Data.fs_HasLast())
				m_Data.f_SetLast(nullptr);
		}

		void f_Clear()
		{
			while (m_Data.f_GetFirst().f_GetNext())
				m_Data.f_GetFirst().f_GetNext()->f_Unlink(&m_Data.f_GetFirst());
			if (m_Data.fs_HasLast())
				m_Data.f_SetLast(&m_Data.f_GetFirst());
		}

		void f_DeleteAll()
		{
			while (m_Data.f_GetFirst().f_GetNext())
			{
				t_CData *pToDelete = fsp_MemberFromLink(m_Data.f_GetFirst().f_GetNext());
				m_Data.f_GetFirst().f_GetNext()->f_Unlink(&m_Data.f_GetFirst());
				fg_DeleteObject(t_CAllocator(), pToDelete);
			}

			if (m_Data.fs_HasLast())
				m_Data.f_SetLast(&m_Data.f_GetFirst());
		}

		void f_DeleteAllDefiniteType()
		{
			while (m_Data.f_GetFirst().f_GetNext())
			{
				t_CData *pToDelete = fsp_MemberFromLink(m_Data.f_GetFirst().f_GetNext());
				m_Data.f_GetFirst().f_GetNext()->f_Unlink(&m_Data.f_GetFirst());
				fg_DeleteObjectDefiniteType(t_CAllocator(), pToDelete);
			}

			if (m_Data.fs_HasLast())
				m_Data.f_SetLast(&m_Data.f_GetFirst());
		}

		void f_Remove(t_CData *_pToRemove)
		{
			CSLinkAggr *pToRemove = fsp_LinkFromMember(_pToRemove);
			CSLinkAggr *pCurrent = &m_Data.f_GetFirst();
			while (pCurrent)
			{
				CSLinkAggr *pNext = pCurrent->f_GetNext();
				if (pNext == pToRemove)
				{
					if (m_Data.fs_HasLast())
					{
						if (!pToRemove->f_GetNext())
							m_Data.f_SetLast(pCurrent);
					}
					pNext->f_Unlink(pCurrent);
					break;
				}
				pCurrent = pNext;
			}
		}

		static void fs_UnsafeUnlink(t_CData *_pToRemove, t_CData *_pPrev)
		{
			CSLinkAggr *pToRemove = fsp_LinkFromMember(_pToRemove);
			CSLinkAggr *pPrev = fsp_LinkFromMember(_pPrev);

			pToRemove->f_UnsafeUnlink(pPrev);
		}

		static t_CData *fs_GetNext(t_CData *_pData)
		{
			CSLinkAggr *pData = fsp_LinkFromMember(_pData);
			auto pNext = pData->f_GetNext();
			if (pNext)
				return fsp_MemberFromLink(pNext);
			return nullptr;
		}

		static void fs_Unlink(t_CData *_pToRemove, t_CData *_pPrev)
		{
			CSLinkAggr *pToRemove = fsp_LinkFromMember(_pToRemove);
			CSLinkAggr *pPrev = fsp_LinkFromMember(pPrev);

			pToRemove->f_Unlink(pPrev);
		}

		inline_medium t_CData *f_Pop()
		{
			auto pNext = m_Data.f_GetFirst().f_GetNext();
			if (pNext)
			{
				if (m_Data.fs_HasLast())
				{
					if (pNext == m_Data.f_GetLast())
						m_Data.f_SetLast(&m_Data.f_GetFirst());
				}
				pNext->f_Unlink(&m_Data.f_GetFirst());
				return fsp_MemberFromLink(pNext);
			}
			else
				return nullptr;
		}

		inline_medium t_CData *f_UnsafePop()
		{
			auto pNext = m_Data.f_GetFirst().f_GetNext();
			if (pNext)
			{
				if (m_Data.fs_HasLast())
				{
					if (pNext == m_Data.f_GetLast())
						m_Data.f_SetLast(&m_Data.f_GetFirst());
				}
				pNext->f_UnsafeUnlink(&m_Data.f_GetFirst());
				return fsp_MemberFromLink(pNext);
			}
			else
				return nullptr;
		}

		inline_small void f_Push(t_CData *_pData)
		{
			f_UnsafeInsertFirst(_pData);
		}

		void f_Delete(t_CData *_pToDelete)
		{
			f_Remove(_pToDelete);
			fg_DeleteObject(t_CAllocator(), _pToDelete);
		}

		inline_small t_CData *f_GetFirst()
		{
			auto pNext = m_Data.f_GetFirst().f_GetNext();
			if (pNext)
				return fsp_MemberFromLink(pNext);
			else
				return nullptr;
		}

		inline_small t_CData *f_GetLast()
		{
			if (m_Data.fs_HasLast())
			{
				if (m_Data.f_GetLast() != &m_Data.f_GetFirst())
					return fsp_MemberFromLink(m_Data.f_GetLast());
				else
					return nullptr;
			}
			else
			{
				CSLinkAggr *pCurrent = m_Data.f_GetFirst().f_GetNext();
				if (pCurrent)
				{
					while (pCurrent->f_GetNext())
						pCurrent = pCurrent->f_GetNext();

					return fsp_MemberFromLink(pCurrent);
				}
				else
					return nullptr;
			}
		}

		inline_small t_CData const *f_GetFirst() const
		{
			if (m_Data.f_GetFirst().f_GetNext())
				return fsp_MemberFromLink(m_Data.f_GetFirst().f_GetNext());
			else
				return nullptr;
		}

		inline_small t_CData const *f_GetLast() const
		{
			if (m_Data.fs_HasLast())
			{
				if (m_Data.f_GetLast() != &m_Data.f_GetFirst())
					return fsp_MemberFromLink(m_Data.f_GetLast());
				else
					return nullptr;
			}
			else
			{
				CSLinkAggr *pCurrent = m_Data.f_GetFirst().f_GetNext();
				if (pCurrent)
				{
					while (pCurrent->f_GetNext())
						pCurrent = pCurrent->f_GetNext();

					return fsp_MemberFromLink(pCurrent);
				}
				else
					return nullptr;
			}
		}

		inline_small bool f_IsEmpty() const
		{
			return (m_Data.f_GetFirst().f_GetNext() == nullptr);
		}

		//===================================
		// Inserts
		//===================================

		inline_small void f_Insert(t_CData *_pToInsert)
		{
			if (m_Data.fs_HasLast())
			{
				CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
				pToLink->f_Link(m_Data.f_GetLast());
				m_Data.f_SetLast(pToLink);
			}
			else
				f_InsertFirst(_pToInsert);
		}

		inline_small void f_Insert(t_CData &_ToInsert)
		{
			f_Insert(&_ToInsert);
		}

		template <class t_CSortClass>
		void f_InsertSorted(t_CData &_ToInsert)
		{
			f_InsertSorted<t_CSortClass>(&_ToInsert);
		}

		template <typename tf_CCompare>
		void f_InsertSorted(t_CData *_pToInsert, tf_CCompare &&_fCompare)
		{
			CSLinkAggr *pCurrent = m_Data.f_GetFirst().f_GetNext();
			if (pCurrent)
			{
				while (fg_CheckOrdering(_fCompare(*fsp_MemberFromLink(pCurrent), *_pToInsert)) > 0)
					pCurrent = pCurrent->f_GetNext();

				CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
				pToLink->f_Link(pCurrent);
			}

			if (pCurrent)
			{
				CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
				pToLink->f_Link(pCurrent);
			}
			else
			{
				CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
				pToLink->f_Link(&m_Data.f_GetFirst());
			}
		}

		// InsertTail
		inline_small void f_InsertLast(t_CData *_pToInsert)
		{
			if (m_Data.fs_HasLast())
			{
				CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
				pToLink->f_Link(m_Data.f_GetLast());
				m_Data.f_SetLast(pToLink);
			}
			else
			{
				CSLinkAggr *pCurrent = m_Data.f_GetFirst().f_GetNext();
				if (pCurrent)
				{
					while (pCurrent->f_GetNext())
						pCurrent = pCurrent->f_GetNext();

					CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
					pToLink->f_Link(pCurrent);
				}
				else
				{
					CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
					pToLink->f_Link(&m_Data.f_GetFirst());
				}
			}
		}

		inline_small void f_InsertLast(t_CData &_ToInsert)
		{
			f_InsertLast(&_ToInsert);
		}

		// InsertHead
		inline_small void f_InsertFirst(t_CData *_pToInsert)
		{
			CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);

			pToLink->f_Link(&m_Data.f_GetFirst());

			if (m_Data.fs_HasLast())
			{
				if (!pToLink->f_GetNext())
					m_Data.f_SetLast(pToLink);
			}
		}

		inline_small void f_InsertFirst(t_CData &_ToInsert)
		{
			f_InsertFirst(&_ToInsert);
		}

		// f_InsertAfter
		inline_small void f_InsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
		{
			CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
			CSLinkAggr *pToLinkAfter = fsp_LinkFromMember(_pToInsertAfter);

			pToLink->f_Link(pToLinkAfter);

			if (m_Data.fs_HasLast())
			{
				if (!pToLink->f_GetNext())
					m_Data.f_SetLast(pToLink);
			}
		}

		inline_small void f_InsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter)
		{
			f_InsertAfter(&_ToInsert, _pToInsertAfter);
		}

		inline_small void f_InsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter)
		{
			f_InsertAfter(_pToInsert, &_ToInsertAfter);
		}

		inline_small void f_InsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter)
		{
			f_InsertAfter(&_ToInsert, &_ToInsertAfter);
		}

		//===================================
		// Unsafe Inserts
		//===================================

		inline_small void f_UnsafeInsert(t_CData *_pToInsert)
		{
			if (m_Data.fs_HasLast())
			{
				CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
				pToLink->f_UnsafeLink(m_Data.f_GetLast());
				m_Data.f_SetLast(pToLink);
			}
			else
				f_UnsafeInsertFirst(_pToInsert);
		}

		inline_small void f_UnsafeInsert(t_CData &_ToInsert)
		{
			f_UnsafeInsert(&_ToInsert);
		}

		// InsertTail
		inline_small void f_UnsafeInsertLast(t_CData *_pToInsert)
		{
			if (m_Data.fs_HasLast())
			{
				CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
				pToLink->f_UnsafeLink(m_Data.f_GetLast());
				m_Data.f_SetLast(pToLink);
			}
			else
			{
				CSLinkAggr *pCurrent = m_Data.f_GetFirst().f_GetNext();
				if (pCurrent)
				{
					while (pCurrent->f_GetNext())
						pCurrent = pCurrent->f_GetNext();

					CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
					pToLink->f_UnsafeLink(pCurrent);
				}
				else
				{
					CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
					pToLink->f_UnsafeLink(&m_Data.f_GetFirst());
				}
			}
		}

		inline_small void f_UnsafeInsertLast(t_CData &_ToInsert)
		{
			f_UnsafeInsertLast(&_ToInsert);
		}

		// InsertHead
		inline_small void f_UnsafeInsertFirst(t_CData *_pToInsert)
		{
			CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);

			pToLink->f_UnsafeLink(&m_Data.f_GetFirst());

			if (m_Data.fs_HasLast())
			{
				if (!pToLink->f_GetNext())
					m_Data.f_SetLast(pToLink);
			}
		}

		inline_small void f_UnsafeInsertFirst(t_CData &_ToInsert)
		{
			f_UnsafeInsertFirst(&_ToInsert);
		}

		// InsertHead
		inline_small void f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
		{
			CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);
			CSLinkAggr *pToLinkAfter = fsp_LinkFromMember(_pToInsertAfter);

			pToLink->f_UnsafeLink(pToLinkAfter);

			if (m_Data.fs_HasLast())
			{
				if (!pToLink->f_GetNext())
					m_Data.f_SetLast(pToLink);
			}
		}

		inline_small void f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter)
		{
			f_UnsafeInsertAfter(&_ToInsert, _pToInsertAfter);
		}

		inline_small void f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter)
		{
			f_UnsafeInsertAfter(_pToInsert, &_ToInsertAfter);
		}

		inline_small void f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter)
		{
			f_UnsafeInsertAfter(&_ToInsert, &_ToInsertAfter);
		}

		inline_small void f_UnsafePush(t_CData *_pData)
		{
			f_UnsafeInsertFirst(_pData);
		}

		void f_Reverse()
		{
			CSLinkAggr *pCurrent = m_Data.f_GetFirst().f_GetNext();
			m_Data.f_GetFirst().f_SetNext(nullptr);
			if (pCurrent)
			{
				if (m_Data.fs_HasLast())
					m_Data.f_SetLast(pCurrent);

				while (pCurrent)
				{
					CSLinkAggr *pTemp = pCurrent;
					pCurrent = pCurrent->f_GetNext();
					pTemp->f_UnsafeLink(&m_Data.f_GetFirst());
				}
			}
		}

		/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
		|	Function:			Sorts the linked list								|
		|																			|
		|	Template params:														|
		|		t_CSortClass:	The class that implements the compare funtion.		|
		|						The funtion must be static and be called Compare.	|
		|						It takes parameters									|
		|						(void *_pContext, void *_pFirst, void *_pSecond)	|
		|						and returns less that 0 if _pFirst is less than		|
		|						_pSecond, more than 0 if _pSecond is more than		|
		|						_pFirst, and 0 if the items are equal.				|
		|																			|
		|						  You can inline the compare function with good		|
		|						results, if the compare is small.					|
		|																			|
		|	Parameters:																|
		|		_pContext:		The context that is sent to the compare function	|
		|																			|
		|		_InsertionBits:	The sort uses insertion sort to sort				|
		|						1 << _InsertionBits parts of the list, then uses	|
		|						mergesort to sort the rest.							|
		|																			|
		|						  If you have an expensive sort function you		|
		|						should use 2 for _InsertionBits, wich will mostly	|
		|						use the smallest number of compares for a given		|
		|						sort session. 4 or largen Will be fastest for		|
		|						small compare funcions, but 3 is probably a good	|
		|						compromise that will work good on most compares.	|
		|																			|
		|	Returns:			description											|
		|																			|
		|	Comments:			You can use this method instead of the callback		|
		|						method when you want the compare function to be		|
		|						able to be inlined, at cost of code size.			|
		|																			|
		\*_________________________________________________________________________*/

		template <class tf_CCompare>
		void f_MergeSort(tf_CCompare &&_fCompare, aint _InsertionBits = 3)
		{
			// We use mergesort as a stable and predictably performing algorithm, needing no extra heapspace or stackspace
			// O = n(log n)

			// Empty list ??
			if (f_IsEmpty())
				return;

			// Start with insertion sorting a bit first
			aint MergeSize = 1 << (_InsertionBits);

			if (_InsertionBits > 0)
			{
				aint const SortSize = 1 << _InsertionBits;

				CSLinkAggr *pCurrent = m_Data.f_GetFirst().f_GetNext();
				CSLinkAggr *pLinkAfter = &m_Data.f_GetFirst();
				CSLinkAggr *pLast = &m_Data.f_GetFirst();
				m_Data.f_GetFirst().f_SetNext(nullptr);
				aint NumSorted = 0;

				while (pCurrent)
				{
					// Add first member

					++NumSorted;
					CSLinkAggr *pTemp = pCurrent->f_GetNext();
					pCurrent->f_UnsafeLink(pLinkAfter);
					pLast = pCurrent;
					pCurrent = pTemp;

					if (!pCurrent)
						break;

					for (aint i = 1; i < SortSize; ++i)
					{
						aint NumChecked = i;
						++NumSorted;

						CSLinkAggr *pTemp = pLinkAfter->f_GetNext();
						CSLinkAggr *pTempAfter = pLinkAfter;

						while (NumChecked)
						{
							if (fg_CheckOrdering(_fCompare(*fsp_MemberFromLink(pCurrent), *fsp_MemberFromLink(pTemp))) <= 0)
								break;
							--NumChecked;
							pTempAfter = pTemp;
							pTemp = pTemp->f_GetNext();
						}

						CSLinkAggr *pTempNext = pCurrent->f_GetNext();
						pCurrent->f_UnsafeLink(pTempAfter);
						if (!pCurrent->f_GetNext())
							pLast = pCurrent;
						pCurrent = pTempNext;
						if (!pCurrent)
							break;
					}

					pLinkAfter = pLast;
				}

				if (m_Data.fs_HasLast())
					m_Data.f_SetLast(pLast);

				if (NumSorted <= MergeSize)
				{
					// Done sorting
					return;
				}
			}

			DMibSafeCheck(_InsertionBits >= 0, "Has to be at least 1");

			while (1)
			{
				CSLinkAggr *pFirst = m_Data.f_GetFirst().f_GetNext();
				CSLinkAggr *pLast = &m_Data.f_GetFirst();

				aint MergesDone = 0; // count number of merges we do in this pass

				while (pFirst)
				{
					MergesDone++; // there exists a merge to be done
					// step `MergeSize' places along from pFirst
					CSLinkAggr *pSecond = pFirst;
					CSLinkAggr *pLastFirst = pFirst;
					aint MergeSizeFirst = 0;
					aint MergeSizeSecond = MergeSize;
					while (MergeSizeFirst < MergeSize)
					{
						MergeSizeFirst++;
						pLastFirst = pSecond;
						pSecond = pSecond->f_GetNext();
						if (!pSecond)
							break;
					}

					// if pSecond hasn't fallen off end, we have two lists to merge

					if (!pSecond)
					{
						// End of list
						pLast->f_SetNext(pFirst);
						pLast = nullptr;
						// pLast = pFirst;
						// pFirst = pFirst->m_pNext;
						// MergeSizeFirst--;
					}
					else
					{
						if (fg_CheckOrdering(_fCompare(*fsp_MemberFromLink(pLastFirst), *fsp_MemberFromLink(pSecond))) <= 0)
						{
							// Lists already sorted
							pLast->f_SetNext(pFirst);
							while (MergeSizeSecond * (umint)pSecond)
							{
								--MergeSizeSecond;
								pLast = pSecond;
								pSecond = pSecond->f_GetNext();
							}
						}
						else
						{
							DMibSafeCheck(MergeSizeSecond && MergeSizeFirst, "Hula");
							// now we have two lists; merge them
							while (1)
							{
								// decide whether m_pNext element of merge comes from pFirst or pSecond
								if (fg_CheckOrdering(_fCompare(*fsp_MemberFromLink(pFirst), *fsp_MemberFromLink(pSecond))) <= 0)
								{
									// First element of pFirst is lower (or same); pTemp must come from pFirst.
									pLast->f_SetNext(pFirst);
									pLast = pFirst;
									pFirst = pFirst->f_GetNext();
									MergeSizeFirst--;
									if (!MergeSizeFirst)
									{
										pLast->f_SetNext(pSecond);

										while (MergeSizeSecond * (umint)pSecond)
										{
											--MergeSizeSecond;
											pLast = pSecond;
											pSecond = pSecond->f_GetNext();
										}
										break;
									}
								}
								else
								{
									// First element of pSecond is lower; pTemp must come from pSecond.
									pLast->f_SetNext(pSecond);
									pLast = pSecond;
									pSecond = pSecond->f_GetNext();
									MergeSizeSecond--;
									if (!pSecond || !MergeSizeSecond)
									{
										pLast->f_SetNext(pFirst);

										while (MergeSizeFirst)
										{
											MergeSizeFirst--;
											pLast = pFirst;
											pFirst = pFirst->f_GetNext();
										}
										break;
									}
								}
							}
						}
					}

					// now pFirst has stepped `MergeSize' places along, and pSecond has too
					pFirst = pSecond;
				}

				if (pLast)
					pLast->f_SetNext(nullptr);

				// If we have done only one merge, we're finished.
				if (MergesDone <= 1) // allow for MergesDone==0, the empty list case
				{
					if (m_Data.fs_HasLast())
						m_Data.f_SetLast(pLast);
					return;
				}

				// Otherwise repeat, merging lists twice the size
				MergeSize <<= 1;
			}
		}

		using FBucketGetIndex = aint (void *_pContext, aint _Place, void *_pItem);

		struct CBucketCallbackSort
		{
			void *m_pContext;
			FBucketGetIndex *m_pSortFunction;
			static inline_small aint fs_GetIndex(void *_pContext, aint _Place, void *_pItem)
			{
				return ((CBucketCallbackSort *)_pContext)->m_pSortFunction(((CBucketCallbackSort *)_pContext)->m_pContext, _Place, _pItem);
			}
		};

		template <aint t_BucketSize>
		void f_BucketSortCallback(FBucketGetIndex *_pSortfunction, void *_pContext = nullptr)
		{
			CBucketCallbackSort SortContext;
			SortContext.m_pContext = _pContext;
			SortContext.m_pSortFunction = _pSortfunction;
			f_BucketSort<CBucketCallbackSort, t_BucketSize>(&SortContext);
		}

		/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
		|	Function:			Sorts the linked list								|
		|																			|
		|	Template params:														|
		|		t_CSortClass:	The class that implements the index funtion.		|
		|						The funtion must be static and be called fs_GetInde |
		|						It takes parameters									|
		|						(void *_pContext, aint _Place, void *_pItem)		|
		|						and returns the least significant index for _pItem	|
		|																			|
		|						  You can inline the compare function with good		|
		|						results, if the compare is small.					|
		|																			|
		|	Parameters:																|
		|		_pContext:		The context that is sent to the compare function	|
		|																			|
		|		_InsertionBits:	The sort uses insertion sort to sort				|
		|						1 << _InsertionBits parts of the list, then uses	|
		|						mergesort to sort the rest.							|
		|																			|
		|						  If you have an expensive sort function you		|
		|						should use 2 for _InsertionBits, wich will mostly	|
		|						use the smallest number of compares for a given		|
		|						sort session. 4 or largen Will be fastest for		|
		|						small compare funcions, but 3 is probably a good	|
		|						compromise that will work good on most compares.	|
		|																			|
		|	Returns:			description											|
		|																			|
		|	Comments:			You can use this method instead of the callback		|
		|						method when you want the compare function to be		|
		|						able to be inlined, at cost of code size.			|
		|																			|
		\*_________________________________________________________________________*/
		template <class t_CSortClass, aint t_BucketSize>
		void f_BucketSort(aint _NumPlaces, void *_pContext = nullptr)
		{
			if (f_IsEmpty())
				return;

			CSLinkAggr *Bucket[t_BucketSize];

			// Reset buckets
			for (aint i = 0; i < t_BucketSize; ++i)
				Bucket[i] = nullptr;

			// Put list in first bucket

			for (aint Place = 0; Place < _NumPlaces; ++Place)
			{
				// Remove last link
				CSLinkAggr *pCurrent = m_Data.f_GetFirst().f_GetNext();

				while (pCurrent)
				{
					aint Index = t_CSortClass::fs_GetIndex(_pContext, Place, fsp_MemberFromLink(pCurrent));
					CSLinkAggr **pBucket = &Bucket[Index];

					CSLinkAggr *pNext = pCurrent->f_GetNext();

					pCurrent->f_SetNext((*pBucket));
					(*pBucket) = pCurrent;

					pCurrent = pNext;
				}

				// Rebuild list

				m_Data.f_GetFirst().f_SetNext(nullptr);
				CSLinkAggr *pLast = &m_Data.f_GetFirst();

				for (aint i = 0; i < t_BucketSize; ++i)
				{
					if (Bucket[i])
					{
						CSLinkAggr *pTemp = Bucket[i];
						Bucket[i] = nullptr;
						CSLinkAggr *pLinkAfter = pLast;
						pLast = pTemp;

						while (pTemp)
						{
							CSLinkAggr *pTempNext = pTemp->f_GetNext();
							pTemp->f_UnsafeLink(pLinkAfter);
							pTemp = pTempNext;
						}
					}
				}
				if (m_Data.fs_HasLast())
					m_Data.f_SetLast(pLast);
			}
		}

		template <typename tf_CCompare>
		void f_Sort(tf_CCompare &&_fCompare)
		{
			f_MergeSort(_fCompare);
		}

		struct CIterator
		{
			CSLinkAggr *m_pCurrent;

			CIterator()
			{
				m_pCurrent = nullptr;
			}

			CIterator(TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List)
			{
				m_pCurrent = _List.m_Data.f_GetFirst().f_GetNext();
			}

			CIterator f_GetIterator() const
			{
				CIterator Iterator;
				Iterator.m_pCurrent = m_pCurrent;
				return Iterator;
			}

			CIterator &operator = (TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List)
			{
				m_pCurrent = _List.m_Data.f_GetFirst().f_GetNext();
				return *this;
			}

			inline_small t_CData *f_GetCurrent()
			{
				if (m_pCurrent)
					return TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fsp_MemberFromLink(m_pCurrent);
				else
					return nullptr;
			}

			inline_small operator t_CData * ()
			{
				return f_GetCurrent();
			}

			inline_small t_CData *operator -> ()
			{
				return f_GetCurrent();
			}

			inline_small void operator ++ ()
			{
				if (m_pCurrent)
					m_pCurrent = m_pCurrent->f_GetNext();
			}

		private:
			using COffset = COffset;
		};

		CIterator f_GetIterator()
		{
			return CIterator(*this);
		}

		struct CIteratorConst
		{
			CSLinkAggr const *m_pCurrent;

			CIteratorConst()
			{
				m_pCurrent = nullptr;
			}

			CIteratorConst(TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List)
			{
				m_pCurrent = _List.m_Data.f_GetFirst().f_GetNext();
			}

			CIteratorConst f_GetIterator() const
			{
				CIteratorConst Iterator;
				Iterator.m_pCurrent = m_pCurrent;
				return Iterator;
			}

			CIteratorConst &operator = (TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List)
			{
				m_pCurrent = _List.m_Data.f_GetFirst().f_GetNext();
				return *this;
			}

			inline_small t_CData const *f_GetCurrent()
			{
				if (m_pCurrent)
					return TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fsp_MemberFromLink(m_pCurrent);
				else
					return nullptr;
			}

			inline_small operator t_CData const * ()
			{
				return f_GetCurrent();
			}

			inline_small t_CData const *operator -> ()
			{
				return f_GetCurrent();
			}

			inline_small void operator ++ ()
			{
				if (m_pCurrent)
					m_pCurrent = m_pCurrent->f_GetNext();
			}

		private:
			using COffset = COffset;
		};

		CIteratorConst f_GetIterator() const
		{
			return CIteratorConst(*this);
		}

		umint f_GetLen() const
		{
			CIteratorConst Iter = *this;
			umint Len = 0;

			while (Iter)
			{
				++Len;
				++Iter;
			}
			return Len;
		}

	private:
		static inline_small CSLinkAggr *fsp_LinkFromMember(t_CData *_pMember)
		{
			return ((CSLinkAggr *)(((uint8 *)_pMember) + COffset::mc_Offset));
		}

		static inline_small t_CData *fsp_MemberFromLink(CSLinkAggr *_pLink)
		{
			return ((t_CData *)(((uint8 *)_pLink) - COffset::mc_Offset));
		}

		static inline_small CSLinkAggr const *fsp_LinkFromMember(t_CData const *_pMember)
		{
			return ((CSLinkAggr const *)(((uint8 *)_pMember) + COffset::mc_Offset));
		}

		static inline_small t_CData const *fsp_MemberFromLink(CSLinkAggr const *_pLink)
		{
			return ((t_CData const *)(((uint8 *)_pLink) - COffset::mc_Offset));
		}

		template <bool tf_bDoDelete>
		TCEnableIf<tf_bDoDelete> fp_AutoDelete()
		{
			f_DeleteAll();
		}

		template <bool tf_bDoDelete>
		TCEnableIf<!tf_bDoDelete> fp_AutoDelete()
		{
		}
	};

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	struct TCSLinkList : public TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>
	{
		TCSLinkList()
		{
			TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Construct();
		}

		~TCSLinkList()
		{
			TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Destruct();
		}

	private:
		TCSLinkList(TCSLinkList const &) = delete;
		TCSLinkList &operator = (TCSLinkList const &) = delete;
	};

#if defined(DCompiler_clang)

#	define DMibListLinkS_Trans(d_Class, d_Member) \
		struct CSLinkTranslator##d_Member \
		{\
			template <typename t_CClass, auto t_pMember = &d_Class::d_Member> \
			struct TCOffset \
			{ \
				constexpr static umint mc_Offset = []\
					{\
						static constexpr t_CClass const *pNode = DMibRelaxConstexpr((t_CClass const *)(void *)(NMib::TCLimitsInt<smint>::mc_Max / 2 + 1));\
						return DMibRelaxConstexpr((uint8 const *)&(pNode->*t_pMember) - (uint8 const *)pNode);\
					}()\
				;\
			}; \
		};
#else

#	define DMibListLinkS_Trans(d_Class, d_Member) \
		struct CSLinkTranslator##d_Member \
		{\
			template <typename t_CClass, auto t_pMember = &d_Class::d_Member> \
			struct TCOffset \
			{ \
				enum\
				{\
					mc_Offset = DMibPOffsetOf(t_CClass, d_Member)\
				};\
			}; \
		};

#endif

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Macros for void * Pointer Holder																	|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#define DMibListLinkS_Link(d_Class, d_Member) \
	NMib::NIntrusive::CSLink d_Member; \
	DMibListLinkS_Trans(d_Class, d_Member)

#define DMibListLinkSA_Link(d_Class, d_Member) \
	NMib::NIntrusive::CSLinkAggr d_Member; \
	DMibListLinkS_Trans(d_Class, d_Member)

#define DMibListLinkS_Member(d_Class, d_Member) \
	NMib::NIntrusive::CSLink d_Member;
#define DMibListLinkSA_Member(d_Class, d_Member) \
	NMib::NIntrusive::CSLinkAggr d_Member;

#define DMibListLinkS_LinkType() \
	NMib::NIntrusive::CSLink
#define DMibListLinkSA_LinkType() \
	NMib::NIntrusive::CSLinkAggr

	// Link with pLast

#define DMibListLinkS_Iter(d_Class, d_Member) DMibListLinkS_List(d_Class, d_Member)::CIterator
#define DMibListLinkS_List(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkList<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, false, NMib::NMemory::CDefaultAllocator>
#define DMibListLinkS_ListAutoDelete(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkList<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, true, NMib::NMemory::CDefaultAllocator>

#define DMibListLinkSA_List(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, false, NMib::NMemory::CDefaultAllocator>
#define DMibListLinkSA_ListAutoDelete(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, true, NMib::NMemory::CDefaultAllocator>

#define DMibListLinkS_Iter_FromTemplate(d_Class, d_Member) DMibListLinkS_List_Fromtemplate(d_Class, d_Member)::CIterator
#define DMibListLinkS_List_Fromtemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkList<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, false, NMib::NMemory::CDefaultAllocator>
#define DMibListLinkS_ListAutoDelete_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkList<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, true, NMib::NMemory::CDefaultAllocator>

#define DMibListLinkSA_List_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, false, NMib::NMemory::CDefaultAllocator>
#define DMibListLinkSA_ListAutoDelete_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, true, NMib::NMemory::CDefaultAllocator>

	// Link Without pLast

#define DMibListLinkS_IterNoLastPtr(d_Class, d_Member) DMibListLinkS_ListNoLastPtr(d_Class, d_Member)::CIterator
#define DMibListLinkS_ListNoLastPtr(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkList<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, false, NMib::NMemory::CDefaultAllocator>
#define DMibListLinkS_ListAutoDeleteNoLastPtr(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkList<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, true, NMib::NMemory::CDefaultAllocator>

#define DMibListLinkSA_ListNoLastPtr(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, false, NMib::NMemory::CDefaultAllocator>
#define DMibListLinkSA_ListAutoDeleteNoLastPtr(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, true, NMib::NMemory::CDefaultAllocator>

#define DMibListLinkS_IterNoLastPtr_FromTemplate(d_Class, d_Member) DMibListLinkS_ListNoLastPtr_FromTemplate(d_Class, d_Member)::CIterator
#define DMibListLinkS_ListNoLastPtr_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkList<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, false, NMib::NMemory::CDefaultAllocator>
#define DMibListLinkS_ListAutoDeleteNoLastPtr_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkList<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, true, NMib::NMemory::CDefaultAllocator>

#define DMibListLinkSA_ListNoLastPtr_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, false, NMib::NMemory::CDefaultAllocator>
#define DMibListLinkSA_ListAutoDeleteNoLastPtr_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, true, NMib::NMemory::CDefaultAllocator>

#ifndef DMibPNoShortCuts
#	define DLinkS_Link(d_Class, d_Member) DMibListLinkS_Link(d_Class, d_Member)
#	define DLinkS_Trans(d_Class, d_Member) DMibListLinkS_Trans(d_Class, d_Member)
#	define DLinkS_Member(d_Class, d_Member) DMibListLinkS_Member(d_Class, d_Member)
#	define DLinkS_LinkType(d_Class, d_Member) DMibListLinkS_LinkType(d_Class, d_Member)
#	define DLinkS_Iter(d_Class, d_Member) DMibListLinkS_Iter(d_Class, d_Member)
#	define DLinkS_List(d_Class, d_Member) DMibListLinkS_List(d_Class, d_Member)
#	define DLinkS_ListAutoDelete(d_Class, d_Member) DMibListLinkS_ListAutoDelete(d_Class, d_Member)
#	define DLinkS_IterNoLastPtr(d_Class, d_Member) DMibListLinkS_IterNoLastPtr(d_Class, d_Member)
#	define DLinkS_ListNoLastPtr(d_Class, d_Member) DMibListLinkS_ListNoLastPtr(d_Class, d_Member)
#	define DLinkS_ListAutoDeleteNoLastPtr(d_Class, d_Member) DMibListLinkS_ListAutoDeleteNoLastPtr(d_Class, d_Member)
#	define DLinkSA_Link(d_Class, d_Member) DMibListLinkSA_Link(d_Class, d_Member)
#	define DLinkSA_Member(d_Class, d_Member) DMibListLinkSA_Member(d_Class, d_Member)
#	define DLinkSA_LinkType(d_Class, d_Member) DMibListLinkSA_LinkType(d_Class, d_Member)
#	define DLinkSA_List(d_Class, d_Member) DMibListLinkSA_List(d_Class, d_Member)
#	define DLinkSA_ListAutoDelete(d_Class, d_Member) DMibListLinkSA_ListAutoDelete(d_Class, d_Member)
#	define DLinkSA_ListNoLastPtr(d_Class, d_Member) DMibListLinkSA_ListNoLastPtr(d_Class, d_Member)
#	define DLinkSA_ListAutoDeleteNoLastPtr(d_Class, d_Member) DMibListLinkSA_ListAutoDeleteNoLastPtr(d_Class, d_Member)
#endif

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Macros for variable Pointer Holder																|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#define DMibListLinkAllocatorS_Member(d_Member, d_Allocator) \
	NMib::NIntrusive::CSLink d_Member;
#define DMibListLinkAllocatorSA_Member(d_Member, d_Allocator) \
	NMib::NIntrusive::CSLinkAggr d_Member;

#define DMibListLinkAllocatorS_Link(d_Class, d_Member, d_Allocator) \
	DMibListLinkAllocatorS_Member(d_Member, d_Allocator) \
	DMibListLinkS_Trans(d_Class, d_Member)

	// List link: | Single | Aggregate | Pointer Holder
#define DMibListLinkAllocatorSA_Link(d_Class, d_Member, d_Allocator) \
	DMibListLinkAllocatorSA_Member(d_Member, d_Allocator) \
	DMibListLinkS_Trans(d_Class, d_Member)

#define DMibListLinkAllocatorS_LinkType(d_Allocator) \
	NMib::NIntrusive::CSLink
#define DMibListLinkAllocatorSA_LinkType(d_Allocator) \
	NMib::NIntrusive::CSLinkAggr

	// Link with pLast

#define DMibListLinkAllocatorS_Iter(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorS_List(d_Class, d_Member, d_Allocator)::CIterator
#define DMibListLinkAllocatorS_List(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkList<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, false, d_Allocator>
#define DMibListLinkAllocatorS_ListAutoDelete(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkList<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, true, d_Allocator>

#define DMibListLinkAllocatorSA_List(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, false, d_Allocator>
#define DMibListLinkAllocatorSA_ListAutoDelete(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, true, d_Allocator>

#define DMibListLinkAllocatorS_Iter_FromTemplate(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorS_List_Fromtemplate(d_Class, d_Member, d_Allocator)::CIterator
#define DMibListLinkAllocatorS_List_Fromtemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkList<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, false, d_Allocator>
#define DMibListLinkAllocatorS_ListAutoDelete_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkList<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, true, d_Allocator>

#define DMibListLinkAllocatorSA_List_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, false, d_Allocator>
#define DMibListLinkAllocatorSA_ListAutoDelete_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData_Last, true, d_Allocator>

	// Link Without pLast

#define DMibListLinkAllocatorS_IterNoLastPtr(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorS_ListNoLastPtr(d_Class, d_Member, d_Allocator)::CIterator
#define DMibListLinkAllocatorS_ListNoLastPtr(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkList<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, false, d_Allocator>
#define DMibListLinkAllocatorS_ListAutoDeleteNoLastPtr(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkList<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, true, d_Allocator>

#define DMibListLinkAllocatorSA_ListNoLastPtr(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, false, d_Allocator>
#define DMibListLinkAllocatorSA_ListAutoDeleteNoLastPtr(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, true, d_Allocator>

#define DMibListLinkAllocatorS_IterNoLastPtr_FromTemplate(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorS_ListNoLastPtr_FromTemplate(d_Class, d_Member, d_Allocator)::CIterator
#define DMibListLinkAllocatorS_ListNoLastPtr_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkList<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, false, d_Allocator>
#define DMibListLinkAllocatorS_ListAutoDeleteNoLastPtr_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkList<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, true, d_Allocator>

#define DMibListLinkAllocatorSA_ListNoLastPtr_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, false, d_Allocator>
#define DMibListLinkAllocatorSA_ListAutoDeleteNoLastPtr_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCSLinkListAggregate<d_Class, typename d_Class::CSLinkTranslator##d_Member, NMib::NIntrusive::CSLinkListData, true, d_Allocator>

#ifndef DMibPNoShortCuts
#	define DLinkAllocatorS_Link(d_Class, d_Member) DMibListLinkAllocatorS_Link(d_Class, d_Member)
#	define DLinkAllocatorS_Member(d_Class, d_Member) DMibListLinkAllocatorS_Member(d_Class, d_Member)
#	define DLinkAllocatorS_LinkType(d_Class, d_Member) DMibListLinkAllocatorS_LinkType(d_Class, d_Member)
#	define DLinkAllocatorS_Iter(d_Class, d_Member) DMibListLinkAllocatorS_Iter(d_Class, d_Member)
#	define DLinkAllocatorS_List(d_Class, d_Member) DMibListLinkAllocatorS_List(d_Class, d_Member)
#	define DLinkAllocatorS_ListAutoDelete(d_Class, d_Member) DMibListLinkAllocatorS_ListAutoDelete(d_Class, d_Member)
#	define DLinkAllocatorS_IterNoLastPtr(d_Class, d_Member) DMibListLinkAllocatorS_IterNoLastPtr(d_Class, d_Member)
#	define DLinkAllocatorS_ListNoLastPtr(d_Class, d_Member) DMibListLinkAllocatorS_ListNoLastPtr(d_Class, d_Member)
#	define DLinkAllocatorS_ListAutoDeleteNoLastPtr(d_Class, d_Member) DMibListLinkAllocatorS_ListAutoDeleteNoLastPtr(d_Class, d_Member)
#	define DLinkAllocatorSA_Link(d_Class, d_Member) DMibListLinkAllocatorSA_Link(d_Class, d_Member)
#	define DLinkAllocatorSA_Member(d_Class, d_Member) DMibListLinkAllocatorSA_Member(d_Class, d_Member)
#	define DLinkAllocatorSA_LinkType(d_Class, d_Member) DMibListLinkAllocatorSA_LinkType(d_Class, d_Member)
#	define DLinkAllocatorSA_List(d_Class, d_Member) DMibListLinkAllocatorSA_List(d_Class, d_Member)
#	define DLinkAllocatorSA_ListAutoDelete(d_Class, d_Member) DMibListLinkAllocatorSA_ListAutoDelete(d_Class, d_Member)
#	define DLinkAllocatorSA_ListNoLastPtr(d_Class, d_Member) DMibListLinkAllocatorSA_ListNoLastPtr(d_Class, d_Member)
#	define DLinkAllocatorSA_ListAutoDeleteNoLastPtr(d_Class, d_Member) DMibListLinkAllocatorSA_ListAutoDeleteNoLastPtr(d_Class, d_Member)
#endif
}
