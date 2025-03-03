#pragma once

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CNavigation final : public CComponent
{
public:
	enum NAVI_LINE { N_LINE_AB, N_LINE_BC, N_LINE_CA, N_LINE_END };
	enum NAVI_POINT { N_POINT_A, N_POINT_B, N_POINT_C, N_POINT_END };
public:
	struct NAVIGATION_DESC
	{
		_int   iCurrentCellIndex = { -1 };
	};

	typedef struct CELL_INFO
	{
		_float3 vPos;

		_int byOption;
		_int iIndex;
		_int iParentIndex;

		_int eLineInfo[N_LINE_END];

		_float3 ePoint[N_POINT_END];
		_float3 eLine[N_LINE_END][2];

	}CELL;

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;

public:
	bool Get_FirstPick() const { return m_bFirstPick; }
	void Set_FirstPick(bool bFirstPick) { m_bFirstPick = bFirstPick; }

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFile);
	HRESULT	        Initialize_Prototype_CreateOnly();
	virtual HRESULT Initialize(void* pArg) override;

public:
	void SetUp_WorldMatrix(_fmatrix WorldMatrix) 
	{
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
		XMStoreFloat4x4(&m_WorldMatrixInverse, XMMatrixInverse(nullptr, WorldMatrix));
	}

	void                  Set_CurrentNaviIndex(_vector _vWorldPos);
	_uint                 Find_Closest_Cell(/*_uint _iFloorNum, */_vector _vWorldPos);

public:
	void                  Set_NaviFloor(_uint _iFloorNumber);
	_uint                 Get_NaviFloorNumber() { return m_iFloorNumber; }

public:
	_bool                 isMove(_fvector vWorldPos);
	_float                Compute_Height(_fvector vWorldPos);
	vector<class CCell*>* Get_VecCells() { return &m_Cells; }
	void                  Set_CurCellIndex(_uint _CellIndex) { m_iCurrentCellIndex = _CellIndex; }
	_uint                 Get_CurCellIndex() { return m_iCurrentCellIndex; }
	_uint                 Get_BestListSize() { return m_BestList.size(); }
	_vector               Setting_SlidingMove(_fvector _vWorldPos);
	_bool                 bIsOn_Line(_fvector _vWorldPos);

public:
	_bool                 MakeRoute(_int iStartIndex, _int iGoalIndex);
	void                  MakeBestList(_int iStartIndex, _int iGoalIndex);
	void                  Start_Astar(_uint iGoalIndex);
	void                  Clear_Astar();
	_vector               MoveAstar(_vector pCurPos, _bool& isEmpty);

public:
	_bool                 CheckOpen(_int iIndex);
	_bool                 CheckClose(_int iIndex);
	_bool                 IsPointsClose(_vector fPointA, const _vector fPointB);
public:
	_float                TriangleArea2x(const _vector fPointA, const _vector fPointB, const _vector fPointC);
	_float                Calculate_Squared_Distance(const _vector fPointA, const _vector fPointB);
	_int                  Navigate_Portals(vector<_vector> pPortals, _float3 fStartPos, _float3 fEndPos, vector<_vector>& vPoints);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif

private:
	/* 이 네비를 이용하는 객체가 현재 어떤 셀 안에 있는지? */
	_int					m_iCurrentCellIndex = { -1 };
	vector<class CCell*>	m_Cells;


	static _float4x4       m_WorldMatrix;
	static _float4x4       m_WorldMatrixInverse;
	int                     m_iNeighborIndex = {};

#ifdef _DEBUG
private:
	class CShader* m_pShader = { nullptr };
	_bool                   m_bFirstPick = { true };
private:
	_uint					m_iFloorNumber = {};

#endif
private:
	_int                   m_iStartIndex = {};
	list<_int>             m_CloseList;
	list<_int>             m_OpenList;
	list<CELL*>            m_BestList;

	vector<CELL*>          m_CellInfo;
	vector<list<CELL*>>    m_CellAdj;

	vector<_vector>        m_Portal;

private:
	HRESULT SetUp_Neighbors();


public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile);
	static CNavigation* CreateOnly(ID3D11Device* pdevice, ID3D11DeviceContext* pContext);


	HRESULT	Create_Cell(const _float3 _vPoints[3]);
	HRESULT Delete_Cell(const _float3 _vPoints[3]);


	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END
