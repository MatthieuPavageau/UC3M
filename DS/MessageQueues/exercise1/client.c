#include "keys.c"

int main()
{
    char key[255] = "key1";
    char value1[255];
    float value2;

    init();

    set_value("key1", "test1", 1.1);

    get_value(key, value1, &value2);

    value2 = 1.2;

    modify_value(key, "modified1", &value2);

    get_value(key, value1, &value2);

    exist(key);

    delete_key(key);

    exist(key);
}