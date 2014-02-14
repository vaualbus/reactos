/*
 * PROJECT:     PAINT for ReactOS
 * LICENSE:     LGPL
 * FILE:        base/applications/paint/toolsettings.h
 * PURPOSE:     Window procedure of the tool settings window
 * PROGRAMMERS: Benedikt Freisen
 */

void RegisterWclSettings();

LRESULT CALLBACK SettingsWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
