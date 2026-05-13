#pragma once
#include "AppContext.h"
#include "SalaVIP.h"
#include "Exceptii.h"
#include <msclr/marshal_cppstd.h>

namespace CinemaApp {

    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class DialogSala : public Form {
    private:
        TextBox^ txtNumar;
        NumericUpDown^ numRanduri;
        NumericUpDown^ numColoane;
        ComboBox^ cbTip;
        Panel^ panelVIP;
        NumericUpDown^ numSupliment;
        TextBox^ txtTipScaun;
        TextBox^ txtMeniu;
        Button^ btnOK;
        Button^ btnCancel;
        Label^ lblCapacitate;

    public:
        DialogSala() { InitializeControls(); ActualizeazaCapacitate(); ActualizeazaPanouVIP(); }

    private:
        void InitializeControls() {
            this->Text = L"Adauga sala";
            this->Size = System::Drawing::Size(440, 510);
            this->StartPosition = FormStartPosition::CenterParent;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Font = gcnew Drawing::Font(L"Segoe UI", 9);

            int y = 25;
            int gap = 38;

            AdaugaLabel(L"Numar sala:", 25, y);
            txtNumar = gcnew TextBox();
            txtNumar->Location = Point(150, y - 2);
            txtNumar->Size = System::Drawing::Size(220, 26);
            this->Controls->Add(txtNumar);
            y += gap;

            AdaugaLabel(L"Randuri:", 25, y);
            numRanduri = gcnew NumericUpDown();
            numRanduri->Location = Point(150, y - 2);
            numRanduri->Size = System::Drawing::Size(100, 26);
            numRanduri->Minimum = 1; numRanduri->Maximum = 30;
            numRanduri->Value = 6;
            numRanduri->ValueChanged += gcnew EventHandler(this, &DialogSala::OnDimChange);
            this->Controls->Add(numRanduri);
            y += gap;

            AdaugaLabel(L"Coloane:", 25, y);
            numColoane = gcnew NumericUpDown();
            numColoane->Location = Point(150, y - 2);
            numColoane->Size = System::Drawing::Size(100, 26);
            numColoane->Minimum = 1; numColoane->Maximum = 30;
            numColoane->Value = 8;
            numColoane->ValueChanged += gcnew EventHandler(this, &DialogSala::OnDimChange);
            this->Controls->Add(numColoane);
            y += gap;

            lblCapacitate = gcnew Label();
            lblCapacitate->Location = Point(150, y);
            lblCapacitate->Size = System::Drawing::Size(220, 22);
            lblCapacitate->ForeColor = Color::DarkSlateBlue;
            this->Controls->Add(lblCapacitate);
            y += gap;

            AdaugaLabel(L"Tip sala:", 25, y);
            cbTip = gcnew ComboBox();
            cbTip->Location = Point(150, y - 2);
            cbTip->Size = System::Drawing::Size(220, 26);
            cbTip->DropDownStyle = ComboBoxStyle::DropDownList;
            cbTip->Items->Add(L"Normala");
            cbTip->Items->Add(L"VIP");
            cbTip->SelectedIndex = 0;
            cbTip->SelectedIndexChanged += gcnew EventHandler(this, &DialogSala::OnTipChange);
            this->Controls->Add(cbTip);
            y += gap;

            panelVIP = gcnew Panel();
            panelVIP->Location = Point(25, y);
            panelVIP->Size = System::Drawing::Size(380, 180);
            panelVIP->BackColor = Color::FromArgb(245, 240, 230);
            panelVIP->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->Controls->Add(panelVIP);

            Label^ titluVIP = gcnew Label();
            titluVIP->Text = L"Optiuni VIP";
            titluVIP->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
            titluVIP->Location = Point(10, 8);
            titluVIP->Size = System::Drawing::Size(180, 20);
            panelVIP->Controls->Add(titluVIP);

            Label^ lblSup = gcnew Label();
            lblSup->Text = L"Supliment (%):";
            lblSup->Location = Point(10, 36);
            lblSup->Size = System::Drawing::Size(120, 22);
            panelVIP->Controls->Add(lblSup);

            numSupliment = gcnew NumericUpDown();
            numSupliment->Location = Point(140, 34);
            numSupliment->Size = System::Drawing::Size(80, 26);
            numSupliment->Minimum = 0; numSupliment->Maximum = 500;
            numSupliment->Value = 50;
            panelVIP->Controls->Add(numSupliment);

            Label^ lblScaun = gcnew Label();
            lblScaun->Text = L"Tip scaun:";
            lblScaun->Location = Point(10, 68);
            lblScaun->Size = System::Drawing::Size(120, 22);
            panelVIP->Controls->Add(lblScaun);

            txtTipScaun = gcnew TextBox();
            txtTipScaun->Location = Point(140, 66);
            txtTipScaun->Size = System::Drawing::Size(220, 26);
            txtTipScaun->Text = L"Recliner";
            panelVIP->Controls->Add(txtTipScaun);

            Label^ lblMeniu = gcnew Label();
            lblMeniu->Text = L"Meniu (separat prin ; ):";
            lblMeniu->Location = Point(10, 100);
            lblMeniu->Size = System::Drawing::Size(180, 22);
            panelVIP->Controls->Add(lblMeniu);

            txtMeniu = gcnew TextBox();
            txtMeniu->Location = Point(10, 124);
            txtMeniu->Size = System::Drawing::Size(355, 26);
            txtMeniu->Text = L"Popcorn premium;Cocktail fara alcool;Platou de branza";
            panelVIP->Controls->Add(txtMeniu);

            y += 190;

            btnOK = gcnew Button();
            btnOK->Text = L"Adauga";
            btnOK->Location = Point(150, y);
            btnOK->Size = System::Drawing::Size(110, 34);
            btnOK->BackColor = Color::FromArgb(60, 130, 80);
            btnOK->ForeColor = Color::White;
            btnOK->FlatStyle = FlatStyle::Flat;
            btnOK->Click += gcnew EventHandler(this, &DialogSala::OnOK);
            this->Controls->Add(btnOK);

            btnCancel = gcnew Button();
            btnCancel->Text = L"Anuleaza";
            btnCancel->Location = Point(275, y);
            btnCancel->Size = System::Drawing::Size(110, 34);
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

        void OnDimChange(Object^ s, EventArgs^ e) { ActualizeazaCapacitate(); }
        void OnTipChange(Object^ s, EventArgs^ e) { ActualizeazaPanouVIP(); }

        void ActualizeazaCapacitate() {
            int cap = (int)numRanduri->Value * (int)numColoane->Value;
            lblCapacitate->Text = String::Format(L"Capacitate: {0} locuri", cap);
        }

        void ActualizeazaPanouVIP() {
            panelVIP->Enabled = (cbTip->SelectedIndex == 1);
            panelVIP->Visible = (cbTip->SelectedIndex == 1);
        }

        void OnOK(Object^ sender, EventArgs^ e) {
            try {
                std::string numar = marshal_as<std::string>(txtNumar->Text->Trim());
                if (numar.empty()) {
                    MessageBox::Show(L"Introduceti numarul salii.", L"Eroare");
                    return;
                }
                if (CinemaContext::cinema->GasesteSala(numar)) {
                    MessageBox::Show(L"Exista deja o sala cu acest numar.", L"Eroare");
                    return;
                }

                int r = (int)numRanduri->Value;
                int c = (int)numColoane->Value;
                int cap = r * c;

                if (cbTip->SelectedIndex == 0) {
                    Sala* s = new Sala(numar, cap, r, c);
                    CinemaContext::cinema->AdaugaSala(s);
                }
                else {
                    double sup = (double)numSupliment->Value / 100.0;
                    std::string tipScaun = marshal_as<std::string>(txtTipScaun->Text->Trim());
                    if (tipScaun.empty()) tipScaun = "Recliner";
                    SalaVIP* sv = new SalaVIP(numar, cap, r, c, sup, tipScaun);

                    std::string meniu = marshal_as<std::string>(txtMeniu->Text);
                    size_t start = 0;
                    while (start < meniu.size()) {
                        size_t pos = meniu.find(';', start);
                        std::string item = (pos == std::string::npos)
                            ? meniu.substr(start)
                            : meniu.substr(start, pos - start);
                        // trim
                        size_t a = item.find_first_not_of(" \t\r\n");
                        size_t b = item.find_last_not_of(" \t\r\n");
                        if (a != std::string::npos)
                            sv->AdaugaProdusMeniu(item.substr(a, b - a + 1));
                        if (pos == std::string::npos) break;
                        start = pos + 1;
                    }
                    CinemaContext::cinema->AdaugaSala(sv);
                }

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
