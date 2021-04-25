//
// 전역변수
//
float4x4		matWVP;		//World * View * Projection Matrix
float4x4		matWorld;	//World Matrix
float3		vLightDir;	//광원 방향
float3		vEyePos;	//눈의 위치
float		specPower;	//Specular 파워

//기본 DiffuseTexture
texture DiffuseTex;
sampler2D DiffuseSamp = sampler_state
{
	Texture = DiffuseTex;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

//기본 NormalTexture
texture NormalTex;
sampler2D NormalSamp = sampler_state
{
	Texture = NormalTex;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

//정점 입력 구조체
struct VS_INPUT{
	float4 Position : POSITION;
	float2 Texcoord : TEXCOORD0;
	float3 Normal : NORMAL0;
	float3 Binormal : BINORMAL0;
	float3 Tangent : TANGENT0;
};

//정점 출력 구조체
struct VS_OUTPUT{
	float4 Position : POSITION;
	float2 Texcoord : TEXCOORD0;
	float3 ViewDir : TEXCOORD1;			//정점으로 부터 시선의 방향
	float3 LightDir : TEXCOORD2;			//정점으로 부터 광워느이 방향
	float3 Normal : TEXCOORD3;			//월드 변환이된 정점의 노말
	float3 Binormal : TEXCOORD4;			//월드 변환이된 정점의 노말
	float3 Tangent : TEXCOORD5;			//월드 변환이된 정점의 노말
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
	VS_OUTPUT Ouput = (VS_OUTPUT)0;		//ZeroMemory 과 같은 효과

	//정점 변환
	Ouput.Position = mul( Input.Position, matWVP );

	//Texcoord 
	Ouput.Texcoord = Input.Texcoord;

	//월드 노말
	Ouput.Normal = mul( Input.Normal, (float3x3)matWorld );

	//월드 바이노말
	Ouput.Binormal = mul( Input.Binormal, (float3x3)matWorld );

	//월드 Tangent
	Ouput.Tangent = mul( Input.Tangent, (float3x3)matWorld );

	//광원 
	Ouput.LightDir = -vLightDir;

	//월드 정점위치
	float3 worldPos = mul( Input.Position, matWorld ).xyz;

	//월드 기준의 시선위치
	Ouput.ViewDir = vEyePos - worldPos;

	return Ouput;
}

//픽셀셰이더 입력 구조체
struct PS_INPUT{
	float2 Texcoord : TEXCOORD0;
	float3 ViewDir : TEXCOORD1;			//정점으로 부터 시선의 방향
	float3 LightDir : TEXCOORD2;			//정점으로 부터 광워느이 방향
	float3 Normal : TEXCOORD3;			//월드 변환이된 정점의 노말
	float3 Binormal : TEXCOORD4;			//월드 변환이된 정점의 노말
	float3 Tangent : TEXCOORD5;			//월드 변환이된 정점의 노말
};


float4 ps_main( PS_INPUT Input ) : COLOR0 {
	
	float3x3 matTBN = float3x3(
		normalize( Input.Tangent ),
		normalize( Input.Binormal ),
		normalize( Input.Normal ) );

	//노말Texture 의 컬러값
	float3 normalTexColor = tex2D( NormalSamp, Input.Texcoord ).rgb;

	//정점스페이스를 기준으로한 노말값
	float3 normalInTangentSpace =
		( normalTexColor * 2.0f ) - 1.0f;

	//외곡된 진짜 월드 노말
	float3 normal = mul( normalInTangentSpace, matTBN );


	//일단 정확한 계산을 위해 Normalize
	float3 viewDir = normalize( Input.ViewDir );
	float3 lightDir = normalize( Input.LightDir );
	

	//확산광의 량
	float d = dot( normal, lightDir );
	float diffuse = saturate( d );

	//Light Reflect
	float3 reflect = -lightDir + 2.0f * d * normal;
	reflect = normalize( reflect );

	//Specular 
	float spec = saturate( dot( reflect, viewDir ) );

	//SpecularPower 적용
	spec = pow( spec, specPower );


	//Texture 컬러
	float3 diffuseTex = tex2D( DiffuseSamp, Input.Texcoord ).rgb;

	//DiffuseColor
	float3 diffuseColor = diffuseTex * diffuse;
	
	//SpecularColor
	float3 specularColor = float3( 1, 1, 1 ) * spec;


	//FinalColor 
	float3 finalColor = diffuseColor + specularColor;


	return float4( finalColor, 1 );
}

//
// 테크닉
//
technique MyShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader  = compile ps_3_0 ps_main();
	}
}



