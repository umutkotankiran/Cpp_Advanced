/*

SPAN LIBRARY
------------
Bu konudan sonra ranges library görülecek. Ranges ten sonraki haline stl 2.0 diyorlar. Neden spani görüyoruz çünkü ranges'tede görülüyor.

Bu stringview ı bilenler için yeni bir fikir değil.

Burada bellekte ardışık contigues olan öğeler üzerinde işlemler yapmak için kopyalama olmaksızın, 
aslında bunu bir pointer ve tipik olarak tamsayı değişkeni bir tür olarak paketlemek yada sarmalamak ve 
buna bir interface sağlamak amaçlı geliştirilmiş.

Böylece containerın kendisini kopyalamak yerine, aslında bir pointer ve tamsayıyı
içeren bir sınıf nesnesini kopyaladığımızda copy maaliyeti düşük olacak.

Bu fikrin avantajlarıda var ama tehlikeli durumlarda oluşturabilir. Arka planda nesneler söz konusu fakat 
bunlar ona erişen sınıf nesnesine ait değil.Non owning reference diyorlar. Riski ise, nesnemizin hayatı devam ediyor, 
içindeki pointerın göstriği nesne öldüğü zaman bizim nesnemizin içindeki ptr ise dangling hale gelecek
Bunu compile timeda sınama şansımız yok. Runtimeda oluyor bunlar.

String view read only erişim sağlıyor. Span ise write amaçlıda erişebilir. Bunu kendimiz belirliyoruz. Okuma veya yazma amaçlı erişim olarak.

Stringview ın faydası, func parametresi string olsaydı, oraya gönderilen argümanın stringe dönüşebilecek türden olması gerekiyor.
Stringview view diyori, ben bu ardışık karakterlerin nerede tutulduğu ile ilgili değilim. Bu bir std::Array<char,20> içinde de olabilir
vector<char> da olabilir, stringview bununla ilgilenmiyor.

Bu tür sınıflar tasarlanırken herkesi memnun etmek zor. Span veya string view dan şikayetçi olan çok. Tasarımla ilgili seçimler yapmak gerekiyor,
her karar noktasında karşımıza trade off lar çıkıyor.

Burada performans mı odak noktası olacak yoksa güvenlik mi?
Const correctness ne olacak?

Const correctness C++ ta baştan yanlış tasarlanmış, ileride görülecek. Sentaks öğeleri çok iyi oluşturulmamış. 
Yeni araçlar eklendğinde ilk problem const correctness ile uyumsuz olduğu görülüyor.Const correctness konusunda dikkatli olunmalı.

Span madem ardışık öğelere read yada write etmek için kullanacak,
Array vector carray den span e dönüşümler olsun mu? bunlar karar noktaları
...

En önemli noktalardan biri şu, sınıf nesnesinin kendisinin const olması başka
ama sınıf nesnesi vasıtasıyla eriştiğimiz nesnelerin compile time da const olarak alınması başka.

--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------

#include <span>

template <typename T, std::size_t Extent = dynamic_extent>

inline constexpr std::size_t dynamic_extent = std::numeric_limits<std::size_t>::max();

burada max yerine -1 desekte olur.

inline constexpr std::size_t dynamic_extent = -1;

--------------------------------------------------------------------------------------------------------------------------------------------------------

BURASI ÇOK ÖNEMLİ !!!!!!!!!!!!

Arka plandaki template teknikleriyle eğer 2. template parameteresine template argüman vermezsek 
bu durumda n in değeri türün bir parçası zaten en büyük değer olacak.

Dynamic extent olduğunda ptr bir adres tutarken val de ne kadarlık alanın gözlemlendiği değerini tutacak 
ve val dinamik olarak değişecek.Eğer 2. template argümanı default olarak alındıysa, dynamic extent olarak alındıysa 
tipik derleyici implementasyonunda sınıfın 2 tane veri elemanı olacak.

template<typename T, std::size n = std::numetic_limits<std::size_t>::max()>
class Span{
	T *ptr;
	size_t val;
};


DİKKAT !!!!!!!!!!!!!
Fakat o default argüman kullanılmamışsa

int main()
{
	int ar[5]{};
	span<int,5> sp(ar); // BU ŞEKILDE NESNE OLUŞTURUSAK MUHTELEMEN IMPLEMENTASYON TARAFINDA
			    // SIZEOF(SP) 4 BYTE OLACAK. BIR POINTER TUTULUYOR. SPAN ARALIĞI DEĞIŞMIYOR.
			    // TÜRÜN BIR PARÇASI OLDU. SP DIYORKI BEN HER ZAMAN INT NESNELER DIZISINDE 
			    // 1 ADRESTEN BAŞLAYARAK 5 TANE NESNELIK BIR ARALIĞI IŞARETLEYECEĞIM
			    // BU DINAMIK OLARAK RUNTIMEDA 4 VEYA 7 OLAMAZ HEP 5 OLARAK KALACAK
			    // UYGUN OLMAYAN ATAMA YAPILDIĞINDA SENTAKS HATASIDA OLMAZSA UNDEFINED BEHAVIOR OLACAK
			    // BÖYLE SPANLERE STATIC/FIXED EXTEND DENIYOR.


	std::cout << sizeof(sp); // 4


	span<int> sp(ar); // BU ŞEKILDE YAZARSAK BU DURUMDA BU DYNAMIC EXTENTLY BIR SPAN OLDU.
			  // BU IÇINDE BIR POINTER TUTMAKLA DEĞIL BIR TAMSAYI DEĞIŞKENDE TUTUYOR VE BU DEĞIŞEBILIR.
			  // 3 NESNELIK ALANI GÖZLEME STATE INDEN 7 NESNELIK ALANI GÖZLEMLEME STATE INE GEÇEBILIR.
			  // BIR SPANIN DYNAMIC MI STATIC EXTENTLY MI OLDUĞUNU NASIL ANLARIZ? BUNUN IÇIN 2. TEMPLATE
			  // ARGÜMANI ILE DEĞERINI ALMIŞ BIR STATIC CONSTEXPR VERI ELEMANI VAR.

}

--------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR:

int main()
{
	int ar[5]{};
	span<int, 5> sp_f(ar);   // 1 adresten başlayıp 5 nesne tutacak demek bu.
				 // Uygun olmayan atama ve ilk değer vermede sentaks hatası yoksa UB olacak
				 // Böyle spanlere static/fixed span deniyor


	span<int>sp_d(ar); // Bu dynamic span.
	
	std::cout << sizeof(sp); // 4 byte. 1 tane ptr var demek. 
	
	std::cout << "sp_f.extent = " << sp_f.extent << '\n'; // 5
	std::cout << "sp_d.extent = " << sp_d.extent << '\n';  // 4294967295

}

İkisininde avatajı ve zavatantajı var
Biri dinamik olarak runtime da değişebiliyor.
Diğeri ise compile timeda bazı yanlışlıklar doğrudan derleyici tarafından saptanabilir.

--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------

CONSTRUCTORS

Default construct edilebilmesi için dynamic extent olmalı. Static extent ise default constructible değil
1 istisnası size_t değerinin 2. template argümanına 0 değerinin geçilmesi

int main()
{
	span<int> spx; //Geçerli

	span<int,10> spy; // Geçersiz. Static extent default ctor edilemez

	span<int,0> spy; // static ama default c. edilmiş.

	std::cout << "spy.extent = " << spy.extent << '\n';	// 0 çıktı.

}

--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------

PARAMETRELI CTORDA DYNAMIC EXTENT ISE HERHANGIBIR ŞEKILDE KOŞULLARI SAĞLAYAN NESNELER DIZI ILE ILK DEĞER VEREBILIRIZ.


int main()
{
	vector<int> ivec{2,5,7,9,1,8,3,4};

	span<int,8> spx{ivec};  // GEÇERLİ. ELEMAN SAYILARI TUTUYOR
	span<int,5> spx{ivec};  // UB
	span<int,20> spx{ivec}; // UB 
	span<int> spx{ivec}; // GEÇERLİ.DYNAMIC EXTENT

	Static extent kullanıyorsak compile time sentaks hatası veya runtime da UB durumuna düşmemesi,
	ne ile ilk değer verildiğine bağlı.

	--------------------------------------------------------------------------------

	vector<int> ivec{2,5,7,9,1,8,3,4};
	array<int,5> ar;

	span<int,5> spx{ar}; // Tamsayı değeri tutarsa problem yok.
	span<int,10> spx{ar}; // Tamsayı değeri tutmazsa compile timeda anlaşılırsa compile time hatası.
			      // burada anlaşılıyor.Compile time da saptıyor ve sentaks hatası verecek

}

--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------

İKİNCİ BİR OLANAK BİR KONUM BİLGİSİ VE BİR TAMSAYI DEĞERİ VERMEK.

int main()
{
	vector<int> ivec{2,5,7,9,1,8,3,4};
	array<int,5> ar{11,22,33,44,55};
	int arr[5] = { 32, 45, 12, 74, 71 };

	span<int,5> spx{ar.data()}; // Böyle bir ctor yok. bir konum bilgisi varsa mutlaka bir tamsayı değişkenide olacak
	span<int, 5> spx{ arr }; // GEÇERLİ. RAW ARRAY OLUNCA GEÇERLİ.
	span<int,5> spx{ar.data(), 5}; // GEÇERLİ
	
	------------------------------------------------

	int a[20];
	std::array ar{1,2,3,4,5};

	span<int> s1 = a; // Geçerli 
	span<int> s2 = ar; //Geçerli

	------------------------------------------------

	span<int,20> s1 = a; // Geçerli
	span<int,10> s3 = a; // Compile time hatası.Compile time da detect edilemeyen yerde runtime hatası olacak
	span<int> s2 = ar; // Geçerli

	------------------------------------------------

	int a[20];
	std::array ar{1,2,3,4,5};

	span<int,5> sp{ar.data(), 5}; // GEÇERLi

}

--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------

SPANİN BİR İNTERFACEİ VAR

int main()
{
	
	span<int> sx;
	sx.  yazıp tüm interface'e bakılır

	--------------------------------------------------------------------------

	vector<int> ivec{2,3,5,8,12};
	span<int> sx{ivec};

	sx.front() = 9; // son öğeyi değiştirdik.İlk öğeye referans
	
	sx[3] = -1; // 3 indisli elemana eriştirir ve değiştirdik
	
	// DİKKAT !!!!!!!!!!!!!!!!!!!!!
	// BURADA EĞER VECTORÜ DEĞIŞTIRIRSEK YANI ELEMEAN EKLERSEK VE VECTOR REALLOCATION YAPARSAK SPAN IÇINDEKI POINTER
	// DANGLING HALE GELEBILIR !!!!!!!!!!!!!!

	for(auto val : ivec)
	{
		std::cout << val << ' ';
	}
}


--------------------------------------------------------------------------------------------------------------------------------------------------------

#include <span>
#include <vector>
#include <iostream>
#include <array>

int main()
{
	using namespace std;

	vector<int> ivec{ 2,3,5,8,12 };
	array ar{ 2,3,5,8,12 };

	span<int> vec_span{ ivec };
	span<int> ar_span{ ar };  // Bu iki spanin türü aynı

	static_assert(std::same_as<decltype(vec_span), decltype(ar_span)>); // True döner


	std::cout << "vec_span.extent = " << vec_span.extent << '\n'; // 4 milyar küsür çünkü dynamic extent
	std::cout << "ar_span.extent = " << ar_span.extent << '\n'; // 4 milyar küsür çünkü dynamic extent

	std::cout << "vec_span.size() = " << vec_span.size() << '\n'; // 5. size öğe sayısını veriyor.
	std::cout << "ar.size() = " << ar.size() << '\n';	// 5

}

--------------------------------------------------------------------------------------------------------------------------------------------------------

CTAD BURADA ÇALIŞIYOR

int main()
{
	using namespace std;

	vector<int> ivec{ 2,3,5,8,12 };
	array ar{ 2,3,5,8,12 };

	span x{ ivec }; // DİKKAT!!! span<int> değil CTAD VAR.Dynamic extent.Arrayda aşağıda ise fixed olacak
	std::cout << x.extent << '\n';

	span y{ ar }; // türü span<int,5U> açılımı. Ctad kurallar buna göre düzenlenmiş.Fixed.Vectorde Dynamicti


	int ar2[]{ 2,3,5 };
	span z{ ar2 }; // x in türü span<int,3U>. Yİne Ctad geçerli.Size bilgisi compile timeda elde edildi.

	std::cout << z.extent << '\n'; //

}

--------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	
	vector<int> ivec(100);

	span x{ivec.data(), 4}; // span<int>. Dynamic extent

	--------------------------------------------------------------------------

	array<int,5> ar;

	span x{ar.data(),4}; // Burası dynamic extent. Size bilgisi compile timeda elde edilemezse dynamic extent

}

--------------------------------------------------------------------------------------------------------------------------------------------------------

CONSTLUK DURUMU

Spanin kendisi const olması durumunda spanin kendisi değiştirilemiyor.

int main()
{
	std::array a{1,3,5,7,9,11,13};
	std::array b{1,3,5,7,9,11,13};

	const span<int> x = a; // türü span<int,7U>

	x[5] = 12; // Geçerli. Constluk nesnenin kendisinindi.

	x = b; // Sentaks hatası constluk var çünkü

	--------------------------------------------------------

	span<const int> x = a;

	x[5] = 12; // şimdi sentaks hatası oldu burası.
}

--------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename C>
void func(const C& x) // buradaki const spanin constluğu, spanin gözlemcisi olduğu dizinin değiştirilmesini engellemiyor
{
	x[3] = 5;
}

int main()
{
	int ar[] = {1,4,56};
	span x = ar;

	func(x); // Geçerli
}

--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------

SPANIN GÖSTERDIĞI ARALIĞI YAZAN FUNCTION TEMPLATE

NOT : Span'i referans ile geçmenin bir anlamı yok !!!

template<typename T, std::size_t size>
void span_print(std::span<T,size>sp)
{
	
	if constexpr(size == std::dynamic_extent)
		std::cout << "(dynamic extent span)\n";
	else
		std::cout << "(fixed extent span)\n";
		
	
	for(const auto &elem : sp)
		std::cout << elem << ' ';
	
	std::cout << '\n';

	//AŞAĞIDAKI GIBIDE YAZILABILIR
	//for(size_t i{}; i < sp.size(); ++i)
	//{
	//	std::cout << sp[i] << ' ';
	//}
}


int main()
{
	vector<int> ivec;
	rfill(ivec, 20, Irand{0,100});
	print(ivec); // Irand ile birlikte bu hocanın yazdığı library içinde

	span<int> sx{ivec};

	std::cout << "sz.size() = " << sx.size() << '\n';

	span_print(sx);

}

--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------


NELER YOK ? 
-----------
Swap yok.
assign yok
at yok
begin ve end var ama cbegin cend crbegin crend yok
tuple dan gelen get interface yok

front var
back var
first var ve return değeri span. .first(3) ilk 3 değeri tutacak
last var

ENTERESAN BİR FUNC VAR 

bu size_bytes funcı.

int main()
{
	vector<double> dvec{1.2, 4.5, 7, 8, 9.3};

	span sx = dvec;

	std::cout << "size = " <<  sx.size() << '\n'; // 5
	
	std::cout << "size_bytes = " <<  sx.size_bytes() << '\n';  // 40

	sx.front() = -1;
	
	sx.back() = -1.;

	----------------------------------------------------------------------------------

	auto val1 = sx.first(3); // first in return değeri span.Dynamic span.
	// span<int, 3> sp3{ spx.first(3) };  //Böylede yapapbiliriz.

	for(auto d : val1)
	{
		std::cout << d << '\n'; // ilk 3 öğe olacak burada.
	}
	
	----------------------------------------------------------------------------------

	auto val2 = sx.last(3); // son 3 öğeye span buda

	for(auto d : val2)
	{
		std::cout << d << '\n'; // Son 3 öğe.
	}
}

--------------------------------------------------------------------------------------------------------------------------------------------------------

SORU:
HANGİSİ GEÇERLİ?

int main()
{
	vector<int> ivec {10,20,30,40};
	array ar{1,2,3,4};
	int ca[]{1,2,3,4};

	span_print(ivec); // Sentaks hatası. Burada çıkarım yapamıyor derleyici. span{ivec} burada problem olmazdı
	span_print(ar);   // Sentaks hatası. Yine tür çıkarımı yapamadı
	span_print(ca);   // Sentaks hatası. 
}

--------------------------------------------------------------------------------------------------------------------------------------------------------

DANGLING REFERANS 

vector<int> get_vec()
{
	return {1,2,3,4,55,6,7};
}

int main()
{
	span<int> x{ get_vec() };  // Sentaks hatası.

	//Ömrü büten nesne ile ilk değer verilirse dangling referans oluşur
	span<int> x {get_vec().data(), 5}; // Sentaks hatası değil ama dangling referans hatası olur

	for(auto i : x)
		std::cout << i << ' '; // burada görülebilir Ub durumu

}

--------------------------------------------------------------------------------------------------------------------------------------------------------

2. DANGLING REFERANS ÖR

int main()
{
	std::vector<int> ivec{1,2,3,4,45,6};
	span sp{ivec};

	for(int i = 0; i < 10; ++i)
	{
		for(auto x : sp)
		{
			std::cout << x << ' ';
		}

		std::cout << n(3);
		ivec.push_back(9);
		(void)getchar();
	}

}

HER REALLOCATIONDA DANGLING REFERENCE OLACAK


=====================================================================================================================================================================
=====================================================================================================================================================================
=====================================================================================================================================================================
=====================================================================================================================================================================
=====================================================================================================================================================================
=====================================================================================================================================================================
=====================================================================================================================================================================
=====================================================================================================================================================================
=====================================================================================================================================================================
=====================================================================================================================================================================


RANGES
------
STL de bir devrim niteliğinde.Çok önemli.Not evolution it's a revolution.
Artık STL tamamen değişiyor ranges ile. Eski STL i de kullanmaya devam edebiliriz ama tabiki yenisi avantaj oluşturuyor.
Header file ranges.

Burada bizi ilgilendiren birinci dereceden algoritmalar.
STL in algorithm header fileda olan bizim eskiden beri kullanmaya alıştığımız algotritmalar şimdi farklı bir şekilde
karşımıza çıkacak. 

Burada eski algoritmaları bilmemiz kötü birşey değil.

Bütün algoritmaların ranges karşılığı var mı ? HAYIR %100 değil.
Nedeni numeric header fileda algoritmaların(accumulate gibi) rangeste karşılıkları yok.C++20 ye yetişmediler.
Neden numaric için böyle bir kısıtlama var ? Bunlar constraint algorithms. Varlık nedenini destekleyen temel öğe
conceptler ile template parametreleri ile ilgili kısıtlamalar getirilmiş olması. Oysa algoritm deki algoritmaları halen
kullanabiliriz ama onlar için conceptler etkin değil.

Normal algoritmalar için constraintleri oluşturmak zor değil.Numeric algoritmalar içinse bu zor.Kolay değil ve bu yüzden
gecikmiş.Bu C++23 e kalmış.

1 - Constrained Algoritm, herbirinin 1 veya daha fazla constained'i var.Bu interface in bir parçası.

2 - Constrained, interface'in bir parçası.

	ranges::sort a baktık ve parametrik yapısı tamamen farklı.

3 - Klasik algoritmalar bizden iterator çifti istiyor.Bazende 1 iterator 1 tamsayı istiyor.
	ranges te biraz daha farklı.Doğrudan container nesnesi argüman olarak gönderilebiliyor

	std::sort(ivec.begin(),ivec.end());
	ranges::sort(ivec);

	Aşağıdakilerde yapılabilir.
	ranges::algo(con);
	ranges::algo(iter,iter);
	ranges::algo(iter, tamsayi);

	Sentinel kavramının aktif olarak kullanılması ve genişletilmesi, dolayısıyla bunu sağlamak için iterator
	parametreli algoritmalarda artık iteratörlerin türlerinin aynı olması gerekmiyor.

	Biz containerda find algoritması ile değer aradığımızı düşünelim.Diyelimki biz bu range de aranan değerin
	olduğundan %100 eminiz ama ilkini bulmak istiyoruz.Bu durumda range in sonuna geldim mi diye kontrole gerek yok.

	Sentinel kavramının genişletilmesi aralığı belirleyen konumların farklı türlerden olabilmesi.

	#include<vector>
	#include<list>
	#include<ranges>
	#include<algorithm>

	int main()
	{
		std::list<int> ivec;

		for(int i = 0; i < 100000; ++i)
		{
			list.push_back(rand()); 

		}
		
		std::sort(con.begin(), con.end()); // list olsaydı hata verirdi.ve gcc de 400 küsür satır hata mesajı
		
		std::ranges::sort(con.begin(), con.end()); //  Burada ise sadece satisfied olmadı diye bir hataveriyor. Daha anlaşılır
	}


4 - Algoritlamarın projection parametreleri algoritmaların daha esnek kullanımını sağlıyor.
	Sıralamada doğrudan tek func call ile vectorde tutulan mesela fighter sınıfı türden nesnelerin kendilerinin
	karşılaştırılması değilde bizim belirlediğimiz bir öğesi karşılaştırması ile sıralayabiliriz.

5 - Dangling iterator belirli durumlarda doğrudan derleme zamanında tespit edilebiliyor.

6 - Performans açısından hiçbir eksiği yok.Hatta sentinel kullanımı ile daha iyi çıkıyor.

7 - ÇOK ÖNEMLİ !!!!!!!  Algoritmalar functional programlama teknikleriyle, declerative programlama yaklaşımıyla
	configure edilebiliyor. Composable algoritmalar oluşturabiliyoruz.


NEGATİF YÖNLERİ
---------------
1 - Bütün algoritmaların range karşılıkları yok.

---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------


namespace std
{
	namespace ranges
	{
		// Yeni Algoritmalar
		// View(function object sınıfları)

		namespace views
		{
			// Adapters

		}
	
	}
	
	//Namespace alias vermişler
	namespace views = ranges::views;

}

namespace rng = std::ranges; // Artık bu şekilde de yazıyorlar.


DİKKAT!!! 
Aşağıdaki yapılırsa algoritmalardaki klasik algoritmalar ile çalışma olur aşağıdaki yazılırsa

using namespace std::ranges; // yapılmamalı


int main()
{
	std::ranges::views::take

	std::views::take // yukarıdaki yerine artık aliastan sonra bu kullanılabilir.	
}

---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------

DİKKAT !!!!!!!!!!

RANGES ALGORITMALARI ALGORITHM HEADER FILE IÇINDE AMA NAMESPACE OLARAK RANGES NAMESPACE IÇINDE !!!!!!!!!!!!!!!!!!

namespace std::ranges{
	//..
}

Ranges header file içerisinde aşağıdakiler var
Conceptler
view sınıfları
range adaptörleri

---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------

Range aslında constrained bir template parametresi

Bizim funca göndereceğimiz birinci argüman input_range conceptini satisfied etmek zorunda.Aksi halde sentaks hatası.
template parametreleri rangelerin tipik olarak constraint.Dolayısıyla bu funca bir range göndermek demek bu concepti
satisfied eden herhangbir varlık gönderebilirsiniz demek. Range olmasını belirleyen tipik 2 özellik var. 2 tane konum verme
olanağına sahip olacak. biri range begin diğeride end olarak görev yapan sentinel. 

Mesela input range e bakıyoruz.input range diyorki ilk önce range conceptini sağlayacak.Range concepti diyorki
ranges:begin(t) ve ranges::end(t) geçerli olacak. mesela vector vec; bunun bir range olup olmadığını anlamak için
ranges::begin(vec) ve ranges::end(vec) bunların geçerli olması gerekiyor.End burada sentinel verecek tabi.
Bir varlığın range olup olmadığını böyle anlarız. input range, range conceptini kapsıyor. Aynı zamanda input_iterator
conceptinin satisfied edilmesi gerekiyor.İnput iterator conceptine, ranges::iterator_t türüne T tür argümanı olabilecek.
Yani aşağıdaki olay karışık gelebilir başta.

template< class T >
  concept input_range = ranges::range<T> && std::input_iterator<ranges::iterator_t<T>>;


template <std:ranges::input_range R>
void Algo(R x)
{
		
}

---------------------------------------------------------------------------------------------------------------------

NELER RANGE ? 
Tüm containerlar range(vector, deque, carray,array...) ama container adapters range değil.

static_assert(std::ranges::range<std::stack<int>&>); //hata.

Range algoritmaları bir varlığın range olup olmadığını constraintlerden anlıyorlar.Range parametre demek
range conceptlerinden biriyle constraint edilmiş demek.

---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------

BURADAKİ ÖĞELERİ YAZALIM

Range : Rnage conceptlerini sağlayan herhangibir varlık ve dolaşılabilen bir aralığa sahip.Bunuda belirleyen global begin
		ve end functionlarına range nesnesini argüman gönderdiğimizde bize iterator ve sentinel vermeli.

Views : Bunlarda range ama bunlar hafif siklet range. yani copy ler constant timeda, move garantileri var. Kopyalama maliyetide 
		çok düşük.Range constraintlerini sağlayan özel davranışları belirleyen rangelere view deniyor.

Range Adapter :  Ya range alarak views verir yada view alıp view verir.

---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------

SENTİNEL KAVRAMI

2 tane ayrı template parametresi var diyelim, bunlardan bir tanesi iterator türünden diğeri ise sentinel türünden.
İkiside iterator de olabilirdi. 

std::string read_from_file(const char *pfilename);

int main()
{
	const auto str = read_from_file("bigfile.txt");

	auto iter = std::find(str.begin(), str.end(),'\n');
				// find kodunda her turda beg != end gibi bir kontrol vardı.Bu kontrolü yapmama şansımız olabilir.
				

	//RANGETEKİ SORT NASIL !!!!!!!!!!!!!!!!!!!!!!
	auto iter = std::ranges::find(str.begin(), std::unreachable_sentinel,'\n');
	// unreachable_sentinel, unreachable_sentinel_t türünden bir constexpr bir nesne.

}

RANGES'İN FIND FUNCI

Burada derleyici template argumenet deductıon ile s türünün unreachable_sentinel_t türü olduğunu anladı ve
2. parametresi unreachable_sentinel_t türünden oldu ve derleyici unreachable_sentinel sınıfınıda görüyor,
koduda inline olduğu için bu karşılaştırmanın her zaman true olduğunu görüyor.

template<typename Iter, typename S, typename T>
Iter Find(Iter beg, S end, const T& val)
{
	while(beg != end)
	{
		if(*beg == val)
			return beg;

		++beg;
	}

	return beg;
}

KARŞILAŞTIRMANIN TRUE OLDUĞUNU NASIL GÖRÜYOR ? 

template<typename T>
struct ursen
{	
	bool operator==(const auto&val)const
	{
		return false;
	}
};

Derleyici bu kodu görüyor ve karşılaştırma unreachable_sentinel_t türü ile iterator türü arasında.
Buna derleyici ursen sınıfı func calluna bakıyor, bunun hiçbir şekilde false olmalı mümkün değil.
çünkü while(beg!=end) diyor. == her zaman true dönerse, != de hep false döner :D:D:D

Aşağıdakine dönüşür
template<typename Iter, typename S, typename T>
Iter Find(Iter beg, S end, const T& val)
{
	while(true)  // DİKKAT !!!!!!!!!!!!!
	{
		if(*beg == val)
			return beg;

		++beg;
	}

	return beg;
}

---------------------------------------------------------------------------------------------------------------------

ÖR:

struct nullchar_t
{
	bool operator==(auto pos)const // operator== i yazarsak yeni kurallara göre != i derleyici yazacak.
	{
		return *pos == '\0';
	}
};


//Bunu sonradan ekledik
inline constexpr nullchar_t nullchar;

int main()
{
	const char *p = "biz bugun range kutuphanesini ogreniyoruz";

	// bu çalışır zaten
	for(auto ptr{ p }; ptr != nullchar_t{}; ++ptr)
		std::cout << *ptr << ' '; 

	-------------------------------------------------------------------------------

	// Burada p, nullchar_t ye eşit mi ditye kontrol ediyor foreach.karşılaştırmayı da nullchar_t içindeki
	// operator== yapacak. C++20 ile a.b --> b.a ya dönüşebiliyordu.

	std::ranges::for_each(p,nullchar_t{}, [](char c){ std::cout.put(c); });

	std::ranges::for_each(p,nullchar, [](char c){ std::cout.put(c); }); // buda inline variable ile geçerli.

}

---------------------------------------------------------------------------------------------------------------------

BURADA SENTİNEL İLLAHA \0 OLACAK DİYE BİRŞEY YOK. -1 GİBİ BİR DEĞERDE OLABİLİRDİ.



*/
