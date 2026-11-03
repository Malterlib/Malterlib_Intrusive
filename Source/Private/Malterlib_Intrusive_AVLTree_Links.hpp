// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

namespace NMib::NIntrusive::NPrivate
{
	inline bool CAVLLinkData::fs_GetPtrIsFast()
	{
		return true;
	}

	inline void CAVLLinkData::f_Construct()
	{
		m_Skew = EAVLTreeSkew_NotInTree;
	}

	inline CAVLLinkData **CAVLLinkData::f_GetRight()
	{
		return &m_Next[1];
	}

	inline CAVLLinkData **CAVLLinkData::f_GetLeft()
	{
		return &m_Next[0];
	}

	inline CAVLLinkData **CAVLLinkData::f_GetNext(aint _iIndex)
	{
		return &m_Next[_iIndex];
	}

	inline CAVLLinkData *CAVLLinkData::f_GetRightP() const
	{
		return m_Next[1];
	}

	inline CAVLLinkData *CAVLLinkData::f_GetLeftP() const
	{
		return m_Next[0];
	}

	inline CAVLLinkData *CAVLLinkData::f_GetNextP(aint _iIndex) const
	{
		return m_Next[_iIndex];
	}

	inline aint CAVLLinkData::f_GetSkew() const
	{
		return m_Skew;
	}

	inline bool CAVLLinkData::f_IsBalanced() const
	{
		return m_Skew == EAVLTreeSkew_None;
	}

	inline void CAVLLinkData::f_SetRight(CAVLLinkData *_pRight)
	{
		m_Next[1] = _pRight;
	}

	inline void CAVLLinkData::f_SetLeft(CAVLLinkData *_pLeft)
	{
		m_Next[0] = _pLeft;
	}

	inline void CAVLLinkData::f_SetRight(CAVLLinkData **_pRight)
	{
		m_Next[1] = *_pRight;
	}

	inline void CAVLLinkData::f_SetLeft(CAVLLinkData **_pLeft)
	{
		m_Next[0] = *_pLeft;
	}

	inline void CAVLLinkData::f_SetNext(int _iIndex, CAVLLinkData *_pPtr)
	{
		m_Next[_iIndex] = _pPtr;
	}

	inline void CAVLLinkData::fs_Assign(CAVLLinkData **_pDest, CAVLLinkData *_pSrc)
	{
		*_pDest = _pSrc;
	}

	inline void CAVLLinkData::fs_AssignInit(CAVLLinkData **_pDest, CAVLLinkData *_pSrc)
	{
		*_pDest = _pSrc;
	}

	inline void CAVLLinkData::fs_Assign(CAVLLinkData **_pDest, CAVLLinkData **_pSrc)
	{
		*_pDest = *_pSrc;
	}

	inline void CAVLLinkData::f_SetSkew(umint _Skew)
	{
		m_Skew = _Skew;
	}

	constexpr CAVLLinkData *CAVLLinkData::fs_GetPtr(CAVLLinkData *_pSrc)
	{
		return _pSrc;
	}

	inline void CAVLLinkData::f_SetAll(CAVLLinkData *_pLeft, CAVLLinkData *_pRight, umint _Skew)
	{
		m_Next[0] = _pLeft;
		m_Next[1] = _pRight;
		m_Skew = _Skew;
	}

	inline void CAVLLinkData::f_Clear()
	{
		m_Next[0] = 0;
		m_Next[1] = 0;
		m_Skew = EAVLTreeSkew_None;
	}

#ifndef DCompiler_MSVC
	constexpr CAVLLinkAlignedData::CAVLLinkAlignedData()
		: m_Next{nullptr, nullptr}
	{
	}
#endif

	inline bool CAVLLinkAlignedData::fs_GetPtrIsFast()
	{
		return false;
	}

	inline void CAVLLinkAlignedData::f_Construct()
	{
		f_SetAll(0, 0, EAVLTreeSkew_NotInTree);
	}

	inline CAVLLinkAlignedData **CAVLLinkAlignedData::f_GetRight()
	{
		return &m_Next[1];
	}

	inline CAVLLinkAlignedData **CAVLLinkAlignedData::f_GetLeft()
	{
		return &m_Next[0];
	}

	inline CAVLLinkAlignedData **CAVLLinkAlignedData::f_GetNext(aint _iIndex)
	{
		return &m_Next[_iIndex];
	}

	inline CAVLLinkAlignedData *CAVLLinkAlignedData::f_GetRightP() const
	{
		return (CAVLLinkAlignedData *)((umint)m_Next[1] & (~umint(0x1)));
	}

	inline CAVLLinkAlignedData *CAVLLinkAlignedData::f_GetLeftP() const
	{
		return (CAVLLinkAlignedData *)((umint)m_Next[0] & (~umint(0x1)));
	}

	inline CAVLLinkAlignedData *CAVLLinkAlignedData::f_GetNextP(aint _iIndex) const
	{
		return (CAVLLinkAlignedData *)((umint)m_Next[_iIndex] & (~umint(0x1)));
	}

	inline umint CAVLLinkAlignedData::f_GetSkew() const
	{
		return (((umint)m_Next[0] & 1) | ((umint)m_Next[1] & 1) << 1);
	}

	inline bool CAVLLinkAlignedData::f_IsBalanced() const
	{
		return f_GetSkew() == EAVLTreeSkew_None;
	}

	constexpr CAVLLinkAlignedData *CAVLLinkAlignedData::fs_GetPtr(CAVLLinkAlignedData const *_pSrc)
	{
		return (CAVLLinkAlignedData *)((umint)_pSrc & (~umint(0x1)));
	}

	inline void CAVLLinkAlignedData::f_SetRight(CAVLLinkAlignedData *_pRight)
	{
		DMibFastCheck((!((umint)_pRight & 1))); // We can only save ptrs that are aligned on 2 bytes
		m_Next[1] = (CAVLLinkAlignedData *)(((umint)m_Next[1] & 1) | (umint)_pRight);
	}

	inline void CAVLLinkAlignedData::f_SetLeft(CAVLLinkAlignedData *_pLeft)
	{
		DMibFastCheck(!((umint)_pLeft & 1)); // We can only save ptrs that are aligned on 2 bytes
		m_Next[0] = (CAVLLinkAlignedData *)(((umint)m_Next[0] & 1) | (umint)_pLeft);
	}

	inline void CAVLLinkAlignedData::f_SetNext(int _iIndex, CAVLLinkAlignedData *_pPtr)
	{
		DMibFastCheck(!((umint)_pPtr & 1)); // We can only save ptrs that are aligned on 2 bytes
		m_Next[_iIndex] = (CAVLLinkAlignedData *)(((umint)m_Next[_iIndex] & 1) | (umint)_pPtr);
	}

	inline void CAVLLinkAlignedData::f_SetRight(CAVLLinkAlignedData **_pRight)
	{
		m_Next[1] = (CAVLLinkAlignedData *)(((umint)m_Next[1] & umint(1)) | ((umint)*_pRight & (~umint(0x1))));
	}

	inline void CAVLLinkAlignedData::f_SetLeft(CAVLLinkAlignedData **_pLeft)
	{
		m_Next[0] = (CAVLLinkAlignedData *)(((umint)m_Next[0] & umint(1)) | ((umint)*_pLeft & (~umint(0x1))));
	}

	inline void CAVLLinkAlignedData::fs_Assign(CAVLLinkAlignedData **_pDest, CAVLLinkAlignedData *_pSrc)
	{
		DMibFastCheck(!((umint)_pSrc & umint(1))); // We can only save ptrs that are aligned on 2 bytes
		*_pDest = (CAVLLinkAlignedData *)((((umint)*_pDest) & umint(1)) | (umint)_pSrc);
	}

	inline void CAVLLinkAlignedData::fs_AssignInit(CAVLLinkAlignedData **_pDest, CAVLLinkAlignedData *_pSrc)
	{
		DMibFastCheck(!((umint)_pSrc & umint(1))); // We can only save ptrs that are aligned on 2 bytes
		*_pDest = _pSrc;
	}

	inline void CAVLLinkAlignedData::fs_Assign(CAVLLinkAlignedData **_pDest, CAVLLinkAlignedData **_pSrc)
	{
		*_pDest = (CAVLLinkAlignedData *)((((umint)*_pDest) & umint(1)) | (((umint)*_pSrc) & (~umint(0x1))));
	}

	inline void CAVLLinkAlignedData::f_SetSkew(umint _Skew)
	{
		DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
		m_Next[0] = (CAVLLinkAlignedData *)(((umint)m_Next[0] & (~1)) | (_Skew & 1));
		m_Next[1] = (CAVLLinkAlignedData *)(((umint)m_Next[1] & (~1)) | (_Skew >> 1));
	}

	inline void CAVLLinkAlignedData::f_SetAll(CAVLLinkAlignedData *_pLeft, CAVLLinkAlignedData *_pRight, umint _Skew)
	{
		DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
		m_Next[0] = (CAVLLinkAlignedData *)((umint)_pLeft | (_Skew & 1));
		m_Next[1] = (CAVLLinkAlignedData *)((umint)_pRight | (_Skew >> 1));
	}

	inline void CAVLLinkAlignedData::f_Clear()
	{
		m_Next[0] = (CAVLLinkAlignedData *)(EAVLTreeSkew_None & 1);
		m_Next[1] = (CAVLLinkAlignedData *)(EAVLTreeSkew_None >> 1);
	}

#ifdef DMibDebug
	inline CAVLLinkAlignedRightData *CAVLLinkAlignedRightData::fp_Debug_GetLeft() const
	{
		return f_GetLeftP();
	}

	inline CAVLLinkAlignedRightData *CAVLLinkAlignedRightData::fp_Debug_GetRight() const
	{
		return f_GetRightP();
	}
#endif

	inline bool CAVLLinkAlignedRightData::fs_GetPtrIsFast()
	{
		return false;
	}

	inline void CAVLLinkAlignedRightData::f_Construct()
	{
		f_SetAll(0, 0, EAVLTreeSkew_NotInTree);
	}

	inline CAVLLinkAlignedRightData **CAVLLinkAlignedRightData::f_GetRight()
	{
		return &m_Next[1];
	}

	inline CAVLLinkAlignedRightData **CAVLLinkAlignedRightData::f_GetLeft()
	{
		return &m_Next[0];
	}

	inline CAVLLinkAlignedRightData **CAVLLinkAlignedRightData::f_GetNext(aint _iIndex)
	{
		return &m_Next[_iIndex];
	}

	inline CAVLLinkAlignedRightData *CAVLLinkAlignedRightData::f_GetRightP() const
	{
		return (CAVLLinkAlignedRightData *)((umint)m_Next[1] & (~umint(0x3)));
	}

	inline CAVLLinkAlignedRightData *CAVLLinkAlignedRightData::f_GetLeftP() const
	{
		return m_Next[0];
	}

	inline CAVLLinkAlignedRightData *CAVLLinkAlignedRightData::f_GetNextP(aint _iIndex) const
	{
		return (CAVLLinkAlignedRightData *)((umint)m_Next[_iIndex] & (~umint(0x3)));
	}

	inline umint CAVLLinkAlignedRightData::f_GetSkew() const
	{
		return ((umint)m_Next[1] & umint(3));
	}

	inline bool CAVLLinkAlignedRightData::f_IsBalanced() const
	{
		return f_GetSkew() == EAVLTreeSkew_None;
	}

	constexpr CAVLLinkAlignedRightData *CAVLLinkAlignedRightData::fs_GetPtr(CAVLLinkAlignedRightData *_pSrc)
	{
		return (CAVLLinkAlignedRightData *)((umint)_pSrc & (~umint(0x3)));
	}

	inline void CAVLLinkAlignedRightData::f_SetRight(CAVLLinkAlignedRightData *_pRight)
	{
		DMibFastCheck((!((umint)_pRight & 3))); // We can only save ptrs that are aligned on 2 bytes
		m_Next[1] = (CAVLLinkAlignedRightData *)(((umint)m_Next[1] & umint(3)) | (umint)_pRight);
	}

	inline void CAVLLinkAlignedRightData::f_SetLeft(CAVLLinkAlignedRightData *_pLeft)
	{
		m_Next[0] = _pLeft;
	}

	inline void CAVLLinkAlignedRightData::f_SetNext(int _iIndex, CAVLLinkAlignedRightData *_pPtr)
	{
		DMibFastCheck(!((umint)_pPtr & 3)); // We can only save ptrs that are aligned on 2 bytes
		m_Next[_iIndex] = (CAVLLinkAlignedRightData *)(((umint)m_Next[_iIndex] & umint(3)) | (umint)_pPtr);
	}

	inline void CAVLLinkAlignedRightData::f_SetRight(CAVLLinkAlignedRightData **_pRight)
	{
		m_Next[1] = (CAVLLinkAlignedRightData *)(((umint)m_Next[1] & umint(3)) | ((umint)*_pRight & (~umint(0x3))));
	}

	inline void CAVLLinkAlignedRightData::f_SetLeft(CAVLLinkAlignedRightData **_pLeft)
	{
		m_Next[0] = (CAVLLinkAlignedRightData *)((umint)*_pLeft & (~umint(0x3)));
	}

	inline void CAVLLinkAlignedRightData::fs_Assign(CAVLLinkAlignedRightData **_pDest, CAVLLinkAlignedRightData *_pSrc)
	{
		DMibFastCheck(!((umint)_pSrc & 3)); // We can only save ptrs that are aligned on 2 bytes
		*_pDest = (CAVLLinkAlignedRightData *)(((umint)*_pDest & umint(3)) | (umint)_pSrc);
	}

	inline void CAVLLinkAlignedRightData::fs_AssignInit(CAVLLinkAlignedRightData **_pDest, CAVLLinkAlignedRightData *_pSrc)
	{
		DMibFastCheck(!((umint)_pSrc & 3)); // We can only save ptrs that are aligned on 2 bytes
		*_pDest = _pSrc;
	}

	inline void CAVLLinkAlignedRightData::fs_Assign(CAVLLinkAlignedRightData **_pDest, CAVLLinkAlignedRightData **_pSrc)
	{
		*_pDest = (CAVLLinkAlignedRightData *)(((umint)*_pDest & umint(3)) | ((umint)*_pSrc & (~umint(0x3))));
	}

	inline void CAVLLinkAlignedRightData::f_SetSkew(umint _Skew)
	{
		DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
		m_Next[1] = (CAVLLinkAlignedRightData *)(((umint)m_Next[1] & (~umint(3))) | (_Skew));
	}

	inline void CAVLLinkAlignedRightData::f_SetAll(CAVLLinkAlignedRightData *_pLeft, CAVLLinkAlignedRightData *_pRight, umint _Skew)
	{
		DMibFastCheck(!(_Skew & (~3))); // We can only save 2 bits
		m_Next[0] = _pLeft;
		m_Next[1] = (CAVLLinkAlignedRightData *)((umint)_pRight | (_Skew));
	}

	inline void CAVLLinkAlignedRightData::f_Clear()
	{
		m_Next[0] = nullptr;
		m_Next[1] = (CAVLLinkAlignedRightData *)umint(EAVLTreeSkew_None);
	}
}
