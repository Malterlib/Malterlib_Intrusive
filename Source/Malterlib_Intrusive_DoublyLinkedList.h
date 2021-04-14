// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include <Mib/Core/Core>
#include <Mib/Core/OnScopeExit>

namespace NMib::NIntrusive
{
	struct CDLinkAggregateListNoPrevPtr;
	/************************************************************************************************\
	||ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ||
	|| Double linked list
	||______________________________________________________________________________________________||
	\************************************************************************************************/
	struct CDLinkAggregateListNoPrevPtrList
	{
		using CThis = CDLinkAggregateListNoPrevPtrList;
		mint m_pNextPtr;

		constexpr CDLinkAggregateListNoPrevPtrList(EAggregateInitialization _Init)
			: m_pNextPtr{0}
		{
		}
		CDLinkAggregateListNoPrevPtrList()
		{
		}

 		inline_small void fp_Internal_SetNextList(void *_pNext)
		{
			DMibFastCheck(fp_Internal_IsListLink());
			m_pNextPtr = ((mint)_pNext | 1);
		}

 		inline_small void fp_Internal_SetNextInitListLink(void *_pNext)
		{
			m_pNextPtr = ((mint)_pNext | 1);
		}

		inline_small bool fp_Internal_IsListLink() const
		{
			return (m_pNextPtr & 1) != 0;
		}

		inline_small void *fp_Internal_GetNextList() const
		{
			DMibFastCheck(fp_Internal_IsListLink());
			return (void *)(m_pNextPtr & (~1));
		}

		inline_small void fp_SetNextList(CDLinkAggregateListNoPrevPtrList *_pNext)
		{
			fp_Internal_SetNextList(_pNext);
		}

		inline_small void fp_SetNextInitListLink(CDLinkAggregateListNoPrevPtrList *_pNext)
		{
			fp_Internal_SetNextInitListLink(_pNext);
		}

		inline_small void f_ConstructList()
		{
			fp_SetNextInitListLink(nullptr);
		}

		inline_small void *fp_Internal_GetNextNotList() const
		{
			DMibFastCheck(!this->fp_Internal_IsListLink());
			return (void *)m_pNextPtr;
		}

		inline_small bool fp_IsListLink() const
		{
			return fp_Internal_IsListLink();
		}

		inline_small CDLinkAggregateListNoPrevPtrList *fp_GetNextList()
		{
			return (CDLinkAggregateListNoPrevPtrList *)fp_Internal_GetNextList();
		}

		inline_small void fp_SetPrevInitListLink(CDLinkAggregateListNoPrevPtrList *_pPrev)
		{
		}

		inline_small CDLinkAggregateListNoPrevPtrList const *fp_GetNextList() const
		{
			return (CDLinkAggregateListNoPrevPtrList const *)fp_Internal_GetNextList();
		}

		inline_small CDLinkAggregateListNoPrevPtrList *fp_GetNextNotList()
		{
			return (CDLinkAggregateListNoPrevPtrList *)fp_Internal_GetNextNotList();
		}

		inline_small CDLinkAggregateListNoPrevPtrList const *fp_GetNextNotList() const
		{
			return (CDLinkAggregateListNoPrevPtrList const *)fp_Internal_GetNextNotList();
		}

		bool fp_IsValid() const;

		inline_small CDLinkAggregateListNoPrevPtr *f_Upcast();
		inline_small CDLinkAggregateListNoPrevPtr const *f_Upcast() const;

		inline_small CDLinkAggregateListNoPrevPtrList *fp_GetPrevList();
		inline_small CDLinkAggregateListNoPrevPtrList const *fp_GetPrevList() const;
		inline_small void fp_TransferList(CDLinkAggregateListNoPrevPtrList *_pFirst, CDLinkAggregateListNoPrevPtrList *_pLast);
	};

	struct CDLinkAggregateListNoPrevPtr final : public CDLinkAggregateListNoPrevPtrList
	{
		using CThis = CDLinkAggregateListNoPrevPtr;
		// Special link to not need a double link in the list header
		// List header has only next pointer pointing to first item (or itself)
		// The first item has a prev pointer that points to the last item in the list
		// The List header has a bit of the next pointer set to 1 to specify that it's the header

		mint m_pPrevPtr;

		constexpr CDLinkAggregateListNoPrevPtr(EAggregateInitialization _Init)
			: CDLinkAggregateListNoPrevPtrList{_Init}
			, m_pPrevPtr{0}
		{
		}
		CDLinkAggregateListNoPrevPtr()
		{
		}

		inline_small void f_Construct()
		{
			fp_SetNextInit(nullptr);
#			ifdef DMibEnableSafeCheck
				// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
				fp_SetPrevInit(nullptr);
#			endif
		}

		inline_small void fp_Internal_SetPrev(void *_pPrev)
		{
			m_pPrevPtr = (mint)_pPrev;
		}

		inline_small void fp_Internal_SetPrevInit(void *_pPrev)
		{
			m_pPrevPtr = (mint)_pPrev;
		}

		inline_small void *fp_Internal_GetPrev() const
		{
			return (void *)m_pPrevPtr;
		}

		inline_small void fp_Internal_SetNextNotList(void *_pNext)
		{
			DMibFastCheck(!this->fp_Internal_IsListLink());
			m_pNextPtr = (mint)_pNext;
		}

		inline_small void fp_Internal_SetNextInit(void *_pNext)
		{
			m_pNextPtr = (mint)_pNext;
		}

		inline_small void fp_SetNextNotList(CDLinkAggregateListNoPrevPtrList *_pNext)
		{
			fp_Internal_SetNextNotList(_pNext);
		}
		inline_small void fp_SetNextInit(CDLinkAggregateListNoPrevPtrList *_pNext)
		{
			fp_Internal_SetNextInit(_pNext);
		}

		inline_small void fp_SetPrevNotList(CDLinkAggregateListNoPrevPtrList *_pPrev)
		{
			DMibSafeCheck(!fp_IsListLink(), "");
			fp_Internal_SetPrev(_pPrev);
		}

		inline_small void fp_SetPrevInit(CDLinkAggregateListNoPrevPtrList *_pPrev)
		{
			//DMibSafeCheck(!fp_IsListLink(), ""); // this could be uninitialized memory
			fp_Internal_SetPrevInit(_pPrev);
		}

		inline_small CDLinkAggregateListNoPrevPtr *fp_GetPrevPtr()
		{
			return ((CDLinkAggregateListNoPrevPtr *)fp_Internal_GetPrev());
		}

		inline_small CDLinkAggregateListNoPrevPtr const *fp_GetPrevPtr() const
		{
			return ((CDLinkAggregateListNoPrevPtr *)fp_Internal_GetPrev());
		}

		inline_small CDLinkAggregateListNoPrevPtrList *fp_GetPrevNotList()
		{
			DMibFastCheck(!fp_IsListLink());
			auto pPrev = fp_GetPrevPtr();
			auto pPrevNext = pPrev->fp_GetNextNotList();
			if (pPrevNext->fp_IsListLink())
				return pPrevNext;
			else
				return pPrev;
		}

		inline_small CDLinkAggregateListNoPrevPtr *fp_GetPrevNoList()
		{
			DMibFastCheck(!fp_IsListLink());
			auto pPrev = fp_GetPrevPtr();
			DMibFastCheck(!pPrev->fp_GetNextNotList()->fp_IsListLink());
			return pPrev;
		}

		inline_small CDLinkAggregateListNoPrevPtrList const *fp_GetPrevNotList() const
		{
			DMibFastCheck(!fp_IsListLink());
			auto pPrev = fp_GetPrevPtr();
			auto pPrevNext = pPrev->fp_GetNextNotList();
			if (pPrevNext->fp_IsListLink())
				return pPrevNext;
			else
				return pPrev;
		}

		inline_small CDLinkAggregateListNoPrevPtr const *fp_GetPrevNoList() const
		{
			DMibFastCheck(!fp_IsListLink());
			auto pPrev = fp_GetPrevPtr();
			DMibFastCheck(!pPrev->fp_GetNextNotList()->fp_IsListLink());
			return pPrev;
		}

		inline_small void fp_Link(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
		{
			DMibSafeCheck(!fp_GetNextNotList(), "Must not be in list here");
			fp_LinkNoUnlink(_pLinkAfter);
		}

		inline_small void fp_LinkFirst(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
		{
			DMibSafeCheck(!fp_GetNextNotList(), "Must not be in list here");
			fp_LinkNoUnlinkFirst(_pLinkAfter);
		}

		inline_small void fp_LinkLast(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
		{
			DMibSafeCheck(!fp_GetNextNotList(), "Must not be in list here");
			fp_LinkNoUnlinkLast(_pLinkAfter);
		}

		static inline_small void fs_UnlinkRange(CDLinkAggregateListNoPrevPtr *_pFirst, CDLinkAggregateListNoPrevPtr *_pLast)
		{
			auto *pFirst = _pFirst->fp_GetPrevNotList();
			auto *pLast = _pLast->fp_GetNextNotList();

			if (pFirst == pLast)
			{
				DMibSafeCheck(pFirst->fp_IsListLink(), "Must be a list link");
				// We should be an empty list pointing to ourselves
				pFirst->fp_SetNextInitListLink(pFirst);
			}
			else if (pFirst->fp_IsListLink())
			{
				// The first pointer is a list link
				auto *pLastInList = pFirst->fp_GetPrevList()->f_Upcast();
				pFirst->fp_SetNextInitListLink(pLast);
				pLast->f_Upcast()->fp_SetPrevInit(pLastInList);
			}
			else if (pLast->fp_IsListLink())
			{
				// The last pointer is a list link
				auto *pFirstInList = pLast->fp_GetNextList();
				pFirst->f_Upcast()->fp_SetNextInit(pLast);
				pFirstInList->f_Upcast()->fp_SetPrevInit(pFirst);
			}
			else
			{
				pFirst->f_Upcast()->fp_SetNextInit(pLast);
				pLast->f_Upcast()->fp_SetPrevInit(pFirst);
			}
		}

		static inline_small void fs_LinkRange(CDLinkAggregateListNoPrevPtr *_pFirst, CDLinkAggregateListNoPrevPtr *_pLast, CDLinkAggregateListNoPrevPtrList *_pInsertAfter)
		{
			DMibSafeCheck(!_pFirst->fp_IsListLink(), "Cannot insert empty list");
			DMibSafeCheck(!_pLast->fp_IsListLink(), "Cannot insert empty list");

			if (_pInsertAfter->fp_IsListLink())
			{
				auto *pInsertAfterNext = _pInsertAfter->fp_GetNextList();
				DMibSafeCheck(pInsertAfterNext != _pInsertAfter, "Cannot insert into empty list");
				auto *pLastInList = _pInsertAfter->fp_GetPrevList();
				// Insert at beginning of list
				_pFirst->fp_SetPrevInit(pLastInList);
				_pInsertAfter->fp_SetNextInitListLink(_pFirst);

				_pLast->fp_SetNextInit(pInsertAfterNext);
				pInsertAfterNext->f_Upcast()->fp_SetPrevInit(_pLast);
			}
			else
			{
				auto *pInsertAfterNext = _pInsertAfter->fp_GetNextNotList();
				DMibSafeCheck(pInsertAfterNext != _pInsertAfter, "Cannot insert into empty list");
				if (pInsertAfterNext->fp_IsListLink())
				{
					// Insert at end of list
					auto *pFirstInList = pInsertAfterNext->fp_GetNextList();

					_pFirst->fp_SetPrevInit(_pInsertAfter);
					_pInsertAfter->f_Upcast()->fp_SetNextInit(_pFirst);

					_pLast->fp_SetNextInit(pInsertAfterNext);
					pFirstInList->f_Upcast()->fp_SetPrevInit(_pLast);
				}
				else
				{
					// Middle of list
					_pFirst->fp_SetPrevInit(_pInsertAfter);
					_pInsertAfter->f_Upcast()->fp_SetNextInit(_pFirst);

					_pLast->fp_SetNextInit(pInsertAfterNext);
					pInsertAfterNext->f_Upcast()->fp_SetPrevInit(_pLast);
				}
			}
		}

		inline_extralarge void fp_LinkNoUnlink(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
		{
			if (unlikely(_pLinkAfter->fp_IsListLink()))
			{
				auto *pAfterNext = _pLinkAfter->fp_GetNextList();
				if (pAfterNext == _pLinkAfter)
				{
					// Empty list
					fp_SetPrevInit(this);
					fp_SetNextInit(_pLinkAfter);
					_pLinkAfter->fp_SetNextList(this);
					return;
				}

				// First in list

				fp_SetPrevInit(pAfterNext->f_Upcast()->fp_GetPrevPtr());
				fp_SetNextInit(pAfterNext);
				pAfterNext->f_Upcast()->fp_SetPrevNotList(this);
				_pLinkAfter->fp_SetNextList(this);
			}
			else
			{
				auto *pAfterNext = _pLinkAfter->fp_GetNextNotList();
				if (unlikely(pAfterNext->fp_IsListLink()))
				{
					// Last in list
					fp_SetPrevInit(_pLinkAfter);
					fp_SetNextInit(pAfterNext);
					pAfterNext->fp_GetNextList()->f_Upcast()->fp_SetPrevNotList(this);
					_pLinkAfter->f_Upcast()->fp_SetNextNotList(this);
				}
				else
				{
					fp_SetPrevInit(_pLinkAfter);
					fp_SetNextInit(pAfterNext);
					pAfterNext->f_Upcast()->fp_SetPrevNotList(this);
					_pLinkAfter->f_Upcast()->fp_SetNextNotList(this);
				}
			}
		}

		inline_extralarge void fp_LinkNoUnlinkNotList(CDLinkAggregateListNoPrevPtr *_pLinkAfter)
		{
			DMibFastCheck(!_pLinkAfter->fp_IsListLink());
			auto *pAfterNext = _pLinkAfter->fp_GetNextNotList();
			DMibFastCheck(!pAfterNext->fp_IsListLink());
			fp_SetPrevInit(_pLinkAfter);
			fp_SetNextInit(pAfterNext);
			pAfterNext->f_Upcast()->fp_SetPrevNotList(this);
			_pLinkAfter->fp_SetNextNotList(this);
		}

		inline_extralarge void fp_LinkNoUnlinkFirst(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
		{
			DMibFastCheck(_pLinkAfter->fp_IsListLink());

			auto *pAfterNext = _pLinkAfter->fp_GetNextList();
			if (pAfterNext == _pLinkAfter)
			{
				// Empty list
				fp_SetPrevInit(this);
				fp_SetNextInit(_pLinkAfter);
				_pLinkAfter->fp_SetNextList(this);
				return;
			}

			// First in list

			fp_SetPrevInit(pAfterNext->f_Upcast()->fp_GetPrevPtr());
			fp_SetNextInit(pAfterNext);
			pAfterNext->f_Upcast()->fp_SetPrevNotList(this);
			_pLinkAfter->fp_SetNextList(this);
		}

		inline_extralarge void fp_LinkNoUnlinkLast(CDLinkAggregateListNoPrevPtrList *_pLinkAfter)
		{
			if (unlikely(_pLinkAfter->fp_IsListLink()))
			{
				DMibFastCheck(_pLinkAfter->fp_GetNextList() == _pLinkAfter);
				// Empty list
				fp_SetPrevInit(this);
				fp_SetNextInit(_pLinkAfter);
				_pLinkAfter->fp_SetNextList(this);
				return;
			}
			else
			{
				auto *pAfterNext = _pLinkAfter->fp_GetNextNotList();
				DMibFastCheck(pAfterNext->fp_IsListLink());
				// Last in list
				fp_SetPrevInit(_pLinkAfter);
				fp_SetNextInit(pAfterNext);
				pAfterNext->fp_GetNextList()->f_Upcast()->fp_SetPrevNotList(this);
				_pLinkAfter->f_Upcast()->fp_SetNextNotList(this);
			}
		}

		inline_small void fp_UnlinkFirstInternal()
		{
			auto pPrevPtr = fp_GetPrevPtr();
			auto pPrev = pPrevPtr->fp_GetNextNotList();
			DMibFastCheck(pPrev->fp_IsListLink());
			auto pNext = fp_GetNextNotList();
			pPrev->fp_SetNextList(pNext);
			if (likely(!pNext->fp_IsListLink()))
				pNext->f_Upcast()->fp_SetPrevNotList(pPrevPtr);
			else
			{
				// Alone in list
				DMibFastCheck(pNext == pPrev);
			}
		}

		inline_small void fp_UnlinkInternal()
		{
			auto pPrev = fp_GetPrevNotList();
			auto pNext = fp_GetNextNotList();
			if (unlikely(pPrev->fp_IsListLink()))
			{
				pPrev->fp_SetNextList(pNext);
				if (likely(!pNext->fp_IsListLink()))
					pNext->f_Upcast()->fp_SetPrevNotList(fp_GetPrevPtr());
				else
				{
					// Alone in list
					DMibFastCheck(pNext == pPrev);
				}
			}
			else
			{
				pPrev->f_Upcast()->fp_SetNextNotList(pNext);

				if (unlikely(pNext->fp_IsListLink())) // Last in list
					pNext->fp_GetNextList()->f_Upcast()->fp_SetPrevNotList(pPrev);
				else // Between two usual blocks
					pNext->f_Upcast()->fp_SetPrevNotList(pPrev);
			}
		}

		// Public functions
		inline_extralarge void f_Unlink()
		{
			if (fp_GetNextNotList())
			{
				fp_UnlinkInternal();
				fp_SetNextInit(nullptr);
#				ifdef DMibEnableSafeCheck
					// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
				fp_SetPrevInit(nullptr);
#				endif
			}
		}
		inline_small void f_UnlinkFirst()
		{
			if (fp_GetNextNotList())
			{
				fp_UnlinkFirstInternal();
				fp_SetNextInit(nullptr);
#				ifdef DMibEnableSafeCheck
					// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
				fp_SetPrevInit(nullptr);
#				endif
			}
		}

		inline_small void f_UnsafeUnlink()
		{
			fp_UnlinkInternal();
		}

		inline_small void f_UnsafeUnlinkFirst()
		{
			fp_UnlinkFirstInternal();
		}

		inline_small void f_UnlinkLinked()
		{
			fp_UnlinkInternal();
			fp_SetNextInit(nullptr);
#			ifdef DMibEnableSafeCheck
			// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
			fp_SetPrevInit(nullptr);
#			endif
		}

		inline_small void f_UnlinkFirstLinked()
		{
			fp_UnlinkFirstInternal();
			fp_SetNextInit(nullptr);
#			ifdef DMibEnableSafeCheck
			// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
			fp_SetPrevInit(nullptr);
#			endif
		}

		inline_small void f_Destruct()
		{
			if (fp_GetNextNotList())
			{
				fp_UnlinkInternal();
#				ifdef DMibEnableSafeCheck
				fp_SetNextInit(nullptr);
				fp_SetPrevInit(nullptr);
#				endif
			}
		}

		inline_small bool f_IsAloneInList() const
		{
			return f_IsInList() && fp_GetPrevPtr() == this;
		}

		inline_small bool f_IsInList() const
		{
			return fp_GetNextNotList() != nullptr;
		}
	};

	inline_small CDLinkAggregateListNoPrevPtr *CDLinkAggregateListNoPrevPtrList::f_Upcast()
	{
		return static_cast<CDLinkAggregateListNoPrevPtr *>(this);
	}

	inline_small CDLinkAggregateListNoPrevPtr const *CDLinkAggregateListNoPrevPtrList::f_Upcast() const
	{
		return static_cast<CDLinkAggregateListNoPrevPtr const *>(this);
	}

	inline_small CDLinkAggregateListNoPrevPtrList *CDLinkAggregateListNoPrevPtrList::fp_GetPrevList()
	{
		DMibFastCheck(fp_IsListLink());
		auto pNext = fp_GetNextList();
		if (pNext->fp_IsListLink())
			return this;
		else
			return pNext->f_Upcast()->fp_GetPrevPtr();
	}

	inline_small CDLinkAggregateListNoPrevPtrList const *CDLinkAggregateListNoPrevPtrList::fp_GetPrevList() const
	{
		DMibFastCheck(fp_IsListLink());
		auto pNext = fp_GetNextList();
		if (pNext->fp_IsListLink())
			return this;
		else
			return pNext->f_Upcast()->fp_GetPrevPtr();
	}

	inline_small void CDLinkAggregateListNoPrevPtrList::fp_TransferList(CDLinkAggregateListNoPrevPtrList *_pFirst, CDLinkAggregateListNoPrevPtrList *_pLast)
	{
		if (_pFirst->fp_IsListLink())
		{
			// Empty list
			f_ConstructList();
		}
		else
		{
			fp_SetNextInitListLink(_pFirst);
			_pFirst->f_Upcast()->fp_SetPrevInit(_pLast);
			_pLast->f_Upcast()->fp_SetNextInit(this);
		}
	}

	inline bool CDLinkAggregateListNoPrevPtrList::fp_IsValid() const
	{
		if (fp_IsListLink())
		{
			auto *pNext = fp_GetNextList();
			if (pNext == this)
				return true;
			if (pNext->f_Upcast()->fp_GetPrevPtr()->fp_GetNextNotList() != this)
				return false;
		}

		return true;
	}

	class CDLinkAggregate final
	{
		typedef CDLinkAggregate CThis;
	public:
		CDLinkAggregate *m_pNextPtr;
		CDLinkAggregate *m_pPrevPtr;

		constexpr CDLinkAggregate(EAggregateInitialization _Init)
			: m_pNextPtr{nullptr}
			, m_pPrevPtr{nullptr}
		{
		}
		CDLinkAggregate()
		{
		}

		bool fp_IsValid() const
		{
			return true;
		}

		inline_extralarge void fp_Link(CDLinkAggregate *_pLinkAfter)
		{
			DMibSafeCheck(!fp_GetNext(), "Must not be in list here");

			fp_SetPrevInit(_pLinkAfter);
			fp_SetNext(_pLinkAfter->fp_GetNext());
			fp_GetNext()->fp_SetPrev(this);
			fp_GetPrev()->fp_SetNext(this);
		}
		inline_small void fp_LinkFirst(CDLinkAggregate *_pLinkAfter)
		{
			fp_Link(_pLinkAfter);
		}
		inline_small void fp_LinkLast(CDLinkAggregate *_pLinkAfter)
		{
			fp_Link(_pLinkAfter);
		}

		static inline_small void fs_UnlinkRange(CDLinkAggregate *_pFirst, CDLinkAggregate *_pLast)
		{
			CThis *pFirst = _pFirst->fp_GetPrev();
			CThis *pLast = _pLast->fp_GetNext();
			pFirst->fp_SetNext(pLast);
			pLast->fp_SetPrev(pFirst);
		}

		static inline_small void fs_LinkRange(CDLinkAggregate *_pFirst, CDLinkAggregate *_pLast, CDLinkAggregate *_pInsertAfter)
		{
			CThis *pInsertAfterNext = _pInsertAfter->fp_GetNext();
			_pFirst->fp_SetPrev(_pInsertAfter);
			_pInsertAfter->fp_SetNext(_pFirst);

			_pLast->fp_SetNext(pInsertAfterNext);
			pInsertAfterNext->fp_SetPrev(_pLast);
		}

		inline_small void fp_LinkNoUnlink(CDLinkAggregate *_pLinkAfter)
		{
			fp_SetPrevInit(_pLinkAfter);
			fp_SetNext(_pLinkAfter->fp_GetNext());
			fp_GetNext()->fp_SetPrev(this);
			fp_GetPrev()->fp_SetNext(this);
		}
		inline_small void fp_LinkNoUnlinkNotList(CDLinkAggregate *_pLinkAfter)
		{
			fp_LinkNoUnlink(_pLinkAfter);
		}
		inline_small void fp_LinkNoUnlinkFirst(CDLinkAggregate *_pLinkAfter)
		{
			fp_LinkNoUnlink(_pLinkAfter);
		}
		inline_small void fp_LinkNoUnlinkLast(CDLinkAggregate *_pLinkAfter)
		{
			fp_LinkNoUnlink(_pLinkAfter);
		}

		inline_small void fp_SetNext(CDLinkAggregate *_pNext)
		{
			m_pNextPtr = _pNext;
		}
		inline_small void fp_SetNextNotList(CDLinkAggregate *_pNext)
		{
			m_pNextPtr = _pNext;
		}
		inline_small void fp_SetNextList(CDLinkAggregate *_pNext)
		{
			m_pNextPtr = _pNext;
		}

		inline_small void fp_SetNextInit(CDLinkAggregate *_pNext)
		{
			m_pNextPtr = _pNext;
		}
		inline_small void fp_SetNextInitListLink(CDLinkAggregate *_pNext)
		{
			m_pNextPtr = _pNext;
		}
		inline_small void fp_SetPrev(CDLinkAggregate *_pPrev)
		{
			m_pPrevPtr = (CDLinkAggregate *)(((mint)(CDLinkAggregate *)m_pPrevPtr & 1) | (mint)_pPrev);
		}
		inline_small void fp_SetPrevNotList(CDLinkAggregate *_pPrev)
		{
			m_pPrevPtr = (CDLinkAggregate *)(((mint)(CDLinkAggregate *)m_pPrevPtr & 1) | (mint)_pPrev);
		}
		inline_small void fp_SetPrevList(CDLinkAggregate *_pPrev)
		{
			m_pPrevPtr = (CDLinkAggregate *)(((mint)(CDLinkAggregate *)m_pPrevPtr & 1) | (mint)_pPrev);
		}
		inline_small void fp_SetPrevInit(CDLinkAggregate *_pPrev)
		{
			m_pPrevPtr = _pPrev;
		}
		inline_small void fp_SetPrevInitListLink(CDLinkAggregate *_pPrev)
		{
			m_pPrevPtr = (CDLinkAggregate *)((mint)_pPrev | 1);
		}

		inline_small bool fp_IsListLink() const
		{
			return ((mint)(CDLinkAggregate *)m_pPrevPtr & 1);
		}

		inline_small const CDLinkAggregate *fp_GetNext() const
		{
			return m_pNextPtr;
		}

		inline_small const CDLinkAggregate *fp_GetNextList() const
		{
			return m_pNextPtr;
		}

		inline_small const CDLinkAggregate *fp_GetNextNotList() const
		{
			return m_pNextPtr;
		}

		inline_small const CDLinkAggregate *fp_GetPrev() const
		{
			return (CDLinkAggregate *)((mint)(CDLinkAggregate *)m_pPrevPtr & (~1));
		}
		inline_small const CDLinkAggregate *fp_GetPrevList() const
		{
			return (CDLinkAggregate *)((mint)(CDLinkAggregate *)m_pPrevPtr & (~1));
		}
		inline_small const CDLinkAggregate *fp_GetPrevNotList() const
		{
			return (CDLinkAggregate *)((mint)(CDLinkAggregate *)m_pPrevPtr & (~1));
		}
		inline_small const CDLinkAggregate *fp_GetPrevNoList() const
		{
			return (CDLinkAggregate *)((mint)(CDLinkAggregate *)m_pPrevPtr & (~1));
		}

		inline_small CDLinkAggregate *fp_GetNext()
		{
			return m_pNextPtr;
		}

		inline_small CDLinkAggregate *fp_GetNextList()
		{
			return m_pNextPtr;
		}

		inline_small CDLinkAggregate *fp_GetNextNotList()
		{
			return m_pNextPtr;
		}

		inline_small CDLinkAggregate *fp_GetPrev()
		{
			return (CDLinkAggregate *)((mint)(CDLinkAggregate *)m_pPrevPtr & (~1));
		}
		inline_small CDLinkAggregate *fp_GetPrevNotList()
		{
			return (CDLinkAggregate *)((mint)(CDLinkAggregate *)m_pPrevPtr & (~1));
		}
		inline_small CDLinkAggregate *fp_GetPrevNoList()
		{
			return (CDLinkAggregate *)((mint)(CDLinkAggregate *)m_pPrevPtr & (~1));
		}
		inline_small CDLinkAggregate *fp_GetPrevList()
		{
			return (CDLinkAggregate *)((mint)(CDLinkAggregate *)m_pPrevPtr & (~1));
		}

		inline_small void fp_TransferList(CDLinkAggregate *_pFirst, CDLinkAggregate *_pLast)
		{
			fp_SetNextInitListLink(_pFirst);
			fp_GetNext()->fp_SetPrev(this);
			fp_SetPrevInitListLink(_pLast);
			fp_GetPrev()->fp_SetNext(this);
		}

		// Public functions

		inline_small CDLinkAggregate *f_Upcast()
		{
			return this;
		}

		inline_small CDLinkAggregate const *f_Upcast() const
		{
			return this;
		}

		inline_small bool f_IsAloneInList() const
		{
			return f_IsInList() && (fp_GetPrev()->fp_IsListLink()) && (fp_GetNext()->fp_IsListLink());
		}

		inline_extralarge void f_Unlink()
		{
			if (fp_GetNext())
			{
				fp_GetNext()->fp_SetPrev(fp_GetPrev());
				fp_GetPrev()->fp_SetNext(fp_GetNext());
				fp_SetNext(nullptr);
#				ifdef DMibEnableSafeCheck
					// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
				fp_SetPrevInit(nullptr);
#				endif
			}
		}
		inline_small void f_UnlinkFirst()
		{
			f_Unlink();
		}

		inline_small void f_UnsafeUnlink()
		{
			fp_GetNext()->fp_SetPrev(fp_GetPrev());
			fp_GetPrev()->fp_SetNext(fp_GetNext());
		}
		inline_small void f_UnsafeUnlinkFirst()
		{
			f_UnsafeUnlink();
		}

		inline_small void f_UnlinkLinked()
		{
			fp_GetNext()->fp_SetPrev(fp_GetPrev());
			fp_GetPrev()->fp_SetNext(fp_GetNext());
			fp_SetNext(nullptr);
#				ifdef DMibEnableSafeCheck
			// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
			fp_SetPrevInit(nullptr);
#				endif
		}

		inline_small void f_UnlinkFirstLinked()
		{
			f_UnlinkLinked();
		}

		inline_small void f_ConstructList()
		{
			fp_SetNext(nullptr);
#			ifdef DMibEnableSafeCheck
				// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
				fp_SetPrevInit(nullptr);
#			endif
		}

		inline_small void f_Construct()
		{
			fp_SetNext(nullptr);
#			ifdef DMibEnableSafeCheck
				// Clear the prev ptr in debug so ppl don't get confused about garbled data :)
				fp_SetPrevInit(nullptr);
#			endif
		}

		inline_small void f_Destruct()
		{
			if (fp_GetNext())
			{
				fp_GetNext()->fp_SetPrev(fp_GetPrev());
				fp_GetPrev()->fp_SetNext(fp_GetNext());
#				ifdef DMibEnableSafeCheck
				fp_SetNext(nullptr);
				fp_SetPrevInit(nullptr);
#				endif
			}
		}

		inline_small bool f_IsInList() const
		{
			return fp_GetNext() != nullptr;
		}
	};

	template <typename t_CLink>
	class TCDLink final
	{
	private:
		TCDLink(TCDLink const &) = delete;
		TCDLink &operator = (TCDLink const &) = delete;

		t_CLink m_Link;
	public:

		inline_small TCDLink()
		{
			m_Link.f_Construct();
		}

		TCDLink(TCDLink &&_Other)
		{
			if (_Other.m_Link.f_IsInList())
			{
				auto pOtherPrev = _Other.m_Link.fp_GetPrevNotList();
				_Other.m_Link.f_UnlinkLinked();
				m_Link.fp_LinkNoUnlink(pOtherPrev);
			}
			else
				m_Link.f_Construct();
		}

		TCDLink &operator =(TCDLink &&_Other)
		{
			m_Link.f_Destruct();
			if (_Other.m_Link.f_IsInList())
			{
				auto pOtherPrev = _Other.m_Link.fp_GetPrevNotList();
				_Other.m_Link.f_UnlinkLinked();
				m_Link.fp_LinkNoUnlink(pOtherPrev);
			}
			else
				m_Link.f_Construct();
			return *this;
		}

		inline_small ~TCDLink()
		{
			m_Link.f_Destruct();
		}

		inline_small t_CLink const *f_GetNext() const
		{
			return (t_CLink const *)m_Link.fp_GetNextNotList();
		}

		inline_small t_CLink const *f_GetPrev() const
		{
			return (t_CLink const *)m_Link.fp_GetPrevNotList();
		}
/*
		inline_small void fp_Link(t_CLink *_pLinkAfter)
		{
			m_Link.fp_Link(_pLinkAfter);
		}

		inline_small void fp_LinkNoUnlink(t_CLink *_pLinkAfter)
		{
			m_Link.fp_LinkNoUnlink(_pLinkAfter);
		}

		inline_small void fp_SetNext(t_CLink *_pNext)
		{
			m_Link.fp_SetNextNotList(_pNext);
		}
		inline_small void fp_SetNextInit(t_CLink *_pNext)
		{
			m_Link.fp_SetNextInit(_pNext);
		}
		inline_small void fp_SetNextInitListLink(t_CLink *_pNext)
		{
			m_Link.fp_SetNextInitListLink(_pNext);
		}
		inline_small void fp_SetPrev(t_CLink *_pPrev)
		{
			m_Link.fp_SetPrevNotList(_pPrev);
		}
		inline_small void fp_SetPrevInit(t_CLink *_pPrev)
		{
			m_Link.fp_SetPrevInit(_pPrev);
		}
		inline_small void fp_SetPrevInitListLink(t_CLink *_pPrev)
		{
			m_Link.fp_SetPrevInitListLink(_pPrev);
		}

		inline_small bool fp_IsListLink() const
		{
			return m_Link.fp_IsListLink();
		}

		inline_small const t_CLink *fp_GetNext() const
		{
			return m_Link.fp_GetNextNotList();
		}

		inline_small t_CLink *fp_GetNext()
		{
			return m_Link.fp_GetNextNotList();
		}

		inline_small const t_CLink *fp_GetPrev() const
		{
			return m_Link.fp_GetPrevNotList();
		}
		inline_small t_CLink *fp_GetPrev()
		{
			return m_Link.fp_GetPrevNotList();
		}
 */

		// Public functions
		inline_small void f_Unlink()
		{
			m_Link.f_Unlink();
		}

		inline_small void f_UnsafeUnlink()
		{
			m_Link.f_UnsafeUnlink();
		}

		inline_small void f_UnlinkLinked()
		{
			m_Link.f_UnlinkLinked();
		}

		// First

		inline_small void f_UnlinkFirst()
		{
			m_Link.f_UnlinkFirst();
		}

		inline_small void f_UnsafeUnlinkFirst()
		{
			m_Link.f_UnsafeUnlinkFirst();
		}

		inline_small void f_UnlinkFirstLinked()
		{
			m_Link.f_UnlinkFirstLinked();
		}

		inline_small bool f_IsAloneInList() const
		{
			return m_Link.f_IsAloneInList();
		}

		inline_small bool f_IsInList() const
		{
			return m_Link.f_IsInList();
		}
	};

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	class TCDLinkListAggregate
	{

	public:
		typedef t_CLink CLink;
		typedef t_CTranslator CTranslator;
		typedef typename CTranslator::template TCOffset<t_CData> COffset;
#ifdef DMibDebuggerHelpers
		static COffset fs_Debug_GetOffset();
#endif

		static inline_small t_CLink *fp_LinkFromMember(t_CData *_pMember)
		{
			return ((t_CLink *)(((uint8 *)_pMember) + COffset::mc_Offset));
		}

		static inline_small t_CData *fp_MemberFromLink(t_CLink *_pLink)
		{
			return ((t_CData *)(((uint8 *)_pLink) - COffset::mc_Offset));
		}

		static inline_small const t_CLink *fp_LinkFromMember(const t_CData *_pMember)
		{
			return ((const t_CLink *)(((uint8 *)_pMember) + COffset::mc_Offset));
		}

		static inline_small const t_CData *fp_MemberFromLink(const t_CLink *_pLink)
		{
			return ((const t_CData *)(((uint8 *)_pLink) - COffset::mc_Offset));
		}


		// Have first and last as dummylinks so they don't have destructors and constructors
//			t_CLink m_First;
//			t_CLink m_Last;
		t_CLinkInList m_Link;

		constexpr TCDLinkListAggregate(EAggregateInitialization _Init)
			: m_Link{_Init}
		{
#ifdef DMibDebuggerHelpers
			static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
		}
		TCDLinkListAggregate()
		{
#ifdef DMibDebuggerHelpers
			static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
		}

		inline_small void f_Construct()
		{
#ifdef DMibDebuggerHelpers
			static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
#ifdef DMibDebug
			if ((mint)this & 1)
				DMibPDebugBreak; // We must be aligned
#endif

			m_Link.fp_SetNextInitListLink(&m_Link);
			m_Link.fp_SetPrevInitListLink(&m_Link);
		}

		template <typename t_CDataInternal1, typename t_CAllocator1, bool _bAutoDelete1>
		class CAutoDeleteHelper
		{
		public:
			static inline_small void fs_Delete(t_CData *_pDeleter)
			{
				fg_DeleteObject(t_CAllocator(), _pDeleter);
			}
		};

		template <typename t_CDataInternal1, typename t_CAllocator1>
		class CAutoDeleteHelper<t_CDataInternal1, t_CAllocator1, 0>
		{
		public:
			static inline_small void fs_Delete(t_CData *_pDeleter)
			{
			}
		};

		inline_small void f_Destruct()
		{
			if constexpr (t_bAutoDelete)
			{
				while(t_CData *pCurrent = f_GetLast())
				{
					// We must use this special template or objects that cannot be deleted will get a compile error here even if t_bAutoDelete is false
					CAutoDeleteHelper<t_CData, t_CAllocator, t_bAutoDelete>::fs_Delete(pCurrent);
				}
			}
			else
				f_Clear();
		}

		void f_Clear()
		{
			auto pLink = &m_Link;
			auto pNext = m_Link.fp_GetNextList();

			while (pNext != pLink)
			{
				auto pCurrent = pNext;
				pNext = pNext->fp_GetNextNotList();

				pCurrent->f_Upcast()->fp_SetNextInit(nullptr);
#				ifdef DMibEnableSafeCheck
				pCurrent->f_Upcast()->fp_SetPrevInit(nullptr);
#				endif
			}

			pLink->fp_SetNextInitListLink(pLink);
			pLink->fp_SetPrevInitListLink(pLink);

		}

		inline_medium void f_Transfer(TCDLinkListAggregate *_pFrom)
		{
			DMibSafeCheck(f_IsEmpty(), "We must be empty to be able to transfer from another list");
			if (_pFrom->f_IsEmpty())
				return;
			f_UnsafeTransfer(_pFrom);
			_pFrom->f_Construct();
		}

		inline_medium void f_UnsafeTransfer(TCDLinkListAggregate *_pFrom)
		{
			auto *pFromLink = &_pFrom->m_Link;
			m_Link.fp_TransferList(pFromLink->fp_GetNextList(), pFromLink->fp_GetPrevList());
		}

		inline_small void f_Transfer(TCDLinkListAggregate &_From)
		{
			f_Transfer(&_From);
		}

		inline_small void f_UnsafeTransfer(TCDLinkListAggregate &_From)
		{
			f_UnsafeTransfer(&_From);
		}

		inline_small t_CData *f_GetFirst()
		{
			auto pLink = &m_Link;
			auto pNext = pLink->fp_GetNextList();
			if (pNext != pLink)
				return fp_MemberFromLink(pNext->f_Upcast());
			else
				return nullptr;
		}

		inline_small t_CData *f_GetLast()
		{
			auto pLink = &m_Link;
			auto pPrev = pLink->fp_GetPrevList();
			if (pPrev != pLink)
				return fp_MemberFromLink(pPrev->f_Upcast());
			else
				return nullptr;
		}

		inline_small const t_CData *f_GetFirst() const
		{
			auto pLink = &m_Link;
			auto pNext = pLink->fp_GetNextList();
			if (pNext != pLink)
				return fp_MemberFromLink(pNext->f_Upcast());
			else
				return nullptr;
		}

		inline_small const t_CData *f_GetLast() const
		{
			auto pLink = &m_Link;
			auto pPrev = pLink->fp_GetPrevList();
			if (pPrev != pLink)
				return fp_MemberFromLink(pPrev->f_Upcast());
			else
				return nullptr;
		}

		static inline_small t_CData *fs_GetNextUnsafe(t_CData *_pCurrent)
		{
			auto *pNext = TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->fp_GetNextNotList();
			if (!pNext->fp_IsListLink())
				return TCDLinkListAggregate::fp_MemberFromLink(pNext->f_Upcast());
			else
				return nullptr;
		}

		static inline_small t_CData *fs_GetNext(t_CData *_pCurrent)
		{
			auto *pNext = TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->fp_GetNextNotList();
			if (pNext && !pNext->fp_IsListLink())
				return TCDLinkListAggregate::fp_MemberFromLink(pNext->f_Upcast());
			else
				return nullptr;
		}

		static inline_small t_CData *fs_GetPrevUnsafe(t_CData *_pCurrent)
		{
			auto *pPrev = TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->fp_GetPrevNotList();
			if (!pPrev->fp_IsListLink())
				return TCDLinkListAggregate::fp_MemberFromLink(pPrev->f_Upcast());
			else
				return nullptr;
		}

		static inline_small t_CData *fs_GetPrev(t_CData *_pCurrent)
		{
			if (!TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->f_IsInList())
				return nullptr;

			return fs_GetPrevUnsafe(_pCurrent);
		}

		static inline_small const t_CData *fs_GetNextUnsafe(const t_CData *_pCurrent)
		{
			auto const *pNext = TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->fp_GetNextNotList();
			if (!pNext->fp_IsListLink())
				return TCDLinkListAggregate::fp_MemberFromLink(pNext->f_Upcast());
			else
				return nullptr;
		}


		static inline_small const t_CData *fs_GetNext(const t_CData *_pCurrent)
		{
			auto const *pNext = TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->fp_GetNextNotList();
			if (pNext && !pNext->fp_IsListLink())
				return TCDLinkListAggregate::fp_MemberFromLink(pNext->f_Upcast());
			else
				return nullptr;
		}

		static inline_small const t_CData *fs_GetPrevUnsafe(const t_CData *_pCurrent)
		{
			auto const *pPrev = TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->fp_GetPrevNotList();
			if (!pPrev->fp_IsListLink())
				return TCDLinkListAggregate::fp_MemberFromLink(pPrev->f_Upcast());
			else
				return nullptr;
		}

		static inline_small const t_CData *fs_GetPrev(const t_CData *_pCurrent)
		{
			if (!TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->f_IsInList())
				return nullptr;
			return fs_GetPrevUnsafe(_pCurrent);
		}


		void f_DeleteAll()
		{
			while (t_CData *pCurrent = f_GetLast())
			{
				fg_DeleteObject(t_CAllocator(), pCurrent);
			}
		}

		template <typename t_CAllocator2>
		void f_DeleteAllAllocator()
		{
			while (t_CData *pCurrent = f_GetLast())
			{
				fg_DeleteObject(t_CAllocator2(), pCurrent);
			}
		}

		template <typename t_CAllocator2>
		void f_DeleteAllAllocator(t_CAllocator2 &_Allocator)
		{
			while (t_CData *pCurrent = f_GetLast())
			{
				fg_DeleteObject(_Allocator, pCurrent);
			}
		}

		void f_DeleteAllDefiniteType()
		{
			while (t_CData *pCurrent = f_GetLast())
			{
				fg_DeleteObjectDefiniteType(t_CAllocator(), pCurrent);
			}
		}

		template <typename t_CAllocator2>
		void f_DeleteAllAllocatorDefiniteType()
		{
			while (t_CData *pCurrent = f_GetLast())
			{
				fg_DeleteObjectDefiniteType(t_CAllocator2(), pCurrent);
			}
		}

		template <typename t_CAllocator2>
		void f_DeleteAllAllocatorDefiniteType(t_CAllocator2 &_Allocator)
		{
			while (t_CData *pCurrent = f_GetLast())
			{
				fg_DeleteObjectDefiniteType(_Allocator, pCurrent);
			}
		}

		template <typename t_CDeleter>
		void f_DeleteAllDeleter()
		{
			while (t_CData *pCurrent = f_GetLast())
			{
				t_CDeleter::fs_Delete(pCurrent);
			}
		}

		void f_DeleteAllDeleter()
		{
			f_DeleteAllDeleter<t_CData>();
		}


		inline_small bool f_IsEmpty() const
		{
			auto pLink = &m_Link;
			return pLink->fp_GetNextList() == pLink;
		}

		mint f_GetLen() const
		{
			CIteratorConst Iter(*this);
			mint Num = 0;
			while (Iter)
			{
				++Num;
				++Iter;
			}
			return Num;
		}

		template <typename t_CData2>
		bool f_Contains(const t_CData2 &_CheckFor) const
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

		template <typename t_CData2>
		const t_CData *f_Find(const t_CData2 &_CheckFor) const
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

		template <typename t_CData2>
		t_CData *f_Find(const t_CData2 &_CheckFor)
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

		template <typename t_CCompare, typename t_CData2>
		const t_CData *f_Find(t_CCompare &&_Sorter, const t_CData2 &_CheckFor) const
		{
			CIteratorConst Iter(*this);
			while (Iter)
			{
				if (!_Sorter((*Iter), _CheckFor) && !_Sorter(_CheckFor, (*Iter)))
					return Iter;
				++Iter;
			}
			return nullptr;
		}

		template <typename t_CCompare, typename t_CData2>
		t_CData *f_Find(t_CCompare &&_Sorter, const t_CData2 &_CheckFor)
		{
			CIterator Iter(*this);
			while (Iter)
			{
				if (!_Sorter((*Iter), _CheckFor) && !_Sorter(_CheckFor, (*Iter)))
					return Iter;
				++Iter;
			}
			return nullptr;
		}

		bool f_Contains(const t_CData *_pCheckFor) const
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

		bool f_Contains(t_CData *_pCheckFor) const
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

		/***************************************************************************************************\
		|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
		| Inserts																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		void f_SplitInto(t_CData *_pFirst, t_CData *_pLast)
		{
			DMibSafeCheck(f_IsEmpty(), "List must be empty here");
			DMibSafeCheck(_pFirst, "First must be specified");
			DMibSafeCheck(_pLast, "Last must be specified");
			t_CLink *pToLinkFirst = fp_LinkFromMember(_pFirst);
			t_CLink *pToLinkLast = fp_LinkFromMember(_pLast);
			DMibSafeCheck(pToLinkFirst->f_IsInList(), "First must be in list");
			DMibSafeCheck(pToLinkLast->f_IsInList(), "Last must be in list");
			DMibSafeCheck(!pToLinkFirst->fp_IsListLink(), "Internal error");
			DMibSafeCheck(!pToLinkLast->fp_IsListLink(), "Internal error");

			t_CLink::fs_UnlinkRange(pToLinkFirst, pToLinkLast);

			m_Link.fp_TransferList(pToLinkFirst, pToLinkLast);
		}

		// Insert
		void f_Insert(TCDLinkListAggregate &_OtherList)
		{
			if (_OtherList.f_IsEmpty())
				return;

			auto pLink = &m_Link;
			auto pOtherLink = &_OtherList.m_Link;
			if (f_IsEmpty())
			{
				pLink->fp_TransferList(pOtherLink->fp_GetNextList(), pOtherLink->fp_GetPrevList());
				_OtherList.f_Construct();
				return;
			}
			auto *pToLinkFirst = pOtherLink->fp_GetNextList();
			auto *pToLinkLast = pOtherLink->f_Upcast()->fp_GetPrevList();
			auto *pToInsertAfter = pLink->f_Upcast()->fp_GetPrevList();

			t_CLink::fs_LinkRange(pToLinkFirst->f_Upcast(), pToLinkLast->f_Upcast(), pToInsertAfter);

			// Just reset other list
			_OtherList.f_Construct();
		}

		inline_small void f_Insert(t_CData *_pToInsert)
		{
			t_CLink *pToLink = fp_LinkFromMember(_pToInsert);
			pToLink->f_Unlink();
			auto *pLast = m_Link.fp_GetPrevList();
			pToLink->fp_LinkLast(pLast);
		}
		inline_small void f_Insert(t_CData &_ToInsert)
		{
			f_Insert(&_ToInsert);
		}

		void f_InsertSorted(TCDLinkListAggregate &_OtherList)
		{
			while (auto *pData = _OtherList.f_Pop())
				f_InsertSorted(pData);
		}

		template <typename t_CSortClass>
		void f_InsertSortedLast(t_CData &_ToInsert, void *_pContext = nullptr)
		{
			f_InsertSortedLast<t_CSortClass>(&_ToInsert, _pContext);
		}

		template <typename t_CSortClass>
		void f_InsertSortedLast(t_CData *_pToInsert, void *_pContext = nullptr)
		{
			t_CLink *pToLink = fp_LinkFromMember(_pToInsert);
			pToLink->f_Unlink();

			auto pLink = &m_Link;
			auto *pCurrent = pLink->fp_GetNextList();
			auto *pLast = pLink;

			while (pCurrent != pLink)
			{
				if (t_CSortClass::fs_Compare(_pContext, fp_MemberFromLink(pCurrent->f_Upcast()), _pToInsert) > 0)
					break;
				pLast = pCurrent;
				pCurrent = pCurrent->fp_GetNextNotList();
			}

			pToLink->fp_Link(pLast);
		}

		template <typename t_CSortClass>
		void f_InsertSorted(t_CData &_ToInsert, void *_pContext = nullptr)
		{
			f_InsertSortedLast<t_CSortClass>(_ToInsert, _pContext);
		}

		template <typename t_CSortClass>
		void f_InsertSorted(t_CData *_pToInsert, void *_pContext = nullptr)
		{
			f_InsertSortedLast<t_CSortClass>(_pToInsert, _pContext);
		}

		template <typename t_CSortClass>
		void f_InsertSortedFirst(t_CData &_ToInsert, void *_pContext = nullptr)
		{
			f_InsertSortedFirst<t_CSortClass>(&_ToInsert, _pContext);
		}

		template <typename t_CSortClass>
		void f_InsertSortedFirst(t_CData *_pToInsert, void *_pContext = nullptr)
		{
			t_CLink *pToLink = fp_LinkFromMember(_pToInsert);
			pToLink->f_Unlink();

			auto *pLink = &m_Link;
			auto *pCurrent = pLink->fp_GetNextList();
			auto *pLast = pLink;

			while (pCurrent != pLink)
			{
				if (t_CSortClass::fs_Compare(_pContext, fp_MemberFromLink(pCurrent->f_Upcast()), _pToInsert) >= 0)
					break;
				pLast = pCurrent;
				pCurrent = pCurrent->fp_GetNextNotList();
			}

			pToLink->fp_Link(pLast);
		}


		// f_InsertLast

		void f_InsertLast(TCDLinkListAggregate &_OtherList)
		{
			f_Insert(_OtherList);
		}


		inline_small void f_InsertLast(t_CData *_pToInsert)
		{
			f_Insert(_pToInsert);
		}
		inline_small void f_InsertLast(t_CData &_ToInsert)
		{
			f_InsertLast(&_ToInsert);
		}

		// f_InsertFirst
		inline_small void f_InsertFirst(t_CData *_pToInsert)
		{
			t_CLink *pToLink = fp_LinkFromMember(_pToInsert);
			pToLink->f_Unlink();
			pToLink->fp_LinkFirst(&m_Link);
		}
		inline_small void f_InsertFirst(t_CData &_ToInsert)
		{
			f_InsertFirst(&_ToInsert);
		}

		void f_InsertFirst(TCDLinkListAggregate &_OtherList)
		{
			if (_OtherList.f_IsEmpty())
				return;
			auto pLink = &m_Link;
			auto pOtherLink = &_OtherList.m_Link;
			if (f_IsEmpty())
			{
				pLink->fp_TransferList(pOtherLink->fp_GetNextList(), pOtherLink->fp_GetPrevList());
				_OtherList.f_Construct();
				return;
			}
			auto *pToLinkFirst = pOtherLink->fp_GetNextList();
			auto *pToLinkLast = pOtherLink->fp_GetPrevList();
			auto *pToInsertAfter = pLink;

			t_CLink::fs_LinkRange(pToLinkFirst->f_Upcast(), pToLinkLast->f_Upcast(), pToInsertAfter);

			// Just reset other list
			_OtherList.f_Construct();
		}

		// InsertHead

		void f_InsertAfter(TCDLinkListAggregate &_OtherList, t_CData *_pToInsertAfter)
		{
			if (_OtherList.f_IsEmpty())
				return;
			DMibSafeCheck(!f_IsEmpty(), "Cannot be empty");
			DMibSafeCheck(fp_LinkFromMember(_pToInsertAfter)->f_IsInList(), "The object has to be in a list to be able to be inserted");

			auto pOtherLink = &_OtherList.m_Link;

			t_CLink *pToLinkFirst = pOtherLink->fp_GetNextList()->f_Upcast();
			t_CLink *pToLinkLast = pOtherLink->fp_GetPrevList()->f_Upcast();

			t_CLink *pToInsertAfter = fp_LinkFromMember(_pToInsertAfter);

			t_CLink::fs_LinkRange(pToLinkFirst, pToLinkLast, pToInsertAfter);

			// Just reset other list
			_OtherList.f_Construct();
		}

		static inline_small void f_InsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
		{
			DMibSafeCheck(fp_LinkFromMember(_pToInsertAfter)->f_IsInList(), "The object has to be in a list to be able to be inserted");
			DMibSafeCheck(_pToInsert != _pToInsertAfter, "You cannot insert an object after itself");
			t_CLink *pToLink = fp_LinkFromMember(_pToInsert);
			pToLink->f_Unlink();
			pToLink->fp_Link(fp_LinkFromMember(_pToInsertAfter));
		}
		static inline_small void f_InsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter)
		{
			f_InsertAfter(&_ToInsert, _pToInsertAfter);
		}
		static inline_small void f_InsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter)
		{
			f_InsertAfter(_pToInsert, &_ToInsertAfter);
		}
		static inline_small void f_InsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter)
		{
			f_InsertAfter(&_ToInsert, &_ToInsertAfter);
		}


		void f_InsertBefore(TCDLinkListAggregate &_OtherList, t_CData *_pLinkBeforeThis)
		{
			if (_OtherList.f_IsEmpty())
				return;
			DMibSafeCheck(!f_IsEmpty(), "Cannot be empty");
			DMibSafeCheck(fp_LinkFromMember(_pLinkBeforeThis)->f_IsInList(), "The object has to be in a list to be able to be inserted");

			auto pOtherLink = &_OtherList.m_Link;
			t_CLink *pToLinkFirst = pOtherLink->fp_GetNextList()->f_Upcast();
			t_CLink *pToLinkLast = pOtherLink->fp_GetPrevList()->f_Upcast();

			t_CLink *pToInsertAfter = fp_LinkFromMember(_pLinkBeforeThis)->fp_GetPrevNotList();

			t_CLink::fs_LinkRange(pToLinkFirst, pToLinkLast, pToInsertAfter);

			// Just reset other list
			_OtherList.f_Construct();
		}

		static inline_small void f_InsertBefore(t_CData *_pToInsert, t_CData *_pLinkBeforeThis)
		{
			DMibSafeCheck(fp_LinkFromMember(_pLinkBeforeThis)->f_IsInList(), "The object has to be in a list to be able to be inserted");
			DMibSafeCheck(_pToInsert != _pLinkBeforeThis, "You cannot insert an object before itself");
			t_CLink *pToLink = fp_LinkFromMember(_pToInsert);
			t_CLink *pLinkBeforeThis = fp_LinkFromMember(_pLinkBeforeThis);

			pToLink->f_Unlink();
			pToLink->fp_Link(pLinkBeforeThis->fp_GetPrevNotList());
		}
		static inline_small void f_InsertBefore(t_CData &_ToInsert, t_CData *_pToInsertBefore)
		{
			f_InsertBefore(&_ToInsert, _pToInsertBefore);
		}
		static inline_small void f_InsertBefore(t_CData *_pToInsert, t_CData &_ToInsertBefore)
		{
			f_InsertBefore(_pToInsert, &_ToInsertBefore);
		}
		static inline_small void f_InsertBefore(t_CData &_ToInsert, t_CData &_ToInsertBefore)
		{
			f_InsertBefore(&_ToInsert, &_ToInsertBefore);
		}


		inline_small void f_Push(t_CData *_pData)
		{
			f_InsertFirst(_pData);
		}
		inline_small void f_Push(t_CData &_Data)
		{
			f_InsertFirst(&_Data);
		}

		/***************************************************************************************************\
		|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
		| Unsafe Inserts																					|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		// Insert
		inline_small void f_UnsafeInsert(t_CData *_pToInsert)
		{
			fp_LinkFromMember(_pToInsert)->fp_LinkNoUnlinkLast(m_Link.fp_GetPrevList());
		}
		inline_small void f_UnsafeInsert(t_CData &_ToInsert)
		{
			f_UnsafeInsert(&_ToInsert);
		}

		template <typename t_CSortClass>
		void f_UnsafeInsertSorted(t_CData &_ToInsert, void *_pContext = nullptr)
		{
			f_UnsafeInsertSorted<t_CSortClass>(&_ToInsert, _pContext);
		}

		template <typename t_CSortClass>
		void f_UnsafeInsertSorted(t_CData *_pToInsert, void *_pContext = nullptr)
		{
			auto pLink = &m_Link;
			auto *pCurrent = pLink->fp_GetNextList();

			while (pCurrent != pLink)
			{
				if (t_CSortClass::fs_Compare(_pContext, this->fp_MemberFromLink(pCurrent->f_Upcast()), _pToInsert) > 0)
					break;
				pCurrent = pCurrent->fp_GetNextNotList();
			}

			if (pCurrent != pLink)
			{
				t_CLink *pToLink = fp_LinkFromMember(_pToInsert);
				pToLink->fp_LinkNoUnlink(pCurrent);
			}
			else
			{
				t_CLink *pToLink = fp_LinkFromMember(_pToInsert);
				pToLink->fp_LinkNoUnlinkFirst(pLink);
			}
		}


		// InsertTail
		inline_small void f_UnsafeInsertLast(t_CData *_pToInsert)
		{
			f_UnsafeInsert(_pToInsert);
		}
		inline_small void f_UnsafeInsertLast(t_CData &_ToInsert)
		{
			f_UnsafeInsertLast(&_ToInsert);
		}

		// InsertHead
		inline_small void f_UnsafeInsertFirst(t_CData *_pToInsert)
		{
			fp_LinkFromMember(_pToInsert)->fp_LinkNoUnlinkFirst(&m_Link);
		}
		inline_small void f_UnsafeInsertFirst(t_CData &_ToInsert)
		{
			f_UnsafeInsertFirst(&_ToInsert);
		}

		// InsertHead
		static inline_small void f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter)
		{
			DMibSafeCheck(fp_LinkFromMember(_pToInsertAfter)->f_IsInList(), "The object has to be in a list to be able to be inserted");
			fp_LinkFromMember(_pToInsert)->fp_LinkNoUnlink(fp_LinkFromMember(_pToInsertAfter));
		}
		static inline_small void f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter)
		{
			f_UnsafeInsertAfter(&_ToInsert, _pToInsertAfter);
		}
		static inline_small void f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter)
		{
			f_UnsafeInsertAfter(_pToInsert, &_ToInsertAfter);
		}
		static inline_small void f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter)
		{
			f_UnsafeInsertAfter(&_ToInsert, &_ToInsertAfter);
		}

		inline_small void f_UnsafePush(t_CData *_pData)
		{
			f_UnsafeInsertFirst(_pData);
		}

		/***************************************************************************************************\
		|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
		| Removes																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		static inline_small void f_Remove(t_CData *_pToRemove)
		{
			fp_LinkFromMember(_pToRemove)->f_Unlink();
		}

		static inline_small void f_Remove(t_CData &_pToRemove)
		{
			f_Remove(&_pToRemove);
		}

		inline_medium t_CData *f_Pop()
		{
			auto *pLink = &m_Link;
			auto *pNext = pLink->fp_GetNextList();
			if (pNext != pLink)
			{
				pNext->f_Upcast()->f_UnlinkFirstLinked();
				return fp_MemberFromLink(pNext->f_Upcast());
			}
			else
				return nullptr;
		}

		inline_medium t_CData *f_UnsafePop()
		{
			auto *pLink = &m_Link;
			auto *pNext = pLink->fp_GetNextList();
			if (pNext != pLink)
			{
				pNext->f_Upcast()->f_UnsafeUnlinkFirst();
				return fp_MemberFromLink(pNext->f_Upcast());
			}
			else
				return nullptr;
		}

		void f_Reverse()
		{
			auto *pLink = &m_Link;
			auto *pCurrent = pLink->fp_GetNextList();
			while (pCurrent != pLink)
			{
				t_CLink *pTemp = pCurrent->f_Upcast();
				pCurrent = pCurrent->fp_GetNextNotList();
				pTemp->f_UnlinkLinked();
				pTemp->fp_LinkFirst(pLink);
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

		/*ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ*\
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
		template <typename t_CSortClass, typename tf_CContext>
		void f_MergeSort(tf_CContext &&_Context, aint _InsertionBits = 3)
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
				auto pNext = pLink->fp_GetNextList();
				auto pPrev = pLink->fp_GetPrevList();
				List.fp_SetNextInit(pNext);
				List.fp_SetPrevInit(pPrev);
				pNext->f_Upcast()->fp_SetPrevInit(&List);
				pPrev->f_Upcast()->fp_SetNextInit(&List);

				pLink->fp_SetNextInitListLink(pLink);
				pLink->fp_SetPrevInitListLink(pLink);
			}


			aint MergeSize = aint(1) << (_InsertionBits);

			if (_InsertionBits > 0)
			{
				const aint SortSize = aint(1) << _InsertionBits;

				auto *pCurrent = List.fp_GetNextNotList();
				t_CLink LastLink;
				t_CLink *pLastLink = &LastLink;
				pLastLink->fp_SetNextInit(pLastLink);
				pLastLink->fp_SetPrevInit(pLastLink);
				aint NumSorted = 0;

				while (pCurrent != &List)
				{
					// Add first member

					++NumSorted;
					t_CLink *pTemp = pCurrent->fp_GetNextNotList()->f_Upcast();
					pCurrent->f_Upcast()->fp_LinkNoUnlinkNotList(pLastLink->fp_GetPrevNoList());
					pCurrent = pTemp;

					if (pCurrent == &List)
						break;

					for (aint i = 1; i < SortSize; ++i)
					{
						aint NumChecked = i;
						++NumSorted;

						t_CLink *pTemp = pLastLink->fp_GetPrevNoList();

						while (NumChecked)
						{
							if (t_CSortClass::fs_Compare(fg_Forward<tf_CContext>(_Context),fp_MemberFromLink(pCurrent->f_Upcast()),fp_MemberFromLink(pTemp)) >= 0)
								break;

							--NumChecked;
							pTemp = pTemp->fp_GetPrevNoList();
						}

						t_CLink *pTempNext = pCurrent->fp_GetNextNotList()->f_Upcast();
						pCurrent->f_Upcast()->fp_LinkNoUnlinkNotList(pTemp);
						pCurrent = pTempNext;
						if (pCurrent == &List)
							break;
					}
				}

				pLink->fp_TransferList(pLastLink->fp_GetNextNotList(), pLastLink->fp_GetPrevNoList());
				if (NumSorted <= MergeSize)
				{
					// Add last link
					return;
				}

				// Remove last link
//					LastLink.fp_GetPrev()->fp_SetNext(nullptr);
			}

			DMibSafeCheck(_InsertionBits >= 0, "Has to be at least 1");

			{
				auto pNext = pLink->fp_GetNextList();
				auto pPrev = pLink->fp_GetPrevList();
				List.fp_SetNextInit(pNext);
				List.fp_SetPrevInit(pPrev);
				pNext->f_Upcast()->fp_SetPrevInit(&List);
				pPrev->f_Upcast()->fp_SetNextInit(&List);

				pLink->fp_SetNextInitListLink(pLink);
				pLink->fp_SetPrevInitListLink(pLink);
			}


			while (1)
			{
				auto *pFirst = List.fp_GetNextNotList()->f_Upcast();
				auto *pLast = &List;

				aint MergesDone = 0;  // count number of merges we do in this pass

				while (pFirst != &List)
				{
					MergesDone++;  // there exists a merge to be done
					// step 'MergeSize' places along from pFirst
					t_CLink *pSecond = pFirst->f_Upcast();
					aint MergeSizeFirst = 0;
					aint MergeSizeSecond = MergeSize;
					while (MergeSizeFirst < MergeSize)
					{
						MergeSizeFirst++;
						pSecond = pSecond->fp_GetNextNotList()->f_Upcast();
						if (pSecond == &List)
							break;
					}
					// if pSecond hasn't fallen off end, we have two lists to merge

					if (pSecond == &List)
					{
						// End of list
						pLast->fp_SetNextNotList(pFirst);
						pFirst->fp_SetPrevNotList(pLast);
						pLast = &List;
						pFirst = pSecond;
						continue;
					}

					if (t_CSortClass::fs_Compare(fg_Forward<tf_CContext>(_Context),fp_MemberFromLink(pSecond->fp_GetPrevNoList()),fp_MemberFromLink(pSecond)) <= 0)
					{
						// Lists already sorted
						pLast->fp_SetNextNotList(pFirst);
						pFirst->fp_SetPrevNotList(pLast);

						while (MergeSizeSecond && pSecond != &List)
						{
							--MergeSizeSecond;
							pLast = pSecond;
							pSecond = pSecond->fp_GetNextNotList()->f_Upcast();
						}
					}
					else
					{

						DMibSafeCheck(MergeSizeSecond && MergeSizeFirst, "Hula");
						// now we have two lists; merge them
						while (1)
						{
							// decide whether m_pNext element of merge comes from pFirst or pSecond
							if (t_CSortClass::fs_Compare(fg_Forward<tf_CContext>(_Context),fp_MemberFromLink(pFirst),fp_MemberFromLink(pSecond)) <= 0)
							{
								// First element of pFirst is lower (or same); pTemp must come from pFirst.
								pLast->fp_SetNextNotList(pFirst);
								pFirst->fp_SetPrevNotList(pLast);
								pLast = pFirst;
								pFirst = pFirst->fp_GetNextNotList()->f_Upcast();
								MergeSizeFirst--;
								if (!MergeSizeFirst)
								{
									pLast->fp_SetNextNotList(pSecond);
									pSecond->fp_SetPrevNotList(pLast);

									while (MergeSizeSecond && pSecond != &List)
									{
										--MergeSizeSecond;
										pLast = pSecond;
										pSecond = pSecond->fp_GetNextNotList()->f_Upcast();
									}
									break;
								}
							}
							else
							{
								// First element of pSecond is lower; pTemp must come from pSecond.
								pLast->fp_SetNextNotList(pSecond);
								pSecond->fp_SetPrevNotList(pLast);
								pLast = pSecond;
								pSecond = pSecond->fp_GetNextNotList()->f_Upcast();
								MergeSizeSecond--;
								if (pSecond == &List || !MergeSizeSecond)
								{
									pLast->fp_SetNextNotList(pFirst);
									pFirst->fp_SetPrevNotList(pLast);

									while (MergeSizeFirst)
									{
										MergeSizeFirst--;
										pLast = pFirst;
										pFirst = pFirst->fp_GetNextNotList()->f_Upcast();
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
					pLast->fp_SetNextNotList(&List);
					List.fp_SetPrevNotList(pLast);
				}

				// If we have done only one merge, we're finished.
				if (MergesDone <= 1)   // allow for MergesDone==0, the empty list case
				{
					pLink->fp_TransferList(List.fp_GetNextNotList(), List.fp_GetPrevNotList());
					return;
				}

				// Otherwise repeat, merging lists twice the size
				MergeSize <<= 1;
			}
		}

		template <typename t_CSortClass>
		void f_MergeSort()
		{
			f_MergeSort<t_CSortClass>((void *)nullptr);
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
			static inline_small CRet fs_Compare(TCComparerer const &_Context, t_CLeft &&_Left, t_CRight &&_Right)
			{
				if (_Context.m_Functor(_Left, _Right))
					return -1;
				else if (_Context.m_Functor(_Right, _Left))
					return 1;
				return 0;
			}
		};

		template <typename t_CFunctor>
		void f_Sort(t_CFunctor &&_Functor)
		{
			TCComparerer<t_CFunctor> Comparerer(_Functor);
			f_MergeSort<TCComparerer<t_CFunctor>>(Comparerer);
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


		/*ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ*\
		|	Function:			Sorts the linked list								|
		|																			|
		|	Template params:														|
		|		t_CSortClass:	The class that implements the index funtion.		|
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

		template <typename t_CSortClass, aint t_BucketSize>
		void f_BucketSort(aint _NumPlaces, void *_pContext = nullptr)
		{
			if (f_IsEmpty())
				return;

			t_CLink *Bucket[t_BucketSize];

			// Reset buckets
			for (aint i = 0; i < t_BucketSize; ++i)
			{
				Bucket[i] = nullptr;
			}

			t_CLink List;
			{
				auto pLink = &m_Link;
				auto pNext = pLink->fp_GetNextList();
				auto pPrev = pLink->fp_GetPrevList();
				List.fp_SetNextInit(pNext);
				List.fp_SetPrevInit(pPrev);
				pNext->fp_SetPrevInit(&List);
				pPrev->fp_SetNextInit(&List);

				pLink->fp_SetNextInitListLink(pLink);
				pLink->fp_SetPrevInitListLink(pLink);
			}

			auto pLink = &List;

			// Put list in first bucket

			for(aint Place = 0; Place < _NumPlaces; ++Place)
			{
				// Remove last link
				pLink->fp_GetPrevNoList()->fp_SetNextNotList(nullptr);

				auto *pCurrent = pLink->fp_GetNextNotList();

				while (pCurrent)
				{
					aint Index = t_CSortClass::fs_GetIndex(_pContext, Place, fp_MemberFromLink(pCurrent->f_Upcast()));
					t_CLink **pBucket = &Bucket[Index];

					t_CLink *pNext = pCurrent->fp_GetNextNotList();

					if (*pBucket)
						(*pBucket)->fp_SetPrevNotList(pCurrent);
					pCurrent->f_Upcast()->fp_SetNextNotList(*pBucket);
					(*pBucket) = pCurrent;

					pCurrent = pNext;
				}


				// Rebuild list

				pLink->fp_SetNextNotList(pLink);
				pLink->fp_SetPrevNotList(pLink);

				for (aint i = 0; i < t_BucketSize; ++i)
				{
					if (Bucket[i])
					{
						t_CLink *pTemp = Bucket[i];
						Bucket[i] = nullptr;
						auto *pLinkAfter = pLink->fp_GetPrevNoList();

						while (pTemp)
						{
							auto *pTempNext = pTemp->fp_GetNextNotList();
							pTemp->fp_LinkNoUnlinkNotList(pLinkAfter);
							pTemp = pTempNext;
						}
					}
				}
			}

			m_Link.fp_TransferList(List.fp_GetNextNotList(), List.fp_GetPrevNoList());

		}

		bool f_CheckList(bool _bBreak) const
		{
			auto *pLink = &m_Link;
			auto *pCurrent = pLink;
			if (!pCurrent->fp_IsListLink())
			{
				if (_bBreak)
					DMibPDebugBreak;
				return false;
			}
			if (!pCurrent->fp_IsValid())
			{
				if (_bBreak)
					DMibPDebugBreak;
				return false;
			}
			auto *pPrev = pCurrent;
			pCurrent = pCurrent->fp_GetNextList();
			while (pCurrent != pLink)
			{
				if (pCurrent->f_Upcast()->fp_GetPrevNotList() != pPrev || !pCurrent->fp_IsValid())
				{
					if (_bBreak)
						DMibPDebugBreak;
					return false;
				}
				pPrev = pCurrent;
				pCurrent = pCurrent->fp_GetNextNotList();
			}
			pCurrent = pLink;
			pPrev = pCurrent;
			pCurrent = pCurrent->fp_GetPrevList();
			while (pCurrent != pLink)
			{
				if (pCurrent->f_Upcast()->fp_GetNextNotList() != pPrev || !pCurrent->fp_IsValid())
				{
					if (_bBreak)
						DMibPDebugBreak;
					return false;
				}
				pPrev = pCurrent;
				pCurrent = pCurrent->f_Upcast()->fp_GetPrevNotList();
			}
			return true;
		}

		class CIterator
		{
		public:
			typedef t_CData CData;
			t_CLink *m_pCurrent;
#ifdef DMibDebuggerHelpers
			static TCDLinkListAggregate *fs_Debug_List();
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

			CIterator(TCDLinkListAggregate &_List)
			{
				auto *pLink = &_List.m_Link;
				auto pCurrent = pLink->fp_GetNextList();
				if (pCurrent == pLink)
					m_pCurrent = nullptr;
				else
					m_pCurrent = pCurrent->f_Upcast();

#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIterator(const CIterator& _Copy)
			{
				m_pCurrent = _Copy.m_pCurrent;
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIterator(t_CData *_pStart)
			{
				m_pCurrent = TCDLinkListAggregate::fp_LinkFromMember(_pStart);
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIterator(t_CData &_Start)
			{
				m_pCurrent = TCDLinkListAggregate::fp_LinkFromMember(&_Start);
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIterator& operator = (TCDLinkListAggregate &_List)
			{
				auto *pLink = &_List.m_Link;
				auto *pNext = pLink->fp_GetNextList();
				if (pNext == pLink)
					m_pCurrent = nullptr;
				else
					m_pCurrent = pNext->f_Upcast();

				return *this;
			}

			CIterator& operator = (const CIterator& _Copy)
			{
				m_pCurrent = _Copy.m_pCurrent;
				return *this;
			}

			CIterator& operator = (t_CData &_Start)
			{
				m_pCurrent = TCDLinkListAggregate::fp_LinkFromMember(&_Start);

				return *this;
			}

			CIterator& operator = (t_CData *_pStart)
			{
				m_pCurrent = TCDLinkListAggregate::fp_LinkFromMember(_pStart);

				return *this;
			}

			static inline_small t_CData *fs_GetNext(t_CData *_pCurrent)
			{
				auto *pNext = TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->fp_GetNextNotList();
				if (pNext && !pNext->fp_IsListLink())
					return TCDLinkListAggregate::fp_MemberFromLink(pNext->f_Upcast());
				else
					return nullptr;
			}

			static inline_small t_CData *fs_GetNextUnsafe(t_CData *_pCurrent)
			{
				auto *pNext = TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->fp_GetNextNotList();
				if (!pNext->fp_IsListLink())
					return TCDLinkListAggregate::fp_MemberFromLink(pNext->f_Upcast());
				else
					return nullptr;
			}

			static inline_small t_CData *fs_GetPrevUnsafe(t_CData *_pCurrent)
			{
				auto *pPrev = TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->fp_GetPrevNotList();
				if (!pPrev->fp_IsListLink())
					return TCDLinkListAggregate::fp_MemberFromLink(pPrev->f_Upcast());
				else
					return nullptr;
			}

			static inline_small t_CData *fs_GetPrev(t_CData *_pCurrent)
			{
				if (!TCDLinkListAggregate::fp_LinkFromMember(_pCurrent)->f_IsInList())
					return nullptr;
				return fs_GetPrevUnsafe(_pCurrent);
			}

			void f_Reverse(TCDLinkListAggregate &_List)
			{
				auto *pLink = &_List.m_Link;
				auto *pPrev = _List.m_Link.fp_GetPrevList();

				if (pPrev == pLink)
					m_pCurrent = nullptr;
				else
					m_pCurrent = pPrev->f_Upcast();
			}

			inline_small t_CData *f_GetCurrent() const
			{
				if (m_pCurrent)
					return TCDLinkListAggregate::fp_MemberFromLink(m_pCurrent);
				else
					return nullptr;
			}

			inline_small operator t_CData *() const
			{
				return f_GetCurrent();
			}

			inline_small t_CData * operator ->() const
			{
				return f_GetCurrent();
			}

			inline_small t_CData * operator [](aint _nPlaces) const
			{
				if (m_pCurrent)
				{
					auto *pCurrent = m_pCurrent;
					if (_nPlaces > 0)
					{
						while (_nPlaces)
						{
							auto *pNext = pCurrent->fp_GetNextNotList();
							if (pNext->fp_IsListLink())
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
							auto *pPrev = pCurrent->fp_GetPrevNotList();
							if (pPrev->fp_IsListLink())
							{
								DMibSafeCheck(0, "You tried to access an element outside the list");
								return nullptr;
							}
							pCurrent = pPrev->f_Upcast();
							++_nPlaces;
						}
					}
					return TCDLinkListAggregate::fp_MemberFromLink(pCurrent);
				}
				DMibSafeCheck(0, "You tried to access an element outside the list");
				return nullptr;
			}

			mint f_GetLen() const
			{
				if (m_pCurrent)
				{
					t_CLinkInList *pCurrent = m_pCurrent;
					mint Len = 0;
					while (!pCurrent->fp_IsListLink())
					{
						++Len;
						pCurrent = pCurrent->fp_GetNextNotList();
					}
					return Len;
				}
				return 0;
			}

#ifdef DMibPUniqueType_int
			inline_small t_CData * operator [](int _nPlaces) const
			{
				return operator [](aint(_nPlaces));
			}
#endif

			inline_small void operator +=(aint _nPlaces)
			{
				if (m_pCurrent)
				{
					while (_nPlaces)
					{
						auto *pNext = m_pCurrent->fp_GetNextNotList();
						if (pNext->fp_IsListLink())
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
			inline_small void operator ++()
			{
				if (m_pCurrent)
				{
					auto pNext = m_pCurrent->fp_GetNextNotList();
					if (pNext->fp_IsListLink())
						m_pCurrent = nullptr;
					else
						m_pCurrent = pNext->f_Upcast();
				}
			}

			inline_small void operator --()
			{
				if (m_pCurrent)
				{
					auto *pPrev = m_pCurrent->fp_GetPrevNotList();
					if (pPrev->fp_IsListLink())
						m_pCurrent = nullptr;
					else
						m_pCurrent = pPrev->f_Upcast();
				}

			}

			inline_small void operator -=(aint _nPlaces)
			{
				if (m_pCurrent)
				{
					while (_nPlaces)
					{
						auto *pPrev = m_pCurrent->fp_GetPrevNotList();
						if (pPrev->fp_IsListLink())
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

			void f_Delete()
			{
				t_CData *pCurrent = f_GetCurrent();
				++(*this);
				fg_DeleteObject(t_CAllocator(), pCurrent);
			}

			template <typename t_CDeleter>
			void f_DeleteDeleter()
			{
				t_CData *pCurrent = f_GetCurrent();
				++(*this);
				t_CDeleter::fs_Delete(pCurrent);
			}

			void f_DeleteDeleter()
			{
				f_DeleteDeleter<t_CData>();
			}

			void f_Remove()
			{
				t_CLink *pLink = m_pCurrent;
				++(*this);
				pLink->f_UnlinkLinked();
			}
		};

		CIterator f_GetIter()
		{
			return CIterator(*this);
		}

		CIterator f_GetIterator()
		{
			return CIterator(*this);
		}

		class CIteratorConst
		{
		public:
			const t_CLink *m_pCurrent;

#ifdef DMibDebuggerHelpers
			static TCDLinkListAggregate *fs_Debug_List();
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

			CIteratorConst(const CIteratorConst& _Copy)
			{
				m_pCurrent = _Copy.m_pCurrent;
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIteratorConst(const TCDLinkListAggregate &_List)
			{
				auto *pLink = &_List.m_Link;
				auto *pNext = pLink->fp_GetNextList();
				if (pNext == pLink)
					m_pCurrent = nullptr;
				else
					m_pCurrent = pNext->f_Upcast();
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIteratorConst(const t_CData *_pStart)
			{
				m_pCurrent = TCDLinkListAggregate::fp_LinkFromMember(_pStart);
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIteratorConst(const t_CData &_Start)
			{
				m_pCurrent = TCDLinkListAggregate::fp_LinkFromMember(&_Start);
#ifdef DMibDebuggerHelpers
				static_assert(TCInstantiateValue<&fs_Debug_List>::mc_Value);
				static_assert(TCInstantiateValue<&fs_Debug_GetOffset>::mc_Value);
#endif
			}

			CIteratorConst& operator = (const TCDLinkListAggregate &_List)
			{
				auto *pLink = &_List.m_Link;
				auto *pNext = pLink->fp_GetNextList();
				if (pNext == pLink)
					m_pCurrent = nullptr;
				else
					m_pCurrent = pNext->f_Upcast();
				return *this;
			}

			CIteratorConst& operator = (const CIteratorConst& _Copy)
			{
				m_pCurrent = _Copy.m_pCurrent;
				return *this;
			}

			CIteratorConst& operator = (const t_CData &_Start)
			{
				m_pCurrent = TCDLinkListAggregate::fp_LinkFromMember(&_Start);

				return *this;
			}

			CIteratorConst& operator = (const t_CData *_pStart)
			{
				m_pCurrent = TCDLinkListAggregate::fp_LinkFromMember(_pStart);

				return *this;
			}

			void f_Reverse(const TCDLinkListAggregate &_List)
			{
				auto *pLink = &_List.m_Link;
				auto *pPrev = pLink->fp_GetPrev();
				if (pPrev == pLink)
					m_pCurrent = nullptr;
				else
					m_pCurrent = pPrev->f_Upcast();
			}

			inline_small const t_CData *f_GetCurrent() const
			{
				if (m_pCurrent)
					return TCDLinkListAggregate::fp_MemberFromLink(m_pCurrent);
				else
					return nullptr;
			}

			inline_small operator const t_CData *() const
			{
				return f_GetCurrent();
			}

			inline_small const t_CData * operator ->() const
			{
				return f_GetCurrent();
			}

			mint f_GetLen() const
			{
				if (m_pCurrent)
				{
					t_CLinkInList *pCurrent = m_pCurrent;
					mint Len = 0;
					while (!pCurrent->fp_IsListLink())
					{
						++Len;
						pCurrent = pCurrent->fp_GetNextNotList();
					}
					return Len;
				}
				return 0;
			}

			inline_small const t_CData * operator [](aint _nPlaces) const
			{
				if (m_pCurrent)
				{
					t_CLinkInList const *pCurrent = m_pCurrent;
					if (_nPlaces > 0)
					{
						while (_nPlaces)
						{
							pCurrent = pCurrent->fp_GetNextNotList();
							if (pCurrent->fp_IsListLink())
							{
								return nullptr;
							}
							--_nPlaces;
						}
					}
					else
					{
						while (_nPlaces)
						{
							pCurrent = pCurrent->fp_GetPrevNotList();
							if (pCurrent->fp_IsListLink())
							{
								return nullptr;
							}
							++_nPlaces;
						}
					}
					return TCDLinkListAggregate::fp_MemberFromLink(pCurrent->f_Upcast());
				}
				return nullptr;
			}

#ifdef DMibPUniqueType_int
			inline_small const t_CData * operator [](int _nPlaces) const
			{
				return operator [](aint(_nPlaces));
			}
#endif
			inline_small void operator +=(aint _nPlaces)
			{
				if (m_pCurrent)
				{
					while (_nPlaces)
					{
						auto *pNext = m_pCurrent->fp_GetNextNotList();
						if (pNext->fp_IsListLink())
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
			inline_small void operator ++()
			{
				if (m_pCurrent)
				{
					auto *pNext = m_pCurrent->fp_GetNextNotList();
					if (pNext->fp_IsListLink())
						m_pCurrent = nullptr;
					else
						m_pCurrent = pNext->f_Upcast();
				}
			}

			inline_small void operator --()
			{
				if (m_pCurrent)
				{
					auto *pPrev = m_pCurrent->fp_GetPrevNotList();
					if (pPrev->fp_IsListLink())
						m_pCurrent = nullptr;
					else
						m_pCurrent = pPrev->f_Upcast();
				}
			}

			inline_small void operator -=(aint _nPlaces)
			{
				if (m_pCurrent)
				{
					while (_nPlaces)
					{
						auto *pPrev = m_pCurrent->fp_GetPrevNotList();
						if (pPrev->fp_IsListLink())
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
		};

		CIteratorConst f_GetIter() const
		{
			return CIteratorConst(*this);
		}

		CIteratorConst f_GetIterator() const
		{
			return CIteratorConst(*this);
		}

		bool operator == (const TCDLinkListAggregate &_Other) const
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
	};
#ifdef DMibDebuggerHelpers
	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::fs_Debug_GetOffset() -> COffset
	{
		return {};
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>*
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::fs_Debug_List()
	{
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIterator::fs_Debug_GetOffset() -> COffset
	{
		return {};
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>*
	TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::fs_Debug_List()
	{
		return nullptr;
	}

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	auto TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>::CIteratorConst::fs_Debug_GetOffset() -> COffset
	{
		return {};
	}
#endif

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	class TCDLinkList final : public TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>
	{
	private:
		TCDLinkList(TCDLinkList const &) = delete;
		TCDLinkList &operator = (TCDLinkList const &) = delete;

	public:

		typedef TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator> CSuper;
		bool operator == (const TCDLinkList &_Other) const
		{
			return (const CSuper &)*this == (const CSuper &)_Other;
		}

		TCDLinkList(TCDLinkList &&_Other)
		{
			CSuper::f_Construct();
			this->f_Transfer(_Other);
		}

		TCDLinkList &operator =(TCDLinkList &&_Other)
		{
			this->f_Clear();
			this->f_Transfer(_Other);
			return *this;
		}

		inline_small TCDLinkList()
		{
			CSuper::f_Construct();
		}

		inline_small ~TCDLinkList()
		{
			CSuper::f_Destruct();
		}
	};

#	define DMibListLinkD_Trans(_Class, _Member) \
		class CDLinkTranslator##_Member \
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
		};

#	define DMibListLinkD_TransType(_Class, _Member) _Class::CDLinkTranslator##_Member
//						(((uint8*)(&(pClass->_Member)) - (uint8*)(pClass)));
//					      (((uint8*)(&((_Class *)0x0)->_Member) - (uint8*)((_Class *)0x0)));

	/***************************************************************************************************\
	|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
	| Fast																								|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#	define DMibListLinkAllocatorD_LinkType(_Allocator) NMib::NIntrusive::TCDLink<NMib::NIntrusive::CDLinkAggregate>
#	define DMibListLinkAllocatorD_Member(_Member, _Allocator) DMibListLinkAllocatorD_LinkType(_Allocator) _Member;

#	define DMibListLinkAllocatorDA_LinkType(_Allocator) NMib::NIntrusive::CDLinkAggregate
#	define DMibListLinkAllocatorDA_Member(_Member, _Allocator) DMibListLinkAllocatorDA_LinkType(_Allocator) _Member;

#	define DMibListLinkAllocatorD_Link(_Class, _Member, _Allocator) \
			DMibListLinkAllocatorD_Member(_Member, _Allocator) \
			DMibListLinkD_Trans(_Class, _Member)

#	define DMibListLinkAllocatorDA_Link(_Class, _Member, _Allocator) \
			DMibListLinkAllocatorDA_Member(_Member, _Allocator) \
			DMibListLinkD_Trans(_Class, _Member)

#	define DMibListLinkAllocatorD_List(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkList<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, _Allocator>
#	define DMibListLinkAllocatorD_ListAutoDelete(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkList<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, _Allocator>
#	define DMibListLinkAllocatorD_List_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkList<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, _Allocator>
#	define DMibListLinkAllocatorD_ListAutoDelete_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkList<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, _Allocator>

#	define DMibListLinkAllocatorDA_List(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkListAggregate<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, _Allocator>
#	define DMibListLinkAllocatorDA_ListAutoDelete(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkListAggregate<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, _Allocator>
#	define DMibListLinkAllocatorDA_List_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkListAggregate<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, _Allocator>
#	define DMibListLinkAllocatorDA_ListAutoDelete_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkListAggregate<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, _Allocator>

#	define DMibListLinkAllocatorD_Iter(_Class, _Member, _Allocator) DMibListLinkAllocatorDA_List(_Class, _Member, _Allocator)::CIterator
#	define DMibListLinkAllocatorD_IterConst(_Class, _Member, _Allocator) DMibListLinkAllocatorDA_List(_Class, _Member, _Allocator)::CIteratorConst
#	define DMibListLinkAllocatorD_Iter_FromTemplate(_Class, _Member, _Allocator) typename DMibListLinkAllocatorDA_List_FromTemplate(_Class, _Member, _Allocator)::CIterator
#	define DMibListLinkAllocatorD_IterConst_FromTemplate(_Class, _Member, _Allocator) typename DMibListLinkAllocatorDA_List_FromTemplate(_Class, _Member, _Allocator)::CIteratorConst

#	ifndef DMibPNoShortCuts
#		define DLinkAllocatorD_Link(_Class, _Member, _Allocator) DMibListLinkAllocatorD_Link(_Class, _Member, _Allocator)
#		define DLinkAllocatorD_Iter(_Class, _Member, _Allocator) DMibListLinkAllocatorD_Iter(_Class, _Member, _Allocator)
#		define DLinkAllocatorD_IterConst(_Class, _Member, _Allocator) DMibListLinkAllocatorD_IterConst(_Class, _Member, _Allocator)
#		define DLinkAllocatorD_List(_Class, _Member, _Allocator) DMibListLinkAllocatorD_List(_Class, _Member, _Allocator)
#		define DLinkAllocatorD_ListAutoDelete(_Class, _Member, _Allocator) DMibListLinkAllocatorD_ListAutoDelete(_Class, _Member, _Allocator)
#		define DLinkAllocatorDA_Link(_Class, _Member, _Allocator) DMibListLinkAllocatorDA_Link(_Class, _Member, _Allocator)
#		define DLinkAllocatorDA_List(_Class, _Member, _Allocator) DMibListLinkAllocatorDA_List(_Class, _Member, _Allocator)
#		define DLinkAllocatorDA_ListAutoDelete(_Class, _Member, _Allocator) DMibListLinkAllocatorDA_ListAutoDelete(_Class, _Member, _Allocator)
#	endif

	/***************************************************************************************************\
	|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
	| List Takes Less Space, but slower																	|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#	define DMibListLinkAllocatorDS_LinkType(_Allocator) NMib::NIntrusive::TCDLink< NMib::NIntrusive::CDLinkAggregateListNoPrevPtr >
#	define DMibListLinkAllocatorDS_Member(_Member, _Allocator) DMibListLinkAllocatorDS_LinkType(_Allocator) _Member;

#	define DMibListLinkAllocatorDSA_LinkType(_Allocator) NMib::NIntrusive::CDLinkAggregateListNoPrevPtr
#	define DMibListLinkAllocatorDSA_Member(_Member, _Allocator) DMibListLinkAllocatorDSA_LinkType(_Allocator) _Member;

#	define DMibListLinkAllocatorDS_Link(_Class, _Member, _Allocator) \
			DMibListLinkAllocatorDS_Member(_Member, _Allocator) \
			DMibListLinkD_Trans(_Class, _Member)

#	define DMibListLinkAllocatorDSA_Link(_Class, _Member, _Allocator) \
			DMibListLinkAllocatorDSA_Member(_Member, _Allocator) \
			DMibListLinkD_Trans(_Class, _Member)

#	define DMibListLinkAllocatorDS_List(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkList<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, false, _Allocator>
#	define DMibListLinkAllocatorDS_ListAutoDelete(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkList<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, true, _Allocator>
#	define DMibListLinkAllocatorDS_List_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkList<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, false, _Allocator>
#	define DMibListLinkAllocatorDS_ListAutoDelete_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkList<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, true, _Allocator>

#	define DMibListLinkAllocatorDSA_List(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkListAggregate<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, false, _Allocator>
#	define DMibListLinkAllocatorDSA_ListAutoDelete(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkListAggregate<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, true, _Allocator>
#	define DMibListLinkAllocatorDSA_List_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkListAggregate<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, false, _Allocator>
#	define DMibListLinkAllocatorDSA_ListAutoDelete_FromTemplate(_Class, _Member, _Allocator) NMib::NIntrusive::TCDLinkListAggregate<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, true, _Allocator>

#	define DMibListLinkAllocatorDS_Iter(_Class, _Member, _Allocator) DMibListLinkAllocatorDSA_List(_Class, _Member, _Allocator)::CIterator
#	define DMibListLinkAllocatorDS_IterConst(_Class, _Member, _Allocator) DMibListLinkAllocatorDSA_List(_Class, _Member, _Allocator)::CIteratorConst
#	define DMibListLinkAllocatorDS_Iter_FromTemplate(_Class, _Member, _Allocator) DMibListLinkAllocatorDSA_List_FromTemplate(_Class, _Member, _Allocator)::CIterator
#	define DMibListLinkAllocatorDS_IterConst_FromTemplate(_Class, _Member, _Allocator) DMibListLinkAllocatorDSA_List_FromTemplate(_Class, _Member, _Allocator)::CIteratorConst

#	ifndef DMibPNoShortCuts
#		define DLinkAllocatorDS_Link(_Class, _Member, _Allocator) DMibListLinkAllocatorDS_Link(_Class, _Member, _Allocator)
#		define DLinkAllocatorDS_Iter(_Class, _Member, _Allocator) DMibListLinkAllocatorDS_Iter(_Class, _Member, _Allocator)
#		define DLinkAllocatorDS_IterConst(_Class, _Member, _Allocator) DMibListLinkAllocatorDS_IterConst(_Class, _Member, _Allocator)
#		define DLinkAllocatorDS_List(_Class, _Member, _Allocator) DMibListLinkAllocatorDS_List(_Class, _Member, _Allocator)
#		define DLinkAllocatorDS_ListAutoDelete(_Class, _Member, _Allocator) DMibListLinkAllocatorDS_ListAutoDelete(_Class, _Member, _Allocator)
#		define DLinkAllocatorDSA_Link(_Class, _Member, _Allocator) DMibListLinkAllocatorDSA_Link(_Class, _Member, _Allocator)
#		define DLinkAllocatorDSA_List(_Class, _Member, _Allocator) DMibListLinkAllocatorDSA_List(_Class, _Member, _Allocator)
#		define DLinkAllocatorDSA_ListAutoDelete(_Class, _Member, _Allocator) DMibListLinkAllocatorDSA_ListAutoDelete(_Class, _Member, _Allocator)
#	endif


	/***************************************************************************************************\
	|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
	| Fast																								|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#	define DMibListLinkD_LinkType NMib::NIntrusive::TCDLink<NMib::NIntrusive::CDLinkAggregate>
#	define DMibListLinkD_Member(_Member) DMibListLinkD_LinkType _Member;

#	define DMibListLinkDA_LinkType NMib::NIntrusive::CDLinkAggregate
#	define DMibListLinkDA_Member(_Member) DMibListLinkDA_LinkType _Member;

#	define DMibListLinkD_Link(_Class, _Member) \
			DMibListLinkD_Member(_Member) \
			DMibListLinkD_Trans(_Class, _Member)

#	define DMibListLinkDA_Link(_Class, _Member) \
			DMibListLinkDA_Member(_Member) \
			DMibListLinkD_Trans(_Class, _Member)

#	define DMibListLinkD_List(_Class, _Member) NMib::NIntrusive::TCDLinkList<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkD_ListAutoDelete(_Class, _Member) NMib::NIntrusive::TCDLinkList<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkD_List_FromTemplate(_Class, _Member) NMib::NIntrusive::TCDLinkList<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkD_ListAutoDelete_FromTemplate(_Class, _Member) NMib::NIntrusive::TCDLinkList<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkDA_List(_Class, _Member) NMib::NIntrusive::TCDLinkListAggregate<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDA_ListAutoDelete(_Class, _Member) NMib::NIntrusive::TCDLinkListAggregate<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDA_List_FromTemplate(_Class, _Member) NMib::NIntrusive::TCDLinkListAggregate<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDA_ListAutoDelete_FromTemplate(_Class, _Member) NMib::NIntrusive::TCDLinkListAggregate<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkD_Iter(_Class, _Member) DMibListLinkDA_List(_Class, _Member)::CIterator
#	define DMibListLinkD_IterConst(_Class, _Member) DMibListLinkDA_List(_Class, _Member)::CIteratorConst
#	define DMibListLinkD_Iter_FromTemplate(_Class, _Member) typename DMibListLinkDA_List_FromTemplate(_Class, _Member)::CIterator
#	define DMibListLinkD_IterConst_FromTemplate(_Class, _Member) typename DMibListLinkDA_List_FromTemplate(_Class, _Member)::CIteratorConst


#	ifndef DMibPNoShortCuts
#		define DLinkD_Trans(_Class, _Member) DMibListLinkD_Trans(_Class, _Member)
#		define DLinkD_Link(_Class, _Member) DMibListLinkD_Link(_Class, _Member)
#		define DLinkD_Iter(_Class, _Member) DMibListLinkD_Iter(_Class, _Member)
#		define DLinkD_IterConst(_Class, _Member) DMibListLinkD_IterConst(_Class, _Member)
#		define DLinkD_List(_Class, _Member) DMibListLinkD_List(_Class, _Member)
#		define DLinkD_ListAutoDelete(_Class, _Member) DMibListLinkD_ListAutoDelete(_Class, _Member)
#		define DLinkDA_Link(_Class, _Member) DMibListLinkDA_Link(_Class, _Member)
#		define DLinkDA_List(_Class, _Member) DMibListLinkDA_List(_Class, _Member)
#		define DLinkDA_ListAutoDelete(_Class, _Member) DMibListLinkDA_ListAutoDelete(_Class, _Member)
#	endif

	/***************************************************************************************************\
	|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
	| List Takes Less Space, but slower																	|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#	define DMibListLinkDS_LinkType NMib::NIntrusive::TCDLink< NMib::NIntrusive::CDLinkAggregateListNoPrevPtr >
#	define DMibListLinkDS_Member(_Member) DMibListLinkDS_LinkType _Member;

#	define DMibListLinkDSA_LinkType NMib::NIntrusive::CDLinkAggregateListNoPrevPtr
#	define DMibListLinkDSA_Member(_Member) DMibListLinkDSA_LinkType _Member;

#	define DMibListLinkDS_Link(_Class, _Member) \
			DMibListLinkDS_Member(_Member) \
			DMibListLinkD_Trans(_Class, _Member)

#	define DMibListLinkDSA_Link(_Class, _Member) \
			DMibListLinkDSA_Member(_Member) \
			DMibListLinkD_Trans(_Class, _Member)

#	define DMibListLinkDS_List(_Class, _Member) NMib::NIntrusive::TCDLinkList<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDS_ListMT(_Class, _MemberType) NMib::NIntrusive::TCDLinkList<_Class, _MemberType, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDS_ListAutoDelete(_Class, _Member) NMib::NIntrusive::TCDLinkList<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, true, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDS_List_FromTemplate(_Class, _Member) NMib::NIntrusive::TCDLinkList<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDS_ListAutoDelete_FromTemplate(_Class, _Member) NMib::NIntrusive::TCDLinkList<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkDSA_List(_Class, _Member) NMib::NIntrusive::TCDLinkListAggregate<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDSA_ListAutoDelete(_Class, _Member) NMib::NIntrusive::TCDLinkListAggregate<_Class, _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, true, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDSA_List_FromTemplate(_Class, _Member) NMib::NIntrusive::TCDLinkListAggregate<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, false, NMib::NMemory::CDefaultAllocator>
#	define DMibListLinkDSA_ListAutoDelete_FromTemplate(_Class, _Member) NMib::NIntrusive::TCDLinkListAggregate<_Class, typename _Class::CDLinkTranslator##_Member, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList, true, NMib::NMemory::CDefaultAllocator>

#	define DMibListLinkDS_Iter(_Class, _Member) DMibListLinkDSA_List(_Class, _Member)::CIterator
#	define DMibListLinkDS_IterConst(_Class, _Member) DMibListLinkDSA_List(_Class, _Member)::CIteratorConst
#	define DMibListLinkDS_Iter_FromTemplate(_Class, _Member) DMibListLinkDSA_List_FromTemplate(_Class, _Member)::CIterator
#	define DMibListLinkDS_IterConst_FromTemplate(_Class, _Member) DMibListLinkDSA_List_FromTemplate(_Class, _Member)::CIteratorConst

#	ifndef DMibPNoShortCuts
#		define DLinkDS_Link(_Class, _Member) DMibListLinkDS_Link(_Class, _Member)
#		define DLinkDS_Iter(_Class, _Member) DMibListLinkDS_Iter(_Class, _Member)
#		define DLinkDS_IterConst(_Class, _Member) DMibListLinkDS_IterConst(_Class, _Member)
#		define DLinkDS_List(_Class, _Member) DMibListLinkDS_List(_Class, _Member)
#		define DLinkDS_ListAutoDelete(_Class, _Member) DMibListLinkDS_ListAutoDelete(_Class, _Member)
#		define DLinkDSA_Link(_Class, _Member) DMibListLinkDSA_Link(_Class, _Member)
#		define DLinkDSA_List(_Class, _Member) DMibListLinkDSA_List(_Class, _Member)
#		define DLinkDSA_ListAutoDelete(_Class, _Member) DMibListLinkDSA_ListAutoDelete(_Class, _Member)
#	endif

}
