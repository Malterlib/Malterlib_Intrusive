// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include <Mib/Test/Test>
#include <Mib/Intrusive/AVLTree>

namespace
{
	using namespace NMib::NIntrusive;
	using namespace NMib::NStr;
 
	class CAVLTree_Tests : public NMib::NTest::CTest
	{
	public:

		template <typename t_CLinkType, typename t_CData, t_CLinkType (t_CData::*t_pMemberPointer)>
		struct TCMemberPointerHolder
		{
		};

		template <typename t_CClass>
		struct TCDefaultAccessor
		{
			static t_CClass &fs_Get();
			typedef decltype(fs_Get().m_Link) CLinkContainer;
			static CLinkContainer &f_GetOffset(t_CClass &_Class)
			{
				return _Class.m_Link;
			}
		};
		struct CData
		{
			CStr m_Data;
			bool m_Valid;
			CData() : m_Data(), m_Valid(false) {}
			~CData() { m_Valid = false; }
			CData(CStr const& _Data) : m_Data(_Data), m_Valid(true) {}
			CData(CData const &_Other) : m_Data(_Other.m_Data), m_Valid(_Other.m_Valid) {}
		public:
			CData(CData &&_Other) : m_Data(_Other.m_Data), m_Valid(_Other.m_Valid) 
			{
				_Other.m_Valid = false;
			}

			CData& operator = (CData &&_Other)
			{
				m_Data = _Other.m_Data;
				m_Valid = _Other.m_Valid;
				_Other.m_Valid = false;
				return *this;
			}
		};
		struct CTestClass
		{
			CData m_Data;
			CTestClass(CStr const& _Data)
				: m_Data(_Data)
			{
			}
			CTestClass(CData const& _Data)
				: m_Data(_Data)
			{
			}

			class CCompare
			{
				bool m_Valid;
			public:
				CCompare() : m_Valid(true) {}
				CCompare(CCompare &&_Other) : m_Valid(true) 
				{ 
					_Other.m_Valid = false; 
				}
				~CCompare() { m_Valid = false; }
				CCompare& operator = (CCompare &&_Other) 
				{ 
					m_Valid = _Other.m_Valid;
					_Other.m_Valid = false;
					return *this; 
				}
				CData const &operator () (CTestClass const &_Node) const
				{
					return _Node.m_Data;
				}
				inline_small bint operator () (CData const &_Left, CData const &_Right) const
				{
					DMibTest(DMibExpr(_Left.m_Valid)) (ETestFlag_Aggregated);
					DMibTest(DMibExpr(_Right.m_Valid)) (ETestFlag_Aggregated);

					return fg_StrCmpNoCase(_Left.m_Data, _Right.m_Data) < 0;
					//return _Left.m_Data > _Right.m_Data;
				}
			};

			TCAVLLinkAutoRemove<> m_Link;

			TCAVLTree<&CTestClass::m_Link, CCompare> m_Tree;
		};


		void f_DoTests() override
		{
			
			DMibTestSuite("AVL")
			{

				typedef CTestClass CNode;
				typedef CData CMint;
				CTestClass Test("4");

				CTestClass Test1("1");
				CTestClass Test2("8");
				CTestClass Test3("3");
				CTestClass Test4("7");

				auto Insert = [&] (CNode &_Test, CStr const &_Path)
				{
					DMibTestPath(_Path);
					DMibTest(DMibExpr(!Test.m_Tree.f_FindEqual((_Test.m_Data))));
					Test.m_Tree.f_Insert(_Test);
					DMibTest(DMibExpr(!!Test.m_Tree.f_FindEqual((_Test.m_Data))));
				};
				auto Insert2 = [&] (CNode &&_Test, CStr const &_Path)
				{
					DMibTestPath(_Path);
					DMibTest(DMibExpr(!Test.m_Tree.f_FindEqual((_Test.m_Data))));
					Test.m_Tree.f_Insert(_Test);
					DMibTest(DMibExpr(!!Test.m_Tree.f_FindEqual((_Test.m_Data))));
				};
				Insert(Test1, "Test1");
				Insert(Test2, "Test2");
				Insert(Test3, "Test3");
				Insert(Test4, "Test4");
				Insert2(CNode("5"), "CNode5");
				Insert2(CNode("9"), "CNode9");
				Insert2(CNode("0"), "CNode0");
				/*
				Test.m_Tree.f_Insert(Test1);
				Test.m_Tree.f_Insert(&Test2);
				Test.m_Tree.f_Insert(Test3);
				Test.m_Tree.f_Insert(Test4);
				Test.m_Tree.f_Insert(CNode("5"));
				Test.m_Tree.f_Insert(CNode("9"));
				Test.m_Tree.f_Insert(CNode("0"));
				*/

				Test.m_Tree.f_FindEqual(CMint("3"));

				DMibTest(DMibExpr(Test.m_Data.m_Valid));
				DMibTest(DMibExpr(Test1.m_Data.m_Valid));
				DMibTest(DMibExpr(Test2.m_Data.m_Valid));
				DMibTest(DMibExpr(Test3.m_Data.m_Valid));
				DMibTest(DMibExpr(Test4.m_Data.m_Valid));

				typedef TCAVLTree<&CNode::m_Link, CNode::CCompare>::CIterator CIter;
				for(CIter It = Test.m_Tree;It;++It)
				{
					DMibTest(DMibExpr(It->m_Data.m_Valid)) (ETestFlag_Aggregated);
					CStr data = It->m_Data.m_Data;
				}


	/*			Test.m_Tree.f_Remove(Test1);
				Test.m_Tree.f_Remove(Test2);
				Test.m_Tree.f_Remove(Test3);
				Test.m_Tree.f_Remove(Test4);*/
			};
			
		}
	};


	[[maybe_unused]] TCAVLTreeAggregate<&CAVLTree_Tests::CTestClass::m_Link, CAVLTree_Tests::CTestClass::CCompare> g_Tree = { DAggregateInit };

	
	DMibTestRegister(CAVLTree_Tests, Malterlib::Intrusive);
}

struct CTest0
{
	mint m_Data;
private:
	void f_Test()
	{
	}
};

CTest0 g_Test = {0};

#if 0
template <typename t_CLinkType, typename t_CData, t_CLinkType (t_CData::*t_pMemberPointer)>
struct TCMemberPointerHolder
{
};

template <int t_Offset>
class TCStaticOffset
{
public:
	static int GetOffset()
	{
		return t_Offset;
	}
};

struct CTestClass
{
	int m_Offf;
	TCMemberPointerHolder<int, CTestClass, &CTestClass::m_Offf> m_Member;
	TCStaticOffset<offsetof(CTestClass, m_Offf)> m_StaticOffset;
};
#endif
