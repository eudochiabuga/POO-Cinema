#pragma once
#include "AppContext.h"
#include "Exceptii.h"
#include <msclr/marshal_cppstd.h>

namespace CinemaApp {

    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class DialogFilm : public Form {
    private:
        Film* filmExistent;     // nullptr daca e "Adauga"

        TextBox^ txtTitlu;
        TextBox^ txtCategorie;
        NumericUpDown^ numDurata;
        NumericUpDown^ numVarsta;
        NumericUpDown^ numAn;
        ComboBox^ cbLimba;
        TextBox^ txtRegizor;
        NumericUpDown^ numRating;
        Button^ btnOK;
        Button^ btnCancel;

    public:
        DialogFilm(Film* filmDeEditat) : filmExistent(filmDeEditat) {
            InitializeControls();
            if (filmExistent) PopuleazaCampuri();
        }

    private:
        void InitializeControls() {
            this->Text = filmExistent ? L"Editare film" : L"Adauga film";
            this->Size = System::Drawing::Size(460, 460);
            this->StartPosition = FormStartPosition::CenterParent;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Font = gcnew Drawing::Font(L"Segoe UI", 9);

            int y = 25;
            int gap = 38;

            AdaugaLabel(L"Titlu:", 30, y);
            txtTitlu = gcnew TextBox();
            txtTitlu->Location = Point(150, y - 2);
            txtTitlu->Size = System::Drawing::Size(260, 26);
            this->Controls->Add(txtTitlu);
            y += gap;

            AdaugaLabel(L"Categorie:", 30, y);
            txtCategorie = gcnew TextBox();
            txtCategorie->Location = Point(150, y - 2);
            txtCategorie->Size = System::Drawing::Size(260, 26);
            this->Controls->Add(txtCategorie);
            y += gap;

            AdaugaLabel(L"Regizor:", 30, y);
            txtRegizor = gcnew TextBox();
            txtRegizor->Location = Point(150, y - 2);
            txtRegizor->Size = System::Drawing::Size(260, 26);
            this->Controls->Add(txtRegizor);
            y += gap;

            AdaugaLabel(L"Durata (min):", 30, y);
            numDurata = gcnew NumericUpDown();
            numDurata->Location = Point(150, y - 2);
            numDurata->Size = System::Drawing::Size(100, 26);
            numDurata->Minimum = 1; numDurata->Maximum = 500;
            numDurata->Value = 120;
            this->Controls->Add(numDurata);
            y += gap;

            AdaugaLabel(L"Varsta minima:", 30, y);
            numVarsta = gcnew NumericUpDown();
            numVarsta->Location = Point(150, y - 2);
            numVarsta->Size = System::Drawing::Size(100, 26);
            numVarsta->Minimum = 0; numVarsta->Maximum = 21;
            numVarsta->Value = 0;
            this->Controls->Add(numVarsta);
            y += gap;

            AdaugaLabel(L"An:", 30, y);
            numAn = gcnew NumericUpDown();
            numAn->Location = Point(150, y - 2);
            numAn->Size = System::Drawing::Size(100, 26);
            numAn->Minimum = 1900; numAn->Maximum = 2100;
            numAn->Value = 2024;
            this->Controls->Add(numAn);
            y += gap;

            AdaugaLabel(L"Rating (0-10):", 30, y);
            numRating = gcnew NumericUpDown();
            numRating->Location = Point(150, y - 2);
            numRating->Size = System::Drawing::Size(100, 26);
            numRating->DecimalPlaces = 1;
            numRating->Increment = (Decimal)0.1;
            numRating->Minimum = 0; numRating->Maximum = 10;
            numRating->Value = (Decimal)7.0;
            this->Controls->Add(numRating);
            y += gap;

            AdaugaLabel(L"Limba:", 30, y);
            cbLimba = gcnew ComboBox();
            cbLimba->Location = Point(150, y - 2);
            cbLimba->Size = System::Drawing::Size(180, 26);
            cbLimba->DropDownStyle = ComboBoxStyle::DropDown;
            cbLimba->Items->Add(L"Romana");
            cbLimba->Items->Add(L"Engleza");
            cbLimba->Items->Add(L"Engleza (subtitrare RO)");
            cbLimba->Items->Add(L"Spaniola");
            cbLimba->Items->Add(L"Franceza");
            cbLimba->SelectedIndex = 1;
            this->Controls->Add(cbLimba);
            y += gap + 10;

            btnOK = gcnew Button();
            btnOK->Text = filmExistent ? L"Salveaza" : L"Adauga";
            btnOK->Location = Point(150, y);
            btnOK->Size = System::Drawing::Size(120, 34);
            btnOK->BackColor = Color::FromArgb(60, 130, 80);
            btnOK->ForeColor = Color::White;
            btnOK->FlatStyle = FlatStyle::Flat;
            btnOK->Click += gcnew EventHandler(this, &DialogFilm::OnOK);
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

        void PopuleazaCampuri() {
            txtTitlu->Text = marshal_as<String^>(filmExistent->GetTitlu());
            txtCategorie->Text = marshal_as<String^>(filmExistent->GetCategorie());
            txtRegizor->Text = marshal_as<String^>(filmExistent->GetRegizor());
            numDurata->Value = filmExistent->GetDurata();
            numVarsta->Value = filmExistent->GetVarstaMinima();
            numAn->Value = filmExistent->GetAn();
            numRating->Value = (Decimal)filmExistent->GetRating();
            cbLimba->Text = marshal_as<String^>(filmExistent->GetLimba());
        }

        void OnOK(Object^ sender, EventArgs^ e) {
            try {
                std::string titlu = marshal_as<std::string>(txtTitlu->Text->Trim());
                std::string categorie = marshal_as<std::string>(txtCategorie->Text->Trim());
                std::string regizor = marshal_as<std::string>(txtRegizor->Text->Trim());
                std::string limba = marshal_as<std::string>(cbLimba->Text->Trim());

                if (titlu.empty()) {
                    MessageBox::Show(L"Titlul nu poate fi gol.", L"Eroare");
                    return;
                }
                if (categorie.empty()) categorie = "General";
                if (limba.empty()) limba = "Romana";

                int durata = (int)numDurata->Value;
                int varsta = (int)numVarsta->Value;
                int an = (int)numAn->Value;
                double rating = (double)numRating->Value;

                if (filmExistent) {
                    filmExistent->SetTitlu(titlu);
                    filmExistent->SetCategorie(categorie);
                    filmExistent->SetRegizor(regizor);
                    filmExistent->SetDurata(durata);
                    filmExistent->SetVarstaMinima(varsta);
                    filmExistent->SetAn(an);
                    filmExistent->SetRating(rating);
                    filmExistent->SetLimba(limba);
                }
                else {
                    Film* f = new Film(titlu, categorie, durata, varsta, limba,
                        regizor, an, rating);
                    CinemaContext::cinema->AdaugaFilm(f);
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
