#pragma once 

#include "Base.h"

BEGIN(Engine)


class CAnimation : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default; 


public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const class CModel* pModel, vector<_uint>& CurrentKeyFrameIndices);
	_bool Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _float* pCurrentTrackPoisiton, vector<_uint>& CurrentKeyFrameIndices, _bool isLoop);


	/* 11월 27일 추가한 코드 */
	void  Reset(const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices, _float* pCurrentTrackPoisiton);
	_uint Get_ChannelIndex(const _char* pChannelName);	
	_bool Lerp_NextAnimation(_float fTimeDelta, class CAnimation* pNextAnimation, const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices);	
	void  SetLerpTime(_float _LerpTime) { m_LerpTime = _LerpTime; }	

	/*1월 15일 추가한 코드*/
	_bool isAniMationFinish() { return m_isFinished; }


	/* 2월 19일 수정 사항 */
	void Set_LerpTime(_float _fLerpTime) { m_LerpTime = _fLerpTime; }

	void Set_StartOffSetTrackPosition(_float _fOffsetTrackValue) { m_fSetStartOffSetTrackPosition = _fOffsetTrackValue; }	

	vector<ANIMEVENT>* Get_vecEvent() { return &m_vecAnimFrameEvent; }

private:
	_bool                      m_bReset = { false };	
	_bool					   m_isFinished = { false };
	_bool					   m_bFirst = { true };	

	_uint					   m_iNumChannels = {};

	_float					   m_fDuration = {};
	_float					   m_fTickPerSecond = {};
	_float					   m_fCurrentTrackPosition = {};
	_float					   m_fAnimationSpeed = { 1.f };	
	_float					   m_fSetStartOffSetTrackPosition = {};	
	_float					   m_LerpTime = 0.2f;	
	_float					   m_LerpTimeAcc = 0.f;	

	/*Channel == Bone*/
	vector<class CChannel*>    m_Channels;
	
	_char					   m_szName[MAX_PATH];	

	vector<_uint>*			   m_CurrentKeyFameIndices;	
	vector<_float>			   m_vecKeyFrameAnimationSpeed;


	vector<ANIMEVENT>		   m_vecAnimFrameEvent;
	_int					   m_iCountFrameEvent = {};

public:

	HRESULT Save_Anim(ostream& os);
	HRESULT Load_Anim(istream& is, vector<_uint>& CurrentKeyFrameIndices);

	static CAnimation* Create(const aiAnimation* pAIAnimation, const class CModel* pModel, vector<_uint>& CurrentKeyFrameIndices);	
	static CAnimation* LoadCreate(istream& is, vector<_uint>* _CurrentKeyFrameIndices);	
	virtual void Free() override;

};
END