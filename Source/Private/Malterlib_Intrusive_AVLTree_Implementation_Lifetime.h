// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib
{
	namespace NIntrusive
	{
		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Aggregate public																					|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Construct(TCAVLTreeAggregate &&_Other)
		{
			CLink::f_Assign(m_Root, CLink::fs_GetPtr(_Other.m_Root));
			CLink::f_Assign(_Other.m_Root, (CLink *)nullptr);
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Construct()
		{
			CLink::f_Assign(m_Root, (CLink *)nullptr);
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		inline_small void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Destruct()
		{
			if (CLink::fs_GetPtr(m_Root))
				fpr_Unlink(m_Root);
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAll(tf_CCompare &&_Compare)
		{
			while (f_GetRoot())
			{
				CNode *pData = f_GetRoot();
				f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
				fg_DeleteObject(CAllocator(), pData);
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAll()
		{
			f_DeleteAll(t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CAllocator, typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllAllocatorCompare(tf_CCompare &&_Compare)
		{
			while (f_GetRoot())
			{
				CNode *pData = f_GetRoot();
				f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
				fg_DeleteObject(tf_CAllocator(), pData);
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllAllocator()
		{
			f_DeleteAllAllocatorCompare<tf_CAllocator>(t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CAllocator, typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllAllocatorCompare(tf_CCompare &&_Compare, tf_CAllocator &_Allocator)
		{
			while (f_GetRoot())
			{
				CNode *pData = f_GetRoot();
				f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
				fg_DeleteObject(_Allocator, pData);
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllAllocator(tf_CAllocator &_Allocator)
		{
			f_DeleteAllAllocatorCompare<tf_CAllocator>(t_CCompare(), _Allocator);
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllDefiniteType(tf_CCompare &&_Compare)
		{
			while (f_GetRoot())
			{
				CNode *pData = f_GetRoot();
				f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
				fg_DeleteObjectDefiniteType(CAllocator(), pData);
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllDefiniteType()
		{
			f_DeleteAllDefiniteType(t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CAllocator, typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllAllocatorCompareDefiniteType(tf_CCompare &&_Compare)
		{
			while (f_GetRoot())
			{
				CNode *pData = f_GetRoot();
				f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
				fg_DeleteObjectDefiniteType(tf_CAllocator(), pData);
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllAllocatorDefiniteType()
		{
			f_DeleteAllAllocatorCompareDefiniteType<tf_CAllocator>(t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CAllocator, typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllAllocatorCompareDefiniteType(tf_CCompare &&_Compare, tf_CAllocator &_Allocator)
		{
			while (f_GetRoot())
			{
				CNode *pData = f_GetRoot();
				f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
				fg_DeleteObjectDefiniteType(_Allocator, pData);
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllAllocatorDefiniteType(tf_CAllocator &_Allocator)
		{
			f_DeleteAllAllocatorCompareDefiniteType<tf_CAllocator>(t_CCompare(), _Allocator);
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CDeleter, typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllDeleter(tf_CCompare &&_Compare)
		{
			while (f_GetRoot())
			{
				CNode *pData = f_GetRoot();
				f_Remove(pData, fg_Forward<tf_CCompare>(_Compare));
				tf_CDeleter::fs_Delete(pData);
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CDeleter>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllDeleter()
		{
			f_DeleteAllDeleter<tf_CDeleter>(t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllDeleter(tf_CCompare &&_Compare)
		{
			f_DeleteAllDeleter<CNode>(fg_Forward<tf_CCompare>(_Compare));
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_DeleteAllDeleter()
		{
			f_DeleteAllDeleter<CNode>();
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_RemoveAll(tf_CCompare &&_Compare)
		{
			while (f_GetRoot())
			{
				f_Remove(f_GetRoot(), fg_Forward<tf_CCompare>(_Compare));
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_RemoveAll()
		{
            f_RemoveAll(t_CCompare());
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		template <typename tf_CCompare>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Clear(tf_CCompare &&_Compare)
		{
			while (f_GetRoot())
			{
				f_Remove(f_GetRoot(), fg_Forward<tf_CCompare>(_Compare));
			}
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		void TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::f_Clear()
		{
            f_RemoveAll(t_CCompare());
		}

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| Public																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		TCAVLTree<t_CLinkTraits, t_CCompare, t_CAllocator>::TCAVLTree(TCAVLTree &&_Other)
		{
			CSuper::f_Construct(fg_Move(_Other));
		}
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		TCAVLTree<t_CLinkTraits, t_CCompare, t_CAllocator> &TCAVLTree<t_CLinkTraits, t_CCompare, t_CAllocator>::operator = (TCAVLTree &&_Other)
		{
			CSuper::f_Clear();
			CSuper::f_Construct(fg_Move(_Other));
			return *this;
		}
		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		TCAVLTree<t_CLinkTraits, t_CCompare, t_CAllocator>::TCAVLTree()
		{
			CSuper::f_Construct();
		}

		template <typename t_CLinkTraits, typename t_CCompare, typename t_CAllocator>
		TCAVLTree<t_CLinkTraits, t_CCompare, t_CAllocator>::~TCAVLTree()
		{
			CSuper::f_Destruct();
		}



	}
}

