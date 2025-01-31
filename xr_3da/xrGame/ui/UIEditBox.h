// CUIEditBox.h: ���� ������ � ����������
// 
//////////////////////////////////////////////////////////////////////

#pragma once
#include "UIStatic.h"
#include "../script_export_space.h"

//////////////////////////////////////////////////////////////////////////

class game_cl_GameState;

//////////////////////////////////////////////////////////////////////////

class CUIEditBox : public CUIStatic
{
	typedef CUIStatic inherited;
public:
					CUIEditBox		();
	virtual			~CUIEditBox		();

	virtual void	SendMessage		(CUIWindow* pWnd, s16 msg, void* pData = NULL);

	virtual void	OnMouse			(int x, int y, EUIMessages mouse_action);
	virtual bool	OnKeyboard		(int dik, EUIMessages keyboard_action);

	virtual void	Update			();
	virtual void	Draw			();

	void			CaptureFocus	(bool bCapture) { m_bInputFocus = bCapture; }
	virtual	void	SetText			(LPCSTR str);
protected:

	bool KeyPressed(int dik);
	bool KeyReleased(int dik);
	
	void AddLetter(char c);
	void AddChar(char c);

	bool m_bInputFocus;
	bool m_bShift;

	//DIK �������, ���. ������ � ������������, 0 ���� ����� ���
	int m_iKeyPressAndHold;
	bool m_bHoldWaitMode;

	//��������� ������� ������� ��� ������ ������
	u32 m_iCursorPos;
	DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CUIEditBox)
#undef script_type_list
#define script_type_list save_type_list(CUIEditBox)
