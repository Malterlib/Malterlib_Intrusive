// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NIntrusive
{
	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Aggregate private																					|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	bint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fpr_CheckTree(CLink *_pCurrent, bint _bBreak, tf_CCompare &&_Compare, mint &_Depth)
	{
		CLink *pLeft = _pCurrent->f_GetLeftP();
		CLink *pRight = _pCurrent->f_GetRightP();

		mint LeftDepth = 0;
		mint RightDepth = 0;
		if (pLeft)
		{
			if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(_pCurrent), *fsp_MemberFromLink(pLeft)) || !fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pLeft), *fsp_MemberFromLink(_pCurrent)))
			{
				if (_bBreak)
				{
					DMibPDebugBreak;
				}
				return false;
			}
			if (!fpr_CheckTree(pLeft, _bBreak, fg_Forward<tf_CCompare>(_Compare), LeftDepth))
				return false;
		}
		if (pRight)
		{
			if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pRight), *fsp_MemberFromLink(_pCurrent)) || !fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(_pCurrent), *fsp_MemberFromLink(pRight)))
			{
				if (_bBreak)
				{
					DMibPDebugBreak;
				}
				return false;
			}
			if (!fpr_CheckTree(pRight, _bBreak, fg_Forward<tf_CCompare>(_Compare), RightDepth))
				return false;
		}
		if (fg_Max(LeftDepth, RightDepth) - fg_Min(LeftDepth, RightDepth) > 1)
		{
			if (_bBreak)
			{
				DMibPDebugBreak;
			}
			return false;
		}
		_Depth += fg_Max(LeftDepth, RightDepth);

		return true;
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Aggregate public																					|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	bint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_CheckTree(bint _bBreak)
	{
		return f_CheckTree(_bBreak, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	bint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_CheckTree(bint _bBreak, tf_CCompare &&_Compare)
	{
		if (CLink::fs_GetPtr(m_Root))
		{
			mint Depth = 0;
			bint bFail = !fpr_CheckTree(CLink::fs_GetPtr(m_Root), _bBreak, fg_Forward<tf_CCompare>(_Compare), Depth);
			if (bFail)
				return false;
		}

		TIterator<> Iter(this);

		while (Iter)
		{
			if (Iter.m_pStack[Iter.m_iStack]->f_GetSkew() == CLink::EAVLTreeSkew_NotInTree)
			{
				if (_bBreak)
				{
					DMibPDebugBreak;
				}
				return false;
			}

			++Iter;
		}
		return true;
	}
}
