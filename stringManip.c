int str_split(char *str, char delim, char *parts[], int max_parts)
{
    int count = 0;
    parts[count++] = str;

    for (char *p = str; *p && count < max_parts; p++)
    {
        if (*p == delim)
        {
            *p = '\0';
            parts[count++] = p + 1;
        }
    }

    return count;
}