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
		bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fsp_BalanceLowest(CLink* &_pLowestObject, CLinkPointer &_pObject)
		{
			CLink *pObject = CLink::fs_GetPtr(_pObject);
				
			if (pObject->f_GetLeftP()) 
			{
				if (fsp_BalanceLowest(_pLowestObject, pObject->f_GetLeft())) 
				{
					return fsp_LeftShrunk(_pObject);
				}
				else
					return false;
			}

			// Save the object that we want at targets place
			_pLowestObject = pObject;

			// Remove pObject from the tree
			CLink::f_Assign(_pObject, pObject->f_GetRight());

			return true;
		}
			
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fsp_BalanceHighest(CLink* &_pHighestObject, CLinkPointer &_pObject)
		{
			CLink *pObject = CLink::fs_GetPtr(_pObject);

			if (pObject->f_GetRightP()) 
			{
				if (fsp_BalanceHighest(_pHighestObject, pObject->f_GetRight()))
				{
					return fsp_RightShrunk(_pObject);
				}
				else 
					return false;
			}

			// Save the object that we want at targets place
			_pHighestObject = pObject;

			// Remove pObject from the tree
			CLink::f_Assign(_pObject, pObject->f_GetLeft());

			return true;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fspr_Insert(CLinkPointer &_pObject, CLink *_pObjectToInsert, tf_CCompare &&_Compare, bint &_bRet)
		{
			if (!CLink::fs_GetPtrIsFast())
			{
				if (CLink *pObj = CLink::fs_GetPtr(_pObject))
				{
					if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(_pObjectToInsert), *fsp_MemberFromLink(pObj))) 
					{
						if (fspr_Insert(pObj->f_GetLeft(), _pObjectToInsert, fg_Forward<tf_CCompare>(_Compare), _bRet)) 
						{
							return fsp_LeftGrown(_pObject);
						}
					}
					else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToInsert)))
					{
						if (fspr_Insert(pObj->f_GetRight(), _pObjectToInsert, fg_Forward<tf_CCompare>(_Compare), _bRet)) 
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
					CLink::f_Assign(_pObject, _pObjectToInsert);

					return true;
				}
			}
			else
			{

				if (!CLink::fs_GetPtr(_pObject)) 
				{
					DMibFastCheck(_pObjectToInsert); // You cannot insert a null object

					_pObjectToInsert->f_SetAll(nullptr, nullptr, CLink::EAVLTreeSkew_None);

					CLink::f_Assign(_pObject, _pObjectToInsert);

					return true;
				}

				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(_pObjectToInsert), *fsp_MemberFromLink(CLink::fs_GetPtr(_pObject)))) 
				{
					if (fspr_Insert(CLink::fs_GetPtr(_pObject)->f_GetLeft(), _pObjectToInsert, fg_Forward<tf_CCompare>(_Compare), _bRet)) 
					{
						return fsp_LeftGrown_Inl(_pObject);
					}
				}
				else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(CLink::fs_GetPtr(_pObject)), *fsp_MemberFromLink(_pObjectToInsert)))
				{
					if (fspr_Insert(CLink::fs_GetPtr(_pObject)->f_GetRight(), _pObjectToInsert, fg_Forward<tf_CCompare>(_Compare), _bRet)) 
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

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fspr_Remove(CLinkPointer &_pObject, CLink *_pObjectToRemove, tf_CCompare &&_Compare)
		{
			CLink* pObj = CLink::fs_GetPtr(_pObject);

			DMibFastCheck(pObj); // Object was not found in tree

			if (pObj != _pObjectToRemove)
			{
				if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(_pObjectToRemove), *fsp_MemberFromLink(pObj))) 
				{
					if (fspr_Remove(pObj->f_GetLeft(), _pObjectToRemove, fg_Forward<tf_CCompare>(_Compare))) 
					{
						return fsp_LeftShrunk(_pObject);
					}
					return false;
				}
				else if (fsp_Compare(fg_Forward<tf_CCompare>(_Compare), *fsp_MemberFromLink(pObj), *fsp_MemberFromLink(_pObjectToRemove))) 
				{
					if (fspr_Remove(pObj->f_GetRight(), _pObjectToRemove, fg_Forward<tf_CCompare>(_Compare))) 
					{
						return fsp_RightShrunk(_pObject);
					}
					return false;
				}
			}
				
			if (pObj->f_GetLeftP()) 
			{
				CLink* pHighestObject;
				bint bLeftShrunk = fsp_BalanceHighest(pHighestObject, pObj->f_GetLeft());

				// Remove target from tree
				pHighestObject->f_SetSkew(pObj->f_GetSkew());
				pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
				// Remove from last place
				// Link in on targets place
				pHighestObject->f_SetLeft(pObj->f_GetLeft());
				pHighestObject->f_SetRight(pObj->f_GetRight());
				CLink::f_Assign(_pObject, pHighestObject);

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

				bint bRightShrunk = fsp_BalanceLowest(pLowestObject, pObj->f_GetRight());

				// Remove target from tree
				pLowestObject->f_SetSkew(pObj->f_GetSkew());
				pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
				// Link in on targets place
				pLowestObject->f_SetLeft(pObj->f_GetLeft());
				pLowestObject->f_SetRight(pObj->f_GetRight());
				CLink::f_Assign(_pObject, pLowestObject);

				if (bRightShrunk)
				{
					return fsp_RightShrunk(_pObject);
				}
				else
					return false;
			}

			pObj->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);

			CLink::f_Assign(_pObject, (CLink *)nullptr);

			return true;
		}

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Public																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fr_Insert(CNode *_pToInsert, tf_CCompare &&_Compare)
		{

			CLink * pToInsert = fsp_LinkFromMember(_pToInsert);
			((CLinkContainer *)pToInsert)->f_SetTree(this);
			DMibFastCheck(pToInsert->f_GetSkew() == CLink::EAVLTreeSkew_NotInTree); // Must not be in tree already
				
			bint bRet = true;
			fspr_Insert(m_Root, pToInsert, fg_Forward<tf_CCompare>(_Compare), bRet);
			return bRet;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fr_Insert(CNode &_ToInsert)
		{
			return fr_Insert(_ToInsert, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fr_Insert(CNode &_ToInsert, tf_CCompare &&_Compare)
		{
			return fr_Insert(&_ToInsert, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fr_Insert(CNode *_pToInsert)
		{
			return fr_Insert(_pToInsert, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fr_Remove(CNode *_pToRemove, tf_CCompare &&_Compare)
		{
			CLink * pToRemove = fsp_LinkFromMember(_pToRemove);

			fspr_Remove(m_Root, pToRemove, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fr_Remove(CNode &_ToRemove, tf_CCompare &&_Compare)
		{
			fr_Remove(&_ToRemove, fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fr_Remove(CNode &_ToRemove)
		{
			fr_Remove(_ToRemove, t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fr_Remove(CNode *_pToRemove)
		{
			fr_Remove(_pToRemove, t_CCompare());
		}

	}
}

