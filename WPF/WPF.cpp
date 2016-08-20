///////////////////////////////////////////////////////////////////////
//  WPF.cpp - Receives files from the Client                         //
//  Language:      Visual C++ 2008, SP1                              //
//  Platform:      Dell XPS 15, Windows 7 Home Edition               //
//  Application:   Dependency Based Remote Code Repository for       //
//                 CSE687 Pr4, Sp16                                  //
//  Author:		   Sushant Garg, Syracuse University		         //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University       //
//                 (315) 443-3948, jfawcett@twcny.rr.com             //
///////////////////////////////////////////////////////////////////////

#include "WPF.h"
using namespace WPFCS;

WPFDemo::WPFDemo()
{
	// set up channel

	ObjectFactory* pObjFact = new ObjectFactory;
	//pSendr_ = pObjFact->createSendr();
	pChann_ = pObjFact->createMockChannel("localhost", 8081);
	pChann_->start();
	delete pObjFact;



	// set Window properties

	this->Title = "WPF Project 4 Demo";
	this->Width = 800;
	this->Height = 650;
	//::Background;

	// Attaching Dock Panel and Status Bar

	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	hDockPanel->Background = System::Windows::Media::Brushes::Black;

	// Events

	hConnectButton->Click += gcnew RoutedEventHandler(this, &WPFDemo::connectServer);
	hExtractButton->Click += gcnew RoutedEventHandler(this, &WPFDemo::extractfiles);
	hSelectFileButton->Click += gcnew RoutedEventHandler(this, &WPFDemo::browseForFile);
	hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFDemo::uploadFile);
	hConfirmButton->Click += gcnew RoutedEventHandler(this, &WPFDemo::confirmDependencies);
	hSelectedButton->Click += gcnew RoutedEventHandler(this, &WPFDemo::getItemsFromList);
	hGetFilesButton->Click += gcnew RoutedEventHandler(this, &WPFDemo::getItemsFromServer);
	hGetSelectedButton->Click += gcnew RoutedEventHandler(this, &WPFDemo::getSelected);
	hGetFilesExtractButton->Click += gcnew RoutedEventHandler(this, &WPFDemo::getSelectedExtract);

	// setup Window controls and views

	setUpTabControl();
	setUpStatusBar();
	//setUpConnectView();
	setUpCheckInView();
	setUpExtractView();
	setupReadMe();
}

// Destructor
WPFDemo::~WPFDemo()
{
	delete pChann_;
	//delete pSendr_;
}

void WPFDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "Not Connected to Server";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
	hDockPanel->Background = System::Windows::Media::Brushes::Beige;
}

void WPFDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hCheckInTab->Header = "Check In";
	hExtractTab->Header = "Extract";
	hReadMeTab->Header = "Read Me";
	hTabControl->Items->Add(hReadMeTab);
	hTabControl->Items->Add(hCheckInTab);
	hTabControl->Items->Add(hExtractTab);
}

void WPFDemo::setUpConnectView()
{
	Console::Write("\n  setting up Connect View");
	hConnectGrid->Margin = Thickness(20);
	hConnectTab->Content = hConnectGrid;
	hConnectTab->Background = System::Windows::Media::Brushes::YellowGreen;

	setTextBlockProperties();
	setButtonsProperties();
}

void WPFDemo::setUpCheckInView()
{
	Console::Write("\n  setting up Check In View");
	hCheckInGrid->Margin = Thickness(20);
	hCheckInTab->Content = hCheckInGrid;

	setFileSelectProperties();
	
}

void WPFDemo::setUpExtractView()
{
	Console::Write("\n  setting up Extract View");
	hExtractGrid->Margin = Thickness(20);
	hExtractTab->Content = hExtractGrid;
	setExtractProperties();
}

void WPFCS::WPFDemo::setupReadMe()
{
	hReadMeGrid->Margin = Thickness(20);
	hReadMeTab->Content = hReadMeGrid;

	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hReadMeGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlockR;
	hTextBlockR->Padding = Thickness(15);
	hTextBlockR->FontWeight = FontWeights::Bold;
	hTextBlockR->FontSize = 14;
	setupReadMeText();
	hScrollVeiwerR->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollVeiwerR->Content = hBorder1;
	hReadMeGrid->SetRow(hScrollVeiwerR, 0);
	hReadMeGrid->Children->Add(hScrollVeiwerR);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hReadMeGrid->RowDefinitions->Add(hRow2Def);
}

void WPFDemo::setTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hConnectGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;
	hTextBlock1->Padding = Thickness(5);
	hTextBlock1->Text = "";
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 16;
	hTextBlock1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hConnectGrid->SetRow(hTextBlock1, 0);
	hConnectGrid->Children->Add(hBorder1);
}

void WPFDemo::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(375);
	hConnectGrid->RowDefinitions->Add(hRow2Def);
	hConnectButton->Content = "Connect";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hConnectButton;
	hStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hConnectGrid->SetRow(hStackPanel1, 1);
	hConnectGrid->Children->Add(hStackPanel1);
}

void WPFDemo::setComboProperties()
{
	RowDefinition^ hRow8Def = gcnew RowDefinition();
	hCheckInGrid->RowDefinitions->Add(hRow8Def);
	hTextBlock2->Padding = Thickness(15);
	hTextBlock2->Text = "Files:";
	hTextBlock2->FontWeight = FontWeights::Bold;
	hTextBlock2->FontSize = 16;
	hTextBlock2->Height = 40;
	hStackPanel2->Children->Add(hTextBlock2);
	RowDefinition^ hRow9Def = gcnew RowDefinition();
	hCheckInGrid->RowDefinitions->Add(hRow9Def);
	for (size_t i = 1; i < 11; i++)
	{
		hCheckInFile->Items->Add("File" + i);
	}
	hCheckInFile->Width = 200;
	hCheckInFile->MaxHeight = 30;
	hCheckInFile->MaxDropDownHeight = 100;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hCheckInFile);
	hStackPanel2->Orientation = Orientation::Horizontal;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hCheckInGrid->SetRow(hStackPanel2, 2);
	hCheckInGrid->Children->Add(hStackPanel2);
}

void WPFDemo::setFileSelectProperties()
{
	RowDefinition^ hRow6Def = gcnew RowDefinition();
	hCheckInGrid->RowDefinitions->Add(hRow6Def);
	Border^ hBorder4 = gcnew Border();
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hListBox->SelectionMode = SelectionMode::Multiple;
	hBorder4->Child = hListBox; hBorder4->Width = 350;
	hBorder4->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hCheckInGrid->SetColumn(hBorder4, 0); hCheckInGrid->Children->Add(hBorder4);
	RowDefinition^ hRow7Def = gcnew RowDefinition();
	hCheckInGrid->RowDefinitions->Add(hRow7Def);
	hSelectFileButton->Content = "Select File";
	Border^ hBorder5 = gcnew Border();
	hBorder5->Width = 120; hBorder5->Height = 25; hBorder5->Margin = Thickness(100, 0, 0, 0);
	hBorder5->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hBorder5->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hBorder5->BorderThickness = Thickness(1); hBorder5->BorderBrush = Brushes::Black;
	hUploadButton->Content = "Upload"; hUploadButton->IsEnabled = false;
    hBorder5->Child = hSelectFileButton;
	Border^ hBorder6 = gcnew Border();
	hBorder6->Width = 120; hBorder6->Height = 25; hBorder6->Margin = Thickness(100, 0, 0, 0);
	hBorder6->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hBorder6->HorizontalAlignment = System::Windows::HorizontalAlignment::Left; hBorder6->BorderThickness = Thickness(1);
	hBorder6->BorderBrush = Brushes::Black; hBorder6->Child = hUploadButton;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 20; hStackPanel3->Children->Add(hSpacer2);
	hStackPanel3->Children->Add(hBorder5); hSelectedButton->Content = "Get Selected";
	Border^ hBorder9 = gcnew Border();
	hBorder9->Width = 120; hBorder9->Height = 25; hBorder9->Margin = Thickness(100, 0, 0, 0);
	hBorder9->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hBorder9->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hBorder9->BorderThickness = Thickness(1);
	hBorder9->BorderBrush = Brushes::Black;
	hBorder9->Child = hSelectedButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 20; hStackPanel3->Children->Add(hSpacer);
	hStackPanel3->Children->Add(hBorder9);
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 20; hStackPanel3->Children->Add(hSpacer3);
	hStackPanel3->Children->Add(hBorder6);
	hStackPanel3->Orientation = Orientation::Vertical;
	hStackPanel3->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hCheckInGrid->SetRow(hStackPanel3, 1);
	hCheckInGrid->Children->Add(hStackPanel3);
	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

void WPFDemo::setFileDependencies()
{
	RowDefinition^ hRow10Def = gcnew RowDefinition();
	hCheckInGrid->RowDefinitions->Add(hRow10Def);
	Border^ hBorder7 = gcnew Border();
	hBorder7->BorderThickness = Thickness(1);
	hBorder7->BorderBrush = Brushes::Black;
	hListBox1->SelectionMode = SelectionMode::Multiple;
	hBorder7->Child = hListBox1; hBorder7->Width = 350;
	hBorder7->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hCheckInGrid->SetColumn(hBorder7, 1);
	hCheckInGrid->Children->Add(hBorder7);
	TextBlock^ hSpacer11 = gcnew TextBlock();
	hSpacer11->Width = 20; hStackPanel4->Children->Add(hSpacer11);
    RowDefinition^ hRow12Def = gcnew RowDefinition();
	hCheckInGrid->RowDefinitions->Add(hRow12Def);
	hGetFilesButton->Content = "Get Files";
	Border^ hBorder10 = gcnew Border();
	hBorder10->Width = 120; hBorder10->Height = 25; hBorder10->Margin = Thickness(0, 0, 100, 0);
	hBorder10->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hBorder10->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hBorder10->BorderThickness = Thickness(1);
	hBorder10->BorderBrush = Brushes::Black;
    hGetSelectedButton->Content = "Get Selected";
	hBorder10->Child = hGetFilesButton; Border^ hBorder11 = gcnew Border();
	hBorder11->Width = 120; hBorder11->Height = 25; hBorder11->Margin = Thickness(0, 0, 100, 0); hBorder11->BorderThickness = Thickness(1);
	hBorder11->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hBorder11->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hBorder11->BorderBrush = Brushes::Black;
	hBorder11->Child = hGetSelectedButton;
	hStackPanel4->Children->Add(hBorder10);
	hConfirmButton->Content = "Confirm Dependencies";
	Border^ hBorder12 = gcnew Border();
	hBorder12->Width = 140; hBorder12->Height = 25; hBorder12->Margin = Thickness(0, 0, 100, 0);
	hBorder12->BorderThickness = Thickness(1);
	hBorder12->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hBorder12->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hBorder12->BorderBrush = Brushes::Black;
	hBorder12->Child = hConfirmButton;
	TextBlock^ hSpacer4 = gcnew TextBlock(); hSpacer4->Width = 20;
	hStackPanel4->Children->Add(hSpacer4);
	hStackPanel4->Children->Add(hBorder11);
	TextBlock^ hSpacer5 = gcnew TextBlock(); hSpacer5->Width = 20;
	hStackPanel4->Children->Add(hSpacer5);
	hStackPanel4->Children->Add(hBorder12);
	hStackPanel4->Orientation = Orientation::Vertical;
	hCheckInGrid->SetRow(hStackPanel4, 1);
	hCheckInGrid->Children->Add(hStackPanel4);
}

void WPFDemo::setExtractProperties()
{
	// List Box for Files
	RowDefinition^ hRow13Def = gcnew RowDefinition();
	hExtractGrid->RowDefinitions->Add(hRow13Def);
	Border^ hBorder13 = gcnew Border();
	hBorder13->BorderThickness = Thickness(1);
	hBorder13->BorderBrush = Brushes::Black;
	hListBox2->SelectionMode = SelectionMode::Multiple;
	hBorder13->Child = hListBox2;
	hBorder13->Height = 160;
	hExtractGrid->SetRow(hBorder13, 0);
	hExtractGrid->Children->Add(hBorder13);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 30;
	hStackPanel6->Children->Add(hSpacer2);
	hStackPanel6->Orientation = Orientation::Horizontal;
	hStackPanel6->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hExtractGrid->SetRow(hStackPanel6, 1);
	hExtractGrid->Children->Add(hStackPanel6);
	hGetFilesExtractButton->Content = "Get Files";
	Border^ hBorder14 = gcnew Border();
	hBorder14->Width = 120;
	hBorder14->Height = 30;
	hBorder14->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hBorder14->VerticalAlignment = System::Windows::VerticalAlignment::Bottom;
	hBorder14->BorderThickness = Thickness(1);
	hBorder14->BorderBrush = Brushes::Black;
	hBorder14->Child = hGetFilesExtractButton;
	hStackPanel5->Children->Add(hBorder14);
	hExtractButton->Content = "Extract";
	Border^ hBorder15 = gcnew Border();
	hBorder15->Width = 140;
	hBorder15->Height = 30;
	hBorder15->BorderThickness = Thickness(1);
	hBorder15->BorderBrush = Brushes::Black;
	hBorder15->Child = hExtractButton;
	TextBlock^ hSpacer6 = gcnew TextBlock();
	hSpacer6->Width = 20;
	hStackPanel5->Children->Add(hSpacer6);
	hStackPanel5->Children->Add(hBorder15);
	hStackPanel5->Orientation = Orientation::Horizontal;
	hBorder15->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hBorder15->VerticalAlignment = System::Windows::VerticalAlignment::Bottom;
	hExtractGrid->SetRow(hStackPanel5, 2);
	hExtractGrid->Children->Add(hStackPanel5);
}


void WPFCS::WPFDemo::setupReadMeText()
{
	hTextBlockR->Text = " IMPORTANT : Select only one file at a time! \n\nThere are 3 TABS - ReadMe Tab, Checkin Tab & Extract Tab \n\n ReadMe Tab is for providing information as to how to go ahead & use this application"
						+ "\n\nCheckin Tab :\n" + "Select File: When a user clicks on this button, he is given an option to search for a file \n to upload to a server.\n\n"
	                    + "Get Selected: Using this button a user can select a particular file from listbox.\n\n"
		                + "Upload: On pressing this button, the selected file/files are sent to the server.\n\n"
						+ "Confirm Dependency: Clicking on this would lead to creation of a folder with the name of \n the file uploaded along with the " 
		                + "time of creation which helps the user know about the file versioning. \nThis folder can be found in 'ServerRoot' folder "
		                + "located in the main project folder.\n\n"
		                + "Each folder has a copy of the file uploaded along with a metadata.xml which when opened shows \n the dependencies. \n\n"
						+ "Extract Tab :\n"
		                + "In here the user is presented with another list box which shows that files have \n been uploaded to a server, i.e., in the ServerRoot folder\n\n"
		                + "On clicking the Extract button the files are sent back to the client which is the Files folder in our case.";
}

// Connect Button Status Bar Update
void WPFDemo::connectServer(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  server connection request");
	pChann_->postMessage("CONNECT", "");
	hStatus->Text = "Connected to Server";
	hTextBlock1->Text = msgText;
	hCheckInTab->IsEnabled = true;
	hExtractTab->IsEnabled = true;
}

// Extract Button Status Bar Update
void WPFDemo::extractfiles(Object^ obj, RoutedEventArgs^ args)
{
	int count = hListBox2->SelectedItems->Count;
	StringBuilder^ sbuild = gcnew StringBuilder();

	if (count > 0)
	{
		for each (String^ item in hListBox2->SelectedItems)
		{
			sbuild->Append(item);
			sbuild->Append(",");
		}
	}
	if (hCheckBox1->IsChecked)
		sbuild->Append("|DEPENDENCY");

	pChann_->postMessage("EXTRACT", toStdString(sbuild->ToString()));
}

// Select File Button Browse System Folder
void WPFDemo::browseForFile(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for file";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		if (files->Length > 0)
			hUploadButton->IsEnabled = true;
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox->Items->Add(L"<> " + dirs[i]);
	}
}

// Get Selected Files Button
void WPFDemo::getItemsFromList(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = hListBox->SelectedItems->Count;
	hStatus->Text = "Updated Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
		}
	}

	hListBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox->Items->Add(item);
		}
	}
}

// Upload button Status Update
void WPFDemo::uploadFile(Object^ sender, RoutedEventArgs^ args)
{
	int count = hListBox->SelectedItems->Count;
	StringBuilder^ sbuild = gcnew StringBuilder();

	if (count > 0)
	{
		for each (String^ item in hListBox->SelectedItems)
		{
			sbuild->Append(item);
			sbuild->Append(",");
		}
	}
	uploadFiles = sbuild->ToString();
	//pChann_->postMessage("CHECKIN", name);
	//hStatus->Text = "Uploaded files to server";
	//Console::Write("\n  Uploading File");
	setFileDependencies();

	pChann_->postMessage("GETFILES", "");
	std::string files = pChann_->getMessage();
	int pos;
	while (pos != std::string::npos)
	{
		pos = files.find(',');
		std::string temp = files.substr(0, pos);
		String^ vFiles = toSystemString(temp);
		hListBox1->Items->Add(vFiles);
		files.erase(0, pos + 1);
	}

	hListBox->IsEnabled = false;
	hUploadButton->IsEnabled = false;
	hSelectFileButton->IsEnabled = false;
	hSelectedButton->IsEnabled = false;
}

// Confirm button Status Update
void WPFDemo::confirmDependencies(Object^ sender, RoutedEventArgs^ args)
{
	int count = hListBox1->SelectedItems->Count;
	StringBuilder^ sbuild = gcnew StringBuilder();

	if (count > 0)
	{
		for each (String^ item in hListBox1->SelectedItems)
		{
			sbuild->Append(item);
			sbuild->Append(",");
		}
	}

	pChann_->postMessage("CHECKIN", toStdString(uploadFiles + "|" + sbuild->ToString()));

	hStatus->Text = "Uploaded files to server";
	Console::Write("\n  Uploading File");
	Console::Write("\n  Dependencies Updated");

	hListBox->IsEnabled = true;
	hUploadButton->IsEnabled = true;
	hSelectFileButton->IsEnabled = true;
	hSelectedButton->IsEnabled = true;

	pChann_->postMessage("GETFILES", "");
	std::string files = pChann_->getMessage();
	int pos;
	while (pos != std::string::npos)
	{
		pos = files.find(',');
		std::string temp = files.substr(0, pos);
		String^ vFiles = toSystemString(temp);
		hListBox2->Items->Add(vFiles);
		files.erase(0, pos + 1);
	}
}

// Get Files button 
void WPFDemo::getItemsFromServer(Object^ sender, RoutedEventArgs^ args)
{
	//pChann_->postMessage("GETFILES", "");
	//std::string files = pChann_->getMessage();
	//int pos;
	//while (pos != std::string::npos)
	//{
	//	pos = files.find(',');
	//	std::string temp = files.substr(0, pos);
	//	String^ vFiles = toSystemString(temp);
	//	hListBox1->Items->Add(vFiles);
	//	files.erase(0, pos + 1);
	//}
}

// Get Selected button
void WPFDemo::getSelected(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = hListBox1->SelectedItems->Count;
	hStatus->Text = "Updated Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox1->SelectedItems)
		{
			items[index++] = item;
		}
	}

	hListBox1->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox1->Items->Add(item);
		}
	}
}

// Get Selected button in Extract
void WPFDemo::getSelectedExtract(Object^ sender, RoutedEventArgs^ args)
{
	//int index = 0;
	//int count = hListBox2->SelectedItems->Count;
	//hStatus->Text = "Updated Items";
	//array<System::String^>^ items = gcnew array<String^>(count);
	//if (count > 0) {
	//	for each (String^ item in hListBox2->SelectedItems)
	//	{
	//		items[index++] = item;
	//	}
	//}

	//hListBox2->Items->Clear();
	//if (count > 0) {
	//	for each (String^ item in items)
	//	{
	//		hListBox2->Items->Add(item);
	//	}
	//}
	pChann_->postMessage("GETFILES", "");
	std::string files = pChann_->getMessage();
	int pos = 0;
	while (pos != std::string::npos)
	{
		pos = files.find(',');
		std::string temp = files.substr(0, pos);
		String^ vFiles = toSystemString(temp);
		hListBox2->Items->Add(vFiles);
		files.erase(0, pos + 1);
	}
}

// Get Files from Server
void WPFDemo::getMessage()
{
	// recvThread runs this function

	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		array<String^>^ args = gcnew array<String^>(100);
		while (true)
		{
			int i = 0;
			std::string msg = pChann_->getMessage();
			//int pos = msg.find(',');
			int pos;
			while ((pos = msg.find(',')) != std::string::npos)
			{
				std::string mSg = ((msg.substr(0, pos)));
				String^ sMsg = toSystemString(msg);
				args[i] = sMsg;
				i++;
				msg.erase(0, pos + 1);
				Action<String^>^ act = gcnew Action<String^>(this, &WPFDemo::addText);
				Dispatcher->Invoke(act, args);  // must call addText on main UI thread
			}
		}
	}
}

// Add Text Recieved by Reciever Thread
void WPFDemo::addText(String^ msg)
{
	//hTextBlock1->Text += msg + "\n\n";
	hListBox1->Items->Add(msg);
}

// Converts System^ string to std::string
std::string WPFDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

// Converts std::string to System^ string
String^ WPFDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

[STAThread]
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFDemo");
	Application^ app = gcnew Application();
	app->Run(gcnew WPFDemo());
	Console::WriteLine(L"\n\n");
}

//void start()
//{
//	::SetConsoleTitle(L"Clients Running on Threads");
//	Show::title("Demonstrating two HttpMessage Clients each running on a child thread");
//	MsgClient c1("localhost:8081");
//
//}