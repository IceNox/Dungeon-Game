/******************************************************************************************
*    Chili DirectX Framework Version 16.07.20                                             *
*    Main.cpp                                                                             *
*    Copyright 2016 PlanetChili.net <http://www.planetchili.net>                          *
*                                                                                         *
*    This file is part of The Chili DirectX Framework.                                    *
*                                                                                         *
*    The Chili DirectX Framework is free software: you can redistribute it and/or modify  *
*    it under the terms of the GNU General Public License as published by                 *
*    the Free Software Foundation, either version 3 of the License, or                    *
*    (at your option) any later version.                                                  *
*                                                                                         *
*    The Chili DirectX Framework is distributed in the hope that it will be useful,       *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of                       *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                        *
*    GNU General Public License for more details.                                         *
*                                                                                         *
*    You should have received a copy of the GNU General Public License                    *
*    along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>. *
******************************************************************************************/
#include "json.hpp"

#include "MainWindow.h"
#include "Game.h"
#include "ChiliException.h"
#include <fstream>
#include <chrono>
#include <thread>

using json = nlohmann::json;

int WINAPI wWinMain( HINSTANCE hInst,HINSTANCE,LPWSTR pArgs,INT )
{
    try
    {
        int sw, sh;

        // Open config file
        json config;
        std::ifstream in(CONFIG_PATH);
        config << in;

        // Get window parameters
        sw = config["settings"]["resolution"]["width"];
        sh = config["settings"]["resolution"]["height"];

        MainWindow wnd( hInst,pArgs,sw,sh );        
        try
        {
            std::ofstream debug("debugtime.txt");

            Game theGame( wnd );

            while (wnd.ProcessMessage())
            {
                std::chrono::time_point<std::chrono::system_clock> start;
                start = std::chrono::system_clock::now();
                theGame.Go();
                std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - start;
                debug << 1 / elapsed_seconds.count() << "\n";
            }
        }
        catch( const ChiliException& e )
        {
            const std::wstring eMsg = e.GetFullMessage() + 
                L"\n\nException caught at Windows message loop.";
            wnd.ShowMessageBox( e.GetExceptionType(),eMsg );
        }
        catch( const std::exception& e )
        {
            // need to convert std::exception what() string from narrow to wide string
            const std::string whatStr( e.what() );
            const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) + 
                L"\n\nException caught at Windows message loop.";
            wnd.ShowMessageBox( L"Unhandled STL Exception",eMsg );
        }
        catch( ... )
        {
            wnd.ShowMessageBox( L"Unhandled Non-STL Exception",
                L"\n\nException caught at Windows message loop." );
        }
    }
    catch( const ChiliException& e )
    {
        const std::wstring eMsg = e.GetFullMessage() +
            L"\n\nException caught at main window creation.";
        MessageBox( nullptr,eMsg.c_str(),e.GetExceptionType().c_str(),MB_OK );
    }
    catch( const std::exception& e )
    {
        // need to convert std::exception what() string from narrow to wide string
        const std::string whatStr( e.what() );
        const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) +
            L"\n\nException caught at main window creation.";
        MessageBox( nullptr,eMsg.c_str(),L"Unhandled STL Exception",MB_OK );
    }
    catch( ... )
    {
        MessageBox( nullptr,L"\n\nException caught at main window creation.",
            L"Unhandled Non-STL Exception",MB_OK );
    }

    return 0;
}
