#include <help_types.h>

namespace help_types
{
	Shape WStr2Shape(const wchar_t *str, int size)
	{
		Shape shape = new wchar_t[size];
		for (int i = 0; i < size; ++i)
		{
			shape[i] = str[i];
		}

		return shape;
	}
}