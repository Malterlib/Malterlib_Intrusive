// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

#include <Mib/Core/Core>

namespace NMib::NStream
{
	template <typename t_CStream, auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	class TCBinaryStreamTypeReference<t_CStream, NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> >
	{
	public:
		static void fs_Feed(t_CStream &_Stream, NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> const &_Data)
		{
			typename NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::template TIterator<> Iter(_Data);

			mint nItems = 0;

			while (Iter)
			{
				++nItems;
				++Iter;
			};

			fg_FeedLenToStream(_Stream, nItems);

			Iter = _Data;

			while (Iter)
			{
				_Stream.f_Feed(*Iter);
				++Iter;
			};
		}

		static void fs_Feed(t_CStream &_Stream, NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &&_Data)
		{
			typename NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::template TIterator<> Iter(_Data);

			mint nItems = 0;

			while (Iter)
			{
				++nItems;
				++Iter;
			};

			fg_FeedLenToStream(_Stream, nItems);

			Iter = _Data;

			while (Iter)
			{
				_Stream.f_Feed(fg_Move(*Iter));
				++Iter;
			};
		}

		static void fs_Consume(t_CStream &_Stream, NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &_Data)
		{
			uint64 nItems;
			fg_ConsumeLenFromStream(_Stream, nItems);
			fg_CheckLengthLimit(_Stream, nItems);

			using CNode = typename NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>::CNode;

			while(nItems)
			{
				auto Memory = t_CAllocator::f_AllocSafe(sizeof(CNode), NTraits::TCAlignmentOf<CNode>::mc_Value);
				CNode *pNewItem = new(Memory.m_pMemory) CNode();
				Memory.f_Claim();
				auto Cleanup = g_OnScopeExit > [&]
					{
						pNewItem->~CNode();
						t_CAllocator::f_Free(pNewItem, sizeof(CNode));
					}
				;

				_Stream.f_Consume(*pNewItem);
				if (!_Data.f_Insert(pNewItem))
				{
					DMibError("AVLTree stream contained a duplicate key");
				}
				else
					Cleanup.f_Clear();

				--nItems;
			}
		}
	};

	template <typename t_CStream, auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	class TCBinaryStreamTypeReference<t_CStream, NIntrusive::TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> >
	{
	public:
		static void fs_Feed(t_CStream &_Stream, NIntrusive::TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> const &_Data)
		{
			_Stream << (const NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &)_Data;
		}

		static void fs_Feed(t_CStream &_Stream, NIntrusive::TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &&_Data)
		{
			_Stream << fg_Move((NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &)_Data);
		}

		static void fs_Consume(t_CStream &_Stream, NIntrusive::TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &_Data)
		{
			_Stream >> (NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &)_Data;
		}
	};
}
