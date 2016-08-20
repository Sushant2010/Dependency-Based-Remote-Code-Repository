#ifndef WPF_H
#define WPF_H

/////////////////////////////////////////////////////////////////////
//  WPF.h -  For GUI                                               //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell XPS 15, Windows 7 Home Edition             //
//  Application:   Dependency Based Remote Code Repository for     //
//                 CSE687 Pr4, Sp16                                //
//  Author:		   Sushant Garg, Syracuse University		       //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*

Module Operations:
==================
This module is for designing Graphical User Interface.

Build commands
- devenv CommPrototype.sln

Maintenance History:
====================
ver 1.0 : 4 May 16
- first release

*/



using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>
#include <string>


namespace WPFCS
{
	ref class WPFDemo : Window
	{
		// MockChannel references

		//ISendr* pSendr_;
		IMockChannel* pChann_;
		// Main Window Controls

		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hConnectTab = gcnew TabItem();         // Connect Tab
		TabItem^ hCheckInTab = gcnew TabItem();         // CheckIn Tab
		TabItem^ hExtractTab = gcnew TabItem();         // Extract Tab
		TabItem^ hReadMeTab = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();

		// Controls for Connect Tab View

		Button^ hConnectButton = gcnew Button();
		TextBlock^ hTextBlock1 = gcnew TextBlock();
		Grid^ hConnectGrid = gcnew Grid();
		StackPanel^ hStackPanel1 = gcnew StackPanel();

		String^ msgText = "Connected to Server\n"   // command
			+ "Sendr:localhost@8080\n"  // send address
			+ "Recvr:localhost@8090\n"  // receive address
			+ "\n";                     // end header

										// Controls for CheckIn Tab View

		Grid^ hCheckInGrid = gcnew Grid();
		ComboBox^ hCheckInFile = gcnew ComboBox();
		TextBlock^ hTextBlock2 = gcnew TextBlock();
		Button^ hSelectFileButton = gcnew Button();
		Button^ hSelectedButton = gcnew Button();
		Button^ hUploadButton = gcnew Button();
		Button^ hGetFilesButton = gcnew Button();
		Button^ hGetSelectedButton = gcnew Button();
		Button^ hConfirmButton = gcnew Button();
		TextBlock^ hTextBlock3 = gcnew TextBlock();
		StackPanel^ hStackPanel2 = gcnew StackPanel();
		StackPanel^ hStackPanel3 = gcnew StackPanel();
		StackPanel^ hStackPanel4 = gcnew StackPanel();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		Forms::OpenFileDialog^ hOpenFileDialog = gcnew Forms::OpenFileDialog();
		ListBox^ hListBox = gcnew ListBox();
		ListBox^ hListBox1 = gcnew ListBox();

		// Controls for Extract Tab View

		Grid^ hExtractGrid = gcnew Grid();
		ComboBox^ hExtractFile = gcnew ComboBox();
		CheckBox^ hCheckBox1 = gcnew CheckBox();
		Button^ hExtractButton = gcnew Button();
		Button^ hGetFilesExtractButton = gcnew Button();
		StackPanel^ hStackPanel5 = gcnew StackPanel();
		StackPanel^ hStackPanel6 = gcnew StackPanel();
		TextBlock^ hTextBlock4 = gcnew TextBlock();
		TextBlock^ hTextBlock5 = gcnew TextBlock();
		TextBox^ hTextBox1 = gcnew TextBox();
		ListBox^ hListBox2 = gcnew ListBox();

		//these are the controls for the Read Me Tab
		Grid^ hReadMeGrid = gcnew Grid();
		TextBlock^ hTextBlockR = gcnew TextBlock();
		ScrollViewer^ hScrollVeiwerR = gcnew ScrollViewer();

		// receive thread

		Thread^ recvThread;

	public:
		WPFDemo();
		~WPFDemo();

		void setUpStatusBar();
		void setUpTabControl();
		void setUpConnectView();
		void setUpCheckInView();
		void setUpExtractView();
		void setupReadMe();

		void connectServer(Object^ obj, RoutedEventArgs^ args);
		void extractfiles(Object^ obj, RoutedEventArgs^ args);
		void browseForFile(Object^ sender, RoutedEventArgs^ args);
		void uploadFile(Object^ sender, RoutedEventArgs^ args);
		void confirmDependencies(Object^ sender, RoutedEventArgs^ args);
		void getItemsFromList(Object^ sender, RoutedEventArgs^ args);
		void getItemsFromServer(Object^ sender, RoutedEventArgs^ args);
		void getSelected(Object^ sender, RoutedEventArgs^ args);
		void getSelectedExtract(Object^ sender, RoutedEventArgs^ args);


		void getMessage();
		void addText(String^ msg);

	private:

		void setTextBlockProperties();         // Connect Tab
		void setButtonsProperties();           // Connect Tab
		void setComboProperties();             // CheckIn Tab
		void setFileSelectProperties();        // CheckIn Tab
		void setFileDependencies();            // CheckIn Tab
		void setExtractProperties();           // Extract Tab
		void setupReadMeText();

		std::string toStdString(String^ pStr);       // Converts String^ -> std::string
		String^ toSystemString(std::string& str);    // Converts std::string -> String^
		String^ uploadFiles;
	};
}

#endif