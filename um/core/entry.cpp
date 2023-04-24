#include "../common.h"

#include "../utils/driver.h"


int main( )
{
	if (g_Driver.Initialize( ) != ERROR_SUCCESS)
	{
		printf("Failed to initialize the driver. Error: %s\r\n", std::system_category( ).message(GetLastError( )).c_str( ));
		return EXIT_FAILURE;
	}

	printf("Driver initialized.\r\n");
	g_Driver.Shutdown( );
	printf("Driver shutdown.\r\n");
	return EXIT_SUCCESS;
}

