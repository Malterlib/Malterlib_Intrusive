// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <Mib/Core/Core>

namespace NMib::NIntrusive
{
	template <typename t_CData, typename t_CKey>
	struct TCTreeCompare_Default
	{
		inline_small auto operator () (t_CData const &_Left, t_CData const &_Right) const;
		inline_small auto operator () (t_CData const &_Left, t_CKey const &_Right) const;
		inline_small auto operator () (t_CKey const &_Left, t_CData const &_Right) const;
	};

	template <typename t_CData, typename t_CKey>
	struct TCTreeCompare_String
	{
		inline_small COrdering_Strong operator () (t_CData const &_Left, t_CData const &_Right) const;
		inline_small COrdering_Strong operator () (t_CData const &_Left, t_CKey const &_Right) const;
		inline_small COrdering_Strong operator () (t_CKey const &_Left, t_CData const &_Right) const;
	};

	template <typename t_CData, typename t_CKey>
	struct TCTreeCompare_StringNoCase
	{
		inline_small COrdering_Weak operator () (t_CData const &_Left, t_CData const &_Right) const;
		inline_small COrdering_Weak operator () (t_CData const &_Left, t_CKey const &_Right) const;
		inline_small COrdering_Weak operator () (t_CKey const &_Left, t_CData const &_Right) const;
	};
}

#include "Malterlib_Intrusive_AVLTree_CompareDefaults.hpp"
