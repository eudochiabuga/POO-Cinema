#pragma once
#include "AppContext.h"
#include "Exceptii.h"
#include <msclr/marshal_cppstd.h>

namespace CinemaApp {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class LoginForm : public System::Windows::Forms::Form {
    private:
        Label^   lblTitlu;
        Label^   lblUser;
        Label^   lblPass;
        Label^   lblMesaj;
        Label^   lblHint;
        TextBox^ txtUser;
        TextBox^ txtPass;
        Button^  btnLogin;
        Button^  btnExit;
        Panel^   panelHeader;

    public:
        LoginForm() {
            InitializeControls();
        }

    private:
        void InitializeControls() {
            this->Text = L"CINEMA - Autentificare";
            this->Size = System::Drawing::Size(440, 380);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->BackColor = Color::FromArgb(245, 245, 250);
            this->Font = gcnew Drawing::Font(L"Segoe UI", 9);

            // Header colorat
            panelHeader = gcnew Panel();
            panelHeader->BackColor = Color::FromArgb(30, 30, 60);
            panelHeader->Dock = DockStyle::Top;
            panelHeader->Height = 70;
            this->Controls->Add(panelHeader);

            lblTitlu = gcnew Label();
            lblTitlu->Text = L"CINEMA";
            lblTitlu->Font = gcnew Drawing::Font(L"Segoe UI", 16, FontStyle::Bold);
            lblTitlu->ForeColor = Color::White;
            lblTitlu->AutoSize = false;
            lblTitlu->TextAlign = ContentAlignment::MiddleCenter;
            lblTitlu->Dock = DockStyle::Fill;
            panelHeader->Controls->Add(lblTitlu);

            // Etichete si campuri
            lblUser = gcnew Label();
            lblUser->Text = L"Utilizator:";
            lblUser->Location = System::Drawing::Point(40, 100);
            lblUser->Size = System::Drawing::Size(100, 22);
            this->Controls->Add(lblUser);

            txtUser = gcnew TextBox();
            txtUser->Location = System::Drawing::Point(150, 98);
            txtUser->Size = System::Drawing::Size(220, 26);
            this->Controls->Add(txtUser);

            lblPass = gcnew Label();
            lblPass->Text = L"Parola:";
            lblPass->Location = System::Drawing::Point(40, 138);
            lblPass->Size = System::Drawing::Size(100, 22);
            this->Controls->Add(lblPass);

            txtPass = gcnew TextBox();
            txtPass->Location = System::Drawing::Point(150, 136);
            txtPass->Size = System::Drawing::Size(220, 26);
            txtPass->UseSystemPasswordChar = true;
            this->Controls->Add(txtPass);

            lblMesaj = gcnew Label();
            lblMesaj->Location = System::Drawing::Point(40, 175);
            lblMesaj->Size = System::Drawing::Size(330, 22);
            lblMesaj->ForeColor = Color::Red;
            lblMesaj->TextAlign = ContentAlignment::MiddleCenter;
            this->Controls->Add(lblMesaj);

            btnLogin = gcnew Button();
            btnLogin->Text = L"Autentificare";
            btnLogin->Location = System::Drawing::Point(150, 210);
            btnLogin->Size = System::Drawing::Size(110, 36);
            btnLogin->BackColor = Color::FromArgb(60, 110, 200);
            btnLogin->ForeColor = Color::White;
            btnLogin->FlatStyle = FlatStyle::Flat;
            btnLogin->FlatAppearance->BorderSize = 0;
            btnLogin->Click += gcnew EventHandler(this, &LoginForm::OnLogin);
            this->Controls->Add(btnLogin);

            btnExit = gcnew Button();
            btnExit->Text = L"Iesire";
            btnExit->Location = System::Drawing::Point(270, 210);
            btnExit->Size = System::Drawing::Size(100, 36);
            btnExit->BackColor = Color::FromArgb(160, 160, 160);
            btnExit->ForeColor = Color::White;
            btnExit->FlatStyle = FlatStyle::Flat;
            btnExit->FlatAppearance->BorderSize = 0;
            btnExit->Click += gcnew EventHandler(this, &LoginForm::OnExit);
            this->Controls->Add(btnExit);

            lblHint = gcnew Label();
            lblHint->Text = L"Utilizatori impliciti:  admin/admin  -  manager/manager  -  casier/casier";
            lblHint->Location = System::Drawing::Point(20, 270);
            lblHint->Size = System::Drawing::Size(400, 40);
            lblHint->ForeColor = Color::Gray;
            lblHint->TextAlign = ContentAlignment::MiddleCenter;
            this->Controls->Add(lblHint);

            this->AcceptButton = btnLogin;
            this->CancelButton = btnExit;
        }

        void OnLogin(Object^ sender, EventArgs^ e) {
            try {
                std::string u = marshal_as<std::string>(txtUser->Text);
                std::string p = marshal_as<std::string>(txtPass->Text);

                if (u.empty() || p.empty()) {
                    lblMesaj->Text = L"Introduceti utilizator si parola.";
                    return;
                }

                CinemaContext::auth->Login(u, p);
                this->DialogResult = System::Windows::Forms::DialogResult::OK;
                this->Close();
            }
            catch (CinemaException& ex) {
                lblMesaj->Text = marshal_as<String^>(ex.GetMesaj());
                txtPass->Clear();
            }
            catch (std::exception& ex) {
                lblMesaj->Text = marshal_as<String^>(std::string(ex.what()));
            }
        }

        void OnExit(Object^ sender, EventArgs^ e) {
            this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Close();
        }
    };
}
