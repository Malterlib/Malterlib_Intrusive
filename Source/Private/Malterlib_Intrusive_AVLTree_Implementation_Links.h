// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib
{
	namespace NIntrusive
	{
		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| TCAVLLinkAggregate																				|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData>
		inline_small void TCAVLLinkAggregate<_LinkType, t_CLinkData>::f_Construct()
		{
			m_Link.f_Construct();
		}

		template <CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData>
		template <typename tf_CTree>
		inline_small void TCAVLLinkAggregate<_LinkType, t_CLinkData>::f_Destruct(tf_CTree *_pTree)
		{
			DMibFastCheck(_pTree); // You have to supply the tree to the destructor
			_pTree->f_Remove(this);
		}
		template <CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData>
		inline_small void TCAVLLinkAggregate<_LinkType, t_CLinkData>::f_Destruct()
		{
#		if DMibEnableSafeCheck > 0
			aint Skew = m_Link.f_GetSkew();
			DMibFastCheck(Skew == t_CLinkData::EAVLTreeSkew_NotInTree); // You have to remove the link from the tree yourself.
#			endif
		}

		template <CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData>
		bint TCAVLLinkAggregate<_LinkType, t_CLinkData>::f_IsInTree() const
		{
			return m_Link.f_GetSkew() != t_CLinkData::EAVLTreeSkew_NotInTree;
		}

		template <CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData>
		template <typename tf_CTree>
		inline_small void TCAVLLinkAggregate<_LinkType, t_CLinkData>::f_SetTree(tf_CTree *_pTree)
		{				
		}


		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| TCAVLLink																							|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData>
		TCAVLLink<_LinkType, t_CLinkData>::TCAVLLink()
		{
			this->f_Construct();
		}

		template <CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData>
		TCAVLLink<_LinkType, t_CLinkData>::~TCAVLLink()
		{
			this->f_Destruct();
		}


		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| TCAVLLinkAutoRemoveAggregate																		|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CCompare, typename t_CAllocator, CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData, typename t_CLinkTraits>
		inline_small void TCAVLLinkAutoRemoveAggregate<t_CCompare, t_CAllocator, _LinkType, t_CLinkData, t_CLinkTraits>::f_Construct()
		{
			m_Link.f_Construct();
		}

		template <typename t_CCompare, typename t_CAllocator, CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData, typename t_CLinkTraits>
		inline_small void TCAVLLinkAutoRemoveAggregate<t_CCompare, t_CAllocator, _LinkType, t_CLinkData, t_CLinkTraits>::f_Destruct()
		{
			if (m_Link.f_GetSkew() != t_CLinkData::EAVLTreeSkew_NotInTree)
				m_pTree->f_Remove(TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator>::fs_MemberFromLink(&m_Link));
		}

		template <typename t_CCompare, typename t_CAllocator, CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData, typename t_CLinkTraits>
		bint TCAVLLinkAutoRemoveAggregate<t_CCompare, t_CAllocator, _LinkType, t_CLinkData, t_CLinkTraits>::f_IsInTree() const
		{
			return m_Link.f_GetSkew() != t_CLinkData::EAVLTreeSkew_NotInTree;
		}

		template <typename t_CCompare, typename t_CAllocator, CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData, typename t_CLinkTraits>
		inline_small void 
		TCAVLLinkAutoRemoveAggregate<t_CCompare, t_CAllocator, _LinkType, t_CLinkData, t_CLinkTraits>
		::f_SetTree(TCAVLTreeAggregate<t_CLinkTraits, t_CCompare, t_CAllocator> *_pTree)
		{				
			m_pTree = _pTree;
		}

		/***************************************************************************************************\
		|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
		| TCAVLLinkAutoRemove																				|
		|___________________________________________________________________________________________________|
		\***************************************************************************************************/

		template <typename t_CCompare, typename t_CAllocator, CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData, typename t_CLinkTraits>
		TCAVLLinkAutoRemove<t_CCompare, t_CAllocator, _LinkType, t_CLinkData, t_CLinkTraits>::TCAVLLinkAutoRemove()
		{
			this->f_Construct();
		}

		template <typename t_CCompare, typename t_CAllocator, CAVLLinkTypeUnderlaying _LinkType, typename t_CLinkData, typename t_CLinkTraits>
		TCAVLLinkAutoRemove<t_CCompare, t_CAllocator, _LinkType, t_CLinkData, t_CLinkTraits>::~TCAVLLinkAutoRemove()
		{
			this->f_Destruct();
		}


	}
}

