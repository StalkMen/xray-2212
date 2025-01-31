//////////////////////////////////////////////////////////////////////
// UIScrollBar.h: ������ ��������� ��� ����
//////////////////////////////////////////////////////////////////////

#ifndef _UI_SCROLLBAR_H_
#define _UI_SCROLLBAR_H_

#pragma once
#include "uiwindow.h"

#include "UIButton.h"
#include "UIScrollBox.h"


class CUIScrollBar :public CUIWindow
{
private:
	typedef CUIWindow inherited;
public:
	CUIScrollBar(void);
	virtual ~CUIScrollBar(void);


	virtual void Init(int x, int y, int length, bool bIsHorizontal);

	//���������, ������������ ������������� ����
//	typedef enum{VSCROLL, HSCROLL} E_MESSAGE;

	virtual void OnMouseWheel(int direction);
	virtual void SendMessage(CUIWindow *pWnd, s16 msg, void *pData);

	virtual void Draw();

	virtual void SetWidth(int width);
	virtual void SetHeight(int height);

	virtual void Reset();
	// � ��������� ��������� ������ ���� �� ������� �������� ����� �����, � ���������
	// � ��� ����� ��� � ������� �� 0. ��� ������� - ����
	void		 Refresh();

	//��������
	void SetRange(s16 iMin, s16 iMax);
	void GetRange(s16& iMin, s16& iMax) {iMin = m_iMinPos;  iMax = m_iMaxPos;}
	int GetRangeSize() {return (m_iMaxPos-m_iMinPos);}
	int GetMaxRange() {return m_iMaxPos;}
	int GetMinRange() {return m_iMinPos;}

	void SetPageSize(u16 iPage) { m_iPageSize = iPage; UpdateScrollBar();}
	u16 GetPageSize() {return m_iPageSize;}

	void SetScrollPos(s16 iPos) { m_iScrollPos = iPos; UpdateScrollBar();}
	s16 GetScrollPos() {return m_iScrollPos;}

	//������� ������� ��� ������
	enum {SCROLLBAR_WIDTH = 16, SCROLLBAR_HEIGHT = 16};
	void TryScrollInc();
	void TryScrollDec();

protected:
	//����������� �������, ���� ��������
	bool ScrollInc();
	bool ScrollDec();

	//�������� ������
	void UpdateScrollBar();


	//�������������� ��� ������������ 
	bool m_bIsHorizontal;
	
	//������ ���������
	CUIButton m_DecButton;
	CUIButton m_IncButton;

	//������� ���������
	CUIScrollBox m_ScrollBox;

	//�������� ��� ���������
	CUIStaticItem m_StaticBackground;

	//������� �������
	s16 m_iScrollPos;

	//������� �����������
	s16 m_iMinPos;
	s16 m_iMaxPos;
	
	//������ ������������ ��������
	u16 m_iPageSize;
};


#endif