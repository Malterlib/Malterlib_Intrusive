// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include <Mib/Core/Core>

namespace NMib::NIntrusive
{
	/************************************************************************************************\
	||¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯||
	|| Single linked list
	||______________________________________________________________________________________________||
	\************************************************************************************************/

	// Had to make this one a template so the functions won't get compiled at header time
//		template <aint t_Dummy = 0>

	class CSLinkAggr
	{
		using CThis = CSLinkAggr;
	public:
#ifndef DMibNoAggregateConstexpr
		constexpr CSLinkAggr(EAggregateInitialization _Init)
			: m_pNext(nullptr)
		{
		}
		inline_always CSLinkAggr()
		{
		}
#endif
		CSLinkAggr *m_pNext;

		inline_small void f_Construct()
		{
#			ifdef DMibEnableSafeCheck
				m_pNext = nullptr;
#			endif
		}

		inline_small void f_Destruct()
		{
			DMibSafeCheck(!m_pNext, "You have to take care of it. No auto delete");
		}

		inline_small void fp_Link(CSLinkAggr *_pLinkAfter)
		{
			DMibSafeCheck(!m_pNext, "Singly linked list cannot be automatically unlinked, you have to remove it from the list manually");
			m_pNext = _pLinkAfter->m_pNext;
			_pLinkAfter->m_pNext = this;
		}

		inline_small void fp_Unlink(CSLinkAggr *_pPrev)
		{
			DMibSafeCheck(_pPrev, "You better know what you are doing");
			_pPrev->m_pNext = m_pNext;
			m_pNext = nullptr;
		}

		inline_small void fp_UnsafeLink(CSLinkAggr *_pLinkAfter)
		{
			m_pNext = _pLinkAfter->m_pNext;
			_pLinkAfter->m_pNext = this;
		}

		inline_small void fp_UnsafeUnlink(CSLinkAggr *_pPrev)
		{
			_pPrev->m_pNext = m_pNext;
#			ifdef DMibEnableSafeCheck
				m_pNext = nullptr;
#			endif
		}

		inline_small CSLinkAggr *fp_GetNext() const
		{
			return m_pNext;
		}

		inline_small void fp_SetNext(CSLinkAggr * _pNext)
		{
			m_pNext = _pNext;
		}

		inline_small bint f_IsInList() const
		{
			return m_pNext != nullptr;
		}
	};


	class CSLink : public CSLinkAggr
	{
	private:
		CSLink(CSLink const &) = delete;
		CSLink &operator = (CSLink const &) = delete;

	public:
		inline_small CSLink()
		{
			CSLinkAggr::f_Construct();
		}

		inline_small ~CSLink()
		{
			CSLinkAggr::f_Destruct();
		}
	};

	class CSLinkListData_Last
	{
	public:
#ifndef DMibNoAggregateConstexpr
		constexpr CSLinkListData_Last(EAggregateInitialization _Init)
			: m_First(_Init)
			, m_pLast(nullptr)
		{
		}
		inline_always CSLinkListData_Last()
		{
		}
#endif
		CSLinkAggr m_First;
		CSLinkAggr *m_pLast;
		inline_small CSLinkAggr &fp_GetFirst()
		{
			return m_First;
		}
		inline_small const CSLinkAggr &fp_GetFirst() const
		{
			return m_First;
		}
		inline_small CSLinkAggr *fp_GetLast()
		{
			return m_pLast;
		}
		inline_small const CSLinkAggr *fp_GetLast() const
		{
			return m_pLast;
		}
		inline_small void fp_SetLast(CSLinkAggr *_pLast)
		{
			m_pLast = _pLast;
		}
		static inline_small bint fp_HasLast()
		{
			return true;
		}
	};

	class CSLinkListData
	{
	public:
#ifndef DMibNoAggregateConstexpr
		constexpr CSLinkListData(EAggregateInitialization _Init)
			: m_First(_Init)
		{
		}
		inline_always CSLinkListData()
		{
		}
#endif
		CSLinkAggr m_First;
		inline_small CSLinkAggr &fp_GetFirst()
		{
			return m_First;
		}
		inline_small CSLinkAggr *fp_GetLast()
		{
			return nullptr;
		}
		inline_small const CSLinkAggr &fp_GetFirst() const
		{
			return m_First;
		}
		inline_small const CSLinkAggr *fp_GetLast() const
		{
			return nullptr;
		}
		inline_small void fp_SetLast(CSLinkAggr *_pLast)
		{
		}
		static inline_small bint fp_HasLast()
		{
			return false;
		}

	};

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
	class TCSLinkListAggregate
	{
		typedef typename t_CTranslator::template TCOffset<t_CData> COffset;
	public:
#ifndef DMibNoAggregateConstexpr
		constexpr TCSLinkListAggregate(EAggregateInitialization _Init)
			: m_Data(_Init)
		{
#ifdef DMibDebuggerHelpers
			static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
		}
		inline_always TCSLinkListAggregate()
		{
#ifdef DMibDebuggerHelpers
			static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
		}
#endif
		t_CListData m_Data;

#ifdef DMibDebuggerHelpers
		static COffset fs_Debug_GetOffset();
#endif

		static inline_small CSLinkAggr *fp_LinkFromMember(t_CData *_pMember)
		{
			return ((CSLinkAggr *)(((uint8 *)_pMember) + COffset::mc_Offset));
		}

		static inline_small t_CData *fp_MemberFromLink(CSLinkAggr *_pLink)
		{
			return ((t_CData *)(((uint8 *)_pLink) - COffset::mc_Offset));
		}

		static inline_small const CSLinkAggr *fp_LinkFromMember(const t_CData *_pMember)
		{
			return ((const CSLinkAggr *)(((uint8 *)_pMember) + COffset::mc_Offset));
		}

		static inline_small const t_CData *fp_MemberFromLink(const CSLinkAggr *_pLink)
		{
			return ((const t_CData *)(((uint8 *)_pLink) - COffset::mc_Offset));
		}

		void f_Construct()
		{
#ifdef DMibDebuggerHelpers
			static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			m_Data.fp_GetFirst().fp_SetNext(nullptr);
			if (m_Data.fp_HasLast())
			{
				m_Data.fp_SetLast(&m_Data.fp_GetFirst());
			}
		}

		template <bool tf_bDoDelete>
		typename TCEnableIf<tf_bDoDelete>::CType fp_AutoDelete()
		{
			f_DeleteAll();
		}
		template <bool tf_bDoDelete>
		typename TCEnableIf<!tf_bDoDelete>::CType fp_AutoDelete()
		{
		}

		void f_Destruct()
		{
			if (t_bAutoDelete)
				fp_AutoDelete<t_bAutoDelete>();
			else
				f_Clear();
		}

		void f_ClearFast() // Fast clear that does not set the linked members to the unlinked state.
		{
			m_Data.fp_GetFirst().fp_SetNext(nullptr);
			if (m_Data.fp_HasLast())
				m_Data.fp_SetLast(nullptr);
		}

		void f_Clear()
		{
			while (m_Data.fp_GetFirst().fp_GetNext())
			{
				m_Data.fp_GetFirst().fp_GetNext()->fp_Unlink(&m_Data.fp_GetFirst());
			}
			if (m_Data.fp_HasLast())
				m_Data.fp_SetLast(&m_Data.fp_GetFirst());
		}


		void f_DeleteAll()
		{
			while (m_Data.fp_GetFirst().fp_GetNext())
			{
				t_CData *pToDelete = fp_MemberFromLink(m_Data.fp_GetFirst().fp_GetNext());
				m_Data.fp_GetFirst().fp_GetNext()->fp_Unlink(&m_Data.fp_GetFirst());
				fg_DeleteObject(t_CAllocator(), pToDelete);
			}

			if (m_Data.fp_HasLast())
				m_Data.fp_SetLast(&m_Data.fp_GetFirst());
		}

		void f_DeleteAllDefiniteType()
		{
			while (m_Data.fp_GetFirst().fp_GetNext())
			{
				t_CData *pToDelete = fp_MemberFromLink(m_Data.fp_GetFirst().fp_GetNext());
				m_Data.fp_GetFirst().fp_GetNext()->fp_Unlink(&m_Data.fp_GetFirst());
				fg_DeleteObjectDefiniteType(t_CAllocator(), pToDelete);
			}

			if (m_Data.fp_HasLast())
				m_Data.fp_SetLast(&m_Data.fp_GetFirst());
		}

		void f_Remove(t_CData *_pToRemove)
		{
			CSLinkAggr * pToRemove = fp_LinkFromMember(_pToRemove);
			CSLinkAggr *pCurrent = &m_Data.fp_GetFirst();
			while (pCurrent)
			{
				CSLinkAggr *pNext = pCurrent->fp_GetNext();
				if (pNext == pToRemove)
				{
					if (m_Data.fp_HasLast())
					{
						if (!pToRemove->fp_GetNext())
							m_Data.fp_SetLast(pCurrent);
					}
					pNext->fp_Unlink(pCurrent);
					break;
				}
				pCurrent = pNext;
			}
		}

		static void fs_UnsafeUnlink(t_CData *_pToRemove, t_CData *_pPrev)
		{
			CSLinkAggr * pToRemove = fp_LinkFromMember(_pToRemove);
			CSLinkAggr * pPrev = fp_LinkFromMember(_pPrev);

			pToRemove->fp_UnsafeUnlink(pPrev);
		}

		static t_CData * fs_GetNext(t_CData *_pData)
		{
			CSLinkAggr * pData = fp_LinkFromMember(_pData);
			auto pNext = pData->fp_GetNext();
			if (pNext)
				return fp_MemberFromLink(pNext);
			return nullptr;
		}

		static void fs_Unlink(t_CData *_pToRemove, t_CData *_pPrev)
		{
			CSLinkAggr * pToRemove = fp_LinkFromMember(_pToRemove);
			CSLinkAggr * pPrev = fp_LinkFromMember(pPrev);

			pToRemove->fp_Unlink(pPrev);
		}

		inline_medium t_CData *f_Pop()
		{
			auto pNext = m_Data.fp_GetFirst().fp_GetNext();
			if (pNext)
			{
				if (m_Data.fp_HasLast())
				{
					if (pNext == m_Data.fp_GetLast())
						m_Data.fp_SetLast(&m_Data.fp_GetFirst());
				}
				pNext->fp_Unlink(&m_Data.fp_GetFirst());
				return fp_MemberFromLink(pNext);
			}
			else
				return nullptr;
		}

		inline_medium t_CData *f_UnsafePop()
		{
			auto pNext = m_Data.fp_GetFirst().fp_GetNext();
			if (pNext)
			{
				if (m_Data.fp_HasLast())
				{
					if (pNext == m_Data.fp_GetLast())
						m_Data.fp_SetLast(&m_Data.fp_GetFirst());
				}
				pNext->fp_UnsafeUnlink(&m_Data.fp_GetFirst());
				return fp_MemberFromLink(pNext);
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
			delete _pToDelete;
		}

		inline_small t_CData *f_GetFirst()
		{
			auto pNext = m_Data.fp_GetFirst().fp_GetNext();
			if (pNext)
				return fp_MemberFromLink(pNext);
			else
				return nullptr;
		}

		inline_small t_CData *f_GetLast()
		{
			if (m_Data.fp_HasLast())
			{
				if (m_Data.fp_GetLast() != &m_Data.fp_GetFirst())
					return fp_MemberFromLink(m_Data.fp_GetLast());
				else
					return nullptr;
			}
			else
			{
				CSLinkAggr *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
				if (pCurrent)
				{
					while (pCurrent->fp_GetNext())
					{
						pCurrent = pCurrent->fp_GetNext();
					}

					return fp_MemberFromLink(pCurrent);
				}
				else
					return nullptr;
			}
		}

		inline_small const t_CData *f_GetFirst() const
		{
			if (m_Data.fp_GetFirst().fp_GetNext())
				return fp_MemberFromLink(m_Data.fp_GetFirst().fp_GetNext());
			else
				return nullptr;
		}

		inline_small const t_CData *f_GetLast() const
		{
			if (m_Data.fp_HasLast())
			{
				if (m_Data.fp_GetLast() != &m_Data.fp_GetFirst())
					return fp_MemberFromLink(m_Data.fp_GetLast());
				else
					return nullptr;
			}
			else
			{
				CSLinkAggr *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
				if (pCurrent)
				{
					while (pCurrent->fp_GetNext())
					{
						pCurrent = pCurrent->fp_GetNext();
					}

					return fp_MemberFromLink(pCurrent);
				}
				else
					return nullptr;
			}
		}

		inline_small bint f_IsEmpty() const
		{
			return (m_Data.fp_GetFirst().fp_GetNext() == nullptr);
		}

		//===================================
		// Inserts
		//===================================

		inline_small void f_Insert(t_CData *_pToInsert)
		{
			if (m_Data.fp_HasLast())
			{
				CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
				pToLink->fp_Link(m_Data.fp_GetLast());
				m_Data.fp_SetLast(pToLink);
			}
			else
			{
				f_InsertFirst(_pToInsert);
			}
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

		template <class t_CSortClass>
		void f_InsertSorted(t_CData *_pToInsert)
		{
			CSLinkAggr *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
			if (pCurrent)
			{
				while (t_CSortClass::fs_Compare(fp_MemberFromLink(pCurrent), _pToInsert) > 0)
				{
					pCurrent = pCurrent->fp_GetNext();
				}

				CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
				pToLink->fp_Link(pCurrent);
			}

			if (pCurrent)
			{
				CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
				pToLink->fp_Link(pCurrent);
			}
			else
			{
				CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
				pToLink->fp_Link(&m_Data.fp_GetFirst());
			}
		}

		// InsertTail
		inline_small void f_InsertLast(t_CData *_pToInsert)
		{
			if (m_Data.fp_HasLast())
			{
				CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
				pToLink->fp_Link(m_Data.fp_GetLast());
				m_Data.fp_SetLast(pToLink);
			}
			else
			{
				CSLinkAggr *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
				if (pCurrent)
				{
					while (pCurrent->fp_GetNext())
					{
						pCurrent = pCurrent->fp_GetNext();
					}

					CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
					pToLink->fp_Link(pCurrent);
				}
				else
				{
					CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
					pToLink->fp_Link(&m_Data.fp_GetFirst());
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
			CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);

			pToLink->fp_Link(&m_Data.fp_GetFirst());

			if (m_Data.fp_HasLast())
			{
				if (!pToLink->fp_GetNext())
					m_Data.fp_SetLast(pToLink);
			}
		}
		inline_small void f_InsertFirst(t_CData &_ToInsert)
		{
			f_InsertFirst(&_ToInsert);
		}

		// f_InsertAfter
		inline_small void f_InsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
		{
			CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
			CSLinkAggr *pToLinkAfter = fp_LinkFromMember(_pToInsertAfter);

			pToLink->fp_Link(pToLinkAfter);

			if (m_Data.fp_HasLast())
			{
				if (!pToLink->fp_GetNext())
					m_Data.fp_SetLast(pToLink);
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
			if (m_Data.fp_HasLast())
			{
				CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
				pToLink->fp_UnsafeLink(m_Data.fp_GetLast());
				m_Data.fp_SetLast(pToLink);
			}
			else
			{
				f_UnsafeInsertFirst(_pToInsert);
			}
		}

		inline_small void f_UnsafeInsert(t_CData &_ToInsert)
		{
			f_UnsafeInsert(&_ToInsert);
		}

		// InsertTail
		inline_small void f_UnsafeInsertLast(t_CData *_pToInsert)
		{
			if (m_Data.fp_HasLast())
			{
				CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
				pToLink->fp_UnsafeLink(m_Data.fp_GetLast());
				m_Data.fp_SetLast(pToLink);
			}
			else
			{
				CSLinkAggr *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
				if (pCurrent)
				{
					while (pCurrent->fp_GetNext())
					{
						pCurrent = pCurrent->fp_GetNext();
					}

					CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
					pToLink->fp_UnsafeLink(pCurrent);
				}
				else
				{
					CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
					pToLink->fp_UnsafeLink(&m_Data.fp_GetFirst());
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
			CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);

			pToLink->fp_UnsafeLink(&m_Data.fp_GetFirst());

			if (m_Data.fp_HasLast())
			{
				if (!pToLink->fp_GetNext())
					m_Data.fp_SetLast(pToLink);
			}
		}
		inline_small void f_UnsafeInsertFirst(t_CData &_ToInsert)
		{
			f_UnsafeInsertFirst(&_ToInsert);
		}

		// InsertHead
		inline_small void f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
		{
			CSLinkAggr *pToLink = fp_LinkFromMember(_pToInsert);
			CSLinkAggr *pToLinkAfter = fp_LinkFromMember(_pToInsertAfter);

			pToLink->fp_UnsafeLink(pToLinkAfter);

			if (m_Data.fp_HasLast())
			{
				if (!pToLink->fp_GetNext())
					m_Data.fp_SetLast(pToLink);
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
			CSLinkAggr *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
			m_Data.fp_GetFirst().fp_SetNext(nullptr);
			if (pCurrent)
			{
				if (m_Data.fp_HasLast())
				{
					m_Data.fp_SetLast(pCurrent);
				}

				while (pCurrent)
				{
					CSLinkAggr *pTemp = pCurrent;
					pCurrent = pCurrent->fp_GetNext();
					pTemp->fp_UnsafeLink(&m_Data.fp_GetFirst());
				}
			}
		}

		//element *listsort(element *list, aint is_circular, aint is_double) {

		typedef aint FMergeCompare(void *_pContext, void *_pFirst, void *_pSecond);
		class CMergeCallbackSort
		{
		public:
			void *m_pContext;
			FMergeCompare *m_pSortFunction;
			typedef aint CRet;
			static inline_small CRet fs_Compare(void *_pContext, void *_pFirst, void *_pSecond)
			{
				return ((CMergeCallbackSort *)_pContext)->m_pSortFunction(((CMergeCallbackSort *)_pContext)->m_pContext, _pFirst, _pSecond);
			}
		};

		void f_MergeSortCallback(FMergeCompare *_pSortfunction, void *_pContext = nullptr, aint _InsertionBits = 3)
		{
			CMergeCallbackSort SortContext;
			SortContext.m_pContext = _pContext;
			SortContext.m_pSortFunction = _pSortfunction;
			f_MergeSort<CMergeCallbackSort>(&SortContext, _InsertionBits);
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

		template <class t_CSortClass>
		void f_MergeSort(void *_pContext = nullptr, aint _InsertionBits = 3)
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
				const aint SortSize = 1 << _InsertionBits;

				CSLinkAggr *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
				CSLinkAggr *pLinkAfter = &m_Data.fp_GetFirst();
				CSLinkAggr *pLast = &m_Data.fp_GetFirst();
				m_Data.fp_GetFirst().fp_SetNext(nullptr);
				aint NumSorted = 0;

				while (pCurrent)
				{
					// Add first member

					++NumSorted;
					CSLinkAggr *pTemp = pCurrent->fp_GetNext();
					pCurrent->fp_UnsafeLink(pLinkAfter);
					pLast = pCurrent;
					pCurrent = pTemp;

					if (!pCurrent)
						break;

					for (aint i = 1; i < SortSize; ++i)
					{
						aint NumChecked = i;
						++NumSorted;

						CSLinkAggr *pTemp = pLinkAfter->fp_GetNext();
						CSLinkAggr *pTempAfter = pLinkAfter;

						while (NumChecked)
						{
							if (t_CSortClass::fs_Compare(_pContext,fp_MemberFromLink(pCurrent),fp_MemberFromLink(pTemp)) <= 0)
								break;
							--NumChecked;
							pTempAfter = pTemp;
							pTemp = pTemp->fp_GetNext();
						}

						CSLinkAggr *pTempNext = pCurrent->fp_GetNext();
						pCurrent->fp_UnsafeLink(pTempAfter);
						if (!pCurrent->fp_GetNext())
							pLast = pCurrent;
						pCurrent = pTempNext;
						if (!pCurrent)
							break;
					}

					pLinkAfter = pLast;
				}

				if (m_Data.fp_HasLast())
				{
					m_Data.fp_SetLast(pLast);
				}

				if (NumSorted <= MergeSize)
				{
					// Done sorting
					return;
				}

			}

			DMibSafeCheck(_InsertionBits >= 0, "Has to be at least 1");


			while (1)
			{
				CSLinkAggr *pFirst = m_Data.fp_GetFirst().fp_GetNext();
				CSLinkAggr *pLast = &m_Data.fp_GetFirst();

				aint MergesDone = 0;  // count number of merges we do in this pass

				while (pFirst)
				{
					MergesDone++;  // there exists a merge to be done
					// step `MergeSize' places along from pFirst
					CSLinkAggr *pSecond = pFirst;
					CSLinkAggr *pLastFirst = pFirst;
					aint MergeSizeFirst = 0;
					aint MergeSizeSecond = MergeSize;
					while (MergeSizeFirst < MergeSize)
					{
						MergeSizeFirst++;
						pLastFirst = pSecond;
						pSecond = pSecond->fp_GetNext();
						if (!pSecond)
							break;
					}

					// if pSecond hasn't fallen off end, we have two lists to merge

					if (!pSecond)
					{
						// End of list
						pLast->fp_SetNext(pFirst);
						pLast = nullptr;
//							pLast = pFirst;
//							pFirst = pFirst->m_pNext;
//							MergeSizeFirst--;
					}
					else
					{
						if (t_CSortClass::fs_Compare(_pContext,fp_MemberFromLink(pLastFirst),fp_MemberFromLink(pSecond)) <= 0)
						{
							// Lists already sorted
							pLast->fp_SetNext(pFirst);
							while (MergeSizeSecond * (mint)pSecond)
							{
								--MergeSizeSecond;
								pLast = pSecond;
								pSecond = pSecond->fp_GetNext();
							}
						}
						else
						{

							DMibSafeCheck(MergeSizeSecond && MergeSizeFirst, "Hula");
							// now we have two lists; merge them
							while (1)
							{
								// decide whether m_pNext element of merge comes from pFirst or pSecond
								if (t_CSortClass::fs_Compare(_pContext,fp_MemberFromLink(pFirst),fp_MemberFromLink(pSecond)) <= 0)
								{
									// First element of pFirst is lower (or same); pTemp must come from pFirst.
									pLast->fp_SetNext(pFirst);
									pLast = pFirst;
									pFirst = pFirst->fp_GetNext();
									MergeSizeFirst--;
									if (!MergeSizeFirst)
									{
										pLast->fp_SetNext(pSecond);

										while (MergeSizeSecond * (mint)pSecond)
										{
											--MergeSizeSecond;
											pLast = pSecond;
											pSecond = pSecond->fp_GetNext();
										}
										break;
									}
								}
								else
								{
									// First element of pSecond is lower; pTemp must come from pSecond.
									pLast->fp_SetNext(pSecond);
									pLast = pSecond;
									pSecond = pSecond->fp_GetNext();
									MergeSizeSecond--;
									if (!pSecond || !MergeSizeSecond)
									{
										pLast->fp_SetNext(pFirst);

										while (MergeSizeFirst)
										{
											MergeSizeFirst--;
											pLast = pFirst;
											pFirst = pFirst->fp_GetNext();
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
					pLast->fp_SetNext(nullptr);

				// If we have done only one merge, we're finished.
				if (MergesDone <= 1)   // allow for MergesDone==0, the empty list case
				{
					if (m_Data.fp_HasLast())
					{
						m_Data.fp_SetLast(pLast);
					}
					return;
				}

				// Otherwise repeat, merging lists twice the size
				MergeSize <<= 1;
			}
		}

		typedef aint FBucketGetIndex(void *_pContext, aint _Place, void *_pItem);
		class CBucketCallbackSort
		{
		public:
			void *m_pContext;
			FBucketGetIndex *m_pSortFunction;
			static inline_small aint fs_GetIndex(void *_pContext, aint _Place, void *_pItem)
			{
				return ((CBucketCallbackSort *)_pContext)->m_pSortFunction(((CBucketCallbackSort *)_pContext)->m_pContext, _pItem);
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
			{
				Bucket[i] = nullptr;
			}

			// Put list in first bucket

			for(aint Place = 0; Place < _NumPlaces; ++Place)
			{
				// Remove last link
				CSLinkAggr *pCurrent = m_Data.fp_GetFirst().fp_GetNext();

				while (pCurrent)
				{
					aint Index = t_CSortClass::fs_GetIndex(_pContext, Place, fp_MemberFromLink(pCurrent));
					CSLinkAggr **pBucket = &Bucket[Index];

					CSLinkAggr *pNext = pCurrent->fp_GetNext();

					pCurrent->fp_SetNext((*pBucket));
					(*pBucket) = pCurrent;

					pCurrent = pNext;
				}


				// Rebuild list

				m_Data.fp_GetFirst().fp_SetNext(nullptr);
				CSLinkAggr *pLast = &m_Data.fp_GetFirst();

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
							CSLinkAggr *pTempNext = pTemp->fp_GetNext();
							pTemp->fp_UnsafeLink(pLinkAfter);
							pTemp = pTempNext;
						}
					}
				}
				if (m_Data.fp_HasLast())
				{
					m_Data.fp_SetLast(pLast);
				}
			}

		}

		template <typename t_CFunctor>
		struct TCComparerer
		{
			t_CFunctor m_Functor;
			TCComparerer(t_CFunctor _Functor)
				: m_Functor(_Functor)
			{
			}
			typedef int32 CRet;
			template <typename t_CLeft, typename t_CRight>
			static inline_small CRet fs_Compare(void * _pContext, t_CLeft &&_Left, t_CRight &&_Right)
			{
				TCComparerer *pContext = (TCComparerer *)_pContext;
				if (pContext->m_Functor(*_Left, *_Right))
					return -1;
				else if (pContext->m_Functor(*_Right, *_Left))
					return 1;
				return 0;
			}
		};

		template <typename t_CFunctor>
		void f_Sort(t_CFunctor &&_Functor)
		{
			TCComparerer<t_CFunctor> Comparerer(_Functor);
			f_MergeSort<TCComparerer<t_CFunctor>>(&Comparerer);
		}

		class CIterator
		{
			typedef COffset COffset;
		public:
			CSLinkAggr *m_pCurrent;

#ifdef DMibDebuggerHelpers
			static TCSLinkListAggregate *fs_Debug_List();
			static COffset fs_Debug_GetOffset();
#endif
			CIterator()
			{
				m_pCurrent = nullptr;
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIterator(const TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> &_List)
			{
				m_pCurrent = _List.m_Data.fp_GetFirst().fp_GetNext();
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIterator& operator = (const TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> &_List)
			{
				m_pCurrent = _List.m_Data.fp_GetFirst().fp_GetNext();
				return *this;
			}

			inline_small t_CData *f_GetCurrent()
			{
				if (m_pCurrent)
					return TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fp_MemberFromLink(m_pCurrent);
				else
					return nullptr;
			}

			inline_small operator t_CData *()
			{
				return f_GetCurrent();
			}

			inline_small t_CData * operator ->()
			{
				return f_GetCurrent();
			}

			inline_small void operator ++()
			{
				if (m_pCurrent)
				{
					m_pCurrent = m_pCurrent->fp_GetNext();
				}
			}

		};

		CIterator f_GetIterator()
		{
			return CIterator(*this);
		}

		class CIteratorConst
		{
			typedef COffset COffset;
		public:
			const CSLinkAggr *m_pCurrent;

#ifdef DMibDebuggerHelpers
			static TCSLinkListAggregate *fs_Debug_List();
			static COffset fs_Debug_GetOffset();
#endif
			CIteratorConst()
			{
				m_pCurrent = nullptr;
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIteratorConst(const TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> &_List)
			{
				m_pCurrent = _List.m_Data.fp_GetFirst().fp_GetNext();
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIteratorConst& operator = (const TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> &_List)
			{
				m_pCurrent = _List.m_Data.fp_GetFirst().fp_GetNext();
				return *this;
			}

			inline_small const t_CData *f_GetCurrent()
			{
				if (m_pCurrent)
					return TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fp_MemberFromLink(m_pCurrent);
				else
					return nullptr;
			}

			inline_small operator const t_CData *()
			{
				return f_GetCurrent();
			}

			inline_small const t_CData * operator ->()
			{
				return f_GetCurrent();
			}

			inline_small void operator ++()
			{
				if (m_pCurrent)
				{
					m_pCurrent = m_pCurrent->fp_GetNext();
				}
			}

		};

		CIteratorConst f_GetIterator() const
		{
			return CIteratorConst(*this);
		}

		mint f_GetLen() const
		{
			CIteratorConst Iter = *this;
			mint Len = 0;

			while (Iter)
			{
				++Len;
				++Iter;
			}
			return Len;
		}

	};

#ifdef DMibDebuggerHelpers
	template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
	auto TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fs_Debug_GetOffset() -> COffset
	{
		return {};
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
	TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>*
	TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::fs_Debug_List()
	{
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
	auto TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::fs_Debug_GetOffset() -> COffset
	{
		return {};
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
	TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>*
	TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::fs_Debug_List()
	{
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
	auto TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::fs_Debug_GetOffset() -> COffset
	{
		return {};
	}
#endif

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
	class TCSLinkList : public TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>
	{
	private:
		TCSLinkList(TCSLinkList const &) = delete;
		TCSLinkList &operator = (TCSLinkList const &) = delete;

	public:
		TCSLinkList()
		{
			TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Construct();
		}
		~TCSLinkList()
		{
			TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Destruct();
		}
	};

#	define DMibListLinkS_Trans(_Class, _Member) \
		class CSLinkTranslator##_Member \
		{\
		public:\
			template <typename t_CClass, auto t_pMember = &_Class::_Member> \
			struct TCOffset \
			{ \
				enum\
				{\
					mc_Offset = DMibPOffsetOf(t_CClass, _Member)\
				};\
			}; \
		};\


	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Macros for void * Pointer Holder																	|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/


#	define DMibListLinkS_Link(_Class, _Member) \
			NMib::NIntrusive::CSLink _Member; \
			DMibListLinkS_Trans(_Class, _Member)

#	define DMibListLinkSA_Link(_Class, _Member) \
			NMib::NIntrusive::CSLinkAggr _Member; \
			DMibListLinkS_Trans(_Class, _Member)


#	define DMibListLinkS_Member(_Class, _Member) \
			NMib::NIntrusive::CSLink _Member;
#	define DMibListLinkSA_Member(_Class, _Member) \
			NMib::NIntrusive::CSLinkAggr _Member;

#	define DMibListLinkS_LinkType() \
			NMib::NIntrusive::CSLink
#	define DMibListLinkSA_LinkType() \
			NMib::NIntrusive::CSLinkAggr

	// Link with pLast

#	define DMibListLinkS_Iter(_Class, _Member) DMibListLinkS_List(_Class, _Member)::CIterator
#	define DMibListLinkS_List(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkS_ListAutoDelete(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkSA_List(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkSA_ListAutoDelete(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkS_Iter_FromTemplate(_Class, _Member) DMibListLinkS_List_Fromtemplate(_Class, _Member)::CIterator
#	define DMibListLinkS_List_Fromtemplate(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkS_ListAutoDelete_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkSA_List_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkSA_ListAutoDelete_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, true, NMib::NMemory::CDefaultAllocator>

	// Link Without pLast

#	define DMibListLinkS_IterNoLastPtr(_Class, _Member) DMibListLinkS_ListNoLastPtr(_Class, _Member)::CIterator
#	define DMibListLinkS_ListNoLastPtr(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkS_ListAutoDeleteNoLastPtr(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkSA_ListNoLastPtr(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkSA_ListAutoDeleteNoLastPtr(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkS_IterNoLastPtr_FromTemplate(_Class, _Member) DMibListLinkS_ListNoLastPtr_FromTemplate(_Class, _Member)::CIterator
#	define DMibListLinkS_ListNoLastPtr_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkS_ListAutoDeleteNoLastPtr_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkSA_ListNoLastPtr_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkSA_ListAutoDeleteNoLastPtr_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, true, NMib::NMemory::CDefaultAllocator>

#	ifndef DMibPNoShortCuts
#		define DLinkS_Link(_Class, _Member) DMibListLinkS_Link(_Class, _Member)
#		define DLinkS_Trans(_Class, _Member) DMibListLinkS_Trans(_Class, _Member)
#		define DLinkS_Member(_Class, _Member) DMibListLinkS_Member(_Class, _Member)
#		define DLinkS_LinkType(_Class, _Member) DMibListLinkS_LinkType(_Class, _Member)
#		define DLinkS_Iter(_Class, _Member) DMibListLinkS_Iter(_Class, _Member)
#		define DLinkS_List(_Class, _Member) DMibListLinkS_List(_Class, _Member)
#		define DLinkS_ListAutoDelete(_Class, _Member) DMibListLinkS_ListAutoDelete(_Class, _Member)
#		define DLinkS_IterNoLastPtr(_Class, _Member) DMibListLinkS_IterNoLastPtr(_Class, _Member)
#		define DLinkS_ListNoLastPtr(_Class, _Member) DMibListLinkS_ListNoLastPtr(_Class, _Member)
#		define DLinkS_ListAutoDeleteNoLastPtr(_Class, _Member) DMibListLinkS_ListAutoDeleteNoLastPtr(_Class, _Member)
#		define DLinkSA_Link(_Class, _Member) DMibListLinkSA_Link(_Class, _Member)
#		define DLinkSA_Member(_Class, _Member) DMibListLinkSA_Member(_Class, _Member)
#		define DLinkSA_LinkType(_Class, _Member) DMibListLinkSA_LinkType(_Class, _Member)
#		define DLinkSA_List(_Class, _Member) DMibListLinkSA_List(_Class, _Member)
#		define DLinkSA_ListAutoDelete(_Class, _Member) DMibListLinkSA_ListAutoDelete(_Class, _Member)
#		define DLinkSA_ListNoLastPtr(_Class, _Member) DMibListLinkSA_ListNoLastPtr(_Class, _Member)
#		define DLinkSA_ListAutoDeleteNoLastPtr(_Class, _Member) DMibListLinkSA_ListAutoDeleteNoLastPtr(_Class, _Member)
#	endif

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Macros for variable Pointer Holder																|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/


#	define DMibListLinkAllocatorS_Member(_Member, _Allocator) \
			NMib::NIntrusive::CSLink _Member;
#	define DMibListLinkAllocatorSA_Member(_Member, _Allocator) \
			NMib::NIntrusive::CSLinkAggr _Member;

#	define DMibListLinkAllocatorS_Link(_Class, _Member, _Allocator) \
			DMibListLinkAllocatorS_Member(_Member, _Allocator) \
			DMibListLinkS_Trans(_Class, _Member)

// List link: | Single | Aggregate | Pointer Holder
#	define DMibListLinkAllocatorSA_Link(_Class, _Member, _Allocator) \
			DMibListLinkAllocatorSA_Member(_Member, _Allocator) \
			DMibListLinkS_Trans(_Class, _Member)


#	define DMibListLinkAllocatorS_LinkType(_Allocator) \
			NMib::NIntrusive::CSLink
#	define DMibListLinkAllocatorSA_LinkType(_Allocator) \
			NMib::NIntrusive::CSLinkAggr

	// Link with pLast

#	define DMibListLinkAllocatorS_Iter(_Class, _Member, _Allocator) DMibListLinkAllocatorS_List(_Class, _Member, _Allocator)::CIterator
#	define DMibListLinkAllocatorS_List(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, false, _Allocator>
#	define DMibListLinkAllocatorS_ListAutoDelete(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, true, _Allocator>

#	define DMibListLinkAllocatorSA_List(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, false, _Allocator>
#	define DMibListLinkAllocatorSA_ListAutoDelete(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, true, _Allocator>

#	define DMibListLinkAllocatorS_Iter_FromTemplate(_Class, _Member, _Allocator) DMibListLinkAllocatorS_List_Fromtemplate(_Class, _Member, _Allocator)::CIterator
#	define DMibListLinkAllocatorS_List_Fromtemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, false, _Allocator>
#	define DMibListLinkAllocatorS_ListAutoDelete_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, true, _Allocator>

#	define DMibListLinkAllocatorSA_List_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, false, _Allocator>
#	define DMibListLinkAllocatorSA_ListAutoDelete_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData_Last, true, _Allocator>

	// Link Without pLast

#	define DMibListLinkAllocatorS_IterNoLastPtr(_Class, _Member, _Allocator) DMibListLinkAllocatorS_ListNoLastPtr(_Class, _Member, _Allocator)::CIterator
#	define DMibListLinkAllocatorS_ListNoLastPtr(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, false, _Allocator>
#	define DMibListLinkAllocatorS_ListAutoDeleteNoLastPtr(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, true, _Allocator>

#	define DMibListLinkAllocatorSA_ListNoLastPtr(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, false, _Allocator>
#	define DMibListLinkAllocatorSA_ListAutoDeleteNoLastPtr(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, true, _Allocator>

#	define DMibListLinkAllocatorS_IterNoLastPtr_FromTemplate(_Class, _Member, _Allocator) DMibListLinkAllocatorS_ListNoLastPtr_FromTemplate(_Class, _Member, _Allocator)::CIterator
#	define DMibListLinkAllocatorS_ListNoLastPtr_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, false, _Allocator>
#	define DMibListLinkAllocatorS_ListAutoDeleteNoLastPtr_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, true, _Allocator>

#	define DMibListLinkAllocatorSA_ListNoLastPtr_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, false, _Allocator>
#	define DMibListLinkAllocatorSA_ListAutoDeleteNoLastPtr_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::CSLinkListData, true, _Allocator>

#	ifndef DMibPNoShortCuts
#		define DLinkAllocatorS_Link(_Class, _Member) DMibListLinkAllocatorS_Link(_Class, _Member)
#		define DLinkAllocatorS_Member(_Class, _Member) DMibListLinkAllocatorS_Member(_Class, _Member)
#		define DLinkAllocatorS_LinkType(_Class, _Member) DMibListLinkAllocatorS_LinkType(_Class, _Member)
#		define DLinkAllocatorS_Iter(_Class, _Member) DMibListLinkAllocatorS_Iter(_Class, _Member)
#		define DLinkAllocatorS_List(_Class, _Member) DMibListLinkAllocatorS_List(_Class, _Member)
#		define DLinkAllocatorS_ListAutoDelete(_Class, _Member) DMibListLinkAllocatorS_ListAutoDelete(_Class, _Member)
#		define DLinkAllocatorS_IterNoLastPtr(_Class, _Member) DMibListLinkAllocatorS_IterNoLastPtr(_Class, _Member)
#		define DLinkAllocatorS_ListNoLastPtr(_Class, _Member) DMibListLinkAllocatorS_ListNoLastPtr(_Class, _Member)
#		define DLinkAllocatorS_ListAutoDeleteNoLastPtr(_Class, _Member) DMibListLinkAllocatorS_ListAutoDeleteNoLastPtr(_Class, _Member)
#		define DLinkAllocatorSA_Link(_Class, _Member) DMibListLinkAllocatorSA_Link(_Class, _Member)
#		define DLinkAllocatorSA_Member(_Class, _Member) DMibListLinkAllocatorSA_Member(_Class, _Member)
#		define DLinkAllocatorSA_LinkType(_Class, _Member) DMibListLinkAllocatorSA_LinkType(_Class, _Member)
#		define DLinkAllocatorSA_List(_Class, _Member) DMibListLinkAllocatorSA_List(_Class, _Member)
#		define DLinkAllocatorSA_ListAutoDelete(_Class, _Member) DMibListLinkAllocatorSA_ListAutoDelete(_Class, _Member)
#		define DLinkAllocatorSA_ListNoLastPtr(_Class, _Member) DMibListLinkAllocatorSA_ListNoLastPtr(_Class, _Member)
#		define DLinkAllocatorSA_ListAutoDeleteNoLastPtr(_Class, _Member) DMibListLinkAllocatorSA_ListAutoDeleteNoLastPtr(_Class, _Member)
#	endif

}
