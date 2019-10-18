#pragma once

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
			//
			//TODO: Add the constructor code here
			//
			
			//TODO add variant if no com port found
			ReNew_ComPorts();
			StatusLabelUpdate( 0 );


			String^ IcoName = "..//resource//ProgressWarn.ico";
			this->trayNotification->Icon = Form::Icon->ExtractAssociatedIcon( "..//resource//ProgressWarn.ico" );
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
	private: System::Windows::Forms::Label^  labelPort;
	protected:

	protected:

	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::ComboBox^  comBoxPortNames;
	private: System::Windows::Forms::NotifyIcon^  trayNotification;



	private: System::Windows::Forms::ContextMenuStrip^  trayMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  trayMenuItemDoor;




	private: System::Windows::Forms::ToolStripMenuItem^  trayMenuItemSettings;

	private: System::Windows::Forms::ToolStripMenuItem^  trayMenuItemExit;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::GroupBox^  groupBushInfo;

	private: System::Windows::Forms::Label^  labelNameBushRelay;
	private: System::Windows::Forms::Label^  labelNameBushSens;
	private: System::Windows::Forms::Label^  labelNameBushLock;
	private: System::Windows::Forms::Label^  labelNameBushDoor;
	private: System::Windows::Forms::Label^  labelNameBushConnect;
	private: System::Windows::Forms::Label^  labelBushConnect;
	private: System::Windows::Forms::Label^  labelBushDoor;


	private: System::Windows::Forms::Label^  labelBushRelay;
	private: System::Windows::Forms::Label^  labelBushSens;
	private: System::Windows::Forms::Label^  labelBushLock;
	private: System::Windows::Forms::Timer^  timerCheckData;







	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.

		/// </summary>

		Int32 BushIOThreadStart( String^ pPortName );
		Void FormGuiEnable( bool isTRUE );

		Void InfoLabelsReset();

		Void StatusLabelUpdate( const Int32& bushStatus );


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent( void )
		{
			this->components = ( gcnew System::ComponentModel::Container() );
			this->labelPort = ( gcnew System::Windows::Forms::Label() );
			this->checkBox1 = ( gcnew System::Windows::Forms::CheckBox() );
			this->checkBox2 = ( gcnew System::Windows::Forms::CheckBox() );
			this->comBoxPortNames = ( gcnew System::Windows::Forms::ComboBox() );
			this->trayNotification = ( gcnew System::Windows::Forms::NotifyIcon( this->components ) );
			this->trayMenu = ( gcnew System::Windows::Forms::ContextMenuStrip( this->components ) );
			this->trayMenuItemDoor = ( gcnew System::Windows::Forms::ToolStripMenuItem() );
			this->trayMenuItemSettings = ( gcnew System::Windows::Forms::ToolStripMenuItem() );
			this->trayMenuItemExit = ( gcnew System::Windows::Forms::ToolStripMenuItem() );
			this->label2 = ( gcnew System::Windows::Forms::Label() );
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
			this->trayMenu->SuspendLayout();
			this->groupBushInfo->SuspendLayout();
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
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point( 22, 54 );
			this->checkBox1->Margin = System::Windows::Forms::Padding( 4, 3, 4, 3 );
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size( 238, 22 );
			this->checkBox1->TabIndex = 2;
			this->checkBox1->Text = L"Нотификация открытия двери";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point( 22, 87 );
			this->checkBox2->Margin = System::Windows::Forms::Padding( 4, 3, 4, 3 );
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size( 308, 22 );
			this->checkBox2->TabIndex = 3;
			this->checkBox2->Text = L"Нотификация превышения температуры";
			this->checkBox2->UseVisualStyleBackColor = true;
			// 
			// comBoxPortNames
			// 
			this->comBoxPortNames->FormattingEnabled = true;
			this->comBoxPortNames->Location = System::Drawing::Point( 73, 18 );
			this->comBoxPortNames->Name = L"comBoxPortNames";
			this->comBoxPortNames->Size = System::Drawing::Size( 288, 26 );
			this->comBoxPortNames->TabIndex = 4;
			this->comBoxPortNames->Text = L"Необходимо выбрать порт подключения";
			this->comBoxPortNames->SelectedIndexChanged += gcnew System::EventHandler( this, &MyForm::comBoxPortNames_SelectedIndexChanged );
			// 
			// trayNotification
			// 
			this->trayNotification->ContextMenuStrip = this->trayMenu;
			this->trayNotification->Text = L"notifyIcon1";
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
			this->trayMenuItemDoor->Text = L"Дверь";
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
			this->trayMenuItemExit->Name = L"trayMenuItemExit";
			this->trayMenuItemExit->Size = System::Drawing::Size( 133, 22 );
			this->trayMenuItemExit->Text = L"Выход";
			this->trayMenuItemExit->Click += gcnew System::EventHandler( this, &MyForm::toolStripMenuItem1_Click );
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point( 21, 116 );
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size( 0, 18 );
			this->label2->TabIndex = 5;
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
			this->groupBushInfo->Location = System::Drawing::Point( 12, 137 );
			this->groupBushInfo->Name = L"groupBushInfo";
			this->groupBushInfo->Size = System::Drawing::Size( 410, 126 );
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
			this->labelNameBushConnect->Size = System::Drawing::Size( 109, 18 );
			this->labelNameBushConnect->TabIndex = 0;
			this->labelNameBushConnect->Text = L"Подключение:";
			// 
			// timerCheckData
			// 
			this->timerCheckData->Interval = 500;
			this->timerCheckData->Tick += gcnew System::EventHandler( this, &MyForm::timerCheckData_Tick );
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF( 9, 18 );
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size( 434, 353 );
			this->Controls->Add( this->groupBushInfo );
			this->Controls->Add( this->label2 );
			this->Controls->Add( this->comBoxPortNames );
			this->Controls->Add( this->checkBox2 );
			this->Controls->Add( this->checkBox1 );
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
			this->Load += gcnew System::EventHandler( this, &MyForm::MyForm_Load );
			this->trayMenu->ResumeLayout( false );
			this->groupBushInfo->ResumeLayout( false );
			this->groupBushInfo->PerformLayout();
			this->ResumeLayout( false );
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::Void MyForm_Load( System::Object^  sender, System::EventArgs^  e ) {
		}
		;

		System::Void trayIcon_MouseDoubleClick( System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e ) {
			//TODO update form
			//TODO Exclude right click
			this->WindowState = FormWindowState::Normal;
			//this->Show();
			this->trayNotification->Icon = Icon->ExtractAssociatedIcon( "..//resource//ProgressSkip.ico" );
		}

		System::Void MyForm_FormClosing( System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e ) {
			if ( e->CloseReason == CloseReason::UserClosing )
			{
				e->Cancel = true;
				this->WindowState = FormWindowState::Minimized;
			}

			return;
		}


		System::Void toolStripMenuItem1_Click( System::Object^  sender, System::EventArgs^  e ) {
			Application::Exit();
			return;
		}
		System::Void toolStripMenuItem2_Click( System::Object^  sender, System::EventArgs^  e ) {
			this->WindowState = FormWindowState::Normal;
			return;
		}
		System::Void timerCheckData_Tick( System::Object^  sender, System::EventArgs^  e ) {
			OnTimerUpdate();
		}

		System::Void ReNew_ComPorts() {
			array<String^>^ serialPorts = nullptr;

			serialPorts = IO::Ports::SerialPort::GetPortNames();
			if( serialPorts )
				for each( String^ port in serialPorts )
					this->comBoxPortNames->Items->Add( port );
		}
	
		System::Void comBoxPortNames_SelectedIndexChanged( System::Object^  sender, System::EventArgs^  e ) {
			//TODO add first start check and closing previous serial port thread
		
			FormGuiEnable( false );
			Int32 fSuccess = BushIOThreadStart( comBoxPortNames->SelectedItem->ToString() );
		}

		Int32 OnTimerUpdate();
};
	
 }
