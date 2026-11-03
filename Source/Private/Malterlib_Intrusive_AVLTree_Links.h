// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <Mib/Core/Core>

namespace NMib::NIntrusive::NPrivate
{
	struct CAVLLinkData
	{
		enum EAVLTreeSkew
		{
			EAVLTreeSkew_None        = 2
			, EAVLTreeSkew_Left      = 0
			, EAVLTreeSkew_Right     = 1
			, EAVLTreeSkew_NotInTree = 3
		};

		CAVLLinkData *m_Next[2];
		aint m_Skew;

		static inline_small bool fs_GetPtrIsFast()
		{
			return true;
		}

		inline_small void f_Construct()
		{
			m_Skew = EAVLTreeSkew_NotInTree;
		}

		inline_small CAVLLinkData **f_GetRight()
		{
			return &m_Next[1];
		}

		inline_small CAVLLinkData **f_GetLeft()
		{
			return &m_Next[0];
		}

		inline_small CAVLLinkData **f_GetNext(aint _iIndex)
		{
			return &m_Next[_iIndex];
		}

		inline_small CAVLLinkData *f_GetRightP() const
		{
			return m_Next[1];
		}

		inline_small CAVLLinkData *f_GetLeftP() const
		{
			return m_Next[0];
		}

		inline_small CAVLLinkData *f_GetNextP(aint _iIndex) const
		{
			return m_Next[_iIndex];
		}

		inline_small aint f_GetSkew() const
		{
			return m_Skew;
		}

		inline_small bool f_IsBalanced() const
		{
			return m_Skew == EAVLTreeSkew_None;
		}

		inline_small void f_SetRight(CAVLLinkData *_pRight)
		{
			m_Next[1] = _pRight;
		}

		inline_small void f_SetLeft(CAVLLinkData *_pLeft)
		{
			m_Next[0] = _pLeft;
		}

		inline_small void f_SetRight(CAVLLinkData **_pRight)
		{
			m_Next[1] = *_pRight;
		}

		inline_small void f_SetLeft(CAVLLinkData **_pLeft)
		{
			m_Next[0] = *_pLeft;
		}

		inline_small void f_SetNext(int _iIndex, CAVLLinkData *_pPtr)
		{
			m_Next[_iIndex] = _pPtr;
		}

		static inline_small void fs_Assign(CAVLLinkData **_pDest, CAVLLinkData *_pSrc)
		{
			*_pDest = _pSrc;
		}

		static inline_small void fs_AssignInit(CAVLLinkData **_pDest, CAVLLinkData *_pSrc)
		{
			*_pDest = _pSrc;
		}

		static inline_small void fs_Assign(CAVLLinkData **_pDest, CAVLLinkData **_pSrc)
		{
			*_pDest = *_pSrc;
		}

		inline_small void f_SetSkew(umint _Skew)
		{
			m_Skew = _Skew;
		}

		static constexpr inline_small CAVLLinkData *fs_GetPtr(CAVLLinkData *_pSrc)
		{
			return _pSrc;
		}

		inline_small void f_SetAll(CAVLLinkData *_pLeft, CAVLLinkData *_pRight, umint _Skew)
		{
			m_Next[0] = _pLeft;
			m_Next[1] = _pRight;
			m_Skew = _Skew;
		}

		inline_small void f_Clear()
		{
			m_Next[0] = 0;
			m_Next[1] = 0;
			m_Skew = EAVLTreeSkew_None;
		}

	private:
		using CThis = CAVLLinkData;
	};

	/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*\
	|	Template:			This class is a template so some more					|
	|						functions can be inlined								|
	|																				|
	|	Parameters:																	|
	|		t_Dummy:		description												|
	\*_____________________________________________________________________________*/
	struct CAVLLinkAlignedData
	{
		using CThis = CAVLLinkAlignedData;

		enum EAVLTreeSkew
		{
			EAVLTreeSkew_None        = 2
			, EAVLTreeSkew_Left      = 0
			, EAVLTreeSkew_Right     = 1
			, EAVLTreeSkew_NotInTree = 3
		};

#ifndef DCompiler_MSVC
		constexpr CAVLLinkAlignedData()
			: m_Next{nullptr, nullptr}
		{
		}
#endif

		CAVLLinkAlignedData *m_Next[2];

		static inline_small bool fs_GetPtrIsFast()
		{
			return false;
		}

		inline_small void f_Construct()
		{
			f_SetAll(0, 0, EAVLTreeSkew_NotInTree);
		}

		inline_small CAVLLinkAlignedData **f_GetRight()
		{
			return &m_Next[1];
		}

		inline_small CAVLLinkAlignedData **f_GetLeft()
		{
			return &m_Next[0];
		}

		inline_small CAVLLinkAlignedData **f_GetNext(aint _iIndex)
		{
			return &m_Next[_iIndex];
		}

		inline_small CAVLLinkAlignedData *f_GetRightP() const
		{
			return (CAVLLinkAlignedData *)((umint)m_Next[1] & (~umint(0x1)));
		}

		inline_small CAVLLinkAlignedData *f_GetLeftP() const
		{
			return (CAVLLinkAlignedData *)((umint)m_Next[0] & (~umint(0x1)));
		}

		inline_small CAVLLinkAlignedData *f_GetNextP(aint _iIndex) const
		{
			return (CAVLLinkAlignedData *)((umint)m_Next[_iIndex] & (~umint(0x1)));
		}

		inline_small umint f_GetSkew() const
		{
			return (((umint)m_Next[0] & 1) | ((umint)m_Next[1] & 1) << 1);
		}

		inline_small bool f_IsBalanced() const
		{
			return f_GetSkew() == EAVLTreeSkew_None;
		}

		static constexpr inline_small CAVLLinkAlignedData *fs_GetPtr(CAVLLinkAlignedData const *_pSrc)
		{
			return (CAVLLinkAlignedData *)((umint)_pSrc & (~umint(0x1)));
		}

		inline_small void f_SetRight(CAVLLinkAlignedData *_pRight)
		{
			DMibFastCheck((!((umint)_pRight & 1))); // We can only save ptrs that are aligned on 2 bytes
			m_Next[1] = (CAVLLinkAlignedData *)(((umint)m_Next[1] & 1) | (umint)_pRight);
		}

		inline_small void f_SetLeft(CAVLLinkAlignedData *_pLeft)
		{
			DMibFastCheck(!((umint)_pLeft & 1)); // We can only save ptrs that are aligned on 2 bytes
			m_Next[0] = (CAVLLinkAlignedData *)(((umint)m_Next[0] & 1) | (umint)_pLeft);
		}

		inline_small void f_SetNext(int _iIndex, CAVLLinkAlignedData *_pPtr)
		{
			DMibFastCheck(!((umint)_pPtr & 1)); // We can only save ptrs that are aligned on 2 bytes
			m_Next[_iIndex] = (CAVLLinkAlignedData *)(((umint)m_Next[_iIndex] & 1) | (umint)_pPtr);
		}

		inline_small void f_SetRight(CAVLLinkAlignedData **_pRight)
		{
			m_Next[1] = (CAVLLinkAlignedData *)(((umint)m_Next[1] & umint(1)) | ((umint)*_pRight & (~umint(0x1))));
		}

		inline_small void f_SetLeft(CAVLLinkAlignedData **_pLeft)
		{
			m_Next[0] = (CAVLLinkAlignedData *)(((umint)m_Next[0] & umint(1)) | ((umint)*_pLeft & (~umint(0x1))));
		}

		static inline_small void fs_Assign(CAVLLinkAlignedData **_pDest, CAVLLinkAlignedData *_pSrc)
		{
			DMibFastCheck(!((umint)_pSrc & umint(1))); // We can only save ptrs that are aligned on 2 bytes
			*_pDest = (CAVLLinkAlignedData *)((((umint)*_pDest) & umint(1)) | (umint)_pSrc);
		}

		static inline_small void fs_AssignInit(CAVLLinkAlignedData **_pDest, CAVLLinkAlignedData *_pSrc)
		{
			DMibFastCheck(!((umint)_pSrc & umint(1))); // We can only save ptrs that are aligned on 2 bytes
			*_pDest = _pSrc;
		}

		static inline_small void fs_Assign(CAVLLinkAlignedData **_pDest, CAVLLinkAlignedData **_pSrc)
		{
			*_pDest = (CAVLLinkAlignedData *)((((umint)*_pDest) & umint(1)) | (((umint)*_pSrc) & (~umint(0x1))));
		}

		inline_small void f_SetSkew(umint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_Next[0] = (CAVLLinkAlignedData *)(((umint)m_Next[0] & (~1)) | (_Skew & 1));
			m_Next[1] = (CAVLLinkAlignedData *)(((umint)m_Next[1] & (~1)) | (_Skew >> 1));
		}

		inline_small void f_SetAll(CAVLLinkAlignedData *_pLeft, CAVLLinkAlignedData *_pRight, umint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_Next[0] = (CAVLLinkAlignedData *)((umint)_pLeft | (_Skew & 1));
			m_Next[1] = (CAVLLinkAlignedData *)((umint)_pRight | (_Skew >> 1));
		}

		inline_small void f_Clear()
		{
			m_Next[0] = (CAVLLinkAlignedData *)(EAVLTreeSkew_None & 1);
			m_Next[1] = (CAVLLinkAlignedData *)(EAVLTreeSkew_None >> 1);
		}
	};

	struct CAVLLinkAlignedRightData
	{
		enum EAVLTreeSkew
		{
			EAVLTreeSkew_None        = 2
			, EAVLTreeSkew_Left      = 0
			, EAVLTreeSkew_Right     = 1
			, EAVLTreeSkew_NotInTree = 3
		};

		CAVLLinkAlignedRightData *m_Next[2];

		static inline_small bool fs_GetPtrIsFast()
		{
			return false;
		}

		inline_small void f_Construct()
		{
			f_SetAll(0, 0, EAVLTreeSkew_NotInTree);
		}

		inline_small CAVLLinkAlignedRightData **f_GetRight()
		{
			return &m_Next[1];
		}

		inline_small CAVLLinkAlignedRightData **f_GetLeft()
		{
			return &m_Next[0];
		}

		inline_small CAVLLinkAlignedRightData **f_GetNext(aint _iIndex)
		{
			return &m_Next[_iIndex];
		}

		inline_small CAVLLinkAlignedRightData *f_GetRightP() const
		{
			return (CAVLLinkAlignedRightData *)((umint)m_Next[1] & (~umint(0x3)));
		}

		inline_small CAVLLinkAlignedRightData *f_GetLeftP() const
		{
			return m_Next[0];
		}

		inline_small CAVLLinkAlignedRightData *f_GetNextP(aint _iIndex) const
		{
			return (CAVLLinkAlignedRightData *)((umint)m_Next[_iIndex] & (~umint(0x3)));
		}

		inline_small umint f_GetSkew() const
		{
			return ((umint)m_Next[1] & umint(3));
		}

		inline_small bool f_IsBalanced() const
		{
			return f_GetSkew() == EAVLTreeSkew_None;
		}

		static constexpr inline_small CAVLLinkAlignedRightData *fs_GetPtr(CAVLLinkAlignedRightData *_pSrc)
		{
			return (CAVLLinkAlignedRightData *)((umint)_pSrc & (~umint(0x3)));
		}

		inline_small void f_SetRight(CAVLLinkAlignedRightData *_pRight)
		{
			DMibFastCheck((!((umint)_pRight & 3))); // We can only save ptrs that are aligned on 2 bytes
			m_Next[1] = (CAVLLinkAlignedRightData *)(((umint)m_Next[1] & umint(3)) | (umint)_pRight);
		}

		inline_small void f_SetLeft(CAVLLinkAlignedRightData *_pLeft)
		{
			m_Next[0] = _pLeft;
		}

		inline_small void f_SetNext(int _iIndex, CAVLLinkAlignedRightData *_pPtr)
		{
			DMibFastCheck(!((umint)_pPtr & 3)); // We can only save ptrs that are aligned on 2 bytes
			m_Next[_iIndex] = (CAVLLinkAlignedRightData *)(((umint)m_Next[_iIndex] & umint(3)) | (umint)_pPtr);
		}

		inline_small void f_SetRight(CAVLLinkAlignedRightData **_pRight)
		{
			m_Next[1] = (CAVLLinkAlignedRightData *)(((umint)m_Next[1] & umint(3)) | ((umint)*_pRight & (~umint(0x3))));
		}

		inline_small void f_SetLeft(CAVLLinkAlignedRightData **_pLeft)
		{
			m_Next[0] = (CAVLLinkAlignedRightData *)((umint)*_pLeft & (~umint(0x3)));
		}

		static inline_small void fs_Assign(CAVLLinkAlignedRightData **_pDest, CAVLLinkAlignedRightData *_pSrc)
		{
			DMibFastCheck(!((umint)_pSrc & 3)); // We can only save ptrs that are aligned on 2 bytes
			*_pDest = (CAVLLinkAlignedRightData *)(((umint)*_pDest & umint(3)) | (umint)_pSrc);
		}

		static inline_small void fs_AssignInit(CAVLLinkAlignedRightData **_pDest, CAVLLinkAlignedRightData *_pSrc)
		{
			DMibFastCheck(!((umint)_pSrc & 3)); // We can only save ptrs that are aligned on 2 bytes
			*_pDest = _pSrc;
		}

		static inline_small void fs_Assign(CAVLLinkAlignedRightData **_pDest, CAVLLinkAlignedRightData **_pSrc)
		{
			*_pDest = (CAVLLinkAlignedRightData *)(((umint)*_pDest & umint(3)) | ((umint)*_pSrc & (~umint(0x3))));
		}

		inline_small void f_SetSkew(umint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_Next[1] = (CAVLLinkAlignedRightData *)(((umint)m_Next[1] & (~umint(3))) | (_Skew));
		}

		inline_small void f_SetAll(CAVLLinkAlignedRightData *_pLeft, CAVLLinkAlignedRightData *_pRight, umint _Skew)
		{
			DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
			m_Next[0] = _pLeft;
			m_Next[1] = (CAVLLinkAlignedRightData *)((umint)_pRight | (_Skew));
		}

		inline_small void f_Clear()
		{
			m_Next[0] = nullptr;
			m_Next[1] = (CAVLLinkAlignedRightData *)umint(EAVLTreeSkew_None);
		}

	private:
		using CThis = CAVLLinkAlignedRightData;

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
	};

	template <EAVLLinkType t_LinkType>
	struct TCLinkTypeToLink
	{
		using CType = void;
	};

	template <>
	struct TCLinkTypeToLink<EAVLLinkType_Aligned>
	{
		using CType = CAVLLinkAlignedData;
	};

	template <>
	struct TCLinkTypeToLink<EAVLLinkType_AlignedRight>
	{
		using CType = CAVLLinkAlignedRightData;
	};

	template <>
	struct TCLinkTypeToLink<EAVLLinkType_Unaligned>
	{
		using CType = CAVLLinkData;
	};
}
