#pragma once
#include "AppContext.h"
#include "Spectacol.h"
#include "TipPersoana.h"
#include "Exceptii.h"
#include <msclr/marshal_cppstd.h>

namespace CinemaApp {

    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class DialogRezervare : public Form {
    private:
        Spectacol* spectacol;

        TextBox^ txtNume;
        TextBox^ txtEmail;
        TextBox^ txtTelefon;
        NumericUpDown^ numVarsta;
        ComboBox^ cbTip;
        Label^ lblFilm;
        Label^ lblSala;
        Label^ lblDataOra;
        Label^ lblPret;
        Label^ lblLocSelectat;
        Label^ lblLegenda;
        Panel^ panelLocuri;
        Button^ btnOK;
        Button^ btnCancel;

        int randSelectat;
        int colSelectata;
        cli::array<Button^, 2>^ butoaneLocuri;

    public:
        DialogRezervare(Spectacol* sp) : spectacol(sp), randSelectat(-1), colSelectata(-1) {
            InitializeControls();
            ConstruiesteHarta();
        }

    private:
        void InitializeControls() {
            this->Text = L"Rezervare noua";
            this->Size = System::Drawing::Size(900, 640);
            this->StartPosition = FormStartPosition::CenterParent;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->Font = gcnew Drawing::Font(L"Segoe UI", 9);
            this->BackColor = Color::FromArgb(248, 248, 252);

            // === Stanga: detalii spectacol + formular client ===
            Panel^ panelStanga = gcnew Panel();
            panelStanga->Location = Point(15, 15);
            panelStanga->Size = System::Drawing::Size(360, 580);
            panelStanga->BackColor = Color::White;
            panelStanga->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->Controls->Add(panelStanga);

            Label^ titlu = gcnew Label();
            titlu->Text = L"Detalii spectacol";
            titlu->Font = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            titlu->Location = Point(15, 12);
            titlu->Size = System::Drawing::Size(330, 24);
            titlu->ForeColor = Color::FromArgb(30, 30, 60);
            panelStanga->Controls->Add(titlu);

            lblFilm = gcnew Label();
            lblFilm->Location = Point(15, 45);
            lblFilm->Size = System::Drawing::Size(330, 36);
            lblFilm->Text = String::Format(L"Film:  {0}",
                marshal_as<String^>(spectacol->GetFilm()->GetTitlu()));
            lblFilm->Font = gcnew Drawing::Font(L"Segoe UI", 10);
            panelStanga->Controls->Add(lblFilm);

            lblSala = gcnew Label();
            lblSala->Location = Point(15, 85);
            lblSala->Size = System::Drawing::Size(330, 22);
            lblSala->Text = String::Format(L"Sala:  {0}",
                marshal_as<String^>(spectacol->GetSala()->Descriere()));
            panelStanga->Controls->Add(lblSala);

            lblDataOra = gcnew Label();
            lblDataOra->Location = Point(15, 110);
            lblDataOra->Size = System::Drawing::Size(330, 22);
            lblDataOra->Text = String::Format(L"Data:  {0}  {1}",
                marshal_as<String^>(spectacol->GetData()),
                marshal_as<String^>(spectacol->GetOra()));
            panelStanga->Controls->Add(lblDataOra);

            lblPret = gcnew Label();
            lblPret->Location = Point(15, 135);
            lblPret->Size = System::Drawing::Size(330, 22);
            lblPret->Text = String::Format(L"Pret baza:  {0:F2} RON", spectacol->GetPretBaza());
            lblPret->ForeColor = Color::FromArgb(180, 70, 70);
            panelStanga->Controls->Add(lblPret);

            // Separator
            Label^ separator = gcnew Label();
            separator->Location = Point(15, 170);
            separator->Size = System::Drawing::Size(330, 2);
            separator->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            panelStanga->Controls->Add(separator);

            Label^ titluClient = gcnew Label();
            titluClient->Text = L"Date client";
            titluClient->Font = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            titluClient->Location = Point(15, 180);
            titluClient->Size = System::Drawing::Size(330, 24);
            titluClient->ForeColor = Color::FromArgb(30, 30, 60);
            panelStanga->Controls->Add(titluClient);

            AdaugaLabelInPanel(panelStanga, L"Nume:", 15, 215);
            txtNume = gcnew TextBox();
            txtNume->Location = Point(110, 213);
            txtNume->Size = System::Drawing::Size(230, 26);
            panelStanga->Controls->Add(txtNume);

            AdaugaLabelInPanel(panelStanga, L"Varsta:", 15, 250);
            numVarsta = gcnew NumericUpDown();
            numVarsta->Location = Point(110, 248);
            numVarsta->Size = System::Drawing::Size(80, 26);
            numVarsta->Minimum = 1; numVarsta->Maximum = 120;
            numVarsta->Value = 25;
            panelStanga->Controls->Add(numVarsta);

            AdaugaLabelInPanel(panelStanga, L"Tip:", 15, 285);
            cbTip = gcnew ComboBox();
            cbTip->Location = Point(110, 283);
            cbTip->Size = System::Drawing::Size(230, 26);
            cbTip->DropDownStyle = ComboBoxStyle::DropDownList;
            cbTip->Items->Add(L"Adult (fara reducere)");
            cbTip->Items->Add(L"Student (-20%)");
            cbTip->Items->Add(L"Elev (-15%)");
            cbTip->Items->Add(L"Pensionar (-30%)");
            cbTip->Items->Add(L"Copil (-50%)");
            cbTip->SelectedIndex = 0;
            panelStanga->Controls->Add(cbTip);

            AdaugaLabelInPanel(panelStanga, L"Email:", 15, 320);
            txtEmail = gcnew TextBox();
            txtEmail->Location = Point(110, 318);
            txtEmail->Size = System::Drawing::Size(230, 26);
            panelStanga->Controls->Add(txtEmail);

            AdaugaLabelInPanel(panelStanga, L"Telefon:", 15, 355);
            txtTelefon = gcnew TextBox();
            txtTelefon->Location = Point(110, 353);
            txtTelefon->Size = System::Drawing::Size(230, 26);
            panelStanga->Controls->Add(txtTelefon);

            Label^ separator2 = gcnew Label();
            separator2->Location = Point(15, 395);
            separator2->Size = System::Drawing::Size(330, 2);
            separator2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            panelStanga->Controls->Add(separator2);

            lblLocSelectat = gcnew Label();
            lblLocSelectat->Location = Point(15, 405);
            lblLocSelectat->Size = System::Drawing::Size(330, 26);
            lblLocSelectat->Text = L"Selectati un loc din harta -->";
            lblLocSelectat->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            lblLocSelectat->ForeColor = Color::DarkBlue;
            panelStanga->Controls->Add(lblLocSelectat);

            btnOK = gcnew Button();
            btnOK->Text = L"Confirma rezervare";
            btnOK->Location = Point(15, 510);
            btnOK->Size = System::Drawing::Size(160, 40);
            btnOK->BackColor = Color::FromArgb(60, 130, 80);
            btnOK->ForeColor = Color::White;
            btnOK->FlatStyle = FlatStyle::Flat;
            btnOK->Enabled = false;
            btnOK->Click += gcnew EventHandler(this, &DialogRezervare::OnOK);
            panelStanga->Controls->Add(btnOK);

            btnCancel = gcnew Button();
            btnCancel->Text = L"Anuleaza";
            btnCancel->Location = Point(185, 510);
            btnCancel->Size = System::Drawing::Size(155, 40);
            btnCancel->BackColor = Color::FromArgb(160, 160, 160);
            btnCancel->ForeColor = Color::White;
            btnCancel->FlatStyle = FlatStyle::Flat;
            btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            panelStanga->Controls->Add(btnCancel);

            // === Dreapta: harta locurilor ===
            Panel^ panelDr = gcnew Panel();
            panelDr->Location = Point(385, 15);
            panelDr->Size = System::Drawing::Size(490, 580);
            panelDr->BackColor = Color::White;
            panelDr->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->Controls->Add(panelDr);

            Label^ titluH = gcnew Label();
            titluH->Text = L"Harta locurilor";
            titluH->Font = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            titluH->Location = Point(15, 12);
            titluH->Size = System::Drawing::Size(330, 24);
            titluH->ForeColor = Color::FromArgb(30, 30, 60);
            panelDr->Controls->Add(titluH);

            Label^ ecran = gcnew Label();
            ecran->Text = L"ECRAN";
            ecran->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            ecran->Location = Point(40, 45);
            ecran->Size = System::Drawing::Size(440, 26);
            ecran->TextAlign = ContentAlignment::MiddleCenter;
            ecran->BackColor = Color::FromArgb(50, 50, 80);
            ecran->ForeColor = Color::White;
            panelDr->Controls->Add(ecran);

            panelLocuri = gcnew Panel();
            panelLocuri->Location = Point(15, 80);
            panelLocuri->Size = System::Drawing::Size(465, 430);
            panelLocuri->AutoScroll = true;
            panelDr->Controls->Add(panelLocuri);

            lblLegenda = gcnew Label();
            lblLegenda->Location = Point(15, 520);
            lblLegenda->Size = System::Drawing::Size(465, 50);
            lblLegenda->Text = L"Legenda:  ■ Liber    ■ Ocupat    ■ VIP    ■ Selectat";
            panelDr->Controls->Add(lblLegenda);

            this->CancelButton = btnCancel;
        }

        void AdaugaLabelInPanel(Panel^ p, String^ text, int x, int y) {
            Label^ l = gcnew Label();
            l->Text = text;
            l->Location = Point(x, y);
            l->Size = System::Drawing::Size(90, 22);
            p->Controls->Add(l);
        }

        void ConstruiesteHarta() {
            int randuri = spectacol->GetRanduri();
            int coloane = spectacol->GetColoane();
            int latime = 34, inaltime = 30, gap = 4;

            butoaneLocuri = gcnew cli::array<Button^, 2>(randuri, coloane);

            // Eticheta randuri (axa Y)
            for (int i = 0; i < randuri; i++) {
                Label^ l = gcnew Label();
                l->Text = (i + 1).ToString();
                l->Location = Point(5, 8 + i * (inaltime + gap));
                l->Size = System::Drawing::Size(25, inaltime);
                l->TextAlign = ContentAlignment::MiddleRight;
                l->Font = gcnew Drawing::Font(L"Segoe UI", 8);
                panelLocuri->Controls->Add(l);
            }
            // Eticheta coloane (axa X)
            for (int j = 0; j < coloane; j++) {
                Label^ l = gcnew Label();
                l->Text = (j + 1).ToString();
                l->Location = Point(35 + j * (latime + gap), -10);
                l->Size = System::Drawing::Size(latime, 18);
                l->TextAlign = ContentAlignment::MiddleCenter;
                l->Font = gcnew Drawing::Font(L"Segoe UI", 7);
                panelLocuri->Controls->Add(l);
            }

            for (int i = 0; i < randuri; i++) {
                for (int j = 0; j < coloane; j++) {
                    Button^ b = gcnew Button();
                    b->Location = Point(35 + j * (latime + gap), 8 + i * (inaltime + gap));
                    b->Size = System::Drawing::Size(latime, inaltime);
                    b->FlatStyle = FlatStyle::Flat;
                    b->FlatAppearance->BorderSize = 1;
                    b->Font = gcnew Drawing::Font(L"Segoe UI", 7);

                    if (!spectacol->LocLiber(i, j)) {
                        b->BackColor = Color::FromArgb(180, 70, 70);
                        b->ForeColor = Color::White;
                        b->Text = L"X";
                        b->Enabled = false;
                    }
                    else if (spectacol->EsteVIP(i)) {
                        b->BackColor = Color::FromArgb(210, 175, 90);
                        b->ForeColor = Color::White;
                        b->Text = L"V";
                    }
                    else {
                        b->BackColor = Color::FromArgb(120, 180, 120);
                        b->ForeColor = Color::White;
                        b->Text = L"L";
                    }

                    b->Tag = gcnew cli::array<int>(2) { i, j };
                    b->Click += gcnew EventHandler(this, &DialogRezervare::OnLocClick);
                    butoaneLocuri[i, j] = b;
                    panelLocuri->Controls->Add(b);
                }
            }
        }

        void OnLocClick(Object^ sender, EventArgs^ e) {
            Button^ b = (Button^)sender;
            cli::array<int>^ pos = (cli::array<int>^)b->Tag;
            int r = pos[0], c = pos[1];

            // Deselecteaza vechiul
            if (randSelectat >= 0 && colSelectata >= 0) {
                Button^ bv = butoaneLocuri[randSelectat, colSelectata];
                if (spectacol->EsteVIP(randSelectat))
                    bv->BackColor = Color::FromArgb(210, 175, 90);
                else
                    bv->BackColor = Color::FromArgb(120, 180, 120);
            }

            randSelectat = r;
            colSelectata = c;
            b->BackColor = Color::FromArgb(60, 110, 200);

            String^ tipLoc = spectacol->EsteVIP(r) ? L" (VIP)" : L"";
            lblLocSelectat->Text = String::Format(L"Selectat: Rand {0}, Coloana {1}{2}",
                r + 1, c + 1, tipLoc);
            btnOK->Enabled = true;
        }

        void OnOK(Object^ sender, EventArgs^ e) {
            try {
                std::string nume = marshal_as<std::string>(txtNume->Text->Trim());
                if (nume.empty()) {
                    MessageBox::Show(L"Introduceti numele clientului.", L"Eroare");
                    return;
                }
                int varsta = (int)numVarsta->Value;
                std::string email = marshal_as<std::string>(txtEmail->Text->Trim());
                std::string telefon = marshal_as<std::string>(txtTelefon->Text->Trim());

                Persoana* client = nullptr;
                switch (cbTip->SelectedIndex) {
                case 0: client = new Adult(nume, varsta, email, telefon); break;
                case 1: client = new Student(nume, varsta, email, telefon); break;
                case 2: client = new Elev(nume, varsta, email, telefon); break;
                case 3: client = new Pensionar(nume, varsta, email, telefon); break;
                case 4: client = new Copil(nume, varsta, email, telefon); break;
                default: client = new Adult(nume, varsta, email, telefon); break;
                }

                Rezervare* r = CinemaContext::cinema->FaRezervare(
                    client, spectacol, randSelectat, colSelectata);
                CinemaContext::cinema->GetClientiRef().Adauga(client);

                // Afiseaza biletul
                System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
                sb->AppendLine(L"=== BILET ===");
                sb->AppendLine(String::Format(L"Nr. bilet: {0}", r->GetID()));
                sb->AppendLine(String::Format(L"Client: {0} ({1})",
                    marshal_as<String^>(r->GetNumeClient()),
                    marshal_as<String^>(r->GetTipClient())));
                sb->AppendLine(String::Format(L"Film:   {0}",
                    marshal_as<String^>(r->GetTitluFilm())));
                sb->AppendLine(String::Format(L"Sala:   {0}",
                    marshal_as<String^>(r->GetNumarSala())));
                sb->AppendLine(String::Format(L"Data:   {0}  {1}",
                    marshal_as<String^>(r->GetData()),
                    marshal_as<String^>(r->GetOra())));
                sb->AppendLine(String::Format(L"Loc:    Rand {0}, Col {1}",
                    r->GetRand() + 1, r->GetColoana() + 1));
                sb->AppendLine(String::Format(L"Pret final: {0:F2} RON", r->GetPretFinal()));

                MessageBox::Show(sb->ToString(), L"Rezervare confirmata",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);

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
