// Copyright © 2015 Hansoft AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include <Mib/Core/Core>

#define TreeDebugNum 128
#define TreeNumTests 32
#define TreeTestNum 64*1024
//#define TreeTestNum 256
//#define TreeMixedList

using namespace NMib::NIntrusive;
#if 0
class CTestTree : public CMalterlibTest
{
public:
	CTestTree()
	{
		holdrand = 548;
	}

	aint holdrand;
	aint Rand (void)
	{
		return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	bool f_AutomaticTest()
	{
		return true;
	}


	class CCompare;
	class CCompare2;
#if 0
	class CTestClass
	{
	public:

		CTestClass()
		{
		}
		~CTestClass()
		{
		}
		DMibTreeAVL_Link(CTestClass, m_Link, CCompare);
		NMib::NStr::CStr m_Data;
	};

	class CTestClass2
	{
	public:

		CTestClass2()
		{
		}
		~CTestClass2()
		{
		}
		DMibTreeAVL2_Link(CTestClass, m_Link, CCompare2);
		NMib::NStr::CStr m_Data;
	};

	class CCompare
	{
	public:
		using CRet = aint;

		static inline_small CRet fs_Compare(const CTestClass *_pFirst, const CTestClass *_pSecond, void *_pContext)
		{
//			DMibTrace("Data1 {} Data2 {}\n", (_pFirst->m_Data), (_pSecond->m_Data));
			return _pFirst->m_Data.f_CmpNoCase(_pSecond->m_Data);
		}

		static inline_small CRet fs_Compare(const CTestClass *_pTestClass, const NMib::NStr::CStr &_Key, void *_pContext)
		{
			return _pTestClass->m_Data.f_CmpNoCase(_Key);
		}
	};
	class CCompare2
	{
	public:
		using CRet = aint;

		static inline_small CRet fs_Compare(const CTestClass2 *_pFirst, const CTestClass2 *_pSecond, void *_pContext)
		{
			return _pFirst->m_Data.f_CmpNoCase(_pSecond->m_Data);
		}

		static inline_small CRet fs_Compare(const CTestClass2 *_pTestClass, const NMib::NStr::CStr &_Key, void *_pContext)
		{
			return _pTestClass->m_Data.f_CmpNoCase(_Key);
		}
	};
	DMibTreeAVL_Tree(CTestClass, m_Link, CCompare) m_TestTree;
	DMibTreeAVL2_Tree(CTestClass2, m_Link, CCompare2) m_TestTree2;
	void RandomizeList()
	{
#ifdef TreeMixedList
		for (aint i = 0; i < mc_TestSorted; i+=2)
		{
			m_ClassList[i].m_Data = i/2;
			m_ClassList2[i].m_Data = i/2;
		}
		for (aint i = 1; i < mc_TestSorted; i+=2)
		{
			m_ClassList[i].m_Data = i/2+mc_TestSorted/2;
			m_ClassList2[i].m_Data = i/2+mc_TestSorted/2;
		}
#else
		for (aint i = 0; i < mc_TestSorted; ++i)
		{
			m_ClassList[i].m_Data = (NMib::NStr::CStr::CFormat("staohnkthoansekhaosntuehsaontekhsnaothkstnaoheknstoahuetnaohksntoahentsuuhaontkho{sj10,sf0}") << i).f_GetStr();
			m_ClassList2[i].m_Data = m_ClassList[i].m_Data;
		}
#endif

	}
#else
	class CTestClass
	{
	public:

		CTestClass()
		{
		}
		~CTestClass()
		{
		}
		TCAVLLink<EAVLLinkType_Unaligned> m_Link;
		aint m_Data;
	};

	class CTestClass2
	{
	public:

		CTestClass2()
		{
		}
		~CTestClass2()
		{
		}
		TCAVLLink<EAVLLinkType_Unaligned> m_Link;
		aint m_Data;
	};

	class CCompare
	{
	public:
		inline_small aint const &operator () (CTestClass const &_Node) const
		{
			return _Node.m_Data;
		}
	};
	class CCompare2
	{
	public:
		inline_small aint const &operator () (CTestClass2 const &_Node) const
		{
			return _Node.m_Data;
		}
	};

	TCAVLTree<&CTestClass::m_Link, CCompare> m_TestTree;
	TCAVLTree<&CTestClass2::m_Link, CCompare2> m_TestTree2;

	void RandomizeList()
	{
#ifdef TreeMixedList
		for (aint i = 0; i < mc_TestSorted; i+=2)
		{
			m_ClassList[i].m_Data = i/2;
			m_ClassList2[i].m_Data = i/2;
		}
		for (aint i = 1; i < mc_TestSorted; i+=2)
		{
			m_ClassList[i].m_Data = i/2+mc_TestSorted/2;
			m_ClassList2[i].m_Data = i/2+mc_TestSorted/2;
		}
#else
		for (aint i = 0; i < mc_TestSorted; ++i)
		{
			m_ClassList[i].m_Data = i;
			m_ClassList2[i].m_Data = m_ClassList[i].m_Data;
		}
#endif

	}
#endif


	static const aint mc_TestSorted = TreeTestNum;

	CTestClass m_ClassList[mc_TestSorted];
	CTestClass2 m_ClassList2[mc_TestSorted];


	template <typename t_CTree, typename t_CTestClass>
	static void TreeGetMaxDepth(t_CTree &_Tree, t_CTestClass *_pObj, aint &_MaxDepth, aint &_CurrentDepth)
	{
		++_CurrentDepth;


		t_CTestClass *pRight = _Tree.fs_GetRight(_pObj);
		t_CTestClass *pLeft = _Tree.fs_GetLeft(_pObj);

		if (!pRight &&!pLeft)
		{
			if (_CurrentDepth > _MaxDepth)
				_MaxDepth = _CurrentDepth;
		}

		if (pLeft)
			TreeGetMaxDepth(_Tree, pLeft, _MaxDepth, _CurrentDepth);
		if (pRight)
			TreeGetMaxDepth(_Tree, pRight, _MaxDepth, _CurrentDepth);


		--_CurrentDepth;
	}

	template <typename t_CTree, typename t_CTestClass>
	static void TreeGetMinDepth(t_CTree &_Tree, t_CTestClass *_pObj, aint &_MinDepth, aint &_CurrentDepth)
	{
		++_CurrentDepth;

		t_CTestClass *pRight = _Tree.fs_GetRight(_pObj);
		t_CTestClass *pLeft = _Tree.fs_GetLeft(_pObj);

		if (!pRight &&!pLeft)
		{
			if (_CurrentDepth < _MinDepth)
				_MinDepth = _CurrentDepth;

		}

		if (pLeft)
			TreeGetMinDepth(_Tree, pLeft, _MinDepth, _CurrentDepth);
		if (pRight)
			TreeGetMinDepth(_Tree, pRight, _MinDepth, _CurrentDepth);

		--_CurrentDepth;
	}


	template <typename t_CTree, typename t_CTestClass>
	static t_CTestClass *GetTreeLargest(t_CTree &_Tree, t_CTestClass *_pObj)
	{
		t_CTestClass *CurrentLargest = _pObj;

		t_CTestClass *pRight = _Tree.fs_GetRight(_pObj);
		t_CTestClass *pLeft = _Tree.fs_GetLeft(_pObj);

		if (pLeft)
		{
			t_CTestClass *Largest;
//			if (t_CTree::CCompare()(*CurrentLargest, *(Largest = GetTreeLargest(_Tree, pLeft))))
	//			CurrentLargest = Largest;
		}

		if (pRight)
		{
			t_CTestClass *Largest;
//			if (t_CTree::CCompare()(*CurrentLargest, *(Largest = GetTreeLargest(_Tree, pRight))))
	//			CurrentLargest = Largest;
		}

		return CurrentLargest;
	}

	template <typename t_CTree, typename t_CTestClass>
	static t_CTestClass *GetTreeSmallest(t_CTree &_Tree, t_CTestClass *_pObj)
	{
		t_CTestClass *CurrentSmallest = _pObj;

		t_CTestClass *pRight = _Tree.fs_GetRight(_pObj);
		t_CTestClass *pLeft = _Tree.fs_GetLeft(_pObj);

		if (pLeft)
		{
			t_CTestClass *Smallest;
//			if (t_CTree::CCompare()(*(Smallest = GetTreeSmallest(_Tree, pLeft)), *CurrentSmallest))
	//			CurrentSmallest = Smallest;
		}

		if (pRight)
		{
			t_CTestClass *Smallest;
//			if (t_CTree::CCompare()(*(Smallest = GetTreeSmallest(_Tree, pRight)), *CurrentSmallest))
	//			CurrentSmallest = Smallest;
		}

		return CurrentSmallest;
	}

	template <typename t_CTree, typename t_CTestClass>
	static void f_CheckTree(t_CTree &_Tree, t_CTestClass *_pObj)
	{
		t_CTestClass *pRight = _Tree.fs_GetRight(_pObj);
		t_CTestClass *pLeft = _Tree.fs_GetLeft(_pObj);

		aint LeftMaxDepth = 0;
		aint RightMaxDepth = 0;

		if (pLeft)
		{
			aint CurrentDepth = 0;
			TreeGetMaxDepth(_Tree, pLeft, LeftMaxDepth, CurrentDepth);
//			DMibSafeCheck(!t_CTree::CCompare()(*_pObj, *GetTreeLargest(_Tree, pLeft)),"Tree is not binary");
			f_CheckTree(_Tree, pLeft);
		}
		if (pRight)
		{
			aint CurrentDepth = 0;
			TreeGetMaxDepth(_Tree, pRight, RightMaxDepth, CurrentDepth);
//			DMibSafeCheck(t_CTree::CCompare()(*_pObj, *GetTreeSmallest(_Tree, pRight)),"Tree is not binary");
			f_CheckTree(_Tree, pRight);
		}
		if (LeftMaxDepth > RightMaxDepth)
		{
			DMibSafeCheck((LeftMaxDepth - RightMaxDepth) <= 1, "Tree has lost its AVL Property");
		}
		else
		{
			DMibSafeCheck((RightMaxDepth - LeftMaxDepth) <= 1, "Tree has lost its AVL Property");
		}

	}

	template <typename t_CTree0, typename t_CTree1, typename t_CTestClass0, typename t_CTestClass1>
	static bool TreesSame(t_CTree0 &_Tree0, t_CTree1 &_Tree1, t_CTestClass0 *_pObj0, t_CTestClass1 *_pObj1)
	{
		if (_pObj0 && !_pObj1)
			return false;
		if (_pObj1 && !_pObj0)
			return false;
		if (!_pObj0)
			return true;
		if (_pObj0->m_Data != _pObj1->m_Data)
			return false;

		int Skew0 = _pObj0->m_Link.m_Link.f_GetSkew();
		switch(Skew0)
		{
		case t_CTree0::CLink::EAVLTreeSkew_None:
				Skew0 = 0; break;
		case t_CTree0::CLink::EAVLTreeSkew_Right:
				Skew0 = 1; break;
		case t_CTree0::CLink::EAVLTreeSkew_Left:
				Skew0 = -1; break;
		default:
			Skew0 = -2;break;
		}
		int Skew1 = _pObj1->m_Link.m_Link.f_GetSkew();
		switch(Skew1)
		{
		case t_CTree1::CLink::EAVLTreeSkew_None:
				Skew1 = 0; break;
		case t_CTree1::CLink::EAVLTreeSkew_Right:
				Skew1 = 1; break;
		case t_CTree1::CLink::EAVLTreeSkew_Left:
				Skew1 = -1; break;
		default:
			Skew1 = -2;break;
		}


		if (Skew0 != Skew1)
			return false;

		t_CTestClass0 *pRight0 = _Tree0.fs_GetRight(_pObj0);
		t_CTestClass0 *pLeft0 = _Tree0.fs_GetLeft(_pObj0);

		t_CTestClass1 *pRight1 = _Tree1.fs_GetRight(_pObj1);
		t_CTestClass1 *pLeft1 = _Tree1.fs_GetLeft(_pObj1);
		if (!TreesSame(_Tree0, _Tree1, pRight0, pRight1))
			return false;
		if (!TreesSame(_Tree0, _Tree1, pLeft0, pLeft1))
			return false;
		return true;


	}

	template <typename t_CTree>
	static void f_CheckTree(t_CTree &_Tree)
	{

		_Tree.f_CheckTree(true);

		if (_Tree.f_GetRoot())
		{
			f_CheckTree(_Tree, _Tree.f_GetRoot());
		}
	}

	template <typename t_CTree, typename t_CTestClass>
	static void TraceTree(t_CTree &_Tree, t_CTestClass *_pParent, t_CTestClass *_pObj, aint _Depth, aint &_CurrentDepth, bool &_bTraced, bool _bLeft, aint _MaxDepth)
	{
		if (_Depth == _CurrentDepth)
		{
			_bTraced = true;
			int Skew = _pObj->m_Link.m_Link.f_GetSkew();
			switch(Skew)
			{
			case t_CTree::CLink::EAVLTreeSkew_None:
					Skew = 0; break;
			case t_CTree::CLink::EAVLTreeSkew_Right:
					Skew = 1; break;
			case t_CTree::CLink::EAVLTreeSkew_Left:
					Skew = -1; break;
			}

			if (_bLeft)
				DMibTrace("{L*1}", (NMib::NStr::CStr::CFormat("{}.L={}.{}") << (_pParent->m_Data) << (_pObj->m_Data) << (Skew)).f_GetStr(), (12));
			else
				DMibTrace("{L*1}", (NMib::NStr::CStr::CFormat("{}.R={}.{}") << (_pParent->m_Data) << (_pObj->m_Data) << (Skew)).f_GetStr(), (12));
		}
		++_CurrentDepth;

		t_CTestClass *pRight = _Tree.fs_GetRight(_pObj);
		t_CTestClass *pLeft = _Tree.fs_GetLeft(_pObj);

		if (pLeft)
		{
			TraceTree(_Tree, _pObj, pLeft, _Depth, _CurrentDepth, _bTraced, true, _MaxDepth);
		}

		if (pRight)
		{
			TraceTree(_Tree, _pObj, pRight, _Depth, _CurrentDepth, _bTraced, false, _MaxDepth);
		}
		--_CurrentDepth;
	}

	template <typename t_CTree>
	static void TraceTree(t_CTree &_Tree)
	{
		if (!_Tree.f_GetRoot())
			return;
		aint MaxDepth = 0;
		aint CurrentDepth = 0;
		TreeGetMaxDepth(_Tree, _Tree.f_GetRoot(), MaxDepth, CurrentDepth);
		DMibTrace("Tree Trace Depth({}):\n", (MaxDepth));
		for (aint i = 0; i < MaxDepth; ++i)
		{
			bool Traced = false;
			aint CurrentDepth = 0;
			TraceTree(_Tree, _Tree.f_GetRoot(), _Tree.f_GetRoot(), i, CurrentDepth, Traced, false, MaxDepth);
			if (Traced)
			{
				DMibTrace("\n", 0);
			}
		}
	}

	NMib::NStr::CStr Certify(CTestInterface &_Interface)
	{
	//	DMibTrace("\n\nAVLTree\n\n", 0);
		DMibTrace("sizeof(CTestClass) = {}\n", (sizeof(CTestClass)));

		RandomizeList();

		aint DebugTest = TreeDebugNum;

#ifdef DMibDebug
		aint NumTests = 1;
#else
		aint NumTests = TreeNumTests;
#endif

		{
			// Ceck validity of tree code
			for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
			{
				m_TestTree.f_Insert(m_ClassList[i]);
				f_CheckTree(m_TestTree);
				m_TestTree2.fr_Insert(m_ClassList2[i]);
				f_CheckTree(m_TestTree2);
				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
			}

			f_CheckTree(m_TestTree);
			f_CheckTree(m_TestTree2);

			int iDebugMax = (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted);

			holdrand = 548;
			for (aint i = 0; i < 2048; ++i)
			{
				int iTest = Rand() % iDebugMax;
				if (m_ClassList[iTest].m_Link.f_IsInTree())
					m_TestTree.f_Remove(m_ClassList[iTest]);
				else
					m_TestTree.f_Insert(m_ClassList[iTest]);
				if (m_ClassList2[iTest].m_Link.f_IsInTree())
					m_TestTree2.fr_Remove(m_ClassList2[iTest]);
				else
					m_TestTree2.fr_Insert(m_ClassList2[iTest]);

				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
				f_CheckTree(m_TestTree);
				f_CheckTree(m_TestTree2);
			}

			f_CheckTree(m_TestTree);
			f_CheckTree(m_TestTree2);
			for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
			{
				if (!m_ClassList[i].m_Link.f_IsInTree())
					m_TestTree.f_Insert(m_ClassList[i]);
				if (!m_ClassList2[i].m_Link.f_IsInTree())
					m_TestTree2.fr_Insert(m_ClassList2[i]);
				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
				f_CheckTree(m_TestTree);
				f_CheckTree(m_TestTree2);
			}

			for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
			{
				m_TestTree.f_Remove(m_ClassList[i]);
				m_TestTree2.fr_Remove(m_ClassList2[i]);
			//	TraceTree();
				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
				f_CheckTree(m_TestTree);
				f_CheckTree(m_TestTree2);
			}
			f_CheckTree(m_TestTree);
			f_CheckTree(m_TestTree2);
		}

		{
			// Ceck validity of tree code
			for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
			{
				m_TestTree.f_Insert(m_ClassList[i]);
				f_CheckTree(m_TestTree);
				m_TestTree2.f_InsertLowStack(m_ClassList2[i]);
				f_CheckTree(m_TestTree2);
				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
			}

			f_CheckTree(m_TestTree);
			f_CheckTree(m_TestTree2);

			int iDebugMax = (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted);

			holdrand = 548;
			for (aint i = 0; i < 2048; ++i)
			{
				int iTest = Rand() % iDebugMax;
				if (m_ClassList[iTest].m_Link.f_IsInTree())
					m_TestTree.f_Remove(m_ClassList[iTest]);
				else
					m_TestTree.f_Insert(m_ClassList[iTest]);
				if (m_ClassList2[iTest].m_Link.f_IsInTree())
					m_TestTree2.f_RemoveLowStack(m_ClassList2[iTest]);
				else
					m_TestTree2.f_InsertLowStack(m_ClassList2[iTest]);

				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
				f_CheckTree(m_TestTree2);
				f_CheckTree(m_TestTree);
			}

			f_CheckTree(m_TestTree);
			f_CheckTree(m_TestTree2);
			for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
			{
				if (!m_ClassList[i].m_Link.f_IsInTree())
					m_TestTree.f_Insert(m_ClassList[i]);
				if (!m_ClassList2[i].m_Link.f_IsInTree())
					m_TestTree2.f_InsertLowStack(m_ClassList2[i]);
				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
				f_CheckTree(m_TestTree);
				f_CheckTree(m_TestTree2);
			}
			for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
			{
				m_TestTree.f_Remove(m_ClassList[i]);
				m_TestTree2.f_RemoveLowStack(m_ClassList2[i]);
			//	TraceTree();
				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
				f_CheckTree(m_TestTree);
				f_CheckTree(m_TestTree2);
			}
			f_CheckTree(m_TestTree);
			f_CheckTree(m_TestTree2);
		}

		{
			// Ceck validity of tree code
			for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
			{
				m_TestTree.f_Insert(m_ClassList[i]);
				f_CheckTree(m_TestTree);
				m_TestTree2.f_Insert(m_ClassList2[i]);
				f_CheckTree(m_TestTree2);
				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
			}

			f_CheckTree(m_TestTree);
			f_CheckTree(m_TestTree2);

			int iDebugMax = (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted);

			holdrand = 548;
			for (aint i = 0; i < 2048; ++i)
			{
				int iTest = Rand() % iDebugMax;
				if (m_ClassList[iTest].m_Link.f_IsInTree())
					m_TestTree.f_Remove(m_ClassList[iTest]);
				else
					m_TestTree.f_Insert(m_ClassList[iTest]);
				if (m_ClassList2[iTest].m_Link.f_IsInTree())
					m_TestTree2.f_Remove(m_ClassList2[iTest]);
				else
					m_TestTree2.f_Insert(m_ClassList2[iTest]);

				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
				f_CheckTree(m_TestTree);
				f_CheckTree(m_TestTree2);
			}

			f_CheckTree(m_TestTree);
			f_CheckTree(m_TestTree2);
			for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
			{
				if (!m_ClassList[i].m_Link.f_IsInTree())
					m_TestTree.f_Insert(m_ClassList[i]);
				if (!m_ClassList2[i].m_Link.f_IsInTree())
					m_TestTree2.f_Insert(m_ClassList2[i]);
				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
				f_CheckTree(m_TestTree);
				f_CheckTree(m_TestTree2);
			}
			for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
			{
				m_TestTree.f_Remove(m_ClassList[i]);
				m_TestTree2.f_Remove(m_ClassList2[i]);
				if (!TreesSame(m_TestTree, m_TestTree2, m_TestTree.f_GetRoot(), m_TestTree2.f_GetRoot()))
				{
					TraceTree(m_TestTree);
					TraceTree(m_TestTree2);
				}
				f_CheckTree(m_TestTree);
				f_CheckTree(m_TestTree2);
			}
			f_CheckTree(m_TestTree);
			f_CheckTree(m_TestTree2);
		}


		DMibSafeCheck(!m_TestTree.f_GetRoot(), "Must be empty");
		DMibSafeCheck(!m_TestTree2.f_GetRoot(), "Must be empty");

		{
			NMib::NTime::CPerfTimeMeasureMin TimerInsert;
			NMib::NTime::CPerfTimeMeasureMin TimerDelete;
			for (aint i = 0; i < NumTests; ++i)
			{
				{
					DMibScopePerfTimeMeasureMin(TimerInsert);
					for (aint i = 0; i < mc_TestSorted; ++i)
					{
						m_TestTree.f_Insert(m_ClassList[i]);
					}
				}
				{
					DMibScopePerfTimeMeasureMin(TimerDelete);
					for (aint i = 0; i < mc_TestSorted; ++i)
					{
						m_TestTree.f_Remove(m_ClassList[i]);
					}
				}
			}

			DMibTrace("Performance for AVLTree f_Insert = {0} inserts per second\n", fp64(mc_TestSorted) / TimerInsert.f_GetTime());
			DMibTrace("Performance for AVLTree f_Remove = {0} removes per second\n", fp64(mc_TestSorted) / TimerDelete.f_GetTime());
		}

		{
			NMib::NTime::CPerfTimeMeasureMin TimerInsert;
			NMib::NTime::CPerfTimeMeasureMin TimerDelete;
			for (aint i = 0; i < NumTests; ++i)
			{
				{
					DMibScopePerfTimeMeasureMin(TimerInsert);
					for (aint i = 0; i < mc_TestSorted; ++i)
					{
						m_TestTree.f_InsertLowStack(m_ClassList[i]);
					}
				}
				{
					DMibScopePerfTimeMeasureMin(TimerDelete);
					for (aint i = 0; i < mc_TestSorted; ++i)
					{
						m_TestTree.f_RemoveLowStack(m_ClassList[i]);
					}
				}
			}

			DMibTrace("Performance for AVLTree f_InsertLowStack = {0} inserts per second\n", fp64(mc_TestSorted) / TimerInsert.f_GetTime());
			DMibTrace("Performance for AVLTree f_RemoveLowStack = {0} removes per second\n", fp64(mc_TestSorted) / TimerDelete.f_GetTime());
		}

		{
			NMib::NTime::CPerfTimeMeasureMin TimerInsert;
			NMib::NTime::CPerfTimeMeasureMin TimerDelete;
			for (aint i = 0; i < NumTests; ++i)
			{
				{
					DMibScopePerfTimeMeasureMin(TimerInsert);
					for (aint i = 0; i < mc_TestSorted; ++i)
					{
						m_TestTree.fr_Insert(m_ClassList[i]);
					}
				}
				{
					DMibScopePerfTimeMeasureMin(TimerDelete);
					for (aint i = 0; i < mc_TestSorted; ++i)
					{
						m_TestTree.fr_Remove(m_ClassList[i]);
					}
				}
			}

			DMibTrace("Performance for AVLTree fr_Insert = {0} inserts per second\n", fp64(mc_TestSorted) / TimerInsert.f_GetTime());
			DMibTrace("Performance for AVLTree fr_Remove = {0} removes per second\n", fp64(mc_TestSorted) / TimerDelete.f_GetTime());
		}


//		for (aint i = 1; i < mc_TestSorted; i+=2)
//		{
//			m_TestTree.f_Remove(m_ClassList[i]);
//			f_CheckTree();
//		}

		return "";

	}

};

class CTestTreeAligned : public CMalterlibTest
{
public:

	aint Rand (void)
	{
		static aint holdrand = 548;
		return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	class CCompare;
	class CTestClass
	{
	public:

		CTestClass()
		{
		}
		~CTestClass()
		{
		}
		NMib::NIntrusive::TCAVLLink<> m_Link;
		aint m_Data;
	};

	class CCompare
	{
	public:
		inline_small const aint &operator ()(CTestClass const &_Node) const
		{
			return _Node.m_Data;
		}
	};

	static const aint mc_TestSorted = TreeTestNum;

	CTestClass m_ClassList[mc_TestSorted];

	void RandomizeList()
	{
#ifdef TreeMixedList
		for (aint i = 0; i < mc_TestSorted; i+=2)
		{
			m_ClassList[i].m_Data = i/2;
		}
		for (aint i = 1; i < mc_TestSorted; i+=2)
		{
			m_ClassList[i].m_Data = i/2+mc_TestSorted/2;
		}
#else
		for (aint i = 0; i < mc_TestSorted; ++i)
		{
			m_ClassList[i].m_Data = i*2;
		}
#endif
	}
	NMib::NIntrusive::TCAVLTree<&CTestClass::m_Link, CCompare> m_TestTree;

	void TreeGetMaxDepth(CTestClass *_pObj, aint &_MaxDepth, aint &_CurrentDepth)
	{
		++_CurrentDepth;


		CTestClass *pRight = m_TestTree.fs_GetRight(_pObj);
		CTestClass *pLeft = m_TestTree.fs_GetLeft(_pObj);

		if (!pRight &&!pLeft)
		{
			if (_CurrentDepth > _MaxDepth)
				_MaxDepth = _CurrentDepth;
		}

		if (pLeft)
			TreeGetMaxDepth(pLeft, _MaxDepth, _CurrentDepth);
		if (pRight)
			TreeGetMaxDepth(pRight, _MaxDepth, _CurrentDepth);


		--_CurrentDepth;
	}

	void TreeGetMinDepth(CTestClass *_pObj, aint &_MinDepth, aint &_CurrentDepth)
	{
		++_CurrentDepth;

		CTestClass *pRight = m_TestTree.fs_GetRight(_pObj);
		CTestClass *pLeft = m_TestTree.fs_GetLeft(_pObj);

		if (!pRight &&!pLeft)
		{
			if (_CurrentDepth < _MinDepth)
				_MinDepth = _CurrentDepth;

		}

		if (pLeft)
			TreeGetMinDepth(pLeft, _MinDepth, _CurrentDepth);
		if (pRight)
			TreeGetMinDepth(pRight, _MinDepth, _CurrentDepth);

		--_CurrentDepth;
	}


	aint GetTreeLargest(CTestClass *_pObj)
	{
		aint CurrentLargest = _pObj->m_Data;

		CTestClass *pRight = m_TestTree.fs_GetRight(_pObj);
		CTestClass *pLeft = m_TestTree.fs_GetLeft(_pObj);

		if (pLeft)
		{
			aint Largest;
			if ((Largest = GetTreeLargest(pLeft)) > CurrentLargest)
				CurrentLargest = Largest;
		}

		if (pRight)
		{
			aint Largest;
			if ((Largest = GetTreeLargest(pRight)) > CurrentLargest)
				CurrentLargest = Largest;
		}

		return CurrentLargest;
	}

	aint GetTreeSmallest(CTestClass *_pObj)
	{
		aint CurrentSmallest = _pObj->m_Data;

		CTestClass *pRight = m_TestTree.fs_GetRight(_pObj);
		CTestClass *pLeft = m_TestTree.fs_GetLeft(_pObj);

		if (pLeft)
		{
			aint Smallest;
			if ((Smallest = GetTreeLargest(pLeft)) < CurrentSmallest)
				CurrentSmallest = Smallest;
		}

		if (pRight)
		{
			aint Smallest;
			if ((Smallest = GetTreeLargest(pRight)) < CurrentSmallest)
				CurrentSmallest = Smallest;
		}

		return CurrentSmallest;
	}

	void f_CheckTree(CTestClass *_pObj)
	{
		CTestClass *pRight = m_TestTree.fs_GetRight(_pObj);
		CTestClass *pLeft = m_TestTree.fs_GetLeft(_pObj);

		aint LeftMaxDepth = 0;
		aint RightMaxDepth = 0;

		if (pLeft)
		{
			aint CurrentDepth = 0;
			TreeGetMaxDepth(pRight, LeftMaxDepth, CurrentDepth);
			DMibSafeCheck(_pObj->m_Data > GetTreeLargest(pLeft),"Tree is not binary");
			f_CheckTree(pLeft);
		}
		if (pRight)
		{
			aint CurrentDepth = 0;
			TreeGetMaxDepth(pRight, RightMaxDepth, CurrentDepth);
			DMibSafeCheck(_pObj->m_Data < GetTreeSmallest(pRight),"Tree is not binary");
			f_CheckTree(pRight);
		}
		if (LeftMaxDepth > RightMaxDepth)
		{
			DMibSafeCheck((LeftMaxDepth - RightMaxDepth) <= 1, "Tree has lost its AVL Property");
		}
		else
		{
			DMibSafeCheck((RightMaxDepth - LeftMaxDepth) <= 1, "Tree has lost its AVL Property");
		}

	}

	void f_CheckTree()
	{
		m_TestTree.f_CheckTree(true);

		if (m_TestTree.f_GetRoot())
		{
			f_CheckTree(m_TestTree.f_GetRoot());
		}
	}

	void TraceTree(CTestClass *_pObj, aint _Depth, aint &_CurrentDepth, bool &_bTraced, bool _bLeft, aint _MaxDepth)
	{
		if (_Depth == _CurrentDepth)
		{
			_bTraced = true;

			if (_bLeft)
				DMibTrace("{L*1}", (NMib::NStr::CStr::CFormat("L{}") << (_pObj->m_Data)).f_GetStr(), 4);
			else
				DMibTrace("{L*1}", (NMib::NStr::CStr::CFormat("R{}") << (_pObj->m_Data)).f_GetStr(), 4);
		}
		++_CurrentDepth;

		CTestClass *pRight = m_TestTree.fs_GetRight(_pObj);
		CTestClass *pLeft = m_TestTree.fs_GetLeft(_pObj);

		if (pLeft)
		{
			TraceTree(pLeft, _Depth, _CurrentDepth, _bTraced, true, _MaxDepth);
		}

		if (pRight)
		{
			TraceTree(pRight, _Depth, _CurrentDepth, _bTraced, false, _MaxDepth);
		}
		--_CurrentDepth;
	}

	void TraceTree()
	{
		if (!m_TestTree.f_GetRoot())
			return;

		DMibTrace("\n", 0);
		auto Iter = m_TestTree.f_GetIterator();

		aint Current = 1;
		while (Iter)
		{
			DMibTrace("0x{nh,sf0,l8} ", ((mint)&Iter->m_Link));
			if (!(Current++ & 15))
				DMibTrace("\n", 0);
			++Iter;
		}
		DMibTrace("\n", 0);
	}

	NMib::NStr::CStr Certify(CTestInterface &_Interface)
	{
//		DMibTrace("\n\nAVLTreeAligned\n\n", 0);
		DMibTrace("sizeof(CTestClass) = {}\n", (sizeof(CTestClass)));

		RandomizeList();

		aint DebugTest = TreeDebugNum;

#ifdef DMibDebug
		aint NumTests = 1;
#else
		aint NumTests = TreeNumTests;
#endif

		// Ceck validity of tree code
		for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
		{
			m_TestTree.f_Insert(m_ClassList[i]);
			f_CheckTree();
		}

		f_CheckTree();

		TraceTree();
		auto Iter = m_TestTree.f_GetIterator();

		int Current = -1;

		while (Iter)
		{
			DMibSafeCheck(Iter->m_Data > Current, "Should come out in sorted order");
			Current = Iter->m_Data;
			++Iter;
		}

		++Current;
		Iter.f_StartBackward(m_TestTree);
		while (Iter)
		{
			DMibSafeCheck(Iter->m_Data < Current, "Should come out in sorted order");
			Current = Iter->m_Data;
			--Iter;
		}

		CTestClass *pTest;
		pTest = m_TestTree.f_FindLargest();
		DMibTrace("Largest {0}\n", (pTest->m_Data));
		pTest = m_TestTree.f_FindSmallest();
		DMibTrace("Smallest {0}\n", (pTest->m_Data));
		aint Key = 100;
		pTest = m_TestTree.f_FindEqual(Key);
		if (pTest)
			DMibTrace("Equal {} = {}\n", (Key), (pTest->m_Data));
		Key = 51;
		pTest = m_TestTree.f_FindLargestLessThanEqual(Key);
		if (pTest)
			DMibTrace("f_FindLargestLessThanEqual {} = {}\n", (Key), (pTest->m_Data));
		Key = 51;
		pTest = m_TestTree.f_FindSmallestGreaterThanEqual(Key);
		if (pTest)
			DMibTrace("f_FindSmallestGreaterThanEqual {} = {}\n", (Key), (pTest->m_Data));

		for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
		{
			m_TestTree.f_Remove(m_ClassList[i]);
		//	TraceTree();
			f_CheckTree();
		}
		f_CheckTree();
		DMibSafeCheck(!m_TestTree.f_GetRoot(), "Must be empty");

		for (aint j = 0; j < 10; ++j)
		{
			for (aint i = 0; i < mc_TestSorted*2; ++i)
			{
				aint Index = Rand()%mc_TestSorted;
				if (m_ClassList[Index].m_Link.f_IsInTree())
					m_TestTree.f_Remove(m_ClassList[Index]);
				else
					m_TestTree.f_Insert(m_ClassList[Index]);

			}

			for (aint u = 0; u < mc_TestSorted; ++u)
			{
				if (m_ClassList[u].m_Link.f_IsInTree())
				{
					CTestClass *pFind = m_TestTree.f_FindEqual(m_ClassList[u].m_Data);
					if (!pFind)
					{
						DMibSafeCheck(0, "Must find in tree");
					}
				}
				else
				{
					CTestClass *pFind = m_TestTree.f_FindEqual(m_ClassList[u].m_Data);
					if (pFind)
					{
						DMibSafeCheck(0, "Must not be in tree");
					}
				}
			}
			for (aint i = 0; i < mc_TestSorted; ++i)
			{
				if (m_ClassList[i].m_Link.f_IsInTree())
					m_TestTree.f_Remove(m_ClassList[i]);
			}
		}

		TraceTree();

		NMib::NTime::CPerfTimeMeasureMin TimerInsert;
		NMib::NTime::CPerfTimeMeasureMin TimerFindEqual;
		NMib::NTime::CPerfTimeMeasureMin TimerFindLargestLessThanEqual;
		NMib::NTime::CPerfTimeMeasureMin TimerFindSmallestGreaterThanEqual;
		NMib::NTime::CPerfTimeMeasureMin TimerDelete;
		NMib::NTime::CPerfTimeMeasureMin TimerInsertRecursive;
		NMib::NTime::CPerfTimeMeasureMin TimerDeleteRecursive;
		NMib::NTime::CPerfTimeMeasureMin TimerInsertLowStack;
		NMib::NTime::CPerfTimeMeasureMin TimerDeleteLowStack;
		aint FinalNumber = 0;
		for (aint i = 0; i < NumTests; ++i)
		{
			{
				DMibScopePerfTimeMeasureMin(TimerInsert);
				for (aint i = 0; i < mc_TestSorted; ++i)
				{
					m_TestTree.f_Insert(m_ClassList[i]);
				}
			}
			{
				DMibScopePerfTimeMeasureMin(TimerFindEqual);

				for (aint i = 0; i < mc_TestSorted; ++i)
				{
					aint Key = Rand() % (mc_TestSorted * 2);
					pTest = m_TestTree.f_FindEqual(Key);
				}

			}
			if (pTest)
				FinalNumber += pTest->m_Data;
			{
				DMibScopePerfTimeMeasureMin(TimerFindLargestLessThanEqual);
				for (aint i = 0; i < mc_TestSorted; ++i)
				{
					aint Key = Rand() % (mc_TestSorted * 2);
					pTest = m_TestTree.f_FindLargestLessThanEqual(Key);
				}
			}
			if (pTest)
				FinalNumber += pTest->m_Data;
			{
				DMibScopePerfTimeMeasureMin(TimerFindSmallestGreaterThanEqual);
				for (aint i = 0; i < mc_TestSorted; ++i)
				{
					aint Key = Rand() % (mc_TestSorted * 2);
					pTest = m_TestTree.f_FindSmallestGreaterThanEqual(Key);
				}
			}
			if (pTest)
				FinalNumber += pTest->m_Data;
			{
				DMibScopePerfTimeMeasureMin(TimerDelete);
				for (aint i = 0; i < mc_TestSorted; ++i)
				{
					m_TestTree.f_Remove(m_ClassList[i]);
				}
			}
			{
				DMibScopePerfTimeMeasureMin(TimerInsertRecursive);
				for (aint i = 0; i < mc_TestSorted; ++i)
				{
					m_TestTree.fr_Insert(m_ClassList[i]);
				}
			}
			{
				DMibScopePerfTimeMeasureMin(TimerDeleteRecursive);
				for (aint i = 0; i < mc_TestSorted; ++i)
				{
					m_TestTree.fr_Remove(m_ClassList[i]);
				}
			}
			{
				DMibScopePerfTimeMeasureMin(TimerInsertLowStack);
				for (aint i = 0; i < mc_TestSorted; ++i)
				{
					m_TestTree.f_InsertLowStack(m_ClassList[i]);
				}
			}
			{
				DMibScopePerfTimeMeasureMin(TimerDeleteLowStack);
				for (aint i = 0; i < mc_TestSorted; ++i)
				{
					m_TestTree.f_RemoveLowStack(m_ClassList[i]);
				}
			}
		}

		DMibTrace("Final Number {}\n", FinalNumber);

		DMibTrace("Performance for AVLTreeAligned f_Insert = {0} inserts per second\n", fp64(mc_TestSorted) / TimerInsert.f_GetTime());
		DMibTrace("Performance for AVLTreeAligned f_Remove = {0} removes per second\n", fp64(mc_TestSorted) / TimerDelete.f_GetTime());
		DMibTrace("Performance for AVLTreeAligned fr_Insert = {0} inserts per second\n", fp64(mc_TestSorted) / TimerInsertRecursive.f_GetTime());
		DMibTrace("Performance for AVLTreeAligned fr_Remove = {0} removes per second\n", fp64(mc_TestSorted) / TimerDeleteRecursive.f_GetTime());
		DMibTrace("Performance for AVLTreeAligned f_InsertLowStack = {0} inserts per second\n", fp64(mc_TestSorted) / TimerInsertLowStack.f_GetTime());
		DMibTrace("Performance for AVLTreeAligned f_RemoveLowStack = {0} removes per second\n", fp64(mc_TestSorted) / TimerDeleteLowStack.f_GetTime());
		DMibTrace("Performance for AVLTreeAligned f_FindEqual = {0} finds per second\n", fp64(mc_TestSorted) / TimerFindEqual.f_GetTime());
		DMibTrace("Performance for AVLTreeAligned f_FindLargestLessThanEqual = {0} finds  per second\n", fp64(mc_TestSorted) / TimerFindLargestLessThanEqual.f_GetTime());
		DMibTrace("Performance for AVLTreeAligned f_FindSmallestGreaterThanEqual = {0} finds per second\n", fp64(mc_TestSorted) / TimerFindSmallestGreaterThanEqual.f_GetTime());


//		for (aint i = 1; i < mc_TestSorted; i+=2)
//		{
//			m_TestTree.f_Remove(m_ClassList[i]);
//			f_CheckTree();
//		}

		return "";

	}

};

DMibRuntimeClass(CMalterlibTest, CTestTree);
DMibRuntimeClass(CMalterlibTest, CTestTreeAligned);
#endif
