technique rainbow
{
	pass p0
	{
		ZWriteEnable = false;
		FogEnable = false;
		Lighting = false;
		CullMode = none;
		AlphaTestEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		ColorOp[0] = modulate;
		ColorArg1[0] = texture;
		ColorArg2[0] = current;

		AlphaOp[0] = disable;

		ColorOp[1] = disable;
	}
}