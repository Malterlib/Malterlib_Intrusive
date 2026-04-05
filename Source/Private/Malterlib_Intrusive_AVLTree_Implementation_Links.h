// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

namespace NMib::NIntrusive
{
	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| TCAVLLinkAggregate																				|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <EAVLLinkType t_LinkType, typename t_CLinkData>
	inline_small void TCAVLLinkAggregate<t_LinkType, t_CLinkData>::f_Construct()
	{
		m_Link.f_Construct();
	}

	template <EAVLLinkType t_LinkType, typename t_CLinkData>
	inline_small void TCAVLLinkAggregate<t_LinkType, t_CLinkData>::f_Destruct()
	{
#		if DMibEnableSafeCheck > 0
		aint Skew = m_Link.f_GetSkew();
		DMibFastCheck(Skew == t_CLinkData::EAVLTreeSkew_NotInTree); // You have to remove the link from the tree yourself.
#			endif
	}

	template <EAVLLinkType t_LinkType, typename t_CLinkData>
	bool TCAVLLinkAggregate<t_LinkType, t_CLinkData>::f_IsInTree() const
	{
		return m_Link.f_GetSkew() != t_CLinkData::EAVLTreeSkew_NotInTree;
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| TCAVLLink																							|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <EAVLLinkType t_LinkType, typename t_CLinkData>
	TCAVLLink<t_LinkType, t_CLinkData>::TCAVLLink()
	{
		this->f_Construct();
	}

	template <EAVLLinkType t_LinkType, typename t_CLinkData>
	TCAVLLink<t_LinkType, t_CLinkData>::~TCAVLLink()
	{
		this->f_Destruct();
	}

	/***************************************************************************************************\
	|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
	| TCAVLLinkAutoRemove																				|
	|___________________________________________________________________________________________________|
	\***************************************************************************************************/

	template <EAVLLinkType t_LinkType, typename t_CLinkData>
	TCAVLLinkAutoRemove<t_LinkType, t_CLinkData>::TCAVLLinkAutoRemove()
	{
	}

	template <EAVLLinkType t_LinkType, typename t_CLinkData>
	TCAVLLinkAutoRemove<t_LinkType, t_CLinkData>::~TCAVLLinkAutoRemove()
	{
		if (TCAVLLink<t_LinkType, t_CLinkData>::m_Link.f_GetSkew() != t_CLinkData::EAVLTreeSkew_NotInTree)
			m_fRemove(m_pTree, TCAVLLink<t_LinkType, t_CLinkData>::m_Link);
	}

	template <EAVLLinkType t_LinkType, typename t_CLinkData>
	inline_small void TCAVLLinkAutoRemove<t_LinkType, t_CLinkData>::f_SetTree(void *_pTree, FRemove *_fRemove)
	{
		m_pTree = _pTree;
		m_fRemove = _fRemove;
	}
}
