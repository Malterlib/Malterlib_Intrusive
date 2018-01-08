// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

#include <Mib/Core/Core>

#include "Malterlib_Intrusive_Shared.h"
#define DMibIncluded_IntusiveAVLTree

namespace NMib
{
	namespace NIntrusive
	{
		using CAVLLinkTypeUnderlaying = int32;
		enum EAVLLinkType : int32
		{
			EAVLLinkType_Aligned
			, EAVLLinkType_AlignedRight
			, EAVLLinkType_Unaligned
			, EAVLLinkType_Custom
		};
	}
}

#include "Private/Malterlib_Intrusive_AVLTree_Links.h"

namespace NMib
{
	namespace NIntrusive
	{
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
			typename t_CLinkTraits
			, typename t_CCompare = CSort_Default
			, typename t_CAllocator = NMib::NMem::CDefaultAllocator
		>
		class TCAVLTreeAggregate
		{
		public:
			typedef typename t_CLinkTraits::CNode CNode;
			typedef t_CCompare CCompare;
			typedef t_CAllocator CAllocator;
			typedef typename t_CLinkTraits::CLinkContainer CLinkContainer; // The outer real link type
			typedef typename CLinkContainer::CLink CLink; // The inner link type containing the storage for links, has to be the first member of CLinkContainer
		protected:
			typedef typename CLink::CLinkPointer CLinkPointer; // The pointer type of the inner link type for each left/right link
			typedef t_CLinkTraits CTranslator;
		public:
			typedef typename CTranslator::template TCOffset<CNode> COffset;
		protected:
			
#ifdef DMibDebuggerHelpers
			static mint ms_OffsetTCAVLTreeAggregate;
			static CNode* ms_pNode;
#endif

			class CStackObj
			{
				CLinkPointer * m_pStack;
				bint m_bLarger;
			public:
				inline_small void f_SetAll(CLinkPointer *_pPtr, bint _bLarger);
				inline_small void f_SetStack(CLinkPointer *_pPtr);
				inline_small CLinkPointer *f_GetStack() const;
				inline_small aint f_IsLarger() const;
			};

		public:
			CLinkPointer m_Root;
			
#ifndef DMibNoAggregateConstexpr
			constexpr TCAVLTreeAggregate(EAggregateInitialization _Init)
				: m_Root{_Init}
			{
			}
			TCAVLTreeAggregate()
			{
#ifdef DMibDebuggerHelpers
				(void)&ms_OffsetTCAVLTreeAggregate;
				(void)&ms_pNode;
#endif
			}
#endif
			
		protected:

			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Debug																								|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			template <typename tf_CCompare>
			bint fpr_CheckTree(CLink *_pCurrent, bint _bBreak, tf_CCompare &&_Compare, mint &_Depth);
			 
			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Misc																								|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			static inline_small return_not_aliased CLink *fsp_LinkFromMember(CNode *_pMember);
			static inline_small return_not_aliased CNode *fsp_MemberFromLink(CLink *_pLink);
			static inline_small return_not_aliased CLink *fsp_LinkFromMemberConst(const CNode *_pMember);
			static inline_small return_not_aliased CNode *fsp_MemberFromLinkConst(const CLink *_pLink);

			void fpr_Unlink(CLinkPointer &_pUnlink);

			template <typename t_Inner>
			struct TCImplicitConvert
			{
				t_Inner& m_Inner;
				TCImplicitConvert(t_Inner& _Inner) : m_Inner(_Inner) {}
				operator t_Inner& () { return m_Inner; }
			};
			template <typename tf_CCompare, typename tf_CNode>
			inline_small static auto fsp_GetKey(tf_CCompare &&_Compare, tf_CNode &&_Node) 
			-> typename TCEnableIf
			<
				NTraits::TCIsCallableWith<typename NTraits::TCRemoveReference<tf_CCompare>::CType, void (TCImplicitConvert<tf_CNode &&>)>::mc_Value
				, typename NTraits::TCIsCallableWith< typename NTraits::TCRemoveReference<tf_CCompare>::CType, void (TCImplicitConvert<tf_CNode &&>)>::CReturnType
			>::CType
			{
				return fg_Forward<tf_CCompare>(_Compare)(fg_Forward<tf_CNode>(TCImplicitConvert<tf_CNode>(_Node)));
			}

			template <typename tf_CCompare, typename tf_CNode>
			inline_small static auto fsp_GetKey(tf_CCompare &&_Compare, tf_CNode &&_Node) 
			-> typename TCEnableIf
			<
				!NTraits::TCIsCallableWith<typename NTraits::TCRemoveReference<tf_CCompare>::CType, void (TCImplicitConvert<tf_CNode &&>)>::mc_Value
				, typename NTraits::TCRemoveRValueReference<tf_CNode>::CType
			>::CType
			{
				return fg_Forward<tf_CNode>(_Node);
			}

			template <typename tf_CCompare, typename tf_CLeft, typename tf_CRight>
			inline_small static auto fsp_DoCompare(tf_CCompare &&_Compare, tf_CLeft &&_Left, tf_CRight &&_Right) 
			-> typename TCEnableIf<NTraits::TCIsCallableWith<typename NTraits::TCRemoveReference<tf_CCompare>::CType, bint (tf_CLeft &&, tf_CRight &&)>::mc_Value, bint>::CType
			{
				return fg_Forward<tf_CCompare>(_Compare)(fg_Forward<tf_CLeft>(_Left), fg_Forward<tf_CRight>(_Right));
			}

			template <typename tf_CCompare, typename tf_CLeft, typename tf_CRight>
			inline_small static auto fsp_DoCompare(tf_CCompare &&_Compare, tf_CLeft &&_Left, tf_CRight &&_Right) 
			-> typename TCEnableIf<!NTraits::TCIsCallableWith<typename NTraits::TCRemoveReference<tf_CCompare>::CType, bint (tf_CLeft &&, tf_CRight &&)>::mc_Value, bint>::CType
			{
				return fg_Forward<tf_CLeft>(_Left) < fg_Forward<tf_CRight>(_Right);
			}

/*
			template <typename tf_CCompare, typename tf_CLeft, typename tf_CRight>
			inline_small static auto fsp_Compare(tf_CCompare &&_Compare, tf_CLeft &&_Left, tf_CRight &&_Right) 
				-> typename TCEnableIf
				<
				NTraits::TCIsCallableWith<typename NTraits::TCRemoveReference<tf_CCompare>::CType, bint (tf_CLeft,tf_CRight)>::mc_Value
				, typename NTraits::TCIsCallableWith<typename NTraits::TCRemoveReference<tf_CCompare>::CType, bint (tf_CLeft,tf_CRight)>::CReturnType
				>::CType
			{
				return fsp_DoCompare
					(
					_Compare
					, fg_Forward<tf_CLeft>(_Left)
					, fg_Forward<tf_CRight>(_Right)
					);
			}
			template <typename tf_CCompare, typename tf_CLeft, typename tf_CRight>
			inline_small static auto fsp_Compare(tf_CCompare &&_Compare, tf_CLeft &&_Left, tf_CRight &&_Right) 
				-> typename TCEnableIf
				<
				!NTraits::TCIsCallableWith<typename NTraits::TCRemoveReference<tf_CCompare>::CType, bint (tf_CLeft,tf_CRight)>::mc_Value
				, bint
				>::CType
			{
				return fsp_DoCompare
					(
					_Compare
					, fsp_GetKey(_Compare, fg_Forward<tf_CLeft>(_Left))
					, fsp_GetKey(_Compare, fg_Forward<tf_CRight>(_Right))
					);
			}
*/
			template <typename tf_CCompare, typename tf_CLeft, typename tf_CRight>
			inline_small static bint fsp_Compare(tf_CCompare &&_Compare, tf_CLeft &&_Left, tf_CRight &&_Right) 
			{
				return fsp_DoCompare
					(
						fg_Forward<tf_CCompare>(_Compare)
						, fsp_GetKey(_Compare, fg_Forward<tf_CLeft>(_Left))
						, fsp_GetKey(_Compare, fg_Forward<tf_CRight>(_Right))
					)
				;
			}


			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Shared																							|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			static inline_medium void fsp_RotateRight(CLinkPointer &_pObject);			
			static inline_medium void fsp_RotateLeft(CLinkPointer &_pObject);
			static inline_medium bint fsp_RightShrunk(CLinkPointer &_pObject);
			static inline_medium bint fsp_LeftShrunk(CLinkPointer &_pObject);
			static bint fsp_RightGrown(CLinkPointer &_pObject);
			static inline_medium bint fsp_RightGrown_Inl(CLinkPointer &_pObject);
			static bint fsp_LeftGrown(CLinkPointer &_pObject);
			static inline_medium bint fsp_LeftGrown_Inl(CLinkPointer &_pObject);

			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Recursive																							|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			static bint fsp_BalanceLowest(CLink* &_pLowestObject, CLinkPointer &_pObject);			
			static bint fsp_BalanceHighest(CLink* &_pHighestObject, CLinkPointer &_pObject);
			template <typename tf_CCompare>
			static bint fspr_Remove(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_CCompare &&_Compare);
			template <typename tf_CCompare>
			static bint fspr_Insert(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_CCompare &&_Compare, bint &_bRet);


			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Static Stack																						|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			static inline_medium bint fp_BalanceLowest(CLink* &_pLowestObject, CLinkPointer &_pObject, CStackObj *&_pStack);
			static inline_medium bint fp_BalanceHighest(CLink* &_pHighestObject, CLinkPointer &_pObject, CStackObj *&_pStack);
			template <typename tf_CCompare>
			static void fp_Remove(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_CCompare &&_Compare);
			template <int tf_Direction>
			static void fp_RemoveRotate3(CLinkPointer *_pObject);
			template <int tf_Direction>
			static void fp_RemoveRotate2(CLinkPointer *_pObject);
			template <typename tf_CCompare>
			static bint fp_Insert(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_CCompare &&_Compare);

			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Low stack																							|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			template <typename tf_CCompare>
			static inline_medium CLinkPointer *fp_RemoveRebalance(CLinkPointer *_pTop, CLinkPointer *_pTarget, CLink *_pObjectToRemove, tf_CCompare &&_Compare);
			template <typename tf_CCompare>
			static void fp_RemoveLowStack(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_CCompare &&_Compare);

			template <int tf_Direction>
			static inline_small CLink *fp_Rotate2(CLinkPointer *_pTop);
			static void fp_Rotate3Short(CLinkPointer *_pTop, int _Direction);
			static CLink *fp_Rotate3(CLinkPointer *_pTop, int _Direction, int _Third);
			template <typename tf_CCompare>
			static inline_small void fp_RebalancePathLowStack(CLink *_pPath, CLink *_pObjectToInsert, tf_CCompare &&_Compare);
			template <typename tf_CCompare>
			static inline_small void fp_RebalanceCase3LowStack(int _Direction, CLink *pPath, CLinkPointer *_pTop, CLink *_pObjectToInsert, tf_CCompare &&_Compare);
			template <typename tf_CCompare>
			static inline_small void fp_RebalanceLowStack(CLinkPointer *_pTop, CLink *_pObjectToInsert, tf_CCompare &&_Compare);
			template <typename tf_CCompare>
			static bint fp_InsertLowStack(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_CCompare &&_Compare);


		public:

			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Lifetime																							|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/


			inline_small void f_Construct(TCAVLTreeAggregate &&_Other);
			inline_small void f_Construct();
			inline_small void f_Destruct();

			template <typename tf_CCompare>
			void f_DeleteAll(tf_CCompare &&_Compare);
			void f_DeleteAll();

			template <typename tf_CAllocator, typename tf_CCompare>
			void f_DeleteAllAllocatorCompare(tf_CCompare &&_Compare);
			template <typename tf_CAllocator, typename tf_CCompare>
			void f_DeleteAllAllocatorCompare(tf_CCompare &&_Compare, tf_CAllocator &_Allocator);

			template <typename tf_CAllocator>
			void f_DeleteAllAllocator();
			template <typename tf_CAllocator>
			void f_DeleteAllAllocator(tf_CAllocator &_Allocator);


			template <typename tf_CCompare>
			void f_DeleteAllDefiniteType(tf_CCompare &&_Compare);
			void f_DeleteAllDefiniteType();

			template <typename tf_CAllocator, typename tf_CCompare>
			void f_DeleteAllAllocatorCompareDefiniteType(tf_CCompare &&_Compare);
			template <typename tf_CAllocator, typename tf_CCompare>
			void f_DeleteAllAllocatorCompareDefiniteType(tf_CCompare &&_Compare, tf_CAllocator &_Allocator);

			template <typename tf_CAllocator>
			void f_DeleteAllAllocatorDefiniteType();
			template <typename tf_CAllocator>
			void f_DeleteAllAllocatorDefiniteType(tf_CAllocator &_Allocator);


			template <typename tf_CDeleter, typename tf_CCompare>
			void f_DeleteAllDeleter(tf_CCompare &&_Compare);
			template <typename tf_CDeleter>
			void f_DeleteAllDeleter();

			template <typename tf_CCompare>
			void f_DeleteAllDeleter(tf_CCompare &&_Compare);
			void f_DeleteAllDeleter();

			template <typename tf_CCompare>
			void f_RemoveAll(tf_CCompare &&_Compare);
			void f_RemoveAll();

			template <typename tf_CCompare>
			void f_Clear(tf_CCompare &&_Compare);
			void f_Clear();

			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Misc																								|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			static inline_small CNode *fs_MemberFromLink(CLink *_pLink);
			inline_small bint f_IsEmpty() const;
			inline_small bint f_HasOneMember() const;
			inline_small CNode *f_GetRoot() const;
			static inline_small CNode *fs_GetRight(CNode *_pObject);
			static inline_small CNode *fs_GetLeft(CNode *_pObject);
			inline_medium mint f_GetLen() const;
			bint operator == (const TCAVLTreeAggregate &_Other) const;
			bint operator < (const TCAVLTreeAggregate &_Other) const;

			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Recursive																							|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			template <typename tf_CCompare>
			inline_small bint fr_Insert(CNode *_pToInsert, tf_CCompare &&_Compare);
			template <typename tf_CCompare>
			inline_small bint fr_Insert(CNode &_ToInsert, tf_CCompare &&_Compare);
			inline_small bint fr_Insert(CNode &_ToInsert);
			inline_small bint fr_Insert(CNode *_pToInsert);

			template <typename tf_CCompare>
			inline_small void fr_Remove(CNode *_pToRemove, tf_CCompare &&_Compare);
			template <typename tf_CCompare>
			inline_small void fr_Remove(CNode &_ToRemove, tf_CCompare &&_Compare);
			inline_small void fr_Remove(CNode &_ToRemove);
			inline_small void fr_Remove(CNode *_pToRemove);

			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Static stack																						|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			template <typename tf_CCompare>
			inline_small bint f_Insert(CNode *_pToInsert, tf_CCompare &&_Compare);
			template <typename tf_CCompare>
			inline_small bint f_Insert(CNode &_ToInsert, tf_CCompare &&_Compare);
			inline_small bint f_Insert(CNode &_ToInsert);
			inline_small bint f_Insert(CNode *_pToInsert);

			template <typename tf_ToMap, typename tf_CCompare>
			inline_small CNode *f_Map(tf_ToMap &_ToMap, tf_CCompare &&_Compare);
			template <typename tf_ToMap>
			inline_small CNode *f_Map(tf_ToMap &_ToMap);

			template <typename tf_CCompare>
			inline_small void f_Remove(CNode *_pToRemove, tf_CCompare &&_Compare);
			template <typename tf_CCompare>
			inline_small void f_Remove(CNode &_ToRemove, tf_CCompare &&_Compare);
			inline_small void f_Remove(CNode &_ToRemove);
			inline_small void f_Remove(CNode *_pToRemove);

			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Low stack																							|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/
			
			template <typename tf_CCompare>
			inline_small bint f_InsertLowStack(CNode *_pToInsert, tf_CCompare &&_Compare);
			inline_small bint f_InsertLowStack(CNode &_ToInsert);
			template <typename tf_CCompare>
			inline_small bint f_InsertLowStack(CNode &_ToInsert, tf_CCompare &&_Compare);
			inline_small bint f_InsertLowStack(CNode *_pToInsert);

			inline_small void f_RemoveLowStack(CNode &_ToRemove);
			template <typename tf_CCompare>
			inline_small void f_RemoveLowStack(CNode &_ToRemove, tf_CCompare &&_Compare);
			inline_small void f_RemoveLowStack(CNode *_pToRemove);
			template <typename tf_CCompare>
			inline_small void f_RemoveLowStack(CNode *_pToRemove, tf_CCompare &&_Compare);
			
			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Find																								|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			CNode* f_FindSmallest() const;
			CNode* f_FindLargest() const;

			template <typename tf_CKey, typename tf_CCompare>
			inline_medium CNode* f_FindEqual(const tf_CKey &_Key, tf_CCompare &&_Compare) const;
			template <typename tf_CKey>
			inline_small CNode* f_FindEqual(const tf_CKey &_Key) const;

			template <typename tf_CKey>
			inline_small CNode* f_FindSmallestGreaterThanEqual(const tf_CKey &_Key) const;
			template <typename tf_CKey, typename tf_CCompare>
			inline_large CNode* f_FindSmallestGreaterThanEqual(const tf_CKey &_Key, tf_CCompare &&_Compare) const;

			template <typename tf_CKey>
			inline_small CNode* f_FindSmallestGreaterThanEqualAndPrev(const tf_CKey &_Key, CNode* &_pPrev) const;
			template <typename tf_CKey, typename tf_CCompare>
			inline_large CNode* f_FindSmallestGreaterThanEqualAndPrev(const tf_CKey &_Key, CNode* &_pPrev, tf_CCompare &&_Compare) const;

			template <typename tf_CKey>
			inline_small CNode* f_FindLargestLessThanEqual(const tf_CKey &_Key) const;
			template <typename tf_CKey, typename tf_CCompare>
			inline_large CNode* f_FindLargestLessThanEqual(const tf_CKey &_Key, tf_CCompare &&_Compare) const;

			/***************************************************************************************************\
			|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
			| Debug																								|
			|___________________________________________________________________________________________________|
			\***************************************************************************************************/

			bint f_CheckTree(bint _bBreak);
			template <typename tf_CCompare>
			bint f_CheckTree(bint _bBreak, tf_CCompare &&_Compare);

			/************************************************************************************************\
			||¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯||
			|| Iterator
			||______________________________________________________________________________________________||
			\************************************************************************************************/

			template 
			<
				aint _RecursionDepth = ((sizeof(void *) * 12) - DMibGetHighestBitSet(sizeof(CLink)) + 1) // Depth of perfect tree * 1.5 approximation of (1.44*Log2(n+2) - 1)
			>
			class TIterator
			{
				friend class TCAVLTreeAggregate;
			public:
				using CNode = typename TCAVLTreeAggregate::CNode;
				
			protected:
				aint m_iStack;
				const CLink *m_pStack[_RecursionDepth];

#ifdef DMibDebuggerHelpers
				static TCAVLTreeAggregate* ms_pTree;
#endif
			public:

				inline_small TIterator();
				inline_medium TIterator(const TIterator &_Tree);
				inline_small TIterator(const TCAVLTreeAggregate &_Tree);
				inline_small TIterator(const TCAVLTreeAggregate *_pTree);


				inline_medium void f_Debug_CheckStack();

				inline_medium mint f_GetLen() const;

				void f_Clear();

				inline_small CNode *f_GetCurrent() const;

				inline_medium void f_Next();
				inline_medium void f_Prev();

				inline_medium void f_NextBidirectional();
				inline_medium void f_PrevBidirectional();

				inline_small void f_StartForward(const TCAVLTreeAggregate &_Tree);
				inline_medium void f_StartForward(const TCAVLTreeAggregate *_pTree);

				inline_small void f_StartBackward(const TCAVLTreeAggregate &_Tree);
				inline_medium void f_StartBackward(const TCAVLTreeAggregate *_pTree);

				void f_InitForSearch(const TCAVLTreeAggregate &_Tree);
				void f_InitForSearch(const TCAVLTreeAggregate *_pTree);

				template <typename tf_CKey, typename tf_CCompare>
				bint f_FindEqualForward(const tf_CKey &_Key, tf_CCompare &&_Compare);
				template <typename tf_CKey>
				bint f_FindEqualForward(const tf_CKey &_Key);

				template <typename tf_CKey, typename tf_CCompare>
				bint f_FindSmallestGreaterThanEqualForward(const tf_CKey &_Key, tf_CCompare &&_Compare);
				template <typename tf_CKey>
				bint f_FindSmallestGreaterThanEqualForward(const tf_CKey &_Key);

				template <typename tf_CKey, typename tf_CCompare>
				bint f_FindLargestLessThanEqualForward(const tf_CKey &_Key, tf_CCompare &&_Compare);
				template <typename tf_CKey>
				bint f_FindLargestLessThanEqualForward(const tf_CKey &_Key);

				template <typename tf_CCompare>
				void f_Delete(TCAVLTreeAggregate &_Tree, tf_CCompare &&_Compare);
				void f_Delete(TCAVLTreeAggregate &_Tree);

				template <typename tf_CCompare, typename tf_CAllocator>
				void f_DeleteAllocator(TCAVLTreeAggregate &_Tree, tf_CCompare &&_Compare, tf_CAllocator &_Allocator);
				template <typename tf_CAllocator>
				void f_DeleteAllocator(TCAVLTreeAggregate &_Tree, tf_CAllocator &_Allocator);

				template <typename tf_CCompare, typename tf_CAllocator>
				void f_DeleteAllocatorDefiniteType(TCAVLTreeAggregate &_Tree, tf_CCompare &&_Compare, tf_CAllocator &_Allocator);
				template <typename tf_CAllocator>
				void f_DeleteAllocatorDefiniteType(TCAVLTreeAggregate &_Tree, tf_CAllocator &_Allocator);

				template <typename tf_CCompare>
				void f_Remove(TCAVLTreeAggregate &_Tree, tf_CCompare &&_Compare);
				void f_Remove(TCAVLTreeAggregate &_Tree);

				inline_small operator CNode *() const;
				inline_small CNode * operator ->() const;
				inline_small void operator ++();
				inline_small void operator --();
			};
			
			typedef TIterator<> CIterator;
			typedef TIterator<> CIteratorConst;

			CIterator f_GetIterator();
			CIteratorConst f_GetIterator() const;
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
			typename t_CLinkTraits
			, typename t_CCompare = CSort_Default
			, typename t_CAllocator = NMib::NMem::CDefaultAllocator
		>
		class TCAVLTree : public TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>
		{
			DMibClassNoCopyAllowed(TCAVLTree);

			typedef TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator> CSuper;
		public:
			TCAVLTree(TCAVLTree &&_Other);
			TCAVLTree();
			~TCAVLTree();
			TCAVLTree &operator = (TCAVLTree &&_Other);

			bint operator == (const TCAVLTree &_Other) const;
			bint operator < (const TCAVLTree &_Other) const;
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
			CAVLLinkTypeUnderlaying _LinkType = EAVLLinkType_Aligned
			, typename t_CLinkData = typename NPrivate::TCLinkTypeToLink<_LinkType>::CType
		>
		class TCAVLLinkAggregate
		{
		public:
			typedef t_CLinkData CLink;
			t_CLinkData m_Link;

#ifdef DCompiler_clang

			constexpr TCAVLLinkAggregate()
				: m_Link()
			{
			}
#endif
			
			inline_small void f_Construct();
			template <typename tf_CTree>
			inline_small void f_Destruct(tf_CTree *_pTree);
			inline_small void f_Destruct();
			bint f_IsInTree() const;
			template <typename tf_CTree>
			inline_small void f_SetTree(tf_CTree *_pTree);
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
			CAVLLinkTypeUnderlaying _LinkType = EAVLLinkType_Aligned
			, typename t_CLinkData = typename NPrivate::TCLinkTypeToLink<_LinkType>::CType
		>
		class TCAVLLink : public TCAVLLinkAggregate<_LinkType, t_CLinkData>
		{
		private:
			DMibClassNoCopyAllowed(TCAVLLink);
		public:
			TCAVLLink();
			~TCAVLLink();
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
			typename t_CCompare = CSort_Default
			, typename t_CAllocator = NMib::NMem::CDefaultAllocator
			, CAVLLinkTypeUnderlaying _LinkType = EAVLLinkType_Aligned
			, typename t_CLinkData = typename NPrivate::TCLinkTypeToLink<_LinkType>::CType
			, typename t_CLinkTraits = void
		>
		class TCAVLLinkAutoRemoveAggregate
		{
		public:
			typedef t_CLinkData CLink;
			CLink m_Link;
			TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator> *m_pTree;

			inline_small void f_Construct();
			inline_small void f_Destruct();
			bint f_IsInTree() const;
			inline_small void f_SetTree(TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator> *_pTree);
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
			typename t_CCompare = CSort_Default
			, typename t_CAllocator = NMib::NMem::CDefaultAllocator
			, CAVLLinkTypeUnderlaying _LinkType = EAVLLinkType_Aligned
			, typename t_CLinkData = typename NPrivate::TCLinkTypeToLink<_LinkType>::CType
			, typename t_CLinkTraits = void
		>
		class TCAVLLinkAutoRemove : public TCAVLLinkAutoRemoveAggregate<t_CCompare, t_CAllocator, _LinkType, t_CLinkData, t_CLinkTraits>
		{
		private:
			DMibClassNoCopyAllowed(TCAVLLinkAutoRemove);
		public:
			TCAVLLinkAutoRemove();
			~TCAVLLinkAutoRemove();
		};

		/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
		|	Template Class:																					|
		|																									|
		|	Description:		Used to replace the link traits in an auto remove AVL link					|
		|																									|
		|	Comments:			This is needed because the link traits is needed inside a auto remove		|
		|						AVL link as it needs to know the type oth the tree it's a member of,		|
		|						but these traits are not known when the DMibIntrusiveLink macro is used.	|
		|																									|
		|	Location:			Template Classes															|
		|																									|
		|	Index:				!name																		|
		\*_________________________________________________________________________________________________*/
		template 
		<
			typename t_CLinkTraits
			, typename t_CCompare
			, typename t_CAllocator
			, CAVLLinkTypeUnderlaying _LinkType
			, typename t_CLinkData
			, typename t_COldLinkTraits
		>
		struct TCReplaceLinkTraits<TCAVLLinkAutoRemove<t_CCompare, t_CAllocator, _LinkType, t_CLinkData, t_COldLinkTraits>, t_CLinkTraits>
		{
			typedef TCAVLLinkAutoRemove<t_CCompare, t_CAllocator, _LinkType, t_CLinkData, t_CLinkTraits> CType;
		};
	}
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
#include "Malterlib_Intrusive_AVLTree_Stream.h"
#endif

#include "Malterlib_Intrusive_AVLTree_CompareDefaults.h"


