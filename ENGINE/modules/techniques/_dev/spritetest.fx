technique spritetest
{
	pass p0
	{
		ZEnable = false;
		Lighting = false;
		CullMode = none;
		AlphaTestEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = srccolor;
		DestBlend = invsrcalpha;

		ColorOp[0] = modulate;
		ColorArg1[0] = diffuse;
		ColorArg2[0] = texture;

		AlphaOp[0] = disable;

		ColorOp[1] = disable;
	}
}
