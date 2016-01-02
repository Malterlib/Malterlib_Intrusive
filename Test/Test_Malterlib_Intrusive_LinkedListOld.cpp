// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

/*************************************************************************************************\
|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
| CTestSort
|__________________________________________________________________________________________________
\*************************************************************************************************/

#if 0
class CTestSort : public CMalterlibTest
{
public:
	
	aint Rand (void)
	{
		static aint holdrand = 548;
		return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	aint Rand2 (void)
	{
		static aint holdrand = 12164;
		return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	bool f_AutomaticTest() 
	{
		return true;
	}

	class CDataDouble
	{
	public:
		DMibListLinkDS_Link(CDataDouble, m_Link);
		aint Hula;
	};
	class CDataSingle
	{
	public:
		DMibListLinkS_Link(CDataSingle, m_Link);
		aint Hula;
	};
	
	typedef DMibListLinkDS_Iter(CDataDouble, m_Link) CIter_CData;
	typedef DMibListLinkS_Iter(CDataSingle, m_Link) CIterS_CData;
	
	static aint ms_NumCompare;
	static const aint mc_BucketSize = 11;
	
	class CMergeSortClassDouble
	{
	public:
		typedef aint CRet;
		static inline_small CRet fs_Compare(void *_pContext, void *_pFirst, void *_pSecond)
		{
			return ((CDataDouble *)(_pFirst))->Hula - ((CDataDouble *)(_pSecond))->Hula;
		}
	};

	class CMergeSortClassSingle
	{
	public:
		typedef aint CRet;
		static inline_small CRet fs_Compare(void *_pContext, void *_pFirst, void *_pSecond)
		{
			return ((CDataSingle *)(_pFirst))->Hula - ((CDataSingle *)(_pSecond))->Hula;
		}
	};

	class CMergeSortClassNumCompareDouble
	{
	public:
		typedef aint CRet;
		static inline_small CRet fs_Compare(void *_pContext, void *_pFirst, void *_pSecond)
		{
			++ms_NumCompare;
			return ((CDataDouble *)(_pFirst))->Hula - ((CDataDouble *)(_pSecond))->Hula;
		}
	};
	
	class CMergeSortClassNumCompareSingle
	{
	public:
		typedef aint CRet;
		static inline_small CRet fs_Compare(void *_pContext, void *_pFirst, void *_pSecond)
		{
			++ms_NumCompare;
			return ((CDataSingle *)(_pFirst))->Hula - ((CDataSingle *)(_pSecond))->Hula;
		}
	};
	
	class CBucketSortClassDouble
	{
	public:
		static inline_small aint fs_GetIndex(void *_pContext, aint _Place, void *_pItem)
		{
			return (((CDataDouble *)(_pItem))->Hula >> (_Place * mc_BucketSize)) & ((1 << mc_BucketSize) - 1);
		}
	};

	class CBucketSortClassSingle
	{
	public:
		static inline_small aint fs_GetIndex(void *_pContext, aint _Place, void *_pItem)
		{
			return (((CDataSingle *)(_pItem))->Hula >> (_Place * mc_BucketSize)) & ((1 << mc_BucketSize) - 1);
		}
	};

	class CBucketSortClassNumCompareDouble
	{
	public:
		static inline_small aint fs_GetIndex(void *_pContext, aint _Place, void *_pItem)
		{
			++ms_NumCompare;
			return (((CDataDouble *)(_pItem))->Hula >> (_Place * mc_BucketSize)) & ((1 << mc_BucketSize) - 1);
		}
	};
	
	class CBucketSortClassNumCompareSingle
	{
	public:
		static inline_small aint fs_GetIndex(void *_pContext, aint _Place, void *_pItem)
		{
			++ms_NumCompare;
			return (((CDataSingle *)(_pItem))->Hula >> (_Place * mc_BucketSize)) & ((1 << mc_BucketSize) - 1);
		}
	};
	
	static const aint mc_TestSorted = 400000;
	CDataSingle m_DatasSingle[mc_TestSorted];
	CDataDouble m_DatasDouble[mc_TestSorted];
	DMibListLinkS_List(CDataSingle, m_Link) m_ListSingle;
	DMibListLinkDS_List(CDataDouble, m_Link) m_ListDouble;

	void RandomifyList()
	{
		for (aint i = 0; i < mc_TestSorted; ++i)
		{
			aint CurrentRand = Rand() * 0x7fff + Rand2();
			m_DatasSingle[i].Hula = CurrentRand;
			m_DatasDouble[i].Hula = CurrentRand;
		}		
	}

	void UpdateList()
	{
		m_ListSingle.f_Clear();
		for (aint i = 0; i < mc_TestSorted; ++i)
		{
			m_ListDouble.f_Insert(m_DatasDouble[i]);
			m_ListSingle.f_Insert(m_DatasSingle[i]);
		}		
	}

	void TraceListDouble()
	{		
		for (CIter_CData Iter(m_ListDouble);Iter;++Iter)
		{			
			DMibTrace("{}\n", Iter->Hula);
		}
	}

	void TraceListSingle()
	{
		for (CIterS_CData Iter(m_ListSingle);Iter;++Iter)
		{			
			DMibTrace("{}\n", Iter->Hula);
		}
	}

	void ReverseList()
	{
		m_ListDouble.f_Reverse();
		m_ListSingle.f_Reverse();
	}

	NMib::NStr::CStr TestListDouble()
	{
		aint NumInList = 0;
		aint LastInt = 0;
		for (CIter_CData Iter(m_ListDouble);Iter;++Iter)
		{
			++NumInList;
			if (Iter->Hula < LastInt)
				return "List did not sort ok";

			LastInt = Iter->Hula;
		}

		if (NumInList != mc_TestSorted)
		{
			return "List has lost members";
		}

		return "";
	}

	NMib::NStr::CStr TestListSingle()
	{
		aint NumInList = 0;
		aint LastInt = 0;
		for (CIterS_CData Iter(m_ListSingle);Iter;++Iter)
		{
			
			++NumInList;
			if (Iter->Hula < LastInt)
				return "List did not sort ok";

			LastInt = Iter->Hula;
		}

		if (NumInList != mc_TestSorted)
		{
			return "List has lost members";
		}

		return "";
	}

	class CDataTest1
	{
	public:
		DMibListLinkDS_Link(CDataTest1, m_Link);
		aint Hula;
	};
	class CMergeSortCDataTest1
	{
	public:
		typedef aint CRet;
		static inline_small CRet fs_Compare(void *_pContext, CDataTest1 *_pFirst, CDataTest1 *_pSecond)
		{
			return _pFirst->Hula - _pSecond->Hula;
		}
	};


	class CTestAutoPtr
	{
	public:
		DMibAutoClearPtrDeclare;

		int m_Test;
	};

	NMib::NStr::CStr Certify(CTestInterface &_Interface)
	{

		NMib::NContainer::TCVector<NMib::NStr::CStr> Testing;

		NMib::NStr::CStr Stringen = "ntahoeu";

		for (mint i = 0; i < 32768; ++i)
		{
			Testing.f_Insert(Stringen);
		}

		for (mint i = 0; i < 18000; i += 256)
		{
			Testing.f_Remove(0, 256);
		}

		NMib::NPtr::TCAutoClearPtr<CTestAutoPtr> pTest;

		{
			CTestAutoPtr Test;
			pTest = &Test;
			pTest->m_Test = 0;
		}

		DMibSafeCheck(!pTest, "Should not be assigned");

#ifdef DMibDebug
		static const aint NumTests = 1;
#else
		static const aint NumTests = 4;
#endif
		NMib::NStr::CStr TestListReturn;

		{
			DMibListLinkDS_List(CDataTest1, m_Link) List;
			CDataTest1 Test00;
			List.f_Insert(Test00);
			Test00.m_Link.f_Unlink();
			List.f_Insert(Test00);
			List.f_Insert(Test00);
		}

		DMibListLinkDS_List(CDataTest1, m_Link) List;
		DMibListLinkDS_Iter(CDataTest1, m_Link) Iterator;

		CDataTest1 TestData[64];
		
		NMib::NMisc::CRandomShiftRNG Random(0,1,2);
		
		for (int i = 0; i < 64; ++i)
		{
			TestData[i].Hula = Random.f_GetValue<uint32>();
		}

		for (int i = 0; i < 64; ++i)
		{
			List.f_Insert(TestData[i]);
			DMibSafeCheck(List.f_GetLen() == (i + 1), "Mustafa");
			
			List.f_MergeSort<CMergeSortCDataTest1>();
			DMibSafeCheck(List.f_GetLen() == (i + 1), "Mustafa");
		}

		RandomifyList();

		/************************************************************************************************\
		|*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*|
		|_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_|
		|*|*|*|*|*|*|*||¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯||*|*|*|*|*|*|*|
		                                          Doubly linked list 
		|*|*|*|*|*|*|*||___________________________________________________________________||*|*|*|*|*|*|*|
		|¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯|
		|*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*|
		\************************************************************************************************/

		DMibTrace("\n\nSort Doubly linked list\n\n", 0);

		/*************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
		| Merge sort
		|__________________________________________________________________________________________________
		\*************************************************************************************************/

		UpdateList();

	//	CIter_CData Iter(m_ListDouble);
		
	/*	int Test112 = Iter[3]->Hula;

		DMibTrace("sizeof(CDataDouble) = {}\n", (Test112));*/

		DMibTrace("sizeof(CDataDouble) = {}\n", (sizeof(CDataDouble)));

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListDouble.f_MergeSort<CMergeSortClassDouble>();
			}

			DMibTrace("Merge sort random               = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListDouble()).f_GetLen())
			return TestListReturn;

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListDouble.f_MergeSort<CMergeSortClassDouble>();
			}

			DMibTrace("Merge sort sorted               = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListDouble()).f_GetLen())
			return TestListReturn;

		ReverseList();

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListDouble.f_MergeSort<CMergeSortClassDouble>();
			}

			DMibTrace("Merge sort reversed             = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListDouble()).f_GetLen())
			return TestListReturn;

		UpdateList();
		m_ListDouble.f_MergeSort<CMergeSortClassNumCompareDouble>();		

		DMibTrace("Merge sort Numcompare random    = {}\n", ms_NumCompare);

		ms_NumCompare = 0;
		m_ListDouble.f_MergeSort<CMergeSortClassNumCompareDouble>();

		DMibTrace("Merge sort Numcompare sorted    = {}\n", ms_NumCompare);

		ReverseList();
		ms_NumCompare = 0;
		m_ListDouble.f_MergeSort<CMergeSortClassNumCompareDouble>();

		DMibTrace("Merge sort Numcompare reversed  = {}\n", ms_NumCompare);

		/*************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
		| Bucketsort
		|__________________________________________________________________________________________________
		\*************************************************************************************************/

		UpdateList();

/*		DMibTrace("Listbefore:\n", 0);

		for (CIter_CData Iter(m_ListDouble);Iter;++Iter)
		{
			DMibTrace("0x{nh,l4,sf0}\n", (Iter->Hula));
		}*/

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListDouble.f_BucketSort<CBucketSortClassDouble, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);
			}

			DMibTrace("Bucket sort random              = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

	/*	DMibTrace("Listafter:\n", 0);

		for (CIter_CData Iter(m_ListDouble);Iter;++Iter)
		{
			DMibTrace("0x{nh,l4,sf0}\n", (Iter->Hula));
		}*/


		if ((TestListReturn = TestListDouble()).f_GetLen())
			return TestListReturn;

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListDouble.f_BucketSort<CBucketSortClassDouble, 1<<mc_BucketSize>((31 / mc_BucketSize) + 1);
			}

			DMibTrace("Bucket sort sorted              = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListDouble()).f_GetLen())
			return TestListReturn;

		ReverseList();

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListDouble.f_BucketSort<CBucketSortClassDouble, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);
			}

			DMibTrace("Bucket sort reversed            = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListDouble()).f_GetLen())
			return TestListReturn;

		UpdateList();
		ms_NumCompare = 0;
		m_ListDouble.f_BucketSort<CBucketSortClassNumCompareDouble, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);

		DMibTrace("Bucket sort Numcompare random   = {}\n", ms_NumCompare);

		ms_NumCompare = 0;
		m_ListDouble.f_BucketSort<CBucketSortClassNumCompareDouble, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);

		DMibTrace("Bucket sort Numcompare sorted   = {}\n", ms_NumCompare);

		ReverseList();
		ms_NumCompare = 0;
		m_ListDouble.f_BucketSort<CBucketSortClassNumCompareDouble, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);

		DMibTrace("Bucket sort Numcompare reversed = {}\n", ms_NumCompare);
	 
		/************************************************************************************************\
		|*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*|
		|_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_|
		|*|*|*|*|*|*|*||¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯||*|*|*|*|*|*|*|
		                                        Singly linked list 
		|*|*|*|*|*|*|*||___________________________________________________________________||*|*|*|*|*|*|*|
		|¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯*¯|
		|*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*|
		\************************************************************************************************/


		/*************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Merge sort
		|__________________________________________________________________________________________________|
		\*************************************************************************************************/

		UpdateList();

		DMibTrace("\n\nSingly linked list\n\n", 0);

		DMibTrace("sizeof(CDataSingle) = {}\n", (sizeof(CDataSingle)));
//		TraceList2();
		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListSingle.f_MergeSort<CMergeSortClassSingle>();
			}

			DMibTrace("Merge sort random               = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

//		TraceList2();

		if ((TestListReturn = TestListSingle()).f_GetLen())
			return TestListReturn;

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListSingle.f_MergeSort<CMergeSortClassSingle>();
			}

			DMibTrace("Merge sort sorted               = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListSingle()).f_GetLen())
			return TestListReturn;

		ReverseList();

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListSingle.f_MergeSort<CMergeSortClassSingle>();
			}

			DMibTrace("Merge sort reversed             = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListSingle()).f_GetLen())
			return TestListReturn;

		UpdateList();
		m_ListSingle.f_MergeSort<CMergeSortClassNumCompareSingle>();		

		DMibTrace("Merge sort Numcompare random    = {}\n", ms_NumCompare);

		ms_NumCompare = 0;
		m_ListSingle.f_MergeSort<CMergeSortClassNumCompareSingle>();

		DMibTrace("Merge sort Numcompare sorted    = {}\n", ms_NumCompare);

		ReverseList();
		ms_NumCompare = 0;
		m_ListSingle.f_MergeSort<CMergeSortClassNumCompareSingle>();

		DMibTrace("Merge sort Numcompare reversed  = {}\n", ms_NumCompare);

		/*************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
		| Bucketsort
		|__________________________________________________________________________________________________
		\*************************************************************************************************/

		UpdateList();

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListSingle.f_BucketSort<CBucketSortClassSingle, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);
			}

			DMibTrace("Bucket sort random              = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListSingle()).f_GetLen())
			return TestListReturn;

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListSingle.f_BucketSort<CBucketSortClassSingle, 1<<mc_BucketSize>((31 / mc_BucketSize) + 1);
			}


			DMibTrace("Bucket sort sorted              = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListSingle()).f_GetLen())
			return TestListReturn;

		ReverseList();

		{
			NMib::NTime::CTimerMin Timer;

			for (aint i = 0; i < NumTests; ++i)
			{
				DMibScopeTimerMin(Timer);
				m_ListSingle.f_BucketSort<CBucketSortClassSingle, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);
			}

			DMibTrace("Bucket sort reversed            = {0} elements per second ({1} ms)\n", fp64(mc_TestSorted) / Timer.f_GetTime() << (Timer.f_GetTime() * 1000.0));
		}

		if ((TestListReturn = TestListSingle()).f_GetLen())
			return TestListReturn;

		UpdateList();
		ms_NumCompare = 0;
		m_ListSingle.f_BucketSort<CBucketSortClassNumCompareSingle, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);

		DMibTrace("Bucket sort Numcompare random   = {}\n", ms_NumCompare);

		ms_NumCompare = 0;
		m_ListSingle.f_BucketSort<CBucketSortClassNumCompareSingle, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);

		DMibTrace("Bucket sort Numcompare sorted   = {}\n", ms_NumCompare);

		ReverseList();
		ms_NumCompare = 0;
		m_ListSingle.f_BucketSort<CBucketSortClassNumCompareSingle, 1 << mc_BucketSize>((31 / mc_BucketSize) + 1);

		DMibTrace("Bucket sort Numcompare reversed = {}\n", ms_NumCompare);

		return "";
	}	
};

aint CTestSort::ms_NumCompare = 0;

DMibRuntimeClass(CMalterlibTest, CTestSort);
#endif
