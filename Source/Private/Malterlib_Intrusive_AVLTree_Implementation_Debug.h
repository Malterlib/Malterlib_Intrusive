// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

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
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fpr_CheckTree(CLink *_pCurrent, bool _bBreak, tf_CCompare &&_fCompare, umint &_Depth)
	{
		CLink *pLeft = _pCurrent->f_GetLeftP();
		CLink *pRight = _pCurrent->f_GetRightP();

		umint LeftDepth = 0;
		umint RightDepth = 0;
		if (pLeft)
		{
			if
				(
					fsp_Compare(_fCompare, *fsp_MemberFromLink(_pCurrent), *fsp_MemberFromLink(pLeft)) < 0
					|| fsp_Compare(_fCompare, *fsp_MemberFromLink(pLeft), *fsp_MemberFromLink(_pCurrent)) > 0
				)
			{
				if (_bBreak)
				{
					DMibPDebugBreak;
				}
				return false;
			}
			if (!fpr_CheckTree(pLeft, _bBreak, _fCompare, LeftDepth))
				return false;
		}
		if (pRight)
		{
			if
				(
					fsp_Compare(_fCompare, *fsp_MemberFromLink(pRight), *fsp_MemberFromLink(_pCurrent)) < 0
					|| fsp_Compare(_fCompare, *fsp_MemberFromLink(_pCurrent), *fsp_MemberFromLink(pRight)) > 0
				)
			{
				if (_bBreak)
				{
					DMibPDebugBreak;
				}
				return false;
			}
			if (!fpr_CheckTree(pRight, _bBreak, _fCompare, RightDepth))
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
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_CheckTree(bool _bBreak)
	{
		return f_CheckTree(_bBreak, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_CheckTree(bool _bBreak, tf_CCompare &&_fCompare)
	{
		if (CLink::fs_GetPtr(m_Root))
		{
			umint Depth = 0;
			bool bFail = !fpr_CheckTree(CLink::fs_GetPtr(m_Root), _bBreak, _fCompare, Depth);
			if (bFail)
				return false;
		}

		TCIterator<> Iter(this);

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
