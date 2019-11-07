#pragma once

const short UNSELECTED_INDEX = -1;

namespace BushWinApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm( void )
		{
			InitializeComponent();
			fnStaticMemberInit();
			fnOnStart();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if ( components )
			{
				delete components;
			}
		}
	private:
		System::Windows::Forms::Label^  labelPort;
		System::Windows::Forms::CheckBox^  chBoxLockDoor;
		System::Windows::Forms::CheckBox^  chBoxOverheat;
		System::Windows::Forms::ComboBox^  comBoxPortNames;
		System::Windows::Forms::NotifyIcon^  trayNotification;
		System::Windows::Forms::ContextMenuStrip^  trayMenu;
		System::Windows::Forms::ToolStripMenuItem^  trayMenuItemDoor;
		System::Windows::Forms::ToolStripMenuItem^  trayMenuItemSettings;
		System::Windows::Forms::ToolStripMenuItem^  trayMenuItemExit;
		System::Windows::Forms::GroupBox^  groupBushInfo;
		System::Windows::Forms::Label^  labelNameBushRelay;
		System::Windows::Forms::Label^  labelNameBushSens;
		System::Windows::Forms::Label^  labelNameBushLock;
		System::Windows::Forms::Label^  labelNameBushDoor;
		System::Windows::Forms::Label^  labelNameBushConnect;
		System::Windows::Forms::Label^  labelBushConnect;
		System::Windows::Forms::Label^  labelBushDoor;
		System::Windows::Forms::Label^  labelBushRelay;
		System::Windows::Forms::Label^  labelBushSens;
		System::Windows::Forms::Label^  labelBushLock;
		System::Windows::Forms::Timer^  timerCheckData;
		System::Windows::Forms::GroupBox^  groupBoxTrayConfig;
		System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required programmer variables
		/// </summary>
		Boolean m_isDoorClosed;
		Boolean m_isLockLocked;
		Boolean m_isRelayOn;
		int m_lastBushStatus;
		short m_lastSelectedComBoxIndex;

		Drawing::Icon^ m_pIcoDisconnect;
		Drawing::Icon^ m_pIcoOpen;
		Drawing::Icon^ m_pIcoClose;
		Drawing::Icon^ m_pIcoLock;
		Drawing::Icon^ m_pIcoOverHeat;

		IO::FileStream^ m_pLogFile;
		Xml::XmlDocument^ m_pConfigXmlFile;
		String^ m_pXmlConfigFilePath;
		String^ m_pSuccessPortConnectedName;

	private:
		/// <summary>
		/// Required designer variable
		/// </summary>

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent( void )
		{
			this->components = ( gcnew System::ComponentModel::Container() );
			this->labelPort = ( gcnew System::Windows::Forms::Label() );
			this->chBoxLockDoor = ( gcnew System::Windows::Forms::CheckBox() );
			this->chBoxOverheat = ( gcnew System::Windows::Forms::CheckBox() );
			this->comBoxPortNames = ( gcnew System::Windows::Forms::ComboBox() );
			this->trayNotification = ( gcnew System::Windows::Forms::NotifyIcon( this->components ) );
			this->trayMenu = ( gcnew System::Windows::Forms::ContextMenuStrip( this->components ) );
			this->trayMenuItemDoor = ( gcnew System::Windows::Forms::ToolStripMenuItem() );
			this->trayMenuItemSettings = ( gcnew System::Windows::Forms::ToolStripMenuItem() );
			this->trayMenuItemExit = ( gcnew System::Windows::Forms::ToolStripMenuItem() );
			this->groupBushInfo = ( gcnew System::Windows::Forms::GroupBox() );
			this->labelBushRelay = ( gcnew System::Windows::Forms::Label() );
			this->labelBushSens = ( gcnew System::Windows::Forms::Label() );
			this->labelBushLock = ( gcnew System::Windows::Forms::Label() );
			this->labelBushDoor = ( gcnew System::Windows::Forms::Label() );
			this->labelBushConnect = ( gcnew System::Windows::Forms::Label() );
			this->labelNameBushRelay = ( gcnew System::Windows::Forms::Label() );
			this->labelNameBushSens = ( gcnew System::Windows::Forms::Label() );
			this->labelNameBushLock = ( gcnew System::Windows::Forms::Label() );
			this->labelNameBushDoor = ( gcnew System::Windows::Forms::Label() );
			this->labelNameBushConnect = ( gcnew System::Windows::Forms::Label() );
			this->timerCheckData = ( gcnew System::Windows::Forms::Timer( this->components ) );
			this->groupBoxTrayConfig = ( gcnew System::Windows::Forms::GroupBox() );
			this->trayMenu->SuspendLayout();
			this->groupBushInfo->SuspendLayout();
			this->groupBoxTrayConfig->SuspendLayout();
			this->SuspendLayout();
			// 
			// labelPort
			// 
			this->labelPort->AutoSize = true;
			this->labelPort->Location = System::Drawing::Point( 18, 21 );
			this->labelPort->Margin = System::Windows::Forms::Padding( 4, 0, 4, 0 );
			this->labelPort->Name = L"labelPort";
			this->labelPort->Size = System::Drawing::Size( 47, 18 );
			this->labelPort->TabIndex = 1;
			this->labelPort->Text = L"Порт:";
			// 
			// chBoxLockDoor
			// 
			this->chBoxLockDoor->AutoSize = true;
			this->chBoxLockDoor->Checked = true;
			this->chBoxLockDoor->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chBoxLockDoor->Location = System::Drawing::Point( 7, 23 );
			this->chBoxLockDoor->Margin = System::Windows::Forms::Padding( 4, 3, 4, 3 );
			this->chBoxLockDoor->Name = L"chBoxLockDoor";
			this->chBoxLockDoor->Size = System::Drawing::Size( 207, 22 );
			this->chBoxLockDoor->TabIndex = 2;
			this->chBoxLockDoor->Text = L"Состояние двери и замка";
			this->chBoxLockDoor->UseVisualStyleBackColor = true;
			this->chBoxLockDoor->CheckedChanged += gcnew System::EventHandler( this, &MyForm::chBoxLockDoor_CheckedChanged );
			// 
			// chBoxOverheat
			// 
			this->chBoxOverheat->AutoSize = true;
			this->chBoxOverheat->Checked = true;
			this->chBoxOverheat->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chBoxOverheat->Location = System::Drawing::Point( 7, 51 );
			this->chBoxOverheat->Margin = System::Windows::Forms::Padding( 4, 3, 4, 3 );
			this->chBoxOverheat->Name = L"chBoxOverheat";
			this->chBoxOverheat->Size = System::Drawing::Size( 273, 22 );
			this->chBoxOverheat->TabIndex = 3;
			this->chBoxOverheat->Text = L"Превышение рабочей температуры";
			this->chBoxOverheat->UseVisualStyleBackColor = true;
			this->chBoxOverheat->CheckedChanged += gcnew System::EventHandler( this, &MyForm::chBoxLockDoor_CheckedChanged );
			// 
			// comBoxPortNames
			// 
			this->comBoxPortNames->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comBoxPortNames->FormattingEnabled = true;
			this->comBoxPortNames->Location = System::Drawing::Point( 73, 18 );
			this->comBoxPortNames->Name = L"comBoxPortNames";
			this->comBoxPortNames->Size = System::Drawing::Size( 288, 26 );
			this->comBoxPortNames->TabIndex = 4;
			this->comBoxPortNames->DropDown += gcnew System::EventHandler( this, &MyForm::comBoxPortNames_DropDown );
			this->comBoxPortNames->SelectionChangeCommitted += gcnew System::EventHandler( this, &MyForm::comBoxPortNames_SelectionChangeCommitted );
			this->comBoxPortNames->DropDownClosed += gcnew System::EventHandler( this, &MyForm::comBoxPortNames_DropDownClosed );
			// 
			// trayNotification
			// 
			this->trayNotification->ContextMenuStrip = this->trayMenu;
			this->trayNotification->Text = L"ПО Эдельвейс-СС-19-С";
			this->trayNotification->Visible = true;
			this->trayNotification->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler( this, &MyForm::trayIcon_MouseDoubleClick );
			// 
			// trayMenu
			// 
			this->trayMenu->Items->AddRange( gcnew cli::array< System::Windows::Forms::ToolStripItem^  >( 3 ) {
				this->trayMenuItemDoor, this->trayMenuItemSettings,
					this->trayMenuItemExit
			} );
			this->trayMenu->Name = L"contextMenuStrip1";
			this->trayMenu->Size = System::Drawing::Size( 134, 70 );
			this->trayMenu->Text = L"notyfyIcon1Menu";
			// 
			// trayMenuItemDoor
			// 
			this->trayMenuItemDoor->Name = L"trayMenuItemDoor";
			this->trayMenuItemDoor->Size = System::Drawing::Size( 133, 22 );
			this->trayMenuItemDoor->Visible = false;
			this->trayMenuItemDoor->Click += gcnew System::EventHandler( this, &MyForm::trayMenuItemDoor_Click );
			// 
			// trayMenuItemSettings
			// 
			this->trayMenuItemSettings->Name = L"trayMenuItemSettings";
			this->trayMenuItemSettings->Size = System::Drawing::Size( 133, 22 );
			this->trayMenuItemSettings->Text = L"Настройка";
			this->trayMenuItemSettings->Click += gcnew System::EventHandler( this, &MyForm::toolStripMenuItem2_Click );
			// 
			// trayMenuItemExit
			// 
			this->trayMenuItemExit->BackColor = System::Drawing::SystemColors::Control;
			this->trayMenuItemExit->Name = L"trayMenuItemExit";
			this->trayMenuItemExit->Size = System::Drawing::Size( 133, 22 );
			this->trayMenuItemExit->Text = L"Выход";
			this->trayMenuItemExit->Click += gcnew System::EventHandler( this, &MyForm::toolStripMenuItem1_Click );
			// 
			// groupBushInfo
			// 
			this->groupBushInfo->BackColor = System::Drawing::SystemColors::Control;
			this->groupBushInfo->Controls->Add( this->labelBushRelay );
			this->groupBushInfo->Controls->Add( this->labelBushSens );
			this->groupBushInfo->Controls->Add( this->labelBushLock );
			this->groupBushInfo->Controls->Add( this->labelBushDoor );
			this->groupBushInfo->Controls->Add( this->labelBushConnect );
			this->groupBushInfo->Controls->Add( this->labelNameBushRelay );
			this->groupBushInfo->Controls->Add( this->labelNameBushSens );
			this->groupBushInfo->Controls->Add( this->labelNameBushLock );
			this->groupBushInfo->Controls->Add( this->labelNameBushDoor );
			this->groupBushInfo->Controls->Add( this->labelNameBushConnect );
			this->groupBushInfo->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->groupBushInfo->Location = System::Drawing::Point( 12, 50 );
			this->groupBushInfo->Name = L"groupBushInfo";
			this->groupBushInfo->Size = System::Drawing::Size( 410, 111 );
			this->groupBushInfo->TabIndex = 7;
			this->groupBushInfo->TabStop = false;
			this->groupBushInfo->Text = L"Состояние устройста:";
			// 
			// labelBushRelay
			// 
			this->labelBushRelay->BackColor = System::Drawing::SystemColors::Info;
			this->labelBushRelay->Location = System::Drawing::Point( 315, 78 );
			this->labelBushRelay->Name = L"labelBushRelay";
			this->labelBushRelay->Size = System::Drawing::Size( 85, 18 );
			this->labelBushRelay->TabIndex = 10;
			// 
			// labelBushSens
			// 
			this->labelBushSens->BackColor = System::Drawing::SystemColors::Info;
			this->labelBushSens->Location = System::Drawing::Point( 315, 51 );
			this->labelBushSens->Name = L"labelBushSens";
			this->labelBushSens->Size = System::Drawing::Size( 85, 18 );
			this->labelBushSens->TabIndex = 9;
			// 
			// labelBushLock
			// 
			this->labelBushLock->BackColor = System::Drawing::SystemColors::Info;
			this->labelBushLock->Location = System::Drawing::Point( 113, 79 );
			this->labelBushLock->Name = L"labelBushLock";
			this->labelBushLock->Size = System::Drawing::Size( 85, 18 );
			this->labelBushLock->TabIndex = 8;
			// 
			// labelBushDoor
			// 
			this->labelBushDoor->BackColor = System::Drawing::SystemColors::Info;
			this->labelBushDoor->Location = System::Drawing::Point( 113, 51 );
			this->labelBushDoor->Name = L"labelBushDoor";
			this->labelBushDoor->Size = System::Drawing::Size( 85, 18 );
			this->labelBushDoor->TabIndex = 7;
			// 
			// labelBushConnect
			// 
			this->labelBushConnect->BackColor = System::Drawing::SystemColors::Info;
			this->labelBushConnect->Location = System::Drawing::Point( 113, 24 );
			this->labelBushConnect->Name = L"labelBushConnect";
			this->labelBushConnect->Size = System::Drawing::Size( 287, 18 );
			this->labelBushConnect->TabIndex = 6;
			// 
			// labelNameBushRelay
			// 
			this->labelNameBushRelay->AutoSize = true;
			this->labelNameBushRelay->Location = System::Drawing::Point( 216, 78 );
			this->labelNameBushRelay->Name = L"labelNameBushRelay";
			this->labelNameBushRelay->Size = System::Drawing::Size( 47, 18 );
			this->labelNameBushRelay->TabIndex = 4;
			this->labelNameBushRelay->Text = L"Реле:";
			// 
			// labelNameBushSens
			// 
			this->labelNameBushSens->AutoSize = true;
			this->labelNameBushSens->Location = System::Drawing::Point( 216, 51 );
			this->labelNameBushSens->Name = L"labelNameBushSens";
			this->labelNameBushSens->Size = System::Drawing::Size( 102, 18 );
			this->labelNameBushSens->TabIndex = 3;
			this->labelNameBushSens->Text = L"Температура:";
			// 
			// labelNameBushLock
			// 
			this->labelNameBushLock->AutoSize = true;
			this->labelNameBushLock->Location = System::Drawing::Point( 7, 79 );
			this->labelNameBushLock->Name = L"labelNameBushLock";
			this->labelNameBushLock->Size = System::Drawing::Size( 58, 18 );
			this->labelNameBushLock->TabIndex = 2;
			this->labelNameBushLock->Text = L"Замок:";
			// 
			// labelNameBushDoor
			// 
			this->labelNameBushDoor->AutoSize = true;
			this->labelNameBushDoor->Location = System::Drawing::Point( 7, 51 );
			this->labelNameBushDoor->Name = L"labelNameBushDoor";
			this->labelNameBushDoor->Size = System::Drawing::Size( 56, 18 );
			this->labelNameBushDoor->TabIndex = 1;
			this->labelNameBushDoor->Text = L"Дверь:";
			// 
			// labelNameBushConnect
			// 
			this->labelNameBushConnect->AutoSize = true;
			this->labelNameBushConnect->Location = System::Drawing::Point( 7, 24 );
			this->labelNameBushConnect->Name = L"labelNameBushConnect";
			this->labelNameBushConnect->Size = System::Drawing::Size( 88, 18 );
			this->labelNameBushConnect->TabIndex = 0;
			this->labelNameBushConnect->Text = L"Состояние:";
			// 
			// timerCheckData
			// 
			this->timerCheckData->Interval = 500;
			this->timerCheckData->Tick += gcnew System::EventHandler( this, &MyForm::timerCheckData_Tick );
			// 
			// groupBoxTrayConfig
			// 
			this->groupBoxTrayConfig->Controls->Add( this->chBoxLockDoor );
			this->groupBoxTrayConfig->Controls->Add( this->chBoxOverheat );
			this->groupBoxTrayConfig->Location = System::Drawing::Point( 12, 180 );
			this->groupBoxTrayConfig->Name = L"groupBoxTrayConfig";
			this->groupBoxTrayConfig->Size = System::Drawing::Size( 410, 81 );
			this->groupBoxTrayConfig->TabIndex = 8;
			this->groupBoxTrayConfig->TabStop = false;
			this->groupBoxTrayConfig->Text = L"Настройка визуальной индикации:";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF( 9, 18 );
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size( 434, 273 );
			this->Controls->Add( this->groupBoxTrayConfig );
			this->Controls->Add( this->groupBushInfo );
			this->Controls->Add( this->comBoxPortNames );
			this->Controls->Add( this->labelPort );
			this->Cursor = System::Windows::Forms::Cursors::Default;
			this->Font = ( gcnew System::Drawing::Font( L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
														static_cast< System::Byte >( 204 ) ) );
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Margin = System::Windows::Forms::Padding( 4, 3, 4, 3 );
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MyForm";
			this->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->ShowInTaskbar = false;
			this->Text = L"Настройка";
			this->WindowState = System::Windows::Forms::FormWindowState::Minimized;
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler( this, &MyForm::MyForm_FormClosing );
			this->trayMenu->ResumeLayout( false );
			this->groupBushInfo->ResumeLayout( false );
			this->groupBushInfo->PerformLayout();
			this->groupBoxTrayConfig->ResumeLayout( false );
			this->groupBoxTrayConfig->PerformLayout();
			this->ResumeLayout( false );
			this->PerformLayout();

		}
#pragma endregion
	private:
		short fnOnStart();

		short fnGetUserSettings( String^ pPathLocalData );
		short fnSetUserSettings();

		short fnReconnectToPort( String^ pPortName );
		short fnStartBushIOThread( String^ pPortName );
		short fnCloseOldBushIOThread();

		short fnInfoLabelsReset();
		short fnLockUnlockDoor();
		short fnReNewComPorts();
		short fnFormGuiEnable( bool isTRUE );
		short fnStatusLabelUpdate( Int16 bushStatus );
		short fnTrayMenuUpdate( Int16 bushStatus, Boolean bIsLockLocked );
		short fnTrayIconUpdate( Int16 bushStatus );
		short fnFormAppear();
		short fnOnTimerUpdate();

		short fnComBoxSelectionChange();
		short fnComBoxSelectionCheck();

		Void fnStaticMemberInit() {
			m_pIcoDisconnect = Form::Icon->ExtractAssociatedIcon( ".//Resource//BushDisconnected.ico" );
			m_pIcoOpen = Form::Icon->ExtractAssociatedIcon( ".//Resource//BushOpened.ico" );
			m_pIcoClose = Form::Icon->ExtractAssociatedIcon( ".//Resource//BushClosed.ico" );
			m_pIcoLock = Form::Icon->ExtractAssociatedIcon( ".//Resource//BushLocked.ico" );
			m_pIcoOverHeat = Form::Icon->ExtractAssociatedIcon( ".//Resource//BushOverHeat.ico" );
		}

		Void MyForm_FormClosing( System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e ) {
			if ( e->CloseReason == CloseReason::UserClosing )
			{
				e->Cancel = true;
				WindowState = FormWindowState::Minimized;
				Visible = false;
			}
			else
			{
				fnSetUserSettings();
				Diagnostics::Trace::TraceInformation( String::Format( "__________| Application finished: {0} |__________", DateTime::Now ) );
			}
			return;
		}

		Void toolStripMenuItem1_Click( System::Object^  sender, System::EventArgs^  e ) {
			Application::Exit();
			return;
		}
		Void toolStripMenuItem2_Click( System::Object^  sender, System::EventArgs^  e ) {
			fnFormAppear();
			return;
		}
		Void trayMenuItemDoor_Click( System::Object^  sender, System::EventArgs^  e ) {
			fnLockUnlockDoor();
			return;
		}
		Void trayIcon_MouseDoubleClick( System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e ) {
			fnFormAppear();
			return;
		}
		Void timerCheckData_Tick( System::Object^  sender, System::EventArgs^  e ) {
			fnOnTimerUpdate();
			return;
		}
		Void chBoxLockDoor_CheckedChanged( System::Object^  sender, System::EventArgs^  e ) {
			fnTrayIconUpdate( m_lastBushStatus );
			return;
		}
		Void comBoxPortNames_DropDown( System::Object^  sender, System::EventArgs^  e ) {
			fnReNewComPorts();
			return;
		}
		Void comBoxPortNames_SelectionChangeCommitted( System::Object^  sender, System::EventArgs^  e ) {
			fnComBoxSelectionCheck(); //second check because if change window before close drop down, dropdown event wont be called
			fnComBoxSelectionChange();
			return;
		}
		Void comBoxPortNames_DropDownClosed( System::Object^  sender, System::EventArgs^  e ) {
			fnComBoxSelectionCheck();
			return;
		}
	};
}
