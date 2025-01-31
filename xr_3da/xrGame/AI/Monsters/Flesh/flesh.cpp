#include "stdafx.h"
#include "flesh.h"
#include "../../../ai_space.h"
#include "../ai_monster_utils.h"
#include "flesh_state_manager.h"

CAI_Flesh::CAI_Flesh()
{
	StateMan = xr_new<CStateManagerFlesh>(this);
	
	m_fEyeShiftYaw		= PI_DIV_6;

	controlled::init_external(this);
}

CAI_Flesh::~CAI_Flesh()
{
	xr_delete(StateMan);
}

BOOL CAI_Flesh::net_Spawn (CSE_Abstract* DC) 
{
	if (!inherited::net_Spawn(DC))
		return(FALSE);

	return TRUE;
}

void CAI_Flesh::Load(LPCSTR section)
{
	inherited::Load(section);

	MotionMan.AddReplacedAnim(&m_bDamaged, eAnimRun,		eAnimRunDamaged);
	MotionMan.AddReplacedAnim(&m_bDamaged, eAnimWalkFwd,	eAnimWalkDamaged);

	MotionMan.accel_load			(section);
	MotionMan.accel_chain_add		(eAnimWalkFwd,		eAnimRun);
	MotionMan.accel_chain_add		(eAnimWalkDamaged,	eAnimRunDamaged);

	if (MotionMan.start_load_shared(CLS_ID)) {

		// define animation set
		MotionMan.AddAnim(eAnimStandIdle,		"stand_idle_",			-1, &inherited::get_sd()->m_fsVelocityNone,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimStandTurnLeft,	"stand_turn_ls_",		-1, &inherited::get_sd()->m_fsVelocityStandTurn,			PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimStandTurnRight,	"stand_turn_rs_",		-1, &inherited::get_sd()->m_fsVelocityStandTurn,			PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		MotionMan.AddAnim(eAnimLieIdle,			"lie_idle_",			-1, &inherited::get_sd()->m_fsVelocityNone,				PS_LIE,		"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimSleep,			"lie_idle_",			-1, &inherited::get_sd()->m_fsVelocityNone,				PS_LIE,		"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		MotionMan.AddAnim(eAnimWalkFwd,			"stand_walk_fwd_",		-1, &inherited::get_sd()->m_fsVelocityWalkFwdNormal,		PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimWalkDamaged,		"stand_walk_fwd_dmg_",	-1, &inherited::get_sd()->m_fsVelocityWalkFwdDamaged,	PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		MotionMan.AddAnim(eAnimRun,				"stand_run_",			-1,	&inherited::get_sd()->m_fsVelocityRunFwdNormal,		PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimRunDamaged,		"stand_run_dmg_",		-1,	&inherited::get_sd()->m_fsVelocityRunFwdDamaged,		PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		MotionMan.AddAnim(eAnimAttack,			"stand_attack_",		-1, &inherited::get_sd()->m_fsVelocityStandTurn,			PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimAttackRat,		"stand_attack_rat_",	-1, &inherited::get_sd()->m_fsVelocityStandTurn,			PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimAttackFromBack,	"stand_attack_back_",	-1, &inherited::get_sd()->m_fsVelocityNone,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimCheckCorpse,		"stand_eat_",			 1,	&inherited::get_sd()->m_fsVelocityNone,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		MotionMan.AddAnim(eAnimEat,				"stand_eat_",			-1, &inherited::get_sd()->m_fsVelocityNone,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimDie,				"stand_die_",			-1, &inherited::get_sd()->m_fsVelocityNone,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		MotionMan.AddAnim(eAnimStandLieDown,	"stand_lie_down_",		-1, &inherited::get_sd()->m_fsVelocityNone,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimLieStandUp,		"lie_stand_up_",		-1, &inherited::get_sd()->m_fsVelocityNone,				PS_LIE,		"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		MotionMan.AddAnim(eAnimSteal,			"stand_crawl_",			-1, &inherited::get_sd()->m_fsVelocitySteal,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimDragCorpse,		"stand_drag_",			-1, &inherited::get_sd()->m_fsVelocityDrag,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		MotionMan.AddAnim(eAnimScared,			"stand_scared_",		-1,	&inherited::get_sd()->m_fsVelocityNone,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		MotionMan.AddAnim(eAnimThreaten,		"stand_threaten_",		-1,	&inherited::get_sd()->m_fsVelocityNone,				PS_STAND,	"fx_stand_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		// define transitions
		MotionMan.AddTransition(PS_STAND,	PS_LIE,		eAnimStandLieDown,		false);
		MotionMan.AddTransition(PS_LIE,		PS_STAND,	eAnimLieStandUp,		false, SKIP_IF_AGGRESSIVE);

		// define links from Action to animations
		MotionMan.LinkAction(ACT_STAND_IDLE,	eAnimStandIdle,	eAnimStandTurnLeft, eAnimStandTurnRight, PI_DIV_6/10);
		MotionMan.LinkAction(ACT_SIT_IDLE,		eAnimLieIdle);
		MotionMan.LinkAction(ACT_LIE_IDLE,		eAnimLieIdle);
		MotionMan.LinkAction(ACT_WALK_FWD,		eAnimWalkFwd);
		MotionMan.LinkAction(ACT_WALK_BKWD,		eAnimWalkBkwd);
		MotionMan.LinkAction(ACT_RUN,			eAnimRun);
		MotionMan.LinkAction(ACT_EAT,			eAnimEat);
		MotionMan.LinkAction(ACT_SLEEP,			eAnimSleep);
		MotionMan.LinkAction(ACT_REST,			eAnimLieIdle);
		MotionMan.LinkAction(ACT_DRAG,			eAnimDragCorpse);
		MotionMan.LinkAction(ACT_ATTACK,		eAnimAttack, eAnimStandTurnLeft, eAnimStandTurnRight, PI_DIV_2);
		MotionMan.LinkAction(ACT_STEAL,			eAnimSteal);
		MotionMan.LinkAction(ACT_LOOK_AROUND,	eAnimScared);
		MotionMan.LinkAction(ACT_TURN,			eAnimStandIdle,	eAnimStandTurnLeft, eAnimStandTurnRight, EPS_S); 

		MotionMan.AA_Load(pSettings->r_string(section, "attack_params"));
		//MotionMan.STEPS_Load(pSettings->r_string(section, "step_params"), get_legs_number());

		MotionMan.finish_load_shared();

	}

#ifdef DEBUG	
	MotionMan.accel_chain_test		();
#endif

}

// ���������� true, ���� ����� ���������� ���� ������� ���������� �������� ���������
// �.�. ���� ������������ ������������������
void CAI_Flesh::CheckSpecParams(u32 spec_params)
{
	if ((spec_params & ASP_DRAG_CORPSE) == 	ASP_DRAG_CORPSE) MotionMan.SetCurAnim(eAnimDragCorpse);

	if ((spec_params & ASP_CHECK_CORPSE) == ASP_CHECK_CORPSE) {
		MotionMan.Seq_Add(eAnimCheckCorpse);
		MotionMan.Seq_Switch();
	}

	if ((spec_params & ASP_BACK_ATTACK) == ASP_BACK_ATTACK) {
		MotionMan.Seq_Add(eAnimAttackFromBack);
		MotionMan.Seq_Switch();
	}

	if ((spec_params & ASP_ATTACK_RAT) == ASP_ATTACK_RAT) MotionMan.SetCurAnim(eAnimAttackRat);
	if ((spec_params & ASP_THREATEN) == ASP_THREATEN) MotionMan.SetCurAnim(eAnimThreaten);
}


////////////////////////////////////////////////////////////////////////////////////////////////
// ������� ConeSphereIntersection
// ����������� ������ (�� �������������) �� ������
// ���������� ��� ����������� ����������� ������ ����� � �����-������ �����
// ���������: ConeVertex - ������� ������, ConeAngle - ���� ������ (����� ������������ � �������)
// ConeDir - ����������� ������, SphereCenter - ����� �����, SphereRadius - ������ �����
bool CAI_Flesh::ConeSphereIntersection(Fvector ConeVertex, float ConeAngle, Fvector ConeDir, Fvector SphereCenter, float SphereRadius)
{
	float fInvSin = 1.0f/_sin(ConeAngle);
	float fCosSqr = _cos(ConeAngle)*_cos(ConeAngle);

	
	Fvector kCmV;	kCmV.sub(SphereCenter,ConeVertex);
	Fvector kD		= kCmV;
	Fvector tempV	= ConeDir;
	tempV.mul		(SphereRadius* fInvSin);
	kD.add			(tempV);

	float fDSqrLen = kD.square_magnitude();
	float fE = kD.dotproduct(ConeDir);
	if ( fE > 0.0f && fE*fE >= fDSqrLen*fCosSqr ) {
		
		float fSinSqr = _sin(ConeAngle)*_sin(ConeAngle);

		fDSqrLen = kCmV.square_magnitude();
		fE = -kCmV.dotproduct(ConeDir);
		if ( fE > 0.0f && fE*fE >= fDSqrLen*fSinSqr ) {
			float fRSqr = SphereRadius*SphereRadius;
			return fDSqrLen <= fRSqr;
		} else return true;
	} 
	
	return false;
}
