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

		static inline_small bool fs_GetPtrIsFast();
		inline_small void f_Construct();
		inline_small CAVLLinkData **f_GetRight();
		inline_small CAVLLinkData **f_GetLeft();
		inline_small CAVLLinkData **f_GetNext(aint _iIndex);
		inline_small CAVLLinkData *f_GetRightP() const;
		inline_small CAVLLinkData *f_GetLeftP() const;
		inline_small CAVLLinkData *f_GetNextP(aint _iIndex) const;
		inline_small aint f_GetSkew() const;
		inline_small bool f_IsBalanced() const;
		inline_small void f_SetRight(CAVLLinkData *_pRight);
		inline_small void f_SetLeft(CAVLLinkData *_pLeft);
		inline_small void f_SetRight(CAVLLinkData **_pRight);
		inline_small void f_SetLeft(CAVLLinkData **_pLeft);
		inline_small void f_SetNext(int _iIndex, CAVLLinkData *_pPtr);
		static inline_small void fs_Assign(CAVLLinkData **_pDest, CAVLLinkData *_pSrc);
		static inline_small void fs_AssignInit(CAVLLinkData **_pDest, CAVLLinkData *_pSrc);
		static inline_small void fs_Assign(CAVLLinkData **_pDest, CAVLLinkData **_pSrc);
		inline_small void f_SetSkew(umint _Skew);
		static constexpr inline_small CAVLLinkData *fs_GetPtr(CAVLLinkData *_pSrc);
		inline_small void f_SetAll(CAVLLinkData *_pLeft, CAVLLinkData *_pRight, umint _Skew);
		inline_small void f_Clear();

		CAVLLinkData *m_Next[2];
		aint m_Skew;

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
		constexpr CAVLLinkAlignedData();
#endif

		static inline_small bool fs_GetPtrIsFast();
		inline_small void f_Construct();
		inline_small CAVLLinkAlignedData **f_GetRight();
		inline_small CAVLLinkAlignedData **f_GetLeft();
		inline_small CAVLLinkAlignedData **f_GetNext(aint _iIndex);
		inline_small CAVLLinkAlignedData *f_GetRightP() const;
		inline_small CAVLLinkAlignedData *f_GetLeftP() const;
		inline_small CAVLLinkAlignedData *f_GetNextP(aint _iIndex) const;
		inline_small umint f_GetSkew() const;
		inline_small bool f_IsBalanced() const;
		static constexpr inline_small CAVLLinkAlignedData *fs_GetPtr(CAVLLinkAlignedData const *_pSrc);
		inline_small void f_SetRight(CAVLLinkAlignedData *_pRight);
		inline_small void f_SetLeft(CAVLLinkAlignedData *_pLeft);
		inline_small void f_SetNext(int _iIndex, CAVLLinkAlignedData *_pPtr);
		inline_small void f_SetRight(CAVLLinkAlignedData **_pRight);
		inline_small void f_SetLeft(CAVLLinkAlignedData **_pLeft);
		static inline_small void fs_Assign(CAVLLinkAlignedData **_pDest, CAVLLinkAlignedData *_pSrc);
		static inline_small void fs_AssignInit(CAVLLinkAlignedData **_pDest, CAVLLinkAlignedData *_pSrc);
		static inline_small void fs_Assign(CAVLLinkAlignedData **_pDest, CAVLLinkAlignedData **_pSrc);
		inline_small void f_SetSkew(umint _Skew);
		inline_small void f_SetAll(CAVLLinkAlignedData *_pLeft, CAVLLinkAlignedData *_pRight, umint _Skew);
		inline_small void f_Clear();

		CAVLLinkAlignedData *m_Next[2];
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

		static inline_small bool fs_GetPtrIsFast();
		inline_small void f_Construct();
		inline_small CAVLLinkAlignedRightData **f_GetRight();
		inline_small CAVLLinkAlignedRightData **f_GetLeft();
		inline_small CAVLLinkAlignedRightData **f_GetNext(aint _iIndex);
		inline_small CAVLLinkAlignedRightData *f_GetRightP() const;
		inline_small CAVLLinkAlignedRightData *f_GetLeftP() const;
		inline_small CAVLLinkAlignedRightData *f_GetNextP(aint _iIndex) const;
		inline_small umint f_GetSkew() const;
		inline_small bool f_IsBalanced() const;
		static constexpr inline_small CAVLLinkAlignedRightData *fs_GetPtr(CAVLLinkAlignedRightData *_pSrc);
		inline_small void f_SetRight(CAVLLinkAlignedRightData *_pRight);
		inline_small void f_SetLeft(CAVLLinkAlignedRightData *_pLeft);
		inline_small void f_SetNext(int _iIndex, CAVLLinkAlignedRightData *_pPtr);
		inline_small void f_SetRight(CAVLLinkAlignedRightData **_pRight);
		inline_small void f_SetLeft(CAVLLinkAlignedRightData **_pLeft);
		static inline_small void fs_Assign(CAVLLinkAlignedRightData **_pDest, CAVLLinkAlignedRightData *_pSrc);
		static inline_small void fs_AssignInit(CAVLLinkAlignedRightData **_pDest, CAVLLinkAlignedRightData *_pSrc);
		static inline_small void fs_Assign(CAVLLinkAlignedRightData **_pDest, CAVLLinkAlignedRightData **_pSrc);
		inline_small void f_SetSkew(umint _Skew);
		inline_small void f_SetAll(CAVLLinkAlignedRightData *_pLeft, CAVLLinkAlignedRightData *_pRight, umint _Skew);
		inline_small void f_Clear();

		CAVLLinkAlignedRightData *m_Next[2];

	private:
		using CThis = CAVLLinkAlignedRightData;

#ifdef DMibDebug
		inline_never CAVLLinkAlignedRightData *fp_Debug_GetLeft() const;
		inline_never CAVLLinkAlignedRightData *fp_Debug_GetRight() const;
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

#include "Malterlib_Intrusive_AVLTree_Links.hpp"
