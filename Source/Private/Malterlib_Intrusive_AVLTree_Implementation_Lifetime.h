// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NIntrusive
{
	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Aggregate public																					|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Construct(TCAVLTreeAggregate &&_Other)
	{
		CLink::f_Assign(&m_Root, CLink::fs_GetPtr(_Other.m_Root));
		CLink::f_Assign(&_Other.m_Root, (CLink *)nullptr);
#ifdef DMibDebuggerHelpers
		static_assert(TCInstantiateValue<&fs_Debug_GetNode>::mc_Value);
#endif
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Construct()
	{
		CLink::f_Assign(&m_Root, (CLink *)nullptr);
#ifdef DMibDebuggerHelpers
		static_assert(TCInstantiateValue<&fs_Debug_GetNode>::mc_Value);
#endif
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Destruct()
	{
		if (CLink::fs_GetPtr(m_Root))
			fpr_Unlink(m_Root);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAll(tf_CCompare &&_Compare)
	{
		while (f_GetRoot())
		{
			CNode *pData = f_GetRoot();
			f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
			fg_DeleteObject(CAllocator(), pData);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAll()
	{
		f_DeleteAll(t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator, typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorCompare(tf_CCompare &&_Compare)
	{
		while (f_GetRoot())
		{
			CNode *pData = f_GetRoot();
			f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
			fg_DeleteObject(tf_CAllocator(), pData);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocator()
	{
		f_DeleteAllAllocatorCompare<tf_CAllocator>(t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator, typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorCompare(tf_CCompare &&_Compare, tf_CAllocator &_Allocator)
	{
		while (f_GetRoot())
		{
			CNode *pData = f_GetRoot();
			f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
			fg_DeleteObject(_Allocator, pData);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocator(tf_CAllocator &_Allocator)
	{
		f_DeleteAllAllocatorCompare<tf_CAllocator>(t_CCompare(), _Allocator);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDefiniteType(tf_CCompare &&_Compare)
	{
		while (f_GetRoot())
		{
			CNode *pData = f_GetRoot();
			f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
			fg_DeleteObjectDefiniteType(CAllocator(), pData);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDefiniteType()
	{
		f_DeleteAllDefiniteType(t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator, typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorCompareDefiniteType(tf_CCompare &&_Compare)
	{
		while (f_GetRoot())
		{
			CNode *pData = f_GetRoot();
			f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
			fg_DeleteObjectDefiniteType(tf_CAllocator(), pData);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorDefiniteType()
	{
		f_DeleteAllAllocatorCompareDefiniteType<tf_CAllocator>(t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator, typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorCompareDefiniteType(tf_CCompare &&_Compare, tf_CAllocator &_Allocator)
	{
		while (f_GetRoot())
		{
			CNode *pData = f_GetRoot();
			f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
			fg_DeleteObjectDefiniteType(_Allocator, pData);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorDefiniteType(tf_CAllocator &_Allocator)
	{
		f_DeleteAllAllocatorCompareDefiniteType<tf_CAllocator>(t_CCompare(), _Allocator);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CDeleter, typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDeleter(tf_CCompare &&_Compare)
	{
		while (f_GetRoot())
		{
			CNode *pData = f_GetRoot();
			f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
			tf_CDeleter::fs_Delete(pData);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CDeleter>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDeleter()
	{
		f_DeleteAllDeleter<tf_CDeleter>(t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDeleter(tf_CCompare &&_Compare)
	{
		f_DeleteAllDeleter<CNode>(fg_Forward<tf_CCompare>(_Compare));
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDeleter()
	{
		f_DeleteAllDeleter<CNode>();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_RemoveAll(tf_CCompare &&_Compare)
	{
		while (f_GetRoot())
		{
			f_Remove(f_GetRoot(), fg_Forward<tf_CCompare>(_Compare));
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_RemoveAll()
	{
		f_RemoveAll(t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Clear(tf_CCompare &&_Compare)
	{
		while (f_GetRoot())
		{
			f_Remove(f_GetRoot(), fg_Forward<tf_CCompare>(_Compare));
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Clear()
	{
		f_RemoveAll(t_CCompare());
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| Public																							|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCAVLTree(TCAVLTree &&_Other)
	{
		CSuper::f_Construct(fg_Move(_Other));
	}
	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::operator = (TCAVLTree &&_Other)
	{
		CSuper::f_Clear();
		CSuper::f_Construct(fg_Move(_Other));
		return *this;
	}
	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::TCAVLTree()
	{
		CSuper::f_Construct();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::~TCAVLTree()
	{
		CSuper::f_Destruct();
	}
}
