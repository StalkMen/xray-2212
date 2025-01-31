// CUIMapWnd.h:  ������ ������������ �����
// 
//////////////////////////////////////////////////////////////////////

#pragma once

#include "UIDialogWnd.h"
#include "UIStatic.h"

#include "UIButton.h"
#include "UICheckButton.h"

#include "UIListWnd.h"
#include "UIFrameWindow.h"
#include "UIFrameLineWnd.h"

#include "UIMapSpot.h"
#include "UIMapBackground.h"
#include "UIGlobalMapLocation.h"

#include "UICharacterInfo.h"

//////////////////////////////////////////////////////////////////////////

class CGameObject;

class CUIMapWnd: public CUIWindow
{
private:
	typedef CUIWindow inherited;
public:
	CUIMapWnd();
	virtual ~CUIMapWnd();

	virtual void		Init			();
	virtual void		Show			(bool status);
	virtual void		SendMessage		(CUIWindow* pWnd, s16 msg, void* pData = NULL);
	virtual void		Draw			();
	virtual void		Update			();
	virtual void		OnMouseWheel	(int direction);

	// ���������� ����� ������� �� �����, �� ������ ��� ����� ������������
	void				SetActivePoint	(const Fvector &vNewPoint);

	// ������ �����
	enum EMapModes
	{
		emmGlobal,
		emmLocal
	};
	// ����������� ����� �����
	void				SwitchMapMode	(EMapModes mode);

protected:
	// ����������� �������� ����� �� ������ � ������� �� ������
	void				ConvertToLocal	(const Fvector& src, Ivector2& dest);
	void				AddObjectSpot	(CGameObject* pGameObject);
	//�������� ����������
	CUIFrameWindow		UIMainMapFrame;
	//�������� PDA
	CUIFrameLineWnd		UIPDAHeader;

	CUICheckButton		UICheckButton1;
	CUICheckButton		UICheckButton2;
	CUICheckButton		UICheckButton3;
	CUICheckButton		UICheckButton4;
		
	//������ � ����������� ��� ��������� �����
	CUIStatic			UIStaticInfo;
	//���������� � ���������
	CUICharacterInfo	UICharacterInfo;

	// ����� � ����������
	CUIMapBackground	UILocalMapBackground;
	CUIMapBackground	UIGlobalMapBackground;
	// ���������� � ������� �� ���������� �����
	CUIStatic			UIMapName;
	CUIListWnd			UIMapGoals;
	// ��� ��������� ����� � ������ ������ ����
	CUIStatic			UILocalMapName;

	// Current map
	CUIMapBackground	*m_pCurrentMap;
	CUIScrollBar		UIMapBgndV,
						UIMapBgndH;
	//������ ������
	CUIMapSpot*			m_pActorSpot;
	// ������ ������������ ����������/��������� �����
	CUIButton			UIMapTypeSwitch;
	// ������� �����
	EMapModes			m_eCurrentMode;
	//�������� ��� �����
	CUIStaticItem		landscape;

	//������ � ������ ����� � �������� �� ������
	int					m_iMapWidth;
	int					m_iMapHeight;

	//���������� ������� �����
	float				m_fWorldMapWidth;
	float				m_fWorldMapHeight;
	float				m_fWorldMapLeft;
	float				m_fWorldMapTop;

	// ������ ��������� ���� ��� ����������
	DEF_VECTOR			(Objectives, shared_str);

	// ��������� ��������� ���� �� ����������
	struct LocalMapDef
	{
		// ������ ��������� ����� �� ����������
		typedef boost::shared_ptr<CUIGlobalMapLocation>	GMLptr;
		GMLptr					mapSpot;
		// ������ �� ������ �����
		Objectives				obj;
		// ��������� ����� �� ������ ������������ �� ���������� �����
		CUIFrameWindow			*smallFrame;
		// ���� �������� �����
		ref_shader				mapTexture;
		// � �� ����������
		Fvector4				mapDimentions;
		// ���� ��������� ���� �����
		bool					visible;

		LocalMapDef()
			:	smallFrame		(NULL),
				mapTexture		(NULL),
				visible			(false)
		{
			mapDimentions.set(0.0f, 0.0f, 0.0f, 0.0f);
		}
	};

	// ������������� ������ ���������� ��������� ���� (���->������)
	DEF_MAP				(LocalMaps, shared_str, LocalMapDef);
	LocalMaps			m_LocalMaps;

	// ������������� ����. ����������� 1 ��� ��� �������� ����
	void				InitGlobalMap			();
	ref_shader			m_GlobalMapShader;
	void				AddLocalMap				(const shared_str leveName, const Ivector4 &v, const Fvector4 &d, const shared_str &textureName);
	void				InitLocalMaps			();
	void				DeleteLocalMapsData		();
	// ������������� ������� ��� ����������� �� �� ������. ����������� ��� ������ ������.

	// ������������� ��������� �����.����������� ��� ������ ������.
	void				SetLocalMap				(const shared_str &levelName);
	// ��������� ������ �� ������������, ��� ������ �������������� �� ������ �����
	void				ApplyFilterToObjectives	(const shared_str &levelName);

public:
	void				InitGlobalMapObjectives	();
	void				InitLocalMapObjectives	();


private:
	// ������� ���������� ����� � �������� ��������
	float				m_GlobalMapWidth;
	float				m_GlobalMapHeight;

	// ����������� �������� �������� �������������� �� ������-������������� �� ���������� �����.
	// �� ���������� ������� �������������� ������������ �����, � ���� ��� �����, �� ������ �����
	// �� ���������� (�.�. ��� ������� ������ ������� ����� �� �������)
	RECT				m_rDefault;
};
