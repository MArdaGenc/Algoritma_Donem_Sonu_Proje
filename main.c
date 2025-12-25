//Kütüphanelerimiz
#include <stdio.h>
#include <string.h>
// Sabitler
#define Kota_siniri_gb 6.0
#define Mb_to_Gb 1024.0
#define Asim_ucreti 0.011719

// struct yapısını burada tanımlıyoruz
struct AdslVerisi {
    char dosyaAy[20];
    int gun;
    float uploadMB;
    float downloadMB;
};

int main() {
    // değişken tanımlamaları
    FILE *dosyaIslem;
    char istenenAy[20];
    float toplamUploadMB = 0.0, toplamDownloadMB = 0.0;
    int kayitBulundu = 0;

    // struct ve pointer tanımlaması
    struct AdslVerisi kayit;          // Verileri tutacak yapı
    struct AdslVerisi *veriptr = &kayit;  // Yapıyı gösteren pointer (Fazladan pointer)

    // kullanıcıdan ay bilgisi alma
    printf("Hangi ayin faturasini istiyorsunuz (Orn: Ocak, Subat): ");
    scanf("%s", istenenAy);

    // Kodun çalıştığı klasördeki adsl dosyasını arar
    dosyaIslem = fopen("adsl.txt", "r");
    //dosyaIslem null ise dosyamız bulunamamıstır ve hata alırız
    if (dosyaIslem == NULL) {
        printf("Hata: 'adsl.txt' dosyasi bulunamadi!\n");
        printf("Lutfen dosyanin programla ayni klasorde oldugundan emin olun.\n");
        return 1;
    }

    // Dosyadan veri okuma döngüsü
    while (fscanf(dosyaIslem, "%s %d %f %f", veriptr->dosyaAy, &veriptr->gun, &veriptr->uploadMB, &veriptr->downloadMB) != EOF) {
        if (strcmp(veriptr->dosyaAy, istenenAy) == 0) {
            // Pointer ile verilere ulaşarak topluyoruz
            toplamUploadMB += veriptr->uploadMB;
            toplamDownloadMB += veriptr->downloadMB;
            //hata bulucumuz
            kayitBulundu = 1;
        }
    }
    fclose(dosyaIslem);

    //Hata kontrol kısmı casus komutumuzla kontrol ediyoruz
    if (kayitBulundu == 0) {
        printf("\nHATA\n");
        printf("Girdiginiz '%s' isminde bir ay kayitlarda bulunamadi.\n", istenenAy);
        printf("Lutfen ay ismini kontrol edip tekrar deneyin (Buyuk/kucuk harfe dikkat edin).\n");
        return 1;
    }

    //Hesap kısmı:mbyi gb ye çevime
    //kota asım cezası hesaplama
    //toplam aşım miktarını hesaplama
    float toplamUploadGB = toplamUploadMB / Mb_to_Gb;
    float toplamDownloadGB = toplamDownloadMB / Mb_to_Gb;
    float kotaAsimUcreti = 0.0;
    if (toplamDownloadGB > Kota_siniri_gb) {
        float asimMiktariMB = toplamDownloadMB - (Kota_siniri_gb * Mb_to_Gb);
        kotaAsimUcreti = asimMiktariMB * Asim_ucreti;
    }

    // Ekrana yazdırma
    printf("\nFATURA CIKTISI\n");
    printf("Fatura Donemi: %s 2012\n", istenenAy);
    printf("Toplam Upload: %.2f GB\n", toplamUploadGB);
    printf("Toplam Download: %.2f GB\n", toplamDownloadGB);
    //kota sınırı aşıldıysa ceza işlemini yazdırma
    if (kotaAsimUcreti > 0)
        printf("Kota Asim Ucreti: %.2f TL\n", kotaAsimUcreti);
    else
        printf("Kota Asim Ucreti: 0 TL\n");
    // Dosyaya yazdırma "w" moduyla açıyoruz dosyaya yazdırma modu
    FILE *fCikis = fopen("fatura.txt", "w");
    //fCikis null değilse komutumuz doğru çalıştı demktir ve dosyaya yazdırma aşaması
    if (fCikis != NULL) {
        fprintf(fCikis, "%s 2012\n", istenenAy);
        fprintf(fCikis, "Toplam\n");
        fprintf(fCikis, "yukleme (upload) :\n");
        fprintf(fCikis, "%.2f GB\n", toplamUploadGB);
        fprintf(fCikis, "Toplam\n");
        fprintf(fCikis, "indirme (download) :\n");
        fprintf(fCikis, "%.2f GB\n", toplamDownloadGB);
        fprintf(fCikis, "Kota asim ucreti :\n");
        //dosyaya aşım yazdırma
        if (kotaAsimUcreti > 0) fprintf(fCikis, "%.2f TL\n", kotaAsimUcreti);
        else fprintf(fCikis, "0 TL\n");
    //dosyayı kapıyoruz program çökmesin diye
        fclose(fCikis);
        printf("Fatura bilgileri 'fatura.txt' dosyasina yazildi.\n");
    } else {
        printf("Hata:fatura.txt olusturulamadi.\n");
    }
    return 0;
}
