#pragma once
#include "AppContext.h"
#include "Exceptii.h"
#include <msclr/marshal_cppstd.h>

namespace CinemaApp {

    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class DialogSpectacol : public Form {
    private:
        ComboBox^ cbFilm;
        ComboBox^ cbSala;
        DateTimePicker^ dtpData;
        DateTimePicker^ dtpOra;
        NumericUpDown^ numPret;
        Button^ btnOK;
        Button^ btnCancel;

    public:
        DialogSpectacol() { InitializeControls(); PopuleazaCombo(); }

    private:
        void InitializeControls() {
            this->Text = L"Adauga spectacol";
            this->Size = System::Drawing::Size(460, 360);
            this->StartPosition = FormStartPosition::CenterParent;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Font = gcnew Drawing::Font(L"Segoe UI", 9);

            int y = 25, gap = 42;

            AdaugaLabel(L"Film:", 25, y);
            cbFilm = gcnew ComboBox();
            cbFilm->Location = Point(150, y - 2);
            cbFilm->Size = System::Drawing::Size(260, 26);
            cbFilm->DropDownStyle = ComboBoxStyle::DropDownList;
            this->Controls->Add(cbFilm);
            y += gap;

            AdaugaLabel(L"Sala:", 25, y);
            cbSala = gcnew ComboBox();
            cbSala->Location = Point(150, y - 2);
            cbSala->Size = System::Drawing::Size(260, 26);
            cbSala->DropDownStyle = ComboBoxStyle::DropDownList;
            this->Controls->Add(cbSala);
            y += gap;

            AdaugaLabel(L"Data:", 25, y);
            dtpData = gcnew DateTimePicker();
            dtpData->Location = Point(150, y - 2);
            dtpData->Size = System::Drawing::Size(260, 26);
            dtpData->Format = DateTimePickerFormat::Custom;
            dtpData->CustomFormat = L"yyyy-MM-dd";
            this->Controls->Add(dtpData);
            y += gap;

            AdaugaLabel(L"Ora:", 25, y);
            dtpOra = gcnew DateTimePicker();
            dtpOra->Location = Point(150, y - 2);
            dtpOra->Size = System::Drawing::Size(260, 26);
            dtpOra->Format = DateTimePickerFormat::Custom;
            dtpOra->CustomFormat = L"HH:mm";
            dtpOra->ShowUpDown = true;
            this->Controls->Add(dtpOra);
            y += gap;

            AdaugaLabel(L"Pret (RON):", 25, y);
            numPret = gcnew NumericUpDown();
            numPret->Location = Point(150, y - 2);
            numPret->Size = System::Drawing::Size(120, 26);
            numPret->DecimalPlaces = 2;
            numPret->Minimum = 0; numPret->Maximum = 500;
            numPret->Value = (Decimal)CinemaContext::cinema->GetPretBaza();
            this->Controls->Add(numPret);
            y += gap + 10;

            btnOK = gcnew Button();
            btnOK->Text = L"Adauga";
            btnOK->Location = Point(150, y);
            btnOK->Size = System::Drawing::Size(120, 34);
            btnOK->BackColor = Color::FromArgb(60, 130, 80);
            btnOK->ForeColor = Color::White;
            btnOK->FlatStyle = FlatStyle::Flat;
            btnOK->Click += gcnew EventHandler(this, &DialogSpectacol::OnOK);
            this->Controls->Add(btnOK);

            btnCancel = gcnew Button();
            btnCancel->Text = L"Anuleaza";
            btnCancel->Location = Point(285, y);
            btnCancel->Size = System::Drawing::Size(120, 34);
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

        void PopuleazaCombo() {
            cbFilm->Items->Clear();
            for (auto f : CinemaContext::cinema->GetFilme()) {
                cbFilm->Items->Add(marshal_as<String^>(f->ToString()));
            }
            if (cbFilm->Items->Count > 0) cbFilm->SelectedIndex = 0;

            cbSala->Items->Clear();
            for (auto s : CinemaContext::cinema->GetSali()) {
                cbSala->Items->Add(marshal_as<String^>(s->Descriere()));
            }
            if (cbSala->Items->Count > 0) cbSala->SelectedIndex = 0;
        }

        void OnOK(Object^ sender, EventArgs^ e) {
            try {
                if (cbFilm->SelectedIndex < 0 || cbSala->SelectedIndex < 0) {
                    MessageBox::Show(L"Selectati film si sala.", L"Eroare");
                    return;
                }
                Film* f = CinemaContext::cinema->GetFilme()[cbFilm->SelectedIndex];
                Sala* s = CinemaContext::cinema->GetSali()[cbSala->SelectedIndex];

                std::string data = marshal_as<std::string>(dtpData->Value.ToString(L"yyyy-MM-dd"));
                std::string ora = marshal_as<std::string>(dtpOra->Value.ToString(L"HH:mm"));
                double pret = (double)numPret->Value;

                CinemaContext::cinema->CreeazaSpectacol(f, s, data, ora, pret);

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
