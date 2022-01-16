/*

Visual Studiodaki hata vardı hocaya sormuştum onu açıklıyor hoca.

C++17 de aşağıdaki ilave edildi. Nontype parameter. C++20 ile double veya literal type ta olabilir
template<auto n>
class Myclass{

};

int main()
{
	Myclass<5.5> x; // n yukarıda nontype parametre.
}

----------------------------------------------------------------------------------------------

Bu type parametre ama constraint.std::integrali satisfied edece concept bekleniyor.
template<std::integral T>
class Myclass{

};

int main()
{
	Myclass<double> x; // T burada type parametre
}

----------------------------------------------------------------------------------------------

Bu nontype parametre.Yine nontype ama template argümanı olan sabitin integral türlerden olmalı.
Auto yerine bir type gelecek, x te o type tan nontype parametre olacak fakat
integral type olması gerekiyor.

template<std::integral auto x>
class Myclass{

};

int main()
{
	Myclass<5.5> x; // Geçersiz
	Myclass<10> y; // Geçerli
}					

----------------------------------------------------------------------------------------------

template<typename T, std::convertible_to<T> auto x>
class Myclass{
	
};

int main()
{
	Myclass<int,10L> x; // Vİsual Studioda bug var.Bu geçerli
	
}

---------------------------------------------------------------------------------------------------

void func(std::integral auto x)
{
	
}

template<std::integral T> bunu yazmanın kısa yolu
void func(T x);

---------------------------------------------------------------------------------------------------

void func(auto x)
{
	
}

template<typename T> // yukarıdaki de bunu yazmanın kısa yolu
void func(T x);

---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------

DECLVAL 
-------
Generic kodlarda bazı durumlarda decltype specifier kullanarak type deduction yapıyoruz. Decltype a argüman
olarak bir ifade veriyoruz. Bu generic kodlarda çok karşımıza çıkıyor. 
Conceptlerden önce kullanılan expression sfinae tekniklerinde çok sık kullanılıyordu.
Conceptten sonra sfinae den kurtuldul

Declval : Decltype ile yazılan ifadelerde decltype a operand olarak verilen ifadede (unevaluated coontext)
bazen şöyle bir durum oluyor.

class Myclass{
public:
	double func();
	Myclass() = delete;
};

int main()
{
	decltype(Myclass{}.func()) x{}; // Sınıfın default ctoru çağrılamadığı için hata.Normalde double
}	

decltype'a operand olarak verilen ifadenin unevaluated contex oluşturması farklı birşey ama bu ifadenin 
sentaks açısından geçerli olması gerekiyor. Neden geçerli değil çünkü default ctor çağrılamaz.


BURADA SAHNEYE DECLVAL ÇIKACAK !!!!!!!!!!!!!!!!!!!
Utility içinde sadece bildirilen tanımlanmayan bir func şablonu oluşturmuşlar. 
Burada hem func funcını çağıran bir ifade olacak, hem unevaluated context hemde ifade geçerli olacak.

template<typename T>
typename std::add_rvalue_reference_t<T> Declval()noexcpet
{};

T mylcass ise func return değeri türü myclass&& olacak


int main()
{
	decltype(Declval<Myclass>().func()) x{};
}

Sentaks açısından legal. Declval<Myclass>() myclass türünden nesne
bununda func functionunu çağırmamızda hata yok.Declval bu.
Bu tür durumlarda default ctoru olmamasına rağmen sınıfın, default construct
edilmiş bir geçici nesne üstüne bir ifade kurulduğunda, decltype ifadesinde
bir hata olmamasını sağlıyor.

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

DEVAM EDİYORUZ

15-20 ÖRNEK YAZILACAK

MAXIMUMU BULAN TEMPLATE

#include<ranges>
#include<vector>

template<std::ranges::input_range Rng, typename Projection = std::identity>
std::ranges::range_value_t<Rng> gmax(Rng &&rg, Projection proj = {})
{
	if(std::ranges::empty(rg))
	{
		return std::ranges::range_value_t<Rng>{};
	}

	auto pos = std::ranges::begin(rg);
	auto max_iter = pos;

	while(++pos != std::ranges::end(rg))
	{
		if(std::invoke(proj,*pos) > std::invoke(proj,*max_iter))
			max_iter = pos;
	}

	return *max_iter;
}

int main()
{
	using namespace std;

	vector ivec{2,-4,8,17,-90,40,34,13,4};

	auto val = gmax(ivec);

	std::cout << "val = " << val << '\n';
 
	-----------------------------------------------------------------------

	vector ivec{2,-4,8,17,-90,40,34,13,4};

	auto val = gmax(ivec, [](int x){ return abs(x);});

	std::cout << "val = " << val << '\n';

}

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------


VIEWLARA BAKIYORUZ

int main()
{
	auto f{[](int x){ return x * 10; }};

	//views::iota(1,30);   buda olurdu
	//constexpr auto v = ranges::transform_view{ranges::reverse_view {ranges::iota_view{1,30}},f}; //böyle yazmak yerine aşağıdaki gibi yazılıyor.
	
	//ranges::transform_view a{ ranges::reverse_view {ranges::iota_view{1,30}},f }; //Aynı zamanda böylede yazılabilir.

	constexpr auto v = views::iota(1,30) | views::transform(f);

	vector<int> ivec{v.begin(), v.end()};	

	for(auto val : ivec)
	{
		cout << val << ' ';
	}
	
}

--------------------------------------------------------------------------------------------------------------

COMPILE TIMEDA POLINDROM TESTİ

kalas yok kutuk koy salak
at sahibi gibi hasta
para hazır ama rıza harap

[[nodiscard]] constexpr bool is_palindrome(const std::string_view sv)
{
	auto first_part = sv | std::views::take(sv.size() / 2); // yarısını aldık
	auto second_part = sv | std::views::reverse | std::views::take(sv.size() / 2); // diğer yarısının tersini aldık

	return std::ranges::equal(first_part,second_part); // ve bunları kıyasladık.
}

int main()
{
	constexpr auto b = is_palindrome("ey edip adanada pide ye");

	constexpr auto b = is_palindrome("parahaziramarizaharap");

	constexpr auto b = is_palindrome("ananacidedenededicanana"); // hepsi true
}


--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

KEYS VALUES ÖRNEĞİ

get interfaceini implemente eden 2 öğeye sahip rangelerde key ve values adı üstünde keyleri ve value ları veriyor.

#include<map>
#include<range>
#include<iostream>
#include"nutility"
#include<string>

int main()
{
	using namespace std;

	map<string,int> simap;

	rfill(simap,20,[]{ return pair{rname() + ' ' + rfname()}, Irand{0,10'000}() }; );

	print(simap,"\n");

	auto v = simap 
			| views::filter([](const auto &s){ return s.first.length() > 12; })
			| views::take(3)
		 	| views::keys;			// view::values ta kullanabilirdik yada views::elements<1>

	for(const auto &elem : v)
		std::cout << elem << '\n';

	-----------------------------------------------------------------------------------------------

	auto v = simap
			| views::filter([](const auto &s){ return s.first.length() > 12; })
			| views::take(3)
			| views::values;	// DİKKAT !!!!!

	for(const auto &elem : v)
		std::cout << elem << '\n';
	
	-----------------------------------------------------------------------------------------------

	auto v = simap
			| views::filter([](const auto &s){ return s.first.length() > 12; })
			| views::take(3)
			| views::elements<0>;	// DİKKAT !!!!!

	for(const auto &elem : v)
		std::cout << elem << '\n';

}

MOLA

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

#include<vector>
#include<utility>
#include<ranges>

int main()
{
	using namespace std;

	vector<pair<int,string>> isvec;

	for(int i = 0; i < 30; ++i)
	{
		isvec.emplace_back(Irand{0,1000}(), rname());
	}

	print(isvec,"\n"); // pairleri yazdık.

	size_t len;
	std::cout << "uzunluk : ";
	cin >> len;

	auto pred = [len](const string &s){ return s.lenght() == len};

	for(const auto &s : isvec | views::values | views::filter(pred))
	{
		std::cout << s << ' ';
	}

}

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

ELEMENTS

Aslında keys ve values element view ın özel hali, sadece typedef ismini vermişler.(muhtemelen)

int main()
{
	using namespace std;

	using Person = tuple<int, string,string, double>;
	vector<Person> pvec;

	Irand irand{0,100'000};
	Drand drand{15., 99.99};

	for(int i = 0; i < 100; ++i)
	{
		pvec.emplace_back(irand(), rname(), rfname(),drand());
	}

	auto vw = views::elements<0>(pvec); // bu intleri 
	auto vw = views::elements<1>(pvec); // bu isimleri 
	auto vw = views::elements<2>(pvec); // bu soyismi
	auto vw = views::elements<3>(pvec); // bu double ları alır

	for(const auto &x : vw)
	{
		std::cout << x << " "; 
		//std::cout << std:format("{}\n",x);   böylede yapabilirdik.
	}
}

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

GÜZEL BİR ÖRNEK

template<typename Iter>
void algo(Iter beg, Iter end)
{
	while(beg != end)
	{
		std::cout << *beg++ << ' ';
	}

	std::cout << '\n';
}

template<typename TBeg, typename TEnd>
void call_algo(TBeg beg, TEnd end)
{
	if constexpr(std::same_as<TBeg, TEnd>)
	{
		algo(beg, end);
	}
	else
	{	
		using iter_type = std::common_iterator<decltype(beg), decltype(end)>>; // common iterator türler aynı değilse onları aynı türden yapmak için 
											// common iterator türünden değer döndürecek.Kod tekrarını engellemk için using bildirimi yapıldı.
		algo(iter_type{beg}, iter_type{end});

		std::cout << '\n';

		//-----------------------------------------------------------------------------------

		// 2. yol
		auto v = std::views::common(std::ranges::subrange(beg,end)); //Common burada bize sentinel türü ile iterator türü aynı olan view döndürecek.
		
		algo(v.begin(), v.end()); //Buda geçerli.
	}
}

int main()
{
	list ilist{1,4,6,1,3,4,7,8,9};

	auto v = views::take(ilist,6);

	call_algo(v.begin(), v.end());
	
}

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------


SPLIT
Bir rangei split ediyor ve split edilen rangeleri dolaşma şansı veriyor.


int main()
{
	using namespace std;

	vector<int> ivec{6,87,5,4,32,43,456,45,4,657,768,324,4,4,9};

	auto spv = std::views::split(ivec,4); // 4 e göre ayıracak.

	for(const auto& split : spv) // dolaştığımız range teki öğelerde range
	{
		std::cout << "["; 
		std::ranges::copy(split,std::ostream_iterator<int>{cout," "}); // bu rangeleri ranges::copy ile cout a gönderiyorum.
		std::cout << "] ";
	}
}

ÇIKTI
-----
[6 87 5 ] [32 43 456 45 ] [657 768 324 ] [] [9 ]

SEPERATOR/DELIMITER OLARAK KULLANILAN ÖĞE BİR RANGE OLSUN

ÖR:
int main()
{
	
	using namespace std;

	vector<int> ivec{3,4,7,6,87,5,32,43,456,3,4,7,45,657,768,3,4,7,324,9};
	array sep{3,4,7};

	auto spv = std::views::split(ivec,sep);

	for(const auto& split : spv)
	{
		std::cout << "[";
		std::ranges::copy(split,std::ostream_iterator<int>{cout," "});
		std::cout << "] ";
	}
}

ÇIKTI
-----
[] [6 87 5 32 43 456 ] [45 657 768 ] [324 9 ]

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

ISTREAM VIEW - RANGE FACTORY
Bunlar range kendi üretiyor. IOTA yı gördük zaten

istream view bizden bir akım nesnesi alıyor ve o range i dolaşınca aslında giriş akımındaki
öğeleri dolaşmış oluyoruz.

int main()
{
	using namespace std;

	istringstream iss{"akif murat kadir harun erkan sercan"};

	//ranges::istream_view<string>(iss);

	for(const auto &s : ranges::istream_view<string>(iss))
	{
		std::cout << s << ' ';
	}

}

ÖR: DOSYAYI RANGE OLARAK KULLANALIM

int main()
{
	using namespace std;

	ofstream ofs{"necati.txt"};
	if(!ofs)
	{
		cerr << "dosya olusturulamadi\n";
	}

	Drand drand{0. , 9.99};
	for(int i = 0; i < 1000; ++i)
	{
		if(i && i % 10 == 0)
			ofs << "\n";
		ofs << format("{:.4f} ",drand());
	}

}


ÖR:

int main()
{
	using namespace std;

	ifstream ifs{"necati.txt"};
	if(!ifs)
	{
		cerr << "dosya olusturulamadi\n";
		return 1;
	}

	double sum{};
	
	ranges::for_each(ranges::istream_view<double>(ifs), [&sum](double d){ sum += d;})

	std::cout << "sum = " << sum << "\n";

}

MOLA

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

All
Elimizde bir range oluyor(view değil sadece range örneğin vector) fakat
biz bunu view olarak kullanmak istiyoruz. Containerlar bir view değil. 
Sadece lightweight olanlar view. Bunun içinde range adaptörü var.Adı all.
Bir range i all a argüman olaran geçince view::all bize o range ilişkin view 
döndürüyor.
 
template<std::ranges::view T>
void vw_print(T x)
{
	for(auto iter = std::ranges::begin(x); iter != std::ranges::end(x); ++iter)
	{
		std::cout << *iter << ' ';
	}
	std::cout << "\n\n";
}

int main()
{
	
	using namespace std;

	vector<string> svec;
	rfill(svec,100,rtown);
	
	char c;
	std::cout << "karakter girin: ";
	cin >> c;

	const auto pred = [c](const string &s){ return s.find(c) != string::npos;};

	vw_print(views::all(svec) | views::filter(pred));

}

==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================
==============================================================================================================


RANGES BİTTİ TASARIM KALIPLARI BAŞLAYACAK
DESIGN PATTERN

*/
