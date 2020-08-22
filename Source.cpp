#include "easendmailobj.tlh"
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace EASendMailObjLib;

const int ConnectNormal = 0;
const int ConnectSSLAuto = 1;
const int ConnectSTARTTLS = 2;
const int ConnectDirectSSL = 3;
const int ConnectTryTLS = 4;


int main(int argc, _TCHAR* argv[])
{
	//Read mail content
	char str[1000];
	char Path[MAX_PATH];
	FILE* fp;
	fp = fopen("Content.txt", "r");
	size_t stream = fread(str, sizeof(char),1000, fp);
	if (ferror(fp) != 0) {
		fputs("Error reading content", stderr);
	}
	else {
		str[stream++] = 0;//Add escape character for any single string
	}
	fclose(fp);
	//Setting for email sending
	::CoInitialize(NULL);
	IMailPtr oSmtp = NULL;
	oSmtp.CreateInstance(__uuidof(EASendMailObjLib::Mail));
	oSmtp->LicenseCode = _T("TryIt");
	oSmtp->FromAddr = _T("...");
	oSmtp->AddRecipientEx(argv[2], 0);
	oSmtp->Subject = _T(argv[1]);
	oSmtp->BodyText = str;
	GetFullPathName(argv[3], MAX_PATH, Path, 0);
	oSmtp->AddAttachment(Path);
	oSmtp->ServerAddr = _T("smtp.gmail.com");
	oSmtp->UserName = _T("...");
	oSmtp->Password = _T("...");
	oSmtp->ConnectType = ConnectSSLAuto;
	//oSmtp->ServerPort = 465;

	cout << "Start to send email ...\r\n";

	if (oSmtp->SendMail() == 0)
	{
		cout << "Email was sent successfully!\r\n";
	}
	else
	{
		cout << "Failed to send email with the following error: \r\n" <<
			oSmtp->GetLastErrDescription();
	}
	system("pause");
	return 0;
}