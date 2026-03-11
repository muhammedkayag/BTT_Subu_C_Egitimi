# Windows Üzerinde C Programlama Ortamı Kurulumu

**VSCode + MSYS2 + GCC + Ninja + CMake**

Bu doküman Windows üzerinde **C programlama dili geliştirme ortamını** kurmak için gerekli adımları anlatır.

Kullanılan araçlar:

* VSCode (Kod editörü)
* MSYS2 (Unix benzeri geliştirme ortamı)
* GCC (C derleyicisi)
* Ninja (Build sistemi)
* CMake (Build konfigürasyon aracı)

---

# 1. VSCode Kurulumu

VSCode'u aşağıdaki linkten indirip kurun.

```
https://code.visualstudio.com/sha/download?build=stable&os=win32-x64-user
```

Kurulum tamamlandıktan sonra VSCode'u açabilirsiniz.

---

# 2. MSYS2 Kurulumu

MSYS2 kurulum dosyasını indirin.

```
https://github.com/msys2/msys2-installer/releases/download/2025-12-13/msys2-x86_64-20251213.exe
```

Kurulum tamamlandıktan sonra **MSYS2 terminalini açın** ve aşağıdaki komutu çalıştırın.

```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-cmake
```

Bu komut aşağıdaki araçları kuracaktır:

* GCC
* Ninja
* CMake
* Gerekli geliştirme araçları

---

# 3. Windows PATH Ayarı

Windows **Sistem Ortam Değişkenleri (Environment Variables)** bölümüne aşağıdaki path eklenmelidir.

```
C:\msys64\ucrt64\bin
```

Bu ayar sayesinde aşağıdaki komutlar her terminalden çalışabilir:

* `gcc`
* `cmake`
* `ninja`

---

# 4. Kurulumu Doğrulama

**CMD (Komut İstemi)** açın ve aşağıdaki komutları çalıştırın.

```bash
gcc --version
```

```bash
ninja --version
```

```bash
cmake --version
```

Eğer sürüm bilgileri görünüyorsa kurulum başarılıdır.

---

# 5. VSCode Ayarları

VSCode içerisinde CMake ile çalışabilmek için bazı ayarlar yapılmalıdır.

---

## Kit Seçimi

Komut paletini açın:

```
Ctrl + Shift + P
```

Ardından şu komutu seçin:

```
CMake: Select a Kit
```

Listeden şu seçeneği seçin:

```
GCC 13.2.0 (UCRT64)
```

---

## Build Variant Seçimi

Tekrar komut paletini açın:

```
Ctrl + Shift + P
```

Şu komutu seçin:

```
CMake: Select Variant
```

Ardından:

```
Debug
```

seçeneğini seçin.

---

## Configure İşlemi

Tekrar komut paletini açın:

```
Ctrl + Shift + P
```

Şu komutu çalıştırın:

```
CMake: Configure
```

Bu işlem proje yapılandırmasını oluşturur.

---

# 6. Örnek Program

## main.c

```c
#include <stdio.h>

int main() {
    printf("Selam Ninja!");
    return 0;
}
```

---

## CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(SelamNinja C)

add_executable(app main.c)
```

---

# 7. Build

Tüm ayarlar tamamlandıktan sonra VSCode'un **sol alt köşesinde bulunan build butonu** aktif olacaktır.

Bu buton kullanılarak proje derlenebilir.
