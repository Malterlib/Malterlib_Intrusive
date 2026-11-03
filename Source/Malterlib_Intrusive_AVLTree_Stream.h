// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <Mib/Core/Core>

namespace NMib::NStream
{
	template <typename t_CStream, auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	struct TCBinaryStreamTypeReference<t_CStream, NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>>
	{
		static constexpr void fs_Feed(t_CStream &_Stream, NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> const &_Data);
		static constexpr void fs_Feed(t_CStream &_Stream, NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &&_Data);
		static constexpr void fs_Consume(t_CStream &_Stream, NIntrusive::TCAVLTreeAggregate<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &_Data);
	};

	template <typename t_CStream, auto t_pLinkMember, typename t_CCompare, typename t_CAllocator, typename t_COverrideNodeType>
	struct TCBinaryStreamTypeReference<t_CStream, NIntrusive::TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType>>
	{
		static constexpr void fs_Feed(t_CStream &_Stream, NIntrusive::TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> const &_Data);
		static constexpr void fs_Feed(t_CStream &_Stream, NIntrusive::TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &&_Data);
		static constexpr void fs_Consume(t_CStream &_Stream, NIntrusive::TCAVLTree<t_pLinkMember, t_CCompare, t_CAllocator, t_COverrideNodeType> &_Data);
	};
}

#include "Malterlib_Intrusive_AVLTree_Stream.hpp"
