// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <Mib/Core/Core>

#define DMibIncluded_IntusiveAVLTree

namespace NMib::NIntrusive
{
	enum EAVLLinkType : int32
	{
		EAVLLinkType_Aligned
		, EAVLLinkType_AlignedRight
		, EAVLLinkType_Unaligned
		, EAVLLinkType_Custom
	};
}

#include "Private/Malterlib_Intrusive_AVLTree_Links.h"

namespace NMib::NIntrusive
{
	namespace NPrivate
	{
		template <typename t_CInner>
		struct TCImplicitConvert
		{
			TCImplicitConvert(t_CInner &_Inner)
				: m_Inner(_Inner)
			{
			}

			operator t_CInner & ()
			{
				return m_Inner;
			}

			t_CInner &m_Inner;
		};
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	struct TCAVLTreeAggregate;

	/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
	|	Template Class:																					|
	|																									|
	|	Description:		...																			|
	|																									|
	|	Comments:			.																			|
	|																									|
	|	See Also:			.																			|
	|																									|
	|	Location:			Template Classes															|
	|																									|
	|	Index:				!name																		|
	\*_________________________________________________________________________________________________*/
	template
	<
		auto t_pLinkMember
		, typename t_CCompare = CSort_Default
		, typename t_CAllocator = NMib::NMemory::CDefaultAllocator
		, typename t_COverrideNodeType = NTraits::TCMemberObjectPointerClass<decltype(t_pLinkMember)>
	>
	struct TCAVLTreeAggregate
	{
		using CCompare = t_CCompare;
		using CAllocator = t_CAllocator;
		using CMemberPointer = decltype(t_pLinkMember);
		using CNode = t_COverrideNodeType;
		using CLinkContainer = NTraits::TCRemoveMemberObjectPointer<CMemberPointer>;
		using CLink = typename CLinkContainer::CLink; // The inner link type containing the storage for links, has to be the first member of CLinkContainer

		// Depth of perfect tree * 1.5 approximation of (1.44*Log2(n+2) - 1)
#ifdef DCompiler_clang_cl
		// NOLINTBEGIN(malterlib-type-naming): as a variable, clang-cl fails on the arrays of this size with an impossible stack frame size
		enum
		{
			mc_SafeTreeDepth = umint(((sizeof(void *) * 12) - gc_HighestBitSet<sizeof(CLink)>))
		};

		// NOLINTEND(malterlib-type-naming)
#else
		static constexpr umint mc_SafeTreeDepth = ((sizeof(void *) * 12) - gc_HighestBitSet<sizeof(CLink)>);
#endif

	protected:
		using CLinkPointer = CLink *; // The pointer type of the inner link type for each left/right link

		struct CTemporaryStack
		{
			CLinkPointer *m_Stack[mc_SafeTreeDepth];
			bool m_Larger[mc_SafeTreeDepth];

			CLinkPointer **m_pStack;
			bool *m_pLarger;
		};

	public:
		CLinkPointer m_Root;

		constexpr TCAVLTreeAggregate(EAggregateInitialization _Init)
			: m_Root{nullptr}
		{
		}

		TCAVLTreeAggregate() = default;

	protected:
		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Debug																								|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename tf_FCompare>
		bool fp_CheckTree(CLink *_pCurrent, bool _bBreak, tf_FCompare &&_fCompare, umint &_Depth);

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Misc																								|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		static inline_small return_not_aliased CLink *fsp_LinkFromMember(CNode *_pMember);
		static inline_small return_not_aliased CNode *fsp_MemberFromLink(CLink *_pLink);
		static inline_small return_not_aliased CLink *fsp_LinkFromMemberConst(CNode const *_pMember);
		static inline_small return_not_aliased CNode *fsp_MemberFromLinkConst(CLink const *_pLink);
		static inline_small void fsp_Remove(void *_pTree, CLink &_Link);

		template <typename tf_FCompare, typename tf_CNode>
		inline_small static auto fsp_GetKey(tf_FCompare &&_fCompare, tf_CNode &&_Node)
			-> NTraits::TCCallableReturnTypeFor<NTraits::TCRemoveReference<tf_FCompare>, void (NPrivate::TCImplicitConvert<tf_CNode &&> _Node)>
			requires (NTraits::cIsCallableWith<NTraits::TCRemoveReference<tf_FCompare>, void (NPrivate::TCImplicitConvert<tf_CNode &&> _Node)>)
		{
			return _fCompare(fg_Forward<tf_CNode>(NPrivate::TCImplicitConvert<tf_CNode>(_Node)));
		}

		template <typename tf_FCompare, typename tf_CNode>
		inline_small static auto fsp_GetKey(tf_FCompare &&_fCompare, tf_CNode &&_Node) -> NTraits::TCRemoveRValueReference<tf_CNode>
			requires (!NTraits::cIsCallableWith<NTraits::TCRemoveReference<tf_FCompare>, void (NPrivate::TCImplicitConvert<tf_CNode &&> _Node)>)
		{
			return _Node;
		}

		template <typename tf_FCompare, typename tf_CLeft, typename tf_CRight>
		inline_small static auto fsp_DoCompare(tf_FCompare &&_fCompare, tf_CLeft &&_Left, tf_CRight &&_Right)
			requires (NTraits::cIsCallableWith<NTraits::TCRemoveReference<tf_FCompare>, void (tf_CLeft &&_Left, tf_CRight &&_Right)>)
		{
			return _fCompare(_Left, _Right);
		}

		template <typename tf_FCompare, typename tf_CLeft, typename tf_CRight>
		inline_small static auto fsp_DoCompare(tf_FCompare &&_fCompare, tf_CLeft &&_Left, tf_CRight &&_Right)
			requires (!NTraits::cIsCallableWith<NTraits::TCRemoveReference<tf_FCompare>, void (tf_CLeft &&_Left, tf_CRight &&_Right)>)
		{
			return _Left <=> _Right;
		}

		template <typename tf_FCompare, typename tf_CLeft, typename tf_CRight>
		inline_small static auto fsp_Compare(tf_FCompare &&_fCompare, tf_CLeft &&_Left, tf_CRight &&_Right)
		{
			return fsp_DoCompare(fg_Forward<tf_FCompare>(_fCompare), fsp_GetKey(_fCompare, fg_Forward<tf_CLeft>(_Left)), fsp_GetKey(_fCompare, fg_Forward<tf_CRight>(_Right)));
		}

		//static void fp_RemoveAll(CLink *_pObject);
		template <typename tf_FOnRemove>
		void fp_RemoveAll(tf_FOnRemove &&_fOnRemove);

		template <typename tf_FOnRemove>
		void fp_RemoveAllRecursive(CLink *_pNode, tf_FOnRemove &&_fOnRemove);

		template <typename tf_FCreateNode>
		CLink *fp_CopyTreeRecursive(CLink const *_pSourceNode, tf_FCreateNode &&_fCreateNode);

		template <typename tf_CTree, typename tf_FCreateNode>
		CLink *fp_CopyTreeRecursive(typename tf_CTree::CLink const *_pSourceNode, tf_FCreateNode &&_fCreateNode);

		template <typename tf_FCreateNode>
		CLink *fp_MoveTreeRecursive(CLink *_pSourceNode, tf_FCreateNode &&_fCreateNode);

		template <typename tf_CTree, typename tf_FCreateNode>
		CLink *fp_MoveTreeRecursive(typename tf_CTree::CLink *_pSourceNode, tf_FCreateNode &&_fCreateNode);

		umint fp_GetDepthRecursive(CLink *_pNode) const;

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Shared																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		static inline_medium void fsp_RotateRight(CLinkPointer &_pObject);
		static inline_medium void fsp_RotateLeft(CLinkPointer &_pObject);
		static inline_medium bool fsp_RightShrunk(CLinkPointer &_pObject);
		static inline_medium bool fsp_LeftShrunk(CLinkPointer &_pObject);
		static bool fsp_RightGrown(CLinkPointer &_pObject);
		static inline_medium bool fsp_RightGrown_Inl(CLinkPointer &_pObject);
		static bool fsp_LeftGrown(CLinkPointer &_pObject);
		static inline_medium bool fsp_LeftGrown_Inl(CLinkPointer &_pObject);

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Recursive																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		static bool fsp_BalanceLowest(CLink * &_pLowestObject, CLinkPointer &_pObject);
		static bool fsp_BalanceHighest(CLink * &_pHighestObject, CLinkPointer &_pObject);
		template <typename tf_FCompare>
		static bool fsp_RemoveRecursive(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		static bool fsp_InsertRecursive(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_FCompare &&_fCompare, bool &_bRet);

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Static Stack																						|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		static inline_medium bool fsp_BalanceLowest(CLink * &_pLowestObject, CLinkPointer &_pObject, CLinkPointer **_pStack);
		static inline_medium bool fsp_BalanceHighest(CLink * &_pHighestObject, CLinkPointer &_pObject, CLinkPointer **_pStack);
		template <typename tf_FCompare>
		static void fsp_Remove(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_FCompare &&_fCompare);
		static void fsp_Removed(CLinkPointer *_pObject, CLink *_pObj, CTemporaryStack &_Stack);

		// No-rebalance removal helpers - used during destructive iteration
		static inline_medium CLink *fsp_DetachHighest(CLinkPointer &_pObject);
		static inline_medium CLink *fsp_DetachLowest(CLinkPointer &_pObject);
		// Returns the replacement node (or nullptr if removed node was a leaf)
		static CLink *fsp_RemovedNoRebalance(CLinkPointer *_pObject, CLink *_pObj);

		template <int tf_Direction>
		static void fsp_RemoveRotate3(CLinkPointer *_pObject);
		template <int tf_Direction>
		static void fsp_RemoveRotate2(CLinkPointer *_pObject);
		template <typename tf_FCompare>
		static bool fsp_Insert(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_FCompare &&_fCompare);
		template <typename tf_CKey, typename tf_FOnInsert, typename tf_FCompare>
		CNode *fp_FindEqualOrInsert(CLinkPointer &_pObject, tf_CKey const &_Key, tf_FOnInsert &&_fOnInsert, tf_FCompare &&_fCompare);

		template <typename tf_CKey, typename tf_FCompare>
		static CNode *fsp_FindEqualAndRemove(CLinkPointer &_pObject, tf_CKey const &_Key, tf_FCompare &&_fCompare);

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Low stack																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename tf_FCompare>
		static inline_medium CLinkPointer *fsp_RemoveRebalance(CLinkPointer *_pTop, CLinkPointer *_pTarget, CLink *_pObjectToRemove, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		static void fsp_RemoveLowStack(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_FCompare &&_fCompare);

		template <int tf_Direction>
		static inline_small CLink *fsp_Rotate2(CLinkPointer *_pTop);
		static void fsp_Rotate3Short(CLinkPointer *_pTop, int _Direction);
		static CLink *fsp_Rotate3(CLinkPointer *_pTop, int _Direction, int _Third);
		template <typename tf_FCompare>
		static inline_small void fsp_RebalancePathLowStack(CLink *_pPath, CLink *_pObjectToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		static inline_small void fsp_RebalanceCase3LowStack(int _Direction, CLink *_pPath, CLinkPointer *_pTop, CLink *_pObjectToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		static inline_small void fsp_RebalanceLowStack(CLinkPointer *_pTop, CLink *_pObjectToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		static bool fsp_InsertLowStack(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_FCompare &&_fCompare);

	public:
		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Lifetime																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		inline_small void f_Construct(TCAVLTreeAggregate &&_Other);
		inline_small void f_Construct();
		inline_small void f_Destruct();

		void f_DeleteAll();
		template <typename tf_FDoDelete>
		void f_DeleteAll(tf_FDoDelete &&_fDoDelete);
		template <typename tf_CAllocator>
		void f_DeleteAllAllocator();
		template <typename tf_CAllocator>
		void f_DeleteAllAllocator(tf_CAllocator &_Allocator);
		void f_DeleteAllDefiniteType();
		template <typename tf_CAllocator>
		void f_DeleteAllAllocatorDefiniteType();
		template <typename tf_CAllocator>
		void f_DeleteAllAllocatorDefiniteType(tf_CAllocator &_Allocator);
		template <typename tf_CDeleter>
		void f_DeleteAllDeleter();
		void f_DeleteAllDeleter();

		void f_RemoveAll();
		void f_Clear();

		template <typename tf_FCreateNode>
		void f_CopyTree(TCAVLTreeAggregate const &_Source, tf_FCreateNode &&_fCreateNode);

		template <auto tf_pLinkMember, typename tf_CCompare, typename tf_CAllocator, typename tf_COverrideNodeType, typename tf_FCreateNode>
		void f_CopyTree(TCAVLTreeAggregate<tf_pLinkMember, tf_CCompare, tf_CAllocator, tf_COverrideNodeType> const &_Source, tf_FCreateNode &&_fCreateNode);

		template <typename tf_FCreateNode>
		void f_MoveTree(TCAVLTreeAggregate &_Source, tf_FCreateNode &&_fCreateNode);

		template <auto tf_pLinkMember, typename tf_CCompare, typename tf_CAllocator, typename tf_COverrideNodeType, typename tf_FCreateNode>
		void f_MoveTree(TCAVLTreeAggregate<tf_pLinkMember, tf_CCompare, tf_CAllocator, tf_COverrideNodeType> &_Source, tf_FCreateNode &&_fCreateNode);

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Misc																								|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		static inline_small CNode *fs_MemberFromLink(CLink *_pLink);
		constexpr inline_small bool f_IsEmpty() const;
		inline_small bool f_HasOneElement() const;
		inline_small CNode *f_GetRoot() const;
		static inline_small CNode *fs_GetRight(CNode *_pObject);
		static inline_small CNode *fs_GetLeft(CNode *_pObject);
		inline_medium umint f_GetLen() const;
		umint f_GetDepth() const;
		bool operator == (TCAVLTreeAggregate const &_Other) const noexcept(noexcept(fg_GetType<CNode const &>() == fg_GetType<CNode const &>()));
		auto operator <=> (TCAVLTreeAggregate const &_Other) const noexcept(noexcept(fg_GetType<CNode const &>() <=> fg_GetType<CNode const &>()));

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Recursive																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename tf_FCompare>
		inline_small bool f_InsertRecursive(CNode *_pToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		inline_small bool f_InsertRecursive(CNode &_ToInsert, tf_FCompare &&_fCompare);
		inline_small bool f_InsertRecursive(CNode &_ToInsert);
		inline_small bool f_InsertRecursive(CNode *_pToInsert);

		template <typename tf_FCompare>
		inline_small void f_RemoveRecursive(CNode *_pToRemove, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		inline_small void f_RemoveRecursive(CNode &_ToRemove, tf_FCompare &&_fCompare);
		inline_small void f_RemoveRecursive(CNode &_ToRemove);
		inline_small void f_RemoveRecursive(CNode *_pToRemove);

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Static stack																						|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename tf_FCompare>
		inline_small bool f_Insert(CNode *_pToInsert, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		inline_small bool f_Insert(CNode &_ToInsert, tf_FCompare &&_fCompare);
		inline_small bool f_Insert(CNode &_ToInsert);
		inline_small bool f_Insert(CNode *_pToInsert);

		template <typename tf_CKey, typename tf_FOnInsert, typename tf_FCompare>
		inline_small CNode *f_FindEqualOrInsert(tf_CKey const &_Key, tf_FOnInsert &&_fOnInsert, tf_FCompare &&_fCompare);
		template <typename tf_CKey, typename tf_FOnInsert>
		inline_small CNode *f_FindEqualOrInsert(tf_CKey const &_Key, tf_FOnInsert &&_fOnInsert);

		template <typename tf_CToMap, typename tf_FCompare>
		inline_small CNode *f_FindEqualOrNew(tf_CToMap &_ToMap, tf_FCompare &&_fCompare);
		template <typename tf_CToMap>
		inline_small CNode *f_FindEqualOrNew(tf_CToMap &_ToMap);

		template <typename tf_FCompare>
		inline_small void f_Remove(CNode *_pToRemove, tf_FCompare &&_fCompare);
		template <typename tf_FCompare>
		inline_small void f_Remove(CNode &_ToRemove, tf_FCompare &&_fCompare);
		inline_small void f_Remove(CNode &_ToRemove);
		inline_small void f_Remove(CNode *_pToRemove);

		template <typename tf_CKey, typename tf_FCompare>
		inline_small CNode *f_FindEqualAndRemove(tf_CKey const &_Key, tf_FCompare &&_fCompare);
		template <typename tf_CKey>
		inline_small CNode *f_FindEqualAndRemove(tf_CKey const &_Key);

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Low stack																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename tf_FCompare>
		inline_small bool f_InsertLowStack(CNode *_pToInsert, tf_FCompare &&_fCompare);
		inline_small bool f_InsertLowStack(CNode &_ToInsert);
		template <typename tf_FCompare>
		inline_small bool f_InsertLowStack(CNode &_ToInsert, tf_FCompare &&_fCompare);
		inline_small bool f_InsertLowStack(CNode *_pToInsert);

		inline_small void f_RemoveLowStack(CNode &_ToRemove);
		template <typename tf_FCompare>
		inline_small void f_RemoveLowStack(CNode &_ToRemove, tf_FCompare &&_fCompare);
		inline_small void f_RemoveLowStack(CNode *_pToRemove);
		template <typename tf_FCompare>
		inline_small void f_RemoveLowStack(CNode *_pToRemove, tf_FCompare &&_fCompare);

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Find																								|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		CNode *f_FindSmallest() const;
		CNode *f_FindLargest() const;

		template <typename tf_CKey, typename tf_FCompare>
		inline_medium CNode *f_FindEqual(tf_CKey const &_Key, tf_FCompare &&_fCompare) const;
		template <typename tf_CKey>
		inline_small CNode *f_FindEqual(tf_CKey const &_Key) const;

		template <typename tf_CKey>
		inline_small CNode *f_FindSmallestGreaterThanEqual(tf_CKey const &_Key) const;
		template <typename tf_CKey, typename tf_FCompare>
		inline_large CNode *f_FindSmallestGreaterThanEqual(tf_CKey const &_Key, tf_FCompare &&_fCompare) const;

		template <typename tf_CKey>
		inline_small CNode *f_FindSmallestGreaterThanEqualAndPrev(tf_CKey const &_Key, CNode * &_pPrev) const;
		template <typename tf_CKey, typename tf_FCompare>
		inline_large CNode *f_FindSmallestGreaterThanEqualAndPrev(tf_CKey const &_Key, CNode * &_pPrev, tf_FCompare &&_fCompare) const;

		template <typename tf_CKey>
		inline_small CNode *f_FindLargestLessThanEqual(tf_CKey const &_Key) const;
		template <typename tf_CKey, typename tf_FCompare>
		inline_large CNode *f_FindLargestLessThanEqual(tf_CKey const &_Key, tf_FCompare &&_fCompare) const;

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Debug																								|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		bool f_CheckTree(bool _bBreak);
		template <typename tf_FCompare>
		bool f_CheckTree(bool _bBreak, tf_FCompare &&_fCompare);

		/************************************************************************************************\
		||¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯||
		|| Iterator
		||______________________________________________________________________________________________||
		\************************************************************************************************/

		template <aint t_RecursionDepth = mc_SafeTreeDepth>
		struct TCIterator
		{
			using CNode = typename TCAVLTreeAggregate::CNode;

		protected:
			aint mp_iStack;
			CLink const *mp_Stack[t_RecursionDepth];

		public:
			inline_small TCIterator();
			inline_medium TCIterator(TCIterator const &_iOther);
			inline_small TCIterator(TCAVLTreeAggregate const &_Tree);
			inline_small TCIterator(TCAVLTreeAggregate const *_pTree);

			TCIterator f_GetIterator() const;

			inline_medium void f_Debug_CheckStack();

			inline_medium umint f_GetLen() const;

			void f_Clear();

			inline_small CNode *f_GetCurrent() const;

			inline_medium void f_Next();
			inline_medium void f_Prev();

			inline_medium void f_NextBidirectional();
			inline_medium void f_PrevBidirectional();

			inline_small void f_StartForward(TCAVLTreeAggregate const &_Tree);
			inline_medium void f_StartForward(TCAVLTreeAggregate const *_pTree);

			inline_small void f_StartBackward(TCAVLTreeAggregate const &_Tree);
			inline_medium void f_StartBackward(TCAVLTreeAggregate const *_pTree);

			void f_InitForSearch(TCAVLTreeAggregate const &_Tree);
			void f_InitForSearch(TCAVLTreeAggregate const *_pTree);

			template <typename tf_CKey, typename tf_FCompare>
			bool f_FindEqualForward(tf_CKey const &_Key, tf_FCompare &&_fCompare);
			template <typename tf_CKey>
			bool f_FindEqualForward(tf_CKey const &_Key);

			template <typename tf_CKey, typename tf_FCompare>
			bool f_FindSmallestGreaterThanEqualForward(tf_CKey const &_Key, tf_FCompare &&_fCompare);
			template <typename tf_CKey>
			bool f_FindSmallestGreaterThanEqualForward(tf_CKey const &_Key);

			template <typename tf_CKey, typename tf_FCompare>
			bool f_FindLargestLessThanEqualForward(tf_CKey const &_Key, tf_FCompare &&_fCompare);
			template <typename tf_CKey>
			bool f_FindLargestLessThanEqualForward(tf_CKey const &_Key);

			template <typename tf_FCompare>
			void f_Delete(TCAVLTreeAggregate &_Tree, tf_FCompare &&_fCompare);
			void f_Delete(TCAVLTreeAggregate &_Tree);

			template <typename tf_FCompare, typename tf_CAllocator>
			void f_DeleteAllocator(TCAVLTreeAggregate &_Tree, tf_FCompare &&_fCompare, tf_CAllocator &_Allocator);
			template <typename tf_CAllocator>
			void f_DeleteAllocator(TCAVLTreeAggregate &_Tree, tf_CAllocator &_Allocator);

			template <typename tf_FCompare, typename tf_CAllocator>
			void f_DeleteAllocatorDefiniteType(TCAVLTreeAggregate &_Tree, tf_FCompare &&_fCompare, tf_CAllocator &_Allocator);
			template <typename tf_CAllocator>
			void f_DeleteAllocatorDefiniteType(TCAVLTreeAggregate &_Tree, tf_CAllocator &_Allocator);

			template <typename tf_FCompare>
			void f_Remove(TCAVLTreeAggregate &_Tree, tf_FCompare &&_fCompare);
			void f_Remove(TCAVLTreeAggregate &_Tree);

			// Extract current node without rebalancing, advance to next, and patch the stack
			// Returns the extracted node. Used during destructive iteration.
			// This must use the bidirectional algorithm to maintain the full ancestor path.
			template <bool tf_bReverse>
			CNode *f_ExtractCurrentNoRebalance(TCAVLTreeAggregate &_Tree);

			inline_small operator CNode * () const;
			inline_small CNode *operator -> () const;
			inline_small void operator ++ ();
			inline_small void operator -- ();

		private:
			friend struct TCAVLTreeAggregate;
		};

		using CIterator = TCIterator<>;
		using CIteratorConst = TCIterator<>;

		CIterator f_GetIterator();
		CIteratorConst f_GetIterator() const;

	private:
		template <auto t_pLinkMember2, typename t_CCompare2, typename t_CAllocator2, typename t_COverrideNodeType2>
		friend struct TCAVLTreeAggregate;
	};

	/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
	|	Template Class:																					|
	|																									|
	|	Description:		...																			|
	|																									|
	|	Comments:			.																			|
	|																									|
	|	See Also:			.																			|
	|																									|
	|	Location:			Template Classes															|
	|																									|
	|	Index:				!name																		|
	\*_________________________________________________________________________________________________*/
	template
	<
		auto t_pLinkMember
		, typename t_CCompare = CSort_Default
		, typename t_CAllocator = NMib::NMemory::CDefaultAllocator
		, typename t_COverrideNodeType = NTraits::TCMemberObjectPointerClass<decltype(t_pLinkMember)>
	>
	struct TCAVLTree : public TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>
	{
		TCAVLTree(TCAVLTree &&_Other);
		constexpr TCAVLTree();
		constexpr ~TCAVLTree();
		TCAVLTree &operator = (TCAVLTree &&_Other);

		bool operator == (TCAVLTree const &_Other) const noexcept(noexcept(fg_GetType<typename CSuper::CNode const &>() == fg_GetType<typename CSuper::CNode const &>()));
		auto operator <=> (TCAVLTree const &_Other) const noexcept(noexcept(fg_GetType<typename CSuper::CNode const &>() <=> fg_GetType<typename CSuper::CNode const &>()));

	private:
		TCAVLTree(TCAVLTree const &_Other) = delete;
		TCAVLTree &operator = (TCAVLTree const &_Other) = delete;

		using CSuper = TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>;
	};

	/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
	|	Template Class:																					|
	|																									|
	|	Description:		...																			|
	|																									|
	|	Comments:			.																			|
	|																									|
	|	See Also:			.																			|
	|																									|
	|	Location:			Template Classes															|
	|																									|
	|	Index:				!name																		|
	\*_________________________________________________________________________________________________*/
	template <EAVLLinkType t_LinkType = EAVLLinkType_AlignedRight, typename t_CLinkData = typename NPrivate::TCLinkTypeToLink<t_LinkType>::CType>
	struct TCAVLLinkAggregate
	{
		using CLink = t_CLinkData;
		using FRemove = void (void *_pTree, t_CLinkData &_Link);

		static constexpr bool mc_bNeedSetTree = false;

		t_CLinkData m_Link;

#ifdef DCompiler_clang

		constexpr TCAVLLinkAggregate()
			: m_Link()
		{
		}
#endif

		inline_small void f_Construct();
		inline_small void f_Destruct();
		bool f_IsInTree() const;
	};

	/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
	|	Template Class:																					|
	|																									|
	|	Description:		...																			|
	|																									|
	|	Comments:			.																			|
	|																									|
	|	See Also:			.																			|
	|																									|
	|	Location:			Template Classes															|
	|																									|
	|	Index:				!name																		|
	\*_________________________________________________________________________________________________*/
	template <EAVLLinkType t_LinkType = EAVLLinkType_AlignedRight, typename t_CLinkData = typename NPrivate::TCLinkTypeToLink<t_LinkType>::CType>
	struct TCAVLLink : public TCAVLLinkAggregate<t_LinkType, t_CLinkData>
	{
		TCAVLLink();
		~TCAVLLink();

	private:
		TCAVLLink(TCAVLLink const &_Other) = delete;
		TCAVLLink &operator = (TCAVLLink const &_Other) = delete;
	};

	/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
	|	Template Class:																					|
	|																									|
	|	Description:		...																			|
	|																									|
	|	Comments:			.																			|
	|																									|
	|	See Also:			.																			|
	|																									|
	|	Location:			Template Classes															|
	|																									|
	|	Index:				!name																		|
	\*_________________________________________________________________________________________________*/
	template <EAVLLinkType t_LinkType = EAVLLinkType_AlignedRight, typename t_CLinkData = typename NPrivate::TCLinkTypeToLink<t_LinkType>::CType>
	struct TCAVLLinkAutoRemove : public TCAVLLink<t_LinkType, t_CLinkData>
	{
		static constexpr bool mc_bNeedSetTree = true;

		using FRemove = void (void *_pTree, t_CLinkData &_Link);

		void *m_pTree = nullptr;
		FRemove *m_fRemove = nullptr;

		inline_small void f_Construct();
		inline_small void f_Destruct();
		bool f_IsInTree() const;
		inline_small void f_SetTree(void *_pTree, FRemove *_fRemove);

		TCAVLLinkAutoRemove();
		~TCAVLLinkAutoRemove();

	private:
		TCAVLLinkAutoRemove(TCAVLLinkAutoRemove const &_Other) = delete;
		TCAVLLinkAutoRemove &operator = (TCAVLLinkAutoRemove const &_Other) = delete;
	};
}

// Implementation
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_Lifetime.h"
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_Shared.h"
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_LowStack.h"
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_Recursive.h"
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_StaticStack.h"
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_Debug.h"
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_Misc.h"
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_Search.h"
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_Iterator.h"
#include "Private/Malterlib_Intrusive_AVLTree_Implementation_Links.h"

#ifdef DMibIncluded_Stream
#	include "Malterlib_Intrusive_AVLTree_Stream.h"
#endif

#include "Malterlib_Intrusive_AVLTree_CompareDefaults.h"

#ifndef DMibPNoShortCuts
using namespace NMib::NIntrusive;
#endif
