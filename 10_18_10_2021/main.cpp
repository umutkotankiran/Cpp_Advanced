/*

STRİNG VIEWDA KALMIŞTIK.
TEMEL VARLIK NEDENI YAZI İLE İLGİLİ İŞLEMLERDE YAZININ KOPYALANMASI İLE OLUŞAN MAALİYETTEN KAÇMAK.

1 pointer 1 tamsayı olabilir içerisinde, 2 pointer olabilir. Standartlar bunu söylemiyor.
Maliyeti az, 2 pointer olsa 8byte zaten. String kopyalanmasına göre çok çok daha maliyetsiz.

1 - String view nesnesi yazının sahibi değil, başkasının yazısının adresini tutuyor içinde.
	Stringview hayatta iken ilgili yazının hayatı biterse dangling referans olacak.
	
2 - Bir functionun stringview döndürmesi en sık hatalardan biri. Otomotik ömürlü nesneyi stringview
	ile dönmek mesela.

3 - Stringview ın bir interface i var.String sınıfının tüm get interface i stringviewda var.
	2 noktaya dikkat etmek lazım. Datayı çağırınca string vector array de olduğu için bir adres veriyor.
	Bu adres stringviewda tutulan adres. Dolayısı ile bu yazı sonunda null character olmak zorunda değil.
	olabilir, olmayadabilir. Yani data funcının return değerini null termnated byte stream olarak kullanma girişimi
	TANIMSIZ DAVRANIŞ.

4 - Başka bir dangling ptr senaryosu. String nesnesinin tuttuğu bir yazıya pointer içeriyorsa, string
	sınıfı türünden nesne belirli işlemlerle yazı büyütülünce reallocation yapıyor, reallocation yaparsa
	bizim pointer yine dangling hale gelecek

5 - Sadece bir pointer tuttuğumuz için bizim pointer hayatta iken yazı değişebilir.
	Yani pointer dangling değil ama adresteki yazı değişti. Bu durumda biz o pointer yada stringview
	nesnesini kullanınca değişmiş yazıyı elde ederiz.


Ctorlara, member funclara baktık.


=================================================================================================================================
=================================================================================================================================
=================================================================================================================================
=================================================================================================================================

#include<string_view>
#include<string>

template <typename T>
T concatanate(const T &x, const T&y)
{
	return x + y;
}

std::string operator+(std::string_view x, std::string_view y)
{
	return std::string{ x } + std::string{ y };
}

int main()
{
	std::string_view sv{"harun"};
	auto val = concatanate(sv,sv);
	std::cout << "val = " << val << "\n"; //TANIMSIZ DAVRANIŞ
}

Concatanate te T türü stringview olacak, bu durumda toplama operatörünün oprandları stringview türünden.
Çağrılan function bizim yazdığımız operator+ olacak.

operator+ içinde stringler toplanmış ve return değeri string. Concatanate içinde ise return değeri T yani stringview
x + y türü string idi, bunu return ettiğimizde T türüne yani string_view türüne dönüşecek.
Buradaki yanıltıcı nokta funcın return değerinin string değil stringview olması.

Ömrü bitmiş bir sağ taraf nesnesini stringview ile döndürdük


---------------------------------------------------------------------------------------------------------------------------------

ÖR:

std::string get_str(int ival)
{
	//..
	return std::to_string(ival);
}

int main()
{
	int x;

	std::cout << "bir tamsayi girin\n";
	cin>>x;

	auto &r1 = get_str(x); // SENTAKS HATASI
	const auto &r2 = get_str(x); // KOD LEGAL. LIFE EXTENSION
	auto &&r3 = get_str(x); //GEÇERLİ

	std::string_view sv = get_str(x); //TANIMSIZ DAVRANIŞ VAR.SAĞ TARAFDEĞERİ VE ÖMRÜ BİTTİ. ÖMRÜ BİTMİŞ NESNEYİ TUTUYOR STRINGVIEW
}

---------------------------------------------------------------------------------------------------------------------------------

TEMEL C++ TA LITERAL OPERATOR FUNCLARI GÖRDÜK.
KISACA HOCA BAHSETTİ.ANLATILACAK BURADA DA

---------------------------------------------------------------------------------------------------------------------------------

int main()
{
	using namespace std::literals;

	std::string_view x = "abc\0\0def";
	std::string_view y = "abc\0\0def"sv;

	std::cout << "x.size() = " << x.size() << "\n"; // 3. Constructor Cstring parametreli, \0 a kadar alır bu sebeple karakterleri
	std::cout << "y.size() = " << y.size() << "\n"; // 8. BU stringview nesnesi, tüm karakterler dahil burada.Artık cstring ctor değil. 
}

y yi kullanınca bu bir func çağprısına karşılık geliyor.

---------------------------------------------------------------------------------------------------------------------------------

ÇOK ÖNEMLİ BURASI !!!!!!!!!!!!!!

int main()
{
	using namespace std;

	char str[] = {'a','l','p','e','r'};

	string_view sv{str}; //Burada çalışan cstring parametreli ctor

	std::cout << sv << "\n"; // UNDEFINED BEHAVIOR VAR. NULL TERMINATED BYTE STREAM DEĞİL.
	
	-------------------------------------------------------------------------------------------------------------
	
	char str[] = {'a','l','p','e','r'};

	string_view sv{str,5}; 

	std::cout << sv << "\n"; // Şuanda kod geçerli.Çalışan inserter string view inserterı.Nullchar beklemiyor.
	
	-------------------------------------------------------------------------------------------------------------
	
	char str[] = {'a','l','p','e','r'};

	string_view sv{str,5}; 

	std::cout << sv.data() << "\n"; // UNDEFINED BEHAVIOR.Çünkü şuanda çalışan cstring inserter. 
					// null char istiyor ama yazı sonunda nullchar yok :D:D
}

---------------------------------------------------------------------------------------------------------------------------------

int main()
{
	string_view sv{"Enes Alp"};

	string str = sv; //Sentaks hatası. Ctor explicit
}

---------------------------------------------------------------------------------------------------------------------------------

void func(std::string str);

int main()
{
	string_view sv{"Enes alp"};

	func(sv); //SENTAKS HATASI
	func(string{sv}); // GEÇERLİ
}

---------------------------------------------------------------------------------------------------------------------------------

RETURN DEĞERİ STRING OLAN NESNENIN RETURN İFADESİ STRING_VIEW OLAMAZ

string func()
{
	string_view sv{"Enes alp"};

	return sv; // BURASI SENTAKS HATASI
}

---------------------------------------------------------------------------------------------------------------------------------

int main()
{
	string str{"melih çiçek"};

	string_view sv = str; // Burada sentaks hatası yok.Çünkü string_view ın string parametreli Ctoru var.

	sv = str; // Burada atama var. HATA DA YOK. NASIL OLUYORDA GEÇERLİ OLDU ??
		  // TÜR DÖNÜŞTÜRME OPERATOR FUNCTIONU
		  // str.operator std::basic_string_view<char, std::char_traits<char>>();
		  // Bu bildiğimiz tür dönüştürme operator func.Stringview gereken yerde string kullnırsak
}		  // Bu stringi stringviewa dönüştürecek. 

---------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------

REMOVE_PREFIX / REMOVE_SUFFIX
-----------------------------
Bunlar gözlem aralığını daraltıyor. Normalde biz string view nesnesi olarak gözlemciyiz sahibi değiliz.
Bu alanı istersek daraltabiliriz. Bu yazıyı değiştirmek değil string view içindeki pointerları değiştirmek oluyor.

remove_prefix : Gözlem aralığını önden daraltıyor
remove_suffix : Gözlem aralığını sondan daraltıyor


int main()
{
	string_view str{"melih cicek"};

	std::cout << "(" << str << ")\n"; // melih cicek
	str.remove_prefix(6);
	std::cout << "(" << str << ")\n"; // cicek
	
	---------------------------------------------------------------

	std::cout << "(" << str << ")\n"; // melih cicek
	str.remove_suffix(6);
	std::cout << "(" << str << ")\n"; // melih
	
}

Bu function sadece pointerın değerini değiştirdi.


---------------------------------------------------------------------------------------------------------------------------------

KOLAY GIBI GÖRÜNEN ZOR SORU !!!!

int main()
{
	string_view str{"melih cicek"};
	std::cout << "(" << str << ")\n";
	str.remove_suffix(6);

	cout << str.data() << "\n"; // melih çiçek yazar. Data değişmedi. O halen melih çiçekin adresi.

	---------------------------------------------------------------------------------------

	DİKKAT!!
	BU FARKLI !!!
	
	std::cout << "(" << str << ")\n";
	str.remove_prefix(6); // dikkat!!! ön tarafdan daralttık

	cout << str.data() << "\n"; // ŞIMDI ÇİÇEK YAZAR
}

---------------------------------------------------------------------------------------------------------------------------------

ÖR:

#include <string>
#include <iostream>
#include <string_view>
#include <charconv> // C++17 ile ekenen bir func var. Bu sebeple eklendi.
#include <optional>

struct from_chars_result{
	const char *ptr;
	std::errc ec;
};

C++17 ile geldi std::from_chars isimli func geldi.

Buradaki olay return değerinin bir struct olması.
from_chars_result türünden struct. ilk elemanı const char * türünden.
Bu kullanılmayan ilk karakteri gösteriyor. Burada ptr nin değeri yukarıdaki A nın adresi olacak
İkinci eleman ayrı bir tür. Bir hata kodu taşıyan yapı elemanı.
Bir hata olmaması durumunda ec value init edilmiş bir std::errc olmalı. Bu derleyiciye göre
değişen bir tür. ec nin hata kodundan hatanın niteliğinide öğrenebiliyoruz. 

---------------------------------------------------------------------------------------------------------------------------------

int main()
{
	const char*p = "2345.3432Ali";
	double dval;

	cout << fixed;
	if(auto[ptr,ec] = from_chars(p, p + std::strlen(p), dval); ec == std::errc{}) // bu durumda başarılı oldu demektir.
	{
		cout << "dval = " << dval << "\n";
		cout << "ptr - p =" << ptr - p << "\n";
	}
	else
	{
		std::cout << "hata\n"; 
	}
}

ÇIKTI
-----
2345.3432
10

MOLA

====================================================================================================================================================
====================================================================================================================================================
====================================================================================================================================================
====================================================================================================================================================
====================================================================================================================================================

std::optional<int> asInt(std::string_view sv)
{
	int val;
	auto[ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);

	if(ec != std::errc{}) // ec değeri std::errc{} değilse hata var demektir.
	{
		return std::nullopt; // hata varsa boş bir optional dönülmüş.
	}
	return val;
}

int main()
{
	for(auto s : {"42","077","hello","0X33"}) // Popüler bir idiyom.Temel C++ ta görüldü.İnitializer list bu.
	{
		std::optional<int> oi = asInt(s); // func param string_view ama param const char * yani problem yok
		//auto oi = asInt(s); te yazılabilirdi
		
		if(oi) // veya oi.has_value da yazılabilirdi. veya io != nullopt veya oi.operator bool() :D:D:D:D
		{	
			std::cout << "(" << s << ") yazisindan eldeedilen tamsayi : " << *oi << "\n";
		}
		else
		{
			std::cout << "(" << s << ") yazisindan tamsayi elde edilemiyor : " << *oi << "\n";
		}
	}
}

ÇIKTI
-----
(42) yazisindan eldeedilen tamsayi : 42
(077) yazisindan eldeedilen tamsayi : 77
(hello) yazisindan tamsayi elde edilemiyor
(0X33) yazisindan eldeedilen tamsayi : 0


---------------------------------------------------------------------------------------------------------------------------------

ÖR:

int main()
{
	string str;

	std::cout << "bir yazi girin\n";
	getline(cin, str);

	std::cout << "|" << str << "|" << "\n"; //yazı başında boşluklar olabilir.
											// biz stringview nesnesi istiyoruz ve bu başlarındaki boşlukları almayacak
											// Yazı tamamen boşluklardan da oluşabilir.
	string_view sv{str};

	sv.remove_prefix(sv.find_first_not_of(" \t")); // Bu function verilen karakterlerden biri olmayan ilk karakterin indexini veriyor.

	std::cout << "|" << sv << "|" << "\n"; // Yazı tamamen boş olduğunda string::npos veriyor.HATA VERİTOR


	---------------------------------------------------------------------------------------------------------

	YAZI TAMAMEN BOŞSA BUNADA BİR ÇÖZÜM BULMALIYIZ

	string str;

	std::cout << "bir yazi girin\n";
	getline(cin, str);

	std::cout << "|" << str << "|" << "\n"; //yazı başında boşluklar olabilir.
											// biz stringview nesnesi istiyoruz ve bu başlarındaki boşlukları almayacak
											// Yazı tamamen boşluklardan da oluşabilir.
	string_view sv{str};

	sv.remove_prefix(min(sv.find_first_not_of(" \t"),sv.size())); //DİKKAT !!! Min func geldi.
									//Burada eğer yazı tamamen boşsa npos max tamsayı değeri olacak ve diğer sayıyı yani 
									// sv.size() değerini döndürecek min func. Eğer bir index dönerse zaten bu size den ufak olacak 
									// çünkü zaten yazı içerisinden index dönüyor. Bu şekilde dönen indexe göre hareket edilecek.
	std::cout << "|" << sv << "|" << "\n";				// artık tamamı yazı olsa bile sorun çözülmüş oldu
}

---------------------------------------------------------------------------------------------------------------------------------

DOSYADAKİ BÜTÜN KARAKTERLERI HEPSINI BIR STRING NESNESINE ATMAK ISTIYORUZ

İterator sınıfıyla yapılabilir.

int main()
{
	ifstream ifs{"notlar.txt"};

	if(!if)
	{
		cerr << "dosya açilamadi\n";
		return 1;
	}

	--------------------------------------------------------------------------------------------------------

	string str{istream_iterator<char>{ifs},{}};
	std::cout << "str.size = " << str.size() << "\n";

	//Mesela
	string::size_type idx;
	size_t n;

	str.substr(idx,n); // Bu bir string döndürecek.Eğer substring 10 bin karakter uzunluğunda ise biz 10 bin karakter
						// uzunluğunda string yaratmış oluyoruz. Bunun yerine string view kullanabiliriz.

	--------------------------------------------------------------------------------------------------------
	
	string str{istream_iterator<char>{ifs},{}};

	string::size_type idx;
	size_t n;

	string_view sv{str};
	//..
	auto x = sv.substr(idx,n); // return değeride stringview. Kopyalama yok 10 bin byte gibi birşey
	//bu tip string parse işlemlerinde çok önemli.
}

---------------------------------------------------------------------------------------------------------------------------------

Bir soru üzerine

decltype(auto) func(int x)
{
	///

	return (x); //UB
}

decltype(auto) da (x) ifadesi bir ifade formunda ve bu ifade olduğundan value categoriside lvalue expr olduğundan
aslında burası int& tir.rvalue expr olsaydı int, xvalue ex int&& olacaktı

Otomatik ömürlü nesneyi referans dönmüş. UNDEFINED BEHAVIOR

---------------------------------------------------------------------------------------------------------------------------------

CPPREFERENCE SERGEY ZUBKOV ÖRNEĞİ

#include <charconv>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <system_error>

int main()
{
	for (std::string_view const str : {"15 foo", "bar", " 42", "5000000000"})   //auto ya bırakmamış kendisi yazmış türü
	{
		std::cout << "String: " << std::quoted(str) << ". "; // std::quoted çift tırnak içine alma işlemi yapıyor.

		int result{}; // Otomatik ömürlü bir değişkene verecek bir değer yoksa,
					  // daha sonra değer verilecekse bile yinede value init etmek öneriliyor.cppcore guideliness tada böyle diyor.
					  // Yani 0 değeri ile başlattık.

		auto [ptr, ec] { std::from_chars(str.data(), str.data() + str.size(), result) };

		if (ec == std::errc()) // Burada başarılı olmuşsa girecek.
		{
			std::cout << "Result: " << result << ", ptr -> " << std::quoted(ptr) << '\n';
		}
		else if (ec == std::errc::invalid_argument) //ec eğer std::errc::ivalid_argument değeriyse bunun sayı olmadığı anlamına gelir 
		{
			std::cout << "That isn't a number.\n";
		}
		else if (ec == std::errc::result_out_of_range) // Burada da range dışı int sınırını aştığını söylüyor.
		{
			std::cout << "This number is larger than an int.\n";
		}
	}
}

ÇIKTI
-----
String: "15 foo". Result: 15, ptr -> " foo"
String: "bar". That isn't a number.
String: " 42". That isn't a number.
String: "5000000000". This number is larger than an int.

---------------------------------------------------------------------------------------------------------------------------------

quated i kendimiz yazsak aşağıdaki gibi yazabiliriz.
std::string myquated(std::string str)
{
	return '\"' + str + '\"'
}

string için olanı C++14
stringview için olanı C++17 de geldi.


============================================================================================================================================
============================================================================================================================================
============================================================================================================================================
============================================================================================================================================
============================================================================================================================================

LITERAL OPERATORS
-----------------
TEMEL C++ 44. DERSTE GÖRÜLDÜ.
Modern C++ ta geldi.Hem standart library kullanıyor hemde custom yazabiliriz.

"Alican"s buda öyle.

Bunlar iki ayrı kategoride 
- cooked 
- uncooked

Cooked olması durumunda son ekten önceki veriyi doğrudan bir funca argüman olarak gönderiyor.
123_x  123 ü doğrudan literal operator func a argüman geçiyor. func ında parametresinin tamsayı olması gerek.

Uncooked ta ise veriyi yazı olarak gönderiyor. Function içinde yazıyı karakter karakter kullanacağız yada
doğrudan adresteki yazıyı kullanacağız.

int main()
{
	"alican"s; // aslında func çağırdık return değeri string
	"alican"sv; // rerturn değeri stringview
}

Özellikle chronoda çok kullanılıyor.

constexpr auto x = 12h + 23min + 45s + 7987ms;

Kendi oluşturduklarımızda _ ile ön ek vermek zorundayız.funclar constxpr olmak zorunda deil ama olmazsa
sabit ifadesi olmayacak.

constexpr double operator""m()	// bu mesela yazılamaz.

Yazdığımız literal operator funcların bir namespace içinde bulunması gerekiyor.İsim çakışması olmaması için.

-------------------------------------------------------------------------------------------------------------

Burada mm km m örneği yapıldı. Yazmadım

-------------------------------------------------------------------------------------------------------------

Uncooked örneği yapalım

int operator""_nec(const char *p)
{
	for(int i = 0; p[i] != '\0';++i)
	{
		std::cout.put(p[i])
	}
}

int main()
{
	auto val = 123549_nec; //operator""_nec("123549") çağrısı yapıldı aslında
}

-------------------------------------------------------------------------------------------------------------

SICAKLIK BIRIMLERI

constexpr double operator""_c(long double val)
{
	return static_static<double>(val);
}

constexpr double operator""_fag(long double val)
{
	return static_cast<double>((val - 32) / 1.8);
}

int main()
{
	constexpr auto diff = 38.7_c - 50_fag;
}

-------------------------------------------------------------------------------------------------------------

ÖR:

constexpr size_t operator""_KB(unsigned long long val)
{
	return static_cast<size_t>(val * 1024);
}
constexpr size_t operator""_MB(unsigned long long val)
{
	return static_cast<size_t>(val * 1024 * 1024);
}
constexpr size_t operator""_GB(unsigned long long val)
{
	return static_cast<size_t>(val * 1024 * 1024 * 1024);
}

int main()
{
	array<char, 10_KB> a; // array<char, 10240> gösterir.

	std:::cout << 23_GB + 5_MB + 45_KB << "\n";

	array<char, 1_MB> a; // array<char, 10240> gösterir.
}

-------------------------------------------------------------------------------------------------------------

Cooked örneği

Date operator""_date(const char* p, size_t n)
{
	return Date(p);
}
int main()
{
	std::cout << "21_12_1987"_date; // Yapılan çağrı operator""_date("21_12_1987",11);
}

-------------------------------------------------------------------------------------------------------------

std::string operator""_s2(const char *p, size_t) // size_t c gibi bir değişken adı verilmedi
{
	return string(p) + p;
}

std::string operator""_s3(const char *p, size_t)
{
	return string(p) + p + p;
}

int main()
{
	auto str = "sercan"s2;
	auto s = "enes"s3;
	
	std::cout << str << "\n" << s << "\n";
}

Çıktı
sercansercan
enesenesenes

-------------------------------------------------------------------------------------------------------------

constexpr int operator""_i(char c)
{
	return c;
}
int main()
{
	auto constexpr x = 'A'_i ; // 65 burası :D. A nın kodu 65 ve return değer int
	std::cout << 'A'_i << '+'_i << '0'_i;
}

-------------------------------------------------------------------------------------------------------------

HOCADA KLAVYE PROBLEMİ ÇIKTI COOKED VE UNCOOKED ARASINDAKİ FARKA BAKACAKTIK
15dk zaten çok birşey değil.
-------------------------------------------------------------------------------------------------------------


*/
