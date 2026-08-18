static int running_total;

int overlay_accumulate(int value)
{
    running_total += value;
    return running_total;
}

int overlay_triple(int value)
{
    return value * 3;
}
