#include "ft_printf.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main()
{
		/* printf */
		printf("%s\n", setlocale(LC_ALL, ""));

		printf("Slut les zouzous\n");
		ft_printf("Slut les zouzous\n");

		printf("Hello le nombre est %d\n", 42);
		ft_printf("Hello le nombre est %d\n", 42);

		printf("Hello le nombre est %d%d\n", 42, 42);
		ft_printf("Hello le nombre est %d%d\n", 42, 42);

		printf("Hello le nombre est decimal %u\n", 42);
		ft_printf("Hello le nombre est decimal %u\n", 42);

		printf("Hello le nombre est hexa %x%X\n", 42, 42);
		ft_printf("Hello le nombre est hexa %x%X\n", 42, 42);

		printf("Hello le nombre est octal %o\n", 42);
		ft_printf("Hello le nombre est octal %o\n", 42);

		printf("Hello la string est %s\n", "Les zouulous !!");
		ft_printf("Hello la string est %s\n", "Les zouulous !!");

		printf ("中\n");
		ft_printf ("中\n");

		printf("Hello la string est %s\n", "汉语");
		ft_printf("Hello la string est %s\n", "汉语");

		printf("Hello la string est %50s\n", "汉语");
		ft_printf("Hello la string est %50s\n", "汉语");

		printf("Hello la string est %.2s\n", "Allo les terriens");
		ft_printf("Hello la string est %.2s\n", "Allo les terriens");

		printf("Hello le nombre est %.8d\n", 42);
		ft_printf("Hello le nombre est %.8d\n", 42);

		printf("Hello le nombre est field 5 %5d\n", 42);
		ft_printf("Hello le nombre est field 5 %5d\n", 42);

		printf("Hello le nombre est space % d\n", 42);
		ft_printf("Hello le nombre est space % d\n", 42);

		printf("Hello le nombre est space field 5 % 5d\n", 42);
		ft_printf("Hello le nombre est space field 5 % 5d\n", 42);

		printf("Hello le nombre est space 0 field 5 % 05d\n", 42);
		ft_printf("Hello le nombre est space 0 field 5 % 05d\n", 42);

		printf("Hello le nombre est space . 105 % .10d\n", 42);
		ft_printf("Hello le nombre est space . 105 % .10d\n", 42);

		printf("Hello le nombre est %05d\n", 42);
		ft_printf("Hello le nombre est %05d\n", 42);

		printf("Hello le nombre est %+5d\n", 42);
		ft_printf("Hello le nombre est %+5d\n", 42);

		printf("Hello le nombre est %+.5d\n", 42);
		ft_printf("Hello le nombre est %+.5d\n", 42);

		printf("Hello le nombre est %-5d\n", 42);
		ft_printf("Hello le nombre est %-5d\n", 42);

		printf("Hello le nombre est %d\n", -5);
		ft_printf("Hello le nombre est %d\n", -5);

		printf("Long string %s", "yabada badou ti to api tu pada pada ko\n");
		ft_printf("Long string %s", "yabada badou ti to api tu pada pada ko\n");

		printf("Longer string %s", "yabada badou ti to api tu pada pada ko etou itou padu to ko ki pa la ma na no pep e pi pa pu to tot ot oto ogre erg erio gjeriogj eior ghello la famille !\n");
		ft_printf("Longer string %s", "yabada badou ti to api tu pada pada ko etou itou padu to ko ki pa la ma na no pep e pi pa pu to tot ot oto ogre erg erio gjeriogj eior ghello la famille !\n");

		/* fprintf */
		fprintf(stderr, "Hello le nombre est %+.5d\n", 42);
		ft_fprintf(2, "Hello le nombre est %+.5d\n", 42);

		return (0);
}
