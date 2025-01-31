// EntityCondition.h: ����� ��������� ����� ��������
//
//////////////////////////////////////////////////////////////////////
 
#ifndef _ENTITY_CONDITION_H_
#define _ENTITY_CONDITION_H_
#pragma once

class CWound;
class NET_Packet;
class CEntityAlive;

#include "hit_immunity.h"
#include "../bone.h"

class CEntityCondition: public CHitImmunity
{
private:
	bool			m_use_limping_state;
	CEntityAlive	*m_object;

public:
	CEntityCondition(CEntityAlive *object);
	virtual ~CEntityCondition(void);

	virtual void LoadCondition (LPCSTR section);

	//serialization
	virtual void save	(NET_Packet &output_packet);
	virtual void load	(IReader &input_packet);

	float GetHealth() const {return m_fHealth;}
	float GetPower() const {return m_fPower;}
	float GetSatiety() const {return m_fSatiety;}
	float GetRadiation() const {return m_fRadiation;}

	float GetCircumspection() const {return m_fCircumspection;}
	float GetEntityMorale() const {return m_fEntityMorale;}

	float GetMaxHealth() const {return m_fHealthMax;}
	float GetHealthLost() const {return m_fHealthLost;}

	virtual bool IsLimping() const;

	void ChangeHealth(float value);
	void ChangePower(float value);
	void ChangeSatiety(float value);
	void ChangeRadiation(float value);

	void ChangeBleeding(float percent);

	void ChangeCircumspection(float value);
	void ChangeEntityMorale(float value);

	//hit_power �������� �� 0 �� 100 (��������� �����������)
	virtual CWound* ConditionHit(CObject* who, float hit_power, ALife::EHitType hit_type, u16 element = BI_NONE);
	//���������� ��������� � �������� �������
	virtual void UpdateCondition	();
	virtual void UpdateWounds		();
	virtual void UpdateConditionTime();

	//��������! ��� �����
	virtual void Sleep(float hours);
	
	virtual void GoSleep();
	virtual void Awoke();
	virtual bool IsSleeping() {return m_bIsSleeping;}
	
	//�������� ������ ����� �� ���� �������� ��� 
	virtual float BleedingSpeed();

	virtual CObject* GetWhoHitLastTime() {return m_pWho;}

protected:
	virtual void UpdateHealth();
	virtual void UpdatePower();
	virtual void UpdateSatiety();
	virtual void UpdateRadiation();

	virtual void UpdateCircumspection();
	virtual void UpdateEntityMorale();

	virtual CWound* AddWound(float hit_power, ALife::EHitType hit_type, u16 element);

	//��������� ���� ���� � ����������� �� �������� �������
	//(������ ��� InventoryOwner)
	virtual float HitOutfitEffect(float hit_power, ALife::EHitType hit_type);
	
	//��� �������� ��������� �������� ���,
	//������������ ����� ���� ��� ������� ���
	//� �������� ������ ����� �� ����
	DEFINE_VECTOR(CWound*, WOUND_VECTOR, WOUND_VECTOR_IT);
	WOUND_VECTOR m_WoundVector;
	//������� ������� ���
	void ClearWounds();

	//��� �������� �� 0 �� 1			
	float m_fHealth;				//��������
	float m_fPower;					//����
	float m_fSatiety;				//������� (�������)
	float m_fRadiation;				//���� ������������� ���������

	float m_fCircumspection;		//���������������	
	float m_fEntityMorale;			//������

	//������������ ��������
	float m_fHealthMax;
	float m_fPowerMax;
	float m_fSatietyMax;
	float m_fRadiationMax;

	float m_fCircumspectionMax;
	float m_fEntityMoraleMax;

	//�������� ��������� ���������� �� ������ ����������
	float m_fDeltaHealth;
	float m_fDeltaPower;
	float m_fDeltaSatiety;
	float m_fDeltaRadiation;

	float m_fDeltaCircumspection;
	float m_fDeltaEntityMorale;

	//�������� ��������� ������������� ���������
	//� �������
	float m_fV_Health;
	float m_fV_Power;
	float m_fV_Satiety;
	float m_fV_Radiation;

	float m_fV_Circumspection;
	float m_fV_EntityMorale;

	//������������ ��������� ��������� ���������� �� ����� ���
	float m_fK_SleepHealth;
	float m_fK_SleepPower;
	float m_fK_SleepSatiety;
	float m_fK_SleepRadiation;

	//������� ���������
	float m_fCurrentSleepHealth;
	float m_fCurrentSleepPower;
	float m_fCurrentSleepSatiety;
	float m_fCurrentSleepRadiation;


	//�������� �������������� �������� � ����
	//�� ���� �������
	float m_fV_SatietyPower;
	float m_fV_SatietyHealth;

	//����������� �������� ������� (� ��������� 0..1), ����� �������� 
	//�������� ����������� ��������
	float m_fSatietyCritical;

	//����������� ��������� �������� �������� �������� �� ��������
	//(��� ������������ ���� ���������)
	float m_fV_RadiationHealth;

	//�������� ������ ����� � �������� ���� �� ������������� ���� 
	float m_fV_Bleeding;
	//�������� ��������� ����
	float m_fV_WoundIncarnation;
	float m_fMinWoundSize;
	//����, ����������� �� �� ��� ���� ������������
	bool m_bIsBleeding;


	//����� ����, ������������� �� ���������� �������� � ����
	float m_fHealthHitPart;
	float m_fPowerHitPart;

	//��������� ���
	bool m_bIsSleeping;


	//������ �������� �� ���������� ����
	float m_fHealthLost;
	//��� ����� ��������� ���
	CObject* m_pWho;


	//��� ������������ ������� 
	u64 m_iLastTimeCalled;
	u64 m_iDeltaTime;
	bool m_bTimeValid;

	//��� �������� ���������� �� DamageManager
	float m_fHitBoneScale;
	float m_fWoundBoneScale;

	float m_limping_threshold;

public:
	virtual void reinit	();
	
	IC		const u64	delta_time	() const 
	{
		return		(m_iDeltaTime);
	}

	IC		const WOUND_VECTOR &wounds	() const
	{
		return		(m_WoundVector);
	}

	IC		float	&health()
	{
		return		(m_fHealth);
	}

	IC		float	&hit_bone_scale	()
	{
		return		(m_fHitBoneScale);
	}

	IC		float	&wound_bone_scale()
	{
		return		(m_fWoundBoneScale);
	}
};

#endif //_ENTITY_CONDITION_H_