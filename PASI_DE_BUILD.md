# Pasi pentru a face proiectul sa compileze - URGENT

## Probleme rezolvate in aceasta sesiune
1. **95 erori "byte is ambiguous"** - rezolvate cu `_HAS_STD_BYTE=0` in vcxproj
2. **MSB8020 v143 build tools** - rezolvat cu setarea corecta v143
3. **Typo `#include <iostream>=`** - corectat
4. **AppContext nepotrivit** - revert la versiune nativa simpla

## ATENTIE - Inainte sa apesi Build:

### Pasul 1: Inchide complet Visual Studio
Toate fisierele au fost modificate. Inchide complet VS, apoi redeschide proiectul.

### Pasul 2: Cleanup
Dupa redeschidere, fa:
- **Build → Clean Solution**
- Sterge manual folderul `Cinema/x64/` si `Cinema/Cinema/x64/` daca exista
- Sterge folderul `.vs/` (poti restarta VS dupa)

### Pasul 3: Verifica ca ai workload-ul instalat
Daca primesti erori despre `.NET` sau `Windows Forms`:
1. Deschide **Visual Studio Installer**
2. Apasa **Modify** la VS 2022
3. Bifeaza: **.NET desktop development**
4. La dreapta, sub "Installation details", asigura-te ca e bifat:
   - **C++/CLI support for v143 build tools (Latest)**
   - **.NET Framework 4.7.2 targeting pack** (sau 4.8)
5. Apasa **Modify** sa instaleze

### Pasul 4: Selectare config corecta
In bara de sus a VS, alege:
- **Debug** (nu Release)
- **x64** (nu Win32)

### Pasul 5: Build
- Apasa **Build → Build Solution** (Ctrl+Shift+B)
- Asteapta sa termine
- Daca apar erori, citeste primele 3-5 erori (restul sunt cascadate)

### Pasul 6: Rulare
- **Debug → Start Without Debugging** (Ctrl+F5)
- Ar trebui sa apara fereastra de login
- Utilizatori impliciti:
  - `admin` / `admin` (Administrator - toate drepturile)
  - `manager` / `manager`
  - `casier` / `casier`

## Daca apar inca erori dupa cleanup:

### Erori despre "marshal_as" sau msclr
Adauga in `Properties → C/C++ → General → Additional Include Directories`:
```
$(VCInstallDir)include;$(VCInstallDir)atlmfc\include
```

### Erori "cannot open System.dll"
- Right-click pe project → **Properties**
- Sectiunea **Common Properties → Framework and References**
- Adauga manual: `System`, `System.Drawing`, `System.Windows.Forms`

### "MSB4019: project file Microsoft.Cpp.Default.props not found"
- Pune `_WIN32_WINNT=0x0A00` in preprocessor defines (Windows 10)

### "WindowsTargetPlatformVersion not specified"
- Deja e setat in .vcxproj la `10.0` (auto-detect Windows SDK)

## Cum sa apesi "Build" si sa NU vezi erori dupa cleanup:
1. Inchide VS complet
2. Sterge `Cinema/x64/`, `Cinema/Cinema/x64/`, `Cinema/.vs/`
3. Deschide `Cinema.slnx`
4. **Debug | x64**
5. **Build → Build Solution**

## Daca tot sunt erori, fa screenshot la primele 5 erori din Error List
(nu la toate 95) si trimite-mi.
