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

#		define DMibIntrusiveLink(_ParentClass, _LinkType, _Member) \
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
			};\
			CLinkTraits_##_Member::CLinkContainer _Member;
#		define DMibIntrusiveLinkT(_ParentClass, _LinkType, _Member) \
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
			typename CLinkTraits_##_Member::CLinkContainer _Member;

#		ifndef DMibPNoShortCuts
#			define DIntrusiveLink DMibIntrusiveLink
#			define DIntrusiveLinkT DMibIntrusiveLinkT
#		endif
	}

}
