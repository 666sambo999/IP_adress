#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<CommCtrl.h>
#include<cstdio>
#include"resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DLGPROC(DlgProc),0);
	return 0;
}
CHAR* ParsAddress(CONST CHAR sz_desctiption[], CHAR sz_adress[], DWORD dw_adress)
{
	sprintf
	(
		sz_adress, 
		"%s%i.%i.%i.%i\n", sz_desctiption,
		FIRST_IPADDRESS(dw_adress),
		SECOND_IPADDRESS(dw_adress),
		THIRD_IPADDRESS(dw_adress),
		FOURTH_IPADDRESS(dw_adress)
	);
	return sz_adress;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hSpin = GetDlgItem(hwnd, IDC_EDIT_Pref);
		SendMessage(hSpin, UDM_SETRANGE32, 0, 32);

		SetFocus(GetDlgItem(hwnd, IDC_IPADDRESS));
	}
	break;
	case WM_COMMAND:
	{
		CONST INT SIZE = 256;
		CONST INT SIZE_PREF = 5;
		CONST INT SIZE_INFO = 10240;

		CHAR sz_prefix[SIZE_PREF]{};
		CHAR sz_info[SIZE_INFO]{};
		CHAR sz_network_adress[SIZE]{};
		CHAR sz_broadcast_adress[SIZE]{};
		CHAR sz_number_of_hosts[SIZE]{}; // количество узлов
		CHAR sz_first_hosts_address[SIZE]{};// первый адрес узла 
		CHAR sz_last_hosts_address[SIZE]{};


		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hIPmask = GetDlgItem(hwnd, IDC_Mask);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_Pref);
		HWND hStaricInfo = GetDlgItem(hwnd, IDC_STATIC_Info);
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
		{
			//if (HIWORD(wParam) == EN_CHANGE)

			//	//BYTE = 8 bit
			//	//WORD = 2 BYTE = 16 bit;
			//	//DWORD = 4 BYTE = 32 bit;
			//	//QWORD = 8 BYTE = 64 bit;
			//	//TBYTE = 10 BYTE = 80 bit;
			//	//DQWORD = 16 BYTE = 128 bit;
			//	DWORD dwAddress = 0;
			//	//https://learn.microsoft.com/en-us/windows/win32/controls/ipm-getaddress
			//	SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwAddress);
			//	BYTE first = FIRST_IPADDRESS(dwAddress);
			//	BYTE dw_prefix = 0;
			//	if (first < 128)dw_prefix = 8;
			//	else if (first < 192)dw_prefix = 16;
			//	else if (first < 224)dw_prefix = 24;
			//	CHAR sz_prefix[5]{};
			//	sprintf(sz_prefix, "%i", dw_prefix);
			//	SendMessage(hEditPrefox, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			//}
		}
		break;
		case IDC_EDIT_Pref:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				//MessageBox(hwnd, "Profix changed", "Notification", MB_OK);
				SendMessage(hEditPrefix, WM_GETTEXT, SIZE, (LPARAM)sz_prefix);
				DWORD dw_adress = 0;
				DWORD dw_prefix = atoi(sz_prefix);
				DWORD dw_mask = UINT_MAX;
				ULONG_MAX;
				//dw_mask >>= (32 - dw_prefix);
				dw_mask <<= (32 - dw_prefix);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dw_mask);
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dw_adress);
				DWORD dw_network_adress = dw_adress & dw_mask;
				DWORD dw_broadcast_adress = ~dw_mask | dw_adress;
				/*sprintf
				(
					sz_network_adress, "Адрес сети: \t\t\t%i.%i.%i.%i",
					FIRST_IPADDRESS(dw_network_adress),
					SECOND_IPADDRESS(dw_network_adress),
					THIRD_IPADDRESS(dw_network_adress),
					FOURTH_IPADDRESS(dw_network_adress)
				);*/
				sprintf(sz_number_of_hosts, "Количество узлов:\t%i\n", dw_broadcast_adress - dw_network_adress - 1);
				//ParsAddress("Адрес сети: \t\t", sz_network_adress, dw_network_adress);
				//sprintf(sz_info, "Information:\t%s", sz_network_adress);
				sprintf
				(
					sz_info,
					"Information:\n%s%s%s%s%s",
					ParsAddress("Адрес сети: \t\t", sz_network_adress, dw_adress),
					ParsAddress("Широковещательный адрес:\t", sz_broadcast_adress, dw_broadcast_adress),
					sz_number_of_hosts,
					ParsAddress("Начальный IP-адрес:\t", sz_first_hosts_address, dw_network_adress+1),
					ParsAddress("Конечный IP-адрес:\t", sz_last_hosts_address, dw_broadcast_adress-1)
				);


				SendMessage(hStaricInfo, WM_SETTEXT, 0, (LPARAM)sz_info);
			}
		}
		break;
		}
	}
	break;
	case WM_NOTIFY:
	{
		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hIPmask = GetDlgItem(hwnd, IDC_Mask);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_Pref);
		switch (wParam)
		{
		case IDC_IPADDRESS:
		{
			NMIPADDRESS* pAddress = ((NMIPADDRESS*)(lParam));
			if (pAddress->iField == 0)
			{
				DWORD dw_prefix = 0;
				if (pAddress->iValue < 128)dw_prefix = 8;
				else if (pAddress->iValue < 192)dw_prefix = 16;
				else if (pAddress->iValue < 224)dw_prefix = 24;
				CHAR sz_prefix[5]{};
				sprintf(sz_prefix, "%i", dw_prefix);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			}
		}
		break;
		case IDC_Mask:
		{
			//NMIPADDRESS* pMask = ((NMIPADDRESS*)(lParam));
			if (HIWORD(wParam) == EN_CHANGE)
			{
				DWORD dw_mask = 0;
				SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dw_mask);
				DWORD dw_prefix = 32;
				for (; dw_mask & 1 ^ 1; dw_mask >>= 1)dw_prefix--;
				CONST INT SIZE = 5;
				CHAR sz_prefix[SIZE]{};
				sprintf(sz_prefix, "%i", dw_prefix);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			}
			
		}
		break;
		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}