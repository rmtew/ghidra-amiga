extern unsigned char relocation_target;

int main(void)
{
	return *(unsigned short *)(&relocation_target - 2);
}
