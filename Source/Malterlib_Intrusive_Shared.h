// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

#include <Mib/Core/Core>

namespace NMib
{
	namespace NIntrusive
	{
		template <typename t_CType, typename t_CLinkTraits>
		struct TCReplaceLinkTraits
		{
			typedef t_CType CType;
		};

#		define DMibIntrusiveDefine(_Member) \
			template <typename t_CClass> \
			struct TCIntrusive_##_Member \
			{\
				enum\
				{\
					mc_Offset = DMibPOffsetOf(t_CClass, _Member)\
				};\
			};

#		define DMibIntrusiveLink_Define(_ParentClass, _LinkType, _Member) \
			struct CLinkTraits_##_Member \
			{\
				typedef _ParentClass CNode;\
				typedef NMib::NIntrusive::TCReplaceLinkTraits<_LinkType, CLinkTraits_##_Member>::CType CLinkContainer;\
				template <typename t_CParentClass>\
				struct TCOffset\
				{\
					enum\
					{\
						mc_Offset = (aint)DMibPOffsetOf(t_CParentClass, _Member)\
					};\
				};\
			};

#		define DMibIntrusiveLinkT_Define(_ParentClass, _LinkType, _Member) \
			struct CLinkTraits_##_Member \
			{\
				typedef _ParentClass CNode;\
				typedef typename NMib::NIntrusive::TCReplaceLinkTraits<_LinkType, CLinkTraits_##_Member>::CType CLinkContainer;\
				template <typename t_CParentClass>\
				struct TCOffset\
				{\
					enum\
					{\
						mc_Offset = (aint)DMibPOffsetOf(t_CParentClass, _Member)\
					};\
				};\
			};\
		
#		define DMibIntrusiveLink_Member(_Member) \
			CLinkTraits_##_Member::CLinkContainer _Member;

#		define DMibIntrusiveLinkT_Member(_Member) \
			typename CLinkTraits_##_Member::CLinkContainer _Member;
		
#		define DMibIntrusiveLink(_ParentClass, _LinkType, _Member) \
			DMibIntrusiveLink_Define(_ParentClass, _LinkType, _Member) \
			DMibIntrusiveLink_Member(_Member)
			
#		define DMibIntrusiveLinkT(_ParentClass, _LinkType, _Member) \
			DMibIntrusiveLinkT_Define(_ParentClass, _LinkType, _Member) \
			DMibIntrusiveLinkT_Member(_Member)

#		ifndef DMibPNoShortCuts
#			define DIntrusiveLink_Member DMibIntrusiveLink_Member
#			define DIntrusiveLink_Define DMibIntrusiveLink_Define
#			define DIntrusiveLink DMibIntrusiveLink
#			define DIntrusiveLinkT_Member DMibIntrusiveLinkT_Member
#			define DIntrusiveLinkT_Define DMibIntrusiveLinkT_Define
#			define DIntrusiveLinkT DMibIntrusiveLinkT
#		endif
	}

}
