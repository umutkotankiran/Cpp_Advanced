/*

Bir referans bir nesneye bağlandığında bir nesne oluşturmuyoruz.
Referans nesnenin yerine geçen isim. Sorulan sorular üzerine anlatıldı.

Myclass &&r = Myclass{}; Burada life extension var. Geçici nesneyi sağ taraf referansına bağlarsak
			 normalde ifade ne zaman biterse o zaman ölecek nesne, referans ne zaman biterse o zaman bitecek hale gelir

const Myclass &r = Myclass{}; // burada da aynısı var.

Myclass &r = Myclass{}; // burası zaten sentaks hatası

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Move tür dönüştürme yapıyor.Taşınmaya aday olmayan objeler taşınmnaya aday hale geliyor.

Klasik örneği verdi hoca. copy ctor nasıl çağrılır move ctor nasıl çağrılır ....

Temel C++ ta görüldüğünden yazmıyorum

move<m> ile static_cast<Myclass&&>(m) aynı şey oldu

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Move un en basit template hali

template <typename T>
constexpr std::remove_reference_t<T>&& Move(T&& t)noexcept
{
	return static_cast<std::remove_reference_t<T>&&>(t);
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Temel C++ konularında da var bu örnek

void func(const Myclass &r)
{
	Myclass mx(r);
}

void func(Myclass &&r)
{
	Myclass mx{std::move(r)};

	Myclass mx{r}; // Bu şekilde çağıranlar var.Neden move çağrılmıyor.r lvalue expression, data type ı rvalue reference :)
}

int main()
{
	Myclass m;

	func(m);
	func(Myclass{});
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Burada çok basic soru soruldu yazmıyorum

moved from state.
moved from state teki datanın ne olduğu unspecified behavior ama valid
tekrar bir atama yapılabilir.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Func parametresi sağ taraf referansı diye illaha kaynak çalınacak değil.
Belki sağ taraf ve sol taraf değeri için ayrı kodlar yazılabilir.%95 çalma yapılacak :D

void func(std::string&&)
{
}

Böyle bir func a çağrı yapılacaksa nesnenin kaynağının çalınacağını göze almalıyız.
Kaynak çalınmamışta olabilir.

Kaynağını çalınmasını istemediğimiz nesne varsa onu göndermeyeceğiz.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

IN OUT parameter

class Myclass{};

void func(Myclass&r); // out veya in-out parametre. Bana nesne göndereceksin ben onu set edeceğim diyor.

Taşıma semantiğinin olmadığı dönemde func return değerini vectoryada string yapmak yüksek maliyet içeriyordu.

string func(Myclass&r) yerine kopyalama maliyetini azaltmak için func parametresini lvalue reference yapıyorduk ve verdiğimiz nesneye yazdırıyorduk
void func(Myclass&r);

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Taşıma semantiği out parametre kullanımını azalttı.

Myclass func(Myclass&r); return değerini Myclass veya başka tür tercihediyoruz.
Taşımada her durumda maliyeti azaltmıyor.

Array düşününce her elemanın taşınması gerekiyor.Ama vectorde bu durum yok. pointerlar takas edilecek.begin end ve size tıtan poitnerlar olabilir bunlar.
mesela array<string,100> ; hepsinin taşınması gerekecek.

1. void func(Myclass&r); // out veya in-out parametre
Bu func a sağ taraf değeri gönderemeyiz.
Sol taraf değeri gönderip gönderdiğimiz nesne set edilecek.

Eğer gelen nesneyi salt yazma amaçlı kullanıyorsa bu out parametre
Hem okuma hem yazma amaçlı ise in-out parametre.

2. void func(const Myclass&r); // in parametre
Salt okuma amaçlı erişecek.Nesnede durum değişikliği olmayacak.
Bunu hem lvalue hemde rvalue ile çağırabiliriz.

3. void func(Myclass&&r);
Parametre sağ taraf değerine bağlanmak zorunda olan bir referans
bunu func(Myclass{}) veya func(std::move(m)) ile çağırabiliriz.(m bir myclass nesnesi)

Bu 3 fonksiyon birbirinin overload u. Bİrarada bulunabilirler

Hatta aşağıdaki overloadda olabilir.
void func(const Myclass&&);

BURADA HANGİ DURUMDA HANGİSİ ÇAĞRILIR KONUSUNA GİRİLDİ ONU BEN YAZMIYORUM.

const Myclass cm;
func(std::move(cm)); burada varsa const Myclass& çağrılır varsa const Myclass&& daha uygun.
const nesne, const olmayan referansa bağlanamaz.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

CALL BY VALUE 

Parametre Myclass ise bunu hiçbir şekilde overload etmeyeceğiz.
Eğer referans parametreli overload lar koyarsak birçok func çarısında ambigiuty oluşur.

Referans parametreli funclarla call by value birarada overload edilmeyecek. Bunların birbirine üstünlükleri yok.

class Myclass{};

void func(Myclass);
void func(Myclass&); // açıklanmak istenen bu şekilde olmaması.

int main()
{
	Myclass m;
	func(m); // AMbigiuty
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Call by value nesneyi değiştiremezdi ama move semantics kurallaır dğeiştiriyor.
Aşağıda call by value olmasına rağmen taşıma semantiği nesneyi dğeiştirecek.


class Myclass{};

void func(Myclass)
{
}

int main()
{
	Myclass m;
	func(std::move(m)); // Burası call by value ama func parametresi sınıf türünden, bu sınıfın move ctoru varsa çağrılacak hayata gelecek nesne için
}						// bu da bizim move ettiğimiz m yi değiştirecek. Call by value olmasına rağmen nesnenin kaynağı çalınacak.


Yani parametresi sınıf türünden olan function dopğrudan kaynak çalan func olarak kullanılıyor.ÇÜnkü bu func sağ taraf değeriyle çağrılırsa func parametresi
için move ctor çağrılacak. Bu da kaynağın çalınması demek.

TEKRAR : PArametresi sağ taraf referans olan bir function bir kaynağı çalıyor demek değil.
Çalabilir ama kesinlikle çalacak diye birşey yok.Kodun ne olduğuna bağlı.

Yani
class Myclass{};

void func(Myclass &&r)
{
	Myclass m{std::move(r)}; // 1. bu şekilde çalınabilir
}

void func(Myclass &&r)
{
	Myclass m
	m = std::move(r); // 2. bu şekilde çalınabilir
}

void func(Myclass &&r)
{
	std::vector<Myclass>vec;
	vec.push_back(std::move(r)); // 3. bu şekilde çalınabilir
}

3 adet çalma görüldü.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Myclass foo()
{
	Myclass mx;
	return mx;
}

int main()
{
	foo(); // Bunun türü R value expression.

	Myclass mx;
	mx = foo(); // çalmaya aday oldu
}



EĞer

const Myclass foo()
{
	Myclass mx;
	return mx;
}

int main()
{
	foo(); // Burada Copy Assignment çağrılır.Move assignment değil. !!!!! Return değeri const sınıf türü çok kullanılmıyor.YAPILMAMALI BU.
}			// Yani const yapınca taşıma yerine kopyalama semantiği devreye giriyor.


----------------------------------------------------------------------------------------------------------------------------------------------------------------

Taşımadan da iyisi var. Zaten taşıma semantiği herzaman daha verimli olacak diye birşey yok.

class Myclass{
public:


private:
	std::array<int,1000>ax; // Burada taşıma hiçbir avantaj getirmeyecek. İnt in taşınması ile kopyalanması arasında fark yok.
							// burada taşıma maliyeti, kopyalama maliyetine eşit.
};


Taşımadan daha iyisi copy ellision

NOT : BU KISIMA KADAR NOTLARIN TAMAMI ZATEN TEMEL C++ TA OLDUĞUNDAN BİLDİĞİMİZ ŞEYLER OLDUĞUNDAN HERŞEYİ YAZMADIM.
	  BURADAN SONRA YAZACAĞIM ARTIK.

=================================================================================================================================================================

COPY ELLISION
Kopyalamanın yapılmaması anlamında bir terim.Elide tan geliyor.
C++17 de Mandatory Copy Ellision oldu. 2017 standartlarına kadar compiler optimization tekniğiydi.

1. Optimizasyon Tekniği
-----------------------
RETURN VALUE OPTIMIZATION

Myclass func() //Temel C++ ta var
{
	return Myclass{}; // C++ 17 öncesinde derleyici bunun return değerini tutacak olan nesnenin yerini zaten biliyor. Buna return slot deniyor.
}					   


Myclass x = func(); //Burada move ctor ile x in hayata gelmesi beklenebilir. Return Value optimization ile return slotta oluşturuluyor bu nesne ve copy ctor çağrılarak
					// yapılıyordu bu. Buna RETURN VALUE OPTIMIZATION deniyor. RVO

C++17 ile kurallar değişti ve biraz daha karmaşık hale geldi.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

2. Optimizasyon Tekniği
-----------------------
NAMED RETURN VALUE OPTIMIZATION. MANDATORY DEĞİL.

Myclass func() // Temel C++ ta var
{
	Myclass x; // Otomatik ömürlü nesne 

	return x; // Otomatik ömürlü nesne return edildi.
}

Myclass x = func();

ÖR:
std::vector<std::string> foo()
{
	std::vector<std::string> vec;

	//.. vectorü doldurduk diyelim

	return vec;
}

int main()
{
	auto mvec = foo(); // Burada yine bu nesneyi mvec için ayrılan yerde oluşturuyor. İki ayrı nesne oluşmuyor burada.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

3. Optimizasyon Tekniği
-----------------------

vodi func(std::vector<int>x);

int main()
{
	func(std::vector<int>(10000,2)); //buradaki geçici nesne, yukarıdaki x in konumunda hayata gelecek.
									// nesne oluşturup sonra hayata gelmeyecek.
}


C++ 17 de çok ciddi değişiklik yapıldı.Buradaki terminoloji yanlış. Buna mandatory copy ellision deniyor.
Bu terim yanlış çünkü kopyalama yokki ellision olsun.C++17 ye göre burada kopyalama söz konusu değil zaten. O yüzden elemine edilmesi söz konusu değil.
bu 1. ve 3. durum için geçerli.

Mesela
class Myclass{};

Myclass func()
{
	return Myclass{};
}

int main()
{
	Myclass mx = func(); // Burada kopyalama olmayacak diyorduk ama gerçek bu değil.
}						 // Sınıfın copy ve move ctoru delete edilse bile bu geçerli.Yani bu ifade bir initializer ifade olarak kullanılıyor.
						 // 2. ise mandatory değil.



Mandatory Copy ellision neden yanlış?
C++17 ye göre simplified value categories and mandatory copy ellision deniyor.

BURASI ÇOK ÖNEMLI!!!!!!!!!!!!!!!!!!!!


=====================================================================================================================================================================
=====================================================================================================================================================================

TEMPORARY MATERALIZAION

Myclass{};	

BIZ BUNA(PR VALUE EXPRESSION) DOĞRUDAN GEÇICI NESNE DIYORDUK YANİ DOĞRUDAN TEMP OBJECT. ARTIK SINIF TÜRDEN PR VALUE EXPRESSION OLAN IFADELER
BIR SINIF NESNESINE KARŞILIK GELMIYOR.BU ADETA BIR SINIF NESNESININ OLUŞTURULMASI IÇIN KULLANILACAK BIR RESIM GIBI, KENDISI BIR NESNE DEĞIL.
GEREKTİĞİ ZAMAN BİR NESNE OLUŞTURACAK, BUNA YENİ STANDARTLAR RESULT OBJECT DENİYOR VE BUNUN OLUŞTURULMASI İÇİN ÖZEL BİR DÖNÜŞÜM GEREKİYOR.
BUNA TEMPORARY MATERALIZATION DENİYOR. BİR SAĞ TARAF DEĞERİ ANCAK BİR SOL TARAF DEĞERİ BEKLENEN YERDE KULLANILDIĞI ZAMAN FİİLEN BİR GEÇİCİ NESNE OLUŞUYOR.
GEÇİCİ NESNE OLUŞMASI İÇİN BİZİM PRVALUE EXP. İFADEMİZ XVALUE CATEGORİSİNE DÖNÜYOR. YANİ PR VALUE EXP DEN XVALUE EXP E DÖNÜŞÜM UYGULANARAK BİR NESNE
HAYATA GELİYOR. GEÇİCİ NESNE O ZAMAN HAYATA GELİYOR.

NE ZAMAN TEMPORARY MATERALIZAION OLUYOR.

class Myclass{
public:
	void func();
	int mx = 23;
};

int main()
{
	Myclass{}; // ŞUANDA TEMP MATERIALIZATION YOK. BU PRVALUE EXP

	Myclass x = Myclass{}; // TEMP MATERIALIZATION YOK. GEÇİCİ NESNE YADA RESULT OF OBJECTTE YOK.ÇÜNKÜ İNİTİALİZATİON AMAÇLI KULLANILMIŞ

	Myclass &&r = Myclass{}; // BURADA TEMP MATERIALIZATION VAR.YANİ PR VALUE OLAN EXP. ÖNCE XVALUE E DÖNÜŞTÜRÜLÜYOR VE BİR RESULT OF OBJECT HAYATA GELİYOR.
							 // R REFERANSIDA BU OBJEYE BĞLANIYOR.


	const Myclass &r = Myclass{}; // AYNI DURUM BURADA DA GEÇERLİ.DERLEYİCİ FİİLEN XVALUE EXP E DÖNÜŞEN BU İFADE İÇİN BİR GEÇİCİ NESNE OLUŞTURACAK KODU ÜRETECEK.


	Myclass{}.func(); // BU NORMALDE BİR PRVALUE EXPR. AMA BU FUNC IN ÇAĞRILMASI İÇİN BİR GLVALUE YA İHTİYAÇ VAR.DERLEYİCİ BURADA TEMP MATER. İLE XVALUE YA DÖNÜŞTÜRÜP
					  // GEÇİCİ NESNE HAYATA GETİRECEK.

	
	Myclass{}.mx; // BİR YUKARIDAKİ GİBİ AYNI.


	sizeof(Myclass{}); // YİNE GEÇİCİ NESNE OLUŞTURULMASI GEREKİYOR.

}

TEMP MATERIALIZATION: YENİ STANDARTLARA GÖRE ARTIK BİR PRVALUE EPXRESSION BİR GEÇİCİ NESNE DEĞİL. 2 İHTİMAL VAR.
1 - PR VALUE EXP BİR SOL TARAF DEĞERİ BEKLENEN YERDE KULLANILIRSA, DERLEYİCİ PRVALUE EXP DAYANARAK BİR GEÇİCİ NESNE OLUŞTURUYOR.
2 - EĞER PRVALUE EXPR BİR NESNEYİ İNİT ETME AMAÇLI KULLANIRSAK BURADA GEÇİCİ NESNE YOK.

YANİ MANDATORY COPY ELLİSİON 2. MADDEDEN ÖTÜRÜ YANLIŞ ASLINDA.
Myclass x = Myclass{}; // BURADA OLUŞAN NESNE YOK, KOPYALAMA OLMADIĞINDAN KOPYALAMANIN ELEMINASYONUDA YOK.BU SEBEPLE YANLIŞ.


Sonuç : Mandatory copy ellision veya simp val cat. yani pr value expressionun init amaçlı kullanılması durumunda ortada bir  geçici nesne olmaması bu tür kodların
sınıfın move ve copy ctorları delete edilse dahi geçerli olmasını sağlıyor.


ÖR:
class Myclass {
public:
	Myclass() {
		std::cout << "Default Ctor\n";
	}
	Myclass(const Myclass&) = delete;
	Myclass(Myclass&&) = delete;
};


Myclass func()
{
	return Myclass{};
}

int main()
{
	Myclass x = Myclass{}; //default ctor
	Myclass y = func(); //default ctor çağrıldı
}

Bu şekilde taşımaya yada kopyalamaya kapatılmış sınıflar için fabrika functionları yazabileceğiz.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Eskiden olsa bir functionun parametresini sınıf türünden yapmaya çekiniliyordu.
Şimdi taşıma semantik ve mandatory copy ellision sayesinde parametresi sınıf türden olan funclar çok sık kullanılmaya başlandı.

void func(Myclass x)
{
	
}

Pr value expr ile çağrı yapsak ne kopyalama ne de taşıma olacak.PR value expr initializaer olarak görev yapacak ve temp mater olmayacak nesne doğrudan hayata gelecek.
Bir sağ taraf değeri ile çağrı yaparsak taşıma semantiği çalışacak.
Dolayısıyla func parametresi sınıf türünden olması durumunda Pr value exp ile yapılan çağrı çok avantajlı hale geliyor.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Myclass func()
{
	Myclass x;

	return x;
}

Burada move semantics devreye girer ama move semanticsten önce compiler NRVO devreye girerse bu sefer çok daha iyi olacak.

Myclass a = func();

NVRO da derleyici optimizasyon yapsa dahi sınıfın move ctor yoksa copy ctor bulunması zorunlu. Mandatory olmamasının sonucu bu.

return Myclass{}; olsaydı gerek yoktu. Doğrudan initializer olarak kullanabilirdik.

NOT : 
Derleyicinin yaptığı 2 tip optimizasyon var.
1. Derleyicinin optimizasyon yaparak oluşturacağı assembly kodunun değişmesi.Bildiğimiz tipik comp optimizastion
2. Derleyicinin yazdığı kodun değişmesi. Assembly kod değil, derleyicinin yazdığı kod değişiyor.Mesela exception handlingteki noexcept.Move Semantics te göreceğiz.

Devam,
derleyicinin yaptığı optimizasyon semantik anlamı değiştirmiyor. 

Burada istisnai duırum var
Named Return value optim. kodun semantik karşılığınıda değiştiriyor.
NRVO da normalde default ctor çağrılıyordu normalde. NRVO olmasaydı Move ctpr yoksa da copy ctor çağrılırdı.
Peki ya burada çağrılacak copy ctorun bir yan etkisi varsa.

copy cotru ben oluştursam içinde static ömürlü sayacı artırsam yada içine loglama kodu koysak o çalışmayacak
Buda semantik karşılığının değişmesi demek. Ama derleyici ne copy ctor ne de move ctor hiçbiriyle ilgilenmiyor.
bu optimizasyonu yapıyor. Derleyicinin yaptığı optimizasyon ile kodun semantik karşılığı değişti. Ender olarak görülüyor.



Mandatory Copy ellision kod yazma biçiminide değiştiriyor. Return değeri sınıf türünden olan funcları artık yazmaya çekinmiyoruz.
Çünkü bu funclarda 2 bazende 3 ihtimal var.

1. Taşıma semantiği devreye girebilir.
2. Copy Ellision.Daha iyi seçim tabi copy ellision

MOLA

----------------------------------------------------------------------------------------------------------------------------------------------------------------

En iyi durum copy ellision yoksa move ctor o da yoksa fall back copy ctor.

Copy ellision ile ilgili yapılan tipik bir hata var.

Nec foo()
{
	Nec x;
	
	return std::move(x); // DİKKAT BURASI BÜYÜK HATA :D:D:D:D:D:D
						 // Burada artık copy ellision ortadan kalktı

	return x; // Halbuki hiçbirşey yapmasına gerek yoktu.Copy ellision bir engel yoksa doğrudan yapılır.
}

int main()
{
	Nec nec{ foo() };
	
}


----------------------------------------------------------------------------------------------------------------------------------------------------------------

Copy Ellision Hangi Durumlarda Devredışı Kalıyor

1. Functionun parametre değişkeninin geridönüş değerinde kullanılması
	functionun parametre dğeişkeni çin ayrılan yer ayrı return slot ayrı bir yerde ise, bu durumda
	özellikle derleyici func ın kodunuda görmüyorsa copy ellision imkansız hale geliyor.

	Nec foo(Nec x)
	{
		return x;	
	}

	int main()
	{
		Nec x;
		Nec nec{foo(x)};
	
	} burası ok
	
	----------------------------------------------------------------------------------------------------------------------------------------------------------------

	Nec foo(Nec x) //dikkat
	{
		//
		return x; // artık copy ellision çalışmaz.Move Ctor çağrılır
	}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

2. Static ömürlü bir nesneyi return ifadesi yaparsak yine copy ellision devre dışı kalır.Static ömürlü nesnenin
	yeri zaten belli, return slotta oluşma ihtimali yok.

	Nec mynec;

	Nec foo()
	{
		return mynec;
	}

	int main()
	{
		Nec x{foo()};
	}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

3. Funcitonun birden fazla return deyimine sahip olamsı fakat hepsinin isimlendirilmiş yerel değişken olmaması.

	Nec foo(bool b)
	{
		Nec x;
		Nec y;

		if(b)
			return x;
	
		return y;

	}

	int main()
	{
		Nec x{foo(true)};  // Birden fazla return statement varsa ve döndürülen nesneler farklı nesneler olduğundan
	}						// hangisi return slot olacağı belli olmadığından copy ellision yok.


	Aşağıdaki durumda da geçerli değil

	Nec foo(bool b)
	{
		Nec x;

		if(b)
			return Nec{};

		return x;
	}

	int main()
	{
		Nec x{foo(true)};   // Yine copy ellision devrede değil.
	}


	----------------------------------------------------------------------------------------------------------------------------------------------------------------

	class Base{};

	class Der : public Base{};

	Base func()
	{
		Der myder

		return myder;  // Burada da copy ellision çalışmaz çünkü object slicing durumu var burada.
	}


----------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------

Taşıma Semantiğinden Faydalanmaya Yönelik Diğer kod Yapıları

En çok tartışılan konulardan biri bir sıfının veri elemanına sınıfın Ctor init list ilk değer vermesi

1. DURUM
class PersonClassic{
public:
	PersonClassic(const std::string& name, const std::string& surname) : m_name{name}, m_surname{surname}
	{

	}
private:
	std::string m_name;
	std::string m_surname;
};

int main()
{
	using namespace std;

	string name(200,'A');
	string surname(200,'B');

	PersonClassic per{name,surname};
	//Burada per bir sol taraf değeri sınıfın parametreleri bunlara bağlanacak.
	//Burada elemanların hayata getirilmesinde copy çağrılacak.
	// taşıma semantiğinden hiç faydanılmadı, veri elemanları initialize ettiğimizde copy ctor çağrıldı.


	----------------------------------------------------------------------------------------------------------------------------------------------------------------

	PersonClassic per{"mustafa ahmet xxxxxxx","kahraman tolgar"};
	//Bu c stringlerin referansa bağlanabilmesi için ortrada string nesnesinin oluşturulması gerekiyor,
	// derleyici bu ifadeden hareketle sınıfın cstring parametreli ctoruna çağrı yaparak(Conversion Ctor) bir string nesnes oluşturacak
	// bu string nesnesi referansa bağlanacak oradan da kopyalama yapılacak.Veri elemanlarını init edeyim derken önce bir
	// geçici nesne oluşturuyoruz, bu geçici nesne için sınıfın Conversion ctoru çağrılacak ve o parametre olan referansa bağlanıyor,
	// referanstanda copy yoluyla sınıfın veri elemanları init ediliyor.


}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

2. DURUM
class PersonRValueRef{
public:
	PersonClassic(std::string&& name, std::string&& surname) : m_name{std::move(name)}, m_surname{std::move(surname)}
	{

	}
private:
	std::string m_name;
	std::string m_surname;
};

Burada parametre Rvalue reference olduğundan bunu bir sağ taraf değeriyle çağırabilirim.Ctora göndereceğimiz argümanın bir sağ
taraf değeri olan string olması zorunlu. move kullanıldı çünkü bu bir rvalue reference a sağ taraf değeri gönderilmiş.
Bunu taşıyacağız. move kullanmasaydık copy ctor çağrılacaktı.

Sol taraf değeriylede çağrılamaz burası.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

3. DURUM

class PersonInitMove{
public:
	PersonInitMove(std::string name, std::string surname) : m_name{std::move(name)}, m_surname{std::move(surname)}
	{
		
	}
private:
	std::string m_name;
	std::string m_surname;
};


Burada ise func parametresi doğrudan string türünden dolayısıyla buna argüman olarak bir sağ taraf değeri gönderirsek zaten
taşıma semantiği paraemtre değişkenlerişnin hayata gelmesinde kullanılmış olacak.
move ile tekrar sağ taraf değeri haline getirip elemanlara taşıma yapıyoruz.

Yani, bu function sağ taraf değeriyle çağrılırsa, önce sağ taraf değeri paramereye taşınacak, oradan da data memberlara
taşınacak.

BUnu sol taraf değeri argümanlarla da çağırabiliriz. Sol taraf değeri argüman olursa burada func parametresi için copy ctor
veri elemanları için move ctor çağrılacak.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

4. durumda her ihtimale karşı herşeyi yazmış hoca onu yazmayacağım.	2:32

----------------------------------------------------------------------------------------------------------------------------------------------------------------

bir benchmark programı yazılmış buradan performansa bakıldı. yazmadım.
chrono library kullanılmış burada.


*/
