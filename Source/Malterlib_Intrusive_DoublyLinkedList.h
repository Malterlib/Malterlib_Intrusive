// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

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
		umint m_NextPtr;

		constexpr CDLinkAggregateListNoPrevPtrList(EAggregateInitialization _Init);
		CDLinkAggregateListNoPrevPtrList();
		inline_small void f_SetNextList(CDLinkAggregateListNoPrevPtrList *_pNext);
		inline_small void f_SetNextInitListLink(CDLinkAggregateListNoPrevPtrList *_pNext);
		inline_small void f_ConstructList();
		inline_small bool f_IsListLink() const;
		inline_small CDLinkAggregateListNoPrevPtrList *f_GetNextList();
		inline_small void f_SetPrevInitListLink(CDLinkAggregateListNoPrevPtrList *_pPrev);
		inline_small CDLinkAggregateListNoPrevPtrList const *f_GetNextList() const;
		inline_small CDLinkAggregateListNoPrevPtrList *f_GetNextNotList();
		inline_small CDLinkAggregateListNoPrevPtrList const *f_GetNextNotList() const;

		bool f_IsValid() const;

		inline_small CDLinkAggregateListNoPrevPtr *f_Upcast();
		inline_small CDLinkAggregateListNoPrevPtr const *f_Upcast() const;

		inline_small CDLinkAggregateListNoPrevPtrList *f_GetPrevList();
		inline_small CDLinkAggregateListNoPrevPtrList const *f_GetPrevList() const;
		inline_small void f_TransferList(CDLinkAggregateListNoPrevPtrList *_pFirst, CDLinkAggregateListNoPrevPtrList *_pLast);

	protected:
		inline_small void fp_Internal_SetNextList(void *_pNext);
		inline_small void fp_Internal_SetNextInitListLink(void *_pNext);
		inline_small bool fp_Internal_IsListLink() const;
		inline_small void *fp_Internal_GetNextList() const;
		inline_small void *fp_Internal_GetNextNotList() const;
	};

	struct CDLinkAggregateListNoPrevPtr final : public CDLinkAggregateListNoPrevPtrList
	{
		using CThis = CDLinkAggregateListNoPrevPtr;
		// Special link to not need a double link in the list header
		// List header has only next pointer pointing to first item (or itself)
		// The first item has a prev pointer that points to the last item in the list
		// The List header has a bit of the next pointer set to 1 to specify that it's the header

		umint m_PrevPtr;

		constexpr CDLinkAggregateListNoPrevPtr(EAggregateInitialization _Init);
		CDLinkAggregateListNoPrevPtr();
		inline_small void f_Construct();
		inline_small void f_SetNextNotList(CDLinkAggregateListNoPrevPtrList *_pNext);
		inline_small void f_SetNextInit(CDLinkAggregateListNoPrevPtrList *_pNext);
		inline_small void f_SetPrevNotList(CDLinkAggregateListNoPrevPtrList *_pPrev);
		inline_small void f_SetPrevInit(CDLinkAggregateListNoPrevPtrList *_pPrev);
		inline_small CDLinkAggregateListNoPrevPtr *f_GetPrevPtr();
		inline_small CDLinkAggregateListNoPrevPtr const *f_GetPrevPtr() const;
		inline_small CDLinkAggregateListNoPrevPtrList *f_GetPrevNotList();
		inline_small CDLinkAggregateListNoPrevPtr *f_GetPrevNoList();
		inline_small CDLinkAggregateListNoPrevPtrList const *f_GetPrevNotList() const;
		inline_small CDLinkAggregateListNoPrevPtr const *f_GetPrevNoList() const;
		inline_small void f_Link(CDLinkAggregateListNoPrevPtrList *_pLinkAfter);
		inline_small void f_LinkFirst(CDLinkAggregateListNoPrevPtrList *_pLinkAfter);
		inline_small void f_LinkLast(CDLinkAggregateListNoPrevPtrList *_pLinkAfter);
		static inline_small void fs_UnlinkRange(CDLinkAggregateListNoPrevPtr *_pFirst, CDLinkAggregateListNoPrevPtr *_pLast);
		static inline_small void fs_LinkRange(CDLinkAggregateListNoPrevPtr *_pFirst, CDLinkAggregateListNoPrevPtr *_pLast, CDLinkAggregateListNoPrevPtrList *_pInsertAfter);
		inline_extralarge void f_LinkNoUnlink(CDLinkAggregateListNoPrevPtrList *_pLinkAfter);
		inline_extralarge void f_LinkNoUnlinkNotList(CDLinkAggregateListNoPrevPtr *_pLinkAfter);
		inline_extralarge void f_LinkNoUnlinkFirst(CDLinkAggregateListNoPrevPtrList *_pLinkAfter);
		inline_extralarge void f_LinkNoUnlinkLast(CDLinkAggregateListNoPrevPtrList *_pLinkAfter);

		// Public functions
		inline_extralarge void f_Unlink();
		inline_small void f_UnlinkFirst();
		inline_small void f_UnsafeUnlink();
		inline_small void f_UnsafeUnlinkFirst();
		inline_small void f_UnlinkLinked();
		inline_small void f_UnlinkFirstLinked();
		inline_small void f_Destruct();
		inline_small bool f_IsAloneInList() const;
		inline_small bool f_IsInList() const;

	private:
		inline_small void fp_Internal_SetPrev(void *_pPrev);
		inline_small void fp_Internal_SetPrevInit(void *_pPrev);
		inline_small void *fp_Internal_GetPrev() const;
		inline_small void fp_Internal_SetNextNotList(void *_pNext);
		inline_small void fp_Internal_SetNextInit(void *_pNext);
		inline_small void fp_UnlinkFirstInternal();
		inline_small void fp_UnlinkInternal();
	};

	struct CDLinkAggregate final
	{
		CDLinkAggregate *m_pNextPtr;
		CDLinkAggregate *m_pPrevPtr;

		constexpr CDLinkAggregate(EAggregateInitialization _Init);
		CDLinkAggregate();
		bool f_IsValid() const;
		inline_extralarge void f_Link(CDLinkAggregate *_pLinkAfter);
		inline_small void f_LinkFirst(CDLinkAggregate *_pLinkAfter);
		inline_small void f_LinkLast(CDLinkAggregate *_pLinkAfter);
		static inline_small void fs_UnlinkRange(CDLinkAggregate *_pFirst, CDLinkAggregate *_pLast);
		static inline_small void fs_LinkRange(CDLinkAggregate *_pFirst, CDLinkAggregate *_pLast, CDLinkAggregate *_pInsertAfter);
		inline_small void f_LinkNoUnlink(CDLinkAggregate *_pLinkAfter);
		inline_small void f_LinkNoUnlinkNotList(CDLinkAggregate *_pLinkAfter);
		inline_small void f_LinkNoUnlinkFirst(CDLinkAggregate *_pLinkAfter);
		inline_small void f_LinkNoUnlinkLast(CDLinkAggregate *_pLinkAfter);
		inline_small void f_SetNextNotList(CDLinkAggregate *_pNext);
		inline_small void f_SetNextInit(CDLinkAggregate *_pNext);
		inline_small void f_SetNextInitListLink(CDLinkAggregate *_pNext);
		inline_small void f_SetPrevNotList(CDLinkAggregate *_pPrev);
		inline_small void f_SetPrevInit(CDLinkAggregate *_pPrev);
		inline_small void f_SetPrevInitListLink(CDLinkAggregate *_pPrev);
		inline_small bool f_IsListLink() const;
		inline_small CDLinkAggregate const *f_GetNextList() const;
		inline_small CDLinkAggregate const *f_GetNextNotList() const;
		inline_small CDLinkAggregate const *f_GetPrev() const;
		inline_small CDLinkAggregate const *f_GetPrevList() const;
		inline_small CDLinkAggregate const *f_GetPrevNotList() const;
		inline_small CDLinkAggregate const *f_GetPrevNoList() const;
		inline_small CDLinkAggregate *f_GetNextList();
		inline_small CDLinkAggregate *f_GetNextNotList();
		inline_small CDLinkAggregate *f_GetPrev();
		inline_small CDLinkAggregate *f_GetPrevNotList();
		inline_small CDLinkAggregate *f_GetPrevNoList();
		inline_small CDLinkAggregate *f_GetPrevList();
		inline_small void f_TransferList(CDLinkAggregate *_pFirst, CDLinkAggregate *_pLast);

		// Public functions

		inline_small CDLinkAggregate *f_Upcast();
		inline_small CDLinkAggregate const *f_Upcast() const;
		inline_small bool f_IsAloneInList() const;
		inline_extralarge void f_Unlink();
		inline_small void f_UnlinkFirst();
		inline_small void f_UnsafeUnlink();
		inline_small void f_UnsafeUnlinkFirst();
		inline_small void f_UnlinkLinked();
		inline_small void f_UnlinkFirstLinked();
		inline_small void f_ConstructList();
		inline_small void f_Construct();
		inline_small void f_Destruct();
		inline_small bool f_IsInList() const;

	private:
		using CThis = CDLinkAggregate;

		inline_small void fp_SetNext(CDLinkAggregate *_pNext);
		inline_small void fp_SetPrev(CDLinkAggregate *_pPrev);
		inline_small void fp_SetPrevList(CDLinkAggregate *_pPrev);
		inline_small CDLinkAggregate const *fp_GetNext() const;
		inline_small CDLinkAggregate *fp_GetNext();

		inline_small void fp_SetNextList(CDLinkAggregate *_pNext);
	};

	template <typename t_CLink>
	struct TCDLink final
	{
		inline_small TCDLink();
		TCDLink(TCDLink &&_Other);
		TCDLink &operator = (TCDLink &&_Other);
		inline_small ~TCDLink();
		inline_small t_CLink const *f_GetNext() const;
		inline_small t_CLink const *f_GetPrev() const;

		/*
		inline_small void fp_Link(t_CLink *_pLinkAfter)
		{
			mp_Link.fp_Link(_pLinkAfter);
		}

		inline_small void fp_LinkNoUnlink(t_CLink *_pLinkAfter)
		{
			mp_Link.fp_LinkNoUnlink(_pLinkAfter);
		}

		inline_small void fp_SetNext(t_CLink *_pNext)
		{
			mp_Link.fp_SetNextNotList(_pNext);
		}
		inline_small void fp_SetNextInit(t_CLink *_pNext)
		{
			mp_Link.fp_SetNextInit(_pNext);
		}
		inline_small void fp_SetNextInitListLink(t_CLink *_pNext)
		{
			mp_Link.fp_SetNextInitListLink(_pNext);
		}
		inline_small void fp_SetPrev(t_CLink *_pPrev)
		{
			mp_Link.fp_SetPrevNotList(_pPrev);
		}
		inline_small void fp_SetPrevInit(t_CLink *_pPrev)
		{
			mp_Link.fp_SetPrevInit(_pPrev);
		}
		inline_small void fp_SetPrevInitListLink(t_CLink *_pPrev)
		{
			mp_Link.fp_SetPrevInitListLink(_pPrev);
		}

		inline_small bool fp_IsListLink() const
		{
			return mp_Link.fp_IsListLink();
		}

		inline_small const t_CLink *fp_GetNext() const
		{
			return mp_Link.fp_GetNextNotList();
		}

		inline_small t_CLink *fp_GetNext()
		{
			return mp_Link.fp_GetNextNotList();
		}

		inline_small const t_CLink *fp_GetPrev() const
		{
			return mp_Link.fp_GetPrevNotList();
		}
		inline_small t_CLink *fp_GetPrev()
		{
			return mp_Link.fp_GetPrevNotList();
		}
		*/

		// Public functions
		inline_small void f_Unlink();
		inline_small void f_UnsafeUnlink();
		inline_small void f_UnlinkLinked();

		// First

		inline_small void f_UnlinkFirst();
		inline_small void f_UnsafeUnlinkFirst();
		inline_small void f_UnlinkFirstLinked();
		inline_small bool f_IsAloneInList() const;
		inline_small bool f_IsInList() const;

	private:
		TCDLink(TCDLink const &_Other) = delete;
		TCDLink &operator = (TCDLink const &_Other) = delete;

		t_CLink mp_Link;
	};

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	struct TCDLinkListAggregate
	{
		using CLink = t_CLink;
		using CTranslator = t_CTranslator;
		using COffset = typename CTranslator::template TCOffset<t_CData>;

		// Have first and last as dummylinks so they don't have destructors and constructors
		// 	t_CLink m_First;
		// 	t_CLink m_Last;
		t_CLinkInList m_Link;

		constexpr TCDLinkListAggregate(EAggregateInitialization _Init);
		TCDLinkListAggregate();
		inline_small void f_Construct();

		template <typename t_CDataInternal1, typename t_CAllocator1, bool t_bAutoDelete1>
		struct TCAutoDeleteHelper
		{
			static inline_small void fs_Delete(t_CData *_pDeleter);
		};

		template <typename t_CDataInternal1, typename t_CAllocator1>
		struct TCAutoDeleteHelper<t_CDataInternal1, t_CAllocator1, 0>
		{
			static inline_small void fs_Delete(t_CData *_pDeleter);
		};

		inline_small void f_Destruct();
		void f_Clear();
		inline_medium void f_Transfer(TCDLinkListAggregate *_pFrom);
		inline_medium void f_UnsafeTransfer(TCDLinkListAggregate *_pFrom);
		inline_small void f_Transfer(TCDLinkListAggregate &_From);
		inline_small void f_UnsafeTransfer(TCDLinkListAggregate &_From);
		inline_small t_CData *f_GetFirst();
		inline_small t_CData *f_GetLast();
		inline_small t_CData const *f_GetFirst() const;
		inline_small t_CData const *f_GetLast() const;
		static inline_small t_CData *fs_GetNextUnsafe(t_CData *_pCurrent);
		static inline_small t_CData *fs_GetNext(t_CData *_pCurrent);
		static inline_small t_CData *fs_GetPrevUnsafe(t_CData *_pCurrent);
		static inline_small t_CData *fs_GetPrev(t_CData *_pCurrent);
		static inline_small t_CData const *fs_GetNextUnsafe(t_CData const *_pCurrent);
		static inline_small t_CData const *fs_GetNext(t_CData const *_pCurrent);
		static inline_small t_CData const *fs_GetPrevUnsafe(t_CData const *_pCurrent);
		static inline_small t_CData const *fs_GetPrev(t_CData const *_pCurrent);
		void f_DeleteAll();
		template <typename tf_CAllocator2>
		void f_DeleteAllAllocator();
		template <typename tf_CAllocator2>
		void f_DeleteAllAllocator(tf_CAllocator2 &_Allocator);
		void f_DeleteAllDefiniteType();
		template <typename tf_CAllocator2>
		void f_DeleteAllAllocatorDefiniteType();
		template <typename tf_CAllocator2>
		void f_DeleteAllAllocatorDefiniteType(tf_CAllocator2 &_Allocator);
		template <typename tf_CDeleter>
		void f_DeleteAllDeleter();
		void f_DeleteAllDeleter();
		inline_small bool f_IsEmpty() const;
		umint f_GetLen() const;
		template <typename tf_CData2>
		bool f_Contains(tf_CData2 const &_CheckFor) const;
		template <typename tf_CData2>
		t_CData const *f_Find(tf_CData2 const &_CheckFor) const;
		template <typename tf_CData2>
		t_CData *f_Find(tf_CData2 const &_CheckFor);
		template <typename tf_FCompare, typename tf_CData2>
		t_CData const *f_Find(tf_FCompare &&_fCompare, tf_CData2 const &_CheckFor) const;
		template <typename tf_FCompare, typename tf_CData2>
		t_CData *f_Find(tf_FCompare &&_fCompare, tf_CData2 const &_CheckFor);
		bool f_Contains(t_CData const *_pCheckFor) const;
		bool f_Contains(t_CData *_pCheckFor) const;

		/***************************************************************************************************\
		|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
		| Inserts																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		void f_SplitInto(t_CData *_pFirst, t_CData *_pLast);

		// Insert
		void f_Insert(TCDLinkListAggregate &_OtherList);
		inline_small void f_Insert(t_CData *_pToInsert);
		inline_small void f_Insert(t_CData &_ToInsert);
		void f_InsertSorted(TCDLinkListAggregate &_OtherList);
		template <typename tf_FCompare>
		void f_InsertSortedLast(t_CData &_ToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		void f_InsertSortedLast(t_CData *_pToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		void f_InsertSorted(t_CData &_ToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		void f_InsertSorted(t_CData *_pToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		void f_InsertSortedFirst(t_CData &_ToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		void f_InsertSortedFirst(t_CData *_pToInsert, tf_FCompare &&_fCompare);

		// f_InsertLast

		void f_InsertLast(TCDLinkListAggregate &_OtherList);
		inline_small void f_InsertLast(t_CData *_pToInsert);
		inline_small void f_InsertLast(t_CData &_ToInsert);

		// f_InsertFirst
		inline_small void f_InsertFirst(t_CData *_pToInsert);
		inline_small void f_InsertFirst(t_CData &_ToInsert);
		void f_InsertFirst(TCDLinkListAggregate &_OtherList);

		// InsertHead

		void f_InsertAfter(TCDLinkListAggregate &_OtherList, t_CData *_pToInsertAfter);
		inline_small void f_InsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter);
		inline_small void f_InsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter);
		inline_small void f_InsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter);
		inline_small void f_InsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter);
		void f_InsertBefore(TCDLinkListAggregate &_OtherList, t_CData *_pLinkBeforeThis);
		inline_small void f_InsertBefore(t_CData *_pToInsert, t_CData *_pLinkBeforeThis);
		inline_small void f_InsertBefore(t_CData &_ToInsert, t_CData *_pToInsertBefore);
		inline_small void f_InsertBefore(t_CData *_pToInsert, t_CData &_ToInsertBefore);
		inline_small void f_InsertBefore(t_CData &_ToInsert, t_CData &_ToInsertBefore);
		inline_small void f_Push(t_CData *_pData);
		inline_small void f_Push(t_CData &_Data);

		/***************************************************************************************************\
		|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
		| Unsafe Inserts																					|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		// Insert
		inline_small void f_UnsafeInsert(t_CData *_pToInsert);
		inline_small void f_UnsafeInsert(t_CData &_ToInsert);
		template <typename tf_FCompare>
		void f_UnsafeInsertSorted(t_CData &_ToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		void f_UnsafeInsertSorted(t_CData *_pToInsert, tf_FCompare &&_fCompare);

		// InsertTail
		inline_small void f_UnsafeInsertLast(t_CData *_pToInsert);
		inline_small void f_UnsafeInsertLast(t_CData &_ToInsert);

		// InsertHead
		inline_small void f_UnsafeInsertFirst(t_CData *_pToInsert);
		inline_small void f_UnsafeInsertFirst(t_CData &_ToInsert);

		// InsertHead
		inline_small void f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter);
		inline_small void f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter);
		inline_small void f_UnsafeInsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter);
		inline_small void f_UnsafeInsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter);
		inline_small void f_UnsafePush(t_CData *_pData);

		/***************************************************************************************************\
		|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
		| Removes																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		inline_small void f_Remove(t_CData *_pToRemove);
		inline_small void f_Remove(t_CData &_ToRemove);
		inline_medium t_CData *f_Pop();
		inline_medium t_CData *f_UnsafePop();
		void f_Reverse();
		template <typename tf_FCompare>
		void f_MergeSort(tf_FCompare &&_fCompare, aint _InsertionBits = 3);
		template <typename tf_CSortClass>
		void f_MergeSort();
		template <typename tf_FCompare>
		void f_Sort(tf_FCompare &&_fCompare);

		using FBucketGetIndex = aint (void *_pContext, aint _Place, void *_pItem);
		struct CBucketCallbackSort
		{
			void *m_pContext;
			FBucketGetIndex *m_fSortFunction;
			static inline_small aint fs_GetIndex(void *_pContext, aint _Place, void *_pItem);
		};

		template <aint tf_BucketSize>
		void f_BucketSortCallback(FBucketGetIndex *_fSortFunction, void *_pContext = nullptr);
		template <typename tf_CSortClass, aint tf_BucketSize>
		void f_BucketSort(aint _NumPlaces, void *_pContext = nullptr);
		bool f_CheckList(bool _bBreak) const;

		struct CIterator
		{
			using CData = t_CData;
			t_CLink *m_pCurrent;

			CIterator();
			CIterator(TCDLinkListAggregate &_List);
			CIterator(CIterator const &_Copy);
			CIterator(t_CData *_pStart);
			CIterator(t_CData &_Start);
			CIterator f_GetIterator() const;
			CIterator &operator = (TCDLinkListAggregate &_List);
			CIterator &operator = (CIterator const &_Copy);
			CIterator &operator = (t_CData &_Start);
			CIterator &operator = (t_CData *_pStart);
			static inline_small t_CData *fs_GetNext(t_CData *_pCurrent);
			static inline_small t_CData *fs_GetNextUnsafe(t_CData *_pCurrent);
			static inline_small t_CData *fs_GetPrevUnsafe(t_CData *_pCurrent);
			static inline_small t_CData *fs_GetPrev(t_CData *_pCurrent);
			void f_Reverse(TCDLinkListAggregate &_List);
			inline_small t_CData *f_GetCurrent() const;
			inline_small operator t_CData * () const;
			inline_small t_CData *operator -> () const;
			inline_small t_CData *operator [] (aint _nPlaces) const;
			umint f_GetLen() const;

#ifdef DMibPUniqueType_int
			inline_small t_CData *operator [] (int _nPlaces) const;
#endif

			inline_small void operator += (aint _nPlaces);
			inline_small void operator ++ ();
			inline_small void operator -- ();
			inline_small void operator -= (aint _nPlaces);
			void f_Delete();
			template <typename tf_CDeleter>
			void f_DeleteDeleter();
			void f_DeleteDeleter();
			void f_Remove();
		};

		CIterator f_GetIterator();

		struct CIteratorConst
		{
			t_CLink const *m_pCurrent;

			CIteratorConst();
			CIteratorConst(CIteratorConst const &_Copy);
			CIteratorConst(TCDLinkListAggregate const &_List);
			CIteratorConst(t_CData const *_pStart);
			CIteratorConst(t_CData const &_Start);
			CIteratorConst f_GetIterator() const;
			CIteratorConst &operator = (TCDLinkListAggregate const &_List);
			CIteratorConst &operator = (CIteratorConst const &_Copy);
			CIteratorConst &operator = (t_CData const &_Start);
			CIteratorConst &operator = (t_CData const *_pStart);
			void f_Reverse(TCDLinkListAggregate const &_List);
			inline_small t_CData const *f_GetCurrent() const;
			inline_small operator t_CData const * () const;
			inline_small t_CData const *operator -> () const;
			umint f_GetLen() const;
			inline_small t_CData const *operator [] (aint _nPlaces) const;

#ifdef DMibPUniqueType_int
			inline_small t_CData const *operator [] (int _nPlaces) const;
#endif
			inline_small void operator += (aint _nPlaces);
			inline_small void operator ++ ();
			inline_small void operator -- ();
			inline_small void operator -= (aint _nPlaces);
		};

		CIteratorConst f_GetIterator() const;
		bool operator == (TCDLinkListAggregate const &_Other) const noexcept(noexcept(fg_GetType<t_CData const &>() == fg_GetType<t_CData const &>()));

	private:
		static inline_small t_CLink *fsp_LinkFromMember(t_CData *_pMember);
		static inline_small t_CData *fsp_MemberFromLink(t_CLink *_pLink);
		static inline_small t_CLink const *fsp_LinkFromMember(t_CData const *_pMember);
		static inline_small t_CData const *fsp_MemberFromLink(t_CLink const *_pLink);
	};

	template <typename t_CData, typename t_CTranslator, typename t_CLink, typename t_CLinkInList, bool t_bAutoDelete, typename t_CAllocator>
	struct TCDLinkList final : public TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>
	{
		using CSuper = TCDLinkListAggregate<t_CData, t_CTranslator, t_CLink, t_CLinkInList, t_bAutoDelete, t_CAllocator>;

		bool operator == (TCDLinkList const &_Other) const noexcept(noexcept((CSuper const &)*this == (CSuper const &)_Other));
		TCDLinkList(TCDLinkList &&_Other);
		TCDLinkList &operator = (TCDLinkList &&_Other);
		inline_small TCDLinkList();
		inline_small ~TCDLinkList();

	private:
		TCDLinkList(TCDLinkList const &_Other) = delete;
		TCDLinkList &operator = (TCDLinkList const &_Other) = delete;
	};

#if defined(DCompiler_clang)
#	define DMibListLinkD_Trans(d_Class, d_Member) \
		struct CDLinkTranslator##d_Member \
		{\
			template <typename t_CClass, auto t_pMember = &d_Class::d_Member> \
			struct TCOffset \
			{ \
				constexpr static umint mc_Offset = []\
					{\
						static constexpr t_CClass const *c_pNode = DMibRelaxConstexpr((t_CClass const *)(void *)(NMib::TCLimitsInt<smint>::mc_Max / 2 + 1));\
						return DMibRelaxConstexpr((uint8 const *)&(c_pNode->*t_pMember) - (uint8 const *)c_pNode);\
					}()\
				;\
			}; \
		};

#else

#	define DMibListLinkD_Trans(d_Class, d_Member) \
		struct CDLinkTranslator##d_Member \
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

#define DMibListLinkD_TransType(d_Class, d_Member) d_Class::CDLinkTranslator##d_Member
	// (((uint8*)(&(pClass->_Member)) - (uint8*)(pClass)));
	// (((uint8*)(&((_Class *)0x0)->_Member) - (uint8*)((_Class *)0x0)));

	/***************************************************************************************************\
	|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
	| Fast																								|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#define DMibListLinkAllocatorD_LinkType(d_Allocator) NMib::NIntrusive::TCDLink<NMib::NIntrusive::CDLinkAggregate>
#define DMibListLinkAllocatorD_Member(d_Member, d_Allocator) DMibListLinkAllocatorD_LinkType(d_Allocator) d_Member;

#define DMibListLinkAllocatorDA_LinkType(d_Allocator) NMib::NIntrusive::CDLinkAggregate
#define DMibListLinkAllocatorDA_Member(d_Member, d_Allocator) DMibListLinkAllocatorDA_LinkType(d_Allocator) d_Member;

#define DMibListLinkAllocatorD_Link(d_Class, d_Member, d_Allocator) \
	DMibListLinkAllocatorD_Member(d_Member, d_Allocator) \
	DMibListLinkD_Trans(d_Class, d_Member)

#define DMibListLinkAllocatorDA_Link(d_Class, d_Member, d_Allocator) \
	DMibListLinkAllocatorDA_Member(d_Member, d_Allocator) \
	DMibListLinkD_Trans(d_Class, d_Member)

#define DMibListLinkAllocatorD_List(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkList<d_Class, d_Class::CDLinkTranslator##d_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, d_Allocator>
#define DMibListLinkAllocatorD_ListAutoDelete(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkList<d_Class, d_Class::CDLinkTranslator##d_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, d_Allocator>
#define DMibListLinkAllocatorD_List_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkList<d_Class, typename d_Class::CDLinkTranslator##d_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, d_Allocator>
#define DMibListLinkAllocatorD_ListAutoDelete_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkList<d_Class, typename d_Class::CDLinkTranslator##d_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, d_Allocator>

#define DMibListLinkAllocatorDA_List(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkListAggregate<d_Class, d_Class::CDLinkTranslator##d_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, false, d_Allocator>
#define DMibListLinkAllocatorDA_ListAutoDelete(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkListAggregate<d_Class, d_Class::CDLinkTranslator##d_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, d_Allocator>
#define DMibListLinkAllocatorDA_List_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregate \
		, NMib::NIntrusive::CDLinkAggregate \
		, false \
		, d_Allocator \
	>
#define DMibListLinkAllocatorDA_ListAutoDelete_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkListAggregate<d_Class, typename d_Class::CDLinkTranslator##d_Member, NMib::NIntrusive::CDLinkAggregate, NMib::NIntrusive::CDLinkAggregate, true, d_Allocator>

#define DMibListLinkAllocatorD_Iter(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDA_List(d_Class, d_Member, d_Allocator)::CIterator
#define DMibListLinkAllocatorD_IterConst(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDA_List(d_Class, d_Member, d_Allocator)::CIteratorConst
#define DMibListLinkAllocatorD_Iter_FromTemplate(d_Class, d_Member, d_Allocator) typename DMibListLinkAllocatorDA_List_FromTemplate(d_Class, d_Member, d_Allocator)::CIterator
#define DMibListLinkAllocatorD_IterConst_FromTemplate(d_Class, d_Member, d_Allocator) typename DMibListLinkAllocatorDA_List_FromTemplate(d_Class, d_Member, d_Allocator)::CIteratorConst

#ifndef DMibPNoShortCuts
#	define DLinkAllocatorD_Link(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorD_Link(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorD_Iter(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorD_Iter(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorD_IterConst(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorD_IterConst(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorD_List(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorD_List(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorD_ListAutoDelete(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorD_ListAutoDelete(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDA_Link(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDA_Link(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDA_List(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDA_List(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDA_ListAutoDelete(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDA_ListAutoDelete(d_Class, d_Member, d_Allocator)
#endif

	/***************************************************************************************************\
	|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
	| List Takes Less Space, but slower																	|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#define DMibListLinkAllocatorDS_LinkType(d_Allocator) NMib::NIntrusive::TCDLink< NMib::NIntrusive::CDLinkAggregateListNoPrevPtr >
#define DMibListLinkAllocatorDS_Member(d_Member, d_Allocator) DMibListLinkAllocatorDS_LinkType(d_Allocator) d_Member;

#define DMibListLinkAllocatorDSA_LinkType(d_Allocator) NMib::NIntrusive::CDLinkAggregateListNoPrevPtr
#define DMibListLinkAllocatorDSA_Member(d_Member, d_Allocator) DMibListLinkAllocatorDSA_LinkType(d_Allocator) d_Member;

#define DMibListLinkAllocatorDS_Link(d_Class, d_Member, d_Allocator) \
	DMibListLinkAllocatorDS_Member(d_Member, d_Allocator) \
	DMibListLinkD_Trans(d_Class, d_Member)

#define DMibListLinkAllocatorDSA_Link(d_Class, d_Member, d_Allocator) \
	DMibListLinkAllocatorDSA_Member(d_Member, d_Allocator) \
	DMibListLinkD_Trans(d_Class, d_Member)

#define DMibListLinkAllocatorDS_List(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, false \
		, d_Allocator \
	>
#define DMibListLinkAllocatorDS_ListAutoDelete(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, true \
		, d_Allocator \
	>
#define DMibListLinkAllocatorDS_List_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, false \
		, d_Allocator \
	>
#define DMibListLinkAllocatorDS_ListAutoDelete_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, true \
		, d_Allocator \
	>

#define DMibListLinkAllocatorDSA_List(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, false \
		, d_Allocator \
	>
#define DMibListLinkAllocatorDSA_ListAutoDelete(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, true \
		, d_Allocator \
	>
#define DMibListLinkAllocatorDSA_List_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, false \
		, d_Allocator \
	>
#define DMibListLinkAllocatorDSA_ListAutoDelete_FromTemplate(d_Class, d_Member, d_Allocator) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, true \
		, d_Allocator \
	>

#define DMibListLinkAllocatorDS_Iter(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDSA_List(d_Class, d_Member, d_Allocator)::CIterator
#define DMibListLinkAllocatorDS_IterConst(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDSA_List(d_Class, d_Member, d_Allocator)::CIteratorConst
#define DMibListLinkAllocatorDS_Iter_FromTemplate(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDSA_List_FromTemplate(d_Class, d_Member, d_Allocator)::CIterator
#define DMibListLinkAllocatorDS_IterConst_FromTemplate(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDSA_List_FromTemplate(d_Class, d_Member, d_Allocator)::CIteratorConst

#ifndef DMibPNoShortCuts
#	define DLinkAllocatorDS_Link(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDS_Link(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDS_Iter(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDS_Iter(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDS_IterConst(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDS_IterConst(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDS_List(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDS_List(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDS_ListAutoDelete(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDS_ListAutoDelete(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDSA_Link(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDSA_Link(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDSA_List(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDSA_List(d_Class, d_Member, d_Allocator)
#	define DLinkAllocatorDSA_ListAutoDelete(d_Class, d_Member, d_Allocator) DMibListLinkAllocatorDSA_ListAutoDelete(d_Class, d_Member, d_Allocator)
#endif

	/***************************************************************************************************\
	|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
	| Fast																								|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#define DMibListLinkD_LinkType NMib::NIntrusive::TCDLink<NMib::NIntrusive::CDLinkAggregate>
#define DMibListLinkD_Member(d_Member) DMibListLinkD_LinkType d_Member;

#define DMibListLinkDA_LinkType NMib::NIntrusive::CDLinkAggregate
#define DMibListLinkDA_Member(d_Member) DMibListLinkDA_LinkType d_Member;

#define DMibListLinkD_Link(d_Class, d_Member) \
	DMibListLinkD_Member(d_Member) \
	DMibListLinkD_Trans(d_Class, d_Member)

#define DMibListLinkDA_Link(d_Class, d_Member) \
	DMibListLinkDA_Member(d_Member) \
	DMibListLinkD_Trans(d_Class, d_Member)

#define DMibListLinkD_List(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregate \
		, NMib::NIntrusive::CDLinkAggregate \
		, false \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkD_ListAutoDelete(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregate \
		, NMib::NIntrusive::CDLinkAggregate \
		, true \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkD_List_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregate \
		, NMib::NIntrusive::CDLinkAggregate \
		, false \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkD_ListAutoDelete_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregate \
		, NMib::NIntrusive::CDLinkAggregate \
		, true \
		, NMib::NMemory::CDefaultAllocator \
	>

#define DMibListLinkDA_List(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregate \
		, NMib::NIntrusive::CDLinkAggregate \
		, false \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDA_ListAutoDelete(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregate \
		, NMib::NIntrusive::CDLinkAggregate \
		, true \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDA_List_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregate \
		, NMib::NIntrusive::CDLinkAggregate \
		, false \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDA_ListAutoDelete_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregate \
		, NMib::NIntrusive::CDLinkAggregate \
		, true \
		, NMib::NMemory::CDefaultAllocator \
	>

#define DMibListLinkD_Iter(d_Class, d_Member) DMibListLinkDA_List(d_Class, d_Member)::CIterator
#define DMibListLinkD_IterConst(d_Class, d_Member) DMibListLinkDA_List(d_Class, d_Member)::CIteratorConst
#define DMibListLinkD_Iter_FromTemplate(d_Class, d_Member) typename DMibListLinkDA_List_FromTemplate(d_Class, d_Member)::CIterator
#define DMibListLinkD_IterConst_FromTemplate(d_Class, d_Member) typename DMibListLinkDA_List_FromTemplate(d_Class, d_Member)::CIteratorConst

#ifndef DMibPNoShortCuts
#	define DLinkD_Trans(d_Class, d_Member) DMibListLinkD_Trans(d_Class, d_Member)
#	define DLinkD_Link(d_Class, d_Member) DMibListLinkD_Link(d_Class, d_Member)
#	define DLinkD_Iter(d_Class, d_Member) DMibListLinkD_Iter(d_Class, d_Member)
#	define DLinkD_IterConst(d_Class, d_Member) DMibListLinkD_IterConst(d_Class, d_Member)
#	define DLinkD_List(d_Class, d_Member) DMibListLinkD_List(d_Class, d_Member)
#	define DLinkD_ListAutoDelete(d_Class, d_Member) DMibListLinkD_ListAutoDelete(d_Class, d_Member)
#	define DLinkDA_Link(d_Class, d_Member) DMibListLinkDA_Link(d_Class, d_Member)
#	define DLinkDA_List(d_Class, d_Member) DMibListLinkDA_List(d_Class, d_Member)
#	define DLinkDA_ListAutoDelete(d_Class, d_Member) DMibListLinkDA_ListAutoDelete(d_Class, d_Member)
#endif

	/***************************************************************************************************\
	|ﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯﾯ|
	| List Takes Less Space, but slower																	|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

#define DMibListLinkDS_LinkType NMib::NIntrusive::TCDLink< NMib::NIntrusive::CDLinkAggregateListNoPrevPtr >
#define DMibListLinkDS_Member(d_Member) DMibListLinkDS_LinkType d_Member;

#define DMibListLinkDSA_LinkType NMib::NIntrusive::CDLinkAggregateListNoPrevPtr
#define DMibListLinkDSA_Member(d_Member) DMibListLinkDSA_LinkType d_Member;

#define DMibListLinkDS_Link(d_Class, d_Member) \
	DMibListLinkDS_Member(d_Member) \
	DMibListLinkD_Trans(d_Class, d_Member)

#define DMibListLinkDSA_Link(d_Class, d_Member) \
	DMibListLinkDSA_Member(d_Member) \
	DMibListLinkD_Trans(d_Class, d_Member)

#define DMibListLinkDS_List(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, false \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDS_ListMT(d_Class, d_MemberType) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, d_MemberType \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, false \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDS_ListAutoDelete(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, true \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDS_List_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, false \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDS_ListAutoDelete_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkList \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, true \
		, NMib::NMemory::CDefaultAllocator \
	>

#define DMibListLinkDSA_List(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, false \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDSA_ListAutoDelete(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, true \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDSA_List_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, false \
		, NMib::NMemory::CDefaultAllocator \
	>
#define DMibListLinkDSA_ListAutoDelete_FromTemplate(d_Class, d_Member) \
	NMib::NIntrusive::TCDLinkListAggregate \
	< \
		d_Class \
		, typename d_Class::CDLinkTranslator##d_Member \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtr \
		, NMib::NIntrusive::CDLinkAggregateListNoPrevPtrList \
		, true \
		, NMib::NMemory::CDefaultAllocator \
	>

#define DMibListLinkDS_Iter(d_Class, d_Member) DMibListLinkDSA_List(d_Class, d_Member)::CIterator
#define DMibListLinkDS_IterConst(d_Class, d_Member) DMibListLinkDSA_List(d_Class, d_Member)::CIteratorConst
#define DMibListLinkDS_Iter_FromTemplate(d_Class, d_Member) DMibListLinkDSA_List_FromTemplate(d_Class, d_Member)::CIterator
#define DMibListLinkDS_IterConst_FromTemplate(d_Class, d_Member) DMibListLinkDSA_List_FromTemplate(d_Class, d_Member)::CIteratorConst

#ifndef DMibPNoShortCuts
#	define DLinkDS_Link(d_Class, d_Member) DMibListLinkDS_Link(d_Class, d_Member)
#	define DLinkDS_Iter(d_Class, d_Member) DMibListLinkDS_Iter(d_Class, d_Member)
#	define DLinkDS_IterConst(d_Class, d_Member) DMibListLinkDS_IterConst(d_Class, d_Member)
#	define DLinkDS_List(d_Class, d_Member) DMibListLinkDS_List(d_Class, d_Member)
#	define DLinkDS_ListAutoDelete(d_Class, d_Member) DMibListLinkDS_ListAutoDelete(d_Class, d_Member)
#	define DLinkDSA_Link(d_Class, d_Member) DMibListLinkDSA_Link(d_Class, d_Member)
#	define DLinkDSA_List(d_Class, d_Member) DMibListLinkDSA_List(d_Class, d_Member)
#	define DLinkDSA_ListAutoDelete(d_Class, d_Member) DMibListLinkDSA_ListAutoDelete(d_Class, d_Member)
#endif
}

#include "Malterlib_Intrusive_DoublyLinkedList.hpp"
