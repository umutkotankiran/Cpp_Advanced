/*


LAMBDA İFADELERİNİN SON BÖLÜMÜNDEYİZ.

int main()
{
	using gset = set<string, decltype([](const string &s1, const string &s1){ return s1 > s2;})>; // STATELESS OLMALI. transparent func object greater<> da kullanabilirdik
	gset set1;
	rfill(set1,10,rname);
	print(set1);

	using lenset = set<string, decltype([](const string &s1, const string &s1){ return s1.length > s2.length;})>;
	lenset set2;
	rfill(set2,5,rname);
	print(set2);

	using absset = set<string, decltype([](int x, int y){ return abs(x) < abs(y);})>; //multak değer küçüklüğü
	absset set3{-3,9,-2,6,7,4,2,-8};
	print(set3);
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

C++ 20 İLE GELEN TEMPLATED lAMBDA

Bazı ilave avantajlar elde etmek için, function için doğrudan tempalte sentaksı ile tanımlama olanağı veriyor. Auto kullanmak yerine template sentaksı kullanarak
bazı kısıtlamalar gerçekleştirmek ve daha kolay yazım biçimi sağlayacak.

Auto kullanınca oluşacak bazı dezavantajlar

int main()
{
	auto f = [](const auto &vals){ 

		Burada val in türü func a gönderilecek argüman tarafından belirlenecek.
		Auto burada typeholder.Function çağrıldığı zaman autonun ne olduğu anlaşılacak.
		Ben burada autoya karşılık gelen türü öğrenmek istesem aşağıdakini yapacağım.

		using V = decltype(vals); // Bu vals in türünü veriyor.Bize autoya karşılık gelen tür lazım.
					  // Burada referanslık ve constluğun gitmesi için decay templateini kullanacağım.Bu bir trait

		using V = decay_t<decltype(vals)>; // artık autoya karşılık gelen türü bulduk. vals e karşılık gelen değil !!!!!!

		using T = typename V::value_type; //template argümanına bağlı türü kullandığımız için typename var.
		
		Burası vector olduğunda, biz vectorde bulunan öğenin static member funcını çağırmak sitesek
		T::func(); gibi bir çağrı yapmak gerekiyor.
		
	};
}

NOT : DECAY AUTOYA KARŞILIK GELEN TÜRÜ VERIYOR. CONST AUTO X = 23;  BURADAX İN TÜRÜ CONST INT AMA AUTOYA KARIŞIK GELEN TÜR INT !!!!!!!!!!!!

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

YENİ SENTAKSTA BUNLARA İHTİYAÇ YOK ÇÜNKÜ BİR T TÜRÜNÜ DOĞRUDAN TEMPLATE SENTAKSI İLE BELİRTEBİLECEĞİZ.

int main()
{
	auto f = []<typename T>(const std::vector<T>&vals){			//Burada type ve nontype parametre oluşturabiliriz.
		
		// Bu template sadece vector türleri için kullanılacak.
		// Bu funca vectorun int açılımı double açılımı ve string açılımı türünden nesneyi geçebiliriz.

		Doğrudan T olarak kullanıyoruz.
		T x{};

		T::foo();
	};

}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DİĞER AVANTAJIDA KISITLAMA SAĞLAMAK

int main()
{
	auto f1 = [](int x, int y){ return x + y;};

	f1(12,3.4); // Engelleyen birşey yok. Double da veri kaybı olur ama sadece uyarı verir.

	------------------------------------------------------------------------------------------------------------------------------------------------

	auto f2 = [](auto x, auto y){ return x + y;};
	
	f2(1,3.4); // 1. int , 2. double olacak. Farklı türlerden değer gönderebilirim. 

	------------------------------------------------------------------------------------------------------------------------------------------------

	auto f3 = [](auto x, decltype(x) y){ return x + y;};
	//Birinci argümanda tür çıkarımı olacak ama 2. cide olmayacak. Yani 2. parametrede tür çıkarmaya çalışmayacak.
	// zaten x e bağlı bir ifade var. x int ise decltype(x) te int olacak.

	auto n = f3(1.2,8); // 1. ve 2. double olacak.
	
	auto n = f3(1.2,"ali"); // Double a dönüşmeyen tür string sentaks hatası olacaktı

	------------------------------------------------------------------------------------------------------------------------------------------------

}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

C++20 TEMPLATED LAMBDA

int main()
{
	auto f4 = []<typename T>(T x, T y){return x + y;}; // BUrada Her iki parametrede aynı türden olmalı

	auto n = f4(12,56); //GEÇERLİ
	auto n = f4(5.6,2.3); //GEÇERLİ
	auto n = f4(5.6,2.3f); // SENTAKS HATASI. AYNI TÜRDEN ARGÜMAN KULLANILMAYA ZORLUYOR.

}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BURADA CLASS KEYWORD TE KULLANILABİLİR

auto f4 = []<class T>(T x, T y){return x + y;}; //GEÇERLİ

int main()
{
	auto f = []<typename T, int n>(T (&ra)[n]){
		for(int &x : ra)
			++x;
	};

	int a[]{1,4,7,9,11};

	f(a);

	for(int i : a)
	{
		std::cout << i << " "; 
	}

}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR:

int main()
{
	auto f = []<typename T>(vector<T> &x, const T &tval){
		x.push_back(tval);
	}

	vector<int> ivec;

	f(ivec,12); // GEÇERLİ
	f(ivec,1.2); // SENTAKS HATASI. DEDUCTION SIRASINDA TÜRLER AYNI OLMADI
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR:

int main()
{
	auto f = [](int){ return x + 5;};

	auto val = f.operator()(12);  //GEÇERLİ

	------------------------------------------------------------------------------------------------------------------------------------------------

	auto f = []<typename T>(T x){ return x + 5;};
	f.operator()<int>(16);  //GEÇERLİ. AMA MARJİNAL BİR SENARYO BU.
				// Generalized lambda expressionda zaten func call oper funcı template halinde

}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR:
int main()
{
	auto f = [](auto&& ...args){
		func(std::forward<decltype(args)>(args)...);
	};
	
	aşağıdaki gibide yazarım

	auto f = []<typename ...Args>(Args&& ...args){
			func(std::forward<args>(args)...); // DECLTYPE YOK.ÇOK GÖRDÜK BUNU :D:D
		};

}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C++20 ILE GELEN ÖZELLIK GENELDE LIBRARY YAZANLARI ILGILENDIRIYOR 

template <class F, class... Args>
auto delay_invoke(F f, Args ...args)
{
	return[f,args...](){
		return std::invoke(f,args...);
	};
}

int foo(int x, int y, int z)
{
	std::cout << "foo cagrildi\n";
	std::cout << x << y << z << "\n";
	return x + y + z;
}

int main()
{
	auto x = delay_invoke(foo,1,2,3)();
	std::cout << "x = " << x << "\n";
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

C++20 ÖNCESINDE STD::MOVE(ARGS)... SENTAKS HATASIYDI

#include <functional>
#include <utility>

template <class F, class... Args>
auto delay_invoke(F f, Args... args)
{
	return [f = std::move(f), _args = std::move(args)...]() -> decltype(auto){		// std::move(args)... SENTAKS HATASIYDI ÖNCEDEN!!!!!
		return std::invoke(f, _args...);						// KODUN TAMAMI ÇALIŞMAYABİLİR !!!
	};
}


BUNU GERÇEKLEŞTİRMEK İÇİN BAZI WORKAROUNDLAR UYDURULMUŞTU. TUPLE DAN FAYDALANILIYORDU
BURASI C++17

template<class F f, Args... Args>
auto delay_invoke(F f, Args... args)
{
	return [f = std::move(f), tup = std::make_tuple(std::move(args)...)]() -> decltype(auto){  //BURADAKİ STD::MOVE(ARGS)... SENTAKS HATASI OLUŞTURMUYOR.
		
		return std::apply(f,tup); // apply tupledaki tutulan öğeleri tek tek f e argüman olarak gönderiyor.
	};
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

C++ 20 İLE C++17 FARKI AŞAĞIDAKİ GİBİ

C++17
-----
template <class F, class ...Args>
auto delay_apply(F&& f, Args&&... args)  // Amaç elemanları perfect forward etmek
{
	return[f = std::forward<F>(f), tup = std::make_tuple(std::forward<Args>(args)...)]() -> decltype(auto)
	{
		return std::apply(f,tup); // 	
	};
}

C++20 SENTAKSA DİKKAT !!!!

template <typename F, typename ...Args>
auto delay_call(F&& f, Args&& ...args)
{
	return[f = std::forward<F>(f), ...f_args = std::forward<Args>(args)]() -> decltype(auto) { // yine aynı örnek yukarıdaki ile
		return f(f_args...);
	};
}

BU SATIRA DİKKAT
...f_args = std::forward<Args>(args)
Init captureda ellipsisten sonra bir isim ve bir ifade. Bu durumda f_args kullandığımda doğrudan forward edilmiş öğeleri kullanacağım.

void f()
{
	delay_call(g,1,2)();
}


BIZIM BIR LAMBDA IFADESI DÖNDÜRMEMIZ GEREKIYOR FAKAT DÖNDÜRDÜĞÜMÜZ LAMBDA IFADESININDE FUNCTIONUN VARIADIC PARAMETRE PAKEDINDEKI ÖĞELERI
MOVE GIBI FORWARD GIBI BIR IŞLEME SOKUP ONU TEKRAR KULLANMASI GEREKIYOR. BU SENARYOYA LİBRARY WRİTERLAR YAZIYOR

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LAMBDA RECURSIVE OLABİLİR

int main()
{
	std::function<long long int(int)> factorial = [&](int a){
		return a < 2 ? 1 : a * factorial(a - 1);
	};

	for(int i = 0; i < 10 ;++i)
	{
		std::cout << i << "! =" << factorial(i) << "\n";
	}
}

veya

int main()
{
	std::function<int(int, int)> gcd = [&](int a, int b) {
		return !b ? a : gcd(b, a % b);
	};

	std::cout << gcd(40, 72) << "\n"; // greatest common divisior. Ortak bölenlerin en büyüğü

}


AŞAĞIDAKIDE VAR BUDA BIRAZ FARKLI. BEYİN CİMNASTİĞİ :D:D

int main()
{
	const auto factorial = [](int n) {								 // LAMBDA
		const auto fact_impl = [](int n, const auto& impl) -> int	 // NESTED LAMBDA 
		{
			return n > 1 ? n * impl(n - 1, impl) : 1;
		};
		return fact_impl(n, fact_impl);
	};

	std::cout << factorial(7) << "\n";

	//Format kullanalım
	for(int i = 0; i < 10; ++i)
	{
		std::cout << std::format("{}! = {}",i,factorial(i)) << "\n"; 
	}
}

formatta {{}} koyarsak {} yazdırır.

=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================

int main()
{
	
	int x = 10;

	auto y = x;
	
	const auto y = x; // const yazabilirim.

	auto y = &x;
	const auto y = &x; // Dİkkat. burada y nin türü int *const. const int * değil.

	------------------------------------------------------------------------------------------------------------------------------------------------

	decltype(auto) val = x; // val in türü int. decltype auto da decltype kuralları geçerli

	int &r = x;
	decltype(auto) val = r; // val int& türü

	------------------------------------------------------------------------------------------------------------------------------------------------

	Decltype(auto) da ilave bir tür kullanılamiyor. yalın olmak zorunda

	const decltype(auto) SENTAKS HATASI

}

=====================================================================================================================================================================================

1-2 TANE TYPE DEDUCTION ILE ILGILI SORU SORDU HOCA YAZMIYORUM.

1 tanesi githubdan mesela örneğin

#include <vector>
#include <string>

template <typename T>
void process(std::vector<T>& vec, T val);

int main()
{
	using namespace std;

	vector<int> ivec;
	process(ivec, 10);
	//process(ivec, 'A'); //gecersiz
	vector<string> svec;
	string str{ "ali" };
	process(svec, str);
	process(svec, "can"); //gecersiz
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BİZ ÖYLE BİRŞEY YAPALIMKİ TÜR ÇIKARIMI DİĞER PARAMETRELER İÇİN YAPILMASIN

#include <vector>
#include <string>

template <typename T>
struct typeidentity {
	using type = T;
};

template <typename T>
using typeidentity_t = typename typeidentity<T>::type;

template <typename T>
void process(std::vector<T>& vec, typeidentity_t<T> val); // BURADA 2. PARAMETRE İÇİN TÜR ÇIKARIMI YAPILMAYACAK.DOĞRUDAN STRİNG TÜRÜ OLACAK.2. PARAMTRESİNE CSTRİNG GÖNDERSEK BİLE
															// BİR SENTAKS HATASI OLMAYACAK DÖNÜŞÜM VAR ÇÜNKÜ STRİNGE

int main()
{
	using namespace std;

	vector<int> ivec;
	process(ivec, 10);
	process(ivec, 'A');
	vector<string> svec;
	string str{ "ali" };
	process(svec, str);
	process(svec, "can");
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

C++20 İLE EKLENDİ TYPE TRAITS HEADER FİLE İÇERİSİNDE

type_identity_t veya type_identity<val>::type gibi açabiliriz.


#include <vector>
#include <string>
#include <type_traits>

template <typename T>
void process(std::vector<T>& vec, std::type_identity_t<T> val); // Tür çıkarımı önceki parametrede int olarak yapılmışsa mesela, 2. parametre için tür çıkarımı yapılmayıp 
																// doğrudan int kullanılacak
int main()
{
	using namespace std;

	vector<int> ivec;
	process(ivec, 10);
	process(ivec, 'A');
	vector<string> svec;
	string str{ "ali" };
	process(svec, str);
	process(svec, "can");
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UNDERLYING TYPE

C de enum ın hangi tamsayı türü olduğu belli. Baz tür int olmak zorunda.
C++ ta böyle değil. Modern C++ ta hem eski hemde yeni enumlar için kendimiz seçebiliriz.

enum class color: unsigned char {white, gray, black};
enum class pos: unsigned int{off, on, hold};

int main()
{
	using namespace std;

	underlying_type_t<color> x{}; //unsigned char x. TÜrü veriyor.
	underlying_type_t<pos> y{}; //unsigned int y
}



=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================

STRINGVIEW SINIFI
-----------------
C++17 de dile eklendi.Bunu maalesef Temel C++ ta görmedik.String view bilinmeli.
Temel varlık sebebini anlamaya çalışalkım. Bİr çok durumda salt okuma amaçlı bir yazıyı alan function yazıyoruz. Mesela C olsaydı parametre const char * olurdu.
Bu bizden salt access amaçlı bir yazı istediğini söylüyor. YAzıyı değiştirmeyeceğini söylüyor.

Dil C++ olunca, C++17 ile dile eklenen string view sayılmazsa, her seçeneğin belirli dezavantajları var.
Mesela func parametresini string türünden yaparsak bunun dezavantajları,

1 - Bu functiona herhangibir yazıyı geçemeyiz. Ya string nesnesi olacak yada bir cstring kullanılacak.

2 - String literali zaten statik ömürlü programın sonuna kalacak bir dizi. BUrada func parametresi referans olduğundan burada bir temporary materialization sözkonusu olacak ve 
	bir string nesnesi oluşturulacak. BU ilavebir maliyet. Neden bir string nesnesi oluşturulsun zaten biz cstring gönderiyoruz.

3 - Const char * yaparsak parametreyi bu seferde string nesnesi gönderemeyiz.Const char * yapınca func içindekioperasyonların bir interface e sahip olmasını istiyoruz.
	char da bu interface yok !!!!!!!!!!!! BURASI ÇOK ÖNEMLİ !!!!!

4 - Overloading yapılabilirdi ama buda okuma amaçlı bir yazıyı kullanma fikri için biraz overkill oluyor.Sırf bunun için 2-3 tane overload mu yazalım ? hayır

5 - Yazı string / cstring olmak zorunda da değil.Mesela std::array<char,20> böyle böyle bir parametre verirsek bu yazıyı tutabilir. Ya da vector<char> olabilir.


void func(const std::string& x);

int main()
{
	func("sercan satici dogukan polat"); 
										 
}

1500. HATIRLATMA
C de string literalleri char, C++ ta const char olarak saklanıyor.Değiştirmek tanımsız davranış.

char *p = "Alican"; //C de  geçerlidir.
const char *p = "Alican"; //C++ de  geçerlidir.

=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================

İDEALİ OKUMA AMAÇLI ALACAĞIM STRİNGİ BİRDE STRİNG MANİPLASYON İŞLEMLERİ İÇİN İSİMLENDİRİPMİŞ APİLER VERİLMİŞ OLMASI
HEM ÇOK DÜŞÜK MALİYETLE HERTÜRLÜ YAZIYI GEÇEYİM AMA BİR SINIFIN İNTERFACE İNEDE SAHİP OLSUN.

Yazı hangi formda olursa olsun karakterler bellekte ardışık.İster char dizisinde olsun ister string nesnesinde tutulsun(string nesnesinde tutulması SSO yapılarak tutuluyor veya
heapten alınan bir bellek alanında tutuluyor, cstring zaten static ömürlü bellekte kalıyor sürekli) Ben bir pointer alsam ve bu pointer yazı karakterlerinden birini gösterse ve bir
pointer daha alsam o da yazının daha sonraki bir karakterini gösterse bu iki pointer arasındaki tüm karakterlere erişebilirim. Pointer olmasıda zorunlu değil.
Pointer aritmetiğine göre 1. pointer 2. tamsayı değişken olsa yine bu adresi elde ederim.

Bunu bir sınıf haline getirsek, bu 2 pointer veya 1 ptr 1 tamsayı değişkeni olarak alıp bunları paketleyip member functionları olsa. Salt okuma amaçlı kullanılacak yazıyla ilgili
bir sınıf olduğu için member funcları yazı üzerinde okuma amaçlı erişim yapacak üye funclar olsun. Yazma amaçlı kullanıldığında sentaks hatası olsun.
Bu yazılabilir. İçerisinde herşeyi yazabiliriz. Bu sınıf STL deki C++17 ile gelen STRING VIEW sınıfı.Adı üstünde stringi görüntülemeye ilişkin bir sınıf.

Bu dizilerede uygulanabilir. Vectore de uygulanabilir.array ede uygulanabilir.Bellekte ardışık olarak tutulan öğelere uygulanabilen bir yapı.

=====================================================================================================================================================================================

HEADER FILE STRING_VIEW
#include <string_view>

Bu bir template. string_view bir typedef ismi.

int main()
{
	using namespace std;

	Biz string view kullanınca aşağıdakini kullanıyoruz.
	template<typename Elements,typename Traits = std::char_traits<Elements>>

	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	std::cout << "sizeof(char *) = " << sizeof(char *) << "\n"; // 4 byte
	std::cout << "sizeof(string_view) = " << sizeof(string_view) << "\n"; // 8 byte

	Kopyalama maliyeti temel türden verileri kopyalama maaliyetinden fazla değil. 

	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	MÜLAKAT/SINAV SORUSU GENELDE DANGLING REFERANS SORUSU OLACAK

	mesela sercansatici yazısındaki ercan yazısını kullanmak istesem, bu yazı stringview tarafından tutulsa, string view nesnesi bu yazının sahibi değil,
	dolayısıyla stringview oluşturulduğunda biz adresi tutmuş oluyoruz. string view adresteki yazı halen bellekte duruyor mu durmuyor mu bunu bilemez.
	Bunu kullanırken çok dikkatli olmak lazım.
	
	Baya küçük bir sınıf string_view

}

=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================

CONSTRUCTORS

int main()
{
	string_view sv; // default construct edilebilir.String view herhangibir yazının gözlemcisi durumunda olmayabilir.

	Stringview içinde stringte olan neredeyse tüm okuma araçlı funclar var.

	sv. interface e bakıldı. NOT : data functa null terminated byte stream olarak kullanma garantisi yok. olmayadabilir oladabilir.
	2 tanesi hariç hepsi const functionlar
}

remove_prefix
remove_suffix  Bunlar gözlem aralığını değiştiriyor


MOLA

=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================


Default construct edilmi bir stringview nesnesi bir yazıyı göstermiyor.


int main()
{
	string_view sv;

	cout << boolalpha;

	std::cout << sv.empty() << "\n"; // true
	std::cout << sv.size() << "\n"; // 0
	std::cout << sv.length() << "\n"; // 0
	std::cout << (sv.data() = nullptr) << "\n"; // true

	--------------------------------------------------------------------------------------------------------------------------------------------
	
	Gözlemci olsaydı

	sv = "Ali";
	std::cout << sv.empty() << "\n"; // false
	std::cout << sv.size() << "\n"; // 3
	std::cout << sv.length() << "\n"; // 3
	std::cout << (sv.data() = nullptr) << "\n"; // false
}


-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------

CTORLAR DEVAM

int main()
{
	using namespace std;

	char str[] = "necati ergin c++ anlatiyor";

	string_biew sv(str);  // sv, str yazısının gözlemcisi oldu

	std::cout << sv.size() << "\n"; // 26
	std::cout << sv<< "\n"; //necati ergin c++ anlatiyor. Bu yazıyı yazdırması demek sonunda null character var demek değil.

	------------------------------------------------------------------------------------------------
	
	string_biew sv(str,6);

	std::cout << sv.size() << "\n"; // 26
	std::cout << sv<< "\n"; //necati. 

	std::cout << sv.data(); // Buradaki çağrılan func cstring parametreli func oldupunda bu yazı nullchar görene
							// kadar yazacak.YANİ necati ergin c++ anlatiyor yazacak yine

	------------------------------------------------------------------------------------------------

	array<char,5> ar{'a','h','m','e','t'};

	string_view sv{ar.data(),ar.size()};

	std::cout << sv << "\n"; // ahmet yazar
	std::cout << sv.data() << "\n"; // TANIMSIZ DAVRANIŞ.Burada çağrılan func yazı sonunda '\0' arıyor.

	std::cout << sv.size() << "\n"; // 5 yazar.

	------------------------------------------------------------------------------------------------

	string str{"gokhan zilanli"};

	string_view sv{str};

	std::cout << "sv = " << sv << "\n";
	std::cout << "sv.size() = " << sv.size() << "\n";

}

-----------------------------------------------------------------------------------------------------------------------------------------

DATA CTOR
---------

int main()
{
	using namespace std;

	string str[] = "necati ergin c++ anlatiyor";
	string_view sv{str.data(), 6};

	std::cout << "sv = " << sv << "\n";
	std::cout << "sv.size() = " << sv.size() << "\n";

}

-----------------------------------------------------------------------------------------------------------------------------------------

RANGE CTOR
----------

int main()
{
	using namespace std;

	string str[] = "necati ergin c++ anlatiyor";

	//char str[] = "Enesalp"; // char da olabilirdi tabiki.

	string_view sv{str.data() + 3, str.data() + 8};

	std::cout << "sv = " << sv << "\n";
	std::cout << "sv.size() = " << sv.size() << "\n";
}



void func(std::string_view sv) // Okuma amaçlı yazı istiyor.
{
	
}

int main()
{
	char str[]{"necati ergin"};
	std::string sx = {"mustafa"};
	std::string_view sv;

	func("bitcoin rises");
	func(str);
	func(sx);
	func(sv);

}

-----------------------------------------------------------------------------------------------------------------------------------------

Stringview kullandığımızda bu yazının sahibi değiliz.Sahibi başkası

int main()
{
	char str[]{"Necati ergin"};

	string_view sv = str;

	std::cout << "sv = " << sv << "\n";

	str[1] = 'u';
	str[3] = 'x';

	std::cout << "sv = " << sv << "\n"; // Yazıyı yazdırır.Adres değişmedi. Değişmiş yazıyı yazdık sadece.
}

-----------------------------------------------------------------------------------------------------------------------------------------

Burada auto ile yapılan type deductiona dikkat


int mani()
{
	auto x = "mustafa";
	auto y = "mustafa"s; // Bu literaling return değeri string
	auto z = "mustafa"sv; // string view için literal operator func var. z nin türü string view oldu.
}



template<typename T>
void foo(T x)
{
	
}

int main()
{
	using namespace std::literals;

	foo("mustafa"); // Parametre const char *
	foo("mustafa"s); // Parametre string
	foo("mustafa"sv); // Parametre string view olur
}

Stringin büyük olduğu yerlerde performans farkıda o kadar büyük olur. 
Burada literal operator func konusunu anlatacağını söyledi hoca. Temel C++ sonlarına doğru gördük.

=====================================================================================================================================================================================
=====================================================================================================================================================================================

TİPİK YANLIŞ KULLANIMLAR

- BIR FUNCIN RETURN DEĞERI STING_VIEW YAPILMAMALI

class Person{
public:
	std::string getname()const;
private:
	std::string mname;
};

int main()
{
	Person p;
	string_view sv = p.getname(); // BU FUNCIN RETURN DEĞERINI BIR CONST LVALUE REF VEYA RVALUE REF INITIALIZE ETMEK IÇIN KULLANIRSAK
}				      // LIFE EXTENSION OLUR AMA BUNU BIR STRINGVIEW NESNESINE ILK DEĞER OLARAK VERIRSEK, BU NESNE ASLINDA SADECE
				      // BU YAZIYA ILIŞKIN POINTER TUTUYOR. BU DURUMDA FUNC RETURN DEĞERI NESNENIN HAYATI BITTIĞINDEN
				      // SV DANGLING REFERANS OLACAK.


-----------------------------------------------------------------------------------------------------------------------------------------

ÖR:

string func()
{
	string s{"akif"};

	return s;
}

int main()
{
	string_view sv = func();
	
	std::cout << "sv = " << sv << "\n"; // TANIMSIZ DAVRANIŞ.
}

-----------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	string sx{"mustafa bakircioglu"};

	string_view sv{sx};

	std::cout << "sv = " << sv << "\n";

	sx.assign(100,'A'); // Yazı büyüdü. Reallocation oldu.Yazı taşındı.string_view içindeki adres dangling hale geldi

	std::cout << "sv = " << sv << "\n"; //DANGLING POINTER OLUŞTU. TANIMSIZ DAVRANIŞ
}

-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------

OKUMA AMAÇLI HER İŞLEMDE STRİNG VİEW KULLANABİLİRİZ.

void process(string_view sv)
{
	sv.substr(); // Böyle çağırmak sv.substr(0, npos); yani tamamının kopyası

	------------------------------------------------------------------------------------------------

	sv.substr(15); // bu indexten başyarak geriye kalan hepsi, sv.substr(15, npos); aynısı

	------------------------------------------------------------------------------------------------

	auto sx = sv.substr(15,20); // 15. indexten başla 20 karakter substring al.
								// sx türüde string view.

	------------------------------------------------------------------------------------------------

	if(auto idx = sv.find('.'); idx != string::npos) // Arama işlemide aynı şekilde yapılır.
	{
		//...
	}

}

int main()
{
	string sx{"mustafa bakircioglu"};
}

-----------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	string sx {"mustafa bakircioglu"};

	cout << boolalpha << sx.starts_with("mustafa") << "\n"; // True
	cout << boolalpha << sx.starts_with("musti") << "\n"; // False
	
	cout << boolalpha << sx.ends_with("oglu") << "\n"; // True
	cout << boolalpha << sx.ends_with("Aglu") << "\n"; // False
}

*/
