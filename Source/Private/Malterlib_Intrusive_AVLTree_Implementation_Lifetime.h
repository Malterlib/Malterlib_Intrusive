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
		fp_RemoveAll([&](CNode *) inline_always_lambda {});
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAll()
	{
		fp_RemoveAll
			(
				[&](CNode *_pToRemove)
				{
					fg_DeleteObject(CAllocator(), _pToRemove);
				}
			)
		;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocator()
	{
		fp_RemoveAll
			(
				[&](CNode *_pToRemove)
				{
					fg_DeleteObject(tf_CAllocator(), _pToRemove);
				}
			)
		;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocator(tf_CAllocator &_Allocator)
	{
		fp_RemoveAll
			(
				[&](CNode *_pToRemove)
				{
					fg_DeleteObject(_Allocator, _pToRemove);
				}
			)
		;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDefiniteType()
	{
		fp_RemoveAll
			(
				[&](CNode *_pToRemove)
				{
					fg_DeleteObjectDefiniteType(CAllocator(), _pToRemove);
				}
			)
		;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorDefiniteType()
	{
		fp_RemoveAll
			(
				[&](CNode *_pToRemove)
				{
					fg_DeleteObjectDefiniteType(tf_CAllocator(), _pToRemove);
				}
			)
		;
	}
	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorDefiniteType(tf_CAllocator &_Allocator)
	{
		fp_RemoveAll
			(
				[&](CNode *_pToRemove)
				{
					fg_DeleteObjectDefiniteType(_Allocator, _pToRemove);
				}
			)
		;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CDeleter>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDeleter()
	{
		fp_RemoveAll
			(
				[&](CNode *_pToRemove)
				{
					tf_CDeleter::fs_Delete(_pToRemove);
				}
			)
		;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDeleter()
	{
		f_DeleteAllDeleter<CNode>();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FOnRemove>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RemoveAllRecursive(CLink *_pNode, tf_FOnRemove &&_fOnRemove)
	{
		auto *pLeft = _pNode->f_GetLeftP();
		auto *pRight = _pNode->f_GetRightP();

		_pNode->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);
		if (pRight)
			fp_RemoveAllRecursive(pRight, _fOnRemove);
		_fOnRemove(fsp_MemberFromLink(_pNode));
		if (pLeft)
			fp_RemoveAllRecursive(pLeft, _fOnRemove);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorDefiniteType()
	{
		f_DeleteAllAllocatorCompareDefiniteType<tf_CAllocator>(t_CCompare());
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CAllocator, typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllAllocatorCompareDefiniteType(tf_CCompare &&_fCompare, tf_CAllocator &_Allocator)
	{
		while (f_GetRoot())
		{
			CNode *pData = f_GetRoot();
			f_Remove(pData, _fCompare);
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
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDeleter(tf_CCompare &&_fCompare)
	{
		while (f_GetRoot())
		{
			CNode *pData = f_GetRoot();
			f_Remove(pData, _fCompare);
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
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDeleter(tf_CCompare &&_fCompare)
	{
		f_DeleteAllDeleter<CNode>(_fCompare);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAllDeleter()
	{
		f_DeleteAllDeleter<CNode>();
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CCompare>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_RemoveAll(tf_CCompare &&_fCompare)
	{
		while (f_GetRoot())
		{
			f_Remove(f_GetRoot(), _fCompare);
		}
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FOnRemove>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RemoveAll(tf_FOnRemove &&_fOnRemove)
	{
		auto pRoot = CLink::fs_GetPtr(m_Root);
		if (!pRoot)
			return;
		fp_RemoveAllRecursive(pRoot, _fOnRemove);
		CLink::f_Assign(&m_Root, (CLink *)nullptr);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_RemoveAll()
	{
		fp_RemoveAll([&](CNode *) inline_always_lambda {});
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Clear()
	{
		fp_RemoveAll([&](CNode *) inline_always_lambda {});
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
