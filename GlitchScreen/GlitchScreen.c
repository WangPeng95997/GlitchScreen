#include <Windows.h>
#define COLOR 3
#define COUNT 7

// ºì, ³È, »Æ, ÂÌ, Çà, À¶, ×Ï
COLORREF dwColor[COUNT][COLOR] = { {255, 0, 0}, {255, 128, 0}, {255, 255, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {128, 0, 128} };

// Ëæ»úÊýº¯Êý
HCRYPTPROV hProv;
int Random()
{
	if (hProv == 0)
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			return 0;

	int out = 0;
	CryptGenRandom(hProv, sizeof(out), (BYTE*)(&out));

	return out & 0x7FFFFFFF;
}

DWORD WINAPI Cubes_x(LPVOID lpParameter)
{
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	int times = 0, delay = 0, n = Random() % COUNT, y = Random() % (height / 10) + (height / 200);

	BOOLEAN bVector = Random() % 2 ? TRUE : FALSE;
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetDC(hwnd);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, y);
	HGDIOBJ hGdiobj = SelectObject(hdcCopy, hBitmap);

	while ((times = (Random() % (width / 2))) < 320) {}
	while ((delay = (Random() % 10000)) < 7000) {}
	delay = delay / times;
	SetPixel(hdcCopy, 0, 0, RGB(dwColor[n][0], dwColor[n][1], dwColor[n][2]));
	StretchBlt(hdcCopy, 0, 0, width, y, hdcCopy, 0, 0, 1, 1, SRCCOPY);

	for (int i = 0, ix = Random() % width, iy = Random() % height; i < times; i++)
	{
		if (bVector)
			BitBlt(hdc, ix + i, iy, 1, y, hdcCopy, 0, 0, SRCCOPY);
		else
			BitBlt(hdc, ix - i, iy, 1, y, hdcCopy, 0, 0, SRCCOPY);
		Sleep(delay);
	}
	SelectObject(hdcCopy, hGdiobj);
	DeleteObject(hBitmap);
	DeleteDC(hdcCopy);
	ReleaseDC(hwnd, hdc);

	return 0;
}

DWORD WINAPI Cubes_y(LPVOID lpParameter)
{
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	int times = 0, delay = 0, n = Random() % COUNT, x = Random() % (width / 10) + (width / 200);

	BOOLEAN bVector = Random() % 2 ? TRUE : FALSE;
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetDC(hwnd);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, x, height);
	HGDIOBJ hGdiobj = SelectObject(hdcCopy, hBitmap);

	while ((times = (Random() % (height / 2))) < 240) {}
	while ((delay = (Random() % 10000)) < 7000) {}
	delay = delay / times;
	SetPixel(hdcCopy, 0, 0, RGB(dwColor[n][0], dwColor[n][1], dwColor[n][2]));
	StretchBlt(hdcCopy, 0, 0, x, height, hdcCopy, 0, 0, 1, 1, SRCCOPY);

	for (int i = 0, ix = Random() % width, iy = Random() % height; i < times; i++)
	{
		if (bVector)
			BitBlt(hdc, ix, iy + i, x, 1, hdcCopy, 0, 0, SRCCOPY);
		else
			BitBlt(hdc, ix, iy - i, x, 1, hdcCopy, 0, 0, SRCCOPY);
		Sleep(delay);
	}
	SelectObject(hdcCopy, hGdiobj);
	DeleteObject(hBitmap);
	DeleteDC(hdcCopy);
	ReleaseDC(hwnd, hdc);

	return 0;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HANDLE hThread;

	for (;;)
	{
		if (Random() % 2)
			hThread = CreateThread(NULL, 0, Cubes_x, NULL, 0, NULL);
		else
			hThread = CreateThread(NULL, 0, Cubes_y, NULL, 0, NULL);

		Sleep(1000);
	}
	CloseHandle(hThread);

	return 0;
}