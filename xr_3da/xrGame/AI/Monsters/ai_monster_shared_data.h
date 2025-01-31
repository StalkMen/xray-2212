#pragma once 
#include "../../shared_data.h"
#include "ai_monster_defs.h"

class _base_monster_shared : public CSharedResource {
public:
	// float speed factors
	SVelocityParam			m_fsVelocityNone;
	SVelocityParam			m_fsVelocityStandTurn;
	SVelocityParam			m_fsVelocityWalkFwdNormal;
	SVelocityParam			m_fsVelocityWalkFwdDamaged;
	SVelocityParam			m_fsVelocityRunFwdNormal;
	SVelocityParam			m_fsVelocityRunFwdDamaged;
	SVelocityParam 			m_fsVelocityDrag;
	SVelocityParam 			m_fsVelocitySteal;
	SVelocityParam			m_fsVelocityRunAttack;

	float					m_fDistToCorpse;
	float					m_fDamagedThreshold;		// ����� ��������, ���� �������� ��������������� ���� m_bDamaged

	// -------------------------------------------------------

	TTime					m_dwIdleSndDelay;
	TTime					m_dwEatSndDelay;
	TTime					m_dwAttackSndDelay;

	// -------------------------------------------------------

	u32						m_dwDayTimeBegin;
	u32						m_dwDayTimeEnd;
	float					m_fMinSatiety;
	float					m_fMaxSatiety;
	// ----------------------------------------------------------- 

	float					m_fSoundThreshold;
	float					m_fHitPower;

	float					m_fEatFreq;
	float					m_fEatSlice;
	float					m_fEatSliceWeight;

	u8						m_legs_number;
	SAttackEffector			m_attack_effector;

	float					m_max_hear_dist;

	float					m_run_attack_path_dist;
	float					m_run_attack_start_dist;
};

class _motion_shared : public CSharedResource {
public:
	ANIM_ITEM_MAP			m_tAnims;			// ����� ��������
	MOTION_ITEM_MAP			m_tMotions;			// ����� ����������� EAction � SMotionItem
	TRANSITION_ANIM_VECTOR	m_tTransitions;		// ������ ��������� �� ����� �������� � ������
	ATTACK_ANIM				aa_all;				// ������ ����

	t_fx_index				default_fx_indexes;
	FX_MAP_STRING			fx_map_string;
	FX_MAP_U16				fx_map_u16;
	bool					map_converted;
	
	AA_MAP					aa_map;
	//STEPS_MAP				steps_map;
};


