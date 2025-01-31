//=============================================================================
//  Filename:   UIAnimatedStatic.cpp
//	Created by Roman E. Marchenko, vortex@gsc-game.kiev.ua
//	Copyright 2004. GSC Game World
//	---------------------------------------------------------------------------
//  ������ ��� ����������� ������������� ������
//=============================================================================

#include "stdafx.h"
#include "UIAnimatedStatic.h"

//////////////////////////////////////////////////////////////////////////

CUIAnimatedStatic::CUIAnimatedStatic()
	:	m_uFrameCount			(0),
		m_uAnimationDuration	(0),
		m_uTimeElapsed			(0),
		m_uAnimCols				(0xffffffff),
		m_bCyclic				(true),
		m_bParamsChanged		(true),
		m_uFrameWidth			(0),
		m_uFrameHeight			(0),
		m_uCurFrame				(0xffffffff),
		m_bPlaying				(false)
{
	ClipperOn();
}

//////////////////////////////////////////////////////////////////////////

void CUIAnimatedStatic::Update()
{
	if (!m_bPlaying) return;

	static u32 oneFrameDuration = 0;

	// ����������� �������� ��������
	if (m_bParamsChanged && 0 != m_uFrameCount)
	{
		// ����������� ����� ������ �����
		oneFrameDuration = iCeil(m_uAnimationDuration / static_cast<float>(m_uFrameCount));

		SetFrame(0);

		m_bParamsChanged = false;
	}

	// ���������� ����� �����
	static u32 aa = 0;
	m_uTimeElapsed += Device.dwTimeGlobal - aa;
	aa = Device.dwTimeGlobal;

	// ���� �������� �����������
	if (m_uTimeElapsed > m_uAnimationDuration)
	{
		Rewind(m_uAnimationDuration - m_uAnimationDuration);
		if (!m_bCyclic)
			Stop();
	}

	// ������ ��������� ����� � ����������� �� �������
	u32 curFrame = m_uTimeElapsed / oneFrameDuration;

	if (curFrame != m_uCurFrame)
	{
		m_uCurFrame = curFrame;
		SetFrame(m_uCurFrame);
	}
}

//////////////////////////////////////////////////////////////////////////

void CUIAnimatedStatic::SetFrame(const u32 frameNum)
{
	static u32 currRow = 0xffffffff, currCol = 0xffffffff;

	currRow = frameNum / m_uAnimCols;
	currCol = frameNum % m_uAnimCols;
	GetUIStaticItem().SetOriginalRect(currCol * m_uFrameWidth, currRow * m_uFrameHeight, m_uFrameWidth, m_uFrameHeight);
}