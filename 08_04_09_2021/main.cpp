/*

Lambda ifadeleri üzerinde çalışıyorduk.Yeni kurallarda fonksiyon cosntexpr koşullarını sağladığında doğrudan constexpr olarak ele alınıyordu. 
Constexpr koşullanı sağlamazsa constexpr olarak ele alınmıyor. Constexpr olmasını engelleyen bir durumla karşılaşılmadığı sürece function constexpr

NOEXCEPT
--------
Noexcept için böyle değil. Defaultta noexcept kabul edilmiyor.


int main()
{
	auto f = [](int x){ return x + 5; }; // Bir ifadenin noexcept olup olmadığını anlayabiliriz. Noexcept specifier ile noexcept operator farklı şeyler.
	noexcept(f(5)); // f(5) ifadesi noexcept ise compile time da boolean true döndürecek alsi taktirde false döndürecek
	
	constexpr auto b1 = noexcept(f(5)); //false çıktı.Noexcept operatorü compile time operatoru

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	auto f = [](int x) noexcept { return x + 5; };
	
	constexpr auto b2 = noexcept(f(5)); //True çıktı
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LAMBDA FUNCLARIN PARAMETRESİ DEFAULT ARGUMAN ALABİLİR

int main()
{
	auto f = [](int x = 10){return x*5;}; // GEÇERLİ
	
	f(20); // GEÇERLI  100
	f() // GEÇERLI	50
}  

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

SORULAR

1.Soru

int g = 77;

auto fx = [=]{ return g + 1};
auto fy = [g = g]{ return g + 1};

int main()
{
	g = 500;
	
	// ÇIKTILAR NE OLUR ??
	std::cout << fx() << "\n";  // 501
	std::cout << fy() << "\n";  // 78
}

- fx teki g doğrudan global değişken olan g.Global olan değişkenler static ömürlüler capture
edilmiyor.Globaldkeki g 500 e eişlendi ve lambda da 500 olan değer 501 yapıldı

- fy de lambda init capture uygulanmış. Buradaki g artık g nin kendisi değil.
Buradaki g değerini global değişkenden alan sınıfın veri elemanı. Daha sonra g nin
500 olması etkilemedi bunu.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

2.Soru

int main()
{
	auto x = []{ static int a{}; return ++a; };
	auto y = []{ static int a{}; return ++a; };
		
	std::cout << x() << x() << x() <<"\n"; // 1 2 3
	std::cout << y() << y() << y() <<"\n"; // 1 2 3
}

Farklı sınıf nesneleri bu yüzden her a bir farklı değişken.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

3.Soru

int main()
{
	auto x = []{ static int a{}; return ++a; };
	
	decltype(x) y;
	decltype(x) z;

	std::cout << x() << x() << x() <<"\n"; // 1 2 3
	std::cout << y() << y() << y() <<"\n"; // 4 5 6
}

Bu kod C++20 ile legal.Stateles lambdalar için Default ctor delete edilmiş değil.
C++17 olsaydı SENTAKS HATASI OLURDU.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

4.Soru

int main()
{
	auto x = []{ static int a{}; return ++a; };

	auto y = x;
	auto z = x;

	std::cout << y() << y() << y() <<"\n"; // 1 2 3
	std::cout << z() << z() << z() <<"\n"; // 4 5 6
}

Yine 3. sorudaki aynı cevabı verir.Burada copy ctor var. Y değişkenini X ten init ediyoruz.
Lambdaların copy ctorları zaten var. C++17 ile ilgisi yok.

C++20 ile eklenen default ctor ve copy assignment functionları
Burada da lambdalar stateless /captureless olmalı.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

5.Soru

int main()
{
	auto up = std::make_unique<int>(10);

	auto f = [up = std::move(up)]{ std::cout << *up << "\n"; };

	//auto g = f; SENTAKS HATASI ÇÜNKÜ KOPYALAMA VAR.UNIQUE PTR DA COPY SENTAKS HATASIDIR
}

HATADA DELETED FUNCTIONA ÇAĞRI YAPTIN DIYOR. NEDEN DELETE EDILDI ÇÜNKÜ COMPILERIN YAZDIĞI SPECIAL MEMBER FUNCTA
BIR SENTAKS HATASI OLUŞTU. BU HATA OLUŞTUĞU IÇIN COMPILER BU FUNC I DELETE EDER.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

6.Soru

 int main()
{
	auto up = std::make_unique<int>(10);

	auto f = [up = std::move(up)]{ std::cout << *up << "\n"; };
	
	auto g = std::move(f); //GEÇERLİ
}

===================================================================================================================================================================================
===================================================================================================================================================================================
===================================================================================================================================================================================

GENERALIZED LAMBDA EXPRESSION
C++14
Derleyicinin oluşturduğu closure type a ilişkin func call operator func ın template olarak
yazılması isteği.


int main()
{
	auto f = [](auto x){ return x + x; };
}

%100 AYNISINI DEĞİL AMA NASIL BİR KOD ÜRETTİĞİNE BAKALIM

class xyz123_t{
public:
	template <typename T>
	auto operator()(T x)const
	{
		return x + x;
	}
};

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	auto f = [](auto x){ return x + x; };
	
	f(12); // yaparsak tür çıkarımı yapılacak.Burada int
	f(4.5); // BUrada ise T nin türü double

}

auto x, auto &x, const auto &x, auto &&x hatta variadic parametre bile kullanabiliriz

Variadic örneği.
auto f = [](auto ...args){  };

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AUTO KULLANMANIN FAYDALARI NELERDİR

1 - Yazım kolaylığı.Uzun uzun türü yazmak karışık olabilir.Uzun yazmak yerine kullanılabilir.
2 - Tür değiştiğinde bizim lambda ifadesini değiştirmemiz gerekiyor.


int main()
{
	auto get_size = [](const auto &c) { return c.size(); };
	vector<int> ivec(100);
	list<double> dlist(20);
	
	std::cout << get_size(ivec) << "\n"; // 2 türlüde kullanabildik
	std::cout << get_size(dlist) << "\n"; // Burasıda öyle
}

Forward list enteresan bir container. Tanımındaki ana gayelerden biri C de yazılmış singly list koduna göre bir maliyet içermemesi.Bunun sonucunda size functionu
constant time da olamayacağından sınıfın size functionu yok. Size için constant time değil containera dolaşmamız gerekiyor. Bunun için kod örneği yazıldı ben yazmıyorum

Ufaktan forward liste girdi hoca. Yazmıyorum.
before begin, erase after konuları...

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

GENERALIZED LAMBDA EXPRESSION ILE ILGILI BIR KOD DAHA 

void func(vector<shared_ptr<string>> &s)
{
	//
	sort(s.begin(), s.end(),[](const shared_ptr<string> &x, const shared_ptr<string> &y)
	{
		return *x < *y;
	}
	
	for_each(s.begin(), s.end(),[](const shared_ptr<string> &s)
	{
		if(s) std::cout << *s << "\n"; 
	}
}


Yukarıdakinin yerine aşağıdakini yazabilirim.

void func(vector<shared_ptr<string>> &s)
{
	//
	sort(s.begin(), s.end(),[](const auto &x, const auto &y)
	{
			return *x < *y;
	}
	

	for_each(s.begin(), s.end(),[](const auto &s)
	{
			if(s) std::cout << *s << "\n";
	}
	
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AUTO İLE İLGİLİ ÖNEMLİ NOKTA, PERFECT FORWARDING AUTO İLE YAPILABİLİYOR.

template <typename ...Args>
void vprint(Args&&... args)
{
	std::initializer_list<int>{((std::cout << std::forward<Args>(args) << "\n"),0)...}; //variadic funcları yazmak için 3 teknik vardı.Bu onlardan biriydi.
}

int main()
{
	auto f = [](auto&& ...args){
		vprint(std::forward<decltype(args)>(args)...);
	}

	f(12,3.4,"Alican");
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BURADA BAZI PROBLEMLER VAR. 

int main()
{
	[](auto &x){
		
		// decltype(x); Buradan elde edilen tür bir referans türüde olabilir. O yüzden bu türün value type diyemeyiz doğrudan.Böyle durumlarda birde decay templatei kullanmalıyız.
	}
	
	[](auto&x){
		decay_t<decltype(x)>::value_type; // Buda okuma ve yazma zahmeti getiriyor. C++20 ILE GELEN TEMPLATED LAMBDA BURAYI ÇÖZÜYOR
	}
}

NOT : DECAY AUTOYA KARŞILIK GELEN TÜRÜ VERIYOR. CONST AUTO X = 23;  BURADA X İN TÜRÜ CONST INT AMA AUTOYA KARIŞIK GELEN TÜR INT !!!!!!!!!!!!

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BELİRLİ IDIYOMLAR
-----------------

#include<vector>
#include<algorithm>

int f(int);
int f(long);
int f(double);

int main()
{
	using namespace std;

	vector<int> ivec(100);
	vector<int> xvec(100);

	transform(ivec.begin(),ivec.end(),xvec.begin(), f); // YUKARIDA 1 FUNC OLSAYDI BIR SENTAKS HATASI YOKTU
							// Aynı isimli birden fazla overload olduğunda range deki öğeler int olduğuna göre derleyici parametresi int olan funcı 
							// seçecek diyemeyiz.Buradaki ambigiuty yi çözmenin yollarından biri static_cast ile tür dönüşümü

	
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	TÜR DÖNÜŞTÜRME İLE ÇÖZÜLEBİLİR

	transform(ivec.begin(),ivec.end(),xvec.begin(), static_cast<int(*)(int)>(f)); // veya int(*)(long)
	
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	LAMBDA İFADESİ KULLANARAK ÇÖZÜLEBİLİR

	transform(ivec.begin(),ivec.end(),xvec.begin(), [](auto x){ return f(x);}); // Derleyici ivec teki elemanların int olduğunu bilecek.x inde türünü çıkarabilecek.
										// int ise x te int olacak. functionda int parametreli seçilecek.

}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

SINIFLARIN MEMBER FUNCI İÇİNDE LAMBDA İFADESİ KULLANIMI

-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
ÇOK ÖNEMLİ !!!!!!!!!!!!!!!!!!!!!!!!!!
FOO İÇİNDE MX KULLANINCA DERLEYİCİ ONU THİS->MX E ÇEVİRİYOR.
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------

Lambda funcın kodu içinde this pointerını kullanamıyoruz.Oradaki this keywordu sınıfın member kodunu yazdığımızdaki this ile aynı değil.

class Myclass
{
public:
	Myclass() = default;
	Myclass(int a, int b): mx{a}, my{b} {}
	void print()const
	{
		std::cout << "(" << mx << ") (" << my << ")\n";
	}

	void foo()
	{
		auto f = [](int x) { return x *(mx + my);}; // MX ILE MY BÖYLE KULLANILAMAZ
		auto f = [mx,my](int x) { return x *(mx + my);}; // MX ILE MY BÖYLE CAPTURE EDILEMEZ YINE

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
		
		FOO İÇİNDE MX KULLANINCA DERLEYİCİ ONU THİS->MX E ÇEVİRİYOR. BİR LAMBDA İÇİNDE MX KJULLANABİLMEM İÇİN THİS POINTERINI CAPTURE ETMELİYİM
		1.  [=] DEPRECATED EDİLDİ. THİS POINTERINI COPY İLE CAPTURE EDİYORUZ. YİNE ADRESİNİ CAPTURE ETMİŞ OLDUK YANİ
		2.  [&] THİS POINTERINI REFERANS İLE CAPTURE EDİYORUZ.THIS BIR POINTER OLUNCA COPY YADA REFERANS CAPTURE ETMEK ARASINDA FARK KALMIYOR.
		3.  [this] BUDA THIS CAPTURE

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
		
		[&] İLE CATCH EDİLİNCE DİKKATLİ KULLANILMAZSA DANGLING REFERANS OLUŞABİLİYOR. 
		THISIN CAPTURE EDİLMESİ DEMEK *THIS İN REFERANS YOLUYLA CAPTURE EDİLMESİ DEMEK.BURASI KARIŞIK
		
		ÖR:
		auto f = [this]{++mx; ++my;}; //GEÇERLİ. ÇÜNKÜ SINIFIN VERİ ELEMANINI DEĞİL THISIN GÖSTERDİĞİ NESNENİN VERİ ELEMANINI DEĞİŞTİRİYORUM.
		f();
		
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

		auto f = [starthis = *this]()mutable {
			starthis.mx = 10; //Mutable yazmasak func const olacağından sentaks hatası olurdu
			starthis.my = 10; //Mutable yazmasak func const olacağından sentaks hatası olurdu
		}			

		f(); // Burada ise veri elemanı değilde kopyası üzerinde değişiklikler yapıldı.

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

		C++2017 İLE *THIS CAPTURE EDİLEBİLİYOR

		auto f = [*this]()mutable {
			mx = 10; // bunlar kullanılabilir.
			my = 10; 
		}			

		f();


	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
		
		C++20 ILE AŞAĞIDAKILER LEGAL

		[=,this] 
		[&,this] 
		[&,*this] 
		[=,*this] 
		....

	}

private:
	int mx{}, my{};
}

int main()
{
	Myclass m{12,65};

	m.print();

	m.foo();
	
	m.print();
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

POSİTİVE LAMBDA IDIYOM

int main()
{
	int ival{3};

	int (*fptr)(int) = [](int x){return x * x;}; //GEÇERLİ.
	
	int (*fptr)(int) = [ival](int x){return x * x;}; //SETAKS HATASI. STATELESSLARDA GEÇERLİ

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	auto f = [](int x){return x * x;}; //GEÇERLİ. İfade closure type türünden bir pr value expression. f burada func pointer değil.
					   // f burada closure type türünden sınıf nesnesi. 

	constexpr auto bx = is_pointer_v<decltype(f)>; //false. Pointer türü değil.
	constexpr auto by = is_class_v<decltype(f)>; //True. Sınıf türü.

	std::cout << typeid(f).name() << "\n";
	std::cout << bx << " " << by << "\n"; // 0 1
	
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	auto f = +[](int x){return x * x;}; //DİKKAT STATELESS OLMALI POSITIVE LAMBDA İÇİN

	constexpr auto bx = is_pointer_v<decltype(f)>; // TRUE
	constexpr auto by = is_class_v<decltype(f)>; // FALSE

	std::cout << typeid(f).name() << "\n";
	std::cout << bx << " " << by << "\n"; // 1 0

}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

IIFE IDIOM - IMMEDIATELY INVOKED FUNCTION/LAMBDA EXPRESSION
Yoğun kullanılan bir idiyom çünkü bir soruna çare oluyor.
C++ta nesneler oluşturuyoruz ve bu nesnelere ilk değer veriyoruz.Nesne const olduğunda zaten initialize etmeme durumuda yok. İlk değer verme mecburi.

Const nesnelere ilk değer vermek istediğimde bazı durumlarda vereceğim ilk değer basit bir ifade, bazı durumlarda da bir hesaplama sonucu elde edilebilecek değer.
Bu hesaplama işleminde çoğu zaman başka değpişkenleride kullanıyoruz. Bunlar basit hesaplama olsaydı ternary operator bizi kurtarıyordu.
const int x = a > 2 ? z : y + 5; gibi

Bazı durumlar varki hesaplama daha karışık. Burada birden fazla ihtimal var.
1 - Standartların dışına çıkmak. Mesela GCC de statement expression extension var. Bunu kullanmak.Ama hoca önermiyor.
2 - HEsaplamayı bir functionun gerçekleştirmesi.

IIFE IDIOM NASIL ÇALIŞIYOR
Madem lambda func var. aşağıdaki gibi yazılıyor.

void func(int a, int b)
{
	int y = 20;
	int x = 30;

	const int x = [=](){
		//..
		//..
		return val;	
	}();

}

Bu konuda tabi farklı görüşler var. Kodun okunması zorlaşıyor diyenler var. Diğer taraftanda ayrı bir func olmasınında dezavantajları var.
Çok okunabilir deniyorsa eğer yada birden fazka kez kullanacaksak halen isimlendirme seçeneğimizde var.Yani lambda ifadesine isim verip herseferinde bunu çağırabiliriz.
Tabi idiyomatik özelliği kalmıyor ama çağırabiliriz.

Diğer seçenekte invoke çağırmak olabilir.

Burada const olmayan bir nesneyede tabiki ilk değer verebiliriz. Ama kullanılan yer genelde const bir nesneye ilk değer verme ifadesi içerisi.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BIR SINIFIN VERI ELEMANLARI IÇINDE KULLANILIYOR

Constructorda sınıfın veri elemanları init ediliyor.İdeali sınıfın veri elemanları member / Ctor init list ile init etmek.
Aynı problem burada da var. Bu bir const veri elemanı olabilir veya bunu ilk değer verecek bir hesaplama sözkonusu olabilir.
Burada da IIFE kullanabiliriz. Gerisi artık bunu ne derece benimsediğimize bağlı. Cpp core guideliness iife idiyomunun özellikle kullanılmasını tavsiye ediyor.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

C++20 İLE GELENLER

std::pair<int,int>foo();

int main()
{
	auto[x,y] = foo();

	auto f = [x](int a){ return a * x;}; // C++17 de sentaks hatası mesela burası.C++20 ile düzeltildi.Farklı Compilerlarda test et.
}

Peki burada x ve y nin tür çıkarımı nasıl yapılıyor?
x pair.first ve y.pairin second ından ilk değerini alıyor demek yanlış.

Tür çıkarımı func ın return değeri olan nesne için yapılıyor. Aslında burada tanımladığımız değişken int türden değişkenler değil.
Aslında burada std::pair oluşturmuş oluyoruz. x ve y std::pairin elemanların eş isim olarak kullanılıyor.

Tür çıkarımı elemanlara göre değil func ın return değeri türüne göre yapılıyor.

PSUDO KARŞILIĞI
auto p = foo();
x yazınca p.first
y yazınca y.second kullanmış oluyoruz.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR:
struct Nec{
	int x;
	int a[10];
};

Nec foo();

int main()
{
	auto [y,z] = foo(); // Z POINTER MI 10 ELEMANLI INT DIZI MI ?
						// DİZİ. Biz burada funcın return değeri olan türden bir nesne oluşturduk, y ve z o nesnenin elemanlarına eş isim olarak kullanılıyor.

	auto constexpr b = is_pointer_v<decltype(z)>; //FALSE

}



MOLA

===================================================================================================================================================================================
===================================================================================================================================================================================
===================================================================================================================================================================================
===================================================================================================================================================================================
===================================================================================================================================================================================

Bazı algoritmalar veya bazı memberfunclar pair döndürüyor.Set in insert func ı iterator bool pairi.
Partition copy nin return değeri yine iterator.

int main()
{
	vector<int> ivec;
	rfill(ivec,100,Irand{0,10'000});

	auto ip = minmax_element(ivec.begin(), ivec.end()); // Bir range teki hem min hemde max değeri buluyor. 
							  // Return değeri iterator.karşılaştırma less functorı kullanılıyor defaultta.Return değeri pair
	
	Burada ip nin türü, pair<vector<int>::iterator, vector<int>::iterator>. Bunu yazmak yerien auto kullanıyoruz daha pratik olduğundan

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	p.first, min öğeye iterator
	p.second, max öğeye iterator

	std::cout << *p.first << "\n";
	std::cout << *p.second << "\n";
							
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	Burasıda kullanılabilir

	auto[iter_min, iter_max] = minmax_element(ivec.begin(), ivec.end());

	Structured Binding i get interace ine sahip tüm varlıklar için kullanabiliriz. Kendi sınıflarımız için kullanacaksak kendi sınıfımız için get yazmalıyız.
	get ten kasıt, tuple daki get interface.

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	BUNU IF WITH INIT İLE BİRLEŞTİRMEK ÇOK SIK KULLANILAN KOD

	if(auto[iter_min, iter_max] = minmax_element(ivec.begin(), ivec.end()); iter_min> iter_max)
	{
		
	}

}

===================================================================================================================================================================================
===================================================================================================================================================================================
===================================================================================================================================================================================

C++20 YENİLİKLER

1- STATELESS LAMBDALAR IÇIN DEFAULT CTOR VE COPY ASSIGNMENT DELETE EDILMIŞ DEĞIL ARTIK

int main()
{
	auto f = [](int x){ return x * x + 1;};

	decltype(f) g;
	decltype(f) h; // Artık bu türden değişkenler default ctor ile hayata getirilebilir.

	g = h; // copy assign da geçerli. Bu ikisi C++20 öncesinde sentaks hastasıydı

}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Bunun faydası, set map unordered set unordered map ... bir çok şablonda template parametresi bir tür ve bizden bir functor istiyor.

Mesela set<int,less, allocator...>

NOT: set<int,less<>>myset;  BÖYLE KULLANILABILIR. BUNA TRANSPARENT LAMBDA OBJECT DENIYOR.
sort(ivec.begin(), ivec.end(),greater<int>{}); // BU LEGAL ZATEN
sort(ivec.begin(), ivec.end(),greater<>{}); // BU DA LEGAL ARTIK.
sort(ivec.begin(), ivec.end(),greater{}); // BU DA LEGAL.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Kendi karşılaştırma kriterimi oluşturmak için bir predicate functor kullanabiliriz.  Bir lambda ifadeside kullanabiliriz.

int main()
{
	auto f = [](int x, int y){ return x % 100 < y % 100;}; //DİKKAT!!! STRICT WEAK ORDERING KURALLARINA GÖRE YAPILIYOR.YOKSA UNDEFINED BEHAVIOR
								//ASSOCIATIVE CONTAINERLARDA STRICT WEAK ORDERING YANI EQUIVALANCE
								//UNORDERED ASSOCIATIVE CONTAINERLARDA EQUALITY
	
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
	KISACA STRICT WEAK ORDERING. YAZMIŞTIM TEMEL C++ TA AMA TEKRAR YAZAYIM
	1. a op a --> false olmak zorunda.true vermeyecek.
	2. a op b --> true ise      b op a --> false vermek zorunda.yoksa equivalance kriteri olarak kullanılamaz.
	3. a op b --> true ise	  ve	b op c --> true     ise     a op c --> true olmak zorunda.
	4. !(a op b) bu yanlış ise yani     &&      !(b op a) buda yanlış ise -->  bu durumda a ve b eşit kabul edilecek.

	!(a op b) && !(b op a) ve !(b op c) && !(c op b) ise ----> !(a op c) && !(c op a) olmak zorunda.
	Yani a ve b eşit ise b ve c eşit ise a ve c eşit olmak zorunda.
	Bunların hepsinin özel adları falanda var.
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	C++17

	set<int,decltype(f)> myset; // C++17 DE SENTAKS HATASI.Setin kodlarına bakarsak bu karşılaştırmayı yapmak için bir nesneye ihtiyaç var ve o nesne default init ediliyor
					// dolayısı ile decltype(f) in default constructible bir tür olmalı.Ama default ctoru yok. Bu sebeple SEntaks hatası oldu 
						
	myset.insert(12);


	C++17 ÇÖZÜMÜ
	set<int,decltype(f)> myset(f);

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	C++20 DE GEÇERLİ
	set<int,decltype(f)> myset; // GEÇERLİ. DEFAULT CTOR ÇÜNKÜ VAR ARTIK.DELETE EDİLMEMİŞ.

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	ASIL DAHA ETKİLİ KULLANIMI C++20 İLE EKLENEN LAMBDA UNEVAUATED CONTEX
	C++17 DE LAMBDALARIN İŞLEM YAPILMAYAN CONTEXT TE KULLANILMASI GEÇERLİ DEĞİL.

	decltype([](int x, int y){ return x % 100 < y % 100;}); //C++17 SENTAKS HATASI. C++20 İLE GEÇERLİ

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	using cmp_type = decltype([](int x, int y){ return x % 100 < y % 100;}); // GEÇERLİ OLSAYDI BU YAZILABİLİRDİ.C++20 ILE GEÇERLİ

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	SETTE TÜR BİLGİSİ OLARAK KULLANABİLİRİZ

	set<int, decltype([](int x, int y){ return x % 100 < y % 100;})> myset;

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	DAHA ANLAŞILIR YAZALIM

	using cmp_type = decltype([](int x, int y){ return x % 100 < y % 100;}); 

	set<int,cmp_type> myset; // C++20 ile geçerli

}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UNİQUE PTR YE BAKALIM

template<typename T>
struct Default_Delete
{
	void operator()(T *p)
	{
		delete p;
	}
};


template <typename T, typename D= std::Default_Delete<T>>
class Unique_Ptr{
public:
	
	~Unique()
	{
		D{}(mp);
	}
private:
	T* mp;
};

int main()
{
	unique_ptr<int> bu demek 
	unique_ptr<int,Default_Delete<int>> aynı şey.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DELETER OLARAK CUSTOM BIR TYPE KULLANMAK ISTIYORUZ DIYELIM. O ZAMAN DELETER OLARAK BIR TÜR VERMEM GEREKIYOR. BUNU LAMBDA FUNCTOR VEYA FUNCTION OLARAK VEREBILIRIZ.

int main()
{
	unique_ptr<int, decltype([](int *p){ })>; // deleterı decltype olarak verdik doğrudan. Çok pratik bence
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

KODLARDA SIKSIK AŞAĞIDAKİ DURUM GÖRÜLÜYOR ARTIK

class Myclass{
public:

private:
	decltype([](){}) mx; // derleyicinin oluşturduğu closure type türünden eleman
};




*/
