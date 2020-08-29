#include "easendmailobj.tlh"
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <codecvt>
#include <fstream>

using namespace std;
using namespace EASendMailObjLib;

const int ConnectNormal = 0;
const int ConnectSSLAuto = 1;
const int ConnectSTARTTLS = 2;
const int ConnectDirectSSL = 3;
const int ConnectTryTLS = 4;

wstringstream readFile(const char* filename)
{
	std::wifstream wif(filename);
	wif.imbue(std::locale(locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();
	return wss;
}

ifstream m_file;
wstring subject, body;
vector<wstring> codes, recipes;

bool command_cmp(const wchar_t* a, const wchar_t* b)
{
	for (; *a && *b; ++a, ++b)
		if (*a != *b) return false;
	return true;
}

const wchar_t* c_code[] =
{
	L":subject:",
	L":recipes:",
	L":body:"
};

int get_command_code(const wstring& command)
{
	for (int i = 0; i < 3; ++i)
		if (command_cmp(command.c_str(), c_code[i]))
			return i;
	return -1;
}

void analysis(vector<wstring>& codes)
{
	int state = -1;
	for (const auto& code : codes)
	{
		if (code[0] == L':')
		{
			state = get_command_code(code);
			continue;
		}
		switch (state)
		{
		case 0:
			subject.append(code);
			subject.append(L" ");
			break;
		case 1:
			recipes.push_back(code);
			break;
		case 2:
			body.append(code);
			body.append(L" ");
			break;
		default:
			break;
		}
	}
}

int main(int argc, _TCHAR* argv[])
{
	//Read mail content
	wstring temp;
	wstringstream wstr = readFile("Content.txt");
	while (wstr >> temp)
	{
		codes.push_back(temp);
	}
	analysis(codes);
	//Setting for email sending
	::CoInitialize(NULL);
	IMailPtr oSmtp = NULL;
	oSmtp.CreateInstance(__uuidof(EASendMailObjLib::Mail));
	oSmtp->LicenseCode = _T("TryIt");
	oSmtp->FromAddr = _T("tuanlinh4321@gmail.com");
	for (auto& i : recipes)
		oSmtp->AddRecipientEx(i.c_str(), 0);
	oSmtp->Subject = subject.c_str();
	oSmtp->BodyText = body.c_str();
	//GetFullPathName(argv[2], MAX_PATH, Path, 0);
	//oSmtp->AddAttachment(Path);
	oSmtp->ServerAddr = _T("smtp.gmail.com");//xD hay la` thg AddRecipientEx kia` no' hong dc 1 luc nhiu` email nho?
	oSmtp->UserName = _T("tuanlinh4321@gmail.com");
	oSmtp->Password = _T("linhtu123");
	oSmtp->ConnectType = ConnectSSLAuto;
	//oSmtp->ServerPort = 465;

	cout << "Start to send email ...\r\n";

	if (oSmtp->SendMail() == 0)
	{
		cout << "Email was sent successfully !!! " << endl;
	}
	else
	{
		cout << "Failed to send email with the following error: \r\n" <<
			oSmtp->GetLastErrDescription();
	}
	system("pause");
	return 0;
}