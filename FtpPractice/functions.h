#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define BTN_UPLOAD 1
#define BTN_DOWNLOAD 2

#include <wininet.h>

void InitializeUI(HWND hMainWIndow);
void UploadFile();
void DownloadFile();

#endif // FUNCTIONS_H_INCLUDED
