﻿
// VideoEffects.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CVideoEffectsApp:
// Сведения о реализации этого класса: VideoEffects.cpp
//

class CVideoEffectsApp : public CWinApp
{
public:
	CVideoEffectsApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CVideoEffectsApp theApp;
