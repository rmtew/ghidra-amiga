long left_value = 0x1234567L;
long right_value = 37L;

long multiply_value(void)
{
	return left_value * right_value;
}

long divide_value(void)
{
	return left_value / right_value;
}

long modulo_value(void)
{
	return left_value % right_value;
}

int main(void)
{
	return (int)(multiply_value() + divide_value() + modulo_value());
}
