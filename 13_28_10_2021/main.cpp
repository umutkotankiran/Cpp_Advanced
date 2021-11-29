/*

Function şablonları overload edilebilir.
FUnction overload resolution zaten karmaşık, templateler girince işin içine dahada karmaşık.

- İmzaları farklı birden fazla aynı isimli template, aynı scopeta bulunabilir.
	Gerçek functionların imza tanımıyla func şablonlarının imza tanımları birbirinden farklı.
	Function template return değeride imzaya dahildi.
	Geçen ders konuştuk

	template<typename T, typename U>
	void func(T x, U y); //SIRAYA DİKKAT !!
	
	template<typename T, typename U>
	void func(U x, T y); //SIRAYA DİKKAT !!

	Burada overloading var. Overloading olup olmaması farklı birşey, bu overloadlar varken
	bu functiona yapılan çağrının geçerli olması farklı birşey.Bu şablon olsada olmasada böyle.

	void f(int&);
	void f(int); // Bunlar overloading ama int gönderirsek ambigiuty oluşur.

	----------------------------------------------------------------------------------------------------------------

	YUKARIDAKI TEMPLATE ÖRNEĞINI YAZALIM TEKRAR AMA OVERLOADING OLMADAN YAZALIM.
	Eğer burada overloading olmasaydı biri bir header fileda diğeri başka header fileda
	olsaydı, bu durumda bu tanımlar birbirinden farklıysa ayrı kaynak dosyalarda instantiate olsa dahi
	tanımsız davranış olacaktı.ODR ihlal edilecekti.

	A.h
	template<typename T, typename U>
	void func(T x, U y) { }

	B.h
	template<typename T, typename U>
	void func(T x, U y) { }

	Farklı dosyadalar ama halen tanımsız davranış.

	----------------------------------------------------------------------------------------------------------------

	ŞİMDİ BAKALIM
	A.h
	template<typename T, typename U>
	void func(T x, U y) { }

	B.h
	template<typename T, typename U>
	void func(U x, T y) { }

	Şimdi bunlar farklı kaynak dosyalarda olsalar ortada sentaks hatası yok ve link aşamasında
	farklı templatelerde oluşturulmuş olan functionlara ilişkin referans isim farklı olacak.

----------------------------------------------------------------------------------------------------------------

- Ortada birden fazla func şablonu varsa hatta gerçek functionlarda varsa overload resolution nasıl yapılıyor ?

template <typename T>
void func(T*)
{
	std::cout << "T *\n";
}

template <typename T>
void func(const T*)
{
	std::cout << "const T *\n";
}

OVERLOADING VAR.

int main()
{
	int x = 10;

	func(&x); // 1. func template çağrılır.
}


-------------------------------------------------------------------

TEKRAR BAKALIM

//#1
template <typename T>
void func(T x)
{
	std::cout << "func(T x)\n";
}

//#2
template <typename T>
void func(T *x)
{
	std::cout << "func(T* x)\n";
}

//#3
template <typename T>
void func(T** x)
{
	std::cout << "func(T** x)\n";
}

int main()
{
	int x{};
	int* ptr{ &x };

	func(10); //only #1 is viable. 

	func(&x); //both viable the more specific one (#2) is chosen.
	
	func(ptr);
	
	func(&ptr);
}


BURADAKI KURAL PARTIAL ORDERING RULES.
Buradaki kurallar baya karmaşık ama özü daha spesifik olan çağrılır.
1. func callda zaten 10 bir sabit doğrudan 1. template çağrılır
2. func callda ise ikiside uyumlu ama daha spesifik olan 2. templateten kod yazılır ve çağrılır.


------------------------------------------------------------------------------------------------------

PARTIAL ORDERING RULES
----------------------
Ortada birden fazla func varsa derleyici önce function overload seti oluşturuyor.Yani function overloada
katılacak funclar anlamında. Bu func overload seti oluştururken ortada bir template varsa, önce
template ile ilgili argument deduction yapmalı.DEDUCTION aşamasında tür belli olduktan sonra
derleyici SUBSTITUTION yapıyor. Substitution, function parametrik yapısını yani imzasını çıkarıyor.
NOT : TEMPLATE OLAN İLE NONTEMPLATE FUNCLARIN İKİSİNDEDE EXACT MATCH DURUMU VARSA, NONTEMPLATE OLAN ÇAĞRILIR.
GERÇEK FUNC ÇAĞRILIR YANİ !!!!!

SFINAE
------
Substitution failiure is not an error.

Diyelimki bir func şablonu var.Ben bu funca bir argüman gönderdiğimde derleyicinin argument deduction
yapması gerekiyor.Argüment deduction ile T türü belli olacak. Derleyici bu functan parametrik bir yapı elde etmesi
için substitution aşamasıona geçecek bu burada bir error olabilir.Bu durumda sentaks hatası vereceği yerde 
derleyici bunu overload setten çıkartacak.

ÖZET: Template in imzasının belli olma aşamasında(substitution) bir sentaks hatası meydana gelirse 
bu hata olarak ele alınmıyor ve derleyici tarafından overload setten çıkartılıyor.

ÖR:
template <typename T>
typename T::nec func(T x); // T burada int. int e bağlı bir tür mümkün değil.SFINAE.
			   // Burada hata olmasının sebebi, bu func overload setten çıkartılınca
			   // başka function kalmıyor :D:D.
int main()
{
	int x = 10;

	func(x);
}

------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>

template<typename T, unsigned N>
std::size_t len(T(&)[N])
{
	return N;
}

template<typename T>
typename T::size_type len(T const& t)
{
	return t.size();
}

int main()
{
	int a[10]{};
	std::cout << len(a); 
	
	 T int, N 10. Derleyici func için parametrik yapı oluşturacak.
	 2. template te T = int[10]. Return değeri dizinin size_type nested type
	 Bu olmaz. Burada SFINAE var. Hata yok.Bu template overload setten çıktı.
	 Yani sfinae out oldu.Terim bu.SFINAE OUT.
	 1. çağrıldı
	
	----------------------------------------------------------------------------------------------------------

	std::cout << len("tmp"); 
	
	// T const char[4]. 2. templatte yine SFINAE VAR. Sentaks hatası yok.
	// 1. çağrıldı

	----------------------------------------------------------------------------------------------------------

	int* p{};
	std::cout << len(p); 
	
	GECERSIZ. Çünkü 1. func template te artık viable değil.Geriye sadece 2. kaldı
	yine SFINAE oldu 2. için. Ortada başka functa kalmadı.SENTAKS HATASI OLDU :D:D:D

	----------------------------------------------------------------------------------------------------------

	AŞIRI ÖNEMLİ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	std::allocator<int> x;
	std::cout << len(x); 

	GECERSIZ. 1. FUNC ZATEN VIABLE DEĞIL.
	2.DE T ALLOCATOR<INT>. BU DURUMDA FUNC RETURN DEĞERI ALLOCATOR<INT>::SIZE_TYPE
	DİKKAT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	BU AÇILIMIN SIZE_TYPE I VAR MI? DERLEYINCE HATA VERMEDI, 2. FUNC SEÇILDI.
	DERLEYICI ARTIK 2. FUNCTAN INSTANTIATE YAPACAK. SUBSTITUTION BAŞKA INSTANTIATION BAŞKA
	INSTANTIATION GERÇEK KODU YAZMA AŞAMASI.SUBSTITUTION İSE FUNCIN PARAMETRİK YAPISININ/İMZASININ ORTAYA ÇIKMA AŞAMASI.
	KODU ÇALIŞTIRINCA T.SIZE() DA HATA VAR DEDI. YANI T NIN SIZE FUNCI YOK :D:D:D::D
	allocator<int> te size func yok :D:D:D:D

}

-----------------------------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>
#include <vector>

template<typename T, unsigned N>
std::size_t len(T(&)[N])
{
	return N;
}

template<typename T>
typename T::size_type len(T const& t)
{
	return t.size();
}

std::size_t len(...) // C den bildiğimiz variadic func.
{
	return 0;
}

int main()
{
	int a[10];
	std::cout << len(a);
	
	1. func çağrılır.
	2. func zaten SFINAE OUT oldu overload sete katılmadı
	3. func variadic conversion en az öncelikli dönüşüm. Bu sebeple 1. çağrılır.
	
	-------------------------------------------------------------------------------------------
	
	std::cout << len("tmp");

	1. func kazanıyor yine
	2. SFINAE return değerinden ötğrğ
	3. kaybetti yine yukarı ile aynı :D
	
	-------------------------------------------------------------------------------------------
	
	std::vector<int> v(5);
	std::cout << len(v);
	
	1. overlaod resolutiona katılmadı
	2. sfine out olmadı. vectorun size functionu var.
	3. yine kaybetti. :D variadic en düşük öncelikli.  2. func çağrılır
	
	-------------------------------------------------------------------------------------------
	
	int* p{};
	std::cout << len(p);

	1. overlaod resolutiona katılmadı
	2. sfine out oldu çünkü pointerın size parametresi yok. size_type ıda yok
	3. variadic kaldı en sona. başka alternatif yok variadic çağrılır.

	-------------------------------------------------------------------------------------------
	
	std::allocator<int> x;
	std::cout << len(x);

	1. overlaod sete katılmaz viable değil.
	2. sfine out ile gitmedi overload resolutiona katıldı
	3. Buda overload resolutiona katıldı.

	2 vs 3 olunca 2. func kazanır çünkü 3. variadic.
	2. functan instantiate aşamasında sentaks hatası oluşur.
	Buradaki önemli konu, pointerın bildiriminden hareket edip yani imzasından hareket edip
	function seçip yazdırıyor. Yazdırıp çalıştırınca geçersiz oldu :D:D Çünkü size functionu yok.

	SENTAKS HATASI

}

---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------

PARTIAL ORDERING RULESA GERİ DÖNELİM

template<typename T>
void t(T*, T const* = nullptr, ...);

template<typename T>
void t(T const*, T*, T* = nullptr);


void example(int* p)
{
	t(p, p); //SENTAKS HATASI
}

TEMPLATE LERE BAKARSAK FARKLI 2 OVERLOAD VAR AMA DAHA SPESIFIK OLAN ÇAĞRILMADI.
CEVAP: ÇÜNKÜ DAHA SPESİFİK OLAN YOK :D:D
ŞİMDİ KARMAŞIK TANIMA BAKALIM

template <typename T>
void func(T x);

template <typename T>
void func(T *);

Hangisi daha spesifik, 2. daha spesifik.

----------------------------------------------------------------

template <typename T>
void func(T (&)[10]);

template <typename T>
void func(T *);

... Cevap vermek zorlaşıyor


Buradaki hangisi daha spesifik sorusunun cevabını veren kurallara partial ordering rules deniyor.
Derleyici hangisinin daha spesifik olduğunu anlamak için her iki templatetede başka yerde kulanmadığı
bir type alacak. BU type başka yerde kullanılmamış olacak.

template <typename T>
void func(T x); // MESELA A TÜRÜ OLSUN

template <typename T>
void func(T *); // BURADAKİDE B TÜRÜ OLSUN

1. functa, functionun parametresi A türünden ve bu parametreyi kullanarak diğer templatte tür çıkarımı 
yapmaya çalışacak. A türünden bir nesne T* ile tutulmaz çünkü A bir adres değil.

2.Tersini yapacağız. B aldığımızda, func parametresi B* oldu. Aynısını yapıyoruz. 
B*, 1. functaki T ile tutulabilir. EVET. T yerine B* gelir.

1. functan match oldu, 2. func match olmadı. Bu durumda hangisi daha spesifik oldu, Alttaki.
Çünkü B* gönderilebildi.

BIRAZ DAHA DETAYI VAR
1 - Burada varsayılan argüman değeri kullanılmışsa o buna dahil değil.
2 - Variadic parametrede buna dahil değil.

----------------------------------------------------------------------------------------------

SORUYA DÖNELİM

template<typename T>
void t(T*, T const* = nullptr, ...);

template<typename T>
void t(T const*, T*, T* = nullptr);

Variadic parametre yokmuş gibi ele alınacak.
Varsayılan argüman varsa ve varsayılandan gelen değer kullanılmamışsa o parametreyi tamamen çıkartıyoruz,
Varsayılan argüman varsa ve varsayılandan gelen değer kullanılmışsa sadece varsayılanlık siliniyor parametre kalıyor.


ASIL DEĞERLENDİRİLECEK ŞEKLİ

template<typename T>
void t(T*, T const*); // variadic param doğrudan silindi.
					 // Default parameterden gelen değer kullanılmamış o yüzden eşitliğin sağ tarafı silinecek
					 //A türünü T yerine alalım

template<typename T>
void t(T const*, T*); // Default parametreden gelen değer kullanılmış çünkü func callda 2 argüman var.
					  // 3. ise defaulttan gelmiş yani kullanılmış.Buda silinecek.
					  //B türünü T yerine alalım

void example(int *p)
{
	t(p,p);
}

T yerine A türünü alalım

A*, const A* bu argümanlarla alttaki template e çağrı yapmaya çalışırsak sentaks hatası olur 2. argüman const çünkü

const B*, B * bunlarla çağrı yaparsak burada da 1. argüman const olduğundan uyum sağlamıyor.

HİÇBİRİ KAZANAMADI SENTAKS HATASI.

--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------

BIR FUNCTION ILE TEMPLATE BIRARADA BULUNUP EXACT MATCH OLURSA GERÇEK FUNC ÇAĞRILIR.

int max(int a, 	int b)
{
	return b < a ? a : b;
}

template<typename T>
T max(T a, T b)
{
	return b < a ? a : b;
}

int main()
{
	::max(7, 42); // max(int, int) gerçek func çağrılır.
	::max(5, 42.0); // max(int,int) Gerçek func çağrılır.Template zaten viable değil.Veri kaybıda olur
	::max('a', 'b'); // max<char> template
	::max<>(7, 42); // max<int> template çünkü<> kullanılmış
	::max<double>(7, 42); // max<double> template
	::max<double>(7, 4.2); // max<double> template.veri kaybı oluşur
	::max('a', 42.7); //max(int, int) gerçek func çağrılır.

}

------------------------------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
auto max(T1 a, T2 b)
{
	return b < a ? a : b;
}

template<typename RT, typename T1, typename T2>
RT max(T1 a, T2 b)
{
	return b < a ? a : b;
}

int main()
{
	auto a = ::max(4, 7.2); // <typename T1, typename T2>

	2. func seçilmez çünkü return değeri türü çıkarımı yapılamaz.Sadece 2. func bırakınca zaten
	sentaks hatası görülüyor. Duruma göre return değeri türü değişebiliyor çünkü
	if else yapısında da çıkarım yapılamıyordu.
	
	------------------------------------------------------------------------------------------

	auto b = ::max<long double>(7.2, 4); // <typename RT, typename T1, typename T2>

	Burada en az bir tanesinde diğerlerinden üstün olacak ve diğerlerinde de kötü olmayacak
	Ona göre 2. seçilir.

	------------------------------------------------------------------------------------------
	
	auto c = ::max<int>(4, 7.2); // gecersiz

	2 side viable ve ikisi arasında seçim yapılamıyor.

}


MOLA

==================================================================================================================
==================================================================================================================
==================================================================================================================
==================================================================================================================

BURADA DERLEYİCİNİN TANIMI GÖRÜLÜYOR AMA BİLDİRİMİ ÜSTÜNDEN SEÇİM YAPILIYOR.
Return değeri auto bildirimde yapılabilir.

Bir func olsun bu sadece int argümanla çağrılsın ama başka hiçbir argümanla çağrılamasın

template <typename T>
void func(T) = delete;

template <>
void func(int)
{
	std::cout << "func(int) called\n";
}

int main()
{
	func(12); // Geçerli
	func(12L); // geriye kalanların hepsi sentaks hatası
	func(1.2f);
}

------------------------------------------------------------------------------------------------------------------------

TEMEL C++ TA ANLATILMADI 
------------------------
Sfinae den faydalanarak belirli bir templatein belirli türleri için kısıtlanmasını sağlamak, ya sentaks htası
oluşturmak yada onu devre dışı bırakıp başka bir templatein devreye girmesini sağlamak.Bunlar görülecek

C++20 ile çok daha üstün SFINAE kullanılan birçok yerde concept onun yerini tutuyor.Çok daha rahat çok daha iyi
derleyicinin çok daha anlaşılır mesaj vermesini sağlıyor.

MESELA

template <typename T>
void func(T x);

Mesela gönderilen argüman tamsayı türlerinden değilse ya sentaks hatası olsun yada başka template 
var ise bu sfinae out yapılsın yani sfinae ile devre dışı bırakılsın.
Bunu sağlamak için sfinae bilinçli olarak kullanılabilir.

Yani substitution aşamasında T türü tamsayı türlerinden değilse sentaks hatası dğeilde sfinae ile
devre dışı bırakılacak. Bunların başında enable if sınıfı var.

------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------

AUTO ILE ILGILI KARIŞTIRILAN YERLER
-----------------------------------

Auto return type C++ 14 te eklendi. Auto return type ile trailing return type farklı şeyler.

template <typename T, typename U>
T sum(T x, U y)
{
	return x + y;
}

int main()
{
	sum(12, 5.6); // Return değer int olacağından veri kaybı olur.
}

------------------------------------------------------------------------------------------------------------------------

Çözümlerden biri 3. template parametresi yapabiliriz.Return değerini belirtebiliriz.
Return değerinide bu tür yaparız.Örneği yazmıyorum.
1:13 

------------------------------------------------------------------------------------------------------------------------

DİĞER ÇÖZÜM
x + y nin türü neyse reutrn türüde o olsun diyebiliriz.

decltype(x + y) sum(T x, U y) // Buradaki problem şu. Return değeri x ve y nin scope u dışında kalıyor.
{
	return x + y;
}

------------------------------------------------------------------------------------------------------------------------

BUNU AŞAĞIDAKI GIBI YAZABILIRIZ.TRAILING RETURN TYPE
auto sum(T x, U y) -> decltype(x + y) // BU GEÇERLİ
{
	return x + y;
}

YADA AŞAĞIDAKİ GİBİ YAPABİLİRİZ:
decltype(auto) sum(T x, U y) // BU GEÇERLİ
{
	return x + y;
}

------------------------------------------------------------------------------------------------------------------------

auto main()-> int // Maini böyle tanımlayanlar var.
{

}

------------------------------------------------------------------------------------------------------------------------

Birde function return değeri bir function pointer ise C den gelen sentaks biraz farklı

int(*fp)(int*,int*) bunu bir funcın return değeri yapalım mesela
int(*fp(int))(int*, int*); böyle oldu

using FPTR = int(*fp)(int*,int*);

FPTR foo(int); gibi oldu mesela

Bunu trailing return type ile yapabilirim.

auto foo(int) -> int(*)(int *, int *) //böyle olabilir
auto foo(int) -> FPTR // buda daha rahat yapılır

Dİkkat : auto return type ta decay oluyor.
Dikkat : auto da return türleri hep aynı olmalı yoksa çıkarım yapamıyor.

------------------------------------------------------------------------------------------------------------------------

auto func(int x)
{
	if(x > 10)
		return x;

	return 1.5 * x;
}

SENTAKS HATASI. İNT YADA DOUBLE OLABILIYOR ÇÜNKÜ

------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------

EXPLICIT(FULL) VE PARTIAL SPECIALIZATION
----------------------------------------

EXPLICIT SPECIALIZATION - TEMEL C++ TA AŞAĞIDAKİ AÇIKLAMALAR BU KADAR DETAYLI DEĞİL.İYİ OKU !!!!!
-------------------------------------------------------------------------------------------------
Templateler belirli türler için uygun olabiliyor ama bazı türler için derleyicinin yazdığı kod uygun
olmayabiliyor. Biz bu durumda belirli türlerin farklı bir muamele görmesini istiyoruz.

Burada funclar sözkonusu olduğunda zaten overloading ile bunu yapabiliyoruz.
Burada ana template ve uyum sağlamayanlar için template yazmak veya bunun için gerçek func yazmak

Geren func gerek sınıf şablonları için Ana kod template değilde bizim verdiğimiz diğer template kullanılsın isteyebiliriz.
Burada explicit specialization denmesinin sebebi vereceğimiz alternatif kodda tüm parametreleri
açıkça bildirme zorunluluğu.

Func tempaltes, class templates, class templatelerin member funcları, 
class static data memberları, class templatelerin member template funcları. bunların hepsi
explicit specialization ile farklı bir özel koda sahip olabiliyor.

Function şablonlarında bu bizim tipik olarak tercih ettiğimiz araç değil daha çok sınıf 
templatelerde kullanılıyor. Funclarda bu yapıyı kullanınca bizim beklentimizin dışında farklı
sonuçlar oluşabiliyor. Dolayısı ile alternatif kod oluşturma konusunda daha iyi teknik 
overloading. Bu func şablonlarında explicit specialization kulanılmadığı anlamına gelmiyor.
Neler için explicit specialization verilebilir ? 
Func şablonları, Sınıf şablonları ve variable değişken şablonları için

SENTAKS
-------
template <typename T>
void func(T x)
{
	std::cout << "primary template\n";
}

template<> // buna diamond denitor <>
void func(int)
{
	std::cout << "explicit/full specialization";
}

DİKKAT: Bu specializationun kendisi bir template değil.Derleyicibu function templateinden, template argümanı 
olarak int seçildiğinde, bir kod yazman gerekirse ana kodu değil verdiğimiz kod olacak.Yani alttaki.
Bunun olması için T karşılığı int olması gerekir argümanın. Double olduğunda bizim specialization kullanılmayacak.

TEMELDE GÖRÜLMEDİ BU YENİ ÇOK ÖNEMLİ !!!!!!!!!!!!!!!!!!!
<> ile ilgili çok önemli bir detay

template <typename T>
void func(T x)
{
	std::cout << "primary template\n";
}

template<>
void func<int>(int) // DİKKAT !!!! BURADA <int> OLMASIYLA BUNUN OLMAMASI ARASINDA HİÇBİR FARK YOK.
{
	std::cout << "explicit/full specialization";
}

int main()
{
	func(12);
}

ÇIKTI
-----
explicit/full specialization

--------------------------------------------------------------------------------------------

template <typename T>
void func(T x)
{
	std::cout << "primary template\n";
}

template<>
void func(int) // DİKKAT !!!!  BURADA MESELA <int> YOK AMA YUKARIDAKİ İLE AYNI ANLAMDALAR.
{
	std::cout << "explicit/full specialization";
}

int main()
{
	func(12);
}

ÇIKTI
-----
explicit/full specialization

YANİ ÇIKTILAR AYNI.


------------------------------------------------------------------------------------------------------------------------

ÖR:

template<typename T>
T Max(T x, T y)
{
	return x > y ? x : y; // Burada func return değeri char pointerların sayısal büyüklüğüü olur yazı büyüklüğü değil
						  // Muhtemelen bizim istediğimiz şey değil.
}

Burada char türü için explicit specialization yazmak mantikli.

------------------------------------------------------------------------------------------------------------------------

Burada hoca specializationlar ile ilgili açıklama yaptı.Basic olarak 
vector<int> ayrı vector<double> ayrı sınıflar her biri birer specialization...
swap<int> ayrı swap<double> ayrı bir function.

Explicit specialization benim verdiğim kodu kullan demek. 
Bunu yapmak için tüm template parametrelerinin verilmesi gerekiyor !!!!!!!!!!!!!! ÇOK ÖNEMLİ !!!!!!!
Temelde de gördük

template<typename T, typename U> // 2 TANE TEMPLATE TÜR PARAMETRESİ
class Myclass{

};

template<>
class Myclass<int,double>  // BURASI ÇOK ÖNEMLİ.YUKARIDA 2 TANE TEMPLATE TÜR PARAMETRESİ OLDUĞUNDAN BURADA DA 
{   					   // O 2 TANEYİ BİLDİRMEMİZ GEREKİR.
  
};

------------------------------------------------------------------------------------------------------------------------

FUNCTION TEMPLATE SPECİALİZATİONLARI FUNCTION OVERLOAD RESOLUTIONA DAHİL EDİLMİYOR !!!!!!!!!!!!!!!!

TEMELDE DE GÖRÜLDÜ AMA ÇOK ÖNEMLİ 

template <typename T>
void func(T x)
{
	std::cout << "1\n";	
}

template <>
void func(int *p)
{
	std::cout << "2\n";	
}

template <typename T>
void func(T *p)
{
	std::cout << "3\n";	
}

int main()
{
	int x{98};
	func(&x);
}

CEVAP = 3

2 NEDEN YANLIŞ ÇÜNKÜ BU OVERLOAD RESOLUTİONA KATILMIYOR

3. KOD OLMASAYDI EXPLİCİT SPECİALİZATİON DEVREYE GİRECEKTİ.

------------------------------------------------------------------------------------------------------------------------

ÖRNEĞİ DEĞİŞTİRELİM

template <typename T>
void func(T x)
{
	std::cout << "1\n";
}

template <typename T>
void func(T *p)
{
	std::cout << "3\n";
}

template <>		// BU SPECİALİZATİON ARTIK BİR ÜSTTEKİ ŞABLONA AİT OLDU.BU ŞABLON GÖRÜLÜR DURUMDA ÇÜNKÜ
void func(int *p)
{
	std::cout << "2\n";
}

int main()
{
	int x{98};
	func(&x);
}

CEVAP = 2

2 TEMPLATE SEÇİLİR VE SPECİALİZATİON VAR. BU KOD İNT* TÜRÜ İÇİN OLDUĞUNDAN ARGÜMANLADA UYUŞUYOR. SPECİALİZATİON ÇALIŞIR.

------------------------------------------------------------------------------------------------------------------------

BİR SORU ÜZERİNE : TEMPLATELER LOCAL DÜZEYDE TANIMLAYAMAYIZ. BİR TEMPLATE İ BİR FUNC İÇİNDE TANIMLAYAMAYIZ YANİ.

void foo()
{
	template <typename T>
	void func(T x)
	{
	
	}
}

TÜM TEMPLATE TÜRLERI IÇIN GEÇERLI. HIÇBIRI LOCAL DÜZEYDE TANIMLANAMAZ.
YA NAMESPACE İÇİNDE OLACAK YADA CLASS DEFINITION OLACAK !!!!!!!!!!

void foo()
{
	class Myclass{}; // BU GEÇERLI
}

void foo()
{
	template<typename T>
	class Myclass{}; // BU GEÇERLI DEĞIL !!! HİÇBİR TEMPLATE TANIMLANAMAZ YANİ.
}

------------------------------------------------------------------------------------------------------------------------

ÇOK GÜZEL BİR ÖRNEK

namespace Nec{
	
	template <typename T>
	class Myclass{
	};
};

template<>   // Geçerli bir specialization
class Nec::Myclass<int>
{
	//
}

BU çok sık yapılyor. Mesela hash template i böyle özelleştirildi mesela unorderd set ve mapte

------------------------------------------------------------------------------------------------------------------------

YUKARIDAKİNİN TERSİ OLUR MU ? HAYIR

template<typename T>   
class Myclass
{

}

namespace Nec{
	//Yani burada bir explicit specialization verilebilir mi ? HAYIR
};

Explicit specialization ya aynı namespace içinde yada kapsayan namespace içinde yapılır.

------------------------------------------------------------------------------------------------------------------------

GITHUBDAKI BENZER ÖRNEKLERE BAKILDI.

------------------------------------------------------------------------------------------------------------------------

NOT : PARTIAL SPECIALIZATION FUNC ŞABLONLARI İÇİN YOK SINIF ŞABLONLARI İÇİN VAR !!!

------------------------------------------------------------------------------------------------------------------------

DEĞİŞKEN ŞABLONU ÖRNEĞİ. AYRI BAŞLIK ALTINDA İNCELENECEK

template <typename T>
T x = T{1234.345213254322};

template <typename T>
constexpr T SZ = sizeof(T);


int main()
{
	constexpr auto x = SZ<int>; // x = 4
	std::cout << "x = " << x << "\n";

	constexpr auto x = SZ<double>; // x = 8
	std::cout << "x = " << x << "\n";

	constexpr auto x = SZ<void>; // sentaks hatası void bir incomplete type.Sizeof u alınamaz

}

------------------------------------------------------------------------------------------------------------------------

VOID IÇIN EXPLICIT SPECIALIZATIONA BAKILACAK

template <typename T>
constexpr T SZ = sizeof(T);

template <>
constexpr size_t SZ<void> = 0; // Burada SZ<void> in return değeride size_t.Farklı yapıldı. Değeri ise 0.
								// hangi tür için özelleştirme yapılacağı yine <> içerisinde yazıyor. void.
int main()
{
	constexpr auto val = SZ<void>;
}

------------------------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>

template<typename T>
constexpr T x = T(78234.324234134);

template <>
constexpr int x<int> = 999;

int main()
{
	std::cout << x<double> << "\n";
	std::cout << x<float> << "\n";
	std::cout << x<int> << "\n";
}

MOLA

==================================================================================================================
==================================================================================================================
==================================================================================================================
==================================================================================================================
==================================================================================================================

FUNCTION ŞABLONLARI IÇIN SPECİALİZATİON TERCİH EDİLMİYOR AMA METAPROGRAMMİNG AYRI

template <size_t n>
size_t factorial()
{
	return n * factorial<n-1>();
}

Base Case olarak aşağıdakini yazıyorum.Recursive yapıyı sonalndırmak için
template<>
size_t factorial<0>() // PARAMETRİK YAPISININ TAMAMEN AYNI OLMASI GEREKİYOR.Yukarıdaki template te size_t ise buda öyle olacak
{						// referans ise buda referans olacak.
	return 1;
}

int main()
{
	constexpr auto val = factorial<5>();
}

------------------------------------------------------------------------------------------------------------------------

POWER FUNC TEMPLATE
ÖDEVLERDE VAR YAP.

------------------------------------------------------------------------------------------------------------------------

1-100 arası sayıları döngü kullanmadan yazdırmak ? Buda vardı

struct Myclass{
	Myclass()
	{
		static int x = 0;
		std::cout << ++x; << " ";
	}

};

int main()
{
	Myclass my[100]; // 1 den 100 e kadar yazdırır.
}

------------------------------------------------------------------------------------------------------------------------

Explicit specialization çözümü

template <int n>
struct Myclass : Myclass <n - 1> // hepsi kendi taban sınıfını çağıracak.
{
	Myclass()
	{
		std::cout << n << " ";
	}
}

template <>
struct Myclass<0>
{
	
}

int main()
{
	Myclass<100> my;
}

------------------------------------------------------------------------------------------------------------------------

BUNLAR DAHA ÇOK METAPROGRAMMING TARAFINDA KULLANILACAK TEKNIKLER

PARTIAL SPECIALIZATION FUNC ŞABLONLARI IÇIN YOK SADECE SINIF ŞABLONLARI IÇIN VAR !!!!!!!!!

Yİne bir template kod yazıyoruz bu primary template e alternatif fakat bu alternatif belirli bir tür için değil
bütün tüm argümanları kümesinde daha sınırlı bir tür grubu için verilen alternatif.Mesela Pointer türleri için
referans türleri için dizi türleri için, 1.ve 2. template tür parametrelerinin aynı olması .... gibi.

Not: explicit vepartial specialization aynı interface e sahip olmak zorunda değil !!!!

Burada önemli olan ana template in yani primary template in tür alanını daraltıp o türler için alternatif kod sağlamak.

template<typename T>
class Myclass{
public:
	Myclass()
	{
		std::cout << "primary template\n";
	}
};


BU EXPLICIT SPECIALIZATION.
template<>
class Myclass<int> //  Buradaki <> içindeki tür primary template ile uyuşmalı
{					// yani ana template te 2 tür burada 1 tane olmaz.Sentaks hatası olur.
public:
	Myclass()
	{
		std::cout << "Primary template\n";
	}
};

BU PARTIAL SPECIALIZATION.
template<typename T>
class Myclass<T *> // SENTAKSA DİKKAT  
{					
public:
	Myclass()
	{
		std::cout << "Myclass<T*>\n";
	}
};

BU PARTIAL SPECIALIZATION.
template<typename T>
class Myclass<T [10]> // SENTAKSA DİKKAT  
{					
public:
	Myclass()
	{
		std::cout << "Myclass<T[10]>\n";
	}
};

BU PARTIAL SPECIALIZATION.
template<typename T>
class Myclass<T &> // SENTAKSA DİKKAT  
{					
public:
	Myclass()
	{
		std::cout << "Myclass<T&>\n";
	}
};

int main()
{
	Myclass<long> a1;
	Myclass<int> a2;
	Myclass<int *> a3;
	Myclass<double[10]> a4;
	Myclass<double[20]> a5; //Primary template kullanılır. Partial specialization T[] olsaydı tüm dizi türleri için olurdu
	Myclass<int &> a6; 
}

EXPLICIT SPECIALIZATION ILE PARTIAL SPECIALIZATION BIRARADA BULUNABILIR. TEST ETTİM !!!!!!!!!!!!!!!!!!!!!!!

------------------------------------------------------------------------------------------------------------------------

ÖR:

template<typename T>
class Myclass{
public:
	Myclass()
	{
		std::cout << "primary template\n";
	}
};

template<typename T>
class Myclass<T,T> //SENTAKS HATASI. Çünkü yukarıdaki primary template tür parametresi 1 tane burada ise 2 vermişiz.
{					
public:
	Myclass()
	{
		std::cout << "Myclass<T,T>\n";
	}
};

------------------------------------------------------------------------------------------------------------------------

ÖR:

template<typename T, typename U> // BURAYA BIR TANE TEMPLATE TÜR PARAMETRESI EKLENDI
class Myclass{
public:
	Myclass()
	{
		std::cout << "primary template\n";
	}
};

template<typename T> // BURADA YUKARIDAKİ İLE AYNI SAYI OLMASI DİYE BİR ZORUNDA DEĞİL
class Myclass<T,T>  // BURADAKİ SAYI AYNI OLMALI. ŞİMDİ YUKARIDAKİ SAYI İLE ÖRTÜŞÜYOR. 
{					// <> İÇİNE YAZDIKLARIMIZ PRİMARY TEMPLATE TEKİLERLE ÖRTÜŞMELİ.
public:
	Myclass()
	{
		std::cout << "Myclass<T,T>\n";
	}
};

//FARKLI BIR PARTIAL SPECIALIZATION YAZALIM
template<typename T> 
class Myclass<T,int> //DİKKAT !!!!! 
{					
public:
	Myclass()
	{
		std::cout << "Myclass<T,T>\n";
	}
};

int main()
{
	Myclass<long,char> a1; // primary template
	Myclass<long,long> a2; // 1.partial spec.
	Myclass<char,char> a3; // 1.partial spec.
	Myclass<double,int> a3; // 2. partial spec.
}

------------------------------------------------------------------------------------------------------------------------

DAHA FAZLA TEMPLATE PARAMETRESİNE SAHİP TEMPLATE SPECİALİZATION OLABİLİR

template<typename T>
class Myclass{
public:
	Myclass()
	{
		std::cout << "primary template\n";
	}
};

template <typename T, typename U>
class Myclass<std::pair<T,U>>    // GEÇERLİ. ÖNEMLİ OLAN TEMPLATE ARGÜMANI OLARAK YAZDIĞIMIZ ARGÜMANIN SAYICA TUTMASI
{								// BURADAKİ ARGÜMAN STD::PAIR. YANİ YİNE 1 TANE ARGÜMAN VAR:D:D:D
public:							// DOĞRUDAN <T,U> SENTAKS HATASI
	Myclass()
	{
		std::cout << "Partial spec.\n";
	}
};

int main()
{
	Myclass <int> x; // primary template
	Myclass<std::pair<int,double>> y; // partial spec.
}

------------------------------------------------------------------------------------------------------------------------

NOT : SFINAE SINIF TEMPLATELERİ İÇİN DEĞİL FUNCTİON TEMPLATELERİ İÇİN SÖZKONUSU VE FUNCTION OVERLOADING İLE İLGİLİ.
SUNSTİTUTİONDA SENTAKS HATASI OLURSA İLGİLİ FUNC OVERLOAD SETTEN ÇIKARTILIYOR

------------------------------------------------------------------------------------------------------------------------

SFINAE Yİ ARAÇ OLARAK KULLANMAK İÇİN ÇOK ŞIK BİR HİLE VAR BU ENABLE_IF

Enable if template
------------------
template< bool B, class T = void >
struct enable_if;

int main()
{
	enable_if<false> ; // enable_if<false,void> yazmış oluruz yani 
	enable_if<true> ; // enable_if<true,void> yazmış oluruz yani 

	--------------------------------------------------------------------------------------------------

	enable_if<false>::type  bu yok
	enable_if<true>::type  bu var çünkü <true,void> için partial specialization var.

	--------------------------------------------------------------------------------------------------

	BURADA YAZILDI
	
	template<bool B, class T = void>
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> { typedef T type; };

	--------------------------------------------------------------------------------------------------

	BIR HERHANGIBIR TEMPLATTE ENABLE IF KULLANIRSAK, BURADA ENABLE IF FALSE AÇILIMI SÖZKONUSU ISE VE
	ENABLE IF TYPE INI KULLANIRSAK GEÇERSIZ BIR TÜR OLACAĞI IÇIN SFINAE OUT OLACAK

	template <typename T>
	std::enable_if<true,T>::type foo(T,x) // T türünün kendisini elde etmiş olurum burada
	{
	
	}
	
	template <typename T>
	std::enable_if<false,T>::type foo(T,x) // Burada type olmadığı için geçersiz tür oluşacak yani substitution failure olacak
	{										// ve overload setten çıkarılacak. SFINAE OUT
	
	}

}

------------------------------------------------------------------------------------------------------------------------

ÖR:

//template <typename T>
//std::enable_if<std::is_floating_point_v<T>, T>::type foo(T,x)

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, T> foo(T,x) //aynı şey yukarıdaki ile
{
	return x;
}

int main()
{
	foo(12.4); // Derletici T türünü double olarak çıkardı. FUnctan bir imza elde etmek istedi.
				// enable if in true double açılımının type ı var sentaks hatası olmadı


	foo(12); // burada enable infin birinci template parametresine gönderdiğim değer false olacak ve bunun
			 // type ına erişince böyle tür olmadığından bu sfinae out olacak.Başka bir functa olmadığı için 
			 //sentaks hatası oldu. void foo(double) gibi bir func daha olsaydı veri kaybına rağmen hata olmazdı

}

NOT : CONCEPTLER HERŞEYİ DEĞİŞTİRİYOR.

------------------------------------------------------------------------------------------------------------------------

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, T> foo(T,x) // Tamsayı gönderince bu SFINAE OUT olacak ve aşağıdaki kalacak
{
	return x;
}

template <typename T>
std::enable_if_t<std::is_integral_v<T>, T> foo(T,x) // Reel sayı gönderince bu SFINAE OUT olacak yukarıdaki kalacak.
{
	return x;
}

int main()
{
	foo(12.4);

	foo(124); 
}

*/
