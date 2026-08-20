extern int overlay_accumulate(int value);
extern int overlay_triple(int value);

int main(void)
{
	return overlay_accumulate(2) + overlay_triple(3);
}
