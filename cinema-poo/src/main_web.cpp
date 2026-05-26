// ============================================================
//  main_web.cpp - Server HTTP simplu pentru interfata web
//
//  Compilare: g++ -std=c++17 -o cinema_web main_web.cpp -lpthread
//  Rulare:    ./cinema_web
//  Acces:     http://localhost:8080
// ============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>
#include <thread>
#include <mutex>
#include <filesystem>

// POSIX sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

namespace fs = std::filesystem;
using namespace std;

static const string DATA_FOLDER = "date";
static const string WEB_FOLDER  = "web";
static const int    PORT        = 8080;
static mutex        dataMutex;

// ============================================================
//  Utilitare
// ============================================================
string CurrentDateTime() {
    time_t now = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&now));
    return string(buf);
}

string ReadFile(const string& path) {
    ifstream f(path);
    if (!f) return "";
    return string(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
}

int NextReservationID() {
    string path = DATA_FOLDER + "/rezervari.txt";
    ifstream f(path);
    int maxID = 0, id;
    string line;
    while (getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line);
        if (ss >> id) maxID = max(maxID, id);
    }
    return maxID + 1;
}

// Citeste ocuparea locurilor pentru un spectacol din rezervari.txt
// Returneaza set de "rand,col" ocupate
vector<pair<int,int>> GetOccupiedSeats(int spectacolID) {
    vector<pair<int,int>> occupied;
    string path = DATA_FOLDER + "/rezervari.txt";
    ifstream f(path);
    string line;
    while (getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line);
        string tok;
        vector<string> fields;
        while (getline(ss, tok, '|')) fields.push_back(tok);
        if (fields.size() >= 9) {
            int spID = stoi(fields[1]);
            if (spID == spectacolID) {
                int r = stoi(fields[7]);
                int c = stoi(fields[8]);
                occupied.push_back({r, c});
            }
        }
    }
    return occupied;
}

// ============================================================
//  URL decode
// ============================================================
string UrlDecode(const string& s) {
    string result;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '+') { result += ' '; }
        else if (s[i] == '%' && i + 2 < s.size()) {
            int v;
            sscanf(s.substr(i+1,2).c_str(), "%x", &v);
            result += (char)v;
            i += 2;
        } else result += s[i];
    }
    return result;
}

// Parseaza form data: "key1=val1&key2=val2"
map<string,string> ParseForm(const string& body) {
    map<string,string> params;
    istringstream ss(body);
    string pair;
    while (getline(ss, pair, '&')) {
        auto eq = pair.find('=');
        if (eq != string::npos) {
            string k = UrlDecode(pair.substr(0, eq));
            string v = UrlDecode(pair.substr(eq + 1));
            params[k] = v;
        }
    }
    return params;
}

// Parseaza JSON simplu: {"key":"val", "key2":123}
map<string,string> ParseJSON(const string& body) {
    map<string,string> params;
    string s = body;
    // elimina { }
    s.erase(remove(s.begin(), s.end(), '{'), s.end());
    s.erase(remove(s.begin(), s.end(), '}'), s.end());
    // split dupa virgula
    istringstream ss(s);
    string token;
    while (getline(ss, token, ',')) {
        auto col = token.find(':');
        if (col == string::npos) continue;
        string key = token.substr(0, col);
        string val = token.substr(col + 1);
        // elimina ghilimele si spatii
        auto trim = [](string& t) {
            t.erase(remove(t.begin(), t.end(), '"'), t.end());
            t.erase(remove(t.begin(), t.end(), ' '), t.end());
            t.erase(remove(t.begin(), t.end(), '\n'), t.end());
            t.erase(remove(t.begin(), t.end(), '\r'), t.end());
        };
        trim(key); trim(val);
        params[key] = val;
    }
    return params;
}

// ============================================================
//  MIME types
// ============================================================
bool EndsWith(const string& s, const string& suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

string MimeType(const string& path) {
    if (EndsWith(path, ".html")) return "text/html; charset=utf-8";
    if (EndsWith(path, ".js"))   return "application/javascript; charset=utf-8";
    if (EndsWith(path, ".css"))  return "text/css; charset=utf-8";
    if (EndsWith(path, ".json")) return "application/json; charset=utf-8";
    if (EndsWith(path, ".ico"))  return "image/x-icon";
    return "text/plain; charset=utf-8";
}

// ============================================================
//  Raspunsuri HTTP
// ============================================================
string HttpResponse(int code, const string& mime, const string& body,
                    const string& extraHeaders = "") {
    string status;
    if (code == 200) status = "200 OK";
    else if (code == 201) status = "201 Created";
    else if (code == 400) status = "400 Bad Request";
    else if (code == 404) status = "404 Not Found";
    else status = "500 Internal Server Error";

    ostringstream r;
    r << "HTTP/1.1 " << status << "\r\n";
    r << "Content-Type: " << mime << "\r\n";
    r << "Content-Length: " << body.size() << "\r\n";
    r << "Access-Control-Allow-Origin: *\r\n";
    r << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
    r << "Access-Control-Allow-Headers: Content-Type\r\n";
    r << "Connection: close\r\n";
    if (!extraHeaders.empty()) r << extraHeaders;
    r << "\r\n" << body;
    return r.str();
}

// ============================================================
//  API: POST /api/rezervare
//  Body JSON: { "spectacolID":1, "rand":0, "coloana":2,
//               "nume":"Ion", "varsta":25, "tip":"Adult", "email":"..." }
// ============================================================
string HandleRezervare(const string& body) {
    auto p = ParseJSON(body);

    string spectacolID = p.count("spectacolID") ? p["spectacolID"] : "";
    string rand_s      = p.count("rand")        ? p["rand"]        : "";
    string col_s       = p.count("coloana")     ? p["coloana"]     : "";
    string nume        = p.count("nume")        ? p["nume"]        : "Anonim";
    string varsta      = p.count("varsta")      ? p["varsta"]      : "0";
    string tip         = p.count("tip")         ? p["tip"]         : "Adult";
    string email       = p.count("email")       ? p["email"]       : "";

    if (spectacolID.empty() || rand_s.empty() || col_s.empty()) {
        return HttpResponse(400, "application/json",
            R"({"success":false,"error":"Date incomplete"})");
    }

    lock_guard<mutex> lock(dataMutex);

    // Verifica daca locul e deja ocupat
    int spID = stoi(spectacolID);
    int r    = stoi(rand_s);
    int c    = stoi(col_s);
    auto occupied = GetOccupiedSeats(spID);
    for (auto& seat : occupied) {
        if (seat.first == r && seat.second == c) {
            return HttpResponse(400, "application/json",
                R"({"success":false,"error":"Loc deja ocupat"})");
        }
    }

    // Adauga rezervarea
    int newID = NextReservationID();
    string path = DATA_FOLDER + "/rezervari.txt";
    ofstream f(path, ios::app);
    if (!f) {
        return HttpResponse(500, "application/json",
            R"({"success":false,"error":"Eroare scriere fisier"})");
    }
    // format: id|spectacolID|tip|nume|varsta|email|telefon|rand|coloana|dataEmiterii
    f << newID << "|" << spectacolID << "|" << tip << "|" << nume
      << "|" << varsta << "|" << email << "||"
      << rand_s << "|" << col_s << "|" << CurrentDateTime() << "\n";
    f.close();

    ostringstream resp;
    resp << R"({"success":true,"id":)" << newID
         << R"(,"mesaj":"Rezervare #)" << newID
         << R"( confirmata! Rand )" << (r+1)
         << R"(, Loc )" << (c+1) << R"("})";
    return HttpResponse(201, "application/json", resp.str());
}

// ============================================================
//  API: GET /api/locuri-ocupate?spectacolID=X
// ============================================================
string HandleLocuriOcupate(const string& query) {
    int spID = 1;
    auto pos = query.find("spectacolID=");
    if (pos != string::npos) spID = stoi(query.substr(pos + 12));

    lock_guard<mutex> lock(dataMutex);
    auto occupied = GetOccupiedSeats(spID);

    ostringstream json;
    json << "[";
    for (size_t i = 0; i < occupied.size(); i++) {
        if (i > 0) json << ",";
        json << "{\"rand\":" << occupied[i].first
             << ",\"coloana\":" << occupied[i].second << "}";
    }
    json << "]";
    return HttpResponse(200, "application/json", json.str());
}

// ============================================================
//  Servire fisiere statice
// ============================================================
// API: GET /api/rezervari - returneaza toate rezervarile din fisier
string HandleGetRezervari() {
    lock_guard<mutex> lock(dataMutex);
    string path = DATA_FOLDER + "/rezervari.txt";
    ifstream f(path);
    string line;
    ostringstream json;
    json << "[";
    bool first = true;
    while (getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line);
        string tok;
        vector<string> fields;
        while (getline(ss, tok, '|')) fields.push_back(tok);
        if (fields.size() < 10) continue;
        if (!first) json << ",";
        first = false;
        // Escape ghilimele din nume
        string nume = fields[3];
        replace(nume.begin(), nume.end(), '"', '\'');
        json << "{"
             << "\"id\":"          << fields[0] << ","
             << "\"spectacolID\":" << fields[1] << ","
             << "\"tip\":\""       << fields[2] << "\","
             << "\"nume\":\""      << nume       << "\","
             << "\"varsta\":"      << (fields[4].empty() ? "0" : fields[4]) << ","
             << "\"email\":\""     << fields[5] << "\","
             << "\"rand\":"        << fields[7] << ","
             << "\"coloana\":"     << fields[8] << ","
             << "\"data\":\""      << fields[9] << "\","
             << "\"online\":"      << (!fields[5].empty() ? "true" : "false")
             << "}";
    }
    json << "]";
    return HttpResponse(200, "application/json", json.str());
}

// ============================================================
//  Sesiuni in-memory: token -> {username, rol}
// ============================================================
struct Sesiune { string username; string rol; string nume; };
static map<string, Sesiune> sesiuni;
static mutex sesiuniMutex;

string GenerateToken() {
    srand((unsigned)time(nullptr) + rand());
    string t;
    const char* chars = "abcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < 32; i++) t += chars[rand() % 36];
    return t;
}

// Citeste angajatii din angajati.txt
// format: id|rol|nume|username|parola|salariu
bool VerificaCredentiale(const string& username, const string& parola,
                          string& rolOut, string& numeOut) {
    string path = DATA_FOLDER + "/angajati.txt";
    ifstream f(path);
    string line;
    while (getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line);
        string tok; vector<string> fields;
        while (getline(ss, tok, '|')) fields.push_back(tok);
        if (fields.size() >= 5) {
            if (fields[3] == username && fields[4] == parola) {
                rolOut  = fields[1];
                numeOut = fields[2];
                return true;
            }
        }
    }
    return false;
}

// POST /api/login  body: {"username":"...","password":"..."}
string HandleLogin(const string& body) {
    auto p = ParseJSON(body);
    string username = p.count("username") ? p["username"] : "";
    string password = p.count("password") ? p["password"] : "";
    string rol, nume;
    if (username.empty() || !VerificaCredentiale(username, password, rol, nume)) {
        return HttpResponse(401, "application/json",
            R"({"success":false,"error":"Username sau parola gresita"})");
    }
    string token = GenerateToken();
    { lock_guard<mutex> lk(sesiuniMutex); sesiuni[token] = {username, rol, nume}; }
    ostringstream resp;
    resp << R"({"success":true,"token":")" << token
         << R"(","rol":")" << rol
         << R"(","nume":")" << nume << R"("})";
    return HttpResponse(200, "application/json", resp.str());
}

// GET /api/logout?token=...
string HandleLogout(const string& query) {
    auto pos = query.find("token=");
    if (pos != string::npos) {
        string token = query.substr(pos + 6);
        lock_guard<mutex> lk(sesiuniMutex);
        sesiuni.erase(token);
    }
    return HttpResponse(200, "application/json", R"({"success":true})");
}

// Verifica token si returneaza sesiunea (sau nullptr)
Sesiune* GetSesiune(const string& request) {
    // Cauta "Authorization: Bearer <token>" sau "X-Token: <token>"
    string token;
    auto pos = request.find("X-Token: ");
    if (pos != string::npos) {
        auto end = request.find("\r\n", pos);
        token = request.substr(pos + 9, end - pos - 9);
    }
    if (token.empty()) return nullptr;
    lock_guard<mutex> lk(sesiuniMutex);
    auto it = sesiuni.find(token);
    return (it != sesiuni.end()) ? &it->second : nullptr;
}

// POST /api/film  body JSON cu datele filmului (necesita Admin/Manager)
string HandleAddFilm(const string& body, const string& request) {
    auto* ses = GetSesiune(request);
    if (!ses || (ses->rol != "Admin" && ses->rol != "Manager"))
        return HttpResponse(403, "application/json", R"({"success":false,"error":"Acces interzis"})");

    auto p = ParseJSON(body);
    string titlu    = p.count("titlu")    ? p["titlu"]    : "";
    string categorie= p.count("categorie")? p["categorie"]: "Drama";
    string durata   = p.count("durata")   ? p["durata"]   : "90";
    string varsta   = p.count("varsta")   ? p["varsta"]   : "0";
    string regizor  = p.count("regizor")  ? p["regizor"]  : "";
    string an       = p.count("an")       ? p["an"]       : "2024";
    string rating   = p.count("rating")   ? p["rating"]   : "7.0";
    string tip      = p.count("tip")      ? p["tip"]      : "2D";

    if (titlu.empty())
        return HttpResponse(400, "application/json", R"({"success":false,"error":"Titlu lipsa"})");

    lock_guard<mutex> lock(dataMutex);
    string path = DATA_FOLDER + "/filme.txt";

    // Gaseste max ID
    int maxID = 0;
    { ifstream f(path); string line, tok; vector<string> fields;
      while (getline(f, line)) {
          if (line.empty() || line[0]=='#') continue;
          fields.clear(); istringstream ss(line);
          while (getline(ss, tok, '|')) fields.push_back(tok);
          if (!fields.empty()) maxID = max(maxID, stoi(fields[0]));
      }
    }
    int newID = maxID + 1;
    ofstream fw(path, ios::app);
    // id|titlu|categorie|durata|varstaMin|limba|regizor|an|rating|poster|tip
    fw << newID << "|" << titlu << "|" << categorie << "|" << durata
       << "|" << varsta << "|Romana|" << regizor << "|" << an
       << "|" << rating << "||" << tip << "\n";

    ostringstream resp;
    resp << R"({"success":true,"id":)" << newID
         << R"(,"mesaj":"Film adaugat!"})";
    return HttpResponse(201, "application/json", resp.str());
}

// POST /api/spectacol
string HandleAddSpectacol(const string& body, const string& request) {
    auto* ses = GetSesiune(request);
    if (!ses || (ses->rol != "Admin" && ses->rol != "Manager"))
        return HttpResponse(403, "application/json", R"({"success":false,"error":"Acces interzis"})");
    auto p = ParseJSON(body);
    string filmID = p.count("filmID") ? p["filmID"] : "";
    string salaID = p.count("salaID") ? p["salaID"] : "";
    string data   = p.count("data")   ? p["data"]   : "";
    string ora    = p.count("ora")    ? p["ora"]    : "";
    string pret   = p.count("pret")   ? p["pret"]   : "35";
    if (filmID.empty() || salaID.empty() || data.empty() || ora.empty())
        return HttpResponse(400, "application/json", R"({"success":false,"error":"Date incomplete"})");
    lock_guard<mutex> lock(dataMutex);
    string path = DATA_FOLDER + "/spectacole.txt";
    int maxID = 0;
    { ifstream f(path); string line, tok; vector<string> fields;
      while (getline(f, line)) {
          if (line.empty() || line[0]=='#') continue;
          fields.clear(); istringstream ss(line);
          while (getline(ss, tok, '|')) fields.push_back(tok);
          if (!fields.empty()) maxID = max(maxID, stoi(fields[0]));
      }
    }
    int newID = maxID + 1;
    ofstream fw(path, ios::app);
    fw << newID << "|" << filmID << "|" << salaID << "|" << data << "|" << ora << "|" << pret << "\n";
    ostringstream resp;
    resp << R"({"success":true,"id":)" << newID << R"(,"mesaj":"Spectacol adaugat!"})";
    return HttpResponse(201, "application/json", resp.str());
}

// DELETE /api/rezervare?id=X
string HandleDeleteRezervare(const string& query, const string& request) {
    auto* ses = GetSesiune(request);
    if (!ses) return HttpResponse(403, "application/json", R"({"success":false,"error":"Neautentificat"})");
    int delID = 0;
    auto pos = query.find("id=");
    if (pos != string::npos) delID = stoi(query.substr(pos + 3));
    if (delID == 0) return HttpResponse(400, "application/json", R"({"success":false,"error":"ID lipsa"})");
    lock_guard<mutex> lock(dataMutex);
    string path = DATA_FOLDER + "/rezervari.txt";
    ifstream fin(path); vector<string> lines; string line; bool found = false;
    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') { lines.push_back(line); continue; }
        istringstream ss(line); string tok; vector<string> fields;
        while (getline(ss, tok, '|')) fields.push_back(tok);
        if (!fields.empty() && stoi(fields[0]) == delID) { found = true; continue; }
        lines.push_back(line);
    }
    fin.close();
    if (!found) return HttpResponse(404, "application/json", R"({"success":false,"error":"Rezervare negasita"})");
    ofstream fout(path);
    for (auto& l : lines) fout << l << "\n";
    return HttpResponse(200, "application/json", R"({"success":true,"mesaj":"Rezervare stearsa"})");
}

// GET /api/filme-toate
string HandleGetFilme() {
    lock_guard<mutex> lock(dataMutex);
    string path = DATA_FOLDER + "/filme.txt";
    ifstream f(path); string line;
    ostringstream json; json << "["; bool first = true;
    while (getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line); string tok; vector<string> fields;
        while (getline(ss, tok, '|')) fields.push_back(tok);
        if (fields.size() < 9) continue;
        if (!first) json << ","; first = false;
        string titlu = fields[1]; replace(titlu.begin(), titlu.end(), '"', '\'');
        json << "{\"id\":" << fields[0] << ",\"titlu\":\"" << titlu
             << "\",\"categorie\":\"" << fields[2] << "\",\"an\":" << fields[7] << "}";
    }
    json << "]";
    return HttpResponse(200, "application/json", json.str());
}

// GET /api/sali-toate
string HandleGetSali() {
    lock_guard<mutex> lock(dataMutex);
    string path = DATA_FOLDER + "/sali.txt";
    ifstream f(path); string line;
    ostringstream json; json << "["; bool first = true;
    while (getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line); string tok; vector<string> fields;
        while (getline(ss, tok, '|')) fields.push_back(tok);
        if (fields.size() < 6) continue;
        if (!first) json << ","; first = false;
        json << "{\"id\":" << fields[1] << ",\"numar\":\"" << fields[2]
             << "\",\"randuri\":" << fields[4] << ",\"coloane\":" << fields[5] << "}";
    }
    json << "]";
    return HttpResponse(200, "application/json", json.str());
}

// ============================================================
string ServeStatic(const string& urlPath) {
    string filePath;
    if (urlPath == "/" || urlPath == "/index.html")
        filePath = WEB_FOLDER + "/index.html";
    else if (urlPath == "/data.js")
        filePath = WEB_FOLDER + "/data.js";
    else
        filePath = WEB_FOLDER + urlPath;
    string content = ReadFile(filePath);
    if (content.empty())
        return HttpResponse(404, "text/plain", "404 - Fisier negasit: " + urlPath);
    return HttpResponse(200, MimeType(filePath), content);
}

// ============================================================
string HandleRequest(const string& request) {
    istringstream ss(request);
    string method, path, version;
    ss >> method >> path >> version;
    string urlPath = path, query;
    auto qpos = path.find('?');
    if (qpos != string::npos) { urlPath = path.substr(0, qpos); query = path.substr(qpos + 1); }
    if (method == "OPTIONS") return HttpResponse(200, "text/plain", "");
    if (method == "POST" && urlPath == "/api/login") {
        auto bp = request.find("\r\n\r\n");
        return HandleLogin(bp != string::npos ? request.substr(bp+4) : "");
    }
    if (method == "GET" && urlPath == "/api/logout") return HandleLogout(query);
    if (method == "POST" && urlPath == "/api/rezervare") {
        auto bp = request.find("\r\n\r\n");
        return HandleRezervare(bp != string::npos ? request.substr(bp+4) : "");
    }
    if (method == "GET" && urlPath == "/api/locuri-ocupate") return HandleLocuriOcupate(query);
    if (method == "GET" && urlPath == "/api/rezervari") return HandleGetRezervari();
    if (method == "POST" && urlPath == "/api/film") {
        auto bp = request.find("\r\n\r\n");
        return HandleAddFilm(bp != string::npos ? request.substr(bp+4) : "", request);
    }
    if (method == "POST" && urlPath == "/api/spectacol") {
        auto bp = request.find("\r\n\r\n");
        return HandleAddSpectacol(bp != string::npos ? request.substr(bp+4) : "", request);
    }
    if (method == "DELETE" && urlPath == "/api/rezervare") return HandleDeleteRezervare(query, request);
    if (method == "GET" && urlPath == "/api/filme-toate") return HandleGetFilme();
    if (method == "GET" && urlPath == "/api/sali-toate") return HandleGetSali();
    if (method == "GET") return ServeStatic(urlPath);
    return HttpResponse(400, "text/plain", "Metoda necunoscuta");
}

void HandleClient(int clientSock) {
    char buf[16384] = {};
    int bytes = recv(clientSock, buf, sizeof(buf)-1, 0);
    if (bytes > 0) {
        string request(buf, bytes);
        string response = HandleRequest(request);
        send(clientSock, response.c_str(), response.size(), 0);
    }
    close(clientSock);
}

int main() {
    if (!fs::exists(DATA_FOLDER)) { cerr << "EROARE: folderul date/ nu exista!\n"; return 1; }
    if (!fs::exists(WEB_FOLDER + "/index.html")) { cerr << "EROARE: web/index.html nu exista!\n"; return 1; }
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) { perror("socket"); return 1; }
    int opt = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(serverSock, (sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }
    listen(serverSock, 16);
    cout << "\n  ╔══════════════════════════════════╗\n";
    cout << "  ║  CinePlex Web Server pornit!     ║\n";
    cout << "  ║  http://localhost:" << PORT << "          ║\n";
    cout << "  ║  Ctrl+C pentru oprire            ║\n";
    cout << "  ╚══════════════════════════════════╝\n\n";
    while (true) {
        sockaddr_in clientAddr{}; socklen_t clientLen = sizeof(clientAddr);
        int clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientLen);
        if (clientSock < 0) continue;
        thread(HandleClient, clientSock).detach();
    }
    close(serverSock);
    return 0;
}
