// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib
{
	namespace NIntrusive
	{
		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Private																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_medium typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CLinkPointer *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_RemoveRebalance(CLinkPointer *_pTop, CLinkPointer *_pTarget, CLink *_pObjectToRemove, tf_CCompare &&_Compare)
		{
			// each node from treep down towards target, but
			// excluding the last, will have a subtree grow
			// and need rebalancing

			CLink *pTarget = CLink::fs_GetPtr(*_pTarget);
			CLinkPointer *pObject = _pTop;
			CLink *pObj = CLink::fs_GetPtr(*pObject);

			while (1)
			{
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToRemove)))
				{
					if (!pObj->f_GetRightP())
						break;
					if (pObj->f_IsBalanced())
						pObj->f_SetSkew(0);
					else if (pObj->f_GetSkew() == 1)
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
					else
					{
						if (pObj->f_GetLeftP()->f_GetSkew() == 1)
							fp_RemoveRotate3<1>(pObject);
						else
							fp_RemoveRotate2<1>(pObject);
						if (pObj == pTarget)
							_pTarget = &(CLink::fs_GetPtr(*pObject)->f_GetRight());
					}
					pObject = &pObj->f_GetRight();
					pObj = CLink::fs_GetPtr(*pObject);
				}
				else
				{
					if (!pObj->f_GetLeftP())
						break;
					if (pObj->f_IsBalanced())
							pObj->f_SetSkew(1);
						else if (pObj->f_GetSkew() == 0)
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
					else
					{
							if (pObj->f_GetRightP()->f_GetSkew() == 0)
								fp_RemoveRotate3<0>(pObject);
						else
								fp_RemoveRotate2<0>(pObject);
						if (pObj == pTarget)
								_pTarget = &(CLink::fs_GetPtr(*pObject)->f_GetLeft());
					}
						pObject = &pObj->f_GetLeft();
					pObj = CLink::fs_GetPtr(*pObject);
				}

			}

			return _pTarget;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <int t_Direction>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CLink *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_Rotate2(CLinkPointer *_pTop)
		{
			CLink *pBP;
			CLink *pCP;
			CLink *pDP;

			pBP = CLink::fs_GetPtr(*_pTop);
			pDP = pBP->f_GetNextP(t_Direction);
			pCP = pDP->f_GetNextP(1-t_Direction);
			CLink::f_Assign(*_pTop, pDP);
			pDP->fp_SetNext(1-t_Direction, pBP);
			pBP->fp_SetNext(t_Direction, pCP);
			pBP->f_SetSkew(CLink::EAVLTreeSkew_None);
			pDP->f_SetSkew(CLink::EAVLTreeSkew_None);
			return pDP->f_GetNextP(t_Direction);
		}


		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_Rotate3Short(CLinkPointer *_pTop, int t_Direction)
		{
			CLink *pBP;
			CLink *pCP;
			CLink *pDP;
			CLink *pEP;
			CLink *pFP;

			pBP = CLink::fs_GetPtr(*_pTop);
			pFP = pBP->f_GetNextP(t_Direction);
			pDP = pFP->f_GetNextP(1-t_Direction);

			pCP = pDP->f_GetNextP(1-t_Direction);
			pEP = pDP->f_GetNextP(t_Direction);

			CLink::f_Assign(*_pTop, pDP);

			pDP->fp_SetNext(1-t_Direction, pBP);
			pDP->fp_SetNext(t_Direction, pFP);
			pBP->fp_SetNext(t_Direction, pCP);
			pFP->fp_SetNext(1-t_Direction, pEP);

			pDP->f_SetSkew(CLink::EAVLTreeSkew_None);
			pBP->f_SetSkew(CLink::EAVLTreeSkew_None);
			pFP->f_SetSkew(CLink::EAVLTreeSkew_None);
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CLink *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_Rotate3(CLinkPointer *_pTop, int t_Direction, int t_Third)
		{
			CLink *pBP;
			CLink *pCP;
			CLink *pDP;
			CLink *pEP;
			CLink *pFP;

			pBP = CLink::fs_GetPtr(*_pTop);
			pFP = pBP->f_GetNextP(t_Direction);
			pDP = pFP->f_GetNextP(1-t_Direction);

			pCP = pDP->f_GetNextP(1-t_Direction);
			pEP = pDP->f_GetNextP(t_Direction);

			CLink::f_Assign(*_pTop, pDP);

			pDP->fp_SetNext(1-t_Direction, pBP);
			pDP->fp_SetNext(t_Direction, pFP);
			pBP->fp_SetNext(t_Direction, pCP);
			pFP->fp_SetNext(1-t_Direction, pEP);

			pDP->f_SetSkew(CLink::EAVLTreeSkew_None);

//				CLink *pTree0 = pDP->f_GetNextP(_Direction);
//				CLink *pTree1 = pDP->f_GetNextP(1-_Direction);

			if (t_Third == t_Direction)
			{
				pBP->f_SetSkew(1-t_Direction);
				pFP->f_SetSkew(CLink::EAVLTreeSkew_None);
				return pFP->f_GetNextP(1-t_Direction);
			}
			else
			{
				pBP->f_SetSkew(CLink::EAVLTreeSkew_None);
				pFP->f_SetSkew(t_Direction);
				return pBP->f_GetNextP(t_Direction);
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_RemoveLowStack(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_CCompare &&_Compare)
		{
			CLinkPointer *pObject = &_pObject;
			CLinkPointer *pPathTop = pObject;
			CLinkPointer *pTarget = nullptr;
			CLink *pObj = CLink::fs_GetPtr(*pObject);

			int Dir = 0;
			while (pObj)
			{
				if (pObj == _pObjectToRemove)
					pTarget = pObject;

				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToRemove)))
				{
					if (!pObj->f_GetRightP())
					{
						Dir = 1;
						break;
					}
					if (pObj->f_IsBalanced() || (pObj->f_GetSkew() == 0 && pObj->f_GetLeftP()->f_IsBalanced()))
					pPathTop = pObject;
					pObject = &pObj->f_GetRight();
				}
				else
				{
					if (!pObj->f_GetLeftP())
					{
						Dir = 0;
						break;
					}
					if (pObj->f_IsBalanced() || (pObj->f_GetSkew() == 1 && pObj->f_GetRightP()->f_IsBalanced()))
						pPathTop = pObject;
					pObject = &pObj->f_GetLeft();
				}
				pObj = CLink::fs_GetPtr(*pObject);
			}

			DMibFastCheck(pTarget); // Target not found in tree

			pTarget = fp_RemoveRebalance(pPathTop, pTarget, _pObjectToRemove, fg_Forward<tf_CCompare>(_Compare));

			CLink *pObjDel = CLink::fs_GetPtr(*pTarget);
			CLink::f_Assign(*pTarget, pObj);
			CLink::f_Assign(*pObject, pObj->f_GetNextP(1-Dir));

			pObj->f_SetLeft(pObjDel->f_GetLeft());
			pObj->f_SetRight(pObjDel->f_GetRight());
			pObj->f_SetSkew(pObjDel->f_GetSkew());

			pObjDel->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_RebalancePathLowStack(CLink *_pPath, CLink *_pObjectToInsert, tf_CCompare &&_Compare)
		{
			// Each node in path is currently balanced.
			// Until we find target, mark each node as longer
			// in the direction of target because we know we have
			// inserted target there
			CLink *pPath = _pPath;

			while (pPath && pPath != _pObjectToInsert) 
			{
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pPath), *fsp_MemberFromLink(_pObjectToInsert)))
				{
					pPath->f_SetSkew(1);
					pPath = pPath->f_GetRightP();
				}
				else
				{
					pPath->f_SetSkew(0);
					pPath = pPath->f_GetLeftP();
				}
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_RebalanceCase3LowStack(int _Direction, CLink *pPath, CLinkPointer *_pTop, CLink *_pObjectToInsert, tf_CCompare &&_Compare)
		{
			CLink *pObj = pPath;
			if (pObj == _pObjectToInsert)
			{
				fp_Rotate3Short(_pTop, _Direction);
			}
			else
			{
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert)))
					pObj = fp_Rotate3(_pTop, _Direction, 1);
				else
					pObj = fp_Rotate3(_pTop, _Direction, 0);
				fp_RebalancePathLowStack(pObj, _pObjectToInsert, fg_Forward<tf_CCompare>(_Compare));
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_RebalanceLowStack(CLinkPointer *_pTop, CLink *_pObjectToInsert, tf_CCompare &&_Compare)
		{
			CLink *pObj = CLink::fs_GetPtr(*_pTop);
			if (pObj->f_IsBalanced())
			{
				if (pObj == _pObjectToInsert)
					return;
			}
			else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert)))
			{
				if (!pObj->f_GetSkew())
				{
					pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
					pObj = pObj->f_GetRightP();
				}
				else
				{
					pObj = pObj->f_GetRightP();
					if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert)))
					{
						pObj = fp_Rotate2<1>(_pTop);
					}
					else
					{
						fp_RebalanceCase3LowStack(1, pObj->f_GetLeftP(), _pTop, _pObjectToInsert, fg_Forward<tf_CCompare>(_Compare));
						return;
					}
				}
			}
			else
			{
				if (pObj->f_GetSkew())
				{
					pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
					pObj = pObj->f_GetLeftP();
				}
				else
				{
					pObj = pObj->f_GetLeftP();
					if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(_pObjectToInsert), *fsp_MemberFromLink(pObj)))
					{
						pObj = fp_Rotate2<0>(_pTop);
					}
					else
					{
						fp_RebalanceCase3LowStack(0,pObj->f_GetRightP(), _pTop, _pObjectToInsert, fg_Forward<tf_CCompare>(_Compare));
						return;
					}
				}
			}

			fp_RebalancePathLowStack(pObj, _pObjectToInsert, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fp_InsertLowStack(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_CCompare &&_Compare)
		{
			// insert the target into the tree, returning 1 on success or 0 if it
			// already existed
				
			CLinkPointer *pTree = &_pObject;
			CLinkPointer *pPathTop = pTree;
			CLink *pObj = CLink::fs_GetPtr(*pTree);
			while (pObj)
			{
					
				if (pObj->f_GetSkew() != CLink::EAVLTreeSkew_None)
					pPathTop = pTree;
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert)))
				{
					pTree = &(pObj->f_GetRight());
					pObj = CLink::fs_GetPtr(*pTree);
				}
				else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(_pObjectToInsert), *fsp_MemberFromLink(pObj)))
				{
					pTree = &(pObj->f_GetLeft());
					pObj = CLink::fs_GetPtr(*pTree);
				}
				else
				{
					DMibFastCheck(false); // Tree does not support inserting two equal objects
					return false;
				}
			}
			_pObjectToInsert->f_Clear();
			CLink::f_Assign(*pTree, _pObjectToInsert);
			fp_RebalanceLowStack(pPathTop, _pObjectToInsert, fg_Forward<tf_CCompare>(_Compare));
			return true;
		}

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Public																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_InsertLowStack(CNode *_pToInsert, tf_CCompare &&_Compare)
		{
			CLink * pToInsert = fsp_LinkFromMember(_pToInsert);
			((CLinkContainer *)pToInsert)->f_SetTree(this);
			DMibFastCheck(pToInsert->f_GetSkew() == CLink::EAVLTreeSkew_NotInTree); // Must not be in tree already
				
			return fp_InsertLowStack(m_Root, pToInsert, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_RemoveLowStack(CNode *_pToRemove, tf_CCompare &&_Compare)
		{
			CLink * pToRemove = fsp_LinkFromMember(_pToRemove);

			DMibFastCheck(pToRemove->f_GetSkew() != CLink::EAVLTreeSkew_NotInTree); // Must be in tree already

			fp_RemoveLowStack(m_Root, pToRemove, fg_Forward<tf_CCompare>(_Compare));
#			ifdef DMibEnableSafeCheck
				// Remove tree ptr in debug
				((CLinkContainer *)pToRemove)->f_SetTree((TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator> *)nullptr);
#			endif

		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_InsertLowStack(CNode &_ToInsert, tf_CCompare &&_Compare)
		{
			return f_InsertLowStack(&_ToInsert, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_RemoveLowStack(CNode &_ToRemove, tf_CCompare &&_Compare)
		{
			f_RemoveLowStack(&_ToRemove, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_InsertLowStack(CNode &_ToInsert) 
		{
			return f_InsertLowStack(&_ToInsert, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_InsertLowStack(CNode *_pToInsert)
		{
			return f_InsertLowStack(_pToInsert, t_CCompare());
		}
			
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_RemoveLowStack(CNode &_ToRemove)
		{
			f_RemoveLowStack(&_ToRemove, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_RemoveLowStack(CNode *_pToRemove)
		{
			f_RemoveLowStack(_pToRemove, t_CCompare());
		}

	}
}

