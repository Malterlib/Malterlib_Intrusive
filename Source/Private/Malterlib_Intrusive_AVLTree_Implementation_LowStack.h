// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NIntrusive
{
	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Private																							|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_medium typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CLinkPointer *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RemoveRebalance
		(
			CLinkPointer *_pTop
			, CLinkPointer *_pTarget
			, CLink *_pObjectToRemove
			, tf_CCompare &&_fCompare
		)
	{
		// each node from treep down towards target, but
		// excluding the last, will have a subtree grow
		// and need rebalancing

		CLink *pTarget = CLink::fs_GetPtr(*_pTarget);
		CLinkPointer *pObject = _pTop;
		CLink *pObj = CLink::fs_GetPtr(*pObject);

		while (1)
		{
			if (fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToRemove)) < 0)
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
						_pTarget = CLink::fs_GetPtr(*pObject)->f_GetRight();
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
							_pTarget = CLink::fs_GetPtr(*pObject)->f_GetLeft();
				}
					pObject = &pObj->f_GetLeft();
				pObj = CLink::fs_GetPtr(*pObject);
			}

		}

		return _pTarget;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <int t_Direction>
	inline_small typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CLink *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_Rotate2(CLinkPointer *_pTop)
	{
		CLink *pBP;
		CLink *pCP;
		CLink *pDP;

		pBP = CLink::fs_GetPtr(*_pTop);
		pDP = pBP->f_GetNextP(t_Direction);
		pCP = pDP->f_GetNextP(1-t_Direction);
		CLink::f_Assign(_pTop, pDP);
		pDP->fp_SetNext(1-t_Direction, pBP);
		pBP->fp_SetNext(t_Direction, pCP);
		pBP->f_SetSkew(CLink::EAVLTreeSkew_None);
		pDP->f_SetSkew(CLink::EAVLTreeSkew_None);
		return pDP->f_GetNextP(t_Direction);
	}


	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_Rotate3Short(CLinkPointer *_pTop, int _Direction)
	{
		CLink *pBP;
		CLink *pCP;
		CLink *pDP;
		CLink *pEP;
		CLink *pFP;

		pBP = CLink::fs_GetPtr(*_pTop);
		pFP = pBP->f_GetNextP(_Direction);
		pDP = pFP->f_GetNextP(1-_Direction);

		pCP = pDP->f_GetNextP(1-_Direction);
		pEP = pDP->f_GetNextP(_Direction);

		CLink::f_Assign(_pTop, pDP);

		pDP->fp_SetNext(1-_Direction, pBP);
		pDP->fp_SetNext(_Direction, pFP);
		pBP->fp_SetNext(_Direction, pCP);
		pFP->fp_SetNext(1-_Direction, pEP);

		pDP->f_SetSkew(CLink::EAVLTreeSkew_None);
		pBP->f_SetSkew(CLink::EAVLTreeSkew_None);
		pFP->f_SetSkew(CLink::EAVLTreeSkew_None);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	typename TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CLink *
	TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_Rotate3(CLinkPointer *_pTop, int _Direction, int _Third)
	{
		CLink *pBP;
		CLink *pCP;
		CLink *pDP;
		CLink *pEP;
		CLink *pFP;

		pBP = CLink::fs_GetPtr(*_pTop);
		pFP = pBP->f_GetNextP(_Direction);
		pDP = pFP->f_GetNextP(1-_Direction);

		pCP = pDP->f_GetNextP(1-_Direction);
		pEP = pDP->f_GetNextP(_Direction);

		CLink::f_Assign(_pTop, pDP);

		pDP->fp_SetNext(1-_Direction, pBP);
		pDP->fp_SetNext(_Direction, pFP);
		pBP->fp_SetNext(_Direction, pCP);
		pFP->fp_SetNext(1-_Direction, pEP);

		pDP->f_SetSkew(CLink::EAVLTreeSkew_None);

//				CLink *pTree0 = pDP->f_GetNextP(_Direction);
//				CLink *pTree1 = pDP->f_GetNextP(1-_Direction);

		if (_Third == _Direction)
		{
			pBP->f_SetSkew(1-_Direction);
			pFP->f_SetSkew(CLink::EAVLTreeSkew_None);
			return pFP->f_GetNextP(1-_Direction);
		}
		else
		{
			pBP->f_SetSkew(CLink::EAVLTreeSkew_None);
			pFP->f_SetSkew(_Direction);
			return pBP->f_GetNextP(_Direction);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RemoveLowStack(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_CCompare &&_fCompare)
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

			if (fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToRemove)) < 0)
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

		pTarget = fp_RemoveRebalance(pPathTop, pTarget, _pObjectToRemove, _fCompare);

		CLink *pObjDel = CLink::fs_GetPtr(*pTarget);
		CLink::f_Assign(pTarget, pObj);
		CLink::f_Assign(pObject, pObj->f_GetNextP(1-Dir));

		pObj->f_SetLeft(pObjDel->f_GetLeft());
		pObj->f_SetRight(pObjDel->f_GetRight());
		pObj->f_SetSkew(pObjDel->f_GetSkew());

		pObjDel->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RebalancePathLowStack
		(
			CLink *_pPath
			, CLink *_pObjectToInsert
			, tf_CCompare &&_fCompare
		)
	{
		// Each node in path is currently balanced.
		// Until we find target, mark each node as longer
		// in the direction of target because we know we have
		// inserted target there
		CLink *pPath = _pPath;

		while (pPath && pPath != _pObjectToInsert)
		{
			if (fsp_Compare(_fCompare, *fsp_MemberFromLink(pPath), *fsp_MemberFromLink(_pObjectToInsert)) < 0)
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

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RebalanceCase3LowStack
		(
			int _Direction
			, CLink *pPath
			, CLinkPointer *_pTop
			, CLink *_pObjectToInsert
			, tf_CCompare &&_fCompare
		)
	{
		CLink *pObj = pPath;
		if (pObj == _pObjectToInsert)
		{
			fp_Rotate3Short(_pTop, _Direction);
		}
		else
		{
			if (fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert)) < 0)
				pObj = fp_Rotate3(_pTop, _Direction, 1);
			else
				pObj = fp_Rotate3(_pTop, _Direction, 0);
			fp_RebalancePathLowStack(pObj, _pObjectToInsert, _fCompare);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RebalanceLowStack
		(
			CLinkPointer *_pTop
			, CLink *_pObjectToInsert
			, tf_CCompare &&_fCompare
		)
	{
		CLink *pObj = CLink::fs_GetPtr(*_pTop);
		if (pObj->f_IsBalanced())
		{
			if (pObj == _pObjectToInsert)
				return;
		}
		else if (fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert)) < 0)
		{
			if (!pObj->f_GetSkew())
			{
				pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
				pObj = pObj->f_GetRightP();
			}
			else
			{
				pObj = pObj->f_GetRightP();
				if (fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert)) < 0)
				{
					pObj = fp_Rotate2<1>(_pTop);
				}
				else
				{
					fp_RebalanceCase3LowStack(1, pObj->f_GetLeftP(), _pTop, _pObjectToInsert, _fCompare);
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
				if (fsp_Compare(_fCompare, *fsp_MemberFromLink(_pObjectToInsert), *fsp_MemberFromLink(pObj)) < 0)
				{
					pObj = fp_Rotate2<0>(_pTop);
				}
				else
				{
					fp_RebalanceCase3LowStack(0,pObj->f_GetRightP(), _pTop, _pObjectToInsert, _fCompare);
					return;
				}
			}
		}

		fp_RebalancePathLowStack(pObj, _pObjectToInsert, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_InsertLowStack(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_CCompare &&_fCompare)
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
			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert));
			if (CompareResult < 0)
			{
				pTree = pObj->f_GetRight();
				pObj = CLink::fs_GetPtr(*pTree);
			}
			else if (CompareResult > 0)
			{
				pTree = pObj->f_GetLeft();
				pObj = CLink::fs_GetPtr(*pTree);
			}
			else
			{
				DMibFastCheck(false); // Tree does not support inserting two equal objects
				return false;
			}
		}
		_pObjectToInsert->f_Clear();
		CLink::f_Assign(pTree, _pObjectToInsert);
		fp_RebalanceLowStack(pPathTop, _pObjectToInsert, _fCompare);
		return true;
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Public																							|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_InsertLowStack(CNode *_pToInsert, tf_CCompare &&_fCompare)
	{
		CLink * pToInsert = fsp_LinkFromMember(_pToInsert);
		if constexpr (CLinkContainer::mc_bNeedSetTree)
			((CLinkContainer *)pToInsert)->f_SetTree(this, &TCAVLTreeAggregate::fsp_Remove);
		DMibFastCheck(pToInsert->f_GetSkew() == CLink::EAVLTreeSkew_NotInTree); // Must not be in tree already

		return fp_InsertLowStack(m_Root, pToInsert, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_RemoveLowStack(CNode *_pToRemove, tf_CCompare &&_fCompare)
	{
		CLink * pToRemove = fsp_LinkFromMember(_pToRemove);

		DMibFastCheck(pToRemove->f_GetSkew() != CLink::EAVLTreeSkew_NotInTree); // Must be in tree already

		fp_RemoveLowStack(m_Root, pToRemove, _fCompare);
#			ifdef DMibEnableSafeCheck
			// Remove tree ptr in debug
			if constexpr (CLinkContainer::mc_bNeedSetTree)
				((CLinkContainer *)pToRemove)->f_SetTree(nullptr, nullptr);
#			endif

	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_InsertLowStack(CNode &_ToInsert, tf_CCompare &&_fCompare)
	{
		return f_InsertLowStack(&_ToInsert, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_RemoveLowStack(CNode &_ToRemove, tf_CCompare &&_fCompare)
	{
		f_RemoveLowStack(&_ToRemove, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_InsertLowStack(CNode &_ToInsert)
	{
		return f_InsertLowStack(&_ToInsert, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_InsertLowStack(CNode *_pToInsert)
	{
		return f_InsertLowStack(_pToInsert, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_RemoveLowStack(CNode &_ToRemove)
	{
		f_RemoveLowStack(&_ToRemove, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_RemoveLowStack(CNode *_pToRemove)
	{
		f_RemoveLowStack(_pToRemove, t_CCompare());
	}
}
