#pragma once

#include "../BaseMonster/base_monster.h"
#include "../ai_monster_jump.h"
#include "../../../script_export_space.h"

class CAI_PseudoDog : public CBaseMonster, public CJumping {
	typedef		CBaseMonster	inherited;

public:
	bool			strike_in_jump;

	SAttackEffector m_psi_effector;

	float			m_anger_hunger_threshold;
	float			m_anger_loud_threshold;

	TTime			m_time_became_angry;

	TTime			time_growling;			// ����� ���������� � ��������� �������

	ref_sound		psy_effect_sound;		// ����, ������� �������� � ������ � ������
	float			psy_effect_turn_angle;

public:
					CAI_PseudoDog		();
	virtual			~CAI_PseudoDog		();	

	virtual void	Load				(LPCSTR section);

	virtual void	reinit				();
	virtual void	reload				(LPCSTR section);

	virtual void	UpdateCL			();

	virtual void	OnJumpStop			();
	virtual bool	CanJump				() {return true;}
	
	virtual bool	ability_can_drag	() {return true;}
	virtual bool	ability_psi_attack	() {return true;}
	virtual bool	ability_can_jump	() {return true;}


	virtual void	CheckSpecParams		(u32 spec_params);
	virtual void	play_effect_sound	();

	DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CAI_PseudoDog)
#undef script_type_list
#define script_type_list save_type_list(CAI_PseudoDog)
