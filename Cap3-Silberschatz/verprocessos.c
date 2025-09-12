#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

void ver_processos()
{
	LPPROCESSENTRY32 pi = ( LPPROCESSENTRY32 )malloc( sizeof( PROCESSENTRY32 ) );

	HANDLE handle_snap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	if ( handle_snap != INVALID_HANDLE_VALUE )
	{
		pi->dwSize = sizeof(*pi);

		if (Process32First( handle_snap, pi ))
        {
            do
            {
                printf( "szExeFile..........: %s\n", pi->szExeFile );
                printf( "th32ProcessID......: %d\n", pi->th32ProcessID );
                printf( "cntThreads.........: %d\n", pi->cntThreads );
                printf( "th32ParentProcessID: %d\n", pi->th32ParentProcessID );
                printf( "---------------------------\n" );

            } while( Process32Next( handle_snap, pi ) );
        }

		CloseHandle( handle_snap );
	}

	free( pi );
}

int main()
{
    ver_processos();


    printf( "\n\n\n" );

    return 0;
}