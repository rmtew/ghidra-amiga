int global_counter = 42;

int read_global(void)
{
	return global_counter;
}

int main(void)
{
	return read_global() == 42 ? 0 : 1;
}
