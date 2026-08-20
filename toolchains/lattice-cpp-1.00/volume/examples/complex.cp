// Complex arithmetic test

#include <stream.h>
#include <complex.h>

main()
{
	cout << "Complex arithmetic test:\n";

	complex a(1,5);
	complex b(3, 3);

	cout << "a= " << a << "\n";
	cout << "b= " << b << "\n";
	cout << "a + b = " << a + b << "\n";
	cout << "a - b = " << a - b << "\n";
}
