// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

namespace NMib::NIntrusive
{
	template <typename t_CData, typename t_CKey>
	inline_small auto TCTreeCompare_Default<t_CData, t_CKey>::operator () (t_CData const &_Left, t_CData const &_Right) const
	{
		return _Left <=> _Right;
	}

	template <typename t_CData, typename t_CKey>
	inline_small auto TCTreeCompare_Default<t_CData, t_CKey>::operator () (t_CData const &_Left, t_CKey const &_Right) const
	{
		return _Left <=> _Right;
	}

	template <typename t_CData, typename t_CKey>
	inline_small auto TCTreeCompare_Default<t_CData, t_CKey>::operator () (t_CKey const &_Left, t_CData const &_Right) const
	{
		return _Left <=> _Right;
	}

	template <typename t_CData, typename t_CKey>
	inline_small COrdering_Strong TCTreeCompare_String<t_CData, t_CKey>::operator () (t_CData const &_Left, t_CData const &_Right) const
	{
		return NStr::fg_StrCmp((t_CKey)_Left, (t_CKey)_Right) <=> 0;
	}

	template <typename t_CData, typename t_CKey>
	inline_small COrdering_Strong TCTreeCompare_String<t_CData, t_CKey>::operator () (t_CData const &_Left, t_CKey const &_Right) const
	{
		return NStr::fg_StrCmp((t_CKey)_Left, _Right) <=> 0;
	}

	template <typename t_CData, typename t_CKey>
	inline_small COrdering_Strong TCTreeCompare_String<t_CData, t_CKey>::operator () (t_CKey const &_Left, t_CData const &_Right) const
	{
		return NStr::fg_StrCmp(_Left, (t_CKey)_Right) <=> 0;
	}

	template <typename t_CData, typename t_CKey>
	inline_small COrdering_Weak TCTreeCompare_StringNoCase<t_CData, t_CKey>::operator () (t_CData const &_Left, t_CData const &_Right) const
	{
		return NStr::fg_StrCmpNoCase((t_CKey const *)_Left, (t_CKey const *)_Right) <=> 0;
	}

	template <typename t_CData, typename t_CKey>
	inline_small COrdering_Weak TCTreeCompare_StringNoCase<t_CData, t_CKey>::operator () (t_CData const &_Left, t_CKey const &_Right) const
	{
		return NStr::fg_StrCmpNoCase((t_CKey const *)_Left, _Right) <=> 0;
	}

	template <typename t_CData, typename t_CKey>
	inline_small COrdering_Weak TCTreeCompare_StringNoCase<t_CData, t_CKey>::operator () (t_CKey const &_Left, t_CData const &_Right) const
	{
		return NStr::fg_StrCmpNoCase(_Left, (t_CKey const *)_Right) <=> 0;
	}
}
