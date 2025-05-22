#include <iostream>
#include <vector>
#include <string>
#include <fstream> //dosya yazma kütüphanesi
#include <cstdlib> //rand kullanabilmek için
#include <sstream>

class Kullanici {
public:
    std::string kullaniciAdi;
    std::string sifre;
    bool yetkiliMi;

    Kullanici(std::string ad, std::string s, bool yetki) {
        kullaniciAdi = ad;
        sifre = s;
        yetkiliMi = yetki;
    }

    bool girisYap(const std::string& ad, const std::string& s) {
        return (ad == kullaniciAdi && s == sifre);
    }
};

//Sürücü Puanlama algoritması sınıfı
class Surucu {
public:
    std::string ad;
    int puan;

    Surucu(std::string isim) : ad(isim), puan(100) {}  // Başlangıç puanı 100

    void puanGuncelle(int degisim) {
        puan += degisim;
        if (puan > 100) puan = 100;
        if (puan < 0) puan = 0;
    }

    void puaniGoster() {
        std::cout << ad << " surucusunun puani: " << puan << "\n";
    }
};

//Dinamik fiyatlandırma sınıfı
class Arac {
public:
    std::string plaka;
    int fiyat;
    std::string bakimTarihi;

    Arac(std::string p, int f) : plaka(p), fiyat(f), bakimTarihi("01.06.2025") {}

    void fiyatGuncelle(int oran) {
        fiyat += oran;
        if (fiyat < 0) fiyat = 0;
    }

    void yazdir() {
        //std::cout << plaka << " - " << fiyat << " TL/gun - Bakim: " << bakimTarihi << "\n";
        std::cout << plaka << " - " << fiyat << " TL/gun \n";
    }

    void bakimTarihiGuncelle(std::string yeniTarih) {
        bakimTarihi = yeniTarih;
    }
};



//menü fonksiyonları

struct AracKonum {
    std::string plaka;
    std::string konum;
};

// Örnek araç konumları (program içinde ya da dosyadan yüklenebilir)
std::vector<AracKonum> aracKonumlari;

//haritayı açma fonksiyonu/ varsayılan tarayıcısa haritayı açar.
void haritayiAc() {
    system("start https://mustafa544185.github.io/OtonomAracKira/map.html");
}

//araç konum takip fonksiyonu
void konumlariJSONDosyasinaYaz() {
    std::ofstream dosya("konumlar.json");
    if (!dosya) {
        std::cout << "konumlar.json dosyasi acilamadi!\n";
        return;
    }

    dosya << "[\n";
    for (size_t i = 0; i < aracKonumlari.size(); ++i) {
        dosya << "  {\n";
        dosya << "    \"plaka\": \"" << aracKonumlari[i].plaka << "\",\n";
        dosya << "    \"konum\": \"" << aracKonumlari[i].konum << "\"\n";
        dosya << "  }";
        if (i != aracKonumlari.size() - 1) dosya << ",";
        dosya << "\n";
    }
    dosya << "]\n";

    dosya.close();
    std::cout << "Konumlar konumlar.json dosyasina yazildi.\n";
}


void aracKonumTakip(bool yetkiliMi) {
    std::cout << "\n\n======= Arac Konum Takibi =======\n";

    // Eğer daha önce eklenmemişse örnek araçlar eklenir
    if (aracKonumlari.empty()) {
        aracKonumlari.push_back(AracKonum());
        aracKonumlari.back().plaka = "34ABC123";
        aracKonumlari.back().konum = "Rauf Denktas Cd.      Selcuklu";

        aracKonumlari.push_back(AracKonum());
        aracKonumlari.back().plaka = "06DEF456";
        aracKonumlari.back().konum = "Eski Meram Cd.        Meram";

        aracKonumlari.push_back(AracKonum());
        aracKonumlari.back().plaka = "35GHI789";
        aracKonumlari.back().konum = "Sehit Nazim Bey Cd.   Karatay";
        
        aracKonumlari.push_back(AracKonum());
        aracKonumlari.back().plaka = "42ABC123";
        aracKonumlari.back().konum = "Fetih Cd.             Karatay";
        
        aracKonumlari.push_back(AracKonum());
        aracKonumlari.back().plaka = "42DEF456";
        aracKonumlari.back().konum = "Hatip Cd.             Meram";
    }

    // Araçları listele
    std::cout << "\nMevcut Arac Konumlari:\n";
    for (size_t i = 0; i < aracKonumlari.size(); ++i) {
        std::cout << i + 1 << ". Plaka: " << aracKonumlari[i].plaka
                  << " | Konum: " << aracKonumlari[i].konum << "\n";
    }
    if (yetkiliMi) {
    std::cout << "\nKonum guncellemesi yapmak istiyor musunuz? (Evet:1 / Hayir:0): ";
    int secim;
    std::cin >> secim;

    if (secim == 1) {
        std::cout << "Guncellemek istediginiz aracin numarasini giriniz (1-"
                  << aracKonumlari.size() << "): ";
        int index;
        std::cin >> index;

        if (index >= 1 && index <= (int)aracKonumlari.size()) {
            std::cout << "Yeni konum giriniz (ornegin: Selcuklu, Meram, Karatay): ";
            std::string yeniKonum;
            std::cin >> yeniKonum;

            aracKonumlari[index - 1].konum = yeniKonum;
            std::cout << "Konum basariyla guncellendi.\n";
        } else {
            std::cout << "Gecersiz secim!\n";
        }
    } else {
        std::cout << "Konumlar degistirilmedi.\n";
    }
} else {
    std::cout << "\nKonumlari sadece goruntuleyebilirsiniz.\n";
}



    // Dosyaya JSON olarak yaz
    konumlariJSONDosyasinaYaz();
}

std::vector<Surucu> suruculer;

//sürücü puanlama fonksiyonu
void surucuPuanlama() {
    std::cout << "\n\n======= Surucu Puanlama =======\n\n";

    // Örnek sürücüler yoksa ekle
    if (suruculer.empty()) {
        suruculer.push_back(Surucu("Ali"));
        suruculer.push_back(Surucu("ahmet"));
        suruculer.push_back(Surucu("Mehmet"));
    }

    // Sürücü listesi göster
    for (size_t i = 0; i < suruculer.size(); ++i) {
        std::cout << i + 1 << ". " << suruculer[i].ad << " - Puan: " << suruculer[i].puan << "\n";
    }

    int secim;
    std::cout << "Puanlarini guncellemek istediginiz surucuyu secin (0 cikis): ";
    std::cin >> secim;

    if (secim > 0 && secim <= (int)suruculer.size()) {
        int degisim;
        std::cout << "Puan degisimi (pozitif ya da negatif): ";
        std::cin >> degisim;

        suruculer[secim - 1].puanGuncelle(degisim);
        std::cout << "Guncellenen puan: ";
        suruculer[secim - 1].puaniGoster();
    }
    else if (secim == 0) {
        std::cout << "Puanlama iptal edildi.\n";
    }
    else {
        std::cout << "Gecersiz secim!\n";
    }
}

//Sürücü bilgileri kaydetme fonksiyonu
void surucuBilgileriniKaydet() {
    std::ofstream dosya("suruculer.txt");
    if (dosya.is_open()) {
        for (int i = 0; i < suruculer.size(); ++i) {
            dosya << suruculer[i].ad << " " << suruculer[i].puan << "\n";
        }
        dosya.close();
    } else {
        std::cout << "Dosya acilamadi!\n";
    }
}

//sürücü bilgilerini dosyadan okuma fonksiyonu
void surucuBilgileriniYukle() {
    std::ifstream dosya("suruculer.txt");
    std::string isim;
    int puan;
    suruculer.clear();

    while (dosya >> isim >> puan) {
        Surucu s(isim);
        s.puan = puan;
        suruculer.push_back(s);
    }

    dosya.close();
}

//araçlar listesini tutan vektör
std::vector<Arac> araclar;
//---------

//araç fiyatlarını dosyadan okuma fonksiyonu
void fiyatlariYukle() {
    araclar.clear();
    std::ifstream dosya("fiyatlar.txt");
    if (!dosya) return;

    std::string plaka;
    int fiyat;
    while (dosya >> plaka >> fiyat) {
        araclar.push_back(Arac(plaka, fiyat));
    }
    dosya.close();
}

//araç fiyatlarını dosyaya yazma fonksiyonu
void fiyatlariKaydet() {
    std::ofstream dosya("fiyatlar.txt");
    for (size_t i = 0; i < araclar.size(); ++i) {
        dosya << araclar[i].plaka << " " << araclar[i].fiyat << std::endl;
    }
    dosya.close();
}

//Dinamik fiyatlandırma fonksiyonu
void dinamikFiyatlandirma(bool yetkiliMi) {
    std::cout << "\n\n======= Dinamik Fiyatlandirma =======\n";

    fiyatlariYukle(); // dosyadan oku

    if (araclar.empty()) {
        // Eğer dosyada yoksa örnek verilerle başla
        araclar.push_back(Arac("34ABC123", 2200));
        araclar.push_back(Arac("06DEF456", 2700));
        araclar.push_back(Arac("35GHI789", 3000));
        fiyatlariKaydet(); // ilk kez yaz
    }

    std::cout << "\nMevcut araclar ve fiyatlar:\n";
    for (size_t i = 0; i < araclar.size(); ++i) {
        araclar[i].yazdir();
    }

    if (yetkiliMi) {
        std::cout << "\nFiyat guncellemesi yapmak istiyor musunuz? (Evet:1 / Hayir:0): ";
        int guncelle;
        std::cin >> guncelle;

        if (guncelle == 1) {
            std::cout << "\nGuncelleme yapilacak araci secin (1-" << araclar.size() << "): ";
            int aracIndex;
            std::cin >> aracIndex;

            if (aracIndex >= 1 && aracIndex <= (int)araclar.size()) {
                std::cout << "Yeni fiyat giriniz (TL): ";
                int yeniFiyat;
                std::cin >> yeniFiyat;

                araclar[aracIndex - 1].fiyat = yeniFiyat;
                fiyatlariKaydet(); // Değişiklikleri dosyaya yaz

                std::cout << "Fiyat basariyla guncellendi.\n";
            } else {
                std::cout << "Gecersiz arac secimi!\n";
            }
        } else {
            std::cout << "Fiyatlar degistirilmedi.\n";
        }
    } else {
        std::cout << "\nFiyatlari sadece goruntuleyebilirsiniz.\n";
    }

    std::cout << "\nGuncel arac fiyatlari:\n";
    for (size_t i = 0; i < araclar.size(); ++i) {
        araclar[i].yazdir();
    }
}

//müşteri araç kiralama fonksiyonu
void musteriAracKirala() {
    std::cout << "\n=== Arac Kiralama ===\n";
	
	if (araclar.empty()) {
        araclar.push_back(Arac("34ABC123", 2200));
        araclar.push_back(Arac("06DEF456", 2700));
        araclar.push_back(Arac("35GHI789", 3000));
    }
    /*if (araclar.empty()) {
        std::cout << "Su anda hic arac bulunmamaktadir.\n";
        return;
    }*/

    std::cout << "Kiralanabilir araclar:\n";
    for (size_t i = 0; i < araclar.size(); ++i) {
        std::cout << (i + 1) << ". ";
        araclar[i].yazdir();
    }

    std::cout << "\nKiralamak istediginiz aracin numarasini girin (1-" << araclar.size() << "): ";
    int secim;
    std::cin >> secim;

    if (secim >= 1 && secim <= (int)araclar.size()) {
        std::cout << "Arac kiralama islemi basarili!\n";
        std::cout << "Kiralanan Arac: ";
        araclar[secim - 1].yazdir();
    } else {
        std::cout << "Gecersiz secim! Lutfen tekrar deneyin.\n";
    }
}

//kaza risk analizi fonksiyonları
void kazaRiskAnalizi() {
    std::cout << "\n\n======= Kaza Risk Analizi =======\n\n";

    std::string bolgeler[] = {"Meram", "Selcuklu", "Karatay", "Beysehir", "Eregli"};
    std::string riskDuzeyleri[] = {"DUSUK", "ORTA", "YUKSEK", "DUSUK", "ORTA"};

    std::cout << "Bolgeler:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << i + 1 << ". " << bolgeler[i] << "\n";
    }

    std::cout << "Lutfen analiz yapmak istediginiz bolgeyi secin (1-5): \n";
    int secim;
    std::cin >> secim;

    if (secim >= 1 && secim <= 5) {
        std::cout << bolgeler[secim - 1] << " bolgesinde kaza riski: " 
                  << riskDuzeyleri[secim - 1] << "\n";

        if (riskDuzeyleri[secim - 1] == "Yuksek") {
            std::cout << "UYARI: Bu bolgede dikkatli olun! Arac sigortasi onemli olabilir.\n";
        }
    } else {
        std::cout << "Gecersiz secim!\n";
    }
}

struct BakimKaydi {
    std::string plaka;
    std::string tarih;
};

std::vector<BakimKaydi> bakimlar;

// Dosyadan bakım kayıtlarını oku
void bakimKayitlariniYukle() {
    std::ifstream dosya("bakimlar.txt");
    if (!dosya.is_open()) return;

    std::string satir;
    while (std::getline(dosya, satir)) {
        std::istringstream ss(satir);
        BakimKaydi kayit;
        if (std::getline(ss, kayit.plaka, ',') && std::getline(ss, kayit.tarih)) {
            bakimlar.push_back(kayit);
        }
    }

    dosya.close();
}

// Dosyaya bakım kaydını ekle
void bakimKaydiEkleDosyaya(const BakimKaydi& kayit) {
    std::ofstream dosya("bakimlar.txt", std::ios::app); // append mod
    if (dosya.is_open()) {
        dosya << kayit.plaka << "," << kayit.tarih << "\n";
        dosya.close();
    }
}

//araç bakım takvimi fonksiyonu
void bakimTakvimi() {
    std::cout << "\n\n======= Arac Bakim Takvimi =======\n\n";

    if (araclar.empty()) {
        std::cout << "Bakim icin tanimli arac bulunamadi.\n";
        return;
    }

    // Mevcut araclari listele
    std::cout << "\nMevcut Araclar:\n";
    for (size_t i = 0; i < araclar.size(); ++i) {
        std::cout << (i + 1) << ". ";
        araclar[i].yazdir();
    }

    std::cout << "\nBakim eklemek istiyor musunuz? (Evet:1 / Hayir:0): ";
    int cevap;
    std::cin >> cevap;

    if (cevap == 1) {
        std::cout << "Bakim yapilacak araci secin (1-" << araclar.size() << "): ";
        int secim;
        std::cin >> secim;

        if (secim >= 1 && secim <= (int)araclar.size()) {
            std::string tarih;
            std::cout << "Bakim tarihi giriniz (GG/AA/YYYY): ";
            std::cin >> tarih;

            BakimKaydi yeniKayit;
            yeniKayit.plaka = araclar[secim - 1].plaka;
            yeniKayit.tarih = tarih;

            bakimlar.push_back(yeniKayit);

            // Dosyaya da yaz
            std::ofstream dosya("bakimlar.txt", std::ios::app);
            if (dosya.is_open()) {
                dosya << yeniKayit.plaka << " " << yeniKayit.tarih << "\n";
                dosya.close();
                std::cout << "Bakim kaydi dosyaya eklendi.\n";
            } else {
                std::cout << "Dosya acilamadi!\n";
            }

            std::cout << "Bakim kaydi eklendi.\n";
        } else {
            std::cout << "Gecersiz secim!\n";
        }
    }

    std::cout << "\nKayitli Bakimlar:\n";
    for (size_t i = 0; i < bakimlar.size(); ++i) {
        std::cout << bakimlar[i].plaka << " - " << bakimlar[i].tarih << "\n";
    }
}

//YETKİLİ MENÜSÜ
void yetkiliMenusu() {
    int secim;

    do {
        //std::cout << "\n=== Yetkili Menusu ===\n";
        std::cout << "\n==================== Yetkili Menusu ====================\n\n";
        std::cout << "1. ARAC KONUM TAKIBI\n";
        std::cout << "2. SURUCU PUANLAMA ALGORITMASI\n";
        std::cout << "3. DINAMIK FIYATLANDIRMA\n";
        std::cout << "4. KAZA RISK ANALIZI\n";
        std::cout << "5. ARAC BAKIM TAKVIMI\n";
        std::cout << "6. HARITADA GOSTER\n";
        std::cout << "0. Cikis\n";
        std::cout << "Seciminiz: ";
        std::cin >> secim;

        switch (secim) {
            case 1:
                aracKonumTakip(true);
                break;
            case 2:
                surucuPuanlama();
                break;
            case 3:
                dinamikFiyatlandirma(true);
                break;
            case 4:
                kazaRiskAnalizi();
                break;
            case 5:
    			bakimTakvimi();
			    break;
			case 6:
    			haritayiAc();
    			break;
            case 0:
                std::cout << "Cikis yapiliyor...\n";
                break;
            default:
                std::cout << "Gecersiz secim!\n";
        }

    } while (secim != 0);
}

//MÜŞTERİ MENÜSÜ
void musteriMenusu() {
    int secim;
    do {
        std::cout << "\n=== Musteri Menusu =========================\n\n";
        std::cout << "1. ARAC KONUM TAKIP\n";
        std::cout << "2. DINAMIK FIYATLANDIRMA\n";
        std::cout << "3. ARAC KIRALA\n";
        std::cout << "4. KAZA RISK ANALIZI\n";
        std::cout << "5. HARITADA GOSTER\n";
        std::cout << "0. Cikis\n";
        std::cout << "Seciminiz: ";
        std::cin >> secim;

        switch (secim) {
            case 1:
                aracKonumTakip(false);
                break;
            case 2:
                dinamikFiyatlandirma(false);
                break;
            case 3:
                musteriAracKirala();
                break;
            case 4:
                kazaRiskAnalizi();
                break;
            case 5:
    			haritayiAc();
    			break;
            case 0:
                std::cout << "Cikis yapiliyor...\n";
                break;
            default:
                std::cout << "Gecersiz secim!\n";
        }

    } while (secim != 0);
}

int main() {
	
	std::cout << "OTONOM ARAC KIRALAMA SISTEMINE HOSGELDINIZ \n\n";
	std::cout << "GIRIS BILGILERINIZ : \n\n";
	
	//bakım tarihlerini dosyadan yükleme fonksiyonu çağırılıyor.
	bakimKayitlariniYukle();
	
	//sürücü bilgilerini dosyadan yükleme fonksiyonu çağırılıyor.
	surucuBilgileriniYukle();

	//Kullanıcılar için olan vektör
    std::vector<Kullanici> kullanicilar;
    kullanicilar.push_back(Kullanici("admin", "1234", true));
    kullanicilar.push_back(Kullanici("mustafa", "123", false));

    std::string ad, sifre;
    std::cout << "Kullanici Adi : ";
    std::cin >> ad;
    std::cout << "Sifre         : ";
    std::cin >> sifre;

    bool girisBasarili = false;
    bool yetkiliMi = false;

    for (int i = 0; i < kullanicilar.size(); ++i) {
        if (kullanicilar[i].girisYap(ad, sifre)) {
            girisBasarili = true;
            yetkiliMi = kullanicilar[i].yetkiliMi;
            break;
        }
    }

    if (girisBasarili) {
        std::cout << "\nGiris Basarili!\n";
        if (yetkiliMi) {
            std::cout << "Yetkili Girisi Yapildi. (Admin Mod)\n";
            yetkiliMenusu(); // YETKİLİ MENÜSÜNÜ BURDA ÇAĞIRIYORUM.
        } else {
            std::cout << "Musteri Girisi Yapildi.\n";
            musteriMenusu(); // MÜŞTERİ MENÜSÜNÜDE BURDA ÇAĞIRIYORUM.
        }
    } else {
        std::cout << "Hatali kullanici adi veya sifre!\n";
    }

	//sürücü bilgilerini dosyaya kaydetme fonksiyonu çağırılıyor.
	surucuBilgileriniKaydet();


    return 0;
}
