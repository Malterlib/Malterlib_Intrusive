// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

#include <Mib/Core/Core>

namespace NMib::NIntrusive
{
	template <typename t_CData, typename t_CKey>
	class TCTreeCompare_Default
	{
	public:

		inline_small bint operator () (t_CData const &_Left, t_CData const &_Right) const
		{
			return _Left < _Right;
		}
		inline_small bint operator () (t_CData const &_Left, t_CKey const &_Right) const
		{
			return _Left < _Right;
		}
		inline_small bint operator () (t_CKey const &_Left, t_CData const &_Right) const
		{
			return _Left < _Right;
		}
	};

	template <typename t_CData, typename t_CKey>
	class TCTreeCompare_String
	{
	public:
		inline_small bint operator () (t_CData const &_Left, t_CData const &_Right) const
		{
			return NStr::fg_StrCmp((t_CKey)_Left, (t_CKey)_Right) < 0;
		}
		inline_small bint operator () (t_CData const &_Left, t_CKey const &_Right) const
		{
			return NStr::fg_StrCmp((t_CKey)_Left, _Right) < 0;
		}
		inline_small bint operator () (t_CKey const &_Left, t_CData const &_Right) const
		{
			return NStr::fg_StrCmp(_Left, (t_CKey)_Right) < 0;
		}
	};

	template <typename t_CData, typename t_CKey>
	class TCTreeCompare_StringNoCase
	{
	public:
		inline_small bint operator () (t_CData const &_Left, t_CData const &_Right) const
		{
			return NStr::fg_StrCmpNoCase((const t_CKey *)_Left, (const t_CKey *)_Right) < 0;
		}
		inline_small bint operator () (t_CData const &_Left, t_CKey const &_Right) const
		{
			return NStr::fg_StrCmpNoCase((const t_CKey *)_Left, _Right) < 0;
		}
		inline_small bint operator () (t_CKey const &_Left, t_CData const &_Right) const
		{
			return NStr::fg_StrCmpNoCase(_Left, (const t_CKey *)_Right) < 0;
		}
	};
}
