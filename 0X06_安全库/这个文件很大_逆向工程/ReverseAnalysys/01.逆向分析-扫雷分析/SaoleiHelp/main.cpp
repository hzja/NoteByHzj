#include <stdio.h>
#include <windows.h>
#include <graphics.h>

int main() {
	DWORD Pid = 0;
	HANDLE hProcess = 0;

	DWORD result1, result2;

	// ��ȡɨ����Ϸ��Ӧ�Ĵ��ھ��
	HWND hWnd = FindWindow(NULL, L"ɨ��");
	if (hWnd != 0) {
		// ��ȡɨ�׽���ID
		GetWindowThreadProcessId(hWnd, &Pid);
		// ��ɨ����Ϸ��ȡ����
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
		if (hProcess == 0) {
			printf("Open winmine process failed.");
			return 0;
		}

		// �����������ʼ��ַ
		DWORD dwBoomAddr = 0x01005340;

		// ���������ֵ�������߽磩
		DWORD dwSize = 832;
		PBYTE pByte = NULL;
		pByte = (PBYTE)malloc(dwSize);

		// ��ȡ��������������
		ReadProcessMemory(hProcess, (LPVOID)dwBoomAddr, pByte, dwSize, 0);
		int i = 0;
		int j = 0;
		int n = dwSize;

		// ��ȡ�����ĳ��Ϳ�
		DWORD dwInfo = 0x01005330;
		DWORD dwHeight = 0, dwWidth = 0;
		ReadProcessMemory(hProcess, (LPVOID)(dwInfo + 4), &dwWidth, sizeof(DWORD), 0);    //���
		ReadProcessMemory(hProcess, (LPVOID)(dwInfo + 8), &dwHeight, sizeof(DWORD), 0);   //�߶�

		int h = dwHeight;
		int count = 0;

		// ����ת����ȥ���������������
		PBYTE pTmpByte = NULL;
		pTmpByte = (PBYTE)malloc(dwHeight * dwWidth);
		while (i < dwSize) {
			//�߽��ж�
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

		// ��ȡ������������꣬Ȼ��ģ�������е��
		int x1 = 0, y1 = 0;
		int x = 0, y = 0;
		for (i = 0; i < dwHeight * dwWidth; i++) {
			if (pTmpByte[i] != 0x8F) { //��
				x1 = i % dwWidth;
				y1 = i / dwWidth;
				x = x1 * 16 + 16;
				y = y1 * 16 + 61;
				SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(x, y));   //��갴��
				SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELONG(x, y));     //���̧��
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