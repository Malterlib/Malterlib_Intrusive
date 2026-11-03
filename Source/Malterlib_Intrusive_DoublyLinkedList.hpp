// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

namespace NMib::NIntrusive
{
	constexpr CDLinkAggregateListNoPrevPtrList::CDLinkAggregateListNoPrevPtrList(EAggregateInitialization _Init)
		: m_NextPtr{0}
	{
	}

	inline CDLinkAggregateListNoPrevPtrList::CDLinkAggregateListNoPrevPtrList()
	{
	}

	inline void CDLinkAggregateListNoPrevPtrList::f_SetNextList(CDLinkAggregateListNoPrevPtrList *_pNext)
	{
		fp_Internal_SetNextList(_pNext);
	}

	inline void CDLinkAggregateListNoPrevPtrList::f_SetNextInitListLink(CDLinkAggregateListNoPrevPtrList *_pNext)
	{
		fp_Internal_SetNextInitListLink(_pNext);
	}

	inline void CDLinkAggregateListNoPrevPtrList::f_ConstructList()
	{
		f_SetNextInitListLink(nullptr);
	}

	inline bool CDLinkAggregateListNoPrevPtrList::f_IsListLink() const
	{
		return fp_Internal_IsListLink();
	}

	inline CDLinkAggregateListNoPrevPtrList *CDLinkAggregateListNoPrevPtrList::f_GetNextList()
	{
		return (CDLinkAggregateListNoPrevPtrList *)fp_Internal_GetNextList();
	}

	inline void CDLinkAggregateListNoPrevPtrList::f_SetPrevInitListLink(CDLinkAggregateListNoPrevPtrList *_pPrev)
	{
	}

	inline CDLinkAggregateListNoPrevPtrList const *CDLinkAggregateListNoPrevPtrList::f_GetNextList() const
	{
		return (CDLinkAggregateListNoPrevPtrList const *)fp_Internal_GetNextList();
	}

	inline CDLinkAggregateListNoPrevPtrList *CDLinkAggregateListNoPrevPtrList::f_GetNextNotList()
	{
		return (CDLinkAggregateListNoPrevPtrList *)fp_Internal_GetNextNotList();
	}

	inline CDLinkAggregateListNoPrevPtrList const *CDLinkAggregateListNoPrevPtrList::f_GetNextNotList() const
	{
		return (CDLinkAggregateListNoPrevPtrList const *)fp_Internal_GetNextNotList();
	}

	inline bool CDLinkAggregateListNoPrevPtrList::f_IsValid() const
	{
		if (f_IsListLink())
		{
			auto *pNext = f_GetNextList();
			if (pNext == this)
				return true;
			if (pNext->f_Upcast()->f_GetPrevPtr()->f_GetNextNotList() != this)
				return false;
		}

		return true;
	}

	inline CDLinkAggregateListNoPrevPtr *CDLinkAggregateListNoPrevPtrList::f_Upcast()
	{
		return static_cast<CDLinkAggregateListNoPrevPtr *>(this);
	}

	inline CDLinkAggregateListNoPrevPtr const *CDLinkAggregateListNoPrevPtrList::f_Upcast() const
	{
		return static_cast<CDLinkAggregateListNoPrevPtr const *>(this);
	}

	inline CDLinkAggregateListNoPrevPtrList *CDLinkAggregateListNoPrevPtrList::f_GetPrevList()
	{
		DMibFastCheck(f_IsListLink());
		auto pNext = f_GetNextList();
		if (pNext->f_IsListLink())
			return this;
		else
			return pNext->f_Upcast()->f_GetPrevPtr();
	}

	inline CDLinkAggregateListNoPrevPtrList const *CDLinkAggregateListNoPrevPtrList::f_GetPrevList() const
	{
		DMibFastCheck(f_IsListLink());
		auto pNext = f_GetNextList();
		if (pNext->f_IsListLink())
			return this;
		else
			return pNext->f_Upcast()->f_GetPrevPtr();
	}

	inline void CDLinkAggregateListNoPrevPtrList::f_TransferList(CDLinkAggregateListNoPrevPtrList *_pFirst, CDLinkAggregateListNoPrevPtrList *_pLast)
	{
		if (_pFirst->f_IsListLink())
		{
			// Empty list
			f_ConstructList();
		}
		else
		{
			f_SetNextInitListLink(_pFirst);
			_pFirst->f_Upcast()->f_SetPrevInit(_pLast);
			_pLast->f_Upcast()->f_SetNextInit(this);
		}
	}

	inline void CDLinkAggregateListNoPrevPtrList::fp_Internal_SetNextList(void *_pNext)
	{
		DMibFastCheck(fp_Internal_IsListLink());
		m_NextPtr = ((umint)_pNext | 1);
	}

	inline void CDLinkAggregateListNoPrevPtrList::fp_Internal_SetNextInitListLink(void *_pNext)
	{
		m_NextPtr = ((umint)_pNext | 1);
	}

	inline bool CDLinkAggregateListNoPrevPtrList::fp_Internal_IsListLink() const
	{
		return (m_NextPtr & 1) != 0;
	}

	inline void *CDLinkAggregateListNoPrevPtrList::fp_Internal_GetNextList() const
	{
		DMibFastCheck(fp_Internal_IsListLink());
		return (void *)(m_NextPtr & (~1));
	}

	inline void *CDLinkAggregateListNoPrevPtrList::fp_Internal_GetNextNotList() const
	{
		DMibFastCheck(!this->fp_Internal_IsListLink());
		return (void *)m_NextPtr;
	}

	constexpr CDLinkAggregateListNoPrevPtr::CDLinkAggregateListNoPrevPtr(EAggregateInitialization _Init)
		: CDLinkAggregateListNoPrevPtrList{_Init}
		, m_PrevPtr{0}
	{
	}

	inline CDLinkAggregateListNoPrevPtr::CDLinkAggregateListNoPrevPtr()
	{
	}

	inline void CDLinkAggregateListNoPrevPtr::f_Construct()
	{
		f_SetNextInit(nullptr);
#ifdef DMibEnableSafeCheck
		// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
		f_SetPrevInit(nullptr);
#endif
	}

	inline void CDLinkAggregateListNoPrevPtr::f_SetNextNotList(CDLinkAggregateListNoPrevPtrList *_pNext)
	{
		fp_Internal_SetNextNotList(_pNext);
	}

	inline void CDLinkAggregateListNoPrevPtr::f_SetNextInit(CDLinkAggregateListNoPrevPtrList *_pNext)
	{
		fp_Internal_SetNextInit(_pNext);
	}

	inline void CDLinkAggregateListNoPrevPtr::f_SetPrevNotList(CDLinkAggregateListNoPrevPtrList *_pPrev)
	{
		DMibSafeCheck(!f_IsListLink(), "");
		fp_Internal_SetPrev(_pPrev);
	}

	inline void CDLinkAggregateListNoPrevPtr::f_SetPrevInit(CDLinkAggregateListNoPrevPtrList *_pPrev)
	{
		//DMibSafeCheck(!f_IsListLink(), ""); // this could be uninitialized memory
		fp_Internal_SetPrevInit(_pPrev);
	}

	inline CDLinkAggregateListNoPrevPtr *CDLinkAggregateListNoPrevPtr::f_GetPrevPtr()
	{
		return ((CDLinkAggregateListNoPrevPtr *)fp_Internal_GetPrev());
	}

	inline CDLinkAggregateListNoPrevPtr const *CDLinkAggregateListNoPrevPtr::f_GetPrevPtr() const
	{
		return ((CDLinkAggregateListNoPrevPtr *)fp_Internal_GetPrev());
	}

	inline CDLinkAggregateListNoPrevPtrList *CDLinkAggregateListNoPrevPtr::f_GetPrevNotList()
	{
		DMibFastCheck(!f_IsListLink());
		auto pPrev = f_GetPrevPtr();
		auto pPrevNext = pPrev->f_GetNextNotList();
		if (pPrevNext->f_IsListLink())
			return pPrevNext;
		else
			return pPrev;
	}

	inline CDLinkAggregateListNoPrevPtr *CDLinkAggregateListNoPrevPtr::f_GetPrevNoList()
	{
		DMibFastCheck(!f_IsListLink());
		auto pPrev = f_GetPrevPtr();
		DMibFastCheck(!pPrev->f_GetNextNotList()->f_IsListLink());
		return pPrev;
	}

	inline CDLinkAggregateListNoPrevPtrList const *CDLinkAggregateListNoPrevPtr::f_GetPrevNotList() const
	{
		DMibFastCheck(!f_IsListLink());
		auto pPrev = f_GetPrevPtr();
		auto pPrevNext = pPrev->f_GetNextNotList();
		if (pPrevNext->f_IsListLink())
			return pPrevNext;
		else
			return pPrev;
	}

	inline CDLinkAggregateListNoPrevPtr const *CDLinkAggregateListNoPrevPtr::f_GetPrevNoList() const
	{
		DMibFastCheck(!f_IsListLink());
		auto pPrev = f_GetPrevPtr();
		DMibFastCheck(!pPrev->f_GetNextNotList()->f_IsListLink());
		return pPrev;
	}

	inline void CDLinkAggregateListNoPrevPtr::f_Link(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
	{
		DMibSafeCheck(!f_GetNextNotList(), "Must not be in list here");
		f_LinkNoUnlink(_pLinkAfter);
	}

	inline void CDLinkAggregateListNoPrevPtr::f_LinkFirst(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
	{
		DMibSafeCheck(!f_GetNextNotList(), "Must not be in list here");
		f_LinkNoUnlinkFirst(_pLinkAfter);
	}

	inline void CDLinkAggregateListNoPrevPtr::f_LinkLast(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
	{
		DMibSafeCheck(!f_GetNextNotList(), "Must not be in list here");
		f_LinkNoUnlinkLast(_pLinkAfter);
	}

	inline void CDLinkAggregateListNoPrevPtr::fs_UnlinkRange(CDLinkAggregateListNoPrevPtr *_pFirst, CDLinkAggregateListNoPrevPtr *_pLast)
	{
		auto *pFirst = _pFirst->f_GetPrevNotList();
		auto *pLast = _pLast->f_GetNextNotList();

		if (pFirst == pLast)
		{
			DMibSafeCheck(pFirst->f_IsListLink(), "Must be a list link");
			// We should be an empty list pointing to ourselves
			pFirst->f_SetNextInitListLink(pFirst);
		}
		else if (pFirst->f_IsListLink())
		{
			// The first pointer is a list link
			auto *pLastInList = pFirst->f_GetPrevList()->f_Upcast();
			pFirst->f_SetNextInitListLink(pLast);
			pLast->f_Upcast()->f_SetPrevInit(pLastInList);
		}
		else if (pLast->f_IsListLink())
		{
			// The last pointer is a list link
			auto *pFirstInList = pLast->f_GetNextList();
			pFirst->f_Upcast()->f_SetNextInit(pLast);
			pFirstInList->f_Upcast()->f_SetPrevInit(pFirst);
		}
		else
		{
			pFirst->f_Upcast()->f_SetNextInit(pLast);
			pLast->f_Upcast()->f_SetPrevInit(pFirst);
		}
	}

	inline void CDLinkAggregateListNoPrevPtr::fs_LinkRange(CDLinkAggregateListNoPrevPtr *_pFirst, CDLinkAggregateListNoPrevPtr *_pLast, CDLinkAggregateListNoPrevPtrList *_pInsertAfter)
	{
		DMibSafeCheck(!_pFirst->f_IsListLink(), "Cannot insert empty list");
		DMibSafeCheck(!_pLast->f_IsListLink(), "Cannot insert empty list");

		if (_pInsertAfter->f_IsListLink())
		{
			auto *pInsertAfterNext = _pInsertAfter->f_GetNextList();
			DMibSafeCheck(pInsertAfterNext != _pInsertAfter, "Cannot insert into empty list");
			auto *pLastInList = _pInsertAfter->f_GetPrevList();
			// Insert at beginning of list
			_pFirst->f_SetPrevInit(pLastInList);
			_pInsertAfter->f_SetNextInitListLink(_pFirst);

			_pLast->f_SetNextInit(pInsertAfterNext);
			pInsertAfterNext->f_Upcast()->f_SetPrevInit(_pLast);
		}
		else
		{
			auto *pInsertAfterNext = _pInsertAfter->f_GetNextNotList();
			DMibSafeCheck(pInsertAfterNext != _pInsertAfter, "Cannot insert into empty list");
			if (pInsertAfterNext->f_IsListLink())
			{
				// Insert at end of list
				auto *pFirstInList = pInsertAfterNext->f_GetNextList();

				_pFirst->f_SetPrevInit(_pInsertAfter);
				_pInsertAfter->f_Upcast()->f_SetNextInit(_pFirst);

				_pLast->f_SetNextInit(pInsertAfterNext);
				pFirstInList->f_Upcast()->f_SetPrevInit(_pLast);
			}
			else
			{
				// Middle of list
				_pFirst->f_SetPrevInit(_pInsertAfter);
				_pInsertAfter->f_Upcast()->f_SetNextInit(_pFirst);

				_pLast->f_SetNextInit(pInsertAfterNext);
				pInsertAfterNext->f_Upcast()->f_SetPrevInit(_pLast);
			}
		}
	}

	inline void CDLinkAggregateListNoPrevPtr::f_LinkNoUnlink(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
	{
		if (_pLinkAfter->f_IsListLink()) [[unlikely]]
		{
			auto *pAfterNext = _pLinkAfter->f_GetNextList();
			if (pAfterNext == _pLinkAfter)
			{
				// Empty list
				f_SetPrevInit(this);
				f_SetNextInit(_pLinkAfter);
				_pLinkAfter->f_SetNextList(this);
				return;
			}

			// First in list

			f_SetPrevInit(pAfterNext->f_Upcast()->f_GetPrevPtr());
			f_SetNextInit(pAfterNext);
			pAfterNext->f_Upcast()->f_SetPrevNotList(this);
			_pLinkAfter->f_SetNextList(this);
		}
		else
		{
			auto *pAfterNext = _pLinkAfter->f_GetNextNotList();
			if (pAfterNext->f_IsListLink()) [[unlikely]]
			{
				// Last in list
				f_SetPrevInit(_pLinkAfter);
				f_SetNextInit(pAfterNext);
				pAfterNext->f_GetNextList()->f_Upcast()->f_SetPrevNotList(this);
				_pLinkAfter->f_Upcast()->f_SetNextNotList(this);
			}
			else
			{
				f_SetPrevInit(_pLinkAfter);
				f_SetNextInit(pAfterNext);
				pAfterNext->f_Upcast()->f_SetPrevNotList(this);
				_pLinkAfter->f_Upcast()->f_SetNextNotList(this);
			}
		}
	}

	inline void CDLinkAggregateListNoPrevPtr::f_LinkNoUnlinkNotList(CDLinkAggregateListNoPrevPtr *_pLinkAfter)
	{
		DMibFastCheck(!_pLinkAfter->f_IsListLink());
		auto *pAfterNext = _pLinkAfter->f_GetNextNotList();
		DMibFastCheck(!pAfterNext->f_IsListLink());
		f_SetPrevInit(_pLinkAfter);
		f_SetNextInit(pAfterNext);
		pAfterNext->f_Upcast()->f_SetPrevNotList(this);
		_pLinkAfter->f_SetNextNotList(this);
	}

	inline void CDLinkAggregateListNoPrevPtr::f_LinkNoUnlinkFirst(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
	{
		DMibFastCheck(_pLinkAfter->f_IsListLink());

		auto *pAfterNext = _pLinkAfter->f_GetNextList();
		if (pAfterNext == _pLinkAfter)
		{
			// Empty list
			f_SetPrevInit(this);
			f_SetNextInit(_pLinkAfter);
			_pLinkAfter->f_SetNextList(this);
			return;
		}

		// First in list

		f_SetPrevInit(pAfterNext->f_Upcast()->f_GetPrevPtr());
		f_SetNextInit(pAfterNext);
		pAfterNext->f_Upcast()->f_SetPrevNotList(this);
		_pLinkAfter->f_SetNextList(this);
	}

	inline void CDLinkAggregateListNoPrevPtr::f_LinkNoUnlinkLast(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
	{
		if (_pLinkAfter->f_IsListLink()) [[unlikely]]
		{
			DMibFastCheck(_pLinkAfter->f_GetNextList() == _pLinkAfter);
			// Empty list
			f_SetPrevInit(this);
			f_SetNextInit(_pLinkAfter);
			_pLinkAfter->f_SetNextList(this);
			return;
		}
		else
		{
			auto *pAfterNext = _pLinkAfter->f_GetNextNotList();
			DMibFastCheck(pAfterNext->f_IsListLink());
			// Last in list
			f_SetPrevInit(_pLinkAfter);
			f_SetNextInit(pAfterNext);
			pAfterNext->f_GetNextList()->f_Upcast()->f_SetPrevNotList(this);
			_pLinkAfter->f_Upcast()->f_SetNextNotList(this);
		}
	}

	inline void CDLinkAggregateListNoPrevPtr::f_Unlink()
	{
		if (f_GetNextNotList())
		{
			fp_UnlinkInternal();
			f_SetNextInit(nullptr);
#ifdef DMibEnableSafeCheck
			// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
			f_SetPrevInit(nullptr);
#endif
		}
	}

	inline void CDLinkAggregateListNoPrevPtr::f_UnlinkFirst()
	{
		if (f_GetNextNotList())
		{
			fp_UnlinkFirstInternal();
			f_SetNextInit(nullptr);
#ifdef DMibEnableSafeCheck
			// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
			f_SetPrevInit(nullptr);
#endif
		}
	}

	inline void CDLinkAggregateListNoPrevPtr::f_UnsafeUnlink()
	{
		fp_UnlinkInternal();
	}

	inline void CDLinkAggregateListNoPrevPtr::f_UnsafeUnlinkFirst()
	{
		fp_UnlinkFirstInternal();
	}

	inline void CDLinkAggregateListNoPrevPtr::f_UnlinkLinked()
	{
		fp_UnlinkInternal();
		f_SetNextInit(nullptr);
#ifdef DMibEnableSafeCheck
		// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
		f_SetPrevInit(nullptr);
#endif
	}

	inline void CDLinkAggregateListNoPrevPtr::f_UnlinkFirstLinked()
	{
		fp_UnlinkFirstInternal();
		f_SetNextInit(nullptr);
#ifdef DMibEnableSafeCheck
		// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
		f_SetPrevInit(nullptr);
#endif
	}

	inline void CDLinkAggregateListNoPrevPtr::f_Destruct()
	{
		if (f_GetNextNotList())
		{
			fp_UnlinkInternal();
#ifdef DMibEnableSafeCheck
			f_SetNextInit(nullptr);
			f_SetPrevInit(nullptr);
#endif
		}
	}

	inline bool CDLinkAggregateListNoPrevPtr::f_IsAloneInList() const
	{
		return f_IsInList() && f_GetPrevPtr() == this;
	}

	inline bool CDLinkAggregateListNoPrevPtr::f_IsInList() const
	{
		return f_GetNextNotList() != nullptr;
	}

	inline void CDLinkAggregateListNoPrevPtr::fp_Internal_SetPrev(void *_pPrev)
	{
		m_PrevPtr = (umint)_pPrev;
	}

	inline void CDLinkAggregateListNoPrevPtr::fp_Internal_SetPrevInit(void *_pPrev)
	{
		m_PrevPtr = (umint)_pPrev;
	}

	inline void *CDLinkAggregateListNoPrevPtr::fp_Internal_GetPrev() const
	{
		return (void *)m_PrevPtr;
	}

	inline void CDLinkAggregateListNoPrevPtr::fp_Internal_SetNextNotList(void *_pNext)
	{
		DMibFastCheck(!this->fp_Internal_IsListLink());
		m_NextPtr = (umint)_pNext;
	}

	inline void CDLinkAggregateListNoPrevPtr::fp_Internal_SetNextInit(void *_pNext)
	{
		m_NextPtr = (umint)_pNext;
	}

	inline void CDLinkAggregateListNoPrevPtr::fp_UnlinkFirstInternal()
	{
		auto pPrevPtr = f_GetPrevPtr();
		auto pPrev = pPrevPtr->f_GetNextNotList();
		DMibFastCheck(pPrev->f_IsListLink());
		auto pNext = f_GetNextNotList();
		pPrev->f_SetNextList(pNext);
		if (!pNext->f_IsListLink()) [[likely]]
			pNext->f_Upcast()->f_SetPrevNotList(pPrevPtr);
		else
		{
			// Alone in list
			DMibFastCheck(pNext == pPrev);
		}
	}

	inline void CDLinkAggregateListNoPrevPtr::fp_UnlinkInternal()
	{
		auto pPrev = f_GetPrevNotList();
		auto pNext = f_GetNextNotList();
		if (pPrev->f_IsListLink()) [[unlikely]]
		{
			pPrev->f_SetNextList(pNext);
			if (!pNext->f_IsListLink()) [[likely]]
				pNext->f_Upcast()->f_SetPrevNotList(f_GetPrevPtr());
			else
			{
				// Alone in list
				DMibFastCheck(pNext == pPrev);
			}
		}
		else
		{
			pPrev->f_Upcast()->f_SetNextNotList(pNext);

			if (pNext->f_IsListLink()) [[unlikely]] // Last in list
				pNext->f_GetNextList()->f_Upcast()->f_SetPrevNotList(pPrev);
			else // Between two usual blocks
				pNext->f_Upcast()->f_SetPrevNotList(pPrev);
		}
	}

	constexpr CDLinkAggregate::CDLinkAggregate(EAggregateInitialization _Init)
		: m_pNextPtr{nullptr}
		, m_pPrevPtr{nullptr}
	{
	}

	inline CDLinkAggregate::CDLinkAggregate()
	{
	}

	inline bool CDLinkAggregate::f_IsValid() const
	{
		return true;
	}

	inline void CDLinkAggregate::f_Link(CDLinkAggregate *_pLinkAfter)
	{
		DMibSafeCheck(!fp_GetNext(), "Must not be in list here");

		f_SetPrevInit(_pLinkAfter);
		fp_SetNext(_pLinkAfter->fp_GetNext());
		fp_GetNext()->fp_SetPrev(this);
		f_GetPrev()->fp_SetNext(this);
	}

	inline void CDLinkAggregate::f_LinkFirst(CDLinkAggregate *_pLinkAfter)
	{
		f_Link(_pLinkAfter);
	}

	inline void CDLinkAggregate::f_LinkLast(CDLinkAggregate *_pLinkAfter)
	{
		f_Link(_pLinkAfter);
	}

	inline void CDLinkAggregate::fs_UnlinkRange(CDLinkAggregate *_pFirst, CDLinkAggregate *_pLast)
	{
		CThis *pFirst = _pFirst->f_GetPrev();
		CThis *pLast = _pLast->fp_GetNext();
		pFirst->fp_SetNext(pLast);
		pLast->fp_SetPrev(pFirst);
	}

	inline void CDLinkAggregate::fs_LinkRange(CDLinkAggregate *_pFirst, CDLinkAggregate *_pLast, CDLinkAggregate *_pInsertAfter)
	{
		CThis *pInsertAfterNext = _pInsertAfter->fp_GetNext();
		_pFirst->fp_SetPrev(_pInsertAfter);
		_pInsertAfter->fp_SetNext(_pFirst);

		_pLast->fp_SetNext(pInsertAfterNext);
		pInsertAfterNext->fp_SetPrev(_pLast);
	}

	inline void CDLinkAggregate::f_LinkNoUnlink(CDLinkAggregate *_pLinkAfter)
	{
		f_SetPrevInit(_pLinkAfter);
		fp_SetNext(_pLinkAfter->fp_GetNext());
		fp_GetNext()->fp_SetPrev(this);
		f_GetPrev()->fp_SetNext(this);
	}

	inline void CDLinkAggregate::f_LinkNoUnlinkNotList(CDLinkAggregate *_pLinkAfter)
	{
		f_LinkNoUnlink(_pLinkAfter);
	}

	inline void CDLinkAggregate::f_LinkNoUnlinkFirst(CDLinkAggregate *_pLinkAfter)
	{
		f_LinkNoUnlink(_pLinkAfter);
	}

	inline void CDLinkAggregate::f_LinkNoUnlinkLast(CDLinkAggregate *_pLinkAfter)
	{
		f_LinkNoUnlink(_pLinkAfter);
	}

	inline void CDLinkAggregate::f_SetNextNotList(CDLinkAggregate *_pNext)
	{
		m_pNextPtr = _pNext;
	}

	inline void CDLinkAggregate::fp_SetNextList(CDLinkAggregate *_pNext)
	{
		m_pNextPtr = _pNext;
	}

	inline void CDLinkAggregate::f_SetNextInit(CDLinkAggregate *_pNext)
	{
		m_pNextPtr = _pNext;
	}

	inline void CDLinkAggregate::f_SetNextInitListLink(CDLinkAggregate *_pNext)
	{
		m_pNextPtr = _pNext;
	}

	inline void CDLinkAggregate::f_SetPrevNotList(CDLinkAggregate *_pPrev)
	{
		m_pPrevPtr = (CDLinkAggregate *)(((umint)(CDLinkAggregate *)m_pPrevPtr & 1) | (umint)_pPrev);
	}

	inline void CDLinkAggregate::f_SetPrevInit(CDLinkAggregate *_pPrev)
	{
		m_pPrevPtr = _pPrev;
	}

	inline void CDLinkAggregate::f_SetPrevInitListLink(CDLinkAggregate *_pPrev)
	{
		m_pPrevPtr = (CDLinkAggregate *)((umint)_pPrev | 1);
	}

	inline bool CDLinkAggregate::f_IsListLink() const
	{
		return ((umint)(CDLinkAggregate *)m_pPrevPtr & 1);
	}

	inline CDLinkAggregate const *CDLinkAggregate::f_GetNextList() const
	{
		return m_pNextPtr;
	}

	inline CDLinkAggregate const *CDLinkAggregate::f_GetNextNotList() const
	{
		return m_pNextPtr;
	}

	inline CDLinkAggregate const *CDLinkAggregate::f_GetPrev() const
	{
		return (CDLinkAggregate *)((umint)(CDLinkAggregate *)m_pPrevPtr & (~1));
	}

	inline CDLinkAggregate const *CDLinkAggregate::f_GetPrevList() const
	{
		return (CDLinkAggregate *)((umint)(CDLinkAggregate *)m_pPrevPtr & (~1));
	}

	inline CDLinkAggregate const *CDLinkAggregate::f_GetPrevNotList() const
	{
		return (CDLinkAggregate *)((umint)(CDLinkAggregate *)m_pPrevPtr & (~1));
	}

	inline CDLinkAggregate const *CDLinkAggregate::f_GetPrevNoList() const
	{
		return (CDLinkAggregate *)((umint)(CDLinkAggregate *)m_pPrevPtr & (~1));
	}

	inline CDLinkAggregate *CDLinkAggregate::f_GetNextList()
	{
		return m_pNextPtr;
	}

	inline CDLinkAggregate *CDLinkAggregate::f_GetNextNotList()
	{
		return m_pNextPtr;
	}

	inline CDLinkAggregate *CDLinkAggregate::f_GetPrev()
	{
		return (CDLinkAggregate *)((umint)(CDLinkAggregate *)m_pPrevPtr & (~1));
	}

	inline CDLinkAggregate *CDLinkAggregate::f_GetPrevNotList()
	{
		return (CDLinkAggregate *)((umint)(CDLinkAggregate *)m_pPrevPtr & (~1));
	}

	inline CDLinkAggregate *CDLinkAggregate::f_GetPrevNoList()
	{
		return (CDLinkAggregate *)((umint)(CDLinkAggregate *)m_pPrevPtr & (~1));
	}

	inline CDLinkAggregate *CDLinkAggregate::f_GetPrevList()
	{
		return (CDLinkAggregate *)((umint)(CDLinkAggregate *)m_pPrevPtr & (~1));
	}

	inline void CDLinkAggregate::f_TransferList(CDLinkAggregate *_pFirst, CDLinkAggregate *_pLast)
	{
		f_SetNextInitListLink(_pFirst);
		fp_GetNext()->fp_SetPrev(this);
		f_SetPrevInitListLink(_pLast);
		f_GetPrev()->fp_SetNext(this);
	}

	inline CDLinkAggregate *CDLinkAggregate::f_Upcast()
	{
		return this;
	}

	inline CDLinkAggregate const *CDLinkAggregate::f_Upcast() const
	{
		return this;
	}

	inline bool CDLinkAggregate::f_IsAloneInList() const
	{
		return f_IsInList() && (f_GetPrev()->f_IsListLink()) && (fp_GetNext()->f_IsListLink());
	}

	inline void CDLinkAggregate::f_Unlink()
	{
		if (fp_GetNext())
		{
			fp_GetNext()->fp_SetPrev(f_GetPrev());
			f_GetPrev()->fp_SetNext(fp_GetNext());
			fp_SetNext(nullptr);
#ifdef DMibEnableSafeCheck
			// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
			f_SetPrevInit(nullptr);
#endif
		}
	}

	inline void CDLinkAggregate::f_UnlinkFirst()
	{
		f_Unlink();
	}

	inline void CDLinkAggregate::f_UnsafeUnlink()
	{
		fp_GetNext()->fp_SetPrev(f_GetPrev());
		f_GetPrev()->fp_SetNext(fp_GetNext());
	}

	inline void CDLinkAggregate::f_UnsafeUnlinkFirst()
	{
		f_UnsafeUnlink();
	}

	inline void CDLinkAggregate::f_UnlinkLinked()
	{
		fp_GetNext()->fp_SetPrev(f_GetPrev());
		f_GetPrev()->fp_SetNext(fp_GetNext());
		fp_SetNext(nullptr);
#ifdef DMibEnableSafeCheck
		// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
		f_SetPrevInit(nullptr);
#endif
	}

	inline void CDLinkAggregate::f_UnlinkFirstLinked()
	{
		f_UnlinkLinked();
	}

	inline void CDLinkAggregate::f_ConstructList()
	{
		fp_SetNext(nullptr);
#ifdef DMibEnableSafeCheck
		// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
		f_SetPrevInit(nullptr);
#endif
	}

	inline void CDLinkAggregate::f_Construct()
	{
		fp_SetNext(nullptr);
#ifdef DMibEnableSafeCheck
		// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
		f_SetPrevInit(nullptr);
#endif
	}

	inline void CDLinkAggregate::f_Destruct()
	{
		if (fp_GetNext())
		{
			fp_GetNext()->fp_SetPrev(f_GetPrev());
			f_GetPrev()->fp_SetNext(fp_GetNext());
#ifdef DMibEnableSafeCheck
			fp_SetNext(nullptr);
			f_SetPrevInit(nullptr);
#endif
		}
	}

	inline bool CDLinkAggregate::f_IsInList() const
	{
		return fp_GetNext() != nullptr;
	}

	inline void CDLinkAggregate::fp_SetNext(CDLinkAggregate *_pNext)
	{
		m_pNextPtr = _pNext;
	}

	inline void CDLinkAggregate::fp_SetPrev(CDLinkAggregate *_pPrev)
	{
		m_pPrevPtr = (CDLinkAggregate *)(((umint)(CDLinkAggregate *)m_pPrevPtr & 1) | (umint)_pPrev);
	}

	inline void CDLinkAggregate::fp_SetPrevList(CDLinkAggregate *_pPrev)
	{
		m_pPrevPtr = (CDLinkAggregate *)(((umint)(CDLinkAggregate *)m_pPrevPtr & 1) | (umint)_pPrev);
	}

	inline CDLinkAggregate const *CDLinkAggregate::fp_GetNext() const
	{
		return m_pNextPtr;
	}

	inline CDLinkAggregate *CDLinkAggregate::fp_GetNext()
	{
		return m_pNextPtr;
	}

	template <typename t_CLink>
	inline_small TCDLink<t_CLink>::TCDLink()
	{
		mp_Link.f_Construct();
	}

	template <typename t_CLink>
	TCDLink<t_CLink>::TCDLink(TCDLink &&_Other)
	{
		if (_Other.mp_Link.f_IsInList())
		{
			auto pOtherPrev = _Other.mp_Link.f_GetPrevNotList();
			_Other.mp_Link.f_UnlinkLinked();
			mp_Link.f_LinkNoUnlink(pOtherPrev);
		}
		else
			mp_Link.f_Construct();
	}

	template <typename t_CLink>
	auto TCDLink<t_CLink>::operator = (TCDLink &&_Other) -> TCDLink &
	{
		mp_Link.f_Destruct();
		if (_Other.mp_Link.f_IsInList())
		{
			auto pOtherPrev = _Other.mp_Link.f_GetPrevNotList();
			_Other.mp_Link.f_UnlinkLinked();
			mp_Link.f_LinkNoUnlink(pOtherPrev);
		}
		else
			mp_Link.f_Construct();
		return *this;
	}

	template <typename t_CLink>
	inline_small TCDLink<t_CLink>::~TCDLink()
	{
		mp_Link.f_Destruct();
	}

	template <typename t_CLink>
	inline_small t_CLink const *TCDLink<t_CLink>::f_GetNext() const
	{
		return (t_CLink const *)mp_Link.f_GetNextNotList();
	}

	template <typename t_CLink>
	inline_small t_CLink const *TCDLink<t_CLink>::f_GetPrev() const
	{
		return (t_CLink const *)mp_Link.f_GetPrevNotList();
	}

	template <typename t_CLink>
	inline_small void TCDLink<t_CLink>::f_Unlink()
	{
		mp_Link.f_Unlink();
	}

	template <typename t_CLink>
	inline_small void TCDLink<t_CLink>::f_UnsafeUnlink()
	{
		mp_Link.f_UnsafeUnlink();
	}

	template <typename t_CLink>
	inline_small void TCDLink<t_CLink>::f_UnlinkLinked()
	{
		mp_Link.f_UnlinkLinked();
	}

	template <typename t_CLink>
	inline_small void TCDLink<t_CLink>::f_UnlinkFirst()
	{
		mp_Link.f_UnlinkFirst();
	}

	template <typename t_CLink>
	inline_small void TCDLink<t_CLink>::f_UnsafeUnlinkFirst()
	{
		mp_Link.f_UnsafeUnlinkFirst();
	}

	template <typename t_CLink>
	inline_small void TCDLink<t_CLink>::f_UnlinkFirstLinked()
	{
		mp_Link.f_UnlinkFirstLinked();
	}

	template <typename t_CLink>
	inline_small bool TCDLink<t_CLink>::f_IsAloneInList() const
	{
		return mp_Link.f_IsAloneInList();
	}

	template <typename t_CLink>
	inline_small bool TCDLink<t_CLink>::f_IsInList() const
	{
		return mp_Link.f_IsInList();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	constexpr TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::TCDLinkListAggregate(EAggregateInitialization _Init)
		: m_Link{_Init}
	{
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::TCDLinkListAggregate()
	{
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Construct()
	{
#ifdef DMibDebug
		if ((umint)this & 1)
			DMibPDebugBreak; // We must be aligned
#endif

		m_Link.f_SetNextInitListLink(&m_Link);
		m_Link.f_SetPrevInitListLink(&m_Link);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename t_CDataInternal1, typename t_CAllocator1, bool t_bAutoDelete1>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>
		::TCAutoDeleteHelper<t_CDataInternal1, t_CAllocator1, t_bAutoDelete1>::fs_Delete(t_CData *_pDeleter)
	{
		fg_DeleteObject(t_CAllocator(), _pDeleter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename t_CDataInternal1, typename t_CAllocator1>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::TCAutoDeleteHelper<t_CDataInternal1, t_CAllocator1, 0>::fs_Delete
		(
			t_CData *_pDeleter
		)
	{
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Destruct()
	{
		if constexpr (t_bAutoDelete)
		{
			while (t_CData *pCurrent = f_GetLast())
			{
				// We must use this special template or objects that cannot be deleted will get a compile error here even if t_bAutoDelete is false
				TCAutoDeleteHelper<t_CData, t_CAllocator, t_bAutoDelete>::fs_Delete(pCurrent);
			}
		}
		else
			f_Clear();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Clear()
	{
		auto pLink = &m_Link;
		auto pNext = m_Link.f_GetNextList();

		while (pNext != pLink)
		{
			auto pCurrent = pNext;
			pNext = pNext->f_GetNextNotList();

			pCurrent->f_Upcast()->f_SetNextInit(nullptr);
#ifdef DMibEnableSafeCheck
			pCurrent->f_Upcast()->f_SetPrevInit(nullptr);
#endif
		}

		pLink->f_SetNextInitListLink(pLink);
		pLink->f_SetPrevInitListLink(pLink);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_medium void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Transfer(TCDLinkListAggregate *_pFrom)
	{
		DMibSafeCheck(f_IsEmpty(), "We must be empty to be able to transfer from another list");
		if (_pFrom->f_IsEmpty())
			return;
		f_UnsafeTransfer(_pFrom);
		_pFrom->f_Construct();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_medium void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeTransfer(TCDLinkListAggregate *_pFrom)
	{
		auto *pFromLink = &_pFrom->m_Link;
		m_Link.f_TransferList(pFromLink->f_GetNextList(), pFromLink->f_GetPrevList());
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Transfer(TCDLinkListAggregate &_From)
	{
		f_Transfer(&_From);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeTransfer(TCDLinkListAggregate &_From)
	{
		f_UnsafeTransfer(&_From);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_GetFirst()
	{
		auto pLink = &m_Link;
		auto pNext = pLink->f_GetNextList();
		if (pNext != pLink)
			return fsp_MemberFromLink(pNext->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_GetLast()
	{
		auto pLink = &m_Link;
		auto pPrev = pLink->f_GetPrevList();
		if (pPrev != pLink)
			return fsp_MemberFromLink(pPrev->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_GetFirst() const
	{
		auto pLink = &m_Link;
		auto pNext = pLink->f_GetNextList();
		if (pNext != pLink)
			return fsp_MemberFromLink(pNext->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_GetLast() const
	{
		auto pLink = &m_Link;
		auto pPrev = pLink->f_GetPrevList();
		if (pPrev != pLink)
			return fsp_MemberFromLink(pPrev->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fs_GetNextUnsafe(t_CData *_pCurrent)
	{
		auto *pNext = TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_GetNextNotList();
		if (!pNext->f_IsListLink())
			return TCDLinkListAggregate::fsp_MemberFromLink(pNext->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fs_GetNext(t_CData *_pCurrent)
	{
		auto *pNext = TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_GetNextNotList();
		if (pNext && !pNext->f_IsListLink())
			return TCDLinkListAggregate::fsp_MemberFromLink(pNext->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fs_GetPrevUnsafe(t_CData *_pCurrent)
	{
		auto *pPrev = TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_GetPrevNotList();
		if (!pPrev->f_IsListLink())
			return TCDLinkListAggregate::fsp_MemberFromLink(pPrev->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fs_GetPrev(t_CData *_pCurrent)
	{
		if (!TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_IsInList())
			return nullptr;

		return fs_GetPrevUnsafe(_pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fs_GetNextUnsafe(t_CData const *_pCurrent)
	{
		auto const *pNext = TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_GetNextNotList();
		if (!pNext->f_IsListLink())
			return TCDLinkListAggregate::fsp_MemberFromLink(pNext->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fs_GetNext(t_CData const *_pCurrent)
	{
		auto const *pNext = TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_GetNextNotList();
		if (pNext && !pNext->f_IsListLink())
			return TCDLinkListAggregate::fsp_MemberFromLink(pNext->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fs_GetPrevUnsafe(t_CData const *_pCurrent)
	{
		auto const *pPrev = TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_GetPrevNotList();
		if (!pPrev->f_IsListLink())
			return TCDLinkListAggregate::fsp_MemberFromLink(pPrev->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fs_GetPrev(t_CData const *_pCurrent)
	{
		if (!TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_IsInList())
			return nullptr;
		return fs_GetPrevUnsafe(_pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_DeleteAll()
	{
		while (t_CData *pCurrent = f_GetLast())
			fg_DeleteObject(t_CAllocator(), pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CAllocator2>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_DeleteAllAllocator()
	{
		while (t_CData *pCurrent = f_GetLast())
			fg_DeleteObject(tf_CAllocator2(), pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CAllocator2>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_DeleteAllAllocator(tf_CAllocator2 &_Allocator)
	{
		while (t_CData *pCurrent = f_GetLast())
			fg_DeleteObject(_Allocator, pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_DeleteAllDefiniteType()
	{
		while (t_CData *pCurrent = f_GetLast())
			fg_DeleteObjectDefiniteType(t_CAllocator(), pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CAllocator2>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_DeleteAllAllocatorDefiniteType()
	{
		while (t_CData *pCurrent = f_GetLast())
			fg_DeleteObjectDefiniteType(tf_CAllocator2(), pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CAllocator2>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_DeleteAllAllocatorDefiniteType(tf_CAllocator2 &_Allocator)
	{
		while (t_CData *pCurrent = f_GetLast())
			fg_DeleteObjectDefiniteType(_Allocator, pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CDeleter>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_DeleteAllDeleter()
	{
		while (t_CData *pCurrent = f_GetLast())
			tf_CDeleter::fs_Delete(pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_DeleteAllDeleter()
	{
		f_DeleteAllDeleter<t_CData>();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small bool TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_IsEmpty() const
	{
		auto pLink = &m_Link;
		return pLink->f_GetNextList() == pLink;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	umint TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_GetLen() const
	{
		CIteratorConst Iter(*this);
		umint Num = 0;
		while (Iter)
		{
			++Num;
			++Iter;
		}
		return Num;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CData2>
	bool TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Contains(tf_CData2 const &_CheckFor) const
	{
		CIteratorConst Iter(*this);
		while (Iter)
		{
			if ((*Iter) == _CheckFor)
				return true;
			++Iter;
		}
		return false;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CData2>
	t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Find(tf_CData2 const &_CheckFor) const
	{
		CIteratorConst Iter(*this);
		while (Iter)
		{
			if ((*Iter) == _CheckFor)
				return Iter;
			++Iter;
		}
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CData2>
	t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Find(tf_CData2 const &_CheckFor)
	{
		CIterator Iter(*this);
		while (Iter)
		{
			if ((*Iter) == _CheckFor)
				return Iter;
			++Iter;
		}
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare, typename tf_CData2>
	t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Find(tf_FCompare &&_fCompare, tf_CData2 const &_CheckFor) const
	{
		CIteratorConst Iter(*this);
		while (Iter)
		{
			if (!_fCompare((*Iter), _CheckFor) && !_fCompare(_CheckFor, (*Iter)))
				return Iter;
			++Iter;
		}
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare, typename tf_CData2>
	t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Find(tf_FCompare &&_fCompare, tf_CData2 const &_CheckFor)
	{
		CIterator Iter(*this);
		while (Iter)
		{
			if (!_fCompare((*Iter), _CheckFor) && !_fCompare(_CheckFor, (*Iter)))
				return Iter;
			++Iter;
		}
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	bool TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Contains(t_CData const *_pCheckFor) const
	{
		CIteratorConst Iter(*this);
		while (Iter)
		{
			if (Iter.f_GetCurrent() == _pCheckFor)
				return true;
			++Iter;
		}
		return false;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	bool TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Contains(t_CData *_pCheckFor) const
	{
		CIteratorConst Iter(*this);
		while (Iter)
		{
			if (Iter.f_GetCurrent() == _pCheckFor)
				return true;
			++Iter;
		}
		return false;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_SplitInto(t_CData *_pFirst, t_CData *_pLast)
	{
		DMibSafeCheck(f_IsEmpty(), "List must be empty here");
		DMibSafeCheck(_pFirst, "First must be specified");
		DMibSafeCheck(_pLast, "Last must be specified");
		t_CLink *pToLinkFirst = fsp_LinkFromMember(_pFirst);
		t_CLink *pToLinkLast = fsp_LinkFromMember(_pLast);
		DMibSafeCheck(pToLinkFirst->f_IsInList(), "First must be in list");
		DMibSafeCheck(pToLinkLast->f_IsInList(), "Last must be in list");
		DMibSafeCheck(!pToLinkFirst->f_IsListLink(), "Internal error");
		DMibSafeCheck(!pToLinkLast->f_IsListLink(), "Internal error");

		t_CLink::fs_UnlinkRange(pToLinkFirst, pToLinkLast);

		m_Link.f_TransferList(pToLinkFirst, pToLinkLast);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Insert(TCDLinkListAggregate &_OtherList)
	{
		if (_OtherList.f_IsEmpty())
			return;

		auto pLink = &m_Link;
		auto pOtherLink = &_OtherList.m_Link;
		if (f_IsEmpty())
		{
			pLink->f_TransferList(pOtherLink->f_GetNextList(), pOtherLink->f_GetPrevList());
			_OtherList.f_Construct();
			return;
		}
		auto *pToLinkFirst = pOtherLink->f_GetNextList();
		auto *pToLinkLast = pOtherLink->f_Upcast()->f_GetPrevList();
		auto *pToInsertAfter = pLink->f_Upcast()->f_GetPrevList();

		t_CLink::fs_LinkRange(pToLinkFirst->f_Upcast(), pToLinkLast->f_Upcast(), pToInsertAfter);

		// Just reset other list
		_OtherList.f_Construct();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Insert(t_CData *_pToInsert)
	{
		t_CLink *pToLink = fsp_LinkFromMember(_pToInsert);
		pToLink->f_Unlink();
		auto *pLast = m_Link.f_GetPrevList();
		pToLink->f_LinkLast(pLast);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Insert(t_CData &_ToInsert)
	{
		f_Insert(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertSorted(TCDLinkListAggregate &_OtherList)
	{
		while (auto *pData = _OtherList.f_Pop())
			f_InsertSorted(pData);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertSortedLast(t_CData &_ToInsert, tf_FCompare &&_fCompare)
	{
		f_InsertSortedLast(&_ToInsert, _fCompare);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertSortedLast(t_CData *_pToInsert, tf_FCompare &&_fCompare)
	{
		t_CLink *pToLink = fsp_LinkFromMember(_pToInsert);
		pToLink->f_Unlink();

		if (f_IsEmpty() || fg_CheckOrdering(_fCompare(*_pToInsert, *f_GetLast())) > 0)
		{
			f_InsertLast(_pToInsert);
			return;
		}

		auto pLink = &m_Link;
		auto *pCurrent = pLink->f_GetNextList();
		auto *pLast = pLink;

		while (pCurrent != pLink)
		{
			if (fg_CheckOrdering(_fCompare(*fsp_MemberFromLink(pCurrent->f_Upcast()), *_pToInsert)) > 0)
				break;
			pLast = pCurrent;
			pCurrent = pCurrent->f_GetNextNotList();
		}

		pToLink->f_Link(pLast);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertSorted(t_CData &_ToInsert, tf_FCompare &&_fCompare)
	{
		f_InsertSortedLast(_ToInsert, _fCompare);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertSorted(t_CData *_pToInsert, tf_FCompare &&_fCompare)
	{
		f_InsertSortedLast(_pToInsert, _fCompare);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertSortedFirst(t_CData &_ToInsert, tf_FCompare &&_fCompare)
	{
		f_InsertSortedFirst(&_ToInsert, _fCompare);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertSortedFirst(t_CData *_pToInsert, tf_FCompare &&_fCompare)
	{
		t_CLink *pToLink = fsp_LinkFromMember(_pToInsert);
		pToLink->f_Unlink();

		if (f_IsEmpty() || fg_CheckOrdering(_fCompare(*_pToInsert, *f_GetLast())) > 0)
		{
			f_InsertLast(_pToInsert);
			return;
		}

		auto *pLink = &m_Link;
		auto *pCurrent = pLink->f_GetNextList();
		auto *pLast = pLink;

		while (pCurrent != pLink)
		{
			if (fg_CheckOrdering(_fCompare(*fsp_MemberFromLink(pCurrent->f_Upcast()), *_pToInsert)) >= 0)
				break;
			pLast = pCurrent;
			pCurrent = pCurrent->f_GetNextNotList();
		}

		pToLink->f_Link(pLast);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertLast(TCDLinkListAggregate &_OtherList)
	{
		f_Insert(_OtherList);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertLast(t_CData *_pToInsert)
	{
		f_Insert(_pToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertLast(t_CData &_ToInsert)
	{
		f_InsertLast(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertFirst(t_CData *_pToInsert)
	{
		t_CLink *pToLink = fsp_LinkFromMember(_pToInsert);
		pToLink->f_Unlink();
		pToLink->f_LinkFirst(&m_Link);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertFirst(t_CData &_ToInsert)
	{
		f_InsertFirst(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertFirst(TCDLinkListAggregate &_OtherList)
	{
		if (_OtherList.f_IsEmpty())
			return;
		auto pLink = &m_Link;
		auto pOtherLink = &_OtherList.m_Link;
		if (f_IsEmpty())
		{
			pLink->f_TransferList(pOtherLink->f_GetNextList(), pOtherLink->f_GetPrevList());
			_OtherList.f_Construct();
			return;
		}
		auto *pToLinkFirst = pOtherLink->f_GetNextList();
		auto *pToLinkLast = pOtherLink->f_GetPrevList();
		auto *pToInsertAfter = pLink;

		t_CLink::fs_LinkRange(pToLinkFirst->f_Upcast(), pToLinkLast->f_Upcast(), pToInsertAfter);

		// Just reset other list
		_OtherList.f_Construct();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertAfter(TCDLinkListAggregate &_OtherList, t_CData *_pToInsertAfter)
	{
		if (_OtherList.f_IsEmpty())
			return;
		DMibSafeCheck(!f_IsEmpty(), "Cannot be empty");
		DMibSafeCheck(fsp_LinkFromMember(_pToInsertAfter)->f_IsInList(), "The object has to be in a list to be able to be inserted");

		auto pOtherLink = &_OtherList.m_Link;

		t_CLink *pToLinkFirst = pOtherLink->f_GetNextList()->f_Upcast();
		t_CLink *pToLinkLast = pOtherLink->f_GetPrevList()->f_Upcast();

		t_CLink *pToInsertAfter = fsp_LinkFromMember(_pToInsertAfter);

		t_CLink::fs_LinkRange(pToLinkFirst, pToLinkLast, pToInsertAfter);

		// Just reset other list
		_OtherList.f_Construct();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
	{
		DMibSafeCheck(fsp_LinkFromMember(_pToInsertAfter)->f_IsInList(), "The object has to be in a list to be able to be inserted");
		DMibSafeCheck(_pToInsert != _pToInsertAfter, "You cannot insert an object after itself");
		t_CLink *pToLink = fsp_LinkFromMember(_pToInsert);
		pToLink->f_Unlink();
		pToLink->f_Link(fsp_LinkFromMember(_pToInsertAfter));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter)
	{
		f_InsertAfter(&_ToInsert, _pToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter)
	{
		f_InsertAfter(_pToInsert, &_ToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter)
	{
		f_InsertAfter(&_ToInsert, &_ToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertBefore(TCDLinkListAggregate &_OtherList, t_CData *_pLinkBeforeThis)
	{
		if (_OtherList.f_IsEmpty())
			return;
		DMibSafeCheck(!f_IsEmpty(), "Cannot be empty");
		DMibSafeCheck(fsp_LinkFromMember(_pLinkBeforeThis)->f_IsInList(), "The object has to be in a list to be able to be inserted");

		auto pOtherLink = &_OtherList.m_Link;
		t_CLink *pToLinkFirst = pOtherLink->f_GetNextList()->f_Upcast();
		t_CLink *pToLinkLast = pOtherLink->f_GetPrevList()->f_Upcast();

		t_CLink *pToInsertAfter = fsp_LinkFromMember(_pLinkBeforeThis)->f_GetPrevNotList();

		t_CLink::fs_LinkRange(pToLinkFirst, pToLinkLast, pToInsertAfter);

		// Just reset other list
		_OtherList.f_Construct();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertBefore(t_CData *_pToInsert, t_CData *_pLinkBeforeThis)
	{
		DMibSafeCheck(fsp_LinkFromMember(_pLinkBeforeThis)->f_IsInList(), "The object has to be in a list to be able to be inserted");
		DMibSafeCheck(_pToInsert != _pLinkBeforeThis, "You cannot insert an object before itself");
		t_CLink *pToLink = fsp_LinkFromMember(_pToInsert);
		t_CLink *pLinkBeforeThis = fsp_LinkFromMember(_pLinkBeforeThis);

		pToLink->f_Unlink();
		pToLink->f_Link(pLinkBeforeThis->f_GetPrevNotList());
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertBefore(t_CData &_ToInsert, t_CData *_pToInsertBefore)
	{
		f_InsertBefore(&_ToInsert, _pToInsertBefore);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertBefore(t_CData *_pToInsert, t_CData &_ToInsertBefore)
	{
		f_InsertBefore(_pToInsert, &_ToInsertBefore);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_InsertBefore(t_CData &_ToInsert, t_CData &_ToInsertBefore)
	{
		f_InsertBefore(&_ToInsert, &_ToInsertBefore);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Push(t_CData *_pData)
	{
		f_InsertFirst(_pData);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Push(t_CData &_Data)
	{
		f_InsertFirst(&_Data);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsert(t_CData *_pToInsert)
	{
		fsp_LinkFromMember(_pToInsert)->f_LinkNoUnlinkLast(m_Link.f_GetPrevList());
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsert(t_CData &_ToInsert)
	{
		f_UnsafeInsert(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertSorted(t_CData &_ToInsert, tf_FCompare &&_fCompare)
	{
		f_UnsafeInsertSorted(&_ToInsert, _fCompare);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertSorted(t_CData *_pToInsert, tf_FCompare &&_fCompare)
	{
		auto pLink = &m_Link;
		auto *pCurrent = pLink->f_GetNextList();

		while (pCurrent != pLink)
		{
			if (fg_CheckOrdering(_fCompare(*fsp_MemberFromLink(pCurrent->f_Upcast()), *_pToInsert)) > 0)
				break;
			pCurrent = pCurrent->f_GetNextNotList();
		}

		if (pCurrent != pLink)
		{
			t_CLink *pToLink = fsp_LinkFromMember(_pToInsert);
			pToLink->f_LinkNoUnlink(pCurrent);
		}
		else
		{
			t_CLink *pToLink = fsp_LinkFromMember(_pToInsert);
			pToLink->f_LinkNoUnlinkFirst(pLink);
		}
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertLast(t_CData *_pToInsert)
	{
		f_UnsafeInsert(_pToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertLast(t_CData &_ToInsert)
	{
		f_UnsafeInsertLast(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertFirst(t_CData *_pToInsert)
	{
		fsp_LinkFromMember(_pToInsert)->f_LinkNoUnlinkFirst(&m_Link);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertFirst(t_CData &_ToInsert)
	{
		f_UnsafeInsertFirst(&_ToInsert);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
	{
		DMibSafeCheck(fsp_LinkFromMember(_pToInsertAfter)->f_IsInList(), "The object has to be in a list to be able to be inserted");
		fsp_LinkFromMember(_pToInsert)->f_LinkNoUnlink(fsp_LinkFromMember(_pToInsertAfter));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter)
	{
		f_UnsafeInsertAfter(&_ToInsert, _pToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter)
	{
		f_UnsafeInsertAfter(_pToInsert, &_ToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter)
	{
		f_UnsafeInsertAfter(&_ToInsert, &_ToInsertAfter);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafePush(t_CData *_pData)
	{
		f_UnsafeInsertFirst(_pData);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Remove(t_CData *_pToRemove)
	{
		fsp_LinkFromMember(_pToRemove)->f_Unlink();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Remove(t_CData &_ToRemove)
	{
		f_Remove(&_ToRemove);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_medium t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Pop()
	{
		auto *pLink = &m_Link;
		auto *pNext = pLink->f_GetNextList();
		if (pNext != pLink)
		{
			pNext->f_Upcast()->f_UnlinkFirstLinked();
			return fsp_MemberFromLink(pNext->f_Upcast());
		}
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_medium t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_UnsafePop()
	{
		auto *pLink = &m_Link;
		auto *pNext = pLink->f_GetNextList();
		if (pNext != pLink)
		{
			pNext->f_Upcast()->f_UnsafeUnlinkFirst();
			return fsp_MemberFromLink(pNext->f_Upcast());
		}
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Reverse()
	{
		auto *pLink = &m_Link;
		auto *pCurrent = pLink->f_GetNextList();
		while (pCurrent != pLink)
		{
			t_CLink *pTemp = pCurrent->f_Upcast();
			pCurrent = pCurrent->f_GetNextNotList();
			pTemp->f_UnlinkLinked();
			pTemp->f_LinkFirst(pLink);
		}
	}

	/*ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ*\
	|	Function:			Sorts the linked list								|
	|																			|
	|	Parameters:																|
	|		_fCompare:		Compare functor										|
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
	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_MergeSort(tf_FCompare &&_fCompare, aint _InsertionBits)
	{
		// We use mergesort as a stable and predictably performing algorithm, needing no extra heapspace or stackspace
		// O = n(log n)

		// Empty list ??
		if (f_IsEmpty())
			return;

		// Start with insertion sorting a bit first
		auto *pLink = &m_Link;
		t_CLink List;
		{
			auto pNext = pLink->f_GetNextList();
			auto pPrev = pLink->f_GetPrevList();
			List.f_SetNextInit(pNext);
			List.f_SetPrevInit(pPrev);
			pNext->f_Upcast()->f_SetPrevInit(&List);
			pPrev->f_Upcast()->f_SetNextInit(&List);

			pLink->f_SetNextInitListLink(pLink);
			pLink->f_SetPrevInitListLink(pLink);
		}

		aint MergeSize = aint(1) << (_InsertionBits);

		if (_InsertionBits > 0)
		{
			aint const SortSize = aint(1) << _InsertionBits;

			auto *pCurrent = List.f_GetNextNotList();
			t_CLink LastLink;
			t_CLink *pLastLink = &LastLink;
			pLastLink->f_SetNextInit(pLastLink);
			pLastLink->f_SetPrevInit(pLastLink);
			aint NumSorted = 0;

			while (pCurrent != &List)
			{
				// Add first member

				++NumSorted;
				t_CLink *pTemp = pCurrent->f_GetNextNotList()->f_Upcast();
				pCurrent->f_Upcast()->f_LinkNoUnlinkNotList(pLastLink->f_GetPrevNoList());
				pCurrent = pTemp;

				if (pCurrent == &List)
					break;

				for (aint iItem = 1; iItem < SortSize; ++iItem)
				{
					aint NumChecked = iItem;
					++NumSorted;

					t_CLink *pTemp = pLastLink->f_GetPrevNoList();

					while (NumChecked)
					{
						if (fg_CheckOrdering(_fCompare(*fsp_MemberFromLink(pCurrent->f_Upcast()), *fsp_MemberFromLink(pTemp))) >= 0)
							break;

						--NumChecked;
						pTemp = pTemp->f_GetPrevNoList();
					}

					t_CLink *pTempNext = pCurrent->f_GetNextNotList()->f_Upcast();
					pCurrent->f_Upcast()->f_LinkNoUnlinkNotList(pTemp);
					pCurrent = pTempNext;
					if (pCurrent == &List)
						break;
				}
			}

			pLink->f_TransferList(pLastLink->f_GetNextNotList(), pLastLink->f_GetPrevNoList());
			if (NumSorted <= MergeSize)
			{
				// Add last link
				return;
			}

			// Remove last link
			// 	LastLink.f_GetPrev()->fp_SetNext(nullptr);
		}

		DMibSafeCheck(_InsertionBits >= 0, "Has to be at least 1");

		{
			auto pNext = pLink->f_GetNextList();
			auto pPrev = pLink->f_GetPrevList();
			List.f_SetNextInit(pNext);
			List.f_SetPrevInit(pPrev);
			pNext->f_Upcast()->f_SetPrevInit(&List);
			pPrev->f_Upcast()->f_SetNextInit(&List);

			pLink->f_SetNextInitListLink(pLink);
			pLink->f_SetPrevInitListLink(pLink);
		}

		while (1)
		{
			auto *pFirst = List.f_GetNextNotList()->f_Upcast();
			auto *pLast = &List;

			aint MergesDone = 0; // count number of merges we do in this pass

			while (pFirst != &List)
			{
				MergesDone++; // there exists a merge to be done
				// step 'MergeSize' places along from pFirst
				t_CLink *pSecond = pFirst->f_Upcast();
				aint MergeSizeFirst = 0;
				aint MergeSizeSecond = MergeSize;
				while (MergeSizeFirst < MergeSize)
				{
					MergeSizeFirst++;
					pSecond = pSecond->f_GetNextNotList()->f_Upcast();
					if (pSecond == &List)
						break;
				}
				// if pSecond hasn't fallen off end, we have two lists to merge

				if (pSecond == &List)
				{
					// End of list
					pLast->f_SetNextNotList(pFirst);
					pFirst->f_SetPrevNotList(pLast);
					pLast = &List;
					pFirst = pSecond;
					continue;
				}

				if (fg_CheckOrdering(_fCompare(*fsp_MemberFromLink(pSecond->f_GetPrevNoList()), *fsp_MemberFromLink(pSecond))) <= 0)
				{
					// Lists already sorted
					pLast->f_SetNextNotList(pFirst);
					pFirst->f_SetPrevNotList(pLast);

					while (MergeSizeSecond && pSecond != &List)
					{
						--MergeSizeSecond;
						pLast = pSecond;
						pSecond = pSecond->f_GetNextNotList()->f_Upcast();
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
							pLast->f_SetNextNotList(pFirst);
							pFirst->f_SetPrevNotList(pLast);
							pLast = pFirst;
							pFirst = pFirst->f_GetNextNotList()->f_Upcast();
							MergeSizeFirst--;
							if (!MergeSizeFirst)
							{
								pLast->f_SetNextNotList(pSecond);
								pSecond->f_SetPrevNotList(pLast);

								while (MergeSizeSecond && pSecond != &List)
								{
									--MergeSizeSecond;
									pLast = pSecond;
									pSecond = pSecond->f_GetNextNotList()->f_Upcast();
								}
								break;
							}
						}
						else
						{
							// First element of pSecond is lower; pTemp must come from pSecond.
							pLast->f_SetNextNotList(pSecond);
							pSecond->f_SetPrevNotList(pLast);
							pLast = pSecond;
							pSecond = pSecond->f_GetNextNotList()->f_Upcast();
							MergeSizeSecond--;
							if (pSecond == &List || !MergeSizeSecond)
							{
								pLast->f_SetNextNotList(pFirst);
								pFirst->f_SetPrevNotList(pLast);

								while (MergeSizeFirst)
								{
									MergeSizeFirst--;
									pLast = pFirst;
									pFirst = pFirst->f_GetNextNotList()->f_Upcast();
								}
								break;
							}
						}
					}
				}

				// now pFirst has stepped 'MergeSize' places along, and pSecond has too
				pFirst = pSecond;
			}

			if (pLast != &List)
			{
				pLast->f_SetNextNotList(&List);
				List.f_SetPrevNotList(pLast);
			}

			// If we have done only one merge, we're finished.
			if (MergesDone <= 1) // allow for MergesDone==0, the empty list case
			{
				pLink->f_TransferList(List.f_GetNextNotList(), List.f_GetPrevNotList());
				return;
			}

			// Otherwise repeat, merging lists twice the size
			MergeSize <<= 1;
		}
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CSortClass>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_MergeSort()
	{
		f_MergeSort<tf_CSortClass>(tf_CSortClass());
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_FCompare>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_Sort(tf_FCompare &&_fCompare)
	{
		f_MergeSort(_fCompare);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CBucketCallbackSort::fs_GetIndex
		(
			void *_pContext
			, aint _Place
			, void *_pItem
		)
		-> aint
	{
		return ((CBucketCallbackSort *)_pContext)->m_fSortFunction(((CBucketCallbackSort *)_pContext)->m_pContext, _Place, _pItem);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <aint tf_BucketSize>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_BucketSortCallback
		(
			aint _NumPlaces
			, FBucketGetIndex *_fSortFunction
			, void *_pContext
		)
	{
		CBucketCallbackSort SortContext;
		SortContext.m_pContext = _pContext;
		SortContext.m_fSortFunction = _fSortFunction;
		f_BucketSort<CBucketCallbackSort, tf_BucketSize>(_NumPlaces, &SortContext);
	}

	/*ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ*\
	|	Function:			Sorts the linked list								|
	|																			|
	|	Template params:														|
	|		tf_CSortClass:	The class that implements the index funtion.		|
	|						The funtion must be static and be called fs_GetIndex|
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
	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CSortClass, aint tf_BucketSize>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_BucketSort(aint _NumPlaces, void *_pContext)
	{
		if (f_IsEmpty())
			return;

		t_CLink *Bucket[tf_BucketSize];

		// Reset buckets
		for (aint iBucket = 0; iBucket < tf_BucketSize; ++iBucket)
			Bucket[iBucket] = nullptr;

		t_CLink List;
		{
			auto pLink = &m_Link;
			auto pNext = pLink->f_GetNextList();
			auto pPrev = pLink->f_GetPrevList();
			List.f_SetNextInit(pNext);
			List.f_SetPrevInit(pPrev);
			pNext->f_SetPrevInit(&List);
			pPrev->f_SetNextInit(&List);

			pLink->f_SetNextInitListLink(pLink);
			pLink->f_SetPrevInitListLink(pLink);
		}

		auto pLink = &List;

		// Put list in first bucket

		for (aint Place = 0; Place < _NumPlaces; ++Place)
		{
			// Remove last link
			pLink->f_GetPrevNoList()->f_SetNextNotList(nullptr);

			auto *pCurrent = pLink->f_GetNextNotList();

			while (pCurrent)
			{
				aint Index = tf_CSortClass::fs_GetIndex(_pContext, Place, fsp_MemberFromLink(pCurrent->f_Upcast()));
				t_CLink **pBucket = &Bucket[Index];

				t_CLink *pNext = pCurrent->f_GetNextNotList();

				if (*pBucket)
					(*pBucket)->f_SetPrevNotList(pCurrent);
				pCurrent->f_Upcast()->f_SetNextNotList(*pBucket);
				(*pBucket) = pCurrent;

				pCurrent = pNext;
			}

			// Rebuild list

			pLink->f_SetNextNotList(pLink);
			pLink->f_SetPrevNotList(pLink);

			for (aint iBucket = 0; iBucket < tf_BucketSize; ++iBucket)
			{
				if (Bucket[iBucket])
				{
					t_CLink *pTemp = Bucket[iBucket];
					Bucket[iBucket] = nullptr;
					auto *pLinkAfter = pLink->f_GetPrevNoList();

					while (pTemp)
					{
						auto *pTempNext = pTemp->f_GetNextNotList();
						pTemp->f_LinkNoUnlinkNotList(pLinkAfter);
						pTemp = pTempNext;
					}
				}
			}
		}

		m_Link.f_TransferList(List.f_GetNextNotList(), List.f_GetPrevNoList());
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	bool TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_CheckList(bool _bBreak) const
	{
		auto *pLink = &m_Link;
		auto *pCurrent = pLink;
		if (!pCurrent->f_IsListLink())
		{
			if (_bBreak)
				DMibPDebugBreak;
			return false;
		}
		if (!pCurrent->f_IsValid())
		{
			if (_bBreak)
				DMibPDebugBreak;
			return false;
		}
		auto *pPrev = pCurrent;
		pCurrent = pCurrent->f_GetNextList();
		while (pCurrent != pLink)
		{
			if (pCurrent->f_Upcast()->f_GetPrevNotList() != pPrev || !pCurrent->f_IsValid())
			{
				if (_bBreak)
					DMibPDebugBreak;
				return false;
			}
			pPrev = pCurrent;
			pCurrent = pCurrent->f_GetNextNotList();
		}
		pCurrent = pLink;
		pPrev = pCurrent;
		pCurrent = pCurrent->f_GetPrevList();
		while (pCurrent != pLink)
		{
			if (pCurrent->f_Upcast()->f_GetNextNotList() != pPrev || !pCurrent->f_IsValid())
			{
				if (_bBreak)
					DMibPDebugBreak;
				return false;
			}
			pPrev = pCurrent;
			pCurrent = pCurrent->f_Upcast()->f_GetPrevNotList();
		}
		return true;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::CIterator()
	{
		m_pCurrent = nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::CIterator(TCDLinkListAggregate &_List)
	{
		auto *pLink = &_List.m_Link;
		auto pCurrent = pLink->f_GetNextList();
		if (pCurrent == pLink)
			m_pCurrent = nullptr;
		else
			m_pCurrent = pCurrent->f_Upcast();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::CIterator(CIterator const &_Copy)
	{
		m_pCurrent = _Copy.m_pCurrent;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::CIterator(t_CData *_pStart)
	{
		m_pCurrent = TCDLinkListAggregate::fsp_LinkFromMember(_pStart);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::CIterator(t_CData &_Start)
	{
		m_pCurrent = TCDLinkListAggregate::fsp_LinkFromMember(&_Start);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::f_GetIterator() const -> CIterator
	{
		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator = (TCDLinkListAggregate &_List) -> CIterator &
	{
		auto *pLink = &_List.m_Link;
		auto *pNext = pLink->f_GetNextList();
		if (pNext == pLink)
			m_pCurrent = nullptr;
		else
			m_pCurrent = pNext->f_Upcast();

		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator = (CIterator const &_Copy) -> CIterator &
	{
		m_pCurrent = _Copy.m_pCurrent;
		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator = (t_CData &_Start) -> CIterator &
	{
		m_pCurrent = TCDLinkListAggregate::fsp_LinkFromMember(&_Start);

		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator = (t_CData *_pStart) -> CIterator &
	{
		m_pCurrent = TCDLinkListAggregate::fsp_LinkFromMember(_pStart);

		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::fs_GetNext(t_CData *_pCurrent)
	{
		auto *pNext = TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_GetNextNotList();
		if (pNext && !pNext->f_IsListLink())
			return TCDLinkListAggregate::fsp_MemberFromLink(pNext->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::fs_GetNextUnsafe(t_CData *_pCurrent)
	{
		auto *pNext = TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_GetNextNotList();
		if (!pNext->f_IsListLink())
			return TCDLinkListAggregate::fsp_MemberFromLink(pNext->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::fs_GetPrevUnsafe(t_CData *_pCurrent)
	{
		auto *pPrev = TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_GetPrevNotList();
		if (!pPrev->f_IsListLink())
			return TCDLinkListAggregate::fsp_MemberFromLink(pPrev->f_Upcast());
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::fs_GetPrev(t_CData *_pCurrent)
	{
		if (!TCDLinkListAggregate::fsp_LinkFromMember(_pCurrent)->f_IsInList())
			return nullptr;
		return fs_GetPrevUnsafe(_pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::f_Reverse(TCDLinkListAggregate &_List)
	{
		auto *pLink = &_List.m_Link;
		auto *pPrev = _List.m_Link.f_GetPrevList();

		if (pPrev == pLink)
			m_pCurrent = nullptr;
		else
			m_pCurrent = pPrev->f_Upcast();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::f_GetCurrent() const
	{
		if (m_pCurrent)
			return TCDLinkListAggregate::fsp_MemberFromLink(m_pCurrent);
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator t_CData * () const
	{
		return f_GetCurrent();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator -> () const
	{
		return f_GetCurrent();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator [] (aint _nPlaces) const
	{
		if (m_pCurrent)
		{
			auto *pCurrent = m_pCurrent;
			if (_nPlaces > 0)
			{
				while (_nPlaces)
				{
					auto *pNext = pCurrent->f_GetNextNotList();
					if (pNext->f_IsListLink())
					{
						DMibSafeCheck(0, "You tried to access an element outside the list");
						return nullptr;
					}
					pCurrent = pNext->f_Upcast();
					--_nPlaces;
				}
			}
			else
			{
				while (_nPlaces)
				{
					auto *pPrev = pCurrent->f_GetPrevNotList();
					if (pPrev->f_IsListLink())
					{
						DMibSafeCheck(0, "You tried to access an element outside the list");
						return nullptr;
					}
					pCurrent = pPrev->f_Upcast();
					++_nPlaces;
				}
			}
			return TCDLinkListAggregate::fsp_MemberFromLink(pCurrent);
		}
		DMibSafeCheck(0, "You tried to access an element outside the list");
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	umint TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::f_GetLen() const
	{
		if (m_pCurrent)
		{
			t_CLinkInList *pCurrent = m_pCurrent;
			umint Len = 0;
			while (!pCurrent->f_IsListLink())
			{
				++Len;
				pCurrent = pCurrent->f_GetNextNotList();
			}
			return Len;
		}
		return 0;
	}

#ifdef DMibPUniqueType_int
	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator [] (int _nPlaces) const
	{
		return operator [] (aint(_nPlaces));
	}
#endif

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator += (aint _nPlaces)
	{
		if (m_pCurrent)
		{
			while (_nPlaces)
			{
				auto *pNext = m_pCurrent->f_GetNextNotList();
				if (pNext->f_IsListLink())
				{
					m_pCurrent = nullptr;
					break;
				}
				m_pCurrent = pNext->f_Upcast();
				--_nPlaces;
			}
		}
		DMibSafeCheck(!_nPlaces, "You tried to access an element outside the list");
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator ++ ()
	{
		if (m_pCurrent)
		{
			auto pNext = m_pCurrent->f_GetNextNotList();
			if (pNext->f_IsListLink())
				m_pCurrent = nullptr;
			else
				m_pCurrent = pNext->f_Upcast();
		}
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator -- ()
	{
		if (m_pCurrent)
		{
			auto *pPrev = m_pCurrent->f_GetPrevNotList();
			if (pPrev->f_IsListLink())
				m_pCurrent = nullptr;
			else
				m_pCurrent = pPrev->f_Upcast();
		}
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::operator -= (aint _nPlaces)
	{
		if (m_pCurrent)
		{
			while (_nPlaces)
			{
				auto *pPrev = m_pCurrent->f_GetPrevNotList();
				if (pPrev->f_IsListLink())
				{
					m_pCurrent = nullptr;
					break;
				}
				else
					m_pCurrent = pPrev->f_Upcast();
				--_nPlaces;
			}
		}
		DMibSafeCheck(!_nPlaces, "You tried to access an element outside the list");
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::f_Delete()
	{
		t_CData *pCurrent = f_GetCurrent();
		++(*this);
		fg_DeleteObject(t_CAllocator(), pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	template <typename tf_CDeleter>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::f_DeleteDeleter()
	{
		t_CData *pCurrent = f_GetCurrent();
		++(*this);
		tf_CDeleter::fs_Delete(pCurrent);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::f_DeleteDeleter()
	{
		f_DeleteDeleter<t_CData>();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::f_Remove()
	{
		t_CLink *pLink = m_pCurrent;
		++(*this);
		pLink->f_UnlinkLinked();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_GetIterator() -> CIterator
	{
		return CIterator(*this);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::CIteratorConst()
	{
		m_pCurrent = nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::CIteratorConst(CIteratorConst const &_Copy)
	{
		m_pCurrent = _Copy.m_pCurrent;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::CIteratorConst(TCDLinkListAggregate const &_List)
	{
		auto *pLink = &_List.m_Link;
		auto *pNext = pLink->f_GetNextList();
		if (pNext == pLink)
			m_pCurrent = nullptr;
		else
			m_pCurrent = pNext->f_Upcast();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::CIteratorConst(t_CData const *_pStart)
	{
		m_pCurrent = TCDLinkListAggregate::fsp_LinkFromMember(_pStart);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::CIteratorConst(t_CData const &_Start)
	{
		m_pCurrent = TCDLinkListAggregate::fsp_LinkFromMember(&_Start);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::f_GetIterator() const -> CIteratorConst
	{
		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator = (TCDLinkListAggregate const &_List) -> CIteratorConst &
	{
		auto *pLink = &_List.m_Link;
		auto *pNext = pLink->f_GetNextList();
		if (pNext == pLink)
			m_pCurrent = nullptr;
		else
			m_pCurrent = pNext->f_Upcast();
		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator = (CIteratorConst const &_Copy) -> CIteratorConst &
	{
		m_pCurrent = _Copy.m_pCurrent;
		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator = (t_CData const &_Start) -> CIteratorConst &
	{
		m_pCurrent = TCDLinkListAggregate::fsp_LinkFromMember(&_Start);

		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator = (t_CData const *_pStart) -> CIteratorConst &
	{
		m_pCurrent = TCDLinkListAggregate::fsp_LinkFromMember(_pStart);

		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::f_Reverse(TCDLinkListAggregate const &_List)
	{
		auto *pLink = &_List.m_Link;
		auto *pPrev = pLink->f_GetPrev();
		if (pPrev == pLink)
			m_pCurrent = nullptr;
		else
			m_pCurrent = pPrev->f_Upcast();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::f_GetCurrent() const
	{
		if (m_pCurrent)
			return TCDLinkListAggregate::fsp_MemberFromLink(m_pCurrent);
		else
			return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator t_CData const * () const
	{
		return f_GetCurrent();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator -> () const
	{
		return f_GetCurrent();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	umint TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::f_GetLen() const
	{
		if (m_pCurrent)
		{
			t_CLinkInList *pCurrent = m_pCurrent;
			umint Len = 0;
			while (!pCurrent->f_IsListLink())
			{
				++Len;
				pCurrent = pCurrent->f_GetNextNotList();
			}
			return Len;
		}
		return 0;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator [] (aint _nPlaces) const
	{
		if (m_pCurrent)
		{
			t_CLinkInList const *pCurrent = m_pCurrent;
			if (_nPlaces > 0)
			{
				while (_nPlaces)
				{
					pCurrent = pCurrent->f_GetNextNotList();
					if (pCurrent->f_IsListLink())
						return nullptr;
					--_nPlaces;
				}
			}
			else
			{
				while (_nPlaces)
				{
					pCurrent = pCurrent->f_GetPrevNotList();
					if (pCurrent->f_IsListLink())
						return nullptr;
					++_nPlaces;
				}
			}
			return TCDLinkListAggregate::fsp_MemberFromLink(pCurrent->f_Upcast());
		}
		return nullptr;
	}

#ifdef DMibPUniqueType_int
	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator [] (int _nPlaces) const
	{
		return operator [] (aint(_nPlaces));
	}
#endif

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator += (aint _nPlaces)
	{
		if (m_pCurrent)
		{
			while (_nPlaces)
			{
				auto *pNext = m_pCurrent->f_GetNextNotList();
				if (pNext->f_IsListLink())
				{
					m_pCurrent = nullptr;
					break;
				}
				else
					m_pCurrent = pNext->f_Upcast();
				--_nPlaces;
			}
		}
		DMibSafeCheck(!_nPlaces, "You tried to access an element outside the list");
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator ++ ()
	{
		if (m_pCurrent)
		{
			auto *pNext = m_pCurrent->f_GetNextNotList();
			if (pNext->f_IsListLink())
				m_pCurrent = nullptr;
			else
				m_pCurrent = pNext->f_Upcast();
		}
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator -- ()
	{
		if (m_pCurrent)
		{
			auto *pPrev = m_pCurrent->f_GetPrevNotList();
			if (pPrev->f_IsListLink())
				m_pCurrent = nullptr;
			else
				m_pCurrent = pPrev->f_Upcast();
		}
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small void TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::operator -= (aint _nPlaces)
	{
		if (m_pCurrent)
		{
			while (_nPlaces)
			{
				auto *pPrev = m_pCurrent->f_GetPrevNotList();
				if (pPrev->f_IsListLink())
				{
					m_pCurrent = nullptr;
					break;
				}
				else
					m_pCurrent = pPrev->f_Upcast();
				--_nPlaces;
			}
		}
		DMibSafeCheck(!_nPlaces, "You tried to access an element outside the list");
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::f_GetIterator() const -> CIteratorConst
	{
		return CIteratorConst(*this);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::operator == (TCDLinkListAggregate const &_Other) const
		noexcept(noexcept(fg_GetType<t_CData const &>() == fg_GetType<t_CData const &>()))
		-> bool
	{
		CIteratorConst Iter0 = *this;
		CIteratorConst Iter1 = _Other;

		while (Iter0 && Iter1)
		{
			if (!(*Iter0 == *Iter1))
				return false;

			++Iter0;
			++Iter1;
		}

		if (Iter0 && !Iter1)
			return false;
		if (!Iter0 && Iter1)
			return false;

		return true;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CLink *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fsp_LinkFromMember(t_CData *_pMember)
	{
		return ((t_CLink *)(((uint8 *)_pMember) + COffset::mc_Offset));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fsp_MemberFromLink(t_CLink *_pLink)
	{
		return ((t_CData *)(((uint8 *)_pLink) - COffset::mc_Offset));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CLink const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fsp_LinkFromMember(t_CData const *_pMember)
	{
		return ((t_CLink const *)(((uint8 *)_pMember) + COffset::mc_Offset));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small t_CData const *TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fsp_MemberFromLink(t_CLink const *_pLink)
	{
		return ((t_CData const *)(((uint8 *)_pLink) - COffset::mc_Offset));
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkList<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::operator == (TCDLinkList const &_Other) const
		noexcept(noexcept((CSuper const &)*this == (CSuper const &)_Other))
		-> bool
	{
		return (CSuper const &)*this == (CSuper const &)_Other;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkList<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::TCDLinkList(TCDLinkList &&_Other)
	{
		CSuper::f_Construct();
		this->f_Transfer(_Other);
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkList<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::operator = (TCDLinkList &&_Other) -> TCDLinkList &
	{
		this->f_Clear();
		this->f_Transfer(_Other);
		return *this;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small TCDLinkList<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::TCDLinkList()
	{
		CSuper::f_Construct();
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	inline_small TCDLinkList<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::~TCDLinkList()
	{
		CSuper::f_Destruct();
	}
}
