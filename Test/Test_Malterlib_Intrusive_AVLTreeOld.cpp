// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Mib/Core/Core>
#include <Mib/Test/Test>
#include <Mib/Test/Memory>
#include <Mib/Test/Performance>

#include <vector>
#include <deque>
#include <boost/intrusive/avl_set.hpp>
using namespace NMib::NTime;
using namespace NMib::NContainer;
using namespace NMib::NTest;

namespace NMib
{
	namespace NIntrusive
	{
		using CTestTimer = CPrefCyclesTimeMeasureMin;

		struct CMalterlibNode
		{
			struct CCompare
			{
				inline_small int const &operator () (CMalterlibNode const &_Node) const
				{
					return _Node.m_Data;
				}
			};

			NIntrusive::TCAVLLink<NIntrusive::EAVLLinkType_AlignedRight> m_Link;
			int m_Data;
			CMalterlibNode()
				: m_Data(-1)
			{
			}

			CMalterlibNode(CMalterlibNode const &_Other)
				: m_Data(_Other.m_Data)
				, m_Link()
			{
			}

			CMalterlibNode &operator = (CMalterlibNode const &_Other)
			{
				m_Data = _Other.m_Data;
				return *this;
			}
		};

		struct CBoostNode : public boost::intrusive::avl_set_base_hook<boost::intrusive::optimize_size<true>, boost::intrusive::constant_time_size<0>>
		{
			struct CCompare
			{
				COrdering_Partial operator () (CBoostNode const &_Left, int _Right) const
				{
					return _Left.m_Data <=> _Right;
				}

				COrdering_Partial operator () (int _Left, CBoostNode const &_Right) const
				{
					return _Left <=> _Right.m_Data;
				}
			};

			struct CCompareLessThan
			{
				bool operator () (CBoostNode const &_Left, int _Right) const
				{
					return _Left.m_Data < _Right;
				}

				bool operator () (int _Left, CBoostNode const &_Right) const
				{
					return _Left < _Right.m_Data;
				}
			};

			int m_Data;
			CBoostNode()
				: m_Data(-1)
			{
			}

			CBoostNode(CMalterlibNode const &_Other)
				: m_Data(_Other.m_Data)
			{
			}

			CBoostNode &operator = (CBoostNode const &_Other)
			{
				m_Data = _Other.m_Data;
				return *this;
			}

			auto operator <=> (CBoostNode const &_Right) const noexcept
			{
				return m_Data <=> _Right.m_Data;
			}

			auto operator <=> (int const &_Right) const noexcept
			{
				return m_Data <=> _Right;
			}
		};

		using CBoostTree = boost::intrusive::avl_set<CBoostNode, boost::intrusive::constant_time_size<0>>;
		using CMalterlibTree = NIntrusive::TCAVLTree<&CMalterlibNode::m_Link, CMalterlibNode::CCompare>;

		template <bool t_bMemoryTests>
		struct TCAVLTreeTester
		{
			TCAVLTreeTester(umint _nItems, bool _bRandom)
			{
				mp_nItems = _nItems;
				mp_nTests = fg_Max((100000u / mp_nItems), 1u) * 5;
#ifdef DMibDebug
				mp_nTests = 2;
#endif
				if (t_bMemoryTests)
					mp_AllowedDelta = 1.0;
				else
					mp_AllowedDelta = 0.9;

				mp_DataMalterlib.f_SetLen(_nItems);
				mp_DataBoost.f_SetLen(_nItems);
				if (!_bRandom)
				{
					for (umint iItem = 0; iItem < _nItems; ++iItem)
					{
						mp_DataMalterlib[iItem].m_Data = iItem;
						mp_DataBoost[iItem].m_Data = iItem;
					}
				}
				else
				{
					NMisc::CRandomShiftRNG RandomRng;
					CMalterlibTree MalterlibTree;
					for (umint iItem = 0; iItem < _nItems; ++iItem)
					{
						int Value = 1 + (RandomRng.f_GetValue<int>() % (10000000 - 1));
						while (MalterlibTree.f_FindEqual(Value))
							Value = 1 + (RandomRng.f_GetValue<int>() % (10000000 - 1));
						mp_DataMalterlib[iItem].m_Data = Value;
						MalterlibTree.f_Insert(mp_DataMalterlib[iItem]);
						mp_DataBoost[iItem].m_Data = Value;
					}
				}
			}

			void f_InsertTests()
			{
				CMeasureType MalterlibTime("Malterlib");
				CMeasureType BoostTime("Boost");
				CMalterlibTree MalterlibTree;
				for (umint iTest = 0; iTest < mp_nTests; ++iTest)
				{
					MalterlibTree.f_Clear();
					auto iData = mp_DataMalterlib.f_GetIterator();
					{
						DMibTestScopeMeasure(MalterlibTime, mp_nItems);
						for (; iData; ++iData)
							MalterlibTree.f_Insert(*iData);
					}
				}
				CBoostTree BoostTree;
				for (umint iTest = 0; iTest < mp_nTests; ++iTest)
				{
					BoostTree.clear();
					auto iData = mp_DataBoost.f_GetIterator();

					{
						DMibTestScopeMeasure(BoostTime, mp_nItems);
						for (; iData; ++iData)
							BoostTree.insert(*iData);
					}
				}

				CBoostNode *pRoot = (CBoostNode *)(((umint)BoostTree.holder.root.parent_) & ~umint(3));
				while ((void *)((umint)pRoot->parent_ & ~umint(3)) != (void *)&BoostTree)
					pRoot = (CBoostNode *)((umint)pRoot->parent_ & ~umint(3));
				bool bTreesSame = fp_CheckTree(MalterlibTree.f_GetRoot(), pRoot);
				bTreesSame = bTreesSame && fp_CheckTree(MalterlibTree.f_GetRoot(), pRoot);

				if (!bTreesSame)
				{
					//fp_TraceTree(MalterlibTree.f_GetRoot(), 0);
					//DMibTrace("\r\n", 0);
					//fp_TraceTree(pRoot, 0);
				}
				DMibTest(DMibExpr(bTreesSame));
				{
					using CIter1 = CMalterlibTree::TCIterator<>;
					CIter1 iMalterlib = MalterlibTree;
					auto iBoost = BoostTree.begin();
					auto iBoostEnd = BoostTree.end();
					bool bTreesSameValues = MalterlibTree.f_GetLen() == BoostTree.size();
					for (; iMalterlib && iBoost != iBoostEnd; ++iMalterlib, ++iBoost)
					{
						auto MalterlibValue = iMalterlib->m_Data;
						auto BoostValue = iBoost->m_Data;
						bTreesSameValues = bTreesSame && MalterlibValue == BoostValue;
					}
					DMibTest(DMibExpr(bTreesSameValues));
				}

				bool bMalterlibTreeValid = MalterlibTree.f_CheckTree(false);
				if (!bMalterlibTreeValid)
					DMibTest(DMibExpr(bMalterlibTreeValid));
				CTestType Performing(mp_AllowedDelta);
				Performing.f_AddReference(BoostTime);
				Performing.f_Add(MalterlibTime);
				DMibTest(DMibExpr(Performing));
			}

			void f_SearchTests()
			{
				CMeasureType MalterlibTime("Malterlib");
				CMeasureType BoostTime("Boost");
				umint bFoundMalterlib = 0;
				umint bFoundBoost = 0;
				for (umint iTest = 0; iTest < mp_nTests; ++iTest)
				{
					CMalterlibTree MalterlibTree;
					auto iData = mp_DataMalterlib.f_GetIterator();
					for (; iData; ++iData)
						MalterlibTree.f_Insert(*iData);
					iData = mp_DataMalterlib.f_GetIterator();
					CMalterlibNode::CCompare Compare;
					{
						DMibTestScopeMeasure(MalterlibTime, mp_nItems);
						for (; iData; ++iData)
							bFoundMalterlib += MalterlibTree.f_FindEqual(iData->m_Data, Compare) != nullptr;
					}
				}
				for (umint iTest = 0; iTest < mp_nTests; ++iTest)
				{
					CBoostTree BoostTree;
					auto iData = mp_DataBoost.f_GetIterator();
					for (; iData; ++iData)
						BoostTree.insert(*iData);
					iData = mp_DataBoost.f_GetIterator();
					auto iTreeEnd = BoostTree.end();
					CBoostNode::CCompareLessThan Compare;
					{
						DMibTestScopeMeasure(BoostTime, mp_nItems);
						for (; iData; ++iData)
							bFoundBoost += BoostTree.find(iData->m_Data, Compare) != iTreeEnd;
					}
				}

				if (bFoundMalterlib != bFoundBoost)
					DMibTest(DMibExpr(bFoundMalterlib) == DMibExpr(bFoundBoost));
				CTestType Performing(mp_AllowedDelta);
				Performing.f_AddReference(BoostTime);
				Performing.f_Add(MalterlibTime);
				DMibTest(DMibExpr(Performing));
			}

			void f_RemoveTests()
			{
				CMeasureType MalterlibTime("Malterlib");
				CMeasureType BoostTime("Boost");
				for (umint iTest = 0; iTest < mp_nTests; ++iTest)
				{
					CMalterlibTree MalterlibTree;
					for (umint iItem = 0; iItem < mp_nItems; ++iItem)
						MalterlibTree.f_Insert(mp_DataMalterlib[iItem]);
					auto iData = mp_DataMalterlib.f_GetIterator();
					{
						DMibTestScopeMeasure(MalterlibTime, mp_nItems);
						for (; iData; ++iData)
							MalterlibTree.f_Remove(*iData);
					}
				}

				for (umint iTest = 0; iTest < mp_nTests; ++iTest)
				{
					CBoostTree BoostTree;
					for (umint iItem = 0; iItem < mp_nItems; ++iItem)
						BoostTree.insert(mp_DataBoost[iItem]);
					auto iData = mp_DataBoost.f_GetIterator();
					{
						DMibTestScopeMeasure(BoostTime, mp_nItems);
						for (; iData; ++iData)
							BoostTree.erase(*iData);
					}
				}

				CTestType Performing(mp_AllowedDelta);
				Performing.f_AddReference(BoostTime);
				Performing.f_Add(MalterlibTime);
				DMibTest(DMibExpr(Performing));
			}

			void f_TraverseTests()
			{
				CMalterlibTree MalterlibTree;
				CBoostTree BoostTree;
				using CIter2 = CBoostTree::const_iterator;
				using CIter1 = CMalterlibTree::TCIterator<>;
				for (umint iItem = 0; iItem < mp_nItems; ++iItem)
					MalterlibTree.f_Insert(&mp_DataMalterlib[iItem]);
				for (umint iItem = 0; iItem < mp_nItems; ++iItem)
					BoostTree.insert(mp_DataBoost[iItem]);

				int MalterlibSum = 0;
				int BoostSum = 0;
				CMeasureType MalterlibTime("Malterlib");
				CMeasureType BoostTime("Boost");

				for (umint iTest = 0; iTest < mp_nTests; ++iTest)
				{
					CIter1 iMalterlib = MalterlibTree;
					{
						DMibTestScopeMeasure(MalterlibTime, mp_nItems);
						for (; iMalterlib; ++iMalterlib)
							MalterlibSum += iMalterlib->m_Data;
					}
				}
				for (umint iTest = 0; iTest < mp_nTests; ++iTest)
				{
					CIter2 iBoost = BoostTree.begin();
					CIter2 iBoostEnd = BoostTree.end();
					{
						DMibTestScopeMeasure(BoostTime, mp_nItems);
						for (; iBoost != iBoostEnd; ++iBoost)
							BoostSum += iBoost->m_Data;
					}
				}

				if (MalterlibSum != BoostSum)
					DMibTest(DMibExpr(MalterlibSum) == DMibExpr(BoostSum));
				CTestType Performing(mp_AllowedDelta);
				Performing.f_AddReference(BoostTime);
				Performing.f_Add(MalterlibTime);
				DMibTest(DMibExpr(Performing));
			}

		private:
			bool fp_CheckTree(CMalterlibNode *_pMalterlibNode, CBoostNode *_pBoostNode)
			{
				if (_pMalterlibNode && !_pBoostNode)
					return false;
				if (!_pMalterlibNode && _pBoostNode)
					return false;
				if (!_pMalterlibNode && !_pBoostNode)
					return true;
				if (_pMalterlibNode->m_Data != _pBoostNode->m_Data)
					return false;

				if (!fp_CheckTree(CMalterlibTree::fs_GetLeft(_pMalterlibNode), (CBoostNode *)_pBoostNode->left_))
					return false;

				if (!fp_CheckTree(CMalterlibTree::fs_GetRight(_pMalterlibNode), (CBoostNode *)_pBoostNode->right_))
					return false;
				return true;
			}

			void fp_TraceTree(CMalterlibNode *_pMalterlibNode, umint _Depth)
			{
				if (!_pMalterlibNode)
					return;
				fp_TraceTree(CMalterlibTree::fs_GetLeft(_pMalterlibNode), _Depth + 1);
				DMibTrace("{sj*}{}\r\n", "", _Depth * 3, _pMalterlibNode->m_Data);
				fp_TraceTree(CMalterlibTree::fs_GetRight(_pMalterlibNode), _Depth + 1);
			}

			void fp_TraceTree(CBoostNode *_pBoostNode, umint _Depth)
			{
				if (!_pBoostNode)
					return;
				fp_TraceTree((CBoostNode *)_pBoostNode->left_, _Depth + 1);
				DMibTrace("{sj*}{}\r\n", "", _Depth * 3, _pBoostNode->m_Data);
				fp_TraceTree((CBoostNode *)_pBoostNode->right_, _Depth + 1);
			}

			TCVector<CMalterlibNode, NMemory::CAllocator_Virtual> mp_DataMalterlib;
			TCVector<CBoostNode, NMemory::CAllocator_Virtual> mp_DataBoost;
			umint mp_nItems;
			umint mp_nTests;
			fp64 mp_AllowedDelta;

			using CMeasureType = TCConditional<t_bMemoryTests, CTestMemoryMeasure, CTestPerformanceMeasure>;
			using CTestType = TCConditional<t_bMemoryTests, CTestMemoryMaxAllocatedBytes, CTestPerformance>;
		};

		struct CAVL_Tests : public CTest
		{
			void f_Suite(bool _bRandom, bool _bMemoryTests)
			{
#ifdef DMibDebug
				umint End = 16 * 1024;
#else
				umint End = 512 * 1024;
#endif
				DMibTestCategory("Insert")
				{
					for (umint nItems = 1; nItems <= End; nItems <<= 1)
					{
						DMibTestSuite(NStr::CStr::CFormat("Iterations {}") << nItems)
						{
							if (_bMemoryTests)
								TCAVLTreeTester<true>(nItems, _bRandom).f_InsertTests();
							else
								TCAVLTreeTester<false>(nItems, _bRandom).f_InsertTests();
						};
					}
				};
				DMibTestCategory("Search")
				{
					for (umint nItems = 4; nItems <= End; nItems <<= 1)
					{
						DMibTestSuite(NStr::CStr::CFormat("Iterations {}") << nItems)
						{
							if (_bMemoryTests)
								TCAVLTreeTester<true>(nItems, _bRandom).f_SearchTests();
							else
								TCAVLTreeTester<false>(nItems, _bRandom).f_SearchTests();
						};
					}
				};
				DMibTestCategory("Remove")
				{
					for (umint nItems = 4; nItems <= End; nItems <<= 1)
					{
						DMibTestSuite(NStr::CStr::CFormat("Iterations {}") << nItems)
						{
							if (_bMemoryTests)
								TCAVLTreeTester<true>(nItems, _bRandom).f_RemoveTests();
							else
								TCAVLTreeTester<false>(nItems, _bRandom).f_RemoveTests();
						};
					}
				};
				DMibTestCategory("Traverse")
				{
					for (umint nItems = 4; nItems <= (_bRandom ? End << 1 : End << 4); nItems <<= 1)
					{
						DMibTestSuite(NStr::CStr::CFormat("Iterations {}") << nItems)
						{
							if (_bMemoryTests)
								TCAVLTreeTester<true>(nItems, _bRandom).f_TraverseTests();
							else
								TCAVLTreeTester<false>(nItems, _bRandom).f_TraverseTests();
						};
					}
				};
			}

			void f_DoTests()
			{
				// Performance differences stem from boost having a parent pointer saved
				DMibTestCategory(CTestCategory("Performace") << CTestGroup("Performance"))
				{
					DMibTestCategory("Random Input")
					{
						f_Suite(true, false);
					};
					DMibTestCategory("Linear input")
					{
						f_Suite(false, false);
					};
				};
				DMibTestCategory(CTestCategory("Memory") << CTestGroup("Memory"))
				{
					CMalterlibTree MalterlibTree;
					CBoostTree BoostTree;
					DMibTestSuite("Size")
					{
						DMibTest(DMibExpr(sizeof(MalterlibTree)) <= DMibExpr(sizeof(BoostTree)));
						DMibTest(DMibExpr(sizeof(CMalterlibNode)) <= DMibExpr(sizeof(CBoostNode)));
					};
				};
			}
		};
		DMibTestRegister(CAVL_Tests, Malterlib::Intrusive);
	}
}

