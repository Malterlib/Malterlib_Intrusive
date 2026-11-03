// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

namespace NMib::NIntrusive
{
	constexpr CSLinkAggr::CSLinkAggr(EAggregateInitialization _Init)
		: m_pNext(nullptr)
	{
	}

	inline CSLinkAggr::CSLinkAggr()
	{
	}

	inline void CSLinkAggr::f_Construct()
	{
#ifdef DMibEnableSafeCheck
		m_pNext = nullptr;
#endif
	}

	inline void CSLinkAggr::f_Destruct()
	{
		DMibSafeCheck(!m_pNext, "You have to take care of it. No auto delete");
	}

	inline void CSLinkAggr::f_Link(CSLinkAggr *_pLinkAfter)
	{
		DMibSafeCheck(!m_pNext, "Singly linked list cannot be automatically unlinked, you have to remove it from the list manually");
		m_pNext = _pLinkAfter->m_pNext;
		_pLinkAfter->m_pNext = this;
	}

	inline void CSLinkAggr::f_Unlink(CSLinkAggr *_pPrev)
	{
		DMibSafeCheck(_pPrev, "You better know what you are doing");
		_pPrev->m_pNext = m_pNext;
		m_pNext = nullptr;
	}

	inline void CSLinkAggr::f_UnsafeLink(CSLinkAggr *_pLinkAfter)
	{
		m_pNext = _pLinkAfter->m_pNext;
		_pLinkAfter->m_pNext = this;
	}

	inline void CSLinkAggr::f_UnsafeUnlink(CSLinkAggr *_pPrev)
	{
		_pPrev->m_pNext = m_pNext;
#ifdef DMibEnableSafeCheck
		m_pNext = nullptr;
#endif
	}

	inline CSLinkAggr *CSLinkAggr::f_GetNext() const
	{
		return m_pNext;
	}

	inline void CSLinkAggr::f_SetNext(CSLinkAggr *_pNext)
	{
		m_pNext = _pNext;
	}

	inline bool CSLinkAggr::f_IsInList() const
	{
		return m_pNext != nullptr;
	}

	inline CSLink::CSLink()
	{
		CSLinkAggr::f_Construct();
	}

	inline CSLink::~CSLink()
	{
		CSLinkAggr::f_Destruct();
	}

	constexpr CSLinkListData_Last::CSLinkListData_Last(EAggregateInitialization _Init)
		: m_First(_Init)
		, m_pLast(nullptr)
	{
	}

	inline CSLinkListData_Last::CSLinkListData_Last()
	{
	}

	inline CSLinkAggr &CSLinkListData_Last::f_GetFirst()
	{
		return m_First;
	}

	inline CSLinkAggr const &CSLinkListData_Last::f_GetFirst() const
	{
		return m_First;
	}

	inline CSLinkAggr *CSLinkListData_Last::f_GetLast()
	{
		return m_pLast;
	}

	inline CSLinkAggr const *CSLinkListData_Last::f_GetLast() const
	{
		return m_pLast;
	}

	inline void CSLinkListData_Last::f_SetLast(CSLinkAggr *_pLast)
	{
		m_pLast = _pLast;
	}

	inline bool CSLinkListData_Last::fs_HasLast()
	{
		return true;
	}

	constexpr CSLinkListData::CSLinkListData(EAggregateInitialization _Init)
		: m_First(_Init)
	{
	}

	inline CSLinkListData::CSLinkListData()
	{
	}

	inline CSLinkAggr &CSLinkListData::f_GetFirst()
	{
		return m_First;
	}

	inline CSLinkAggr *CSLinkListData::f_GetLast()
	{
		return nullptr;
	}

	inline CSLinkAggr const &CSLinkListData::f_GetFirst() const
	{
		return m_First;
	}

	inline CSLinkAggr const *CSLinkListData::f_GetLast() const
	{
		return nullptr;
	}

	inline void CSLinkListData::f_SetLast(CSLinkAggr *_pLast)
	{
	}

	inline bool CSLinkListData::fs_HasLast()
	{
		return false;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	constexpr TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::TCSLinkListAggregate(EAggregateInitialization _Init)
		: m_Data(_Init)
	{
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Construct()
	{
		m_Data.f_GetFirst().f_SetNext(nullptr);
		if (m_Data.fs_HasLast())
			m_Data.f_SetLast(&m_Data.f_GetFirst());
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Destruct()
	{
		if constexpr (t_bAutoDelete)
			fp_AutoDelete<t_bAutoDelete>();
		else
			f_Clear();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_ClearFast()
	{
		m_Data.f_GetFirst().f_SetNext(nullptr);
		if (m_Data.fs_HasLast())
			m_Data.f_SetLast(nullptr);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Clear()
	{
		while (m_Data.f_GetFirst().f_GetNext())
			m_Data.f_GetFirst().f_GetNext()->f_Unlink(&m_Data.f_GetFirst());
		if (m_Data.fs_HasLast())
			m_Data.f_SetLast(&m_Data.f_GetFirst());
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_DeleteAll()
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_DeleteAllDefiniteType()
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Remove(t_CData *_pToRemove)
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fs_UnsafeUnlink(t_CData *_pToRemove, t_CData *_pPrev)
	{
		CSLinkAggr *pToRemove = fsp_LinkFromMember(_pToRemove);
		CSLinkAggr *pPrev = fsp_LinkFromMember(_pPrev);

		pToRemove->f_UnsafeUnlink(pPrev);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	t_CData *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fs_GetNext(t_CData *_pData)
	{
		CSLinkAggr *pData = fsp_LinkFromMember(_pData);
		auto pNext = pData->f_GetNext();
		if (pNext)
			return fsp_MemberFromLink(pNext);
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fs_Unlink(t_CData *_pToRemove, t_CData *_pPrev)
	{
		CSLinkAggr *pToRemove = fsp_LinkFromMember(_pToRemove);
		CSLinkAggr *pPrev = fsp_LinkFromMember(pPrev);

		pToRemove->f_Unlink(pPrev);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_medium t_CData *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Pop()
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_medium t_CData *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafePop()
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Push(t_CData *_pData)
	{
		f_UnsafeInsertFirst(_pData);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Delete(t_CData *_pToDelete)
	{
		f_Remove(_pToDelete);
		fg_DeleteObject(t_CAllocator(), _pToDelete);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_GetFirst()
	{
		auto pNext = m_Data.f_GetFirst().f_GetNext();
		if (pNext)
			return fsp_MemberFromLink(pNext);
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_GetLast()
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_GetFirst() const
	{
		if (m_Data.f_GetFirst().f_GetNext())
			return fsp_MemberFromLink(m_Data.f_GetFirst().f_GetNext());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_GetLast() const
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small bool TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_IsEmpty() const
	{
		return (m_Data.f_GetFirst().f_GetNext() == nullptr);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Insert(t_CData *_pToInsert)
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Insert(t_CData &_ToInsert)
	{
		f_Insert(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	template <class tf_CSortClass>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertSorted(t_CData &_ToInsert)
	{
		f_InsertSorted<tf_CSortClass>(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertSorted(t_CData *_pToInsert, tf_FCompare &&_fCompare)
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertLast(t_CData *_pToInsert)
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertLast(t_CData &_ToInsert)
	{
		f_InsertLast(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertFirst(t_CData *_pToInsert)
	{
		CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);

		pToLink->f_Link(&m_Data.f_GetFirst());

		if (m_Data.fs_HasLast())
		{
			if (!pToLink->f_GetNext())
				m_Data.f_SetLast(pToLink);
		}
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertFirst(t_CData &_ToInsert)
	{
		f_InsertFirst(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter)
	{
		f_InsertAfter(&_ToInsert, _pToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter)
	{
		f_InsertAfter(_pToInsert, &_ToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_InsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter)
	{
		f_InsertAfter(&_ToInsert, &_ToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsert(t_CData *_pToInsert)
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsert(t_CData &_ToInsert)
	{
		f_UnsafeInsert(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertLast(t_CData *_pToInsert)
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertLast(t_CData &_ToInsert)
	{
		f_UnsafeInsertLast(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertFirst(t_CData *_pToInsert)
	{
		CSLinkAggr *pToLink = fsp_LinkFromMember(_pToInsert);

		pToLink->f_UnsafeLink(&m_Data.f_GetFirst());

		if (m_Data.fs_HasLast())
		{
			if (!pToLink->f_GetNext())
				m_Data.f_SetLast(pToLink);
		}
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertFirst(t_CData &_ToInsert)
	{
		f_UnsafeInsertFirst(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter)
	{
		f_UnsafeInsertAfter(&_ToInsert, _pToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter)
	{
		f_UnsafeInsertAfter(_pToInsert, &_ToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter)
	{
		f_UnsafeInsertAfter(&_ToInsert, &_ToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_UnsafePush(t_CData *_pData)
	{
		f_UnsafeInsertFirst(_pData);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Reverse()
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
	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	template <class tf_FCompare>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_MergeSort(tf_FCompare &&_fCompare, aint _InsertionBits)
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

				for (aint iItem = 1; iItem < SortSize; ++iItem)
				{
					aint NumChecked = iItem;
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small aint TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CBucketCallbackSort::fs_GetIndex(void *_pContext, aint _Place, void *_pItem)
	{
		return ((CBucketCallbackSort *)_pContext)->m_fSortFunction(((CBucketCallbackSort *)_pContext)->m_pContext, _Place, _pItem);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	template <aint tf_BucketSize>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_BucketSortCallback(aint _NumPlaces, FBucketGetIndex *_fSortFunction, void *_pContext)
	{
		CBucketCallbackSort SortContext;
		SortContext.m_pContext = _pContext;
		SortContext.m_fSortFunction = _fSortFunction;
		f_BucketSort<CBucketCallbackSort, tf_BucketSize>(_NumPlaces, &SortContext);
	}

	/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
	|	Function:			Sorts the linked list								|
	|																			|
	|	Template params:														|
	|		tf_CSortClass:	The class that implements the index funtion.		|
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
	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	template <class tf_CSortClass, aint tf_BucketSize>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_BucketSort(aint _NumPlaces, void *_pContext)
	{
		if (f_IsEmpty())
			return;

		CSLinkAggr *Bucket[tf_BucketSize];

		// Reset buckets
		for (aint iBucket = 0; iBucket < tf_BucketSize; ++iBucket)
			Bucket[iBucket] = nullptr;

		// Put list in first bucket

		for (aint Place = 0; Place < _NumPlaces; ++Place)
		{
			// Remove last link
			CSLinkAggr *pCurrent = m_Data.f_GetFirst().f_GetNext();

			while (pCurrent)
			{
				aint Index = tf_CSortClass::fs_GetIndex(_pContext, Place, fsp_MemberFromLink(pCurrent));
				CSLinkAggr **pBucket = &Bucket[Index];

				CSLinkAggr *pNext = pCurrent->f_GetNext();

				pCurrent->f_SetNext((*pBucket));
				(*pBucket) = pCurrent;

				pCurrent = pNext;
			}

			// Rebuild list

			m_Data.f_GetFirst().f_SetNext(nullptr);
			CSLinkAggr *pLast = &m_Data.f_GetFirst();

			for (aint iBucket = 0; iBucket < tf_BucketSize; ++iBucket)
			{
				if (Bucket[iBucket])
				{
					CSLinkAggr *pTemp = Bucket[iBucket];
					Bucket[iBucket] = nullptr;
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Sort(tf_FCompare &&_fCompare)
	{
		f_MergeSort(_fCompare);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::CIterator()
	{
		m_pCurrent = nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::CIterator
		(
			TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List
		)
	{
		m_pCurrent = _List.m_Data.f_GetFirst().f_GetNext();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	auto TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::f_GetIterator() const -> CIterator
	{
		CIterator Iterator;
		Iterator.m_pCurrent = m_pCurrent;
		return Iterator;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	auto TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>
		::CIterator::operator = (TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List)
		-> CIterator &
	{
		m_pCurrent = _List.m_Data.f_GetFirst().f_GetNext();
		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::f_GetCurrent()
	{
		if (m_pCurrent)
			return TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fsp_MemberFromLink(m_pCurrent);
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::operator t_CData * ()
	{
		return f_GetCurrent();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::operator -> ()
	{
		return f_GetCurrent();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIterator::operator ++ ()
	{
		if (m_pCurrent)
			m_pCurrent = m_pCurrent->f_GetNext();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	auto TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_GetIterator() -> CIterator
	{
		return CIterator(*this);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::CIteratorConst()
	{
		m_pCurrent = nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::CIteratorConst
		(
			TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List
		)
	{
		m_pCurrent = _List.m_Data.f_GetFirst().f_GetNext();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	auto TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::f_GetIterator() const -> CIteratorConst
	{
		CIteratorConst Iterator;
		Iterator.m_pCurrent = m_pCurrent;
		return Iterator;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	auto TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>
		::CIteratorConst::operator = (TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List)
		-> CIteratorConst &
	{
		m_pCurrent = _List.m_Data.f_GetFirst().f_GetNext();
		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::f_GetCurrent()
	{
		if (m_pCurrent)
			return TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fsp_MemberFromLink(m_pCurrent);
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator t_CData const * ()
	{
		return f_GetCurrent();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator -> ()
	{
		return f_GetCurrent();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator ++ ()
	{
		if (m_pCurrent)
			m_pCurrent = m_pCurrent->f_GetNext();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	auto TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_GetIterator() const -> CIteratorConst
	{
		return CIteratorConst(*this);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	umint TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_GetLen() const
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

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small CSLinkAggr *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fsp_LinkFromMember(t_CData *_pMember)
	{
		return ((CSLinkAggr *)(((uint8 *)_pMember) + COffset::mc_Offset));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fsp_MemberFromLink(CSLinkAggr *_pLink)
	{
		return ((t_CData *)(((uint8 *)_pLink) - COffset::mc_Offset));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small CSLinkAggr const *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fsp_LinkFromMember(t_CData const *_pMember)
	{
		return ((CSLinkAggr const *)(((uint8 *)_pMember) + COffset::mc_Offset));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fsp_MemberFromLink(CSLinkAggr const *_pLink)
	{
		return ((t_CData const *)(((uint8 *)_pLink) - COffset::mc_Offset));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	template <bool tf_bDoDelete>
	TCEnableIf<tf_bDoDelete> TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fp_AutoDelete()
	{
		f_DeleteAll();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	template <bool tf_bDoDelete>
	TCEnableIf<!tf_bDoDelete> TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::fp_AutoDelete()
	{
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	TCSLinkList<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::TCSLinkList()
	{
		TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Construct();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	TCSLinkList<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::~TCSLinkList()
	{
		TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>::f_Destruct();
	}
}
