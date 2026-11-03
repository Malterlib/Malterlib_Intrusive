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
		constexpr CSLinkAggr(EAggregateInitialization _Init);
		inline_always CSLinkAggr();

		CSLinkAggr *m_pNext;

		inline_small void f_Construct();
		inline_small void f_Destruct();
		inline_small void f_Link(CSLinkAggr *_pLinkAfter);
		inline_small void f_Unlink(CSLinkAggr *_pPrev);
		inline_small void f_UnsafeLink(CSLinkAggr *_pLinkAfter);
		inline_small void f_UnsafeUnlink(CSLinkAggr *_pPrev);
		inline_small CSLinkAggr *f_GetNext() const;
		inline_small void f_SetNext(CSLinkAggr *_pNext);
		inline_small bool f_IsInList() const;

	private:
		using CThis = CSLinkAggr;
	};

	struct CSLink : public CSLinkAggr
	{
		inline_small CSLink();
		inline_small ~CSLink();

	private:
		CSLink(CSLink const &_Other) = delete;
		CSLink &operator = (CSLink const &_Other) = delete;
	};

	struct CSLinkListData_Last
	{
		constexpr CSLinkListData_Last(EAggregateInitialization _Init);
		inline_always CSLinkListData_Last();

		CSLinkAggr m_First;
		CSLinkAggr *m_pLast;
		inline_small CSLinkAggr &f_GetFirst();
		inline_small CSLinkAggr const &f_GetFirst() const;
		inline_small CSLinkAggr *f_GetLast();
		inline_small CSLinkAggr const *f_GetLast() const;
		inline_small void f_SetLast(CSLinkAggr *_pLast);
		static inline_small bool fs_HasLast();
	};

	struct CSLinkListData
	{
		constexpr CSLinkListData(EAggregateInitialization _Init);
		inline_always CSLinkListData();

		CSLinkAggr m_First;
		inline_small CSLinkAggr &f_GetFirst();
		inline_small CSLinkAggr *f_GetLast();
		inline_small CSLinkAggr const &f_GetFirst() const;
		inline_small CSLinkAggr const *f_GetLast() const;
		inline_small void f_SetLast(CSLinkAggr *_pLast);
		static inline_small bool fs_HasLast();
	};

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	struct TCSLinkListAggregate
	{
	private:
		// Declared before the public section, which uses it.
		using COffset = typename t_CTranslator::template TCOffset<t_CData>;

	public:
		constexpr TCSLinkListAggregate(EAggregateInitialization _Init);

		inline_always TCSLinkListAggregate() = default;

		t_CListData m_Data;

		void f_Construct();
		void f_Destruct();
		void f_ClearFast(); // Fast clear that does not set the linked members to the unlinked state.
		void f_Clear();
		void f_DeleteAll();
		void f_DeleteAllDefiniteType();
		void f_Remove(t_CData *_pToRemove);
		static void fs_UnsafeUnlink(t_CData *_pToRemove, t_CData *_pPrev);
		static t_CData *fs_GetNext(t_CData *_pData);
		static void fs_Unlink(t_CData *_pToRemove, t_CData *_pPrev);
		inline_medium t_CData *f_Pop();
		inline_medium t_CData *f_UnsafePop();
		inline_small void f_Push(t_CData *_pData);
		void f_Delete(t_CData *_pToDelete);
		inline_small t_CData *f_GetFirst();
		inline_small t_CData *f_GetLast();
		inline_small t_CData const *f_GetFirst() const;
		inline_small t_CData const *f_GetLast() const;
		inline_small bool f_IsEmpty() const;

		//===================================
		// Inserts
		//===================================

		inline_small void f_Insert(t_CData *_pToInsert);
		inline_small void f_Insert(t_CData &_ToInsert);
		template <class tf_CSortClass>
		void f_InsertSorted(t_CData &_ToInsert);
		template <typename tf_FCompare>
		void f_InsertSorted(t_CData *_pToInsert, tf_FCompare &&_fCompare);

		// InsertTail
		inline_small void f_InsertLast(t_CData *_pToInsert);
		inline_small void f_InsertLast(t_CData &_ToInsert);

		// InsertHead
		inline_small void f_InsertFirst(t_CData *_pToInsert);
		inline_small void f_InsertFirst(t_CData &_ToInsert);

		// f_InsertAfter
		inline_small void f_InsertAfter(t_CData *_pToInsert, t_CData *_pToInsertAfter);
		inline_small void f_InsertAfter(t_CData &_ToInsert, t_CData *_pToInsertAfter);
		inline_small void f_InsertAfter(t_CData *_pToInsert, t_CData &_ToInsertAfter);
		inline_small void f_InsertAfter(t_CData &_ToInsert, t_CData &_ToInsertAfter);

		//===================================
		// Unsafe Inserts
		//===================================

		inline_small void f_UnsafeInsert(t_CData *_pToInsert);
		inline_small void f_UnsafeInsert(t_CData &_ToInsert);

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

		void f_Reverse();

		template <class tf_FCompare>
		void f_MergeSort(tf_FCompare &&_fCompare, aint _InsertionBits = 3);

		using FBucketGetIndex = aint (void *_pContext, aint _Place, void *_pItem);

		struct CBucketCallbackSort
		{
			void *m_pContext;
			FBucketGetIndex *m_fSortFunction;
			static inline_small aint fs_GetIndex(void *_pContext, aint _Place, void *_pItem);
		};

		template <aint tf_BucketSize>
		void f_BucketSortCallback(FBucketGetIndex *_fSortFunction, void *_pContext = nullptr);

		template <class tf_CSortClass, aint tf_BucketSize>
		void f_BucketSort(aint _NumPlaces, void *_pContext = nullptr);

		template <typename tf_FCompare>
		void f_Sort(tf_FCompare &&_fCompare);

		struct CIterator
		{
			CSLinkAggr *m_pCurrent;

			CIterator();
			CIterator(TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List);
			CIterator f_GetIterator() const;
			CIterator &operator = (TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List);
			inline_small t_CData *f_GetCurrent();
			inline_small operator t_CData * ();
			inline_small t_CData *operator -> ();
			inline_small void operator ++ ();

		private:
			using COffset = COffset;
		};

		CIterator f_GetIterator();

		struct CIteratorConst
		{
			CSLinkAggr const *m_pCurrent;

			CIteratorConst();
			CIteratorConst(TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List);
			CIteratorConst f_GetIterator() const;
			CIteratorConst &operator = (TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator> const &_List);
			inline_small t_CData const *f_GetCurrent();
			inline_small operator t_CData const * ();
			inline_small t_CData const *operator -> ();
			inline_small void operator ++ ();

		private:
			using COffset = COffset;
		};

		CIteratorConst f_GetIterator() const;
		umint f_GetLen() const;

	private:
		static inline_small CSLinkAggr *fsp_LinkFromMember(t_CData *_pMember);
		static inline_small t_CData *fsp_MemberFromLink(CSLinkAggr *_pLink);
		static inline_small CSLinkAggr const *fsp_LinkFromMember(t_CData const *_pMember);
		static inline_small t_CData const *fsp_MemberFromLink(CSLinkAggr const *_pLink);
		template <bool tf_bDoDelete>
		TCEnableIf<tf_bDoDelete> fp_AutoDelete();
		template <bool tf_bDoDelete>
		TCEnableIf<!tf_bDoDelete> fp_AutoDelete();
	};

	template <typename t_CData, typename t_CTranslator, typename t_CListData, bool t_bAutoDelete, typename t_CAllocator>
	struct TCSLinkList : public TCSLinkListAggregate<t_CData, t_CTranslator, t_CListData, t_bAutoDelete, t_CAllocator>
	{
		TCSLinkList();
		~TCSLinkList();

	private:
		TCSLinkList(TCSLinkList const &_Other) = delete;
		TCSLinkList &operator = (TCSLinkList const &_Other) = delete;
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
						static constexpr t_CClass const *c_pNode = DMibRelaxConstexpr((t_CClass const *)(void *)(NMib::TCLimitsInt<smint>::mc_Max / 2 + 1));\
						return DMibRelaxConstexpr((uint8 const *)&(c_pNode->*t_pMember) - (uint8 const *)c_pNode);\
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

#include "Malterlib_Intrusive_SinglyLinkedList.hpp"
