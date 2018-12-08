// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

#include <Mib/Core/Core>

namespace NMib::NIntrusive::NPrivate
{
	template <typename t_CPointerHolder = CDefaultPointerHolder>
	class TCAVLLinkData
	{
		typedef TCAVLLinkData CThis;
	public:
		typedef TCDynamicPtr<t_CPointerHolder, TCAVLLinkData> CLinkPointer;

		enum EAVLTreeSkew
		{
			 EAVLTreeSkew_None		= 2
			,EAVLTreeSkew_Left		= 0
			,EAVLTreeSkew_Right		= 1
			,EAVLTreeSkew_NotInTree	= 3
		};
		CLinkPointer m_pNext[2];
		aint m_Skew;

		static inline_small bint fs_GetPtrIsFast()
		{
			return true;
		}

		inline_small void f_Construct()
		{
			m_Skew = EAVLTreeSkew_NotInTree;
		}

		inline_small CLinkPointer &f_GetRight()
		{
			return m_pNext[1];
		}
		inline_small CLinkPointer &f_GetLeft()
		{
			return m_pNext[0];
		}

		inline_small CLinkPointer &f_GetNext(aint _iIndex)
		{
			return m_pNext[_iIndex];
		}

		inline_small TCAVLLinkData *f_GetRightP() const
		{
			return m_pNext[1];
		}
		inline_small TCAVLLinkData *f_GetLeftP() const
		{
			return m_pNext[0];
		}

		inline_small TCAVLLinkData *f_GetNextP(aint _iIndex) const
		{
			return m_pNext[_iIndex];
		}

		inline_small aint f_GetSkew() const
		{
			return m_Skew;
		}

		inline_small bint f_IsBalanced() const
		{
			return m_Skew == EAVLTreeSkew_None;
		}


		inline_small void f_SetRight(TCAVLLinkData *_pRight)
		{
			m_pNext[1] = _pRight;
		}

		inline_small void f_SetLeft(TCAVLLinkData *_pLeft)
		{
			m_pNext[0] = _pLeft;
		}

		inline_small void fp_SetNext(int _iIndex, TCAVLLinkData *_pPtr)
		{
			m_pNext[_iIndex] = _pPtr;
		}

		static inline_small void f_Assign(CLinkPointer &_pDest, TCAVLLinkData *_pSrc)
		{
			_pDest = _pSrc;
		}

		static inline_small void f_Assign(CLinkPointer &_pDest, CLinkPointer &_pSrc)
		{
			_pDest = _pSrc;
		}

		inline_small void f_SetSkew(mint _Skew)
		{
			m_Skew = _Skew;
		}

		static inline_small TCAVLLinkData *fs_GetPtr(const CLinkPointer &_Src)
		{
			return (TCAVLLinkData *)_Src;
		}

		inline_small void f_SetAll(TCAVLLinkData *_pLeft, TCAVLLinkData *_pRight, mint _Skew)
		{
			m_pNext[0] = _pLeft;
			m_pNext[1] = _pRight;
			m_Skew = _Skew;
		}

		inline_small void f_Clear()
		{
			m_pNext[0] = 0;
			m_pNext[1] = 0;
			m_Skew = EAVLTreeSkew_None;
		}

	};

	/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
	|	Template:			This class is a template so some more					|
	|						functions can be inlined								|
	|																				|
	|	Parameters:																	|
	|		t_Dummy:		description												|
	\*_____________________________________________________________________________*/
	template <typename t_CPointerHolder = CDefaultPointerHolder>
	class TCAVLLinkAlignedData
	{
	public:
		typedef TCAVLLinkAlignedData CThis;
		typedef TCDynamicPtr<t_CPointerHolder, TCAVLLinkAlignedData> CLinkPointer;

		enum EAVLTreeSkew
		{
			 EAVLTreeSkew_None		= 2
			,EAVLTreeSkew_Left		= 0
			,EAVLTreeSkew_Right		= 1
			,EAVLTreeSkew_NotInTree	= 3
		};

#ifndef DCompiler_MSVC
		constexpr TCAVLLinkAlignedData()
			: m_pNext{EAggregateInitialization_Force, EAggregateInitialization_Force}
		{
		}
#endif

		CLinkPointer m_pNext[2];

		static inline_small bint fs_GetPtrIsFast()
		{
			return false;
		}

		inline_small void f_Construct()
		{
			f_SetAll(0, 0, EAVLTreeSkew_NotInTree);
		}

		inline_small CLinkPointer &f_GetRight()
		{
			return m_pNext[1];
		}
		inline_small CLinkPointer &f_GetLeft()
		{
			return m_pNext[0];
		}

		inline_small CLinkPointer &f_GetNext(aint _iIndex)
		{
			return m_pNext[_iIndex];
		}

		inline_small TCAVLLinkAlignedData *f_GetRightP() const
		{
			return (TCAVLLinkAlignedData *)((mint)(TCAVLLinkAlignedData *)m_pNext[1] & (~mint(0x1)));
		}
		inline_small TCAVLLinkAlignedData *f_GetLeftP() const
		{
			return (TCAVLLinkAlignedData *)((mint)(TCAVLLinkAlignedData *)m_pNext[0] & (~mint(0x1)));
		}

		inline_small TCAVLLinkAlignedData *f_GetNextP(aint _iIndex) const
		{
			return (TCAVLLinkAlignedData *)((mint)(TCAVLLinkAlignedData *)m_pNext[_iIndex] & (~mint(0x1)));
		}

		inline_small mint f_GetSkew() const
		{
			return (((mint)(TCAVLLinkAlignedData *)m_pNext[0] & 1) | ((mint)(TCAVLLinkAlignedData *)m_pNext[1] & 1) << 1);
		}

		inline_small bint f_IsBalanced() const
		{
			return f_GetSkew() == EAVLTreeSkew_None;
		}


		static inline_small TCAVLLinkAlignedData *fs_GetPtr(const CLinkPointer &_Src)
		{
			return (TCAVLLinkAlignedData *)((mint)(TCAVLLinkAlignedData *)_Src & (~mint(0x1)));
		}

		inline_small void f_SetRight(TCAVLLinkAlignedData *_pRight)
		{
			DMibFastCheck((!((mint)_pRight & 1))); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[1] = (TCAVLLinkAlignedData *)(((mint)(TCAVLLinkAlignedData *)m_pNext[1] & 1) | (mint)_pRight);
		}

		inline_small void f_SetLeft(TCAVLLinkAlignedData *_pLeft)
		{
			DMibFastCheck(!((mint)_pLeft & 1)); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[0] = (TCAVLLinkAlignedData *)(((mint)(TCAVLLinkAlignedData *)m_pNext[0] & 1) | (mint)_pLeft);
		}

		inline_small void fp_SetNext(int _iIndex, TCAVLLinkAlignedData *_pPtr)
		{
			DMibFastCheck(!((mint)_pPtr & 1)); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[_iIndex] = (TCAVLLinkAlignedData *)(((mint)(TCAVLLinkAlignedData *)m_pNext[_iIndex] & 1) | (mint)_pPtr);
		}

		inline_small void f_SetRight(CLinkPointer &_pRight)
		{
			m_pNext[1] = (TCAVLLinkAlignedData *)(((mint)(TCAVLLinkAlignedData *)m_pNext[1] & mint(1)) | ((mint)(TCAVLLinkAlignedData *)_pRight & (~mint(0x1))));
		}

		inline_small void f_SetLeft(CLinkPointer &_pLeft)
		{
			m_pNext[0] = (TCAVLLinkAlignedData *)(((mint)(TCAVLLinkAlignedData *)m_pNext[0] & mint(1)) | ((mint)(TCAVLLinkAlignedData *)_pLeft & (~mint(0x1))));
		}

		static inline_small void f_Assign(CLinkPointer &_Dest, TCAVLLinkAlignedData *_pSrc)
		{
			DMibFastCheck(!((mint)_pSrc & mint(1))); // We can only save ptrs that are aligned on 2 bytes
			_Dest = (TCAVLLinkAlignedData *)((((mint)(TCAVLLinkAlignedData *)_Dest) & mint(1)) | (mint)_pSrc);
		}

		static inline_small void f_Assign(CLinkPointer &_Dest, CLinkPointer &_Src)
		{
			_Dest = (TCAVLLinkAlignedData *)((((mint)(TCAVLLinkAlignedData *)_Dest) & mint(1)) | (((mint)(TCAVLLinkAlignedData *)_Src) & (~mint(0x1))));
		}

		inline_small void f_SetSkew(mint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_pNext[0] = (TCAVLLinkAlignedData *)(((mint)(TCAVLLinkAlignedData *)m_pNext[0]&(~1)) | (_Skew & 1));
			m_pNext[1] = (TCAVLLinkAlignedData *)(((mint)(TCAVLLinkAlignedData *)m_pNext[1]&(~1)) | (_Skew >> 1));
		}

		inline_small void f_SetAll(TCAVLLinkAlignedData *_pLeft, TCAVLLinkAlignedData *_pRight, mint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_pNext[0] = (TCAVLLinkAlignedData *)((mint)_pLeft | (_Skew & 1));
			m_pNext[1] = (TCAVLLinkAlignedData *)((mint)_pRight | (_Skew >> 1));
		}
		inline_small void f_Clear()
		{
			m_pNext[0] = (TCAVLLinkAlignedData *)(EAVLTreeSkew_None & 1);
			m_pNext[1] = (TCAVLLinkAlignedData *)(EAVLTreeSkew_None >> 1);
		}
	};


	template <typename t_CPointerHolder = CDefaultPointerHolder>
	class TCAVLLinkAlignedRightData
	{
		typedef TCAVLLinkAlignedRightData CThis;
	public:
#ifdef DMibDebug
		inline_never TCAVLLinkAlignedRightData *fp_Debug_GetLeft() const
		{
			return f_GetLeftP();
		}
		inline_never TCAVLLinkAlignedRightData *fp_Debug_GetRight() const
		{
			return f_GetRightP();
		}
#endif
	public:

		typedef TCDynamicPtr<t_CPointerHolder, TCAVLLinkAlignedRightData> CLinkPointer;

		enum EAVLTreeSkew
		{
			 EAVLTreeSkew_None		= 2
			,EAVLTreeSkew_Left		= 0
			,EAVLTreeSkew_Right		= 1
			,EAVLTreeSkew_NotInTree	= 3
		};

		CLinkPointer m_pNext[2];

		static inline_small bint fs_GetPtrIsFast()
		{
			return false;
		}

		inline_small void f_Construct()
		{
			f_SetAll(0, 0, EAVLTreeSkew_NotInTree);
		}

		inline_small CLinkPointer &f_GetRight()
		{
			return m_pNext[1];
		}
		inline_small CLinkPointer &f_GetLeft()
		{
			return m_pNext[0];
		}

		inline_small CLinkPointer &f_GetNext(aint _iIndex)
		{
			return m_pNext[_iIndex];
		}

		inline_small TCAVLLinkAlignedRightData *f_GetRightP() const
		{
			return (TCAVLLinkAlignedRightData *)((mint)(TCAVLLinkAlignedRightData *)m_pNext[1] & (~mint(0x3)));
		}
		inline_small TCAVLLinkAlignedRightData *f_GetLeftP() const
		{
			return (TCAVLLinkAlignedRightData *)m_pNext[0];
		}

		inline_small TCAVLLinkAlignedRightData *f_GetNextP(aint _iIndex) const
		{
			return (TCAVLLinkAlignedRightData *)((mint)(TCAVLLinkAlignedRightData *)m_pNext[_iIndex] & (~mint(0x3)));
		}

		inline_small mint f_GetSkew() const
		{
			return ((mint)(TCAVLLinkAlignedRightData *)m_pNext[1] & mint(3));
		}

		inline_small bint f_IsBalanced() const
		{
			return f_GetSkew() == EAVLTreeSkew_None;
		}


		static inline_small TCAVLLinkAlignedRightData *fs_GetPtr(const CLinkPointer &_Src)
		{
			return (TCAVLLinkAlignedRightData *)((mint)(TCAVLLinkAlignedRightData *)_Src & (~mint(0x3)));
		}

		inline_small void f_SetRight(TCAVLLinkAlignedRightData *_pRight)
		{
			DMibFastCheck((!((mint)_pRight & 3))); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[1] = (TCAVLLinkAlignedRightData *)(((mint)(TCAVLLinkAlignedRightData *)m_pNext[1] & mint(3)) | (mint)_pRight);
		}

		inline_small void f_SetLeft(TCAVLLinkAlignedRightData *_pLeft)
		{
			m_pNext[0] = _pLeft;
		}

		inline_small void fp_SetNext(int _iIndex, TCAVLLinkAlignedRightData *_pPtr)
		{
			DMibFastCheck(!((mint)_pPtr & 3)); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[_iIndex] = (TCAVLLinkAlignedRightData *)(((mint)(TCAVLLinkAlignedRightData *)m_pNext[_iIndex] & mint(3)) | (mint)_pPtr);
		}

		inline_small void f_SetRight(CLinkPointer &_pRight)
		{
			m_pNext[1] = (TCAVLLinkAlignedRightData *)(((mint)(TCAVLLinkAlignedRightData *)m_pNext[1] & mint(3)) | ((mint)(TCAVLLinkAlignedRightData *)_pRight & (~mint(0x3))));
		}

		inline_small void f_SetLeft(CLinkPointer &_pLeft)
		{
			m_pNext[0] = (TCAVLLinkAlignedRightData *)((mint)(TCAVLLinkAlignedRightData *)_pLeft & (~mint(0x3)));
		}

		static inline_small void f_Assign(CLinkPointer &_Dest, TCAVLLinkAlignedRightData *_pSrc)
		{
			DMibFastCheck(!((mint)_pSrc & 3)); // We can only save ptrs that are aligned on 2 bytes
			_Dest = (TCAVLLinkAlignedRightData *)(((mint)(TCAVLLinkAlignedRightData *)_Dest & mint(3)) | (mint)_pSrc);
		}

		static inline_small void f_Assign(CLinkPointer &_Dest, CLinkPointer &_Src)
		{
			_Dest = (TCAVLLinkAlignedRightData *)(((mint)(TCAVLLinkAlignedRightData *)_Dest & mint(3)) | ((mint)(TCAVLLinkAlignedRightData *)_Src & (~mint(0x3))));
		}

		inline_small void f_SetSkew(mint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_pNext[1] = (TCAVLLinkAlignedRightData *)(((mint)(TCAVLLinkAlignedRightData *)m_pNext[1]&(~mint(3))) | (_Skew));
		}

		inline_small void f_SetAll(TCAVLLinkAlignedRightData *_pLeft, TCAVLLinkAlignedRightData *_pRight, mint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_pNext[0] = _pLeft;
			m_pNext[1] = (TCAVLLinkAlignedRightData *)((mint)_pRight | (_Skew));
		}
		inline_small void f_Clear()
		{
			m_pNext[0] = nullptr;
			m_pNext[1] = (TCAVLLinkAlignedRightData *)mint(EAVLTreeSkew_None);
		}
	};

	template <EAVLLinkType t_LinkType>
	struct TCLinkTypeToLink
	{
		typedef void CType;
	};
	template <>
	struct TCLinkTypeToLink<EAVLLinkType_Aligned>
	{
		typedef TCAVLLinkAlignedData<> CType;
	};
	template <>
	struct TCLinkTypeToLink<EAVLLinkType_AlignedRight>
	{
		typedef TCAVLLinkAlignedRightData<> CType;
	};
	template <>
	struct TCLinkTypeToLink<EAVLLinkType_Unaligned>
	{
		typedef TCAVLLinkData<> CType;
	};
}
