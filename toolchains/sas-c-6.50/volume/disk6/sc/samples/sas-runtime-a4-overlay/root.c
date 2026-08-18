extern int global_counter;
int overlay_value(void);

int main(void)
{
	return overlay_value() == global_counter ? 0 : 1;
}
