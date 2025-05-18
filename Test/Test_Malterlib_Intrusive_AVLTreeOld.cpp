// Copyright © 2015 Hansoft AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

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
		typedef CCyclesMin CTestTimer;
		class CMalterlibNode
		{
		public:
			struct CCompare
			{
				inline_small int const &operator () (CMalterlibNode const &_Node) const
				{
					return _Node.m_Data;
				}
			};
			NIntrusive::TCAVLLink<NIntrusive::EAVLLinkType_AlignedRight> m_Link;
			int m_Data;
			CMalterlibNode() : m_Data(-1)
			{
			}

			CMalterlibNode(CMalterlibNode const& rhs) : m_Data(rhs.m_Data), m_Link()
			{
			}

			CMalterlibNode &operator= (CMalterlibNode const& rhs)
			{
				m_Data=rhs.m_Data;
				return *this;
			}
		};
		class CBoostNode : public boost::intrusive::avl_set_base_hook<boost::intrusive::optimize_size<true>, boost::intrusive::constant_time_size<0>>
		{
		public:
			class CCompare
			{
			public:
				COrdering_Partial operator () (CBoostNode const & _Left, int _Right) const
				{
					return _Left.m_Data <=> _Right;
				}
				COrdering_Partial operator () (int _Left, CBoostNode const & _Right) const
				{
					return _Left <=> _Right.m_Data;
				}
			};

			class CCompareLessThan
			{
			public:
				bool operator () (CBoostNode const & _Left, int _Right) const
				{
					return _Left.m_Data < _Right;
				}
				bool operator () (int _Left, CBoostNode const & _Right) const
				{
					return _Left < _Right.m_Data;
				}
			};

			int m_Data;
			CBoostNode() : m_Data(-1)
			{
			}
			CBoostNode(CMalterlibNode const& rhs) : m_Data(rhs.m_Data)
			{
			}
			CBoostNode& operator=(CBoostNode const& rhs)
			{
				m_Data=rhs.m_Data;
				return *this;
			}
			auto operator <=> (CBoostNode const &_Right) const
			{
				return m_Data <=> _Right.m_Data;
			}
			auto operator <=> (int const &_Right) const
			{
				return m_Data <=> _Right;
			}
		};

		typedef boost::intrusive::avl_set<CBoostNode, boost::intrusive::constant_time_size<0>> CBoostTree;
		typedef NIntrusive::TCAVLTree<&CMalterlibNode::m_Link, CMalterlibNode::CCompare> CMalterlibTree;

		template <bool t_bMemoryTests>
		class TCAVLTreeTester
		{
			TCVector<CMalterlibNode, NMemory::CAllocator_Virtual> m_DataMalterlib;
			TCVector<CBoostNode, NMemory::CAllocator_Virtual> m_DataBoost;
			mint m_nItems;
			mint m_nTests;
			fp64 m_AllowedDelta;

			typedef TCConditional<t_bMemoryTests, CTestMemoryMeasure, CTestPerformanceMeasure> CMeasureType;
			typedef TCConditional<t_bMemoryTests, CTestMemoryMaxAllocatedBytes, CTestPerformance> CTestType;
		public:
			TCAVLTreeTester(mint _nItems, bool _bRandom)
			{
				m_nItems = _nItems;
				m_nTests = fg_Max((100000u / m_nItems), 1u) * 5;
#ifdef DMibDebug
				m_nTests = 2;
#endif
				if (t_bMemoryTests)
					m_AllowedDelta = 1.0;
				else
					m_AllowedDelta = 0.9;

				m_DataMalterlib.f_SetLen(_nItems);
				m_DataBoost.f_SetLen(_nItems);
				if(!_bRandom)
				{
					for (mint i = 0; i < _nItems; ++i)
					{
						m_DataMalterlib[i].m_Data = i;
						m_DataBoost[i].m_Data = i;
					}
				}
				else
				{
					NMisc::CRandomShiftRNG RandomRng;
					CMalterlibTree MalterlibTree;
					for(mint i = 0; i < _nItems; ++i)
					{
						int x = 1 + (RandomRng.f_GetValue<int>() % (10000000-1));
						while (MalterlibTree.f_FindEqual(x))
							x = 1 + (RandomRng.f_GetValue<int>() % (10000000-1));
						m_DataMalterlib[i].m_Data = x;
						MalterlibTree.f_Insert(m_DataMalterlib[i]);
						m_DataBoost[i].m_Data = x;
					}
				}
			}

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

			void fp_TraceTree(CMalterlibNode *_pMalterlibNode, mint _Depth)
			{
				if (!_pMalterlibNode)
					return;
				fp_TraceTree(CMalterlibTree::fs_GetLeft(_pMalterlibNode), _Depth + 1);
				DMibTrace("{sj*}{}\r\n", "" << _Depth*3 << _pMalterlibNode->m_Data);
				fp_TraceTree(CMalterlibTree::fs_GetRight(_pMalterlibNode), _Depth + 1);
			}

			void fp_TraceTree(CBoostNode *_pBoostNode, mint _Depth)
			{
				if (!_pBoostNode)
					return;
				fp_TraceTree((CBoostNode *)_pBoostNode->left_, _Depth + 1);
				DMibTrace("{sj*}{}\r\n", "" << _Depth*3 << _pBoostNode->m_Data);
				fp_TraceTree((CBoostNode *)_pBoostNode->right_, _Depth + 1);
			}

			void f_InsertTests()
			{

				CMeasureType MalterlibTime("Malterlib");
				CMeasureType BoostTime("Boost");
				CMalterlibTree MalterlibTree;
				for (mint i = 0; i < m_nTests; ++i)
				{
					MalterlibTree.f_Clear();
					auto Iter = m_DataMalterlib.f_GetIterator();
					{
						DMibTestScopeMeasure(MalterlibTime, m_nItems);
						for(;Iter; ++Iter)
						{
							MalterlibTree.f_Insert(*Iter);
						}
					}
				}
				CBoostTree BoostTree;
				for (mint i = 0; i < m_nTests; ++i)
				{
					BoostTree.clear();
					auto Iter = m_DataBoost.f_GetIterator();

					{
						DMibTestScopeMeasure(BoostTime, m_nItems);
						for(;Iter; ++Iter)
						{
							BoostTree.insert(*Iter);
						}
					}
				}

				CBoostNode *pRoot = (CBoostNode *)(((mint)BoostTree.holder.root.parent_) & ~mint(3));
				while ((void *)((mint)pRoot->parent_ & ~mint(3)) != (void *)&BoostTree)
					pRoot = (CBoostNode *)((mint)pRoot->parent_ & ~mint(3));
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
					typedef CMalterlibTree::TCIterator<> Iter1;
					Iter1 it1 = MalterlibTree;
					auto it2 = BoostTree.begin();
					auto end2 = BoostTree.end();
					bool bTreesSameValues = MalterlibTree.f_GetLen() == BoostTree.size();
					for (;it1 && it2!=end2;++it1,++it2)
					{
						auto dummy1 = it1->m_Data;
						auto dummy2 = it2->m_Data;
						bTreesSameValues = bTreesSame && dummy1 == dummy2;
					}
					DMibTest(DMibExpr(bTreesSameValues));
				}

				bool bMalterlibTreeValid = MalterlibTree.f_CheckTree(false);
				if (!bMalterlibTreeValid)
					DMibTest(DMibExpr(bMalterlibTreeValid));
				CTestType Performing(m_AllowedDelta);
				Performing.f_AddReference(BoostTime);
				Performing.f_Add(MalterlibTime);
				DMibTest(DMibExpr(Performing));
			}
			void f_SearchTests()
			{

				CMeasureType MalterlibTime("Malterlib");
				CMeasureType BoostTime("Boost");
				mint bFoundMalterlib = 0;
				mint bFoundBoost = 0;
				for (mint i = 0; i < m_nTests; ++i)
				{
					CMalterlibTree MalterlibTree;
					auto Iter = m_DataMalterlib.f_GetIterator();
					for (;Iter; ++Iter)
					{
						MalterlibTree.f_Insert(*Iter);
					}
					Iter = m_DataMalterlib.f_GetIterator();
					CMalterlibNode::CCompare Compare;
					{
						DMibTestScopeMeasure(MalterlibTime, m_nItems);
						for (;Iter; ++Iter)
						{
							bFoundMalterlib += MalterlibTree.f_FindEqual(Iter->m_Data, Compare) != nullptr;
						}
					}
				}
				for (mint i = 0; i < m_nTests; ++i)
				{
					CBoostTree BoostTree;
					auto Iter = m_DataBoost.f_GetIterator();
					for(;Iter; ++Iter)
					{
						BoostTree.insert(*Iter);
					}
					Iter = m_DataBoost.f_GetIterator();
					auto TreeEnd = BoostTree.end();
					CBoostNode::CCompareLessThan Compare;
					{
						DMibTestScopeMeasure(BoostTime, m_nItems);
						for (;Iter; ++Iter)
						{
							bFoundBoost += BoostTree.find(Iter->m_Data, Compare) != TreeEnd;
						}
					}
				}

				if (bFoundMalterlib != bFoundBoost)
					DMibTest(DMibExpr(bFoundMalterlib) == DMibExpr(bFoundBoost));
				CTestType Performing(m_AllowedDelta);
				Performing.f_AddReference(BoostTime);
				Performing.f_Add(MalterlibTime);
				DMibTest(DMibExpr(Performing));
			}
			void f_RemoveTests()
			{
				CMeasureType MalterlibTime("Malterlib");
				CMeasureType BoostTime("Boost");
				for (mint i = 0; i < m_nTests; ++i)
				{
					CMalterlibTree MalterlibTree;
					for (mint i = 0 ; i < m_nItems; ++i)
					{
						MalterlibTree.f_Insert(m_DataMalterlib[i]);
					}
					auto Iter = m_DataMalterlib.f_GetIterator();
					{
						DMibTestScopeMeasure(MalterlibTime, m_nItems);
						for (;Iter; ++Iter)
						{
							MalterlibTree.f_Remove(*Iter);
						}
					}
				}

				for (mint i = 0; i < m_nTests; ++i)
				{
					CBoostTree BoostTree;
					for (mint i = 0; i < m_nItems; ++i)
					{
						BoostTree.insert(m_DataBoost[i]);
					}
					auto Iter = m_DataBoost.f_GetIterator();
					{
						DMibTestScopeMeasure(BoostTime, m_nItems);
						for (;Iter; ++Iter)
						{
							BoostTree.erase(*Iter);
						}
					}
				}

				CTestType Performing(m_AllowedDelta);
				Performing.f_AddReference(BoostTime);
				Performing.f_Add(MalterlibTime);
				DMibTest(DMibExpr(Performing));
			}
			void f_TraverseTests()
			{
				CMalterlibTree MalterlibTree;
				CBoostTree BoostTree;
				typedef CBoostTree::const_iterator Iter2;
				typedef CMalterlibTree::TCIterator<> Iter1;
				for(mint i = 0; i < m_nItems; ++i)
				{
					MalterlibTree.f_Insert(&m_DataMalterlib[i]);
				}
				for(mint i = 0; i < m_nItems; ++i)
				{
					BoostTree.insert(m_DataBoost[i]);
				}

				int dummy1 = 0;
				int dummy2 = 0;
				CMeasureType MalterlibTime("Malterlib");
				CMeasureType BoostTime("Boost");

				for (mint i = 0; i < m_nTests; ++i)
				{
					Iter1 it1 = MalterlibTree;
					{
						DMibTestScopeMeasure(MalterlibTime, m_nItems);
						for (;it1;++it1)
						{
							dummy1 += it1->m_Data;
						}
					}
				}
				for (mint i = 0; i < m_nTests; ++i)
				{
					Iter2 it2 = BoostTree.begin();
					Iter2 end2 = BoostTree.end();
					{
						DMibTestScopeMeasure(BoostTime, m_nItems);
						for (;it2!=end2;++it2)
						{
							dummy2 += it2->m_Data;
						}
					}
				}

				if (dummy1 != dummy2)
					DMibTest(DMibExpr(dummy1)==DMibExpr(dummy2));
				CTestType Performing(m_AllowedDelta);
				Performing.f_AddReference(BoostTime);
				Performing.f_Add(MalterlibTime);
				DMibTest(DMibExpr(Performing));
			}
		};
		class CAVL_Tests : public CTest
		{

		public: // Define test suites
			void f_Suite(bool _bRandom, bool _bMemoryTests)
			{
#ifdef DMibDebug
				mint End = 16*1024;
#else
				mint End = 512*1024;
#endif
				DMibTestCategory("Insert")
				{
					for (mint i = 1; i <= End; i <<= 1)
					{
						DMibTestSuite(NStr::CStr::CFormat("Iterations {}") << i)
						{
							if (_bMemoryTests)
								TCAVLTreeTester<true>(i,_bRandom).f_InsertTests();
							else
								TCAVLTreeTester<false>(i,_bRandom).f_InsertTests();
						};
					}
				};
				DMibTestCategory("Search")
				{
					for (mint i = 4; i <= End; i <<= 1)
					{
						DMibTestSuite(NStr::CStr::CFormat("Iterations {}") << i)
						{
							if (_bMemoryTests)
								TCAVLTreeTester<true>(i,_bRandom).f_SearchTests();
							else
								TCAVLTreeTester<false>(i,_bRandom).f_SearchTests();
						};
					}
				};
				DMibTestCategory("Remove")
				{
					for (mint i = 4; i <= End; i <<= 1)
					{
						DMibTestSuite(NStr::CStr::CFormat("Iterations {}") << i)
						{
							if (_bMemoryTests)
								TCAVLTreeTester<true>(i,_bRandom).f_RemoveTests();
							else
								TCAVLTreeTester<false>(i,_bRandom).f_RemoveTests();
						};
					}
				};
				DMibTestCategory("Traverse")
				{
					for (mint i = 4; i <= (_bRandom? End << 1 : End << 4); i <<= 1)
					{
						DMibTestSuite(NStr::CStr::CFormat("Iterations {}") << i)
						{
							if (_bMemoryTests)
								TCAVLTreeTester<true>(i,_bRandom).f_TraverseTests();
							else
								TCAVLTreeTester<false>(i,_bRandom).f_TraverseTests();
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
						DMibTest(DMibExpr(sizeof(MalterlibTree))<=DMibExpr(sizeof(BoostTree)));
						DMibTest(DMibExpr(sizeof(CMalterlibNode))<=DMibExpr(sizeof(CBoostNode)));
					};
				};
			}
		};
		DMibTestRegister(CAVL_Tests, Malterlib::Intrusive);
	}
}

