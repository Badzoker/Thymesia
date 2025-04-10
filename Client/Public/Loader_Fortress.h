#pragma once

#include "Client_Defines.h"
#include "Loader.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader_Fortress final : public CLoader
{
private:
	CLoader_Fortress(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader_Fortress() = default;


public:
	virtual HRESULT Initialize(LEVELID eNextLevelID) override;
	virtual HRESULT Start_Loading() override;

private:
	HRESULT Loading_For_Level_Fortress();
	
private:
	HRESULT Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix) override;

public:
	static CLoader_Fortress* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END