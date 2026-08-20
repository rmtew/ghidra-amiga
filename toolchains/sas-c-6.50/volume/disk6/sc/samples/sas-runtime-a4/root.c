int read_global(void);

int main(void)
{
	return read_global() == 42 ? 0 : 1;
}
