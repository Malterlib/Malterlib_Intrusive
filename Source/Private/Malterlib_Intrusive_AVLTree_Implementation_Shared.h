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
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_RotateRight(CLinkPointer &_pObject)
	{
		CLink *tmp = CLink::fs_GetPtr(_pObject);
		CLink *pObj = tmp->f_GetLeftP();
		CLink::f_Assign(_pObject, pObj);
		tmp->f_SetLeft(pObj->f_GetRight());
		pObj->f_SetRight(tmp);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_RotateLeft(CLinkPointer &_pObject)
	{
		CLink *tmp = CLink::fs_GetPtr(_pObject);
		CLink *pObj = tmp->f_GetRightP();
		CLink::f_Assign(_pObject, pObj);
		tmp->f_SetRight(pObj->f_GetLeft());
		pObj->f_SetLeft(tmp);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium bint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_RightShrunk(CLinkPointer &_pObject)
	{
		CLink *pObj = CLink::fs_GetPtr(_pObject);

		switch (pObj->f_GetSkew())
		{
		case CLink::EAVLTreeSkew_Right:
			pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
			return true;

		case CLink::EAVLTreeSkew_Left:
			{
				CLink *pLeft = pObj->f_GetLeftP();

				switch (pLeft->f_GetSkew())
				{
				case CLink::EAVLTreeSkew_Left:
					pLeft->f_SetSkew(CLink::EAVLTreeSkew_None);
					pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
					fsp_RotateRight(_pObject);
					return true;
				case CLink::EAVLTreeSkew_Right:
					switch (pLeft->f_GetRightP()->f_GetSkew())
					{
					case CLink::EAVLTreeSkew_Left:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_Right);
						pLeft->f_SetSkew(CLink::EAVLTreeSkew_None);
						break;
					case CLink::EAVLTreeSkew_Right:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
						pLeft->f_SetSkew(CLink::EAVLTreeSkew_Left);
						break;
					default:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
						pLeft->f_SetSkew(CLink::EAVLTreeSkew_None);
					}
					pLeft->f_GetRightP()->f_SetSkew(CLink::EAVLTreeSkew_None);

					fsp_RotateLeft(pObj->f_GetLeft());
					fsp_RotateRight(_pObject);
					return true;
				default:
					pObj->f_SetSkew(CLink::EAVLTreeSkew_Left);
					pLeft->f_SetSkew(CLink::EAVLTreeSkew_Right);
					fsp_RotateRight(_pObject);
					return false;
				}
			}

		default:
			pObj->f_SetSkew(CLink::EAVLTreeSkew_Left);
			return false;
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium bint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_LeftShrunk(CLinkPointer &_pObject)
	{
		CLink *pObj = CLink::fs_GetPtr(_pObject);
		switch (pObj->f_GetSkew())
		{
		case CLink::EAVLTreeSkew_Left:
			pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
			return true;

		case CLink::EAVLTreeSkew_Right:
			{
				CLink *pRight = pObj->f_GetRightP();

				switch (pRight->f_GetSkew())
				{
				case CLink::EAVLTreeSkew_Right:
					pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
					pRight->f_SetSkew(CLink::EAVLTreeSkew_None);
					fsp_RotateLeft(_pObject);
					return true;
				case CLink::EAVLTreeSkew_Left:
					switch (pRight->f_GetLeftP()->f_GetSkew())
					{
					case CLink::EAVLTreeSkew_Left:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
						pRight->f_SetSkew(CLink::EAVLTreeSkew_Right);
						break;
					case CLink::EAVLTreeSkew_Right:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_Left);
						pRight->f_SetSkew(CLink::EAVLTreeSkew_None);
						break;
					default:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
						pRight->f_SetSkew(CLink::EAVLTreeSkew_None);
					}

					pRight->f_GetLeftP()->f_SetSkew(CLink::EAVLTreeSkew_None);

					fsp_RotateRight(pObj->f_GetRight());
					fsp_RotateLeft(_pObject);
					return true;
				default:
					pObj->f_SetSkew(CLink::EAVLTreeSkew_Right);
					pRight->f_SetSkew(CLink::EAVLTreeSkew_Left);
					fsp_RotateLeft(_pObject);
					return false;
				}
			}

		default:
			pObj->f_SetSkew(CLink::EAVLTreeSkew_Right);
			return false;
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	bint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_RightGrown(CLinkPointer &_pObject)
	{
		return fsp_RightGrown_Inl(_pObject);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium bint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_RightGrown_Inl(CLinkPointer &_pObject)
	{
		CLink *pObj = CLink::fs_GetPtr(_pObject);
		switch (pObj->f_GetSkew())
		{
		case CLink::EAVLTreeSkew_Left:
			pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
			return false;

		case CLink::EAVLTreeSkew_Right:
			{
				CLink * pRight = pObj->f_GetRightP();
				if (pRight->f_GetSkew() == CLink::EAVLTreeSkew_Right)
				{
					pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
					pRight->f_SetSkew(CLink::EAVLTreeSkew_None);
					fsp_RotateLeft(_pObject);
				}
				else
				{
					CLink *pLeft = pRight->f_GetLeftP();
					aint Skew = pLeft->f_GetSkew();
					pLeft->f_SetSkew(CLink::EAVLTreeSkew_None);
					switch (Skew)
					{
					case CLink::EAVLTreeSkew_Right:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_Left);
						pRight->f_SetSkew(CLink::EAVLTreeSkew_None);
						break;
					case CLink::EAVLTreeSkew_Left:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
						pRight->f_SetSkew(CLink::EAVLTreeSkew_Right);
						break;
					default:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
						pRight->f_SetSkew(CLink::EAVLTreeSkew_None);
					}

					fsp_RotateRight(pObj->f_GetRight());
					fsp_RotateLeft(_pObject);
				}
				return false;
			}

		default:
			pObj->f_SetSkew(CLink::EAVLTreeSkew_Right);
			return true;
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	bint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_LeftGrown(CLinkPointer &_pObject)
	{
		return fsp_LeftGrown_Inl(_pObject);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_medium bint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fsp_LeftGrown_Inl(CLinkPointer &_pObject)
	{
		CLink *pObj = CLink::fs_GetPtr(_pObject);
		switch (pObj->f_GetSkew())
		{
		case CLink::EAVLTreeSkew_Right:
			pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
			return false;

		case CLink::EAVLTreeSkew_Left:
			{
				CLink * pLeft = pObj->f_GetLeftP();
				if (pLeft->f_GetSkew() == CLink::EAVLTreeSkew_Left)
				{
					pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
					pLeft->f_SetSkew(CLink::EAVLTreeSkew_None);
					fsp_RotateRight(_pObject);
				}
				else
				{
					CLink * pRight = pLeft->f_GetRightP();
					aint Skew = pRight->f_GetSkew();
					pRight->f_SetSkew(CLink::EAVLTreeSkew_None);
					switch (Skew)
					{
					case CLink::EAVLTreeSkew_Left:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_Right);
						pLeft->f_SetSkew(CLink::EAVLTreeSkew_None);
						break;
					case CLink::EAVLTreeSkew_Right:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
						pLeft->f_SetSkew(CLink::EAVLTreeSkew_Left);
						break;
					default:
						pObj->f_SetSkew(CLink::EAVLTreeSkew_None);
						pLeft->f_SetSkew(CLink::EAVLTreeSkew_None);
					}

					fsp_RotateLeft(pObj->f_GetLeft());
					fsp_RotateRight(_pObject);
				}
				return false;
			}

		default:
			pObj->f_SetSkew(CLink::EAVLTreeSkew_Left);
			return true;
		}
	}
}
