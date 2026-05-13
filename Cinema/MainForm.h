#pragma once
#define _HAS_STD_BYTE 0
#include <windows.h>
#ifdef System
#undef System
#endif
#include "AppContext.h"
#include "Persistenta.h"
#include "Raport.h"
#include "Exceptii.h"
#include "DialogFilm.h"
#include "DialogSala.h"
#include "DialogSpectacol.h"
#include "DialogRezervare.h"
#include "DialogAngajat.h"
#include "SalaVIP.h"
#include <msclr/marshal_cppstd.h>
#define Microsoft_Visual_Studio_v143
namespace CinemaApp {

    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class MainForm : public Form {
    private:
        // Header
        Panel^ panelHeader;
        Label^ lblTitlu;
        Label^ lblUtilizator;
        Button^ btnLogout;
        Button^ btnSalveaza;
        Button^ btnIncarca;

        TabControl^ tabs;
        TabPage^ tabDashboard;
        TabPage^ tabFilme;
        TabPage^ tabSali;
        TabPage^ tabSpectacole;
        TabPage^ tabRezervari;
        TabPage^ tabStatistici;
        TabPage^ tabAngajati;

        // Filme
        DataGridView^ dgvFilme;
        Button^ btnAddFilm;
        Button^ btnEditFilm;
        Button^ btnDelFilm;

        // Sali
        DataGridView^ dgvSali;
        Button^ btnAddSala;
        Button^ btnDelSala;

        // Spectacole
        DataGridView^ dgvSpectacole;
        Button^ btnAddSpectacol;
        Button^ btnRezerva;
        Button^ btnHartaSpectacol;
        Button^ btnDelSpectacol;

        // Rezervari
        DataGridView^ dgvRezervari;
        Button^ btnDelRezervare;
        Label^ lblTotalIncasari;

        // Statistici
        TextBox^ txtRaport;

        // Angajati
        DataGridView^ dgvAngajati;
        Button^ btnAddAngajat;
        Button^ btnDelAngajat;

        // Status bar
        StatusStrip^ statusBar;
        ToolStripStatusLabel^ statusLabel;

    public:
        MainForm() {
            InitializeControls();
            ActualizeazaToate();
        }

    private:
        void InitializeControls() {
            this->Text = String::Format(L"{0} - Sistem de gestiune",
                marshal_as<String^>(CinemaContext::cinema->GetNume()));
            this->Size = System::Drawing::Size(1280, 780);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->WindowState = FormWindowState::Normal;
            this->Font = gcnew Drawing::Font(L"Segoe UI", 9);
            this->MinimumSize = System::Drawing::Size(1100, 720);
            this->BackColor = Color::FromArgb(245, 245, 250);

            InitHeader();
            InitTabs();
            InitStatus();

            ActualizeazaDrepturi();
        }

        void InitHeader() {
            panelHeader = gcnew Panel();
            panelHeader->Dock = DockStyle::Top;
            panelHeader->Height = 70;
            panelHeader->BackColor = Color::FromArgb(30, 30, 60);
            this->Controls->Add(panelHeader);

            lblTitlu = gcnew Label();
            lblTitlu->Text = String::Format(L"  {0}",
                marshal_as<String^>(CinemaContext::cinema->GetNume()));
            lblTitlu->Font = gcnew Drawing::Font(L"Segoe UI", 16, FontStyle::Bold);
            lblTitlu->ForeColor = Color::White;
            lblTitlu->Location = Point(15, 10);
            lblTitlu->Size = System::Drawing::Size(380, 32);
            panelHeader->Controls->Add(lblTitlu);

            Label^ lblSubtitlu = gcnew Label();
            lblSubtitlu->Text = String::Format(L"  {0}, {1}",
                marshal_as<String^>(CinemaContext::cinema->GetAdresa()),
                marshal_as<String^>(CinemaContext::cinema->GetOras()));
            lblSubtitlu->Font = gcnew Drawing::Font(L"Segoe UI", 9);
            lblSubtitlu->ForeColor = Color::FromArgb(200, 200, 220);
            lblSubtitlu->Location = Point(18, 42);
            lblSubtitlu->Size = System::Drawing::Size(380, 20);
            panelHeader->Controls->Add(lblSubtitlu);

            lblUtilizator = gcnew Label();
            Angajat* a = CinemaContext::auth->GetUtilizatorCurent();
            if (a) {
                lblUtilizator->Text = String::Format(L"Logat ca: {0} ({1})",
                    marshal_as<String^>(a->GetNume()),
                    marshal_as<String^>(a->GetRol()));
            }
            lblUtilizator->Font = gcnew Drawing::Font(L"Segoe UI", 10);
            lblUtilizator->ForeColor = Color::White;
            lblUtilizator->TextAlign = ContentAlignment::MiddleRight;
            lblUtilizator->Location = Point(550, 12);
            lblUtilizator->Size = System::Drawing::Size(380, 24);
            lblUtilizator->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Right);
            panelHeader->Controls->Add(lblUtilizator);

            btnSalveaza = gcnew Button();
            btnSalveaza->Text = L"Salveaza";
            btnSalveaza->Location = Point(770, 38);
            btnSalveaza->Size = System::Drawing::Size(85, 26);
            btnSalveaza->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Right);
            btnSalveaza->BackColor = Color::FromArgb(70, 130, 90);
            btnSalveaza->ForeColor = Color::White;
            btnSalveaza->FlatStyle = FlatStyle::Flat;
            btnSalveaza->Click += gcnew EventHandler(this, &MainForm::OnSalveaza);
            panelHeader->Controls->Add(btnSalveaza);

            btnIncarca = gcnew Button();
            btnIncarca->Text = L"Incarca";
            btnIncarca->Location = Point(860, 38);
            btnIncarca->Size = System::Drawing::Size(85, 26);
            btnIncarca->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Right);
            btnIncarca->BackColor = Color::FromArgb(70, 110, 180);
            btnIncarca->ForeColor = Color::White;
            btnIncarca->FlatStyle = FlatStyle::Flat;
            btnIncarca->Click += gcnew EventHandler(this, &MainForm::OnIncarca);
            panelHeader->Controls->Add(btnIncarca);

            btnLogout = gcnew Button();
            btnLogout->Text = L"Iesire";
            btnLogout->Location = Point(950, 38);
            btnLogout->Size = System::Drawing::Size(85, 26);
            btnLogout->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Right);
            btnLogout->BackColor = Color::FromArgb(180, 70, 70);
            btnLogout->ForeColor = Color::White;
            btnLogout->FlatStyle = FlatStyle::Flat;
            btnLogout->Click += gcnew EventHandler(this, &MainForm::OnLogout);
            panelHeader->Controls->Add(btnLogout);
        }

        void InitTabs() {
            tabs = gcnew TabControl();
            tabs->Dock = DockStyle::Fill;
            tabs->Font = gcnew Drawing::Font(L"Segoe UI", 10);
            tabs->Padding = Point(15, 6);
            this->Controls->Add(tabs);
            tabs->BringToFront();

            tabDashboard = gcnew TabPage(L"Dashboard");
            tabFilme = gcnew TabPage(L"Filme");
            tabSali = gcnew TabPage(L"Sali");
            tabSpectacole = gcnew TabPage(L"Spectacole");
            tabRezervari = gcnew TabPage(L"Rezervari");
            tabStatistici = gcnew TabPage(L"Statistici");
            tabAngajati = gcnew TabPage(L"Angajati");

            tabs->TabPages->Add(tabDashboard);
            tabs->TabPages->Add(tabFilme);
            tabs->TabPages->Add(tabSali);
            tabs->TabPages->Add(tabSpectacole);
            tabs->TabPages->Add(tabRezervari);
            tabs->TabPages->Add(tabStatistici);
            tabs->TabPages->Add(tabAngajati);

            InitDashboard();
            InitTabFilme();
            InitTabSali();
            InitTabSpectacole();
            InitTabRezervari();
            InitTabStatistici();
            InitTabAngajati();
        }

        void InitStatus() {
            statusBar = gcnew StatusStrip();
            statusLabel = gcnew ToolStripStatusLabel();
            statusLabel->Text = L"Gata.";
            statusBar->Items->Add(statusLabel);
            this->Controls->Add(statusBar);
        }

        // -------------------- DASHBOARD --------------------
        Label^ lblDashFilme;
        Label^ lblDashSali;
        Label^ lblDashSpectacole;
        Label^ lblDashRezervari;
        Label^ lblDashIncasari;
        Label^ lblDashAngajati;
        ListBox^ lstUltimeleRezervari;

        void InitDashboard() {
            tabDashboard->BackColor = Color::FromArgb(245, 245, 250);

            Label^ titlu = gcnew Label();
            titlu->Text = L"Sumar general";
            titlu->Font = gcnew Drawing::Font(L"Segoe UI", 14, FontStyle::Bold);
            titlu->Location = Point(20, 20);
            titlu->Size = System::Drawing::Size(400, 32);
            titlu->ForeColor = Color::FromArgb(30, 30, 60);
            tabDashboard->Controls->Add(titlu);

            int cx = 20, cy = 70, w = 220, h = 100, gap = 18;
            lblDashFilme = AdaugaCard(tabDashboard, cx, cy, w, h, L"Filme",
                Color::FromArgb(120, 160, 220));
            lblDashSali = AdaugaCard(tabDashboard, cx + (w + gap), cy, w, h, L"Sali",
                Color::FromArgb(180, 130, 200));
            lblDashSpectacole = AdaugaCard(tabDashboard, cx + 2 * (w + gap), cy, w, h, L"Spectacole",
                Color::FromArgb(110, 180, 150));
            lblDashRezervari = AdaugaCard(tabDashboard, cx + 3 * (w + gap), cy, w, h, L"Rezervari",
                Color::FromArgb(220, 160, 90));
            lblDashIncasari = AdaugaCard(tabDashboard, cx, cy + h + gap, w * 2 + gap, h, L"Incasari totale",
                Color::FromArgb(60, 130, 90));
            lblDashAngajati = AdaugaCard(tabDashboard, cx + 2 * (w + gap), cy + h + gap, w, h, L"Angajati",
                Color::FromArgb(80, 80, 130));

            Label^ titluR = gcnew Label();
            titluR->Text = L"Ultimele 10 rezervari";
            titluR->Font = gcnew Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            titluR->Location = Point(20, 320);
            titluR->Size = System::Drawing::Size(400, 28);
            titluR->ForeColor = Color::FromArgb(30, 30, 60);
            tabDashboard->Controls->Add(titluR);

            lstUltimeleRezervari = gcnew ListBox();
            lstUltimeleRezervari->Location = Point(20, 350);
            lstUltimeleRezervari->Size = System::Drawing::Size(940, 280);
            lstUltimeleRezervari->Font = gcnew Drawing::Font(L"Consolas", 10);
            lstUltimeleRezervari->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Left
                | AnchorStyles::Right | AnchorStyles::Bottom);
            tabDashboard->Controls->Add(lstUltimeleRezervari);
        }

        Label^ AdaugaCard(TabPage^ parent, int x, int y, int w, int h,
            String^ titlu, Color culoare) {
            Panel^ p = gcnew Panel();
            p->Location = Point(x, y);
            p->Size = System::Drawing::Size(w, h);
            p->BackColor = culoare;
            parent->Controls->Add(p);

            Label^ lblT = gcnew Label();
            lblT->Text = titlu;
            lblT->Font = gcnew Drawing::Font(L"Segoe UI", 11);
            lblT->ForeColor = Color::White;
            lblT->Location = Point(15, 12);
            lblT->Size = System::Drawing::Size(w - 30, 22);
            p->Controls->Add(lblT);

            Label^ lblV = gcnew Label();
            lblV->Text = L"0";
            lblV->Font = gcnew Drawing::Font(L"Segoe UI", 22, FontStyle::Bold);
            lblV->ForeColor = Color::White;
            lblV->Location = Point(15, 38);
            lblV->Size = System::Drawing::Size(w - 30, 52);
            p->Controls->Add(lblV);

            return lblV;
        }

        // -------------------- FILME --------------------
        void InitTabFilme() {
            tabFilme->BackColor = Color::White;

            dgvFilme = gcnew DataGridView();
            dgvFilme->Location = Point(15, 15);
            dgvFilme->Size = System::Drawing::Size(1130, 520);
            dgvFilme->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Left
                | AnchorStyles::Right | AnchorStyles::Bottom);
            dgvFilme->ReadOnly = true;
            dgvFilme->AllowUserToAddRows = false;
            dgvFilme->AllowUserToDeleteRows = false;
            dgvFilme->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgvFilme->MultiSelect = false;
            dgvFilme->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgvFilme->RowHeadersVisible = false;
            dgvFilme->BackgroundColor = Color::White;
            dgvFilme->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;

            dgvFilme->Columns->Add(L"ID", L"ID");
            dgvFilme->Columns->Add(L"Titlu", L"Titlu");
            dgvFilme->Columns->Add(L"Categorie", L"Categorie");
            dgvFilme->Columns->Add(L"Regizor", L"Regizor");
            dgvFilme->Columns->Add(L"An", L"An");
            dgvFilme->Columns->Add(L"Durata", L"Durata");
            dgvFilme->Columns->Add(L"Varsta", L"Varsta min.");
            dgvFilme->Columns->Add(L"Rating", L"Rating");
            dgvFilme->Columns->Add(L"Limba", L"Limba");

            tabFilme->Controls->Add(dgvFilme);

            btnAddFilm = AdaugaButon(tabFilme, L"Adauga film", 15, 545,
                Color::FromArgb(60, 130, 80));
            btnAddFilm->Click += gcnew EventHandler(this, &MainForm::OnAddFilm);

            btnEditFilm = AdaugaButon(tabFilme, L"Editeaza", 145, 545,
                Color::FromArgb(60, 110, 180));
            btnEditFilm->Click += gcnew EventHandler(this, &MainForm::OnEditFilm);

            btnDelFilm = AdaugaButon(tabFilme, L"Sterge", 275, 545,
                Color::FromArgb(180, 70, 70));
            btnDelFilm->Click += gcnew EventHandler(this, &MainForm::OnDelFilm);
        }

        Button^ AdaugaButon(TabPage^ parent, String^ text, int x, int y, Color culoare) {
            Button^ b = gcnew Button();
            b->Text = text;
            b->Location = Point(x, y);
            b->Size = System::Drawing::Size(120, 34);
            b->Anchor = (AnchorStyles)(AnchorStyles::Bottom | AnchorStyles::Left);
            b->BackColor = culoare;
            b->ForeColor = Color::White;
            b->FlatStyle = FlatStyle::Flat;
            parent->Controls->Add(b);
            return b;
        }

        // -------------------- SALI --------------------
        void InitTabSali() {
            tabSali->BackColor = Color::White;

            dgvSali = gcnew DataGridView();
            dgvSali->Location = Point(15, 15);
            dgvSali->Size = System::Drawing::Size(1130, 520);
            dgvSali->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Left
                | AnchorStyles::Right | AnchorStyles::Bottom);
            dgvSali->ReadOnly = true;
            dgvSali->AllowUserToAddRows = false;
            dgvSali->AllowUserToDeleteRows = false;
            dgvSali->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgvSali->MultiSelect = false;
            dgvSali->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgvSali->RowHeadersVisible = false;
            dgvSali->BackgroundColor = Color::White;
            dgvSali->Columns->Add(L"ID", L"ID");
            dgvSali->Columns->Add(L"Numar", L"Numar sala");
            dgvSali->Columns->Add(L"Tip", L"Tip");
            dgvSali->Columns->Add(L"Randuri", L"Randuri");
            dgvSali->Columns->Add(L"Coloane", L"Coloane");
            dgvSali->Columns->Add(L"Capacitate", L"Capacitate");
            dgvSali->Columns->Add(L"Detalii", L"Detalii");
            tabSali->Controls->Add(dgvSali);

            btnAddSala = AdaugaButon(tabSali, L"Adauga sala", 15, 545,
                Color::FromArgb(60, 130, 80));
            btnAddSala->Click += gcnew EventHandler(this, &MainForm::OnAddSala);

            btnDelSala = AdaugaButon(tabSali, L"Sterge sala", 145, 545,
                Color::FromArgb(180, 70, 70));
            btnDelSala->Click += gcnew EventHandler(this, &MainForm::OnDelSala);
        }

        // -------------------- SPECTACOLE --------------------
        void InitTabSpectacole() {
            tabSpectacole->BackColor = Color::White;

            dgvSpectacole = gcnew DataGridView();
            dgvSpectacole->Location = Point(15, 15);
            dgvSpectacole->Size = System::Drawing::Size(1130, 520);
            dgvSpectacole->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Left
                | AnchorStyles::Right | AnchorStyles::Bottom);
            dgvSpectacole->ReadOnly = true;
            dgvSpectacole->AllowUserToAddRows = false;
            dgvSpectacole->AllowUserToDeleteRows = false;
            dgvSpectacole->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgvSpectacole->MultiSelect = false;
            dgvSpectacole->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgvSpectacole->RowHeadersVisible = false;
            dgvSpectacole->BackgroundColor = Color::White;
            dgvSpectacole->Columns->Add(L"ID", L"ID");
            dgvSpectacole->Columns->Add(L"Data", L"Data");
            dgvSpectacole->Columns->Add(L"Ora", L"Ora");
            dgvSpectacole->Columns->Add(L"Film", L"Film");
            dgvSpectacole->Columns->Add(L"Sala", L"Sala");
            dgvSpectacole->Columns->Add(L"Pret", L"Pret");
            dgvSpectacole->Columns->Add(L"Libere", L"Locuri libere");
            dgvSpectacole->Columns->Add(L"Ocupare", L"Ocupare %");
            tabSpectacole->Controls->Add(dgvSpectacole);

            btnAddSpectacol = AdaugaButon(tabSpectacole, L"Adauga spectacol", 15, 545,
                Color::FromArgb(60, 130, 80));
            btnAddSpectacol->Click += gcnew EventHandler(this, &MainForm::OnAddSpectacol);
            btnAddSpectacol->Width = 150;

            btnRezerva = AdaugaButon(tabSpectacole, L"Rezerva loc", 175, 545,
                Color::FromArgb(220, 140, 60));
            btnRezerva->Click += gcnew EventHandler(this, &MainForm::OnRezerva);

            btnHartaSpectacol = AdaugaButon(tabSpectacole, L"Vezi harta", 305, 545,
                Color::FromArgb(60, 110, 180));
            btnHartaSpectacol->Click += gcnew EventHandler(this, &MainForm::OnVeziHarta);

            btnDelSpectacol = AdaugaButon(tabSpectacole, L"Sterge", 435, 545,
                Color::FromArgb(180, 70, 70));
            btnDelSpectacol->Click += gcnew EventHandler(this, &MainForm::OnDelSpectacol);
        }

        // -------------------- REZERVARI --------------------
        void InitTabRezervari() {
            tabRezervari->BackColor = Color::White;

            dgvRezervari = gcnew DataGridView();
            dgvRezervari->Location = Point(15, 15);
            dgvRezervari->Size = System::Drawing::Size(1130, 500);
            dgvRezervari->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Left
                | AnchorStyles::Right | AnchorStyles::Bottom);
            dgvRezervari->ReadOnly = true;
            dgvRezervari->AllowUserToAddRows = false;
            dgvRezervari->AllowUserToDeleteRows = false;
            dgvRezervari->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgvRezervari->MultiSelect = false;
            dgvRezervari->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgvRezervari->RowHeadersVisible = false;
            dgvRezervari->BackgroundColor = Color::White;
            dgvRezervari->Columns->Add(L"ID", L"Bilet #");
            dgvRezervari->Columns->Add(L"Client", L"Client");
            dgvRezervari->Columns->Add(L"Tip", L"Tip");
            dgvRezervari->Columns->Add(L"Film", L"Film");
            dgvRezervari->Columns->Add(L"Sala", L"Sala");
            dgvRezervari->Columns->Add(L"Data", L"Data");
            dgvRezervari->Columns->Add(L"Loc", L"Loc");
            dgvRezervari->Columns->Add(L"Pret", L"Pret final");
            dgvRezervari->Columns->Add(L"Emis", L"Emis la");
            tabRezervari->Controls->Add(dgvRezervari);

            lblTotalIncasari = gcnew Label();
            lblTotalIncasari->Location = Point(15, 525);
            lblTotalIncasari->Size = System::Drawing::Size(500, 28);
            lblTotalIncasari->Font = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            lblTotalIncasari->ForeColor = Color::FromArgb(60, 130, 90);
            lblTotalIncasari->Anchor = (AnchorStyles)(AnchorStyles::Bottom | AnchorStyles::Left);
            tabRezervari->Controls->Add(lblTotalIncasari);

            btnDelRezervare = AdaugaButon(tabRezervari, L"Anuleaza rezervare", 535, 545,
                Color::FromArgb(180, 70, 70));
            btnDelRezervare->Width = 160;
            btnDelRezervare->Click += gcnew EventHandler(this, &MainForm::OnDelRezervare);
        }

        // -------------------- STATISTICI --------------------
        Button^ btnRefreshRaport;
        Button^ btnExportRaport;
        void InitTabStatistici() {
            tabStatistici->BackColor = Color::White;

            txtRaport = gcnew TextBox();
            txtRaport->Location = Point(15, 15);
            txtRaport->Size = System::Drawing::Size(1130, 520);
            txtRaport->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Left
                | AnchorStyles::Right | AnchorStyles::Bottom);
            txtRaport->Multiline = true;
            txtRaport->ReadOnly = true;
            txtRaport->ScrollBars = ScrollBars::Vertical;
            txtRaport->Font = gcnew Drawing::Font(L"Consolas", 10);
            txtRaport->BackColor = Color::FromArgb(252, 252, 250);
            tabStatistici->Controls->Add(txtRaport);

            btnRefreshRaport = AdaugaButon(tabStatistici, L"Recalculeaza", 15, 545,
                Color::FromArgb(60, 110, 180));
            btnRefreshRaport->Click += gcnew EventHandler(this, &MainForm::OnRefreshRaport);

            btnExportRaport = AdaugaButon(tabStatistici, L"Exporta raport", 145, 545,
                Color::FromArgb(60, 130, 80));
            btnExportRaport->Width = 140;
            btnExportRaport->Click += gcnew EventHandler(this, &MainForm::OnExportRaport);
        }

        // -------------------- ANGAJATI --------------------
        void InitTabAngajati() {
            tabAngajati->BackColor = Color::White;

            dgvAngajati = gcnew DataGridView();
            dgvAngajati->Location = Point(15, 15);
            dgvAngajati->Size = System::Drawing::Size(1130, 520);
            dgvAngajati->Anchor = (AnchorStyles)(AnchorStyles::Top | AnchorStyles::Left
                | AnchorStyles::Right | AnchorStyles::Bottom);
            dgvAngajati->ReadOnly = true;
            dgvAngajati->AllowUserToAddRows = false;
            dgvAngajati->AllowUserToDeleteRows = false;
            dgvAngajati->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgvAngajati->MultiSelect = false;
            dgvAngajati->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgvAngajati->RowHeadersVisible = false;
            dgvAngajati->BackgroundColor = Color::White;
            dgvAngajati->Columns->Add(L"ID", L"ID");
            dgvAngajati->Columns->Add(L"Nume", L"Nume");
            dgvAngajati->Columns->Add(L"User", L"Username");
            dgvAngajati->Columns->Add(L"Rol", L"Rol");
            dgvAngajati->Columns->Add(L"Salariu", L"Salariu");
            tabAngajati->Controls->Add(dgvAngajati);

            btnAddAngajat = AdaugaButon(tabAngajati, L"Adauga angajat", 15, 545,
                Color::FromArgb(60, 130, 80));
            btnAddAngajat->Width = 140;
            btnAddAngajat->Click += gcnew EventHandler(this, &MainForm::OnAddAngajat);

            btnDelAngajat = AdaugaButon(tabAngajati, L"Concediere", 165, 545,
                Color::FromArgb(180, 70, 70));
            btnDelAngajat->Click += gcnew EventHandler(this, &MainForm::OnDelAngajat);
        }

        // -------------------- ACTUALIZARI --------------------
        void ActualizeazaDrepturi() {
            Angajat* a = CinemaContext::auth->GetUtilizatorCurent();
            if (!a) return;

            btnAddFilm->Enabled = a->PoateModificaFilme();
            btnEditFilm->Enabled = a->PoateModificaFilme();
            btnDelFilm->Enabled = a->PoateModificaFilme();

            btnAddSala->Enabled = a->PoateModificaSali();
            btnDelSala->Enabled = a->PoateModificaSali();

            btnAddSpectacol->Enabled = a->PoateAdaugaSpectacole();
            btnDelSpectacol->Enabled = a->PoateAdaugaSpectacole();

            btnAddAngajat->Enabled = a->PoateGestionaAngajati();
            btnDelAngajat->Enabled = a->PoateGestionaAngajati();

            tabAngajati->Enabled = a->PoateGestionaAngajati();
        }

        void ActualizeazaToate() {
            ActualizeazaFilme();
            ActualizeazaSali();
            ActualizeazaSpectacole();
            ActualizeazaRezervari();
            ActualizeazaAngajati();
            ActualizeazaDashboard();
            ActualizeazaRaport();
        }

        void ActualizeazaFilme() {
            dgvFilme->Rows->Clear();
            for (auto f : CinemaContext::cinema->GetFilme()) {
                cli::array<Object^>^ row = gcnew cli::array<Object^>(9);
                row[0] = f->GetID();
                row[1] = marshal_as<String^>(f->GetTitlu());
                row[2] = marshal_as<String^>(f->GetCategorie());
                row[3] = marshal_as<String^>(f->GetRegizor());
                row[4] = f->GetAn();
                row[5] = f->GetDurata() + L" min";
                row[6] = f->GetVarstaMinima() + L"+";
                row[7] = f->GetRating().ToString(L"F1") + L"/10";
                row[8] = marshal_as<String^>(f->GetLimba());

                dgvFilme->Rows->Add(row);
            }
        }

        void ActualizeazaSali() {
            dgvSali->Rows->Clear();
            for (auto s : CinemaContext::cinema->GetSali()) {
                String^ tip = s->EsteVIP() ? L"VIP" : L"Normala";
                String^ detalii = L"";
                if (s->EsteVIP()) {
                    SalaVIP* sv = dynamic_cast<SalaVIP*>(s);
                    if (sv) {
                        detalii = String::Format(L"{0}, +{1}%, {2} produse",
                            marshal_as<String^>(sv->GetTipScaun()),
                            (int)(sv->GetSuplimentPret() * 100),
                            (int)sv->GetMeniu().size());
                    }
                }
                dgvSali->Rows->Add(gcnew cli::array<Object^> {
                    s->GetID(),
                        marshal_as<String^>(s->GetNumarSala()),
                        tip,
                        s->GetRanduri(),
                        s->GetColoane(),
                        s->GetCapacitate(),
                        detalii
                });
            }
        }

        void ActualizeazaSpectacole() {
            dgvSpectacole->Rows->Clear();
            for (auto sp : CinemaContext::cinema->GetSpectacole()) {
                int locuriTotal = sp->GetRanduri() * sp->GetColoane();
                dgvSpectacole->Rows->Add(gcnew cli::array<Object^> {
                    sp->GetID(),
                        marshal_as<String^>(sp->GetData()),
                        marshal_as<String^>(sp->GetOra()),
                        marshal_as<String^>(sp->GetFilm()->GetTitlu()),
                        marshal_as<String^>(sp->GetSala()->GetNumarSala()),
                        sp->GetPretBaza().ToString(L"F2") + L" RON",
                        String::Format(L"{0}/{1}", sp->LocuriDisponibile(), locuriTotal),
                        sp->ProcentOcupare().ToString(L"F1") + L"%"
                });
            }
        }

        void ActualizeazaRezervari() {
            dgvRezervari->Rows->Clear();
            double total = 0;
            for (auto r : CinemaContext::cinema->GetRezervari()) {
                dgvRezervari->Rows->Add(gcnew cli::array<Object^> {
                    r->GetID(),
                        marshal_as<String^>(r->GetNumeClient()),
                        marshal_as<String^>(r->GetTipClient()),
                        marshal_as<String^>(r->GetTitluFilm()),
                        marshal_as<String^>(r->GetNumarSala()),
                        marshal_as<String^>(r->GetData() + " " + r->GetOra()),
                        String::Format(L"R{0}, C{1}", r->GetRand() + 1, r->GetColoana() + 1),
                        r->GetPretFinal().ToString(L"F2") + L" RON",
                        marshal_as<String^>(r->GetDataEmiterii())
                });
                total += r->GetPretFinal();
            }
            lblTotalIncasari->Text = String::Format(L"Total incasari: {0:F2} RON ({1} bilete)",
                total, (int)CinemaContext::cinema->GetRezervari().Marime());
        }

        void ActualizeazaAngajati() {
            dgvAngajati->Rows->Clear();
            for (auto a : CinemaContext::auth->GetAngajati()) {
                dgvAngajati->Rows->Add(gcnew cli::array<Object^> {
                    a->GetID(),
                        marshal_as<String^>(a->GetNume()),
                        marshal_as<String^>(a->GetUsername()),
                        marshal_as<String^>(a->GetRol()),
                        a->GetSalariu().ToString(L"F2") + L" RON"
                });
            }
        }

        void ActualizeazaDashboard() {
            lblDashFilme->Text = ((int)CinemaContext::cinema->GetFilme().Marime()).ToString();
            lblDashSali->Text = ((int)CinemaContext::cinema->GetSali().Marime()).ToString();
            lblDashSpectacole->Text = ((int)CinemaContext::cinema->GetSpectacole().Marime()).ToString();
            lblDashRezervari->Text = ((int)CinemaContext::cinema->GetRezervari().Marime()).ToString();
            lblDashAngajati->Text = ((int)CinemaContext::auth->GetAngajati().Marime()).ToString();

            double total = 0;
            for (auto r : CinemaContext::cinema->GetRezervari()) total += r->GetPretFinal();
            lblDashIncasari->Text = total.ToString(L"F2") + L" RON";

            // ultimele 10 rezervari
            lstUltimeleRezervari->Items->Clear();
            const auto& rez = CinemaContext::cinema->GetRezervari().GetToate();
            int start = (int)rez.size() - 10; if (start < 0) start = 0;
            for (int i = (int)rez.size() - 1; i >= start; i--) {
                lstUltimeleRezervari->Items->Add(
                    marshal_as<String^>(rez[i]->ToString()));
            }
        }

        void ActualizeazaRaport() {
            Raport r(CinemaContext::cinema);
            txtRaport->Text = marshal_as<String^>(r.RaportComplet())->Replace(L"\n", L"\r\n");
        }

        void SetStatus(String^ s) {
            statusLabel->Text = s;
        }

        // -------------------- HANDLERE FILME --------------------
        void OnAddFilm(Object^ s, EventArgs^ e) {
            try {
                CinemaContext::auth->VerificaDrepturiFilme();
                DialogFilm^ d = gcnew DialogFilm(nullptr);
                if (d->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    ActualizeazaToate();
                    SetStatus(L"Film adaugat.");
                }
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
        }
        void OnEditFilm(Object^ s, EventArgs^ e) {
            try {
                CinemaContext::auth->VerificaDrepturiFilme();
                if (dgvFilme->SelectedRows->Count == 0) {
                    MessageBox::Show(L"Selectati un film.", L"Atentie"); return;
                }
                int idx = dgvFilme->SelectedRows[0]->Index;
                Film* f = CinemaContext::cinema->GetFilme()[idx];
                DialogFilm^ d = gcnew DialogFilm(f);
                if (d->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    ActualizeazaToate();
                    SetStatus(L"Film modificat.");
                }
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
        }
        void OnDelFilm(Object^ s, EventArgs^ e) {
            try {
                CinemaContext::auth->VerificaDrepturiFilme();
                if (dgvFilme->SelectedRows->Count == 0) {
                    MessageBox::Show(L"Selectati un film.", L"Atentie"); return;
                }
                int idx = dgvFilme->SelectedRows[0]->Index;
                Film* f = CinemaContext::cinema->GetFilme()[idx];
                if (MessageBox::Show(
                    String::Format(L"Stergeti filmul \"{0}\"? (vor fi sterse si spectacolele asociate)",
                        marshal_as<String^>(f->GetTitlu())),
                    L"Confirmare", MessageBoxButtons::YesNo, MessageBoxIcon::Question)
                    == System::Windows::Forms::DialogResult::Yes) {
                    CinemaContext::cinema->StergeFilm(f->GetID());
                    ActualizeazaToate();
                    SetStatus(L"Film sters.");
                }
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
        }

        // -------------------- HANDLERE SALI --------------------
        void OnAddSala(Object^ s, EventArgs^ e) {
            try {
                CinemaContext::auth->VerificaDrepturiSali();
                DialogSala^ d = gcnew DialogSala();
                if (d->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    ActualizeazaToate();
                    SetStatus(L"Sala adaugata.");
                }
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
        }
        void OnDelSala(Object^ s, EventArgs^ e) {
            try {
                CinemaContext::auth->VerificaDrepturiSali();
                if (dgvSali->SelectedRows->Count == 0) {
                    MessageBox::Show(L"Selectati o sala.", L"Atentie"); return;
                }
                int idx = dgvSali->SelectedRows[0]->Index;
                Sala* sa = CinemaContext::cinema->GetSali()[idx];
                if (MessageBox::Show(
                    String::Format(L"Stergeti sala \"{0}\"? (vor fi sterse si spectacolele)",
                        marshal_as<String^>(sa->GetNumarSala())),
                    L"Confirmare", MessageBoxButtons::YesNo, MessageBoxIcon::Question)
                    == System::Windows::Forms::DialogResult::Yes) {
                    CinemaContext::cinema->StergeSala(sa->GetID());
                    ActualizeazaToate();
                    SetStatus(L"Sala stearsa.");
                }
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
        }

        // -------------------- HANDLERE SPECTACOLE --------------------
        void OnAddSpectacol(Object^ s, EventArgs^ e) {
            try {
                CinemaContext::auth->VerificaDrepturiSpectacole();
                if (CinemaContext::cinema->GetFilme().Vid() ||
                    CinemaContext::cinema->GetSali().Vid()) {
                    MessageBox::Show(L"Trebuie sa existe cel putin un film si o sala.",
                        L"Atentie"); return;
                }
                DialogSpectacol^ d = gcnew DialogSpectacol();
                if (d->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    ActualizeazaToate();
                    SetStatus(L"Spectacol adaugat.");
                }
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
        }
        void OnRezerva(Object^ s, EventArgs^ e) {
            if (dgvSpectacole->SelectedRows->Count == 0) {
                MessageBox::Show(L"Selectati un spectacol.", L"Atentie"); return;
            }
            int idx = dgvSpectacole->SelectedRows[0]->Index;
            Spectacol* sp = CinemaContext::cinema->GetSpectacole()[idx];
            DialogRezervare^ d = gcnew DialogRezervare(sp);
            if (d->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                ActualizeazaToate();
                SetStatus(L"Rezervare facuta.");
            }
        }
        void OnVeziHarta(Object^ s, EventArgs^ e) {
            if (dgvSpectacole->SelectedRows->Count == 0) return;
            int idx = dgvSpectacole->SelectedRows[0]->Index;
            Spectacol* sp = CinemaContext::cinema->GetSpectacole()[idx];

            // construieste o forma simpla cu harta vizuala (read-only)
            Form^ f = gcnew Form();
            f->Text = String::Format(L"Harta spectacolului #{0}", sp->GetID());
            f->Size = System::Drawing::Size(700, 600);
            f->StartPosition = FormStartPosition::CenterParent;
            f->BackColor = Color::White;

            Label^ ecran = gcnew Label();
            ecran->Text = L"ECRAN";
            ecran->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            ecran->Location = Point(40, 15);
            ecran->Size = System::Drawing::Size(600, 30);
            ecran->TextAlign = ContentAlignment::MiddleCenter;
            ecran->BackColor = Color::FromArgb(50, 50, 80);
            ecran->ForeColor = Color::White;
            f->Controls->Add(ecran);

            Panel^ p = gcnew Panel();
            p->Location = Point(15, 55);
            p->Size = System::Drawing::Size(650, 490);
            p->AutoScroll = true;
            f->Controls->Add(p);

            int latime = 36, inaltime = 32, gap = 4;
            for (int i = 0; i < sp->GetRanduri(); i++) {
                Label^ l = gcnew Label();
                l->Text = (i + 1).ToString();
                l->Location = Point(5, 8 + i * (inaltime + gap));
                l->Size = System::Drawing::Size(25, inaltime);
                l->TextAlign = ContentAlignment::MiddleRight;
                p->Controls->Add(l);
            }
            for (int i = 0; i < sp->GetRanduri(); i++) {
                for (int j = 0; j < sp->GetColoane(); j++) {
                    Label^ b = gcnew Label();
                    b->Location = Point(35 + j * (latime + gap), 8 + i * (inaltime + gap));
                    b->Size = System::Drawing::Size(latime, inaltime);
                    b->TextAlign = ContentAlignment::MiddleCenter;
                    b->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                    b->Font = gcnew Drawing::Font(L"Segoe UI", 8);
                    if (!sp->LocLiber(i, j)) {
                        b->BackColor = Color::FromArgb(180, 70, 70);
                        b->ForeColor = Color::White;
                        b->Text = L"X";
                    }
                    else if (sp->EsteVIP(i)) {
                        b->BackColor = Color::FromArgb(210, 175, 90);
                        b->ForeColor = Color::White;
                        b->Text = L"V";
                    }
                    else {
                        b->BackColor = Color::FromArgb(120, 180, 120);
                        b->ForeColor = Color::White;
                        b->Text = L"L";
                    }
                    p->Controls->Add(b);
                }
            }
            f->ShowDialog();
        }
        void OnDelSpectacol(Object^ s, EventArgs^ e) {
            try {
                CinemaContext::auth->VerificaDrepturiSpectacole();
                if (dgvSpectacole->SelectedRows->Count == 0) return;
                int idx = dgvSpectacole->SelectedRows[0]->Index;
                Spectacol* sp = CinemaContext::cinema->GetSpectacole()[idx];
                if (MessageBox::Show(L"Stergeti spectacolul si rezervarile asociate?",
                    L"Confirmare", MessageBoxButtons::YesNo)
                    == System::Windows::Forms::DialogResult::Yes) {
                    CinemaContext::cinema->StergeSpectacol(sp->GetID());
                    ActualizeazaToate();
                    SetStatus(L"Spectacol sters.");
                }
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
        }

        // -------------------- HANDLERE REZERVARI --------------------
        void OnDelRezervare(Object^ s, EventArgs^ e) {
            if (dgvRezervari->SelectedRows->Count == 0) {
                MessageBox::Show(L"Selectati o rezervare.", L"Atentie"); return;
            }
            int idx = dgvRezervari->SelectedRows[0]->Index;
            Rezervare* r = CinemaContext::cinema->GetRezervari()[idx];
            if (MessageBox::Show(
                String::Format(L"Anulati biletul #{0} pentru {1}?",
                    r->GetID(), marshal_as<String^>(r->GetNumeClient())),
                L"Confirmare", MessageBoxButtons::YesNo)
                == System::Windows::Forms::DialogResult::Yes) {
                CinemaContext::cinema->StergeRezervare(r->GetID());
                ActualizeazaToate();
                SetStatus(L"Rezervare anulata.");
            }
        }

        // -------------------- HANDLERE STATISTICI --------------------
        void OnRefreshRaport(Object^ s, EventArgs^ e) {
            ActualizeazaRaport();
            SetStatus(L"Raport recalculat.");
        }
        void OnExportRaport(Object^ s, EventArgs^ e) {
            SaveFileDialog^ sfd = gcnew SaveFileDialog();
            sfd->Filter = L"Fisier text (*.txt)|*.txt";
            sfd->FileName = L"raport_cinema.txt";
            if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                System::IO::File::WriteAllText(sfd->FileName, txtRaport->Text);
                SetStatus(L"Raport exportat.");
                MessageBox::Show(L"Raport exportat cu succes.", L"OK");
            }
        }

        // -------------------- HANDLERE ANGAJATI --------------------
        void OnAddAngajat(Object^ s, EventArgs^ e) {
            try {
                CinemaContext::auth->VerificaDrepturiAngajati();
                DialogAngajat^ d = gcnew DialogAngajat();
                if (d->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    ActualizeazaAngajati();
                    ActualizeazaDashboard();
                    SetStatus(L"Angajat adaugat.");
                }
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
        }
        void OnDelAngajat(Object^ s, EventArgs^ e) {
            try {
                CinemaContext::auth->VerificaDrepturiAngajati();
                if (dgvAngajati->SelectedRows->Count == 0) {
                    MessageBox::Show(L"Selectati un angajat.", L"Atentie"); return;
                }
                int idx = dgvAngajati->SelectedRows[0]->Index;
                Angajat* a = CinemaContext::auth->GetAngajati()[idx];
                if (a == CinemaContext::auth->GetUtilizatorCurent()) {
                    MessageBox::Show(L"Nu va puteti concedia singur :)", L"Eroare");
                    return;
                }
                if (MessageBox::Show(
                    String::Format(L"Concediati pe {0}?", marshal_as<String^>(a->GetNume())),
                    L"Confirmare", MessageBoxButtons::YesNo)
                    == System::Windows::Forms::DialogResult::Yes) {
                    CinemaContext::auth->StergeAngajat(a->GetID());
                    ActualizeazaAngajati();
                    ActualizeazaDashboard();
                    SetStatus(L"Angajat concediat.");
                }
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
        }

        // -------------------- SALVARE/INCARCARE --------------------
        void OnSalveaza(Object^ s, EventArgs^ e) {
            try {
                Persistenta::SalveazaTot(*CinemaContext::cinema, *CinemaContext::auth,
                    CinemaContext::folderDate);
                MessageBox::Show(L"Datele au fost salvate cu succes in folderul \"date\".",
                    L"OK", MessageBoxButtons::OK, MessageBoxIcon::Information);
                SetStatus(L"Date salvate.");
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
            catch (std::exception& ex) {
                MessageBox::Show(marshal_as<String^>(std::string(ex.what())), L"Eroare");
            }
        }
        void OnIncarca(Object^ s, EventArgs^ e) {
            if (MessageBox::Show(
                L"Datele curente vor fi inlocuite cu cele din fisier. Continuati?",
                L"Confirmare", MessageBoxButtons::YesNo, MessageBoxIcon::Warning)
                != System::Windows::Forms::DialogResult::Yes) return;
            try {
                CinemaContext::cinema->Clear();
                Persistenta::IncarcaTot(*CinemaContext::cinema, *CinemaContext::auth,
                    CinemaContext::folderDate);
                ActualizeazaToate();
                SetStatus(L"Date incarcate.");
                MessageBox::Show(L"Datele au fost incarcate cu succes.", L"OK");
            }
            catch (CinemaException& ex) {
                MessageBox::Show(marshal_as<String^>(ex.GetMesaj()), L"Eroare");
            }
            catch (std::exception& ex) {
                MessageBox::Show(marshal_as<String^>(std::string(ex.what())), L"Eroare");
            }
        }
        void OnLogout(Object^ s, EventArgs^ e) {
            if (MessageBox::Show(L"Salvati datele inainte de iesire?",
                L"Iesire", MessageBoxButtons::YesNoCancel, MessageBoxIcon::Question)
                == System::Windows::Forms::DialogResult::Yes) {
                OnSalveaza(nullptr, nullptr);
            }
            CinemaContext::auth->Logout();
            this->Close();
        }
    };
}
