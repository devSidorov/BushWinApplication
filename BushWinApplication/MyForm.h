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
			this->comboBox1->Items->Add( "COM1" );
			this->comboBox1->Items->Add( "COM2" );
			this->comboBox1->Items->Add( "COM3" );			
			//TODO add search for every serial port available


			String^ IcoName = "..//resource//ProgressWarn.ico";
			this->notifyIcon1->Icon = Icon->ExtractAssociatedIcon( "..//resource//ProgressWarn.ico" );
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

	protected:
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::NotifyIcon^  notifyIcon1;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent( void )
		{
			this->components = ( gcnew System::ComponentModel::Container() );
			this->label1 = ( gcnew System::Windows::Forms::Label() );
			this->checkBox1 = ( gcnew System::Windows::Forms::CheckBox() );
			this->checkBox2 = ( gcnew System::Windows::Forms::CheckBox() );
			this->comboBox1 = ( gcnew System::Windows::Forms::ComboBox() );
			this->notifyIcon1 = ( gcnew System::Windows::Forms::NotifyIcon( this->components ) );
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point( 18, 21 );
			this->label1->Margin = System::Windows::Forms::Padding( 4, 0, 4, 0 );
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size( 47, 18 );
			this->label1->TabIndex = 1;
			this->label1->Text = L"Порт:";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point( 22, 54 );
			this->checkBox1->Margin = System::Windows::Forms::Padding( 4, 3, 4, 3 );
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size( 323, 22 );
			this->checkBox1->TabIndex = 2;
			this->checkBox1->Text = L"Визуальная сигнализация открытия двери";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point( 22, 87 );
			this->checkBox2->Margin = System::Windows::Forms::Padding( 4, 3, 4, 3 );
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size( 393, 22 );
			this->checkBox2->TabIndex = 3;
			this->checkBox2->Text = L"Визуальная сигнализация превышения температуры";
			this->checkBox2->UseVisualStyleBackColor = true;
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point( 73, 18 );
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size( 288, 26 );
			this->comboBox1->TabIndex = 4;
			this->comboBox1->Text = L"Выберите порт из списка";
			// 
			// notifyIcon1
			// 
			this->notifyIcon1->Text = L"notifyIcon1";
			this->notifyIcon1->Visible = true;
			this->notifyIcon1->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler( this, &MyForm::notifyIcon1_MouseDoubleClick );
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF( 9, 18 );
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size( 434, 172 );
			this->Controls->Add( this->comboBox1 );
			this->Controls->Add( this->checkBox2 );
			this->Controls->Add( this->checkBox1 );
			this->Controls->Add( this->label1 );
			this->Cursor = System::Windows::Forms::Cursors::Default;
			this->Font = ( gcnew System::Drawing::Font( L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
														static_cast< System::Byte >( 204 ) ) );
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Margin = System::Windows::Forms::Padding( 4, 3, 4, 3 );
			this->Name = L"MyForm";
			this->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->ShowInTaskbar = false;
			this->Text = L"Настройка";
			this->WindowState = System::Windows::Forms::FormWindowState::Minimized;
			this->Load += gcnew System::EventHandler( this, &MyForm::MyForm_Load );
			this->ResumeLayout( false );
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load( System::Object^  sender, System::EventArgs^  e ) {
	}
	;
	private: System::Void notifyIcon1_MouseDoubleClick( System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e ) {
		//TODO update form
		this->WindowState = FormWindowState::Normal;
		//this->Show();
		this->notifyIcon1->Icon = Icon->ExtractAssociatedIcon( "..//resource//ProgressSkip.ico" );
	}
}
	;
 }
