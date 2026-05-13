#pragma once
#include "AppContext.h"
#include "TipAngajat.h"
#include "Exceptii.h"
#include <msclr/marshal_cppstd.h>

namespace CinemaApp {

    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class DialogAngajat : public Form {
    private:
        TextBox^ txtNume;
        TextBox^ txtUsername;
        TextBox^ txtParola;
        NumericUpDown^ numSalariu;
        ComboBox^ cbRol;
        Button^ btnOK;
        Button^ btnCancel;

    public:
        DialogAngajat() { InitializeControls(); }

    private:
        void InitializeControls() {
            this->Text = L"Adauga angajat";
            this->Size = System::Drawing::Size(440, 380);
            this->StartPosition = FormStartPosition::CenterParent;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->Font = gcnew Drawing::Font(L"Segoe UI", 9);

            int y = 25, gap = 42;

            AdaugaLabel(L"Nume complet:", 25, y);
            txtNume = gcnew TextBox();
            txtNume->Location = Point(150, y - 2);
            txtNume->Size = System::Drawing::Size(240, 26);
            this->Controls->Add(txtNume);
            y += gap;

            AdaugaLabel(L"Username:", 25, y);
            txtUsername = gcnew TextBox();
            txtUsername->Location = Point(150, y - 2);
            txtUsername->Size = System::Drawing::Size(240, 26);
            this->Controls->Add(txtUsername);
            y += gap;

            AdaugaLabel(L"Parola:", 25, y);
            txtParola = gcnew TextBox();
            txtParola->Location = Point(150, y - 2);
            txtParola->Size = System::Drawing::Size(240, 26);
            this->Controls->Add(txtParola);
            y += gap;

            AdaugaLabel(L"Rol:", 25, y);
            cbRol = gcnew ComboBox();
            cbRol->Location = Point(150, y - 2);
            cbRol->Size = System::Drawing::Size(240, 26);
            cbRol->DropDownStyle = ComboBoxStyle::DropDownList;
            cbRol->Items->Add(L"Casier");
            cbRol->Items->Add(L"Manager");
            cbRol->Items->Add(L"Administrator");
            cbRol->SelectedIndex = 0;
            this->Controls->Add(cbRol);
            y += gap;

            AdaugaLabel(L"Salariu (RON):", 25, y);
            numSalariu = gcnew NumericUpDown();
            numSalariu->Location = Point(150, y - 2);
            numSalariu->Size = System::Drawing::Size(120, 26);
            numSalariu->Minimum = 0; numSalariu->Maximum = 50000;
            numSalariu->Value = 4000;
            this->Controls->Add(numSalariu);
            y += gap + 15;

            btnOK = gcnew Button();
            btnOK->Text = L"Adauga";
            btnOK->Location = Point(150, y);
            btnOK->Size = System::Drawing::Size(110, 34);
            btnOK->BackColor = Color::FromArgb(60, 130, 80);
            btnOK->ForeColor = Color::White;
            btnOK->FlatStyle = FlatStyle::Flat;
            btnOK->Click += gcnew EventHandler(this, &DialogAngajat::OnOK);
            this->Controls->Add(btnOK);

            btnCancel = gcnew Button();
            btnCancel->Text = L"Anuleaza";
            btnCancel->Location = Point(275, y);
            btnCancel->Size = System::Drawing::Size(115, 34);
            btnCancel->BackColor = Color::FromArgb(160, 160, 160);
            btnCancel->ForeColor = Color::White;
            btnCancel->FlatStyle = FlatStyle::Flat;
            btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Controls->Add(btnCancel);

            this->AcceptButton = btnOK;
            this->CancelButton = btnCancel;
        }

        void AdaugaLabel(String^ text, int x, int y) {
            Label^ l = gcnew Label();
            l->Text = text;
            l->Location = Point(x, y);
            l->Size = System::Drawing::Size(120, 22);
            this->Controls->Add(l);
        }

        void OnOK(Object^ sender, EventArgs^ e) {
            try {
                std::string nume = marshal_as<std::string>(txtNume->Text->Trim());
                std::string user = marshal_as<std::string>(txtUsername->Text->Trim());
                std::string par = marshal_as<std::string>(txtParola->Text);
                double sal = (double)numSalariu->Value;

                if (nume.empty() || user.empty() || par.empty()) {
                    MessageBox::Show(L"Completati toate campurile.", L"Eroare");
                    return;
                }

                Angajat* a = nullptr;
                switch (cbRol->SelectedIndex) {
                case 0: a = new Casier(nume, user, par, sal); break;
                case 1: a = new Manager(nume, user, par, sal); break;
                case 2: a = new Administrator(nume, user, par, sal); break;
                default: a = new Casier(nume, user, par, sal); break;
                }
                CinemaContext::auth->AdaugaAngajat(a);

                this->DialogResult = System::Windows::Forms::DialogResult::OK;
                this->Close();
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
            catch (std::exception& ex) {
                MessageBox::Show(marshal_as<String^>(std::string(ex.what())), L"Eroare");
            }
        }
    };
}
