//������������� � ����������� ���� � ������� ����� fxc.exe �� dxsdk9
//
//>fxc /T vs_1_1 /Fc asm.txt HLSL.fx
//
//������������ ������ ��������� � GrassEx.sha
//��� decl �����
//��� def ��������� � cpp � ����� �� ams
//

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 pos : POSITION;
    float4 clr : COLOR;
    float2 uv : TEXCOORD0;
};

//�������
float3 ang_table[16];	//������� ����� (sin(ay), cos(ay), (fabsf(-cos(ay)*lDir.x + sin(ay)*lDir.z)*0.3f + 0.7f))
float2 uv_table[16];
//������� ���*��������
float4x4 wpMatrix;
//��������� ���������
float2 lDir;
float kLitWF;
float3 aColor;
float3 lColor;

//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT main(float3 pos : POSITION, float4 params : COLOR0, float4 offset : COLOR1, float3 wa : NORMAL)
{
    VS_OUTPUT Output;	
	
	//���� �������� �� ����!!!
	float kLitWF = 1.23456f;
	
	//������� ���������� ��������
	float3 ang = ang_table[params.z*15.0f];
	//���������
	float kClr = (wa.x*lDir.x + wa.y*lDir.y);
	kClr = max(kClr, -0.2f/0.4f);
	kClr = min(kClr, 0.4f/0.4f);
	kClr = (kClr*0.4f + ang.z*0.2f)*kLitWF*offset.y;
	Output.clr.xyz = aColor + lColor*kClr;
	Output.clr.w = wa.z;
	//�������
	float2 size = params.xy*float2(0.3f*0.6f, 0.4f*0.6f) + float2(0.7f*0.6f, 0.6f*0.6f);
	float4 tmpPos;
	tmpPos.xz = pos.xz - ang.xy*size.xx*(offset.xx - 0.5f) + wa.xy*offset.yy;
	tmpPos.y = pos.y + sqrt(size.y - (wa.x*wa.x + wa.y*wa.y))*offset.y;
	tmpPos.w = 1.0f;
	Output.pos = mul(wpMatrix, tmpPos);
	//���������� ����������
	Output.uv = uv_table[params.w*15.0f].xy + offset.xy*float2(0.245f, -0.245f);
	//��
    return Output;    
}

