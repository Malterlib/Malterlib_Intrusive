// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include <Mib/Core/Core>

namespace NMib
{
	namespace NIntrusive
	{		

		/************************************************************************************************\
		||¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯||
		|| Single linked list
		||______________________________________________________________________________________________||
		\************************************************************************************************/

		// Had to make this one a template so the functions won't get compiled at header time
//		template <aint t_Dummy = 0>

		template <typename t_CAllocator>
		class TCSLinkAggr
		{
			using CThis = TCSLinkAggr;
		public:
#ifndef DMibNoAggregateConstexpr
			constexpr TCSLinkAggr(EAggregateInitialization _Init)
				: m_pNext(_Init)
			{
			}
			inline_always TCSLinkAggr()
			{
			}
#endif
			TCDynamicPtr<typename t_CAllocator::CPtrHolder, TCSLinkAggr> m_pNext;

			inline_small void f_Construct()
			{
#				ifdef DMibEnableSafeCheck
					m_pNext = nullptr;
#				endif
			}

			inline_small void f_Destruct()
			{
				DMibSafeCheck(!m_pNext, "You have to take care of it. No auto delete");
			}

			inline_small void fp_Link(TCSLinkAggr *_pLinkAfter)
			{
				DMibSafeCheck(!m_pNext, "Singly linked list cannot be automatically unlinked, you have to remove it from the list manually");
				m_pNext = _pLinkAfter->m_pNext;
				_pLinkAfter->m_pNext = (TCSLinkAggr *)this;
			}

			inline_small void fp_Unlink(TCSLinkAggr *_pPrev)
			{
				DMibSafeCheck(_pPrev, "You better know what you are doing");
				_pPrev->m_pNext = m_pNext;
				m_pNext = nullptr;
			}

			inline_small void fp_UnsafeLink(TCSLinkAggr *_pLinkAfter)
			{
				m_pNext = _pLinkAfter->m_pNext;
				_pLinkAfter->m_pNext = (TCSLinkAggr *)this;
			}

			inline_small void fp_UnsafeUnlink(TCSLinkAggr *_pPrev)
			{
				_pPrev->m_pNext = m_pNext;
#				ifdef DMibEnableSafeCheck
					m_pNext = nullptr;
#				endif
			}

			inline_small TCSLinkAggr *fp_GetNext() const
			{
				return m_pNext;
			}

			inline_small void fp_SetNext(TCSLinkAggr * _pNext)
			{
				m_pNext = _pNext;
			}

			inline_small bint f_IsInList() const
			{
				return m_pNext != nullptr;
			}
		};


		template <typename t_CAllocator>
		class TCSLink : public TCSLinkAggr<t_CAllocator>
		{
		private:
			DMibClassNoCopyAllowed(TCSLink);
		public:
			inline_small TCSLink()
			{
				TCSLinkAggr<t_CAllocator>::f_Construct();
			}

			inline_small ~TCSLink()
			{
				TCSLinkAggr<t_CAllocator>::f_Destruct();
			}
		};

		template <typename t_CAllocator>
		class TCSLinkListData_Last
		{
		public:
#ifndef DMibNoAggregateConstexpr
			constexpr TCSLinkListData_Last(EAggregateInitialization _Init)
				: m_First(_Init)
			{
			}
			inline_always TCSLinkListData_Last()
			{
			}
#endif
			TCSLinkAggr<t_CAllocator> m_First;
			TCDynamicPtr<typename t_CAllocator::CPtrHolder, TCSLinkAggr<t_CAllocator> > m_pLast;
			inline_small TCSLinkAggr<t_CAllocator> &fp_GetFirst()
			{
				return m_First;
			}
			inline_small const TCSLinkAggr<t_CAllocator> &fp_GetFirst() const
			{
				return m_First;
			}
			inline_small TCSLinkAggr<t_CAllocator> *fp_GetLast()
			{
				return m_pLast;
			}
			inline_small const TCSLinkAggr<t_CAllocator> *fp_GetLast() const
			{
				return m_pLast;
			}
			inline_small void fp_SetLast(TCSLinkAggr<t_CAllocator> *_pLast)
			{				
				m_pLast = _pLast;
			}
			static inline_small bint fp_HasLast()
			{
				return true;
			}
		};

		template <typename t_CAllocator>
		class TCSLinkListData
		{
		public:
#ifndef DMibNoAggregateConstexpr
			constexpr TCSLinkListData(EAggregateInitialization _Init)
				: m_First(_Init)
			{
			}
			inline_always TCSLinkListData()
			{
			}
#endif
			TCSLinkAggr<t_CAllocator> m_First;
			inline_small TCSLinkAggr<t_CAllocator> &fp_GetFirst()
			{
				return m_First;
			}
			inline_small TCSLinkAggr<t_CAllocator> *fp_GetLast()
			{
				return nullptr;
			}
			inline_small const TCSLinkAggr<t_CAllocator> &fp_GetFirst() const
			{
				return m_First;
			}
			inline_small const TCSLinkAggr<t_CAllocator> *fp_GetLast() const
			{
				return nullptr;
			}
			inline_small void fp_SetLast(TCSLinkAggr<t_CAllocator> *_pLast)
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
			}
			inline_always TCSLinkListAggregate()
			{
#ifdef DMibDebuggerHelpers
				(void)&ms_OffsetTCSLinkListAggregate;
#endif
			}
#endif
			t_CListData m_Data;
			
#ifdef DMibDebuggerHelpers
			static mint ms_OffsetTCSLinkListAggregate;
#endif

			static inline_small TCSLinkAggr<t_CAllocator> *fp_LinkFromMember(t_CData *_pMember)
			{				
				return ((TCSLinkAggr<t_CAllocator> *)(((uint8 *)_pMember) + COffset::mc_Offset));
			}

			static inline_small t_CData *fp_MemberFromLink(TCSLinkAggr<t_CAllocator> *_pLink)
			{				
				return ((t_CData *)(((uint8 *)_pLink) - COffset::mc_Offset));
			}

			static inline_small const TCSLinkAggr<t_CAllocator> *fp_LinkFromMember(const t_CData *_pMember)
			{				
				return ((const TCSLinkAggr<t_CAllocator> *)(((uint8 *)_pMember) + COffset::mc_Offset));
			}

			static inline_small const t_CData *fp_MemberFromLink(const TCSLinkAggr<t_CAllocator> *_pLink)
			{				
				return ((const t_CData *)(((uint8 *)_pLink) - COffset::mc_Offset));
			}

			void f_Construct()
			{
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
				TCSLinkAggr<t_CAllocator> * pToRemove = fp_LinkFromMember(_pToRemove);
				TCSLinkAggr<t_CAllocator> *pCurrent = &m_Data.fp_GetFirst();
				while (pCurrent)
				{
					TCSLinkAggr<t_CAllocator> *pNext = pCurrent->fp_GetNext();
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
				TCSLinkAggr<t_CAllocator> * pToRemove = fp_LinkFromMember(_pToRemove);
				TCSLinkAggr<t_CAllocator> * pPrev = fp_LinkFromMember(_pPrev);
				
				pToRemove->fp_UnsafeUnlink(pPrev);
			}

			static t_CData * fs_GetNext(t_CData *_pData)
			{
				TCSLinkAggr<t_CAllocator> * pData = fp_LinkFromMember(_pData);
				auto pNext = pData->fp_GetNext();
				if (pNext)
					return fp_MemberFromLink(pNext);
				return nullptr;
			}

			static void fs_Unlink(t_CData *_pToRemove, t_CData *_pPrev)
			{
				TCSLinkAggr<t_CAllocator> * pToRemove = fp_LinkFromMember(_pToRemove);
				TCSLinkAggr<t_CAllocator> * pPrev = fp_LinkFromMember(pPrev);
				
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
					TCSLinkAggr<t_CAllocator> *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
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
					TCSLinkAggr<t_CAllocator> *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
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
					TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
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
				TCSLinkAggr<t_CAllocator> *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
				if (pCurrent)
				{
					while (t_CSortClass::fs_Compare(fp_MemberFromLink(pCurrent), _pToInsert) > 0)
					{
						pCurrent = pCurrent->fp_GetNext();
					}

					TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
					pToLink->fp_Link(pCurrent);
				}

				if (pCurrent)
				{
					TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
					pToLink->fp_Link(pCurrent);
				}
				else
				{
					TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
					pToLink->fp_Link(&m_Data.fp_GetFirst());
				}
			}
			
			// InsertTail
			inline_small void f_InsertLast(t_CData *_pToInsert)
			{
				if (m_Data.fp_HasLast())
				{
					TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
					pToLink->fp_Link(m_Data.fp_GetLast());
					m_Data.fp_SetLast(pToLink);
				}
				else
				{
					TCSLinkAggr<t_CAllocator> *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
					if (pCurrent)
					{
						while (pCurrent->fp_GetNext())
						{
							pCurrent = pCurrent->fp_GetNext();
						}

						TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
						pToLink->fp_Link(pCurrent);
					}
					else
					{
						TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
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
				TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);

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
				TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
				TCSLinkAggr<t_CAllocator> *pToLinkAfter = fp_LinkFromMember(_pToInsertAfter);

				pToLink->fp_Link(&pToLinkAfter);

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
					TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
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
					TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
					pToLink->fp_UnsafeLink(m_Data.fp_GetLast());
					m_Data.fp_SetLast(pToLink);
				}
				else
				{
					TCSLinkAggr<t_CAllocator> *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
					if (pCurrent)
					{
						while (pCurrent->fp_GetNext())
						{
							pCurrent = pCurrent->fp_GetNext();
						}

						TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
						pToLink->fp_UnsafeLink(pCurrent);
					}
					else
					{
						TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
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
				TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);

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
				TCSLinkAggr<t_CAllocator> *pToLink = fp_LinkFromMember(_pToInsert);
				TCSLinkAggr<t_CAllocator> *pToLinkAfter = fp_LinkFromMember(_pToInsertAfter);

				pToLink->fp_UnsafeLink(&pToLinkAfter);

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
				TCSLinkAggr<t_CAllocator> *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
				m_Data.fp_GetFirst().fp_SetNext(nullptr);
				if (pCurrent)
				{
					if (m_Data.fp_HasLast())
					{
						m_Data.fp_SetLast(pCurrent);
					}
					
					while (pCurrent)
					{
						TCSLinkAggr<t_CAllocator> *pTemp = pCurrent;
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

					TCSLinkAggr<t_CAllocator> *pCurrent = m_Data.fp_GetFirst().fp_GetNext();
					TCSLinkAggr<t_CAllocator> *pLinkAfter = &m_Data.fp_GetFirst();
					TCSLinkAggr<t_CAllocator> *pLast = &m_Data.fp_GetFirst();
					m_Data.fp_GetFirst().fp_SetNext(nullptr);
					aint NumSorted = 0;

					while (pCurrent)
					{
						// Add first member

						++NumSorted;
						TCSLinkAggr<t_CAllocator> *pTemp = pCurrent->fp_GetNext();
						pCurrent->fp_UnsafeLink(pLinkAfter);
						pLast = pCurrent;
						pCurrent = pTemp;

						if (!pCurrent)
							break;

						for (aint i = 1; i < SortSize; ++i)
						{
							aint NumChecked = i;
							++NumSorted;

							TCSLinkAggr<t_CAllocator> *pTemp = pLinkAfter->fp_GetNext();
							TCSLinkAggr<t_CAllocator> *pTempAfter = pLinkAfter;

							while (NumChecked)
							{
								if (t_CSortClass::fs_Compare(_pContext,fp_MemberFromLink(pCurrent),fp_MemberFromLink(pTemp)) <= 0)
									break;
								--NumChecked;
								pTempAfter = pTemp;
								pTemp = pTemp->fp_GetNext();
							}

							TCSLinkAggr<t_CAllocator> *pTempNext = pCurrent->fp_GetNext();
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
					TCSLinkAggr<t_CAllocator> *pFirst = m_Data.fp_GetFirst().fp_GetNext();
					TCSLinkAggr<t_CAllocator> *pLast = &m_Data.fp_GetFirst();
					
					aint MergesDone = 0;  // count number of merges we do in this pass
					
					while (pFirst) 
					{
						MergesDone++;  // there exists a merge to be done
						// step `MergeSize' places along from pFirst
						TCSLinkAggr<t_CAllocator> *pSecond = pFirst;
						TCSLinkAggr<t_CAllocator> *pLastFirst = pFirst;
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

				TCSLinkAggr<t_CAllocator> *Bucket[t_BucketSize];

				// Reset buckets
				for (aint i = 0; i < t_BucketSize; ++i)
				{
					Bucket[i] = nullptr;
				}

				// Put list in first bucket

				for(aint Place = 0; Place < _NumPlaces; ++Place)
				{					
					// Remove last link
					TCSLinkAggr<t_CAllocator> *pCurrent = m_Data.fp_GetFirst().fp_GetNext();

					while (pCurrent)
					{
						aint Index = t_CSortClass::fs_GetIndex(_pContext, Place, fp_MemberFromLink(pCurrent));
						TCSLinkAggr<t_CAllocator> **pBucket = &Bucket[Index];

						TCSLinkAggr<t_CAllocator> *pNext = pCurrent->fp_GetNext();

						pCurrent->fp_SetNext((*pBucket));
						(*pBucket) = pCurrent;

						pCurrent = pNext;
					}

					
					// Rebuild list

					m_Data.fp_GetFirst().fp_SetNext(nullptr);
					TCSLinkAggr<t_CAllocator> *pLast = &m_Data.fp_GetFirst();
					
					for (aint i = 0; i < t_BucketSize; ++i)
					{
						if (Bucket[i])
						{
							TCSLinkAggr<t_CAllocator> *pTemp = Bucket[i];
							Bucket[i] = nullptr;
							TCSLinkAggr<t_CAllocator> *pLinkAfter = pLast;
							pLast = pTemp;

							while (pTemp)
							{
								TCSLinkAggr<t_CAllocator> *pTempNext = pTemp->fp_GetNext();
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
				TCSLinkAggr<t_CAllocator> *m_pCurrent;

#ifdef DMibDebuggerHelpers
				static TCSLinkListAggregate* ms_pList;
#endif
				CIterator()
				{
					m_pCurrent = nullptr;
#ifdef DMibDebuggerHelpers
					(void)&ms_pList;
#endif
				}

				CIterator(const TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> &_List)
				{
					m_pCurrent = _List.m_Data.fp_GetFirst().fp_GetNext();
#ifdef DMibDebuggerHelpers
					(void)&ms_pList;
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
				const TCSLinkAggr<t_CAllocator> *m_pCurrent;

#ifdef DMibDebuggerHelpers
				static TCSLinkListAggregate* ms_pList;
#endif
				CIteratorConst()
				{
					m_pCurrent = nullptr;
#ifdef DMibDebuggerHelpers
					(void)&ms_pList;
#endif
				}

				CIteratorConst(const TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> &_List)
				{
					m_pCurrent = _List.m_Data.fp_GetFirst().fp_GetNext();
#ifdef DMibDebuggerHelpers
					(void)&ms_pList;
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
		assure_used mint TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::ms_OffsetTCSLinkListAggregate = COffset::mc_Offset;

		template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
		assure_used TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>* 
		TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::ms_pList = nullptr;

		template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
		assure_used TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>* 
		TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::ms_pList = nullptr;
#endif

		template <typename t_CData, typename t_CTranslator, typename t_CListData, bint t_bAutoDelete, typename t_CAllocator>
		class TCSLinkList : public TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>
		{
		private:
			DMibClassNoCopyAllowed(TCSLinkList);
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

#		define DMibListLinkS_Trans(_Class, _Member) \
				class CSLinkTranslator##_Member \
				{\
				public:\
					template <typename t_CClass> \
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


#		define DMibListLinkS_Link(_Class, _Member) \
				DMibListLinkS_Trans(_Class, _Member) \
				NMib::NIntrusive::TCSLink<NMib::NMem::CDefaultAllocator> _Member;

#		define DMibListLinkSA_Link(_Class, _Member) \
				DMibListLinkS_Trans(_Class, _Member) \
				NMib::NIntrusive::TCSLinkAggr<NMib::NMem::CDefaultAllocator> _Member;


#		define DMibListLinkS_Member(_Class, _Member) \
				NMib::NIntrusive::TCSLink<NMib::NMem::CDefaultAllocator> _Member;
#		define DMibListLinkSA_Member(_Class, _Member) \
				NMib::NIntrusive::TCSLinkAggr<NMib::NMem::CDefaultAllocator> _Member;

#		define DMibListLinkS_LinkType() \
				NMib::NIntrusive::TCSLink<NMib::NMem::CDefaultAllocator>
#		define DMibListLinkSA_LinkType() \
				NMib::NIntrusive::TCSLinkAggr<NMib::NMem::CDefaultAllocator>

		// Link with pLast

#		define DMibListLinkS_Iter(_Class, _Member) DMibListLinkS_List(_Class, _Member)::CIterator
#		define DMibListLinkS_List(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<NMib::NMem::CDefaultAllocator>, false, NMib::NMem::CDefaultAllocator>
#		define DMibListLinkS_ListAutoDelete(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<NMib::NMem::CDefaultAllocator>, true, NMib::NMem::CDefaultAllocator>

#		define DMibListLinkSA_List(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<NMib::NMem::CDefaultAllocator>, false, NMib::NMem::CDefaultAllocator>
#		define DMibListLinkSA_ListAutoDelete(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<NMib::NMem::CDefaultAllocator>, true, NMib::NMem::CDefaultAllocator>

#		define DMibListLinkS_Iter_FromTemplate(_Class, _Member) DMibListLinkS_List_Fromtemplate(_Class, _Member)::CIterator
#		define DMibListLinkS_List_Fromtemplate(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<NMib::NMem::CDefaultAllocator>, false, NMib::NMem::CDefaultAllocator>
#		define DMibListLinkS_ListAutoDelete_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<NMib::NMem::CDefaultAllocator>, true, NMib::NMem::CDefaultAllocator>

#		define DMibListLinkSA_List_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<NMib::NMem::CDefaultAllocator>, false, NMib::NMem::CDefaultAllocator>
#		define DMibListLinkSA_ListAutoDelete_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<NMib::NMem::CDefaultAllocator>, true, NMib::NMem::CDefaultAllocator>
					
		// Link Without pLast

#		define DMibListLinkS_IterNoLastPtr(_Class, _Member) DMibListLinkS_ListNoLastPtr(_Class, _Member)::CIterator
#		define DMibListLinkS_ListNoLastPtr(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<NMib::NMem::CDefaultAllocator>, false, NMib::NMem::CDefaultAllocator>
#		define DMibListLinkS_ListAutoDeleteNoLastPtr(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<NMib::NMem::CDefaultAllocator>, true, NMib::NMem::CDefaultAllocator>

#		define DMibListLinkSA_ListNoLastPtr(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<NMib::NMem::CDefaultAllocator>, false, NMib::NMem::CDefaultAllocator>
#		define DMibListLinkSA_ListAutoDeleteNoLastPtr(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<NMib::NMem::CDefaultAllocator>, true, NMib::NMem::CDefaultAllocator>

#		define DMibListLinkS_IterNoLastPtr_FromTemplate(_Class, _Member) DMibListLinkS_ListNoLastPtr_FromTemplate(_Class, _Member)::CIterator
#		define DMibListLinkS_ListNoLastPtr_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<NMib::NMem::CDefaultAllocator>, false, NMib::NMem::CDefaultAllocator>
#		define DMibListLinkS_ListAutoDeleteNoLastPtr_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<NMib::NMem::CDefaultAllocator>, true, NMib::NMem::CDefaultAllocator>

#		define DMibListLinkSA_ListNoLastPtr_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<NMib::NMem::CDefaultAllocator>, false, NMib::NMem::CDefaultAllocator>
#		define DMibListLinkSA_ListAutoDeleteNoLastPtr_FromTemplate(_Class, _Member) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<NMib::NMem::CDefaultAllocator>, true, NMib::NMem::CDefaultAllocator>
					
#		ifndef DMibPNoShortCuts
#			define DLinkS_Link(_Class, _Member) DMibListLinkS_Link(_Class, _Member)
#			define DLinkS_Trans(_Class, _Member) DMibListLinkS_Trans(_Class, _Member)
#			define DLinkS_Member(_Class, _Member) DMibListLinkS_Member(_Class, _Member)
#			define DLinkS_LinkType(_Class, _Member) DMibListLinkS_LinkType(_Class, _Member)
#			define DLinkS_Iter(_Class, _Member) DMibListLinkS_Iter(_Class, _Member)
#			define DLinkS_List(_Class, _Member) DMibListLinkS_List(_Class, _Member)
#			define DLinkS_ListAutoDelete(_Class, _Member) DMibListLinkS_ListAutoDelete(_Class, _Member)
#			define DLinkS_IterNoLastPtr(_Class, _Member) DMibListLinkS_IterNoLastPtr(_Class, _Member)
#			define DLinkS_ListNoLastPtr(_Class, _Member) DMibListLinkS_ListNoLastPtr(_Class, _Member)
#			define DLinkS_ListAutoDeleteNoLastPtr(_Class, _Member) DMibListLinkS_ListAutoDeleteNoLastPtr(_Class, _Member)
#			define DLinkSA_Link(_Class, _Member) DMibListLinkSA_Link(_Class, _Member)
#			define DLinkSA_Member(_Class, _Member) DMibListLinkSA_Member(_Class, _Member)
#			define DLinkSA_LinkType(_Class, _Member) DMibListLinkSA_LinkType(_Class, _Member)
#			define DLinkSA_List(_Class, _Member) DMibListLinkSA_List(_Class, _Member)
#			define DLinkSA_ListAutoDelete(_Class, _Member) DMibListLinkSA_ListAutoDelete(_Class, _Member)
#			define DLinkSA_ListNoLastPtr(_Class, _Member) DMibListLinkSA_ListNoLastPtr(_Class, _Member)
#			define DLinkSA_ListAutoDeleteNoLastPtr(_Class, _Member) DMibListLinkSA_ListAutoDeleteNoLastPtr(_Class, _Member)
#		endif

        /***************************************************************************************************\
        |¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
        | Macros for variable Pointer Holder																|
        |___________________________________________________________________________________________________|
        \***************************************************************************************************/


#		define DMibListLinkAllocatorS_Member(_Member, _Allocator) \
				NMib::NIntrusive::TCSLink<_Allocator> _Member;
#		define DMibListLinkAllocatorSA_Member(_Member, _Allocator) \
				NMib::NIntrusive::TCSLinkAggr<_Allocator> _Member;

#		define DMibListLinkAllocatorS_Link(_Class, _Member, _Allocator) \
				DMibListLinkS_Trans(_Class, _Member) \
				DMibListLinkAllocatorS_Member(_Member, _Allocator)

// List link: | Single | Aggregate | Pointer Holder
#		define DMibListLinkAllocatorSA_Link(_Class, _Member, _Allocator) \
				DMibListLinkS_Trans(_Class, _Member) \
				DMibListLinkAllocatorSA_Member(_Member, _Allocator)


#		define DMibListLinkAllocatorS_LinkType(_Allocator) \
				NMib::NIntrusive::TCSLink<_Allocator>
#		define DMibListLinkAllocatorSA_LinkType(_Allocator) \
				NMib::NIntrusive::TCSLinkAggr<_Allocator>

		// Link with pLast

#		define DMibListLinkAllocatorS_Iter(_Class, _Member, _Allocator) DMibListLinkAllocatorS_List(_Class, _Member, _Allocator)::CIterator
#		define DMibListLinkAllocatorS_List(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<_Allocator>, false, _Allocator>
#		define DMibListLinkAllocatorS_ListAutoDelete(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<_Allocator>, true, _Allocator>

#		define DMibListLinkAllocatorSA_List(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<_Allocator>, false, _Allocator>
#		define DMibListLinkAllocatorSA_ListAutoDelete(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<_Allocator>, true, _Allocator>

#		define DMibListLinkAllocatorS_Iter_FromTemplate(_Class, _Member, _Allocator) DMibListLinkAllocatorS_List_Fromtemplate(_Class, _Member, _Allocator)::CIterator
#		define DMibListLinkAllocatorS_List_Fromtemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<_Allocator>, false, _Allocator>
#		define DMibListLinkAllocatorS_ListAutoDelete_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<_Allocator>, true, _Allocator>

#		define DMibListLinkAllocatorSA_List_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<_Allocator>, false, _Allocator>
#		define DMibListLinkAllocatorSA_ListAutoDelete_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData_Last<_Allocator>, true, _Allocator>
					
		// Link Without pLast

#		define DMibListLinkAllocatorS_IterNoLastPtr(_Class, _Member, _Allocator) DMibListLinkAllocatorS_ListNoLastPtr(_Class, _Member, _Allocator)::CIterator
#		define DMibListLinkAllocatorS_ListNoLastPtr(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<_Allocator>, false, _Allocator>
#		define DMibListLinkAllocatorS_ListAutoDeleteNoLastPtr(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<_Allocator>, true, _Allocator>

#		define DMibListLinkAllocatorSA_ListNoLastPtr(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<_Allocator>, false, _Allocator>
#		define DMibListLinkAllocatorSA_ListAutoDeleteNoLastPtr(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<_Allocator>, true, _Allocator>

#		define DMibListLinkAllocatorS_IterNoLastPtr_FromTemplate(_Class, _Member, _Allocator) DMibListLinkAllocatorS_ListNoLastPtr_FromTemplate(_Class, _Member, _Allocator)::CIterator
#		define DMibListLinkAllocatorS_ListNoLastPtr_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<_Allocator>, false, _Allocator>
#		define DMibListLinkAllocatorS_ListAutoDeleteNoLastPtr_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkList<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<_Allocator>, true, _Allocator>

#		define DMibListLinkAllocatorSA_ListNoLastPtr_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<_Allocator>, false, _Allocator>
#		define DMibListLinkAllocatorSA_ListAutoDeleteNoLastPtr_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCSLinkListAggregate<_Class, typename _Class::CSLinkTranslator##_Member, NMib::NIntrusive::TCSLinkListData<_Allocator>, true, _Allocator>
					
#		ifndef DMibPNoShortCuts
#			define DLinkAllocatorS_Link(_Class, _Member) DMibListLinkAllocatorS_Link(_Class, _Member)
#			define DLinkAllocatorS_Member(_Class, _Member) DMibListLinkAllocatorS_Member(_Class, _Member)
#			define DLinkAllocatorS_LinkType(_Class, _Member) DMibListLinkAllocatorS_LinkType(_Class, _Member)
#			define DLinkAllocatorS_Iter(_Class, _Member) DMibListLinkAllocatorS_Iter(_Class, _Member)
#			define DLinkAllocatorS_List(_Class, _Member) DMibListLinkAllocatorS_List(_Class, _Member)
#			define DLinkAllocatorS_ListAutoDelete(_Class, _Member) DMibListLinkAllocatorS_ListAutoDelete(_Class, _Member)
#			define DLinkAllocatorS_IterNoLastPtr(_Class, _Member) DMibListLinkAllocatorS_IterNoLastPtr(_Class, _Member)
#			define DLinkAllocatorS_ListNoLastPtr(_Class, _Member) DMibListLinkAllocatorS_ListNoLastPtr(_Class, _Member)
#			define DLinkAllocatorS_ListAutoDeleteNoLastPtr(_Class, _Member) DMibListLinkAllocatorS_ListAutoDeleteNoLastPtr(_Class, _Member)
#			define DLinkAllocatorSA_Link(_Class, _Member) DMibListLinkAllocatorSA_Link(_Class, _Member)
#			define DLinkAllocatorSA_Member(_Class, _Member) DMibListLinkAllocatorSA_Member(_Class, _Member)
#			define DLinkAllocatorSA_LinkType(_Class, _Member) DMibListLinkAllocatorSA_LinkType(_Class, _Member)
#			define DLinkAllocatorSA_List(_Class, _Member) DMibListLinkAllocatorSA_List(_Class, _Member)
#			define DLinkAllocatorSA_ListAutoDelete(_Class, _Member) DMibListLinkAllocatorSA_ListAutoDelete(_Class, _Member)
#			define DLinkAllocatorSA_ListNoLastPtr(_Class, _Member) DMibListLinkAllocatorSA_ListNoLastPtr(_Class, _Member)
#			define DLinkAllocatorSA_ListAutoDeleteNoLastPtr(_Class, _Member) DMibListLinkAllocatorSA_ListAutoDeleteNoLastPtr(_Class, _Member)
#		endif

	};	
};



