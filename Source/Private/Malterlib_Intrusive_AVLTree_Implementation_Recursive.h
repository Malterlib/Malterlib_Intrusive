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
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_BalanceLowest(CLink* &_pLowestObject, CLinkPointer &_pObject)
	{
		CLink *pObject = CLink::fs_GetPtr(_pObject);

		if (pObject->f_GetLeftP())
		{
			if (fsp_BalanceLowest(_pLowestObject, *pObject->f_GetLeft()))
			{
				return fsp_LeftShrunk(_pObject);
			}
			else
				return false;
		}

		// Save the object that we want at targets place
		_pLowestObject = pObject;

		// Remove pObject from the tree
		CLink::f_Assign(&_pObject, pObject->f_GetRight());

		return true;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_BalanceHighest(CLink* &_pHighestObject, CLinkPointer &_pObject)
	{
		CLink *pObject = CLink::fs_GetPtr(_pObject);

		if (pObject->f_GetRightP())
		{
			if (fsp_BalanceHighest(_pHighestObject, *pObject->f_GetRight()))
			{
				return fsp_RightShrunk(_pObject);
			}
			else
				return false;
		}

		// Save the object that we want at targets place
		_pHighestObject = pObject;

		// Remove pObject from the tree
		CLink::f_Assign(&_pObject, pObject->f_GetLeft());

		return true;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fspr_Insert(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_CCompare &&_fCompare, bool &_bRet)
	{
		if (!CLink::fs_GetPtrIsFast())
		{
			if (CLink *pObj = CLink::fs_GetPtr(_pObject))
			{
				auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(_pObjectToInsert), *fsp_MemberFromLink(pObj));
				static_assert(cIsOrderType<decltype(CompareResult)>);
				if (CompareResult < 0)
				{
					if (fspr_Insert(pObj->f_GetLeft(), _pObjectToInsert, _fCompare, _bRet))
					{
						return fsp_LeftGrown(_pObject);
					}
				}
				else if (CompareResult > 0)
				{
					if (fspr_Insert(pObj->f_GetRight(), _pObjectToInsert, _fCompare, _bRet))
					{
						return fsp_RightGrown(_pObject);
					}
				}
				else
				{
					DMibFastCheck(false); // Tree does not support inserting two equal objects
					_bRet = false;
					return false;
				}
				return false;
			}
			else
			{
				DMibFastCheck(_pObjectToInsert); // You cannot insert a null object
				_pObjectToInsert->f_Clear();
				CLink::f_Assign(&_pObject, _pObjectToInsert);

				return true;
			}
		}
		else
		{

			if (!CLink::fs_GetPtr(_pObject))
			{
				DMibFastCheck(_pObjectToInsert); // You cannot insert a null object

				_pObjectToInsert->f_SetAll(nullptr, nullptr, CLink::EAVLTreeSkew_None);

				CLink::f_Assign(&_pObject, _pObjectToInsert);

				return true;
			}

			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(_pObjectToInsert), *fsp_MemberFromLink(CLink::fs_GetPtr(_pObject)));
			static_assert(cIsOrderType<decltype(CompareResult)>);
			if (CompareResult < 0)
			{
				if (fspr_Insert(CLink::fs_GetPtr(_pObject)->f_GetLeft(), _pObjectToInsert, _fCompare, _bRet))
				{
					return fsp_LeftGrown_Inl(_pObject);
				}
			}
			else if (CompareResult > 0)
			{
				if (fspr_Insert(CLink::fs_GetPtr(_pObject)->f_GetRight(), _pObjectToInsert, _fCompare, _bRet))
				{
					return fsp_RightGrown_Inl(_pObject);
				}
			}
			else
			{
				DMibFastCheck(false); // Tree does not support inserting two equal objects
				_bRet = false;
				return false;
			}
			return false;
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fspr_Remove(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_CCompare &&_fCompare)
	{
		CLink* pObj = CLink::fs_GetPtr(_pObject);

		DMibFastCheck(pObj); // Object was not found in tree

		if (pObj != _pObjectToRemove)
		{
			auto CompareResult = fsp_Compare(_fCompare, *fsp_MemberFromLink(_pObjectToRemove), *fsp_MemberFromLink(pObj));
			static_assert(cIsOrderType<decltype(CompareResult)>);
			if (CompareResult < 0)
			{
				if (fspr_Remove(pObj->f_GetLeft(), _pObjectToRemove, _fCompare))
				{
					return fsp_LeftShrunk(_pObject);
				}
				return false;
			}
			else if (CompareResult > 0)
			{
				if (fspr_Remove(pObj->f_GetRight(), _pObjectToRemove, _fCompare))
				{
					return fsp_RightShrunk(_pObject);
				}
				return false;
			}
		}

		if (pObj->f_GetLeftP())
		{
			CLink* pHighestObject;
			bool bLeftShrunk = fsp_BalanceHighest(pHighestObject, *pObj->f_GetLeft());

			// Remove target from tree
			pHighestObject->f_SetSkew(pObj->f_GetSkew());
			pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
			// Remove from last place
			// Link in on targets place
			pHighestObject->f_SetLeft(pObj->f_GetLeft());
			pHighestObject->f_SetRight(pObj->f_GetRight());
			CLink::f_Assign(&_pObject, pHighestObject);

			if (bLeftShrunk)
			{
				return fsp_LeftShrunk(_pObject);
			}
			else
				return false;
		}

		if (pObj->f_GetRightP())
		{
			CLink* pLowestObject;

			bool bRightShrunk = fsp_BalanceLowest(pLowestObject, *pObj->f_GetRight());

			// Remove target from tree
			pLowestObject->f_SetSkew(pObj->f_GetSkew());
			pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
			// Link in on targets place
			pLowestObject->f_SetLeft(pObj->f_GetLeft());
			pLowestObject->f_SetRight(pObj->f_GetRight());
			CLink::f_Assign(&_pObject, pLowestObject);

			if (bRightShrunk)
			{
				return fsp_RightShrunk(_pObject);
			}
			else
				return false;
		}

		pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);

		CLink::f_Assign(&_pObject, (CLink *)nullptr);

		return true;
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Public																							|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fr_Insert(CNode *_pToInsert, tf_CCompare &&_fCompare)
	{

		CLink * pToInsert = fsp_LinkFromMember(_pToInsert);
		if constexpr (CLinkContainer::mc_bNeedSetTree)
			((CLinkContainer *)pToInsert)->f_SetTree(this, &TCAVLTreeAggregate::fsp_Remove);
		DMibFastCheck(pToInsert->f_GetSkew() == CLink::EAVLTreeSkew_NotInTree); // Must not be in tree already

		bool bRet = true;
		fspr_Insert(m_Root, pToInsert, _fCompare, bRet);
		return bRet;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fr_Insert(CNode &_ToInsert)
	{
		return fr_Insert(_ToInsert, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fr_Insert(CNode &_ToInsert, tf_CCompare &&_fCompare)
	{
		return fr_Insert(&_ToInsert, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small bool TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fr_Insert(CNode *_pToInsert)
	{
		return fr_Insert(_pToInsert, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fr_Remove(CNode *_pToRemove, tf_CCompare &&_fCompare)
	{
		CLink * pToRemove = fsp_LinkFromMember(_pToRemove);

		fspr_Remove(m_Root, pToRemove, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fr_Remove(CNode &_ToRemove, tf_CCompare &&_fCompare)
	{
		fr_Remove(&_ToRemove, _fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fr_Remove(CNode &_ToRemove)
	{
		fr_Remove(_ToRemove, t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fr_Remove(CNode *_pToRemove)
	{
		fr_Remove(_pToRemove, t_CCompare());
	}
}
