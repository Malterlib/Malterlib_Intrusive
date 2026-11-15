// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Mib/Test/Test>
#include <Mib/Intrusive/DoublyLinkedList>
#include <Mib/Intrusive/SinglyLinkedList>

namespace
{
	using namespace NMib;
	using namespace NMib::NStr;

	struct CDoubleEntry
	{
		DMibListLinkD_Link(CDoubleEntry, m_Link);
		aint m_Value;
	};

	struct CSingleEntry
	{
		DMibListLinkS_Link(CSingleEntry, m_Link);
		aint m_Value;
	};

	constexpr aint gc_BucketBits = 2;
	constexpr aint gc_nBuckets = aint(1) << gc_BucketBits;
	constexpr aint gc_nPlaces = 2;
	constexpr aint gc_Values[] = {13, 2, 7, 0, 11, 2, 15, 4};

	// The bucket of an entry for one place: two bits of its value, the least significant first.
	template <typename tf_CEntry>
	aint fg_GetBucketIndex(void *_pContext, aint _Place, void *_pItem)
	{
		return (static_cast<tf_CEntry *>(_pItem)->m_Value >> (_Place * gc_BucketBits)) & (gc_nBuckets - 1);
	}

	template <typename t_CEntry>
	struct TCBucketSortIndex
	{
		static aint fs_GetIndex(void *_pContext, aint _Place, void *_pItem);
	};

	template <typename t_CEntry>
	aint TCBucketSortIndex<t_CEntry>::fs_GetIndex(void *_pContext, aint _Place, void *_pItem)
	{
		return fg_GetBucketIndex<t_CEntry>(_pContext, _Place, _pItem);
	}

	struct CLinkedList_Tests : public NMib::NTest::CTest
	{
		template <typename tf_CEntry, typename tf_CList, typename tf_CIterator, typename tf_FSort>
		static CStr fs_SortValues(tf_FSort &&_fSort);

		void f_DoTests();
	};

	// Sorts gc_Values with _fSort and returns the values in list order.
	template <typename tf_CEntry, typename tf_CList, typename tf_CIterator, typename tf_FSort>
	CStr CLinkedList_Tests::fs_SortValues(tf_FSort &&_fSort)
	{
		tf_CEntry Entries[sizeof(gc_Values) / sizeof(gc_Values[0])];
		tf_CList List;
		for (umint iEntry = 0; iEntry < sizeof(gc_Values) / sizeof(gc_Values[0]); ++iEntry)
		{
			Entries[iEntry].m_Value = gc_Values[iEntry];
			List.f_Insert(Entries[iEntry]);
		}

		_fSort(List);

		CStr Values;
		for (tf_CIterator iEntry(List); iEntry; ++iEntry)
			fg_AddStrSep(Values, CStr("{}"_f << iEntry->m_Value), ", ");

		List.f_Clear();
		return Values;
	}

	void CLinkedList_Tests::f_DoTests()
	{
		CStr const Sorted = "0, 2, 2, 4, 7, 11, 13, 15";

		DMibTestSuite("BucketSort")
		{
			using CDoubleList = DMibListLinkD_List(CDoubleEntry, m_Link);
			using CDoubleIterator = DMibListLinkD_Iter(CDoubleEntry, m_Link);
			using CSingleList = DMibListLinkS_List(CSingleEntry, m_Link);
			using CSingleIterator = DMibListLinkS_Iter(CSingleEntry, m_Link);

			CStr DoubleSorted = fs_SortValues<CDoubleEntry, CDoubleList, CDoubleIterator>
				(
					[](CDoubleList &_List)
					{
						_List.template f_BucketSort<TCBucketSortIndex<CDoubleEntry>, gc_nBuckets>(gc_nPlaces);
					}
				)
			;
			CStr DoubleCallbackSorted = fs_SortValues<CDoubleEntry, CDoubleList, CDoubleIterator>
				(
					[](CDoubleList &_List)
					{
						_List.template f_BucketSortCallback<gc_nBuckets>(gc_nPlaces, &fg_GetBucketIndex<CDoubleEntry>);
					}
				)
			;
			CStr SingleSorted = fs_SortValues<CSingleEntry, CSingleList, CSingleIterator>
				(
					[](CSingleList &_List)
					{
						_List.template f_BucketSort<TCBucketSortIndex<CSingleEntry>, gc_nBuckets>(gc_nPlaces);
					}
				)
			;
			CStr SingleCallbackSorted = fs_SortValues<CSingleEntry, CSingleList, CSingleIterator>
				(
					[](CSingleList &_List)
					{
						_List.template f_BucketSortCallback<gc_nBuckets>(gc_nPlaces, &fg_GetBucketIndex<CSingleEntry>);
					}
				)
			;

			CStr AllSorted = "{}\n{}\n{}\n{}"_f << DoubleSorted << DoubleCallbackSorted << SingleSorted << SingleCallbackSorted;
			DMibExpect(AllSorted, ==, CStr("{0}\n{0}\n{0}\n{0}"_f << Sorted));
		};
	}

	DMibTestRegister(CLinkedList_Tests, Malterlib::Intrusive);
}
