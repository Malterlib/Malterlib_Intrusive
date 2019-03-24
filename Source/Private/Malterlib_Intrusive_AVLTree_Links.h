// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

#include <Mib/Core/Core>

namespace NMib::NIntrusive::NPrivate
{
	class CAVLLinkData
	{
		typedef CAVLLinkData CThis;
	public:
		enum EAVLTreeSkew
		{
			 EAVLTreeSkew_None		= 2
			,EAVLTreeSkew_Left		= 0
			,EAVLTreeSkew_Right		= 1
			,EAVLTreeSkew_NotInTree	= 3
		};
		CAVLLinkData *m_pNext[2];
		aint m_Skew;

		static inline_small bint fs_GetPtrIsFast()
		{
			return true;
		}

		inline_small void f_Construct()
		{
			m_Skew = EAVLTreeSkew_NotInTree;
		}

		inline_small CAVLLinkData **f_GetRight()
		{
			return &m_pNext[1];
		}
		inline_small CAVLLinkData **f_GetLeft()
		{
			return &m_pNext[0];
		}

		inline_small CAVLLinkData **f_GetNext(aint _iIndex)
		{
			return &m_pNext[_iIndex];
		}

		inline_small CAVLLinkData *f_GetRightP() const
		{
			return m_pNext[1];
		}
		inline_small CAVLLinkData *f_GetLeftP() const
		{
			return m_pNext[0];
		}

		inline_small CAVLLinkData *f_GetNextP(aint _iIndex) const
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


		inline_small void f_SetRight(CAVLLinkData *_pRight)
		{
			m_pNext[1] = _pRight;
		}

		inline_small void f_SetLeft(CAVLLinkData *_pLeft)
		{
			m_pNext[0] = _pLeft;
		}

		inline_small void f_SetRight(CAVLLinkData **_pRight)
		{
			m_pNext[1] = *_pRight;
		}

		inline_small void f_SetLeft(CAVLLinkData **_pLeft)
		{
			m_pNext[0] = *_pLeft;
		}

		inline_small void fp_SetNext(int _iIndex, CAVLLinkData *_pPtr)
		{
			m_pNext[_iIndex] = _pPtr;
		}

		static inline_small void f_Assign(CAVLLinkData **_pDest, CAVLLinkData *_pSrc)
		{
			*_pDest = _pSrc;
		}

		static inline_small void f_Assign(CAVLLinkData **_pDest, CAVLLinkData **_pSrc)
		{
			*_pDest = *_pSrc;
		}

		inline_small void f_SetSkew(mint _Skew)
		{
			m_Skew = _Skew;
		}

		static inline_small CAVLLinkData *fs_GetPtr(CAVLLinkData *_Src)
		{
			return _Src;
		}

		inline_small void f_SetAll(CAVLLinkData *_pLeft, CAVLLinkData *_pRight, mint _Skew)
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
	class CAVLLinkAlignedData
	{
	public:
		typedef CAVLLinkAlignedData CThis;

		enum EAVLTreeSkew
		{
			 EAVLTreeSkew_None		= 2
			,EAVLTreeSkew_Left		= 0
			,EAVLTreeSkew_Right		= 1
			,EAVLTreeSkew_NotInTree	= 3
		};

#ifndef DCompiler_MSVC
		constexpr CAVLLinkAlignedData()
			: m_pNext{nullptr, nullptr}
		{
		}
#endif

		CAVLLinkAlignedData * m_pNext[2];

		static inline_small bint fs_GetPtrIsFast()
		{
			return false;
		}

		inline_small void f_Construct()
		{
			f_SetAll(0, 0, EAVLTreeSkew_NotInTree);
		}

		inline_small CAVLLinkAlignedData **f_GetRight()
		{
			return &m_pNext[1];
		}
		inline_small CAVLLinkAlignedData **f_GetLeft()
		{
			return &m_pNext[0];
		}

		inline_small CAVLLinkAlignedData **f_GetNext(aint _iIndex)
		{
			return &m_pNext[_iIndex];
		}

		inline_small CAVLLinkAlignedData *f_GetRightP() const
		{
			return (CAVLLinkAlignedData *)((mint)m_pNext[1] & (~mint(0x1)));
		}
		inline_small CAVLLinkAlignedData *f_GetLeftP() const
		{
			return (CAVLLinkAlignedData *)((mint)m_pNext[0] & (~mint(0x1)));
		}

		inline_small CAVLLinkAlignedData *f_GetNextP(aint _iIndex) const
		{
			return (CAVLLinkAlignedData *)((mint)m_pNext[_iIndex] & (~mint(0x1)));
		}

		inline_small mint f_GetSkew() const
		{
			return (((mint)m_pNext[0] & 1) | ((mint)m_pNext[1] & 1) << 1);
		}

		inline_small bint f_IsBalanced() const
		{
			return f_GetSkew() == EAVLTreeSkew_None;
		}


		static inline_small CAVLLinkAlignedData *fs_GetPtr(CAVLLinkAlignedData const *_Src)
		{
			return (CAVLLinkAlignedData *)((mint)_Src & (~mint(0x1)));
		}

		inline_small void f_SetRight(CAVLLinkAlignedData *_pRight)
		{
			DMibFastCheck((!((mint)_pRight & 1))); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[1] = (CAVLLinkAlignedData *)(((mint)m_pNext[1] & 1) | (mint)_pRight);
		}

		inline_small void f_SetLeft(CAVLLinkAlignedData *_pLeft)
		{
			DMibFastCheck(!((mint)_pLeft & 1)); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[0] = (CAVLLinkAlignedData *)(((mint)m_pNext[0] & 1) | (mint)_pLeft);
		}

		inline_small void fp_SetNext(int _iIndex, CAVLLinkAlignedData *_pPtr)
		{
			DMibFastCheck(!((mint)_pPtr & 1)); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[_iIndex] = (CAVLLinkAlignedData *)(((mint)m_pNext[_iIndex] & 1) | (mint)_pPtr);
		}

		inline_small void f_SetRight(CAVLLinkAlignedData **_pRight)
		{
			m_pNext[1] = (CAVLLinkAlignedData *)(((mint)m_pNext[1] & mint(1)) | ((mint)*_pRight & (~mint(0x1))));
		}

		inline_small void f_SetLeft(CAVLLinkAlignedData **_pLeft)
		{
			m_pNext[0] = (CAVLLinkAlignedData *)(((mint)m_pNext[0] & mint(1)) | ((mint)*_pLeft & (~mint(0x1))));
		}

		static inline_small void f_Assign(CAVLLinkAlignedData **_Dest, CAVLLinkAlignedData *_pSrc)
		{
			DMibFastCheck(!((mint)_pSrc & mint(1))); // We can only save ptrs that are aligned on 2 bytes
			*_Dest = (CAVLLinkAlignedData *)((((mint)*_Dest) & mint(1)) | (mint)_pSrc);
		}

		static inline_small void f_Assign(CAVLLinkAlignedData **_Dest, CAVLLinkAlignedData **_pSrc)
		{
			*_Dest = (CAVLLinkAlignedData *)((((mint)*_Dest) & mint(1)) | (((mint)*_pSrc) & (~mint(0x1))));
		}

		inline_small void f_SetSkew(mint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_pNext[0] = (CAVLLinkAlignedData *)(((mint)m_pNext[0]&(~1)) | (_Skew & 1));
			m_pNext[1] = (CAVLLinkAlignedData *)(((mint)m_pNext[1]&(~1)) | (_Skew >> 1));
		}

		inline_small void f_SetAll(CAVLLinkAlignedData *_pLeft, CAVLLinkAlignedData *_pRight, mint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_pNext[0] = (CAVLLinkAlignedData *)((mint)_pLeft | (_Skew & 1));
			m_pNext[1] = (CAVLLinkAlignedData *)((mint)_pRight | (_Skew >> 1));
		}
		inline_small void f_Clear()
		{
			m_pNext[0] = (CAVLLinkAlignedData *)(EAVLTreeSkew_None & 1);
			m_pNext[1] = (CAVLLinkAlignedData *)(EAVLTreeSkew_None >> 1);
		}
	};

	class CAVLLinkAlignedRightData
	{
		typedef CAVLLinkAlignedRightData CThis;
	public:
#ifdef DMibDebug
		inline_never CAVLLinkAlignedRightData *fp_Debug_GetLeft() const
		{
			return f_GetLeftP();
		}
		inline_never CAVLLinkAlignedRightData *fp_Debug_GetRight() const
		{
			return f_GetRightP();
		}
#endif
	public:

		enum EAVLTreeSkew
		{
			 EAVLTreeSkew_None		= 2
			,EAVLTreeSkew_Left		= 0
			,EAVLTreeSkew_Right		= 1
			,EAVLTreeSkew_NotInTree	= 3
		};

		CAVLLinkAlignedRightData * m_pNext[2];

		static inline_small bint fs_GetPtrIsFast()
		{
			return false;
		}

		inline_small void f_Construct()
		{
			f_SetAll(0, 0, EAVLTreeSkew_NotInTree);
		}

		inline_small CAVLLinkAlignedRightData **f_GetRight()
		{
			return &m_pNext[1];
		}
		inline_small CAVLLinkAlignedRightData **f_GetLeft()
		{
			return &m_pNext[0];
		}

		inline_small CAVLLinkAlignedRightData **f_GetNext(aint _iIndex)
		{
			return &m_pNext[_iIndex];
		}

		inline_small CAVLLinkAlignedRightData *f_GetRightP() const
		{
			return (CAVLLinkAlignedRightData *)((mint)m_pNext[1] & (~mint(0x3)));
		}
		inline_small CAVLLinkAlignedRightData *f_GetLeftP() const
		{
			return m_pNext[0];
		}

		inline_small CAVLLinkAlignedRightData *f_GetNextP(aint _iIndex) const
		{
			return (CAVLLinkAlignedRightData *)((mint)m_pNext[_iIndex] & (~mint(0x3)));
		}

		inline_small mint f_GetSkew() const
		{
			return ((mint)m_pNext[1] & mint(3));
		}

		inline_small bint f_IsBalanced() const
		{
			return f_GetSkew() == EAVLTreeSkew_None;
		}


		static inline_small CAVLLinkAlignedRightData *fs_GetPtr(CAVLLinkAlignedRightData *_Src)
		{
			return (CAVLLinkAlignedRightData *)((mint)_Src & (~mint(0x3)));
		}

		inline_small void f_SetRight(CAVLLinkAlignedRightData *_pRight)
		{
			DMibFastCheck((!((mint)_pRight & 3))); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[1] = (CAVLLinkAlignedRightData *)(((mint)m_pNext[1] & mint(3)) | (mint)_pRight);
		}

		inline_small void f_SetLeft(CAVLLinkAlignedRightData *_pLeft)
		{
			m_pNext[0] = _pLeft;
		}

		inline_small void fp_SetNext(int _iIndex, CAVLLinkAlignedRightData *_pPtr)
		{
			DMibFastCheck(!((mint)_pPtr & 3)); // We can only save ptrs that are aligned on 2 bytes
			m_pNext[_iIndex] = (CAVLLinkAlignedRightData *)(((mint)m_pNext[_iIndex] & mint(3)) | (mint)_pPtr);
		}

		inline_small void f_SetRight(CAVLLinkAlignedRightData **_pRight)
		{
			m_pNext[1] = (CAVLLinkAlignedRightData *)(((mint)m_pNext[1] & mint(3)) | ((mint)*_pRight & (~mint(0x3))));
		}

		inline_small void f_SetLeft(CAVLLinkAlignedRightData **_pLeft)
		{
			m_pNext[0] = (CAVLLinkAlignedRightData *)((mint)*_pLeft & (~mint(0x3)));
		}

		static inline_small void f_Assign(CAVLLinkAlignedRightData **_Dest, CAVLLinkAlignedRightData *_pSrc)
		{
			DMibFastCheck(!((mint)_pSrc & 3)); // We can only save ptrs that are aligned on 2 bytes
			*_Dest = (CAVLLinkAlignedRightData *)(((mint)*_Dest & mint(3)) | (mint)_pSrc);
		}

		static inline_small void f_Assign(CAVLLinkAlignedRightData **_Dest, CAVLLinkAlignedRightData **_pSrc)
		{
			*_Dest = (CAVLLinkAlignedRightData *)(((mint)*_Dest & mint(3)) | ((mint)*_pSrc & (~mint(0x3))));
		}

		inline_small void f_SetSkew(mint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_pNext[1] = (CAVLLinkAlignedRightData *)(((mint)m_pNext[1]&(~mint(3))) | (_Skew));
		}

		inline_small void f_SetAll(CAVLLinkAlignedRightData *_pLeft, CAVLLinkAlignedRightData *_pRight, mint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_pNext[0] = _pLeft;
			m_pNext[1] = (CAVLLinkAlignedRightData *)((mint)_pRight | (_Skew));
		}
		inline_small void f_Clear()
		{
			m_pNext[0] = nullptr;
			m_pNext[1] = (CAVLLinkAlignedRightData *)mint(EAVLTreeSkew_None);
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
		typedef CAVLLinkAlignedData CType;
	};
	template <>
	struct TCLinkTypeToLink<EAVLLinkType_AlignedRight>
	{
		typedef CAVLLinkAlignedRightData CType;
	};
	template <>
	struct TCLinkTypeToLink<EAVLLinkType_Unaligned>
	{
		typedef CAVLLinkData CType;
	};
}
