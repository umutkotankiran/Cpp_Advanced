/*

INITIALIZER LIST 
----------------
Derleyici nasıl bir kod üretiyor. header file initializer_list
Aşağıdakine benzer bir kodu var. 

------------------------------------------------------------------
------------------------------------------------------------------
------------------------------------------------------------------
NOT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
INITIALIZER LIST İLERİ C++ TAN TEMEL C++ DERS NOTLARINADA EKLENDİ.
------------------------------------------------------------------
------------------------------------------------------------------
------------------------------------------------------------------

template<typename T>
class initializer_list{
public:
	constexpr initializer_list(const T* fst, const T* lst): first {fst}, last{lst} {  }

	constexpr const T* begin()const {return first;}
	constexpr const T* end()const {return last;}
	constexpr std::size_t size()const {return static_cast<size_t>(last - first);}
private:
	const T* first;
	const T* last;
};

-------------------------------------------------------------------------------------------------------------------------------

#include<initializer_list>

int main()
{
	auto x = {1,3,5,7};

	auto y = x;

	// x.begin() == y.begin() // Bunlar eşit midir. Eşit olmak zorunda

	// &*x.begin() == &*y.begin() // Bunlar eşit midir. Eşit olmak zorunda
}

-------------------------------------------------------------------------------------------------------------------------------

int main()
{
	int a[] = {1,2,3,4};

	initializer_list<int>x(a,a+4); // normal parantez kullanıldı dikkat.

	for(auto val : x)
	{
		std::cout << val << ' '; // 1 2 3 4
	}
}

-------------------------------------------------------------------------------------------------------------------------------

SONUÇLAR
--------
1. Bu light bir sinif
2. 2 pointer elemanı var.
3. constexpr ctoru var.
4. ptr dan ötürü copy maliyeti çok düşük

Kopyalandığı zaman aslında biz init list sınıfı türünden nesnenin copy sini çıkartmış oluyoruz.
Dolayısı ile bir funcın parametresi init list ise aslında func, o init listin işaret ettiği rangeteki
öğelerin copy edilmesi sonucunu doğurmuyor, sadece 2 ptr copy edecek.

İnitializer listin tuttuğu adresteki nesneler const olarak ele alınıyor ve değiştirilemez.


-------------------------------------------------------------------------------------------------------------------------------

İNIT LIST ELIMIZDEYSE NELER YAPARIZ

1.
DÖNGÜYLE DOLAŞILABİLİR.

int main()
{
	//auto x = {1,3,5,7,9}; // Geçerli

	//auto x = {string{"ali"},string{"veli"},string{"ayse"}}; // Geçerli.Derleyici arka planda dizi oluşturmalı. 
															  // aggregate type
	x.begin()
	x.end()
	x.size()

	Bu funclar var.
	for döngüsüylede yapılabilir aşağıdaki gibi range base dor loop ilede yapılabilir.

	------------------------------------------------------------------------------------------------

	for(auto x : {1,3,6,9,12}) // elemanlar sabit ifadesi olacak diye birşey de yok.
	{
		foo(x); // yapılabilir.
	}

}

Dizinin ilk elemanının ve bittiği yerin adresini init list in begin ve end ptr larında tutuyor.
Initializer listteki elemanlar const ele alınır ve değiştirilemez.Contigues range olarak kullanılma garantisi var.

-------------------------------------------------------------------------------------------------------------------------------

2.
SABIT IFADESI OLMAK ZORUNDA DEĞIL.

void func(int a, int b, int c)
{
	auto x = {a,b,c}; // Geçerli
}

-------------------------------------------------------------------------------------------------------------------------------

3.
ADRESLER

void func(std::initializer_list<int> x)
{
	std::cout << "&x = " << &x << '\n'; // 008FF7E0
	std::cout << x.begin() << '\n';		// // 0012FFA48
}

int main()
{
	auto mylist = {1,5,7,8};

	std:cout << "&mylist = " << &mylist << "\n"; // 008ff8d4

	func(mylist);		// 008FF7E0 nesnelerin adresi farklı çıktı

	cout << mylist.begin() << '\n'; // 0012FFA48 ilk eleman adresleri aynı
}

NOT : ELEMANLAR CONST T*.DOLAYISIYLA BEGİN VE ENDİ ÇAĞIRDIĞIMIZDA DÖNEN ADRES CONST T*. DEREFERENCE ETTİĞİMİZDE
	  CONST BİR NESNE OLUYOR. BU DURUMDA SET İŞLEMİNDE SENTAKS HATASI OLUŞUR.

İNIT LIST ADRESLERI FARKLI AMA TUTTUĞU ELEMANLARIN ADRESLERI AYNI !!!!!!!!!

-------------------------------------------------------------------------------------------------------------------------------

4.
CTORLAR ARASI FARKLILIK

int main()
{
	initializer_list<int> x{1,2}; // Bizden listedeki öğeleri istiyor.
	initializer_list<int> x(1,2); // Bizden range istiyor
}

-------------------------------------------------------------------------------------------------------------------------------

Yerel bir dizi ile init listi hayata getirirsek dangling hale getirme ihtimalimiz var mı? 
VAR. İnit listi kullandığımız yerlerin neredeyse hepsinde.

-------------------------------------------------------------------------------------------------------------------------------

İKİ NOKTAYA DİKKAT ETMEK GEREKLİ

Dilin kuralları arasındaki bir uyumsuzluk.Normalde auto type deduction ile template argument deduction aynı
Sadeec initializer listte farklılık var.

template<typename T>
void func(T x);

int main()
{
	func(10); // int türü. Kurallar auto ile aynı

	------------------------------------------------------------------------------------------------

	auto x = {1,4,5,6}; // Burada auto yerine initializer_list<int>

	func({1,3,5,7,9}); // Burada initializer_list<int> değil.Scott Meyers bunu eleştiriyor

}

-------------------------------------------------------------------------------------------------------------------------------

DİĞER NOKTA. BİR SINIFIN INIT LIST CTORU OLMASI DURUMU

class Nec{
public:
	
	// BU CTOR ILE NE YAPILIR
	Nec(std::initializer_list<int> list) : mvec{list}{ }	// vectorun init list parametreli ctoru var.İnit edilebilir.
	// DATA MEMBER STL CONTAINERDIR VE CTOR INIT LIST ILE INIT EDILIR

	------------------------------------------------------------------------------------------------
	
	//INIT LIST DOLAŞILABİLİR
	Nec(std::initializer_list<int> list) 
	{
		for(auto x : list)
		{
			//x...
		}
	}

	------------------------------------------------------------------------------------------------

	Nec(std::initializer_list<int> list) : mx(std::count(list.begin(),list.end(),7)) //buda olabilir
	{
	
	}

private:
	std::vector<int>mvec;
	int mx;
};

-------------------------------------------------------------------------------------------------------------------------------

int main()
{
	auto list = {2,5,7,1,9};
	auto list = {2,5,7,1,9};

	vector<int> ivec(list); // Burada init list parametreli ctor kullanıldı
	vector<int> ivec{list}; // Geçerli

}

-------------------------------------------------------------------------------------------------------------------------------

int main()
{
	{x,y,z,t} // içindekilerin(x,y,z,t init list olsaydı) kendisi init list olsaydı vectore argüman göndersekdik 
				// vector<intializer_list> olmalıydı

}

-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------

class Myclass{
public:
	Myclass(std::initializer_list<int>)
	{
		std::cout << "ilist\n";
	}

	Myclass(int)
	{
		std::cout << "int\n";	
	}

	Myclass(int,int)
	{
		std::cout << "int int\n";	
	
	}

};

int main()
{
	Myclass m1{1}; // Burada init listin önceliği var

	Myclass m1{1,2}; // Burada init listin önceliği var yine

	Myclass m2(1); // Burada int ctor çağrılır
	
	Myclass m3(1,2); // Burada int int ctor çağrılır
}

-------------------------------------------------------------------------------------------------------------------------------

int main()
{
	vector<int>ivec1(10); // size_t parametreli ctor
	vector<int>ivec2(10,3); // size_t,const&t parametreli ctor

	vector<int>ivec1{10}; // init list parametreli ctor
	vector<int>ivec1{10,3}; // init list parametreli ctor

	vector<int> vec = 10; // Sentaks hatası Size_t parametreli ctor explicit 

	string str(10,'A'); // Fill ctor
	string str{'A'}; // Initalizer list parametreli ctor. char değil.

	initializer_lisat<int> x{3.4}; //Narrowing conversion.Sentaks hatası

	auto x = {1,4,5,4u}; // Sentaks hatası

}

-------------------------------------------------------------------------------------------------------------------------------

void func(std::initializer_list<int>&) // bu gereksiz. referansa gerek yok.
{
	
}

-------------------------------------------------------------------------------------------------------------------------------

int main()
{
	initializer_list<int> x{}; //Geçerli

	std::cout << x.size(); // 0
}

-------------------------------------------------------------------------------------------------------------------------------

int main()
{
	std::initializer_list<int> x{1,5,8,12};

	x.rbegin(); // Bu func yok

	auto iter = rbegin(x); // Global olanı var
}

===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================
===============================================================================================================================


DESIGN PATTERNS
---------------
Tüm paternler bir problemi açıklıyor.Ortada bir problem olması gerekiyor.
1 - Tekrarlanan bir problem olmalı.
2 - Çözümün temelini tanımlıyor.
3 - Bu çözüm birçok kere kullanılabilir ama hiçbiri aynı olmayabilir.

OOP design patternda bazı problemler sürekli karşımıza çıkıyor.
Mesela strateji paterni birçok STL algoritmasında kullanılıyor...

- Tasarım kalıpları problemlerin çözümü için programlama dili ve programlama domaininden bağımsız olan stratejilerdir.
Böyle densede tamamen böyle olmayabilir. Mesela RAII bir idiyom mu pattern mi? İkisinide söyleyen var.

- Geçmiş deneyimlerden devşiriliyor

- Bu problemler her uygulamada karşımıza çıkma eğiliminde

- Her bir tasarım kalıbı bir çözüm önerisi sunuyor.

- Geliştiriciler bu önerileri bir kılavuz niteliğinde ele alıp kendi problemleri için çözüm oluşturuyorlar.

- Öneriler, programlama diline, kullanılan frameworke yada spesifik bir platforma bağlı olarak 
  farklı implementasyon seçenekleri olabilir.

Bir uygulama yada sistemin tasarımında ortaya çıkan tekrarlanan problemlerin format hale getirilmiş best practice leri

-------------------------------------------------------------------------------------------------------------------------------

TEKRARLANAN PROBLEMLER NELER ?
------------------------------
-Nesneler arası emir komuta zinciri oluşturmak

-Çok sayıda nesne arasında iletişim sağlamak

-Bir nesneye runtime da ilave işlevsellik ve yetkimlik kazandırılabilir

-Karmaşık durum bilgilerine sahip nesneler nasıl yönetilebilir.

-Bir nesnenin durum bilgisi nasıl saklanabilir.

-Birden fazla aynı strateji ile nasıl yönetilebilir.

-Aynı operasyon birden fazla nesneye nasıl uygulanabilir

-Bir sınıf nesnesi bir işlem gerçekleştirdiğinde başka türlerden sınıflar bu durumdan nasıl haberdar edilebilir

-Bir nesnenin bir işi gerçekleştirmesi için kullandığı algoritma yada strateji nasıl değiştirilebilir.

-------------------------------------------------------------------------------------------------------------------------------

PROBLEM DERKEN KASTEDİLEN NEDİR ?
---------------------------------
Problem,
- Paternin hangi durumda uygulanacağını tanımlar.

- Problemi ve problemin içeriğini tarif eder.

- problem spesifik bir tasarım problemini tanımlıyor olabilir.

ÖRNEK: Bir algoritma bir nesne olarak nasıl temsil edilebilir.

- Bazı durumlarda çözümün uygulanması için sağlanması gereken koşulları açıklaması gerekir.

-------------------------------------------------------------------------------------------------------------------------------

ÇÖZÜM DERLEN NE KASTEDİLİYOR
----------------------------
- Tasarımı oluşturan elemanları, bunların ilişkilerini sorumluluklarını ve işbirliklerini tanımlar.

- Çözüm soyuttur. Çözüm somut bir implementasyona veya tasarıma ilişkin değildir

- Bir pattern bir şablon gibi birçok durumda kulanılabilir. Bu sebeple soyut

- Patern, tasarım problemlerinin ve onu çözün elemanların genel düzeninin soyut bir tanımıdır. !!!!!!!!!!!!!!!!!!!!!!!! 

-------------------------------------------------------------------------------------------------------------------------------

SONUÇLAR/AVANTAJLAR
-------------------
- Paterni uygulamanın sonuçları ve trade-off larıdır (ne kazanırız ne kaybederiz)

- Bunlara göre farklı patern alternatiflerini yada bunların maaliyetlerini değerlendirip bunların arasında seçim yapabiliriz.

- Bunlar programlama diline yada implementasyona ilişkinde olabilir.

- Sistemin esnekliği tekrar kullanılabilirliği genişliğiveya taşınabilirliği üzerinde belirleyici olabilir.Bu yüzden
  tercih edilebilir yada edilemez.

Design pattern, nesne yönelimli sistemlerdeki tekrar eden tasarım problemlerini isimlendirip, açıklayıp, çözümler sunar. !!!!!!

- Önce problemleri tarif eder sonra çözümü, ne zaman çözümün uygulanacağını ve sonuçlarını açıklar.Ayrıca implementasyona
  yönelik örnekler ve ipuçları verir.

- Çözüm sınıfların ve nesnelerin genel bir düzenlenmesi.

- Çözüm farklı contexlerde değiştirilebilir.


-------------------------------------------------------------------------------------------------------------------------------

NEDEN TASARIM KALIPLARI
-----------------------
İstediğimiz flexible reusable couplingi düşük yapılar oluşturmak. Karşımıza çıkan problemleri kendi düşüncemize uygun çözümleri
uygularsak tipik olarak bunlar endüstride kullanılmış best practice çözümler gibi iyi olmaz.

Genel istenilen nitelikleri sağlamama ihtimali çok yüksek kendimiz istediğimiz gibi yaparsak.

Değişikliğe karşı direncin azaltılması
reusable olması,
maintainable olması,
scalable olması,
robust olması... Tasarım kalıpları bunları sağlıyor.

Birde formal bir dil oluşturuyor. Abstract factory deyince ne denildiği anlaşılıyor.Uzun uzun anlatmaya gerek yok.
İletişim daha kolay oluyor.

Bir paterni bu terminoloji olmadan anlatmak çok uzun ve zor.

Daha bir çokta sebebi var.

-------------------------------------------------------------------------------------------------------------------------------

TASARIM KALIPLARI 3 ANABAŞLIĞA AYRILIYOR

1. Creational Patterns
----------------------
Varlık nedenleri nesne oluşturmak. Nesneleri doğrudan oluşturmak yerine couplingi azaltacak şekilde nesne oluşturan 
sınıf yapılarıyla ilgili. Daha flexible bir yapı sağlıyorlar

BUNLARDA IKIYE AYRILIYOR
Class creational patterns : Nesnesi oluşturulan sınıfın kalıtım yoluyla değiştirilmesine yönelik.

Object creational patterns : Nesne oluşumunu başka objeye delege ediyorlar. 

2. Structural Patterns
----------------------
Daha geniş yapıları oluşturmak için sınıflar ve nesneler nasıl biraraya getirilmeli.
Esnekliği ve verimliliği koruyarak nesneler ve sınıfların daha büyük yapıları oluşturmak için nasıl biraraya getirileceğini 
tanımlarlar

Class structural design patterns : Kalıtım mekanizmasını kullanarak sınıfların interfacelerini birleştirerek
yapıları genişleten paternler.

Object structural design patterns : Nesneleri birleştirerek onlara yeni işlevler kazandırma yollarıyla ilgli

3. Behavioral Patterns
----------------------
Birden fazla sınıfın bir işi yerine getirirken nasıl davranacağını belirlerler. 
Nesneler arasında etkin bir iletişim ve sorumluluk atamasının yapılmasını sağlarlar.

-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------

SINGLETON
---------
Bir sınıf türünden sadece tek bir nesne olmasını garanti altına alacağız ve bu nesneye tüm kodların global erişimi olacak.
Bu problemli biraz.

Problem domaininde bazı nesnelerden bir tane olması gerekiyor.File sistem kodundan bir tane olması gerekiyor gibi.
İleride koşullar değişince 2. bir nesne daha gerekebilir. Bir sınıf türünden bir nesne olması sınıfın kendi işi olmamalı.
Çok tartışma var konu hakkında.

Biz zaten global değişkenler yada global state kullanmak istemiyoruz.

Baya bir açıklaması var ama yazmadım.

SINGLETON FAYDALARI OLARAK KITAPTA YAZANLAR 1:47

- Tek nesneye kontrollü erişim sağlaması.

- Global değişkenlerin iyileştirilmiş hali.

- Singleton class subclass olabilir.Runtime da yapılabilir.

- Singleton olarak başlayıp sonra değişken sayıda nesneyede geçilebilir.Pratikte iş çok zor oluyor tabi.

- Daha esnek sınıf operasyonları sağlıyor. 

Bu maddeler bugün çok tartışmalı

-------------------------------------------------------------------------------------------------------------------------------

ELEŞTİRİLER
-----------
Tek bir nesne konusunda erken taahhütte bulunmuş olunuyor.Kodu rigit hale getiriyor.

Bağımlılıklar gizlenmiş oluyor.Bir veya daha fazla singleton barındıran bileşen, hayati önem taşıyan bağımlılığı gizliyor.
Bağımlılıklarını bşilirsek requirementlarını daha iyi biliriz.Bu çok kötü.

SRP / Single resposibiliy principle çiğnenmiş oluyor.

En kötü durum unit testler. Singleton içeriyorsa kod iş çok zor.

Multithread sorunu.Global bir değişkene eriştiğimiz için sınıfın tüm veri elemanları birden fazla thread den aynı zamanda
okuma yazma yapmasına karşı korumalıyız. Global değişkenler concurrency friendly değil.Yönetimi zor

Birbirinden farklı singleton nesnelerinin birbirine bağımlı hale gelmesi.

Standart İmplementasyon yok.

Bazı sorular var, desturction gerekli mi, initializetion thread safe olacak mı kalıtıma uygun olacak mı .....

...

-------------------------------------------------------------------------------------------------------------------------------

NEDEN SINGLETON BU KADAR ÇOK KULLANILIYOR?

Mecburiyet senaryoları ağır basıyor.Programcıya singleton zaman kazandırıyor. Bir feature yazacağız diyelim ama projede de
birsürü sınıf var. Bizim o featureı eklememiz iin varolan sınıflarlada iletişim gerekiyor.Diğer sistemlerde karmaşık yapıda.
Bunlar bizim sınıfımızda veriler iletecek. Normalde sınıfı oluşturup iletişim sağlamak için diğer sınıfların funclarına parametrelerle
bir aktarım gerekiyor fakat bunu yapmanın maaliyeti yüksek.Bunla uğraşmak yerine singleton yapıyorlar.


SINGLETON IMPLEMENTASYONUNA GEÇMEDEN ÖNCE STORAGE DURATION VE LIFE TIME GÖRÜLECEK BUNLAR ÇÖOK ÖNEMLİ VE
SİNGLETONDA DA KULLANILIYOR.

-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------

STORAGE DURATION VE LIFE TIME
-----------------------------
Bunlar birbiriyle ilişkili ama farklı kavramlar.

Storage Duration
----------------
Nesnemizin runtime da bir yere ihtiyacı var.Dolayısı ile o yerin bir şekilde allocate edilmesi gerekiyor.
Bu nesnenin yeri runtimeda ne zaman elde ediliyor ve ne zaman geri veriliyor ?
Bunu belirleyen storage duration

Nesnenin hayata gelmesiyle onun yerinin edinilmesi farklı şeyler.Nesnenin yerinin elde edilmesi
storage duration ile ilgili. Nesnenin hayata gelmesi değerini alması ise initialization ile ilgili.

Automatic Storage Duration 
--------------------------
Programın akışı sözkonusu nesnenin bulunduğu kod bloğuna girdiğinde onun yeri ayrılıyor.
Bloğa girdiğinde ayrılıyor, tanımına geldiğinde değil

{  // Storage ı burada elde ediliyor automatic storage durationda

	T x;

} // Burada ise bu yer geri veriliyor

Allocation ile initialization birbirinden ayrılmalı.Farklı kavramlar.


Dynamic Storage Duration
------------------------
Programın akışı new ifadesine geldiğinde yer elde ediliyor yada ayırt ediliyor.
Programın akışı delete ifadesine elde edilmiş yer geri veriliyor.

Nesnenin hayatından bahsetmedik yerinden bahsettik.


Thread Local Storage Duration
-----------------------------
Bizim bir nesnemiz var thread_local ile tanımlanmış.Bu nesnenin yeri thread nesnesi oluşturulduğunda başlıyor.
thread join edildiğinde yeri geri veriliyor.Eski C++ta bu yok zaten bu yeni.

thread_local x;
std::thread t{};

t.join();


Static Storage Duration
-----------------------
Program çalışmaya başladığında storage başlıyor. Çalışması bittiğinde ise storage sona eriyor.


-------------------------------------------------------------------------------------------------------------------------------

Burada problem olan static storage class. Projelerde çok hata yapılıyor.

class neco{
public:
	Neco()
	{
		std::cout << "";	
		//..				
	}
};


Buradaki kodlar global bir state i kullanıyor olabilir.Buradaki problem şu. Ctorun kodunun çalışması sırasında cout nesnesinin
hayata gelmiş kullanılabilir durumda olduğundan emin olabilir miyim ? Ya da aşağıdaki örneğe bakalım

extern Nec nec;

A x = nec.foo(); // x bir global değişken. ya bu global değişken hayata geldiğinde nec construct edilmediyse sorun olur
				 // yukarıdaki örnekte de neco ctoru static ömürlü global bir neco için çağrıldıysa fakat o init edildiğinde
				 // halen cout init edilmediyse problem.

-------------------------------------------------------------------------------------------------------------------------------

HANGİ NESNELERİN STORAGE'I STATIC ? 
-----------------------------------
1 - Namespace scope ta tanımlanan değişkenler.İllaha global namespace olmak zorunda değil

ÖR:
namespace A{
	X ax; // Buda static storage a sahip.

}


2 - Sınıfların static veri elemanları

ÖR:
class Myclass{

	static A ax;
};


3 - Static local/yerel değişkenler

ÖR:
void f()
{
	static double dx;
}

BUNLARIN STORAGEI STATİC YANİ PROGRAM ÇALIŞMAYA BAŞLAMADAN MAİN BAŞLAMADAN HAYATA GELİYOR.
STATIC YEREL NESNELER PROGRAMIN AKIŞI ORAYA GİTTİĞİNDE İNİTİALİZE EDİLİYOR.

-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------

LIFE TIME
---------
Tipik olarak constructoru ne zaman çağrılacak destructoru ne zaman çağrılacak demek.Yani initialization demek.
Sonuç olarak life time nesnenin initialize edilmesiyle ilgili.Ne zaman construct edileceğini belirliyor.

Hayata başlaması başka ama yerinin ayrılması başka.

Otomatik ömürlü nesneler tanımı neredeyse orada hayata başlıyor.

-------------------------------------------------------------------------------------------------------------------------------

Static ömürlü nesneler için eğer global değişken ise life time/initializationu 2 ayrı aşamada oluyor

1. aşama static initialization
2. aşama dynamic initialization(Dynamic storage değil bu)

1. Static initialization
------------------------
Nesnenin bellek alanının sınıflanması. Zero initialization deniyor buna.Allocate edilen bellek bloğu sıfırlanıyor.

2. Dynamic İnitialization
-------------------------
İlk değer veren ifade yürütülüyor ve nesnenin hayata gelmesi bu şekilde tamamlanıyor.

birinci aşamada (static initializationda) bellek bloğu sıfırlanıyor.
İkinci aşamada (dynamic initializationda) o ilk değer veren ifade yürütüldü.

ÖR:

Myclass x = expr; // global değişken olsun. Global değişken olduğundan static storagea sahip.
					// önce static initialization yapılacak. x nesnesi için ayrılan storage sıfılanacak..
					// primitive tür için 0, ptr için nullptr, sınıf için sadece sıfırlama.
					// Dynamic init aşasında x in ctoru çağrılacak.


Farklı kaynak dosyalardaki static storage a sahip nesnelerin dynamic initializationu dil tarafından garanti edilen bir sırada değil.
Birden fazla kaynak dosyada static storage a sahip n tane nesne için bunların dynamic initi dil tarafından öngörülen bir sıraya
sahip değil. Derleyiciye bağlı. Bunlar arasında birebirini kullanma ilişkisi varsa hiçbir garanti yok.
Dynamic initialization sırası yukarıdan aşağıya bildiğimiz gibi sırayla gidiyor.


a.cpp										b.cpp
std::ofstream ofs("ali.txt");				extern Myclass m;
Myclass m(ofs);								string name = m.foo();	// Sorun burada.


Farklı translation unitlerdeki global değişkenlerin dynamic initialization sırası kesinlikle garanti altında değil
Bu ne demek. Örneğin derleyicinin ürettiği kodda bu initialization runtime da yapıldığında m nesnesi henüz initialize edilmemiş olabilir.
Bu runtime hatası yada UB olabilir. Derleyiciler garanti verebilir ama dil bir garanti vermiyor.
Bu kod kesinlikle sorunlu.

Bir sonraki derste devam edilecek. Hatta bu kısmı bir sonraki dersede yapıştıracağım devam etmesi için

*/
