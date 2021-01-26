/************************************************************************************************************
**						SAKARYA ÜNÝVERSÝTESÝ
**				BÝLGÝSAYAR VE BÝLÝÞÝM BÝLÝMLERÝ FAKÜLTESÝ
**					BÝLGÝSAYAR MÜHENDÝSLÝÐÝ BÖLÜMÜ
**					 PROGRAMLAMAYA GÝRÝÞÝ DERSÝ
**
**                           PROJE ÖDEVÝ
**
**					ÖÐRENCÝ ADI......: Davud Samed Tombul
**					ÖÐRENCÝ NUMARASI.: B171210007
**					DERS GRUBU.......: 1D
*************************************************************************************************************/

#include<iostream>//Temel komutlarý(cout vs) kullanabilmemizi saðlayan kütüphane.
#include<Windows.h>//Kursörün yerini deðiþtirmek ve kursörü gizlemek için yazacaðýmýz komutlarý bu kütüphane sayesinde kullanabiliyoruz.
#include<time.h>//Rand() komutunu sýfýrlayýp yeni deðer atamak için gerekli kütüphane.

using namespace std;

//Kontoller = W: Yukarý, S: Aþaðý, K: Ateþ

//Yönler
enum YON
{
	YON_SOL = 1,
	YON_SAG = 2,
	YON_YUKARI = 3,
	YON_ASAGI = 4
};

//Uçaðýn koordinat, yön ve karakterini içeren yapý.
struct ucakHucre
{
	int		x;
	int		y;
	YON		yon;
	char	karakter;
};

//Merminin koordinat ve karakterini içeren yapý.
struct atesYapi
{
	int		x;
	int		y;
	char	karakter;
};

//Düþman biriminin koordinatlarýný,yönünü ve karakterini içeren yapý.
struct dusmanHucre
{
int		x;
int		y;
YON		yon;
char	karakter;
};

const int	genislik = 110;//Sahnenin toplam geniþliði
const int	yukseklik = 20;//Sahnenin yüksekliði


const char	ucakKarakteri = 219;

const char dusmanKarakteri = 176;

const char	atesKarakteri = 254;

const int dusmanSayisi = 50;

const int	govdeUzunlugu = 5;

const int  atesSayisi = 1000;

char	sahne[genislik][yukseklik];

char    tuslar[256];

int		a = 0, e = 0;


ucakHucre	ucakGovdesi[govdeUzunlugu];

atesYapi    atesBirimi[atesSayisi];

dusmanHucre dusman[dusmanSayisi];

//Klavyede bastýðýmýz tuþu okuyan ve kaydeden fonksiyon.
void klavyeOku(char tuslar[])//*
{
	for (int x = 0; x < 256; x++)
		tuslar[x] = (char)(GetAsyncKeyState(x) >> 8);
}

//Kursörün yerini belirleyen fonksiyon.
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Programýn çalýþmasý esnasýnda kusörü gizleyerek daha iyi bir görüntü oluþmasýný saðlayan fonksiyon.
void kursoruGizle()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}

//Tüm sahneyi çizen fonksiyon.
void sahneyiCiz()
{
	for (int y = 0; y < yukseklik; y++)
	{
		for (int x = 0; x < genislik; x++)
		{
			cout << sahne[x][y];
		}
		cout << endl;
	}
}

//Sahneyi temizleyen fonksiyon.
void sahneyiTemizle()
{
	for (int y = 0; y < yukseklik; y++)
	{
		for (int x = 0; x < genislik; x++)
		{
			sahne[x][y] = ' ';
		}
	}
}

//Oyunu oynadýðýmýz sýnýrlarýn oluþmasýný saðlayan fonksiyon.
void sinirlariOlustur()
{
	for (int x = 0; x < genislik-30; x++)
	{
		sahne[x][0] = 219;
		sahne[x][yukseklik - 1] = 219;
	}
	for (int y = 0; y < yukseklik; y++)
	{
		sahne[0][y] = 219;
		sahne[genislik-30 - 1][y] = 219;
	}
}

//Uçaðýmýzýn koordinatlarýný ve karakterini oluþturan fonksiyon.
void ucakOlustur()
{

	ucakGovdesi[0].x = 1;
	ucakGovdesi[0].y = 9;
	ucakGovdesi[0].karakter = ucakKarakteri;


	ucakGovdesi[1].x = 2;
	ucakGovdesi[1].y = 10;
	ucakGovdesi[1].karakter = ucakKarakteri;


	ucakGovdesi[2].x = 3;
	ucakGovdesi[2].y = 11;
	ucakGovdesi[2].karakter = ucakKarakteri;


	ucakGovdesi[3].x = 2;
	ucakGovdesi[3].y = 12;
	ucakGovdesi[3].karakter = ucakKarakteri;


	ucakGovdesi[4].x = 1;
	ucakGovdesi[4].y = 13;
	ucakGovdesi[4].karakter = ucakKarakteri;
}

//Uçaðýn hareket etmesini saðlayan fonksiyon.
void ucagiHareketEttir()
{
	for (int t = 0; t < 5; t++)
	{
		switch (ucakGovdesi[t].yon)
		{
		case YON_ASAGI:
			ucakGovdesi[t].y++;
			break;
		case YON_YUKARI:
			ucakGovdesi[t].y--;
			break;
		}
	}
}

//Uçak karakterlerinin sahneye çizilmesini saðlayan fonksiyon.
void ucagiSahneyeYerlestir()
{
	for (int i = 0; i < 5; i++)
	{
		int x = ucakGovdesi[i].x;
		int y = ucakGovdesi[i].y;

		sahne[x][y] = ucakGovdesi[i].karakter;
	}
}

//Mermimizin uçaðýn ortadaki karakterinin bir birim önünde oluþmasýný saðlayan fonksiyon.
void atesBirimiOlustur()
{
	atesBirimi[a].x = ucakGovdesi[2].x+1;
	atesBirimi[a].y = ucakGovdesi[2].y;
	atesBirimi[a].karakter = atesKarakteri;

	//Ateþ sayýsý 1000 e geldiðinde ateþ sýfýrlanýr ve baþa döner.
	if (a == 1000)
		a = 0;
		a++;
}

//Sahneye yerleþtirilen ateþin sahnenin sað tarafýna doðru ilerlemesini saðlayan fonksiyon.
void atesiHareketEttir()
{
	for (int t = 0; t < a; t++)
	{
		atesBirimi[t].x++;
	}
}

//Ateþin sahnede gözükmesini saðlayan fonksiyon. 
void atesiSahneyeYerlestir()
{
	for (int i = 0; i < a; i++)
	{
		int x = atesBirimi[i].x;
		int y = atesBirimi[i].y;

		//Ateþ çizdiðimiz sýnýrlarý aþtýðýnda yani x koordinatý 79 u geçtiðinde sahneye çizimi engellenir. Böylece sýnýrlarý aþan ateþ gözükmez.
		if(x<79)
		sahne[x][y] = atesBirimi[i].karakter;
	}
}

//KlavyeOku fonksiyonuna kaydedilen verileri tarayýp klavyeden basýlan tuþlar W,S ve K olduðunda çeþitli iþlemler yapan fonksiyon.
void klavyeKontrol()
{
	klavyeOku(tuslar);

	if (tuslar['W'] != 0 && ucakGovdesi[0].y != 1)//Yukarý hareket
	{

		for (int a = 0; a < 5; a++)
		{
			ucakGovdesi[a].y = ucakGovdesi[a].y--;
		}
	}

	if (tuslar['S'] != 0 && ucakGovdesi[4].y != 18)//Aþaðý hareket
	{
		for (int a = 0; a < 5; a++)
		{
			ucakGovdesi[a].y = ucakGovdesi[a].y++;
		}

	}

	if (tuslar['K'] != 0)//Ateþ
	{
		atesBirimiOlustur();
	}
}

//Düþman biriminin koordinatýný ve yönünü içerisinde barýndýran fonksiyon.
void dusmaniOlustur()
{
dusman[e].x = 76;
dusman[e].y = rand()%14+5;
dusman[e].yon = YON_SOL;

//Düþman sayýsý 1000 e gelene kadar arttýrýlýr.
if (e < 10000)
	e++;
}

//Düþman biriminin hareketini saðlayan fonksiyon.
void dusmaniHareketEttir()
{
	
	for (int t = 0; t < e; t++)
	{
		//Düþmanýmýz çizdiðimiz sýnýrlar içerisinde ise x ekseninde sola doðru hareket eder. Sýnýrlar dýþarýsýndaki düþmanlar hareket etmez.
		if (dusman[t].x<80)
			dusman[t].x--;
	}
}

//Sýnýrlar içerisindeki düþmanlarýn sahnede gözükmesini saðlayan fonksiyon.
void dusmaniSahneyeYerlestir()

{
	for (int i = 0; i < e; i++)
	{
		int x = dusman[i].x;
		int y = dusman[i].y;
	
		if (x > 0 && x<100) 
		{

		sahne[x][y] = dusmanKarakteri;
		sahne[x][y-1] = dusmanKarakteri;
		sahne[x][y-2] = dusmanKarakteri;

		sahne[x+1][y] = dusmanKarakteri;
		sahne[x+2][y] = dusmanKarakteri;
		sahne[x+2][y-1] = dusmanKarakteri;
		sahne[x+2][y-2] = dusmanKarakteri;
		sahne[x+1][y-2] = dusmanKarakteri;
		}
	}
}

//Mermi düþmana çarptýðýnda mermi ve düþmanýn çizdiðimiz alanýn dýþýnda bir boþluða gönderilmesini yani ekrandan kaybolmasýný saðlayan fonksiyon.
void carpismaKontrol()
{
	for (int i = 0; i < a; i++)
	{
		for (int t = 0; t < e; t++)
		{
			if (dusman[t].x == atesBirimi[i].x && dusman[t].y == atesBirimi[i].y || dusman[t].x == atesBirimi[i].x && dusman[t].y-1 == atesBirimi[i].y|| dusman[t].x == atesBirimi[i].x && dusman[t].y-2 == atesBirimi[i].y)
			{
				dusman[t].x = 100;
				atesBirimi[i].x = 120;
			}
		}
	}
}


int main()
{
	//Rand() komutunun programýn bir sonraki çalýþtýrýlmasýnda farklý deðer almasýný saðlýyor.
	srand(time(NULL));

	kursoruGizle();

	ucakOlustur();

	//Düþmanýn belirli aralýklarla gelmesini saðlamak için bir tamsayý deðeri oluþturdum. Bu deðer 15 in katlarý olduðunda düþman oluþturuluyor
	int sayi = 0;
	
	//While döngüsünün sürekli çalýþmasýný istediðimizden dolayý içine true yazdýk. Döngü her baþa döndüðünde tüm fonksiyonlar teker teker çalýþtýrýlýr.
	while (true)
	{

		sahneyiTemizle();
		
		sinirlariOlustur();

		klavyeKontrol();

		ucagiHareketEttir();

		ucagiSahneyeYerlestir();
		
		atesiHareketEttir();
		
		carpismaKontrol();

		atesiSahneyeYerlestir();
		
		//Sayi 15 ve 15'in katlarý olduðunda düþman oluþtur fonksiyonu çalýþtýrýlýr.
		if(sayi%15==0)
		dusmaniOlustur();

		dusmaniHareketEttir();

		dusmaniSahneyeYerlestir();

		carpismaKontrol();
		
		gotoxy(0, 0);

		sahneyiCiz();

		sayi++;
	}

}