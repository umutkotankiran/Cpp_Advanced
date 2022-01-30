/*


STATIC INITIALIZATION ORDER FIASCOYA GELECEĞİZ.CONSTINITE GELECEĞİZ

Birden fazla kaynak dosya var ve bu kaynak dosyada static ömürlü(static storageı olan değişkenler) değişkenler var
2 aşamada initialize ediliyor bunlar. 
1. Aşama static initialization.
2. Aşama dynamic initialization.

BU NEYI GETIRIYOR?
Birden fazla değişken farklı modüllerde farklı cpp dosyalarındaysa ve bunlar static ömürlüyse
ve her ikiside staticten initten sonraki dynamic initte ilk değerini alıyorsa, bu durumda hangisinin ilk değerini
daha önce alacağı unspecified behavior.Buda Runtime başında programın çökmesi sonucunu doğurabilir.
Burada initialize edilmeden bir nesnenin kullanılma riski var.

NASIL OLABİLİR BU?
Global bir nesnenin ctoru içinde global(static storaga sahip) başka bir nesneyi kullanmak.
Bir problem olması için sadece sınıf türünden nesne olması gerekmiyor.Dikkat etmezsek
Primitive türlerde de başımız derde girebilir. Primitive türlerde şöyle bir avantaj var(aritmetik türlerde),
dynamic initialization aşamasına gelmeden zaten zero initializate ediliyorlar.

//a.cpp
extern int b;
int a = b + 5;


//main.cpp
extern int a;
int b = a + 4;

Bunların hangisinin dynamic initialize edileceği derleyiciye bağlı.
Bu kod yazılmamalı.Unspecified behavior.

------------------------------------------------------------------------------------------------------------------------------

BUNUN BIR ISTISNASI VAR.
Eğer biz static ömürlü global bir nesneye sabit ifadesi ile ilk değer verirsek, buna constant initialization deniyor.
Bu durumda sadece static initialize yapılıyor.Dynamic initialize yapılmıyor.


int x = 5; // Durum değişti.Bunun için dynamic init yapılmayacak.Sadece static init order fiasco problemi yok
	   // Static init order fiasco açısından sabit değer vermek daha güvenli.


constexpr int x = 5; // Sabit ifadesiyle ilk değer alacak ve her zaman static init edilecek.
	  	     // Dynamic init edilme şansı yok.Constexpr değişkenler immutable.
		     // sınıf türündense mesela readonly kullanırız.

Eğer constexpr olmasaydı ve mutable bir değişken olsaydı yine dynamic init yapılmayacaktı.Bir önceki örnek işte.



NOT !!!!!!!!!!!!!!!!!!
SABİT İLE SABİT İFADESİNİ KARIŞTIRMA !!!!!!!!!!!!!!!!

cosntexpr int foo(int x)
{
	return x * x;
}

constexpr int func(int x)
{
	return x + 5;
}

int y = foo(10); // static init
int z = foo(10) + func(4); // static init

------------------------------------------------------------------------------------------------------------------------------

FUNCLAR CONSTEXPR OLMASAYDI ?
Dilin kuralı değil ama derleyiciler optimizasyon yaparak yine static init ile durumu kurtarabilir.
Yukarıdaki ise optimizasyon değildi.

int foo(int x)	// constexprlar gitti
{
	return x * x;
}
 
int func(int x) // constexprlar gitti
{
	return x + 5;
}

int y = foo(10); // static init olabilir optimizasyon ile
int z = foo(10) + func(4); // static init olabilir optimizasyon ile

------------------------------------------------------------------------------------------------------------------------------

CONSTEVAL İN COMPİLE TİMEDA ÇAĞRILACAK. CONSTEXPR İSE GÖNDERDİĞİMİZ ARGÜMANA BAĞLI
CONSTEVALE SABİT GÖNDERİLMEZSE SENTAKS HATASI OLUYOR.

------------------------------------------------------------------------------------------------------------------------------

DERLEYİCİ DYANMİC İNİTİ MAİN ÇAĞRILMADAN ÖNCE GERÇEKLEŞTİRİYOR, 
BURADA BAŞKA BİR PROBLEM YOKSA MAİN ÇAĞRILDIKTAN SONRADA BUNU GERÇEKLEŞTİREN
BİR OPTİMİZASYONDA YAPABİLİR.

------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------

CONSTINIT - C++20
-----------------
Biz bir değişkenin static init edileceğinden yani dynamic initialize edilmeyeceğinden,
static initin constant init olarak yapılacağından emin olmak istiyoruz.BİRAZ KARMAŞIK OLDU :D:D

int x = expr;	// expr bir sabit ifadesiyse constant init olacak, değilse dynamic inite gidecek
				// Biz bunun sadece constant expr olmasından ve dolayısıyla constant init edilmesinden ve
				// dolayısıyla sadece statiac init edilmesinden emin olmak istiyorsak constinit yapacağız :D:D::D:D:D:D:


Constinit yapıldığında legal olması için sadece static initialize yapılması lazım.
Yapılmazsa sentaks hatası oluşur.

constinit int x = expr; // static init olacaksa geçerli yapılamazsa sentaks hatası

constexpr int foo()
{
	return a * a;
}

constinit int x = foo(5); // GEÇERLİ. foo(5) sabit ifadesi.x constant initialize edilecek.

------------------------------------------------------------------------------------------------------------------------------

int foo(int 5)
{
	return a * a;
}

constinit int x = foo(5); // Sentaks hatası. foo(5) sabit ifadesi değil.

constinit, const demek değil.Sadece constant initialize edilecek demek.
const immutable.
constexpr tür değil. Değişkenin constant expr olarak kullnabileceğini söylüyor.

constinit = constexpr - const 


constexpr int foo(int a)
{
	return a * a;
}

constinit int x = foo(10);

int main()
{
	x = 5; // Değeride değiştirilebilir demek.GEÇERLi
}

Global değişkeni constinit ile tanımlayınca, artık o dynamic init değilde static
init yapılacağı garanti altında. Buda static initialization order fiascodan kurtuluyoruz demek.
dynamic inite gelmeden init edilmiş olacak.Aynı zamanda değişkenin değerinide değiştirebiliyoruz.

------------------------------------------------------------------------------------------------------------------------------

BU DURUM SADECE PRIMITIVE TÜRLER İÇİN Mİ GEÇERLİ? HAYIR
BIR SINIF TÜRÜ IÇINDE GEÇERLI MI? EVET
CONSTEXPR CTORDA BURADA DEVREYE GİRİYOR !!!!!!!!!!

struct A{

};

constinit A ax; // Geçerli çünkü constexpr bunun ctoru


class A{
public:
	constexpr A(int val) : mx(val){}
private:
	int mx;
};


constinit A ax{12}; // Geçerli.Demekki bu static/constant initialize edilecek.

------------------------------------------------------------------------------------------------------------------------------

CONSTINIT İLE CONST AYNI ŞEY DEĞİL

class A{
public:
	constexpr A(int val) : mx(val){}
	void set(int);
private:
	int mx;
};

constinit A ax{12};

int main()
{
	ax.set(12); //Geçerli çünkü constinit demek const demek değil.Const olsaydı this ptr const olacaktı ama
				// sınıf içindeki const değil.
}

------------------------------------------------------------------------------------------------------------------------------

CONSTINIT İLE CONST BİRLİKTE OLUR

class A{
public:
	constexpr A(int val) : mx(val){}
	void set(int);
private:
	int mx;
};

constinit const A ax{12}; // DİKKAT!!!

int main()
{
	ax.set(12); // Artık const oldu
				
}

------------------------------------------------------------------------------------------------------------------------------

BİR FUNCIN CONSTEXPR OLMASI BİZİM SEÇİMİMİZE BAĞLI DEĞİL.YAPILABİLİYORSA YAPILMALI ZATEN.
CONSTEXPR FUNCIN GEREKTİRDİKLERİ VAR.BU ŞARTLAR BAYA GEVŞETİLDİ AMA HALEN ŞARTLAR VAR.
BİRÇOK YERDE KULLANILIYORLAR.CONTAİNERLARIN DEFAULT CTORLARI CONSTEXPR GENELLİKLE ARTIK.


#include<mutex>
#include<string>
#include<vector>

constinit std::mutex m; // burada hata vermedi
constinit std::string s; // burada hata verdi
constinit std::vector<int> v; // bunuda dene sonra

int main()
{
	// burası visual studioda hata vermiyor ama gcc de veriyor.
}

------------------------------------------------------------------------------------------------------------------------------

SONUÇLAR
--------
Global değişkenleri constexpr yapmak en iyi durum.
Constexpr olmuyorsa constinit olması iyi çünkü bu constant inite tabi tutulacak demek.
Constinit hata verirse compile timeda constant inite tabi tutulmayacak demek.

------------------------------------------------------------------------------------------------------------------------------

Static init order fiascodan kurtulmanın yollarından biri eğer sınıf constexpr ctora sahipse ve 
constinit ile initialize edersek, contant initialize edilmeme durumunda sentaks hatası oluşuyor.
Yani constinit ile sınıyoruz durumu.

------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------

FARKLI KAYNAK DOSYALARDA GLOBAL DEĞİŞKENLER OLDUĞUNDA VE BUNLAR BİRARADA OLDUKLARINDA BİR SIRA SÖZKONUSU DEĞİLDİ.
UNSPECİFİED BEHAVİOR. BURADA BAZI İDİYOMLAR VE TEKNİKLER DEVREYE GİRİYOR.

ÖR:
struct Neco{
public:
	Neco()
	{
		std::cout << "ali"; // Burada da static init order fiasco olmalı.Neco türünden bir global değişen hayata
							// geldiğinde ve bu değişken dynamic init edildiğinde Ctor çağrılacak.
							// Normalde cout nesneside global bir nesne olduğuna göre bunun dynamic initi tamamlanmış
							// olması gerekiyor. Bu durumda nasıl bu durum static initialization order fiasco oluyor.
	}	

};

NIFTY COUNTER IDIYOMU ILE BU SAĞLANIYOR.
BU İDİYOMU KENDİ DEĞİŞKENLERİMİZE UYARLAYABİLİRİZ.

------------------------------------------------------------------------------------------------------------------------------

INLINE'DANDA BAHSEDİYORUZ

İnline değişkenler sanki herhangibir kaynak dosyada tanımlanıyor gibi aynı kurallara tabi.

inline int h = func(); 

header fildeda olsa dahi, o header file ı include eden cpp dosyasında nasıl
initialize ediliyorsa o kurallar tabi.

Destructorların çağrılmasında, normalde hangi kaynak dosyadaki nesnenin ilk defa
hayata geleceği nasıl belli değilse, aynı şey destructor içinde geçerli.
Sadece ctor için değil dtor içinde global değişkeni kullanırsak bu seferde global değişken
destroy olmuş olabilir. Bu önemli !!!

Hem ctor için hem dtor için dikkat etmemiz gerek.
Ctor içinde ve dtor içinde global değişkenler dökümante edilmişse kullanabiliriz.
Böyle bir dökümantasyon yoksa kullanmamalıyız.


------------------------------------------------------------------------------------------------------------------------------

ÖNEMLİ NOT!!!

constexpr int func(bool b)
{
	if(b)
		return 16;

	return std::getchar();
}

int x = func(true); // Geçerli.Constant init ediliyor
int y = func(false); // Geçerli. Constant init edilmiyor.Dynamic init

Bunu görmek için const init kullanırız.
constinit int x = func(true); // Geçerli
constinit int y = func(false); // Sentaks hatası


NOT : CONSTEVAL FUNCLAR OLABİLİR !!!!!!!

consteval int foo(int x)
{
	return x + 5;
}

constexpr int func(int x)
{
	return x + 5;
}


int main()
{
	int a = 5;

	auto b = foo(10); // Geçerli

	auto c = func(10); // Geçerli

	func(a); // Geçerli. Bu ifade constant expr değil artık. Geçerli ama constant exrp değil

	foo(a); // SENTAKS HATASI. Buradadaki ifade %100 constant time constant expr olmalı.
}

------------------------------------------------------------------------------------------------------------------------------

//nec.cpp
Nec nec;

//other.cpp
A ga = nec.foo(); // nec init edilmemiş olabilir burada.Static order init fiasco
					// bunu engellemk için 2-3 tane teknik var.


------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------

BURADA STATIC INIT ORDER FIASCODAN KURTULMAK İÇİN TEKNİKLER

1. TEKNİK
LEVERAGE CONSTANT INITIALIZATION
Burada maç şu. Loger sınıfının ctorunun çağrıldığından emin olacağız.
Yani global bir değişkeni kullanırken loger nesnesinin hayata geldiğinden emin olacağız.

Bu kod gcc de derleniyor. Visual studioda derlenmiyor olabilir.

//logger.h
class Logger{
public:
	constexpr Logger() = default;  // Default ctor constexpr. Global olarak default init edilmiş loger nesnesi 
									// Constant init edilecek demek bu.Static init order ifasco yok yani.

	void log(std::string_view msg);
private:
	std::optional<std::ofstream> m_ofs;
};

extern Logger;


//Logger.cpp
CONSTINIT Logger Logger;  // Bu değişken static initialize edilmesinde bir problem yok. Ctor constexpr çünkü.

void Logger::log(std::string_view msg)
{
	if(!m_ofs)	// boş ise
		m_ofs.emplace("log.txt"); // optional içindeki ofstream nesnesini construct etmiş olduk.

	*m_ofs << msg << '\n';
	std::cout << msag << '\n';
}

------------------------------------------------------------------------------------------------------------------------------

2. TEKNİK
LAZY INITIALIZATION/CONSTRUCT ON FIRST USE

class Myclass{

};

// Myclass m;  //ctor constexpr olmazsa bu dynamic inite tabi olur.Problemli durum.


Lazy initin en kolay yolu aşağıda.

Myclass& gx()
{
	static Myclass m; // Static yerel değişkenlerin dynamic initi, programın akışı onun tanımlandığı yere gelince yapılıyor.
					  // static initi daha önce yapılıyor yine
	return m;
}

//A ax = gx().foo(); // burada static init order fiasco yok.

NEDEN ?
ÇÜNKÜ ILK DEĞER VEREN IFADE YÜRÜTÜLDÜĞÜNDE YANI FUNC ÇAĞRILDIĞINDA NESNE HAYATA GELECEK.
HERHANGIBIR ŞEKILDE BIZ BIR GLOBAL DEĞIŞKENIN CTORU IÇINDE M NESNESINI KULLANDIĞIMIZDA PROBLEM OLMAYACAĞI GARANTISI VAR.
BU GARANTİ SADECE CTOR KULLANIMINDA VAR !!!!!!!!!!!!!!!!!!!!


M NESNESİ PROGRAMIN AKIŞI static Myclass m; BLOĞUNA GELDİĞİNDE M HAYATA GELECEK AMA BU NESNENİN 
DTORUNUN ÇAĞRILDIĞI NOKTA TAMAMEN BAŞKA MYCLASS NESNELERİ İLE İLGİLİ, BUNLARIN ÇAĞRILMA SIRASIYLA İLGİLİ !!!!!!!!!
YANİ BU BAŞKA BİR SINIFIN GLOBAL DEĞİŞKENİN DTORUNDA M Yİ KULLANABİLECEĞİM GARANTİSİNİ VERMİYOR.
C++ TA HAYATA VEDA ETME SIRASI HAYATA GELME SIRASIYLA TERS ORANTILI.İŞTE BURADA PROBLEM OLUŞUYOR.

BURADA RİSK YARATAN UNSURLARDAN BİRİ BUNUN DESTRUCTORUNUN ÇAĞRILACAĞI.
YEREL DEĞİŞKENLER İÇİN DTOR MAİN FUNCIN ÇALIŞMASINDAN SONRA ÇAĞRILIR.
BU DURUMDA BİR BAŞKA DEĞİŞKENİN DTORUNDA BUNUN KULLANILMASI DURUMUNDA HALEN BAŞIMIZ DERDE GİREBİLİR.
HİÇ GLOBAL DEĞİŞKEN KULLANMAZSAK HİÇ DERDE GİRMİYOR O AYRI BİR KONU :D:D
EN AZINDAN PROGRAMIN BAŞINDA HAYATA GELME GARANTİSİ VAR.
EĞER DTORDA BU ÇAĞRILIRSA, ÇAĞRILMASIYLA TERS SIRADA HAYATA VEDA EDİYORDU C++TA DEĞİŞKENLER.
DİYELİMKİ DTORDA ÇAĞRILDI, AMA ÇAĞRILDIĞI ZAMANDAN ÖNCE HAYATI SONA ERDİYSE BURADA SORUN OLUŞUYOR.

AŞAĞIDAKİ NEDEN KULLANILABİLİR???

class Myclass{

};

Myclass& gx()
{
	static Myclass *p = new Myclass; 
					  
	return *p;
}

BURADA DINAMIK ÖMÜRLÜ NESNE OLDUĞUNA GÖRE DTORU DELETE EDILDIĞINDE ÇAĞRILACAK.AMA BURADA ŞUANDA
DELETE EDILME IHTIMALI YOK.BIZ ETMEDIK DELETE. HER NESNE IÇIN BU UYGULANAMAZ AMA BU BIR MEMORY LEAK DEĞIL.
NEDEN DEĞIL ? PROGRAM BITENE KADAR HAYATTA KALACAKSA ZATEN MEMORY LEAK DIYE BIRŞEY YOK :D:D:D
PROGRAMIN SONUNA KADAR KALAN BIR NESNENIN KAYNAKLARINI GERI VERMEMESI ÖNEMLI DEĞIL.
AMA HER NESNE İÇİN BUNU SÖYLEYEMEYİZ. MESELA SEMAFORLAR... DTORUNUN ÇAĞRILMASINI İSTEYEBİLİRİZ.
NESNELERİN %98 İ BU KALIBA UYUYOR. BURADAKİ GARANTİ P NİN DTORUNUN HİÇ ÇAĞRILMAYACAĞINA GÖRE
BAŞKA GLOBAL DEĞİŞKENLERİN DTORUNDA BUNU KULLANSAK DAHİ BİR PROBLEM OLMAYACAK.


NOT:

Myclass& gx()
{
	static Myclass m; 
					  
	return m;
}

Myclass &gr = gx(); // Bu bizi kurtazmaz. Referans olan m de global çünkü.Static init order fiasco
					// constinit başına koyduğumuzda sentaks hatası verir.

------------------------------------------------------------------------------------------------------------------------------

3. TEKNİK

//lazy_init.h

template<typename Tag,typename T>  // 1. Template tür parametresi neden var? Birden fazla static instance yaratabilmek için
class lazy_init{
public:
	constexpr lazy_init() = default; // Lazy init sınıfı türünden global bir nesne default construct edildiğinde constant init edilecek

	T& get()
	{
		static T global;  // T türünden yerel bir nesne
		return global;  // bu nesne döndürülmüş
	}

	T& operator*()
	{
		return get();
	}

	T* operator->()
	{
		return &get();
	}
};

//lazy_init<struct MyclassTag, Myclass>gm; // Bu farklı bir tür
//lazy_init<struct NecTag, Nec>gm; // Bu ise farklı bir tür. Bu sebeple 1. template tür parametresi yazıldı.
									// farklı nesneler yaratma şansı verdi.


Burada halen static yerel nesnemiz ctor içinde myclass nesnesinin kullanılmasına engel değil ama dtor problemini çözmüyor.

COUT NASIL YAPILIYOR ?
Yani herhangibir sınıfın ctorunda kullanıldığında nesne hayata gelmiş olma garantisi vermeli,
herhangibir nesnenin dtorunda kullanıldığında hayatının henüz bitmediğinden emin olmamız gerekili.

BURADA DEVREYE NIFTY COUNTER IDIYOM GİRİYOR

------------------------------------------------------------------------------------------------------------------------------

NIFTY COUNTER IDIYOM

Burada stream türünden global nesne hangi sınıfın ctorunda kullanılırsa kullanılsın hayata gelmiş bir nesneyi temsil edecek.
Hangi sınıfın dtorunda kullanılırsa kullanılsın nesne hala hayatta olacak.

//stream.h
#ifndef STREAM_H
#define STREAM_H

class Stream
{
public:
	Stream();
	~Stream();
};

extern Stream& gstream; // global stream object

static struct StreamInitializer{  // gstreamInitializer türünden nesne yaratılmış. Internal linkagea ait bu.static var çünkü
								  // herhangibir kod dosyası bu header file ı include ettiğinde her kaynak dosya bir tane
								  // internal linkageda bir tane struct streamInitializer türünden değişken tanımlayacak.
	StreamInitializer();
	~StreamInitializer();

} gstreamInitializer;



//stream.cpp
#include"stream.h"
#include<new> // placement new
#include<type_traits> // aligned_storage

static int g_nifty_counter;   // dışarı kapalı bir değişken.Zero init olacak tabi

static std::aligned_storage_t<sizeof(Stream),alignof(Stream)> g_stream_buffer;  
// Bu bize bir sizeof(Stream) kadar bellek alanı sağlıyor
// bunun yerine static char buffer; kulanılabilirdi ama alignment
// burada bazı avantajlara sahip.
// static unsigned char buffer[sizeof(Stream)]  buda yazılabilirdi.ama diğeri daha avantajlı
// elimde olan bir bellek alanında doğrudan nesneyi construct etmek için placement new kullanılıyor.
// placement new heapten alan harcamıyor.Verdiğimiz adresi bize geri veriyor.Derleyici cosntructor çağırdığında
// this olarak bizim verdiğimiz adresteki bellek alanını kullanıyor.


Stream& stream = reinterpret_cast<Stream&>(g_stream_buffer); // Bellek alanını cast ettik ve steam isimli referansa bağladık.

Stream::Stream()
{
	//initialize things....
}

Stream::~Stream()
{
	//clean up
}

StreamInitializer::StreamInitializer()
{
	if(g_nifty_counter++ == 0)  // global değişken 0 ise içeri girdi ve değeri 1 artırıldı. 5 tane cpp dosyası include edilse bu değer artacak sürekli
		new(&stream)Stream(); // Placement new. streamin adresinde nesneyi oluşturuyor.
}

StreamInitializer::~StreamInitializer()
{
	if(--g_nifty_counter == 0) // destructorda değerini 1 azaltıyor eğer 0 olursa destructor çağırıyor.
		(&stream)->~Stream();  // Zaten placement new ile oluşturulan nesne delete edilmiyor, destructoru çağrılıyor.Placement newda görüldü.
}

cout, ceer... bunlar tamamen bu idiyomla yazılmış.

29.00 da aligned storage'a cppreference'tan baktık.

------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------


SINGLETON IMPLEMENTASYONLARI

class CSingleton{
public:
	CSingleton(const CSingleton&) = delete;				// copy memberlar delete edildi.
	CSingleton& operator=(const CSingleton&) = delete;

	void foo()
	{
		//...
	}

	static CSingleton& instance()
	{
		if(!p_instance)
		{
			p_instance = new CSingleton;
		}
		return *p_instance;
	}

private:
	 CSingleton();
	 inline static CSingleton *p_instance{}; //sınıfın static veri elemanını C++17 ile inline olarak tanımladık
};


//csingleton.cpp
CSingleton* CSingleton::p_instance; // bu sınıf içinde inline olmasaydı bnu şekilde cpp de tanımlayacaktık.
									// Header içinde yapılamaz bu.Header içinde yapsaydık ODR bozulurdu.
									// header içinde yapılması için yine inline olması lazımdı :D:D:D:D


//main.cpp

#include "csingleton.h"
int main()
{
	CSingleton &cs = CSingleton::instance();

	// Şuanda burada data race riski var. yani birden fazla thread olduğunda 2 kez initialize edilme riski var.
	// Bu risk çok yüksek.Yani bu thread safe değil.
}

Burada nesne new ile dinamik olarak hayata getirip delete etmiyoruz.Bu iyi mi kötü mü?
Bİzim durumumuza bağlı. Eğer bizim nesnemiz herhangibir şekilde program sonlanmasına rağmen dtor çağrılıp dtorun bazı
kaynakları resourceları geri vermesi gerekiyorsa dtorunun çağrılması gerekiyor.

Mesela QT de widgetlardan birisi singleton olsun, program bitene kadar zaten kalıyor.
Program bittikten sonra zaten kaynağın geri verilmemesi diye birşey yok.Leak olması için program devam ederken
kaynağın geri verilmemesi resource leak oluşturuyor.Memory leak zaten yok.
Program tekrar çalıştırıldığında heap zaten temiz olacak.Başka programlar belki bazı kaynakları ortak kullanıyorsa
program biterken bazı sınıf nesnelerinin dtorunun çağrılması gerekiyor.Shared memory olan durumlar gibi.

Sonuç olarak kaynağı geri vermiyorsa kötü diyemeyiz. Tam tersi Dtorun çağrılacağı bir implementasyon seçersek
o zamanda global bir nesnenin dtoru için bizim singleton kullanıldığında problem olacak.


------------------------------------------------------------------------------------------------------------------------------

//dsingleton.h
class DSingleton{
	DSingleton();
	void foo_impl();
	inline static DSingleton* p_instance{};

public:
	static void foo();  // clientın çağırdığı foo funcı
};


//dsingleton.cpp
#include "dsingleton.h"

#define PRIVATE
#define STATIC

PRIVATE DSingleton::DSingleton() // bu kısımda önemli değil sadece static member funcı gösterdi hoca.
{
	if(!p_instance)
		p_instance = new DSingleton;
}

PRIVATE void DSingleton::foo_impl()
{
	//...işi yapan bu
}

STATIC void DSingleton::foo()
{
	p_instance->foo_impl();  // kolaylık olsun diye bu funcı ekledik.
}



------------------------------------------------------------------------------------------------------------------------------

//Eager İnstantiation

Lazy init değil burası

//esingleton.h
class ESingleton{
public:
	ESingleton(const ESingleton&) = delete;
	ESingleton& operator=(const ESingleton&) = delete;
	~ESingleton();
	static ESingleton& get_instance();
	void foo();

private:
	static ESingleton instance; //Burası incomplete type olabilir. Başına inline koysaydık tanımlama olacaktı.
								// Incomplete type tanımlamada sentaks hatası verecekti.
	ESingleton();
};


//esingleton.cpp

ESingleton Esingleton::instance; // Burada static değişken var

ESingleton& ESingleton::get_instance();
{
	return instance; //static veri elemanı get ediliyor
}

ESingleton::ESingleton()
{
	//..
}

ESingleton::~ESingleton()
{
	//..
}

void ESingleton::foo()
{
	//..
}

Burası static initialization order fiascoya açık
Lazy eager Instantiation var burada.Kullanılsada kullanılmasada main çağrılmadan hayata gelecek demek.
Bunun avantajı destructor çağrılacak olması.Dİğerinde çağrılmıyordu.

------------------------------------------------------------------------------------------------------------------------------

Meyer Singletonı.

Burada avantaj C++11 ile static yerel değişkenlerin initi artık doğrudan thread safe.
HErhangibir korumaya gerek yok.Dil bunu garanti ediyor. Yani multithread çalışsada bu init edildiğinde 
diğer threadler bunu init etmeyecek.

Avantajları
-----------
Lazy init sağlıyor
Singleton Initin thread safe olması

Diğer taraftanda destructor çağrılıyor. Bu avantajda olabilir dezavantajda olabilir.


//m.h
class MSingleton{
public:
	static MSingleton& get_instance();
	MSingleton(const ESingleton&) = delete;
	MSingleton& operator=(const ESingleton&) = delete;
	void foo();

private:
	MSingleton();
}

//m.cpp
#include "m.h"

#define PRIVATE
#define STATIC

STATIC MSingleton& MSingleton::get_instance()
{
	static MSingleton instance;

	return instance;
}

PRIVATE MSingleaton::MSingleton()
{
	//..
}


BİRAZ DAHA FARKLI VERSİYONUDA VAR
Bu durumda bu nesne delete edilmeyecek.Delete edilmeyecek dedik ama orada da bir problem var.

STATIC MSingleton& MSingleton::get_instance()
{
	static MSingleton *p = new MSingleton; //DİKKAT

	return *p;
}

PRIVATE MSingleaton::MSingleton()
{
	//..
}

------------------------------------------------------------------------------------------------------------------------------

TSingleton

destructor not called


class TSingleton{
public:
	void foo();
	static TSingleton& instance();
	TSingleton(const TSingleton&) = delete;
	TSingleton operator=(const TSingleton&) = delete;
private:
	TSingleton();
	inline static TSingleton* mp_instance{};
};



#include<mutex>
#include "tsingleton.h"

#define PRIVATE
#define STATIC

std::once_flag flag;

STATIC TSingleton& TSingleton::instance()
{
	std::call_once(flag,[](){ mp_instance = new TSingleton;}); /// bu callableın sadece 1 kez çağrılmasını sağlıyor.
	return *mp_instance;
}

void TSingleton::foo()
{

}

PRIVATE TSingleton::TSingleton()
{
	//..
}

------------------------------------------------------------------------------------------------------------------------------

POINTERIN KENDISINI RAW PTR YAPMAK YERINE UNIQUE POINTER YAPMAK.UNIQUE PTR NESNESI IÇIN DTOR ÇAĞRILDIĞINDA
DTOR DINAMIK ÖMÜRLÜ NESNEYIDE DELETE EDECEK.


#include<memory>

class Usingleton{
private:
	struct Deleter{
		void operator()(USingleton*p);
	};
	inline static std::unique_ptr<USingleton,Deleter> mp_instance; // Static bir unique ptr koyduk.NEDEN DELETER YAZDIK? AÇIKlANACAK
	USingleton();
	~USingleton();
public:
	static Usingleton& get_instance();
	USingleton(const USingleton&) = delete;
	USingleton& operator=(const USingleton&) = delete;
	void foo();
};



void USingleton::Deleter::operator()(USingleton *p)
{
	delete p;
}

PRIVATE USingleton::USingleton()
{
	//..
}

USingleton::~USingleton()
{
	std::cout << "destructor caled\n";
}

STATIC USingleton& USingleton::get_instance()
{
	if(!mp_instance)
	{
		mp_instance.reset(new USingleton); // unique ptr boşsa doldurabiliriz.Dolu ise eskisini delete edip yenisini almasını sağlarız
	}

	return *mp_instance; // ptr ın gösterdiği nesneye eriştiriyor.
}

void USingleton::foo()
{
	
}


Deleter olmasaydı yukarıda default deletei kullanacaktı.unique ptr in dtoru doğrudan nesneyi delete etmiyor.
2 template parametresi olan türün operator funcını çağırıyor ve buna aslında eleman olarak aldığı nesne adresini gönderiyor.

Sınıfa yazdığımız deleterda delete ediyor. Ne fark var ?????
Eğer yazdığımız deleter olmasaydı

int main()
{
	USingleton& x = USingleton::get_instance(); // bunu çağırınca bir referans verdi.

	delete &x; // burada delete edince neden sentaks hatası oldu.Destructor private 
				// Dtor private yapılınca ve default delete kullansaydık sentaks hatası olacaktı.
}

ŞIMDI UNIQUE PTR NIN DELETE ETMESINI KENDI DELETERIM ILE SAĞLIYORUM.
DIĞERINDE DELETE EDEMEYECEKTI. BUNU YAPMAMIN SEBEBI DTORUN PRIVATE OLMASINI SAĞLAMAK.
DTOR PRIVATE OLMAZSA DIŞARIDAN DA DELETE EDILEBILIR. PRIVATE YAPINCA UNIQUE_PTR DE DELETE EDEMIYOR :D::D:D
BURADA BU PROBLEMI AŞTIK.DELETER OLUŞTURDUK. BU TÜR KENDİSİ DELETE EDİTYOR.CALLABLE OLARAK OPERATOR FUNC KULLANIYOR.
UNIQUE PTR NESNESİNİN DTORU ÇAĞRILDIĞINDA, DTOR ASLINDA BURADAKİ OPERATOR FUNCI ÇAĞIRACAK.

*/
