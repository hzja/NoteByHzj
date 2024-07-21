#include <stdio.h>
#include <windows.h>
#include <graphics.h>

int main() {
	DWORD Pid = 0;
	HANDLE hProcess = 0;

	DWORD result1, result2;

	// 获取扫雷游戏对应的窗口句柄
	HWND hWnd = FindWindow(NULL, L"扫雷");
	if (hWnd != 0) {
		// 获取扫雷进程ID
		GetWindowThreadProcessId(hWnd, &Pid);
		// 打开扫雷游戏获取其句柄
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
		if (hProcess == 0) {
			printf("Open winmine process failed.");
			return 0;
		}

		// 存放雷区的起始地址
		DWORD dwBoomAddr = 0x01005340;

		// 雷区的最大值（包含边界）
		DWORD dwSize = 832;
		PBYTE pByte = NULL;
		pByte = (PBYTE)malloc(dwSize);

		// 读取整个雷区的数据
		ReadProcessMemory(hProcess, (LPVOID)dwBoomAddr, pByte, dwSize, 0);
		int i = 0;
		int j = 0;
		int n = dwSize;

		// 读取雷区的长和宽
		DWORD dwInfo = 0x01005330;
		DWORD dwHeight = 0, dwWidth = 0;
		ReadProcessMemory(hProcess, (LPVOID)(dwInfo + 4), &dwWidth, sizeof(DWORD), 0);    //宽度
		ReadProcessMemory(hProcess, (LPVOID)(dwInfo + 8), &dwHeight, sizeof(DWORD), 0);   //高度

		int h = dwHeight;
		int count = 0;

		// 雷区转换，去掉雷区多余的数据
		PBYTE pTmpByte = NULL;
		pTmpByte = (PBYTE)malloc(dwHeight * dwWidth);
		while (i < dwSize) {
			//边界判断
			if (pByte[i] == 0x10 && pByte[i + 1] == 0x10) {
				i = i + dwWidth + 2;
				continue;
			}
			else if (pByte[i] == 0x10) {
				for (j = 1; j <= dwWidth; j++) {
					pTmpByte[count] = pByte[i + j];
					count++;
				}
				i = i + dwWidth + 2;
				continue;
				h--;
				if (h == 0) break;
			}
			i++;
		}

		// 获取雷区方块的坐标，然后模拟鼠标进行点击
		int x1 = 0, y1 = 0;
		int x = 0, y = 0;
		for (i = 0; i < dwHeight * dwWidth; i++) {
			if (pTmpByte[i] != 0x8F) { //雷
				x1 = i % dwWidth;
				y1 = i / dwWidth;
				x = x1 * 16 + 16;
				y = y1 * 16 + 61;
				SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(x, y));   //鼠标按下
				SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELONG(x, y));     //鼠标抬起
			}
		}

		free(pByte);
		CloseHandle(hProcess);
	}
	else {
		printf("Get hWnd failed.");
	}
	return 0;
}