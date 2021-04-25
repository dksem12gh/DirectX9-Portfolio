#include "stdafx.h"
#include "xMeshStatic.h"

LPD3DXEFFECT		xMeshStatic::_xMeshEffect = NULL;
LPDIRECT3DTEXTURE9	xMeshStatic::_defDiffuseTex = NULL;
LPDIRECT3DTEXTURE9	xMeshStatic::_defNormalTex = NULL;
LPDIRECT3DTEXTURE9	xMeshStatic::_defSpecularTex = NULL;
LPDIRECT3DTEXTURE9	xMeshStatic::_defEmssionTex = NULL;

HRESULT xMeshStatic::init(string filePath, D3DXMATRIXA16 * matCorrection)
{
	//  쉐이더와 디폭트 텍스쳐 초기화
	if (_xMeshEffect == NULL)
	{
		_xMeshEffect = RM_SHADER->addResource("Resources/Shader/StaticMesh.fx");
	}
	if (_defDiffuseTex == NULL)
	{
		_defDiffuseTex = RM_TEXTURE->addResource("Resources/Mesh/diffuseDefault.png");
	}
	if (_defNormalTex == NULL)
	{
		_defNormalTex = RM_TEXTURE->addResource("Resources/Mesh/normalDefault.png");
	}
	if (_defSpecularTex == NULL)
	{
		_defSpecularTex = RM_TEXTURE->addResource("Resources/Mesh/specularDefault.png");
	}
	if (_defEmssionTex == NULL)
	{
		_defEmssionTex = RM_TEXTURE->addResource("Resources/Mesh/emissionDefault.png");
	}


	LPD3DXBUFFER		adjacency = NULL;		//  인접 버퍼
	LPD3DXBUFFER		material = NULL;		//  재질 버퍼

												//  xfile로 부터 메쉬를 불러옴.
	if (FAILED(D3DXLoadMeshFromX(
		filePath.c_str(),			//  X 파일 경로
		D3DXMESH_MANAGED,			//  옵션 4가지 32BIT : 32비트 인덱스 사용, WRITEONLY : 메쉬 데이터에 쓰기만 허용. DYNAMIC : 메쉬버퍼는 동적으로 만들어짐.
		device,						//  메쉬와 연계될 장치
		&adjacency,					//  ID3DBUFFER(D3DX 사용되는 범용 데이터 구조체)를 리턴. 
		&material,					//  재질의 정보. D3DXMATERIAL 구조체 배열을 리턴.
		NULL,						//  EffectInstance. 인자를 무시.
		&_materialsNum,				//  재질의 수
		&_mesh						//  X 파일의 기하정보 객체를 리턴.
	)))
	{
		SAFE_RELEASE(adjacency);
		SAFE_RELEASE(material);
		return E_FAIL;
	}

	//  로딩 경로에서 파일명을 제거하고 경로만
	string path;
	int lastPathIndex = 0;
	//  '\' 마지막 찾으면 됨.
	lastPathIndex = filePath.find_last_of('/');

	//  만약 못찾았다면
	if (lastPathIndex == -1)
	{
		lastPathIndex = filePath.find_last_of('\\');
	}
	else
	{
		path = filePath.substr(0, lastPathIndex + 1);
	}

	//  텍스쳐 로딩시 필요한 문자열
	string texFilePath;			//  최종 경로
	string texExp;				//  파일 확장자 명
	string texFile;				//  파일명

								//  재질 정보 가져오기
	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)material->GetBufferPointer();

	//  재질 갯수만큼 for문 돌림
	for (DWORD i = 0; i < _materialsNum; i++)
	{
		//  로드된 마테리얼
		D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

		//  메쉬의 마테리얼에는 Abbient 정보가 없다.
		//  Diffuse 정보와 동일하게 설정
		mtrl.Ambient = mtrl.Diffuse;

		//  마테리얼 정보 
		_vMaterials.push_back(mtrl);

		//  마테리얼의 texture 정보가 없으면
		if (pMaterials[i].pTextureFilename != NULL)
		{
			//  파일경로 = Mesh 파일경로 + 텍스쳐 파일이름
			texFilePath = path + pMaterials[i].pTextureFilename;

			//  텍스쳐를 로딩하고 푸쉬
			_vDiffuseTex.push_back(RM_TEXTURE->addResource(texFilePath));

			//  파일 이름과 확장자 위치
			int dotIndex = texFilePath.find_last_of(".");

			//  파일명과 확장자
			texFile = texFilePath.substr(0, dotIndex);
			texExp = texFilePath.substr(dotIndex + 1, filePath.length());

			//  _N 은 노말맵, _S 는 스펙큘라 맵, _E 는 이미션 맵을 말함.

			// 
			//  노말맵 _N
			// 
			texFilePath = texFile + "_N." + texExp;
			LPDIRECT3DTEXTURE9 pNorTex = RM_TEXTURE->addResource(texFilePath);
			
			//  만약 파일이 없다면
			if (pNorTex == NULL)
			{
				_vNormalTex.push_back(_defNormalTex);
			}
			else
			{
				_vNormalTex.push_back(pNorTex);
			}
			LPDIRECT3DTEXTURE9 pGlossTex = RM_TEXTURE->addResource("Resources/mabinogi/gloss/mat_128_gray.dds");
			_vGlossTex.push_back(pGlossTex);
			// 
			//  스펙큘라맵 _S
			// 
			texFilePath = texFile + "_S." + texExp;
			LPDIRECT3DTEXTURE9 pSpecTex = RM_TEXTURE->addResource(texFilePath);

			//  만약 파일이 없다면
			if (pSpecTex == NULL)
			{
				_vSpecularTex.push_back(_defSpecularTex);
			}
			else
			{
				_vSpecularTex.push_back(pSpecTex);
			}

			// 
			//  이미션맵 _E
			// 
			texFilePath = texFile + "_E." + texExp;
			LPDIRECT3DTEXTURE9 pEmissionTex = RM_TEXTURE->addResource(texFilePath);

			//  만약 파일이 없다면
			if (pEmissionTex == NULL)
			{
				_vEmissionTex.push_back(_defEmssionTex);
			}
			else
			{
				_vEmissionTex.push_back(pEmissionTex);
			}
		}
		else
		{
			_vDiffuseTex.push_back(_defDiffuseTex);
			_vNormalTex.push_back(_defNormalTex);
			_vSpecularTex.push_back(_defSpecularTex);
			_vEmissionTex.push_back(_defEmssionTex);
			_vGlossTex.push_back(RM_TEXTURE->addResource("Resources/mabinogi/gloss/mat_flat.dds"));
		}
	}

	//  pMaterial 재질 버퍼는 필요가 없으니 반드시 해제 시켜줘야 함.
	SAFE_RELEASE(material);


	//  메쉬 최적화
	_mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |		//  메쉬 서브셋 순서대로 정렬(DrawSubset 효율 up)
		D3DXMESHOPT_COMPACT |		//  메쉬에서 사용되지 않는 정점과 인덱스 해제
		D3DXMESHOPT_VERTEXCACHE,	//  정점 캐쉬에서 히트율을 높여줌.
		(DWORD*)adjacency->GetBufferPointer(),		//  인접버퍼 넘김.
		NULL,						//  최적화를 마치고 결과로 얻는 인접버퍼
		NULL,						//  최적화된 인덱스 정보
		NULL						//  최적화된 정점 버퍼 정보
	);

	//  최적화가 완료 되면 인접 버퍼 해제
	SAFE_RELEASE(adjacency);

	//  메쉬 보정 처리
	//  보정 행렬을 받으면
	if (matCorrection)
	{
		meshCorrection(matCorrection);
	}
	else
	{
		//  보정 행렬이 없더라도 보정처리를 해야 bound 정보를 얻을 수 있음
		D3DXMATRIXA16 matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		meshCorrection(&matIdentity);
	}


	return S_OK;
}

HRESULT xMeshStatic::init(string filePath, string mtrlPath, D3DXMATRIXA16 * matCorrection)
{
	return S_OK;
}

void xMeshStatic::release(void)
{
	SAFE_RELEASE(_mesh);
}

void xMeshStatic::render(transform * trans)
{
	//  월드 행렬 셋팅
	D3DXMATRIXA16 matWorld = trans->getFinalMatrix();
	_xMeshEffect->SetMatrix("matWorld", &matWorld);

	//  일단 광원 방향 셋팅
	D3DXVECTOR4 lightDir(1, -1, 1, 1);
	_xMeshEffect->SetVector("vLightDir", &lightDir);

	//  라이트 컬러 일단 흰색
	D3DXVECTOR4 lightColor(1, 1, 1, 1);
	_xMeshEffect->SetVector("vLightColor", &lightColor);

	//  effect로 그리기 시작
	_xMeshEffect->SetTechnique("Mesh");

	//  쉐이더로 큐브렌더
	UINT iPassNum = 0;
	//  이펙트로 렌더링 시작
	_xMeshEffect->Begin(&iPassNum, 0);

	//  패스 숫자만큼 
	for (UINT i = 0; i < iPassNum; i++)
	{
		_xMeshEffect->BeginPass(i);
		//  큐브 렌더
		for (int m = 0; m < _materialsNum; m++)
		{
			//  텍스쳐 셋팅
			_xMeshEffect->SetTexture("Diffuse_Tex", _vDiffuseTex[m]);
			_xMeshEffect->SetTexture("Normal_Tex", _vNormalTex[m]);
			_xMeshEffect->SetTexture("Specular_Tex", _vSpecularTex[m]);
			_xMeshEffect->SetTexture("Emission_Tex", _vEmissionTex[m]);
			_xMeshEffect->SetTexture("Gloss_Tex", _vGlossTex[m]);
			//  스펙 파워
			_xMeshEffect->SetFloat("fSpecPower", _vMaterials[m].Power);
			//  
			_xMeshEffect->CommitChanges();
			//  메쉬 그림
			_mesh->DrawSubset(m);
		}
		_xMeshEffect->EndPass();
	}
	_xMeshEffect->End();

}


void xMeshStatic::meshCorrection(D3DXMATRIXA16 * matCorrection)
{
	//  정점 Element를 얻어와서 정점 정보를 구함
	D3DVERTEXELEMENT9 pVerElement[MAX_FVF_DECL_SIZE];

	//  pVerElement 배열에는 정점의 요소 정보를 넣어줌
	_mesh->GetDeclaration(pVerElement);

	//  정점 포지션 offset
	int positionOffset = -1;

	//  정점 노말 offset
	int normalOffset = -1;

	//  정점 접선 offset
	int tangentOffset = -1;

	//  정점 법선 offset
	int binormalOffset = -1;

	//  정점 정보 최고 사이즈 만큼 
	for (DWORD i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		//  마지막이라면
		if (pVerElement[i].Type == D3DDECLTYPE_UNUSED)
		{
			break;
		}

		//  정점 위치 정보를 만나면
		if (pVerElement[i].Usage == D3DDECLUSAGE_POSITION)
		{
			positionOffset = pVerElement[i].Offset;
		}
		else if (pVerElement[i].Usage == D3DDECLUSAGE_NORMAL)
		{
			normalOffset = pVerElement[i].Offset;
		}
		else if (pVerElement[i].Usage == D3DDECLUSAGE_TANGENT)
		{
			tangentOffset = pVerElement[i].Offset;
		}
		else if (pVerElement[i].Usage == D3DDECLUSAGE_BINORMAL)
		{
			binormalOffset = pVerElement[i].Offset;
		}
	}

	// 버텍스 갯수
	DWORD verNum = _mesh->GetNumVertices();

	// 메쉬의 정점 FVF 정보혹은 Decl 정보를 이용하여 정점하나당 크기를 알아내자.
	// DWORD verSize = D3DXGetFVFVertexSize( pMesh->GetFVF() );
	DWORD verSize = D3DXGetDeclVertexSize(pVerElement, 0);

	// 메쉬의 버텍스 버퍼를 Lock 한다
	void* p = NULL;
	_mesh->LockVertexBuffer(0, &p);

	// 바운드 MinMax 계산을 위한 초기화
	_boundMin = D3DXVECTOR3(0, 0, 0);
	_boundMax = D3DXVECTOR3(0, 0, 0);

	// 버텍스 수만클 돌아 재낀다
	for (DWORD i = 0; i < verNum; i++) {

		// 버텍스 시작 주소
		char* pVertex = ((char*)p + (i * verSize));

		// 정점 위치가 있다면
		if (positionOffset != -1)
		{
			D3DXVECTOR3* pos = (D3DXVECTOR3*)(pVertex + positionOffset);

			D3DXVec3TransformCoord(pos, pos, matCorrection);

			// 정점 최소 값갱신
			if (_boundMin.x > pos->x)		_boundMin.x = pos->x;
			if (_boundMin.y > pos->y)		_boundMin.y = pos->y;
			if (_boundMin.z > pos->z)		_boundMin.z = pos->z;

			// 정점 최대 값갱신
			if (_boundMax.x < pos->x)		_boundMax.x = pos->x;
			if (_boundMax.y < pos->y)		_boundMax.y = pos->y;
			if (_boundMax.z < pos->z)		_boundMax.z = pos->z;

			// 정점 위치 푸쉬
			_vertices.push_back(*pos);
		}

		// 노말정보가 있다면..
		if (normalOffset != -1)
		{
			D3DXVECTOR3* nor = (D3DXVECTOR3*)(pVertex + normalOffset);
			D3DXVec3TransformNormal(nor, nor, matCorrection);
			D3DXVec3Normalize(nor, nor);
			// 정점 노말 푸쉬
			_normals.push_back(*nor);
		}

		// tangent 정보가 있다면.
		if (tangentOffset != -1)
		{
			D3DXVECTOR3* tangent = (D3DXVECTOR3*)(pVertex + tangentOffset);
			D3DXVec3TransformNormal(tangent, tangent, matCorrection);
			D3DXVec3Normalize(tangent, tangent);
		}

		// binormal 정보가 있다면
		if (binormalOffset != -1)
		{
			D3DXVECTOR3* binor = (D3DXVECTOR3*)(pVertex + binormalOffset);
			D3DXVec3TransformNormal(binor, binor, matCorrection);
			D3DXVec3Normalize(binor, binor);
		}
	}

	_mesh->UnlockVertexBuffer();

	// Bound 추가 계산
	_boundCenter = (_boundMin + _boundMax) * 0.5f;
	_boundSize = D3DXVECTOR3(
		_boundMax.x - _boundMin.x,
		_boundMax.y - _boundMin.y,
		_boundMax.z - _boundMin.z);
	_boundHalfSize = _boundSize * 0.5f;
	_boundRadius = D3DXVec3Length(&(_boundCenter - _boundMin));

	// 
	//  인덱스 버퍼를 얻는다
	// 
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	_mesh->GetIndexBuffer(&pIndexBuffer);

	// 면의 갯수
	_triNum = _mesh->GetNumFaces();

	// 인덱스 버퍼에 대한 정보를 얻는다.
	D3DINDEXBUFFER_DESC desc;
	pIndexBuffer->GetDesc(&desc);

	if (desc.Format == D3DFMT_INDEX16)
	{
		WORD* p = NULL;
		pIndexBuffer->Lock(0, 0, (void**)&p, 0);
		for (DWORD i = 0; i < _triNum; i++)
		{
			_indices.push_back(p[i * 3 + 0]);
			_indices.push_back(p[i * 3 + 1]);
			_indices.push_back(p[i * 3 + 2]);
		}
		pIndexBuffer->Unlock();
	}
	else if (desc.Format == D3DFMT_INDEX32)
	{
		DWORD* p = NULL;
		pIndexBuffer->Lock(0, 0, (void**)&p, 0);
		for (DWORD i = 0; i < _triNum; i++)
		{
			_indices.push_back(p[i * 3 + 0]);
			_indices.push_back(p[i * 3 + 1]);
			_indices.push_back(p[i * 3 + 2]);
		}
		pIndexBuffer->Unlock();
	}

	// 얻어온 인덱스 버퍼는 해재
	SAFE_RELEASE(pIndexBuffer);


}

void xMeshStatic::setCamera(camera * cam)
{
	D3DXMATRIXA16 matViewProj = cam->getViewProjectionMatrix();
	_xMeshEffect->SetMatrix("matViewProjection", &matViewProj);
	D3DXVECTOR3 vEyePos = cam->getWorldPosition();
	_xMeshEffect->SetVector("vEyePos", &D3DXVECTOR4(vEyePos, 1));
	_xMeshEffect->SetFloat("camFar", cam->getCamFar());
	_xMeshEffect->SetFloat("camNear", cam->getCamNear());
}

void xMeshStatic::setBaseLight(lightDirection * pDirLight)
{
	_xMeshEffect->SetMatrix("baseDirectionLight", &pDirLight->getLightMatrix());
}

void xMeshStatic::setLighting(vector<light*>* pLights)
{
	// 라이트 행렬 셋팅
	D3DXMATRIXA16 matLights[10];

	for (int i = 0; i < pLights->size(); i++)
	{
		matLights[i] = (*(pLights))[i]->getLightMatrix();
	}

	// effect에 lighting 셋팅
	_xMeshEffect->SetMatrixArray("matLights", matLights, 10);
	_xMeshEffect->SetInt("LightNum", pLights->size());
}

void xMeshStatic::setLighting(light ** pLights, int lightNum)
{
	// 라이트 행렬 세팅
	D3DXMATRIXA16 matLight[10];
	for (int i = 0; i < lightNum; i++)
	{
		matLight[i] = pLights[i]->getLightMatrix();
	}
	// Effect에 라이팅 세팅
	_xMeshEffect->SetMatrixArray("matLights", matLight, 10);
	_xMeshEffect->SetInt("LightNum", lightNum);
}

void xMeshStatic::setTechniqueName(string name)
{
	_xMeshEffect->SetTechnique(name.c_str());
}
