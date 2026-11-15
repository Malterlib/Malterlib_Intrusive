// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Mib/Test/Test>
#include <Mib/Intrusive/AVLTree>

namespace
{
	using namespace NMib::NIntrusive;
	using namespace NMib::NStr;

	struct CAVLTree_Tests : public NMib::NTest::CTest
	{
		template <typename t_CLinkType, typename t_CData, t_CLinkType (t_CData:: *t_pMemberPointer)>
		struct TCMemberPointerHolder
		{
		};

		template <typename t_CClass>
		struct TCDefaultAccessor
		{
			using CLinkContainer = decltype(NMib::fg_GetType<t_CClass &>().m_Link);

			static CLinkContainer &fs_GetOffset(t_CClass &_Class);
		};

		struct CData
		{
			CData();
			~CData();
			CData(CStr const &_Data);
			CData(CData const &_Other);
			CData(CData &&_Other);

			CData &operator = (CData &&_Other);

			CStr m_Data;
			bool m_bValid;
		};

		struct CTestClass
		{
			struct CCompare
			{
				CCompare();
				CCompare(CCompare &&_Other);
				~CCompare();

				CCompare &operator = (CCompare &&_Other);
				CData const &operator () (CTestClass const &_Node) const;
				inline_small NMib::COrdering_Partial operator () (CData const &_Left, CData const &_Right) const;

			private:
				bool mp_bValid;
			};

			CTestClass(CStr const &_Data);
			CTestClass(CData const &_Data);

			CData m_Data;

			TCAVLLinkAutoRemove<> m_Link;

			TCAVLTree<&CTestClass::m_Link, CCompare> m_Tree;
		};

		void f_DoTests() override;
	};

	template <typename t_CClass>
	auto CAVLTree_Tests::TCDefaultAccessor<t_CClass>::fs_GetOffset(t_CClass &_Class) -> CLinkContainer &
	{
		return _Class.m_Link;
	}

	CAVLTree_Tests::CData::CData()
		: m_Data()
		, m_bValid(false)
	{
	}

	CAVLTree_Tests::CData::~CData()
	{
		m_bValid = false;
	}

	CAVLTree_Tests::CData::CData(CStr const &_Data)
		: m_Data(_Data)
		, m_bValid(true)
	{
	}

	CAVLTree_Tests::CData::CData(CData const &_Other)
		: m_Data(_Other.m_Data)
		, m_bValid(_Other.m_bValid)
	{
	}

	CAVLTree_Tests::CData::CData(CData &&_Other)
		: m_Data(_Other.m_Data)
		, m_bValid(_Other.m_bValid)
	{
		_Other.m_bValid = false;
	}

	auto CAVLTree_Tests::CData::operator = (CData &&_Other) -> CData &
	{
		m_Data = _Other.m_Data;
		m_bValid = _Other.m_bValid;
		_Other.m_bValid = false;
		return *this;
	}

	CAVLTree_Tests::CTestClass::CCompare::CCompare()
		: mp_bValid(true)
	{
	}

	CAVLTree_Tests::CTestClass::CCompare::CCompare(CCompare &&_Other)
		: mp_bValid(true)
	{
		_Other.mp_bValid = false;
	}

	CAVLTree_Tests::CTestClass::CCompare::~CCompare()
	{
		mp_bValid = false;
	}

	auto CAVLTree_Tests::CTestClass::CCompare::operator = (CCompare &&_Other) -> CCompare &
	{
		mp_bValid = _Other.mp_bValid;
		_Other.mp_bValid = false;
		return *this;
	}

	auto CAVLTree_Tests::CTestClass::CCompare::operator () (CTestClass const &_Node) const -> CData const &
	{
		return _Node.m_Data;
	}

	NMib::COrdering_Partial CAVLTree_Tests::CTestClass::CCompare::operator () (CData const &_Left, CData const &_Right) const
	{
		DMibTest(DMibExpr(_Left.m_bValid))(ETestFlag_Aggregated);
		DMibTest(DMibExpr(_Right.m_bValid))(ETestFlag_Aggregated);

		return fg_StrCmpNoCase(_Left.m_Data, _Right.m_Data) <=> 0;
	}

	CAVLTree_Tests::CTestClass::CTestClass(CStr const &_Data)
		: m_Data(_Data)
	{
	}

	CAVLTree_Tests::CTestClass::CTestClass(CData const &_Data)
		: m_Data(_Data)
	{
	}

	void CAVLTree_Tests::f_DoTests()
	{
		DMibTestSuite("AVL")
		{
			using CNode = CTestClass;
			using CMint = CData;

			CTestClass Test("4");

			CTestClass Test1("1");
			CTestClass Test2("8");
			CTestClass Test3("3");
			CTestClass Test4("7");

			auto fInsert = [&](CNode &_Test, CStr const &_Path)
				{
					DMibTestPath(_Path);
					DMibTest(DMibExpr(!Test.m_Tree.f_FindEqual((_Test.m_Data))));
					Test.m_Tree.f_Insert(_Test);
					DMibTest(DMibExpr(!!Test.m_Tree.f_FindEqual((_Test.m_Data))));
				}
			;
			auto fInsert2 = [&](CNode &&_Test, CStr const &_Path)
				{
					DMibTestPath(_Path);
					DMibTest(DMibExpr(!Test.m_Tree.f_FindEqual((_Test.m_Data))));
					Test.m_Tree.f_Insert(_Test);
					DMibTest(DMibExpr(!!Test.m_Tree.f_FindEqual((_Test.m_Data))));
				}
			;
			fInsert(Test1, "Test1");
			fInsert(Test2, "Test2");
			fInsert(Test3, "Test3");
			fInsert(Test4, "Test4");
			fInsert2(CNode("5"), "CNode5");
			fInsert2(CNode("9"), "CNode9");
			fInsert2(CNode("0"), "CNode0");
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

			DMibTest(DMibExpr(Test.m_Data.m_bValid));
			DMibTest(DMibExpr(Test1.m_Data.m_bValid));
			DMibTest(DMibExpr(Test2.m_Data.m_bValid));
			DMibTest(DMibExpr(Test3.m_Data.m_bValid));
			DMibTest(DMibExpr(Test4.m_Data.m_bValid));

			for (auto iNode = Test.m_Tree.f_GetIterator(); iNode; ++iNode)
			{
				DMibTest(DMibExpr(iNode->m_Data.m_bValid))(ETestFlag_Aggregated);
				CStr Data = iNode->m_Data.m_Data;
			}

			/*
			Test.m_Tree.f_Remove(Test1);
			Test.m_Tree.f_Remove(Test2);
			Test.m_Tree.f_Remove(Test3);
			Test.m_Tree.f_Remove(Test4);*/
		};
	}

	[[maybe_unused]] constinit TCAVLTreeAggregate<&CAVLTree_Tests::CTestClass::m_Link, CAVLTree_Tests::CTestClass::CCompare> g_Tree = {DAggregateInit};

	DMibTestRegister(CAVLTree_Tests, Malterlib::Intrusive);
}

struct CTest0
{
	umint m_Data;

private:
	void fp_Test();
};

void CTest0::fp_Test()
{
}

CTest0 g_Test = {0};

#if 0
template <typename t_CLinkType, typename t_CData, t_CLinkType (t_CData:: *t_pMemberPointer)>
struct TCMemberPointerHolder
{
};

template <int t_Offset>
struct TCStaticOffset
{
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
