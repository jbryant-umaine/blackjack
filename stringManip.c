// Take a string, a delimiter, a char array, and a maximum parts and split the string by its delimeter
// For example, "hello,world" becomes:
// parts[0] = hello
// parts[1] = world
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