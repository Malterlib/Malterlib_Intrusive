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
		CLink::f_AssignInit(&m_Root, CLink::fs_GetPtr(_Other.m_Root));
		CLink::f_AssignInit(&_Other.m_Root, (CLink *)nullptr);
#ifdef DMibDebuggerHelpers
		static_assert(TCInstantiateValue<&fs_Debug_GetNode>::mc_Value);
#endif
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	inline_small void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_Construct()
	{
		CLink::f_AssignInit(&m_Root, (CLink *)nullptr);
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
	template <typename tf_FDoDelete>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_DeleteAll(tf_FDoDelete &&_fDoDelete)
	{
		fp_RemoveAll(_fDoDelete);
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
	mint TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_GetDepthRecursive(CLink *_pNode) const
	{
		auto *pLeft = _pNode->f_GetLeftP();
		auto *pRight = _pNode->f_GetRightP();

		if (pLeft && pRight)
			return fg_Max(fp_GetDepthRecursive(pLeft), fp_GetDepthRecursive(pRight)) + 1;
		else if (pLeft)
			return fp_GetDepthRecursive(pLeft) + 1;
		else if (pRight)
			return fp_GetDepthRecursive(pRight) + 1;
		else
			return 1;
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
	template <typename tf_CTree, typename tf_FCreateNode>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_CopyTreeRecursive(typename tf_CTree::CLink const *_pSourceNode, tf_FCreateNode &&_fCreateNode)
		-> CLink *
	{
		auto *pLeftSource = _pSourceNode->f_GetLeftP();
		auto *pRightSource = _pSourceNode->f_GetRightP();
		auto OldSourceSkew = _pSourceNode->f_GetSkew();

		CLink *pLeft;
		if (pLeftSource)
			pLeft = fp_CopyTreeRecursive<tf_CTree>(pLeftSource, _fCreateNode);
		else
			pLeft = nullptr;

		auto *pNewNode = _fCreateNode(*tf_CTree::fsp_MemberFromLinkConst(_pSourceNode));
		auto *pNewLink = fsp_LinkFromMember(pNewNode);

		CLink *pRight;
		if (pRightSource)
			pRight = fp_CopyTreeRecursive<tf_CTree>(pRightSource, _fCreateNode);
		else
			pRight = nullptr;

		pNewLink->f_SetAll(pLeft, pRight, OldSourceSkew);

		return pNewLink;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FCreateNode>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_CopyTreeRecursive(CLink const *_pSourceNode, tf_FCreateNode &&_fCreateNode) -> CLink *
	{
		auto *pLeftSource = _pSourceNode->f_GetLeftP();
		auto *pRightSource = _pSourceNode->f_GetRightP();
		auto OldSourceSkew = _pSourceNode->f_GetSkew();

		CLink *pLeft;
		if (pLeftSource)
			pLeft = fp_CopyTreeRecursive(pLeftSource, _fCreateNode);
		else
			pLeft = nullptr;

		auto *pNewNode = _fCreateNode(*fsp_MemberFromLinkConst(_pSourceNode));
		auto *pNewLink = fsp_LinkFromMember(pNewNode);

		CLink *pRight;
		if (pRightSource)
			pRight = fp_CopyTreeRecursive(pRightSource, _fCreateNode);
		else
			pRight = nullptr;

		pNewLink->f_SetAll(pLeft, pRight, OldSourceSkew);

		return pNewLink;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FCreateNode>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_CopyTree(TCAVLTreeAggregate const &_Source, tf_FCreateNode &&_fCreateNode)
	{
		auto pSourceRoot = CLink::fs_GetPtr(_Source.m_Root);
		if (!pSourceRoot)
			return;

		auto pRoot = fp_CopyTreeRecursive(pSourceRoot, _fCreateNode);
		CLink::f_AssignInit(&m_Root, pRoot);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template
	<
		auto tf_pLinkMember
		, typename tf_CCompare
		, typename tf_CAllocator
		, typename tf_COverrideNodeType
		, typename tf_FCreateNode
	>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_CopyTree
		(
			TCAVLTreeAggregate<tf_pLinkMember, tf_CCompare, tf_CAllocator, tf_COverrideNodeType> const &_Source
			, tf_FCreateNode &&_fCreateNode
		)
	{
		auto pSourceRoot = CLink::fs_GetPtr(_Source.m_Root);
		if (!pSourceRoot)
			return;

		auto pRoot = fp_CopyTreeRecursive<TCAVLTreeAggregate<tf_pLinkMember, tf_CCompare, tf_CAllocator, tf_COverrideNodeType>>(pSourceRoot, _fCreateNode);
		CLink::f_AssignInit(&m_Root, pRoot);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_CTree, typename tf_FCreateNode>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_MoveTreeRecursive(typename tf_CTree::CLink *_pSourceNode, tf_FCreateNode &&_fCreateNode) -> CLink *
	{
		auto *pLeftSource = _pSourceNode->f_GetLeftP();
		auto *pRightSource = _pSourceNode->f_GetRightP();
		auto OldSourceSkew = _pSourceNode->f_GetSkew();
		_pSourceNode->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);

		CLink *pLeft;
		if (pLeftSource)
			pLeft = fp_MoveTreeRecursive<tf_CTree>(pLeftSource, _fCreateNode);
		else
			pLeft = nullptr;

		auto *pNewNode = _fCreateNode(tf_CTree::fsp_MemberFromLink(_pSourceNode));
		auto *pNewLink = fsp_LinkFromMember(pNewNode);

		CLink *pRight;
		if (pRightSource)
			pRight = fp_MoveTreeRecursive<tf_CTree>(pRightSource, _fCreateNode);
		else
			pRight = nullptr;

		pNewLink->f_SetAll(pLeft, pRight, OldSourceSkew);

		return pNewLink;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FCreateNode>
	auto TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_MoveTreeRecursive(CLink *_pSourceNode, tf_FCreateNode &&_fCreateNode) -> CLink *
	{
		auto *pLeftSource = _pSourceNode->f_GetLeftP();
		auto *pRightSource = _pSourceNode->f_GetRightP();
		auto OldSourceSkew = _pSourceNode->f_GetSkew();
		_pSourceNode->f_SetSkew(CLink::EAVLTreeSkew_NotInTree);

		CLink *pLeft;
		if (pLeftSource)
			pLeft = fp_MoveTreeRecursive(pLeftSource, _fCreateNode);
		else
			pLeft = nullptr;

		auto *pNewNode = _fCreateNode(fsp_MemberFromLink(_pSourceNode));
		auto *pNewLink = fsp_LinkFromMember(pNewNode);

		CLink *pRight;
		if (pRightSource)
			pRight = fp_MoveTreeRecursive(pRightSource, _fCreateNode);
		else
			pRight = nullptr;

		pNewLink->f_SetAll(pLeft, pRight, OldSourceSkew);

		return pNewLink;
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FCreateNode>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_MoveTree(TCAVLTreeAggregate &_Source, tf_FCreateNode &&_fCreateNode)
	{
		auto pSourceRoot = CLink::fs_GetPtr(_Source.m_Root);
		if (!pSourceRoot)
			return;

		auto pRoot = fp_MoveTreeRecursive(pSourceRoot, _fCreateNode);
		CLink::f_AssignInit(&m_Root, pRoot);
		CLink::f_AssignInit(&_Source.m_Root, (CLink *)nullptr);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template
	<
		auto tf_pLinkMember
		, typename tf_CCompare
		, typename tf_CAllocator
		, typename tf_COverrideNodeType
		, typename tf_FCreateNode
	>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::f_MoveTree
		(
			TCAVLTreeAggregate<tf_pLinkMember, tf_CCompare, tf_CAllocator, tf_COverrideNodeType> &_Source
			, tf_FCreateNode &&_fCreateNode
		)
	{
		auto pSourceRoot = TCAVLTreeAggregate<tf_pLinkMember, tf_CCompare, tf_CAllocator, tf_COverrideNodeType>::CLink::fs_GetPtr(_Source.m_Root);
		if (!pSourceRoot)
			return;
		auto pRoot = fp_MoveTreeRecursive<TCAVLTreeAggregate<tf_pLinkMember, tf_CCompare, tf_CAllocator, tf_COverrideNodeType>>(pSourceRoot, _fCreateNode);
		CLink::f_AssignInit(&m_Root, pRoot);
		TCAVLTreeAggregate<tf_pLinkMember, tf_CCompare, tf_CAllocator, tf_COverrideNodeType>::CLink::f_AssignInit(&_Source.m_Root, (CLink *)nullptr);
	}

	template <auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	template <typename tf_FOnRemove>
	void TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::fp_RemoveAll(tf_FOnRemove &&_fOnRemove)
	{
		auto pRoot = CLink::fs_GetPtr(m_Root);
		if (!pRoot)
			return;
		fp_RemoveAllRecursive(pRoot, _fOnRemove);
		CLink::f_AssignInit(&m_Root, (CLink *)nullptr);
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
