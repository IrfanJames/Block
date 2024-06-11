// Colapse all Scopes - it'll be easier
// Ctrl + M + A in Visual Studio
//
// All Windows APIs used in Project

#include <iostream>
#include <string>
#include <Windows.h>
#include <Mmdeviceapi.h>
#include <endpointvolume.h>

#include "LOG.hpp"

#include "Windows_Stuff.hpp"

std::string OpenFileDialog(const char* filter, int tempNumber)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	//---------------
	std::string tn = "Project-" + std::to_string(tempNumber);
	for (int c = 0; c < tn.size(); c++)
		szFile[c] = (int)tn[c];
	//---------------


	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	//ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
	//ofn.hwndOwner = CircuitGUI::app.getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();

}

std::string SaveFileDialog(const char* filter, int tempNumber)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	//---------------
	std::string tn = "Untitled-" + std::to_string(tempNumber);
	for (int c = 0; c < tn.size(); c++)
		szFile[c] = (int)tn[c];
	//---------------

	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	//ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	// Sets the default extension by extracting it from the filter
	ofn.lpstrDefExt = strchr(filter, '\0') + 1;

	if (GetSaveFileNameA(&ofn) == TRUE) {
		/*LPCWSTR temp = (wchar_t*)ofn.lpstrFile;
		ShellExecute(NULL, NULL, temp, NULL, NULL, SW_SHOWNORMAL);*/
		return ofn.lpstrFile;
	}

	return std::string();
}

int GetSystemVolumn()
{
	HRESULT hr;
	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice* pDevice = NULL;
	IAudioEndpointVolume* pVolume = NULL;
	float volumeLevel = 0.0f;

	// Initialize COM Library
	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		LOG("Failed to initialize COM Library." << std::endl);
		//std::cerr << "Failed to initialize COM Library." << std::endl;
		return -1;
	}

	// Create IMMDeviceEnumerator
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
		(void**)&pEnumerator);
	if (FAILED(hr)) {
		LOG("Failed to create IMMDeviceEnumerator." << std::endl);
		//std::cerr << "Failed to create IMMDeviceEnumerator." << std::endl;
		CoUninitialize();
		return -1;
	}

	// Get default audio endpoint device
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	if (FAILED(hr)) {
		LOG("Failed to get default audio endpoint device." << std::endl);
		//std::cerr << "Failed to get default audio endpoint device." << std::endl;
		pEnumerator->Release();
		CoUninitialize();
		return -1;
	}

	// Activate IAudioEndpointVolume
	hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL, NULL, (void**)&pVolume);
	if (FAILED(hr)) {
		LOG("Failed to activate IAudioEndpointVolume." << std::endl);
		//std::cerr << "Failed to activate IAudioEndpointVolume." << std::endl;
		pDevice->Release();
		pEnumerator->Release();
		CoUninitialize();
		return -1;
	}

	// Get volume level
	hr = pVolume->GetMasterVolumeLevelScalar(&volumeLevel);
	if (FAILED(hr)) {
		LOG("Failed to get volume level." << std::endl);
		//std::cerr << "Failed to get volume level." << std::endl;
		pVolume->Release();
		pDevice->Release();
		pEnumerator->Release();
		CoUninitialize();
		return -1;
	}

	//std::cout << "System Volume Level: " << volumeLevel * 100 << "%" << std::endl;

	// Release COM objects
	pVolume->Release();
	pDevice->Release();
	pEnumerator->Release();
	CoUninitialize();

	return volumeLevel * 100;
}

tm* GetSystemDateAndTime()
{
	static time_t now = time(0);
	now = time(0);

	//LOG("Number of sec since January 1,1970 is:: " << now << std::endl);

	tm* ltm = localtime(&now);

	return ltm;
}

void wintest() {
	std::cout <<
		MessageBoxW(
			NULL,
			L"Holoy Mooly\nYou Pressed 'M'",
			L"My 1st Box",
			MB_YESNOCANCEL | MB_ICONEXCLAMATION);
}