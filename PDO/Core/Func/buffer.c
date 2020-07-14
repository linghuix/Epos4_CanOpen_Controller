
/*
 * author lhx
 * 7 1, 2020
 *
 * @brief : ?buff?????,buffer?????0
 * Window > Preferences > C/C++ > Editor > Templates.
 */
uint8_t addBuffer(char c)
{
	if((debugBuffer.in+1)%BufferSize == debugBuffer.out)
		return 0;
	debugBuffer.data[debugBuffer.in] = c;
	debugBuffer.in = (debugBuffer.in + 1)%BufferSize;
	return 1;
}
/*
 * author lhx
 * 7 1, 2020
 *
 * @brief : ??buff????,buffer?????0
 * Window > Preferences > C/C++ > Editor > Templates.
 */
char GetBuffer(void)
{
	char c;
	if(debugBuffer.in == debugBuffer.out)
			return 0;
	c = debugBuffer.data[debugBuffer.out++];
	debugBuffer.out = (debugBuffer.out)%BufferSize;
	return c;
}