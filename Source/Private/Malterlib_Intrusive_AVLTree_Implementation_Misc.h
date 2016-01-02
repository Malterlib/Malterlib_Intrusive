// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib
{
	namespace NIntrusive
	{
		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Aggregate private																					|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

#ifdef DMibDebuggerHelpers
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		assure_used mint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::ms_OffsetTCAVLTreeAggregate = COffset::mc_Offset;
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		assure_used typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode* TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::ms_pNode = nullptr;
		
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <aint _RecursionDepth>
		assure_used TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>* 
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::TIterator<_RecursionDepth>::ms_pTree = nullptr;
#endif
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CStackObj::f_SetAll(CLinkPointer *_pPtr, bint _bLarger)
		{
			m_pStack = _pPtr;
			m_bLarger = _bLarger;
		}
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CStackObj::f_SetStack(CLinkPointer *_pPtr)
		{
			m_pStack = _pPtr;
		}
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CLinkPointer *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CStackObj::f_GetStack() const
		{
			return m_pStack;
		}
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small aint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CStackObj::f_IsLarger() const
		{
			return m_bLarger;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		return_not_aliased inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CLink *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fsp_LinkFromMember(CNode *_pMember)
		{				
			return ((CLink *)(((uint8 *)_pMember) + COffset::mc_Offset));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		return_not_aliased inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fsp_MemberFromLink(CLink *_pLink)
		{				
			return ((CNode *)(((uint8 *)_pLink) - COffset::mc_Offset));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		return_not_aliased inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CLink *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fsp_LinkFromMemberConst(const CNode *_pMember)
		{				
			return ((const CLink *)(((uint8 *)_pMember) + COffset::mc_Offset));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		return_not_aliased inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fsp_MemberFromLinkConst(const CLink *_pLink)
		{				
			return ((CNode *)(((uint8 *)_pLink) - COffset::mc_Offset));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fpr_Unlink(CLinkPointer &_pUnlink)
		{
			if (CLink::fs_GetPtr(_pUnlink)->f_GetLeftP())
				fpr_Unlink(CLink::fs_GetPtr(_pUnlink)->f_GetLeft());

			if (CLink::fs_GetPtr(_pUnlink)->f_GetRightP())
				fpr_Unlink(CLink::fs_GetPtr(_pUnlink)->f_GetRight());

			CLink::fs_GetPtr(_pUnlink)->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
		}

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Aggregate public																					|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fs_MemberFromLink(CLink *_pLink)
		{				
			return fsp_MemberFromLink(_pLink);
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_IsEmpty() const 
		{
			return !(CLink::fs_GetPtr(m_Root));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_HasOneMember() const 
		{
			CLink * pObject = CLink::fs_GetPtr(m_Root);
			if (pObject)
			{
				if (!pObject->f_GetLeftP() && !pObject->f_GetRightP())
					return true;
			}
			return false;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_GetRoot() const
		{				
			if (CLink::fs_GetPtr(m_Root))
				return fsp_MemberFromLink(CLink::fs_GetPtr(m_Root));
			else
				return nullptr;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fs_GetRight(CNode *_pObject)
		{
			CLink * pObject = fsp_LinkFromMember(_pObject);
				
			if (pObject->f_GetRightP())
				return fsp_MemberFromLink(pObject->f_GetRightP());
			else
				return nullptr;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small typename TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::CNode *
		TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fs_GetLeft(CNode *_pObject)
		{
			CLink * pObject = fsp_LinkFromMember(_pObject);
				
			if (pObject->f_GetLeftP())
				return fsp_MemberFromLink(pObject->f_GetLeftP());
			else
				return nullptr;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_medium mint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_GetLen() const
		{
			TIterator<> Iter(this);
			mint Len = 0;
			while (Iter)
			{
				++Len;
				++Iter;
			}
			return Len;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::operator == (const TCAVLTreeAggregate &_Other) const
		{
			TIterator<> Iter0 = *this;
			TIterator<> Iter1 = _Other;

			while (Iter0 && Iter1)
			{
				if (!(*Iter0 == *Iter1))
					return false;
				++Iter0;
				++Iter1;
			}

			if (Iter0 && !Iter1)
				return false;
			if (!Iter0 && Iter1)
				return false;
			return true;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		bint TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::operator < (const TCAVLTreeAggregate &_Other) const
		{
			TIterator<> Iter0 = *this;
			TIterator<> Iter1 = _Other;

			while (Iter0 && Iter1)
			{
				if (*Iter0 < *Iter1)
					return true;
				if (*Iter0 > *Iter1)
					return false;
				++Iter0;
				++Iter1;
			}

			if (Iter0 && !Iter1)
				return false;
			if (!Iter0 && Iter1)
				return true;
			return false;
		}

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Public																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		bint TCAVLTree<t_CLinkTraits, t_CCompare, t_CAllocator>::operator == (const TCAVLTree &_Other) const
		{
			return (const CSuper &)(*this) == (const CSuper &)_Other;
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		bint TCAVLTree<t_CLinkTraits, t_CCompare, t_CAllocator>::operator < (const TCAVLTree &_Other) const
		{
			return (const CSuper &)(*this) < (const CSuper &)_Other;
		}



	}
}

