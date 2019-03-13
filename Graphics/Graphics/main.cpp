#include "PCH.h"
#include "Game.h"

int WINAPI wWinMain(HINSTANCE _hInstance, HINSTANCE _prevHInstance, LPWSTR _argV, int _argC)
{
	UNREFERENCED_PARAMETER(_argV);
	UNREFERENCED_PARAMETER(_prevHInstance);

	int returnValue = CGame::Get()->Initialize(_hInstance, _argC);
	if (FAILED(returnValue))
	{
		return returnValue;
	}
	returnValue = CGame::Get()->Run();
	CGame::Get()->Finalize();

	return returnValue;
}