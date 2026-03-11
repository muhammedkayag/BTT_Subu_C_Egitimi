#include <stdio.h>      // Standart giriş/çıkış operasyonları için dahil edildi.
#include <windows.h>    // Windows çekirdek fonksiyonları ve veri tipleri için dahil edildi.
#include <psapi.h>      // Süreç (Process) durum bilgilerini almak için kullanılan kütüphane.
#include <string.h>     // Bellek kopyalama ve dizgi karşılaştırma işlemleri için dahil edildi.
#include <ctype.h>      // Karakter dönüşüm (tolower) işlemleri için dahil edildi.

// Fonksiyon: Karakter dizisindeki tüm harfleri küçük harfe dönüştürür.
// Parametreler: str (karakter dizisi), uzunluk (dizinin uzunluğu).
void kucukHarfYap(char str[], int uzunluk) {
    for (int i = 0; i < uzunluk; i++) {
        str[i] = tolower(str[i]); // Her bir karakteri küçük harfe çevirir.
    }
}

// Fonksiyon: Belirli bir isim veya kelimeye göre sistemdeki süreçleri arar.
void islemAra(char arananIsim[]) {
    DWORD processes[2048]; // Sistemdeki süreç kimliklerini (PID) tutacak dizi.
    DWORD cbNeeded;        // EnumProcesses fonksiyonundan dönen toplam bayt miktarı.
    DWORD cProcesses;      // Hesaplanan toplam süreç sayısı.
    char arananAlt[100];   // Arama teriminin küçük harf kopyası.

    // Arama işlemini büyük/küçük harf duyarsız hale getirmek için hazırlık yapılır.
    strcpy(arananAlt, arananIsim);
    kucukHarfYap(arananAlt, strlen(arananAlt));

    // Sistemdeki tüm aktif süreçlerin listesini (PID) alır.
    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) return;

    // Alınan toplam bayt miktarını DWORD boyutuna bölerek süreç sayısını belirler.
    cProcesses = cbNeeded / sizeof(DWORD);

    printf("\n--- ARAMA SONUCLARI (%s) ---\n", arananIsim);
    printf("%-10s %-30s\n", "PID", "Islem Adi");
    printf("---------- ------------------------------\n");

    for (unsigned int i = 0; i < cProcesses; i++) {
        if (processes[i] == 0) continue; // Geçersiz veya boş PID'leri atlar.

        char processName[MAX_PATH];
        strcpy(processName, "<Erisim Yok>"); // Varsayılan değer atanır.

        // İlgili sürece bilgi okuma yetkisiyle erişim sağlanmaya çalışılır.
        HANDLE hProcess = OpenProcess(
            PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ,
            FALSE,
            processes[i]
        );

        if (hProcess != NULL) {
            HMODULE hMod;
            DWORD cbNeededMod;

            // Sürece ait modül listesini alır; ilk modül genellikle yürütülebilir dosyadır (.exe).
            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededMod)) {
                // Modülün temel ismini (süreç adını) alır ve değişkene yazar.
                GetModuleBaseNameA(hProcess, hMod, processName, sizeof(processName));
            }
            CloseHandle(hProcess); // Süreç tutamacı (handle) serbest bırakılır.
        }

        // Karşılaştırma için bulunan süreç isminin küçük harf kopyası oluşturulur.
        char tempName[MAX_PATH];
        strcpy(tempName, processName);
        kucukHarfYap(tempName, strlen(tempName));

        // Eğer aranan kelime süreç ismi içinde geçiyorsa sonuç ekrana yazdırılır.
        if (strstr(tempName, arananAlt) != NULL) {
            printf("%-10lu %-30s\n", processes[i], processName);
        }
    }
}

// Fonksiyon: Verilen PID değerine sahip süreci sonlandırır.
void islemKapat(DWORD pid) {
    // Süreci sonlandırma (TERMINATE) yetkisiyle açmaya çalışır.
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

    if (hProcess == NULL) {
        // Erişim engellendiyse veya PID mevcut değilse kullanıcı bilgilendirilir.
        printf("\n[HATA] PID %lu acilamadi. Yetki yetersiz veya islem bulunamadi.\n", pid);
        return;
    }

    // TerminateProcess fonksiyonu ile süreci zorla durdurur.
    if (TerminateProcess(hProcess, 0)) {
        printf("\n[BASARILI] PID %lu basariyla kapatildi.\n", pid);
    } else {
        // İşlem başarısız olursa Windows hata kodunu döndürür.
        printf("\n[HATA] PID %lu kapatilamadi. Hata kodu: %lu\n", pid, GetLastError());
    }

    CloseHandle(hProcess); // Kaynak sızıntısını önlemek için handle kapatılır.
}

int main() {
    int secim;
    char arananIsim[100];
    DWORD pid;

    printf("=== WINDOWS ISLEM YONETICISI ===\n");

    // Kullanıcı çıkış yapana kadar devam eden ana döngü.
    while (1) {
        printf("\n1 - Islem Ara\n");
        printf("2 - Islemi Kapat (PID ile)\n");
        printf("3 - Cikis\n");
        printf("Seciminiz: ");
        scanf("%d", &secim);

        if (secim == 1) {
            printf("Aramak istediginiz islemi girin: ");
            scanf("%99s", arananIsim);
            islemAra(arananIsim);

        } else if (secim == 2) {
            printf("Kapatmak istediginiz PID'i girin: ");
            scanf("%lu", &pid);
            islemKapat(pid);

        } else if (secim == 3) {
            printf("Cikiliyor...\n");
            break; // Döngüden çıkarak programı sonlandırır.

        } else {
            printf("[!] Gecersiz secim. Tekrar deneyin.\n");
        }
    }

    return 0;
}
