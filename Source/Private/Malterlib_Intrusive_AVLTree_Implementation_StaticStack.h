// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib
{
	namespace NIntrusive
	{
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_medium bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_BalanceLowest(CLink* &_pLowestObject, CLinkPointer &_pObject, CStackObj *&_pStack)
		{
			CStackObj *pStack = _pStack;

			CLinkPointer *pObject = &_pObject;

			CLink *pObj = CLink::fs_GetPtr(_pObject);
			while (pObj->f_GetLeftP())
			{
				pStack->f_SetAll(pObject, 0);
				++pStack;
				pObject = &(pObj->f_GetLeft());
				pObj = CLink::fs_GetPtr(*pObject);
			}

			// Save the object that we want at targets place
			_pLowestObject = pObj;
			// Remove pObject from the tree
			CLink::f_Assign(*pObject, pObj->f_GetRight());
			//_pStack = pStack;
				
			while (pStack - _pStack)
			{
				--pStack;
				if (!fsp_LeftShrunk(*pStack->f_GetStack()))
					return false;					
			}
				
			return true;
		}
			
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_medium bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_BalanceHighest(CLink* &_pHighestObject, CLinkPointer &_pObject, CStackObj *&_pStack)
		{
			CStackObj *pStack = _pStack;

			CLinkPointer *pObject = &_pObject;

			CLink *pObj = CLink::fs_GetPtr(_pObject);
			while (pObj->f_GetRightP())
			{
				pStack->f_SetAll(pObject, 1);
				++pStack;
				pObject = &(pObj->f_GetRight());
				pObj = CLink::fs_GetPtr(*pObject);
			}

			// Save the object that we want at targets place
			_pHighestObject = pObj;
			// Remove pObject from the tree
			CLink::f_Assign(*pObject, pObj->f_GetLeft());
		//	_pStack = pStack;

			while (pStack - _pStack)
			{
				--pStack;
				if (!fsp_RightShrunk(*pStack->f_GetStack()))
					return false;					
			}
				
			return true;

		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_Remove(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_CCompare &&_Compare)
		{
			const int Size = ((sizeof(void *) * 12) - DMibGetHighestBitSet(sizeof(CLink)) + 1);
			CStackObj Stack[Size]; // Depth of perfect tree * 1.5 approximation of (1.44*Log2(n+2) - 1)
			CStackObj *pStack = Stack;

			CLinkPointer *pObject = &_pObject;

			CLink *pObj = CLink::fs_GetPtr(*pObject);
			while (pObj != _pObjectToRemove)
			{
				DMibFastCheck(pObj); // Object not found

				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToRemove)))
				{
					pStack->f_SetAll(pObject, true);
					++pStack;
					pObject = &pObj->f_GetRight();
				}
				else
				{
					pStack->f_SetAll(pObject, false);
					++pStack;
					pObject = &pObj->f_GetLeft();
				}
				pObj = CLink::fs_GetPtr(*pObject);
			}

			{
				if (pObj->f_GetLeftP()) 
				{
					CLink* pHighestObject;
					bint bLeftShrunk = fp_BalanceHighest(pHighestObject, pObj->f_GetLeft(), pStack);

					// Remove target from tree
					pHighestObject->f_SetSkew(pObj->f_GetSkew());
					pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
					// Remove from last place
					// Link in on targets place
					pHighestObject->f_SetLeft(pObj->f_GetLeft());
					pHighestObject->f_SetRight(pObj->f_GetRight());
					CLink::f_Assign(*pObject, pHighestObject);

					if (!bLeftShrunk)
						return;
					pStack->f_SetAll(pObject, 0);
					++pStack;
				}
				else if (pObj->f_GetRightP()) 
				{
					CLink* pLowestObject;

					bint bRightShrunk = fp_BalanceLowest(pLowestObject, pObj->f_GetRight(), pStack);

					// Remove target from tree
					pLowestObject->f_SetSkew(pObj->f_GetSkew());
					pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
					// Link in on targets place
					pLowestObject->f_SetLeft(pObj->f_GetLeft());
					pLowestObject->f_SetRight(pObj->f_GetRight());
					CLink::f_Assign(*pObject, pLowestObject);

					if (!bRightShrunk)
						return;
					pStack->f_SetAll(pObject, 1);
					++pStack;
				}
				else
				{
					pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);

					CLink::f_Assign(*pObject, (CLink *)nullptr);
				}
			}

			while (pStack - Stack)
			{
				--pStack;
				if (pStack->f_IsLarger())
				{
					if (!fsp_RightShrunk(*(pStack->f_GetStack())))
						break;
				}
				else
				{
					if (!fsp_LeftShrunk(*(pStack->f_GetStack())))
						break;
				}
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <int tf_Direction>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_RemoveRotate3(CLinkPointer *_pObject)
		{
			CLink *pD;
			CLink *pB;
			CLink *pC;
			CLink *pE;
			CLink *pF;

			pF = CLink::fs_GetPtr(*_pObject);
			pB = pF->f_GetNextP(1-tf_Direction);
			pD = pB->f_GetNextP(tf_Direction);
			pC = pD->f_GetNextP(1-tf_Direction);
			pE = pD->f_GetNextP(tf_Direction);

			CLink::f_Assign(*_pObject, pD);

			pD->fp_SetNext(1-tf_Direction, pB);
			pD->fp_SetNext(tf_Direction, pF);
			pB->fp_SetNext(tf_Direction, pC);
			pF->fp_SetNext(1-tf_Direction, pE);

			pB->f_SetSkew(CLink::EAVLTreeSkew_None);
			pF->f_SetSkew(CLink::EAVLTreeSkew_None);
			int DSkew = pD->f_GetSkew();
			if (DSkew == tf_Direction)
				pB->f_SetSkew(1-tf_Direction);
			else if (DSkew == 1-tf_Direction)
				pF->f_SetSkew(tf_Direction);
			pD->f_SetSkew(CLink::EAVLTreeSkew_None);

		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <int tf_Direction>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_RemoveRotate2(CLinkPointer *_pObject)
		{
			CLink *pD;
			CLink *pB;
			CLink *pC;

			pD = CLink::fs_GetPtr(*_pObject);
			pB = pD->f_GetNextP(1-tf_Direction);
			pC = pB->f_GetNextP(tf_Direction);

			CLink::f_Assign(*_pObject, pB);
			pB->fp_SetNext(tf_Direction, pD);
			pD->fp_SetNext(1-tf_Direction, pC);

			if (pB->f_IsBalanced())
			{
				pB->f_SetSkew(tf_Direction);
				pD->f_SetSkew(1-tf_Direction);
			}
			else
			{
				pB->f_SetSkew(CLink::EAVLTreeSkew_None);
				pD->f_SetSkew(CLink::EAVLTreeSkew_None);
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_Insert(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_CCompare &&_Compare)
		{
			const int Size = ((sizeof(void *) * 12) - DMibGetHighestBitSet(sizeof(CLink)) + 1);
			CStackObj Stack[Size]; // Depth of perfect tree * 1.5 approximation of (1.44*Log2(n+2) - 1)
			CStackObj *pStack = Stack;

			CLinkPointer *pObject = &_pObject;
//				int iStack = 0;
			CLink *pObj = CLink::fs_GetPtr(*pObject);

			while (pObj)
			{
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert)))
				{
					pStack->f_SetAll(pObject, true);
					++pStack;
					pObject = &pObj->f_GetRight();
				}
				else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(_pObjectToInsert), *fsp_MemberFromLink(pObj)))
				{
					pStack->f_SetAll(pObject, false);
					++pStack;
					pObject = &pObj->f_GetLeft();
				}
				else
				{
					DMibFastCheck(false); // Tree does not support inserting two equal objects
					return false;
				}
				pObj = CLink::fs_GetPtr(*pObject);
			}

			_pObjectToInsert->f_Clear();
			CLink::f_Assign(*pObject, _pObjectToInsert);
			while (pStack - Stack)
			{
				--pStack;
//					const CStackObj &StackObj = Stack[iStack];
				if (pStack->f_IsLarger())
				{
					if (!fsp_RightGrown_Inl(*(pStack->f_GetStack())))
						break;
				}
				else
				{
					if (!fsp_LeftGrown_Inl(*(pStack->f_GetStack())))
						break;
				}
			}
			return true;
		}

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Public																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Insert(CNode *_pToInsert, tf_CCompare &&_Compare)
		{
			CLink * pToInsert = fsp_LinkFromMember(_pToInsert);
			((CLinkContainer *)pToInsert)->f_SetTree(this);
			DMibFastCheck(pToInsert->f_GetSkew() == CLink::EAVLTreeSkew_NotInTree); // Must not be in tree already
				
			return fp_Insert(m_Root, pToInsert, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CToMap, typename tf_CCompare>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Map(tf_CToMap &_ToMap, tf_CCompare &&_Compare)
		{
			CNode *pData = f_FindEqual(_ToMap);
			if (pData)
				return pData;

			auto Memory = CAllocator::f_AllocSafe(sizeof(CNode), NTraits::TCAlignmentOf<CNode>::mc_Value);
			pData = new(Memory.m_pMemory) CNode(_ToMap);
			Memory.f_Claim();
			f_Insert(pData, fg_Forward<tf_CCompare>(_Compare));
			return pData;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Insert(CNode &_ToInsert) 
		{
			return f_Insert(_ToInsert, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Insert(CNode &_ToInsert, tf_CCompare &&_Compare)
		{
			return f_Insert(&_ToInsert, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Insert(CNode *_pToInsert)
		{
			return f_Insert(_pToInsert, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CToMap>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Map(tf_CToMap &_ToMap)
		{
			return f_Map(_ToMap, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Remove(CNode *_pToRemove, tf_CCompare &&_Compare)
		{
			CLink * pToRemove = fsp_LinkFromMember(_pToRemove);

			fp_Remove(m_Root, pToRemove, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Remove(CNode &_ToRemove, tf_CCompare &&_Compare)
		{
			f_Remove(&_ToRemove, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Remove(CNode &_ToRemove)
		{
			f_Remove(_ToRemove, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Remove(CNode *_pToRemove)
		{
			f_Remove(_pToRemove, t_CCompare());
		}

	}
}

