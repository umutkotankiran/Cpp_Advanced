/*


VIEWSTA KALMIŞTIK
Projection parametresi ile ilgili bir tekrar yapılacak ufak. Projection ile algoritma içinde belirli veya birden fazla noktada
algoritmanın gördüğü öğenin ne olacağını biliyoruz.Yani range deki öğenin kendisi mi yoksa algoritma o öğeyi kullandığı 1 veya
daha fazla yerde başka birşey mi görsün ? Bunu belirlemeye yarıyor.

ÇOK ÖNEMLİ ÖRNEK

#include<ranges>
#include<functional>

template<std::input_iterator Iter, std::sentinel_for<Iter> S, typename T, typename Proj = std::identity>
auto Find(Iter beg, S end, const T& val, Proj proj = {})	// S ile iter aynı tür oladabilir olmayadabilir.
{
	while(beg != end)
	{
		if(std::invoke(proj, *beg) == val) // invoke proj çağırıyor ve *beg'i argüman olarak gönderiyor.
		{				   // Data member ptr de gönderildiğidne yine çağrı yapsın diye invoke var.
						   // default argüman kullanırsak identity olacak proj. İdentity de *beg değerini verir

			return beg;
		}

		++beg;
	}

	return beg;
}

int main()
{
	using namespace std;

	vector ivec{10,5,6,400,4,6,20,9,2};

	if(auto iter = Find(ivec.begin(), ivec.end(), 400); iter != ivec.end())
	{
		cout << "bulundu " << *iter << '\n';
		cout << "index = " << distance(ivc.begin(), iter)<< '\n';
	}
	else
	{
		std::cout << "bulunamadi\n";
	}

	-----------------------------------------------------------------------------------------

	4. ARGÜMANA IDENTITY YERINE KENDI PARAMETREMI GEÇIYORUM

	vector ivec{10,5,6,400,4,6,20,9,2};

	if(auto iter = Find(ivec.begin(), ivec.end(), 400,[](int x){ return x * x; }); iter != ivec.end()) // Kendisi değil karesi 400 e eşit olanı bulacak
	{																									// bu değerde 20
		cout << "bulundu " << *iter << '\n';
		cout << "index = " << distance(ivc.begin(), iter)<< '\n';
	}
	else
	{
		std::cout << "bulunamadi\n";
	}

}


----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------

ÖNEMLİ NOKTALAR:
Bu yeni algoritmaların return değeri türleri her zaman eski algoritmalarla aynı değil.Bu çok büyük fayda sağlayan mekanizma için
oluşturulmuş.

Yeni algoritmalarda avantaj var. İterator çifti yerine doğrudan range conceptlerinden birini yada birden
fazlasını satisfied eden nesneyi geçebiliyoruz.

int main()
{
	auto vec = getvec(100,0,1000);
	print(vec);

	auto iter = ranges::min_element(vec);

	std::cout << "min = " << *iter << " iter = " << iter - vec.begin() << '\n';

	-----------------------------------------------------------------------------------------------------------

	ALGORITMAYA RVALUE EXPR GEÇERSEK !!!!!!!!!!!!!
	BU ESKİDEN OLMAYAN BİR PROBLEM !!!!!!!!!!!

	auto iter = ranges::min_element(getvec(10,0,100000)); // Sentaks hatası. getvec(10,0,100000) bu bir prvalue expr. getvec ifadesi çalıştıktan sonra
							      // Vectorun ömrü bitiyor. Dolayısı ile buradaki iterator bir dangling iterator olacak.

}

2 TANE SEÇENEĞI VAR LIBRARY TASARLAYANLARIN
1 - Range parametresi olan algoritmaya pr value expr ile çağrı yapmak doğrudan compile timeda hata olacak.Bunu tercih etmemişler
2 - iteratörü indirection operandı yapınca (*iter) sentaks hatası oluyor. Nasıl oldu bu ?

Compile time teknikleriyle func sağ taraf değeri aldığında return değeri özel bir return değeri.
Bu ranges::dangling'i denilen bir değer. vector<int>::iterator değil. Bunu compile timeda anlıyor.
Bu tür indirectionun operandı olunca sentaks hatası oluşuyor.

NEDEN BÖYLE TASARLAMIŞLAR?
Bunun nedeni algoritmaya range parametresine sağ taraf değeri geçtiğimiz zaman sentaks hatası olmasını istememişler.
Bunun cevabı C++20 ile gelen barrowed range isimli kavram. Range isteyen algoritmalar ya bir lvalue range geçmek zorunda
yada rvalue range geçilirse eğer return değeri iterator ise o iteratörün kullanılabilmesi için bunun barrowed range olması gerekiyor.

BARROWED RANGE : Bu rangelerin iteratörlerinin dangling olma ihtimali doğrudan mevcut değil.
Bir rvalue vector bir barrowed range değil.

Spani düşünelim

int main()
{
	auto vec = getvec(10,0,100000);

	auto iter = ranges::min_element(span(vec)); // span(vec)'te bir rvalue expr.iter burada dangling iterator değil.

	std:cout << "min = " << *iter; // GEÇERLİ.yukarıdaki iterator dangling değil. Dangling iterator olunca indirection yapınca sentaks hatası oluyor
}

BURADA NASIL KOD ÜRETILDI ?
Derleyici funca gönderilen türün bir rvalue expr olmasına rağmen bunun bir barrowed range olduğunu tür sistemindeki
araçları kullanarak anladı.

BARROWED RANGE IÇIN 2 IHTIMAL VAR
1- Lvalue range olacak
2- Variable template var. İsmi enable_barrow_range. O tür için explicit specialization yapılmış std library'de.
   Ve inline variable'ın değerini true yapacağız.


template<>
inline constexpr bool std::ranges::enable_borrowed_range<Myclass> = true;

Yani tür sistemine barrowed range olduğunu anlatmak için bu inline variable ın true değerde olması gerekiyor.

----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------

YANI BIR ALGORITMAYA BIR RANGE GÖNDERILMESI GEREKIYORSA LVALUE RANGE OLACAK YADA BARROWED RANGE NİTELİĞİNE SAHİP
OLMASI İÇİN AZ ÖNCEKİ GİBİ DEĞİŞKENİN DEĞERİNİN TRUE OLMASI GEREKİYOR

----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------

FARKLAR NELER?

1 - Function return değeri duruma göre compile timeda iterator yada ranges dangling türü olabiliyor.
Böylece dangling türünden değeri dereference ettiğimizde yada işlemlere soktuğumuzda sentaks hatası oluşuyor.
Böylece algoritmalara eğer uygunsa rvalue expr olan bir rangei geçebiliyoruz.Uygun değilse aldığımız iteratörü
dereference edince compile timeda hata oluşuyor

2 - Projectin parametresi

3 - Algoritmaların hepsinin constraint edilmesi

============================================================================================================================
============================================================================================================================
============================================================================================================================

ÖNEMLI EKSIKLIKLER VAR.

1- Numerics header file içinde numetric algoritmaların henüz ranges karşılıkları yok.Bunları constraint etme biraz zor olduğundan
   yapacak zaman kalmmamış. C++23 e kalmış

2- Algoritmalar paralel olarak çalıştırılabiliyor.Kendimiz doğrudan thread oluşturmadan algoritmanın 1. overloaduna özel bir
   değer geçerek algoritmanın paralel çalışmasını sağlayabiliriz. Bu özellik rangeste yok !!!!!
   C++23 te muhtemelen eklenecek

============================================================================================================================
============================================================================================================================
============================================================================================================================

ACCULUMATE ALGORİTMASI

STL 1.0 dan STL 2.0 a kadar getireceğiz :D:D
Bu algoritma bir range deki öğeleri kümülatif olarak işleme sokuyor. bu işlemde tipik olarak toplama.

int main()
{
	auto ivec = getvec(100,0,2000);
	print(ivec);

	auto val = accumulate(ivec.begin(), ivec.end(),0);  // üretilen 100 sayıyı topladı. initial value da 0.
							    // rangedeki öğeler toplanmasın ama şu işlem olsun dersek onuda algoritmanın diğer overloaduna
							    // 4. parametre olarak geçebiliriz. Defaultta toplama işlemi.
	std::cout << "val = " << val << '\n';
}

---------------------------------------------------------------------------------------------------------------------------

ESKİ HALİ AŞAĞIDAKİ GİBİ

template<typename Iter, typename Init>
Init Accumulate1(Iter beg, Iter end, Init init)
{
	while(beg != end)
	{
		init = std::move(init) + *beg;
		++beg;
	}

	return init;
}

---------------------------------------------------------------------------------------------------------------------------

TEMPLATE PARAMETRELERİ GELDİ.SENTINEL VE OPERATION

template<typename Iter,typename SenType, typename Init, typename Op = std::plus<>>
Init Accumulate2(Iter beg, SenType end, Init init, Op op = {})
{
	while(beg != end)
	{
		init = op(std::move(init), *beg);
		++beg;
	}

	return init;
}

-------------------------------------------------------------------------------------------------

CONCEPTLER İLE CONSTRAINT EDELİM

std::sentinel_for ifadesi bir range olabilip olamayacağını belirtiyor. 
beg != end ifadesi karşılaştırılabilir bir ifademi bunada bakıyor. Aynı zamanda bir concept.

template<typename Iter,typename SenType, typename Init, typename Op = std::plus<>>
requires std::input_iterator<Iter> && std::sentinel_for<Iter,SenType>
Init Accumulate3(Iter beg, SenType end, Init init, Op op = {})
{
	while(beg != end)
	{
		init = op(std::move(init), *beg);
		++beg;
	}

	return init;
}

DİKKAT!!!!!!!!!!!!!!!!!!
requires clause içerisindeki sentinel_for cepcentine bak. 2 tür parametresi var.
Bir sonraki örnekte template tür parametresi içerisinde olduğundaki durumu göreceğiz.

-------------------------------------------------------------------------------------------------

CONSTRAINT TEMPLATE PARAMETRESİ YAPALIM

template<std::input_iterator Iter, std::sentinel_for<Iter> SenType, typename Init, typename Op = std::plus<>>
Init Accumulate4(Iter beg, SenType end, Init init, Op op = {})
{
	while(beg != end)
	{
		init = op(std::move(init), *beg);
		++beg;
	}

	return init;
}

DİKKAT!!!!!!!!!!!!!!!
Burada sentinel_for conceptinin 2 parametresi var.
Burada şu olacak 
sentinel_for<Iter> Sentype ----> sentinel_for<Iter, Decltype<Sentype>> olacak !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


-------------------------------------------------------------------------------------------------

INVOKE GELDİ

template<std::input_iterator Iter, std::sentinel_for<Iter> SenType, typename Init, typename Op = std::plus<>>
Init Accumulate5(Iter beg, SenType end, Init init, Op op = {})
{
	while(beg != end)
	{
		init = std::invoke(op, std::move(init), *beg);
		++beg;
	}

	return init;
}


-------------------------------------------------------------------------------------------------

INITTE DEFAULT ARGÜMAN ALACAK

template<std::input_iterator Iter, std::sentinel_for<Iter> SenType, typename Init = std::iter_value_t<Iter>, typename Op = std::plus<>>
Init Accumulate6(Iter beg, SenType end, Init init = Init{}, Op op = {})
{
	while(beg != end)
	{
		init = std::invoke(op, std::move(init), *beg);
		++beg;
	}

	return init;
}

iter_value_t = bu hangi argümanı verirsek o iteratör konumundaki türü alıyor.
iter_value_t< vector<int>::iterator > burası int olacak

Init init = Init{} primitive türler için değer 0, sınıf türleri için default ctor çağrılacak.

-------------------------------------------------------------------------------------------------

PROJECTION PARAMETRESI EKLİYORUZ EN SONDA

template<std::input_iterator Iter, std::sentinel_for<Iter> SenType, typename Init = std::iter_value_t<Iter>, typename Op = std::plus<>,typename Proj = std::identity>
Init Accumulate7(Iter beg, SenType end, Init init = Init{}, Op op = {}, Proj proj = {})
{
	while(beg != end)
	{
		init = std::invoke(op, std::move(init), std::invoke(proj,*beg)); // Burasıda değişti !!!!!
		++beg;
	}

	return init;
}

-------------------------------------------------------------------------------------------------

Bunlar iteratör parametreli overloadlardı.Range parametreli overload var ve bu da yine iterator parametreli overloadu çağırıyor.

template<std::ranges::input_range R, class Init = std::ranges::range_value_t<R>, typename Op = std::plus<>, typename Proj = std::identity>
Init Accumulate(R &&r, Init init = Init{}, Op op = {}, Proj proj = {})
{
	return Accumulate(std::ranges::begin(r), std::ranges::end(r),std::move(init), std::move(op), std::move(proj));
}	// DİKKAT!!! Parametrelerde range olanlar var.

MOLA


========================================================================================================================
========================================================================================================================
========================================================================================================================
========================================================================================================================


ÖNEMLI ARAÇLARDAN BIRI SUBRANGE DENEN SINIF ŞABLONU LIGHTWEIGHT OLARAK BUNU KULLANIYORUZ.

using namespace std;

int main()
{
	auto ivec = getvec(20,0,100);

	ranges::subrange sr1{ ivec }; // Ctad dan faydalandık

	cout << "sizeof(sr1) = " << sizeof(sr1) << '\n'; // 8

	---------------------------------------------------------------

	ÜYE FUNCTIONLAR

	std::cout << sr1.size() << '\n'; // Rangedeki öğe sayısını verir

	sr1.data();
	sr1.front();

	sr1.advance(5);
	// rangei daraltmış olduk

	---------------------------------------------------------------

	DİKKAT!!!

	ranges::subrange sr{ivec.begin(), ivec.end()};

	for(auto x : sr)
	{
		std::cout << x << ' ';
	}

	---------------------------------------------------------------

	for(auto iter = sr.begin(); iter != sr.end(); ++iter)
	{
		//..
	}

	---------------------------------------------------------------

	ranges::for_each(sr,[](int x){ std::cout << x << " ";});

	---------------------------------------------------------------

	Subrange tuple like interface e sahip.

	auto [beg,end] = sr;

	next ve prev functionlarıda var.

}



Diyelimki 2 tane iterator var elimde. Bir algoritma yazdık ama bu algoritmada bu rangei
doğrudan range nesnesi olarak ifade etmek istiyoruz diyelim. Belki range parametreli algoritmaya
çağrı yapacağız. Bu durumda subrange oluşturabiliriz.

template<typename Iter>
void func(Iter beg, Iter end)
{
	subrange sr{beg, end};
}

Bazı range adaptörleri belirli türden argüman aldıklarında bir subrange döndürüyorlar.
Bazı durumlarda subrangei kendimiz oluşturacağız bazende kullandığımız function bize subrange verecek.

----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------


VIEW SINIFLARI

Hafifsiklet range ler.Geçen ders burada kaldık. Default ctor edilebilir destroy edilebilir.
copy maliyeti taşıma maliyeti düşük. Bunlar composable.ya type sentaksı ile compose ediyorum
yada bu sınıflar türünden nesneleri başka view sınıfları ctoruna argüman olarak gönderiyoruz.

Bunların olayı önemli özelliği bir davranışı encapsule edip dolaşma sırasında
lazy evaluation ile bir bunları dolaşırken önce range deki öğe birtakım işlemlere sokulabilir.


View Class
----------
std::ranges::filter_view    sınıf
std::ranges::view::filter   range adaptör
std::views::filter    views aynı zamanda namespace alias olduğundan buda geçerli

transform_view
take_view
take_while_view
drop_view
drop_while_view
reverse_view
join_view
split_view
common_view
keys_view
values_view

Range Factories (view factories)
--------------------------------
Bunların farkı, range'in sahibi durumundalar. Bir containera referans tutmuyorlar.Doğrudan sahibi.
iota_view
single_view
empty_view
basic_istream_view


Hepsi olmasada çoğunu döndüren range adaptörleri var.


----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------

Filter View
-----------
Bir predicate e bağlı olarak filtre edilmiş bir range döndürüyor.

int main()
{
	auto ivec = getvec(100,0,100);


	ranges::filter_view vw{ivec, [](int x){ return x % 5 == 0;}}; // sınıf bu

	auto vw = views::filter(ivec, [](int x){ return x % 5 == 0;}); // range adapter

	auto vw = ivec | views::filter([](int x){ return x % 5 == 0;}); // buda geçerli.
	// Burada herhangibir lambda ifadesi çağrılıyor dğeil. Bu ancak range dolaşıldığında
	// çağrılacak. Bu yüzden lazy evaluation deniyor.

	for(auto i : vw)
	{
		std::cout << i << " "; // 5 e bölünenler geldi bizde dolaştık
	}
}

----------------------------------------------------------------------------------------------------------

İstersek biren fazla view compose edilebilir
TRANSFORMDA VAR


int main()
{
	auto ivec = getvec(100,0,100);

	//Birden fazla view compose edilebilir.
	ranges::transform_view (ranges::filter_view{ivec, [](int x){ return x % 5 == 0}}, İKİNCİ_PARAMETRE);

	---------------------------------------------------------------------------------------------

	//Yada adaptörden aldığım değeri diğer adaptöre argüman olaran geçebiliyorum.
	views::transform(views::filter(ivec,[](int x){return x % 5 == 0;}), [](int x){ return x * x  - 1;});
}

----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------

TAKE
---------
Take range adaptörü range_view döndürüyor. Rangedeki öğelerden n tanesini almış oluyoruz.

int main()
{
	auto ivec = getvec(40,0,100);
	print(ivec);

	for(auto i : views::take(ivec,10))
	{
		std::cout << i << ' '; // ilk 10 taneyi aldı
	}
}

----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------

DROP
----
Take in tam tersi. Verdiği tamsayı kadar olanı range e dahil etmiyor.

int main()
{
	auto ivec = getvec(40,0,100);

	for(auto i : views::drop(ivec,10))
	{
		std::cout << i << ' ';
	}	// ilk 10 tanesini attı geriye kalanı aldı
}


----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------

TAKE_WHILE - DROP_WHILE
-----------------------
Yeni bir tamsayı alıyorlar ama bir predicate alıyorlar.
TAKE WHILE : Range üzerinde, predicate'ı sağlayan öğeye kadar range'i alıyor. sağlamayan olursa bundan sonrasını almıyor.
Yada predicateın sağlamadığı ilk öğeye kadar alıyor diğer deyişle.

TAKE WHILE ÖRNEĞI

int main()
{
	auto ivec = getvec(40,0,100);
	print(ivec);

	std::cout << "Degeri girin : ";
	cin >> ival;

	int ival;

	for( auto i : ivec | views::take_while([ival](int x){ return x > ival; })) // range i pipe ile bağladık
	{
		cout << i <<  ' ';  // 40 72 75 63 57 19 25 30 ve 23 girersek, SONUC : 40 72 75 63 57
	}
}


int main()
{

	list<string> slist;
	rfill(slist,20,rname);

	print(slist);

	size_t len;
	std::cout << "uzunluk değeri girin: ";

	cin >> len;

	auto v = views::take_while(slist,[](const string &s){return s.size() > len; })

	ranges::for_each(v,[](const auto &s){ std::cout << quoted(s) << " "; });

	// uzunluğu girineln değeri karşılayanları alacak ilk karşılamayandan sonrasını almayacak
}

----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------

LAZY EVALUATION
örnek üzerinde görelim.views are lazy

int main()
{
	auto odd = [](int){

		std::cout << "odd => x = " << x << '\n';
		return x % 2 != 0;
	};

	auto cube = [](int y){

		std::cout << "cube => y = " << y << '\n';
		return y * y * y;
	};

	auto vec = getvec(20,0,30);

	
	auto v = vec | views::filter(odd) | views::transform(cube); // filter(odd) 2 ye bölünmeyenler geldikçe çalışacak
								    // tek bir sayı gelince mesela 3, cube a 3 gönderilmiş
								    // ilk değer 27 oldu mesela
	
	// şu ana kadar ekrana birşey çıkmaz. Çünkü rangei dolaşmadık.
	// ne zaman range dolaşılırsa o zaman bu işlemler yapılır.
	// Bu yönteme programlamada pull metod deniyor.

	for(auto i : v )
	{
		std::cout << "i = " << i << "\n"; // artık ekrana çıktılar verilir.
	}
}

ÇIKTIYI YAZ

----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------

DROP_WHILE 
Predicatetaki koşulu sağlayanları ilk başta ard arda skip ediyor. Daha sonra predicate uymayan olanları yazarken
tekrar sağlayanlar çıkarsa bu sefer onlara müdahale etmiyor.

int main()
{

	list<string> slist;
	rfill(slist,20,rname);

	print(slist);

	size_t len;
	std::cout << "uzunluk değeri girin: ";

	cin >> len;

	auto v = views::drop_while(slist,[len](const string &s){return s.size() > len; })

	ranges::for_each(v,[](const auto &s){ std::cout << quoted(s) << " "; });

	// Uzunluk değerini girdikten sonra koşulu sağlayanları drop ediyor. Mesela 3 değerinden büyük isim uzunluğuna sahip olanları
	// drop edecek. Sonra 3 ten küçük veya eşit çıkarsa ondan sona kadar alacak.

}

----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------

REVERSE
Range'in tersinin kullanılmasını sağlıyor.

int main()
{
	list<string> slist;

	rfill(slist,10,rname);
	print(slist);

	for(auto x : views::reverse(slist) | views::take(5))
	{
		std::cout << x << ' ';
	}
	
	//auto vw = ivec | views::reverse;  // böylede yazılabilirdi. Sonra () yok dikkat.Yukarıdakinde vardı

}

----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------

RANGE FACTORIES (VIEW FACTORIES)

Kendileri range oluşturuyorlar. En sık kullanılanı iota.Iota aynı zamanda modern c++ ile gelen algoritmanında ismi

ranges::iota_view  sınıf
view::iota	range adapter buda

int main()
{
	auto v = views::iota(10); // Bu rangein dışarı bağımlığı yok değeri kendisi tutuyor. İlk defa bu range i dolaştırğımda
				  // bana ilk öğe olarak 10 değeri verilecek. Sonrasında ++ operatorü kullanılıp birsonraki değer
				  // verilecek. Range şaunda infinite durumda. Sonlandırılmamış.


	//İstersek Ctora ikincibir argüman gönderebiliriz.
	auto v = views::iota(10,20);

	for(auto x : v)
	{
		std::cout << x << " "; // 10 dan başlayıp en son 20 ye geldiğimde range tamamlanmış olacak.
	}

	----------------------------------------------------------------------------------------------------

	auto v = views::iota(10);

	for(auto x : v | views::take(25))
		std::cout << x << ' '; // Burada da ilk 25 öğe alınır.

	----------------------------------------------------------------------------------------------------

	for(auto x : views::iota(10) | views::transform([](int x){ return x + 10;}) | views::take(20)
		std::cout << x << ' '; // 20 21 22 .... 39 40

}
 
----------------------------------------------------------------------------------------------------------

// (			murat			)
// trimden sonra
// (murat)
std::string trim(std::string s)
{
	auto v = s | views::drop_while(std::isspace)		// boşluk karakterlerini range dahil etmeyuecek
                   | views::reverse				// ters çevirdik ki sonradki boşluklarıda silebilelim 
		   | views::drop_while(std::isspace)		// yine başa hgelen boşluklar silindi. Şuanda yazıda ters
		   | views::reverse;				// ters olan yazıyı düzelttik.


	return std::string(v.begin(), v.end());	

	// return {v.begin(), v.end()}; buda olurdu
}

int main()
{
	using namespace std;

	string str{"  \t\nmustafa aksoy\t	\n"};

	cout << "|" << trim(str) << "|";
}

----------------------------------------------------------------------------------------------------------

IOTA
Bir range'i sıralı artan değerlerde dolduruyor.

int main()
{
	for(const auto &d : views::iota(Date{25,12,2021},Date{5,4,2022}))
	{
		std::cout << d << '\n';
	}
	
	---------------------------------------------------------------------------------------

	for(const auto &d : views::iota(Date{25,12,2021},Date{5,4,2022})) | 
		views::transform([](const Date& date){ return date + 5; })
	{
		std::cout << d << '\n';
	}
}

MOLA

================================================================================================================================
================================================================================================================================
================================================================================================================================
================================================================================================================================

Sentinel kullanımı ile birlikte, yani iteratorden farklı bir tür olarak kullanılabilmesiyle ve bazı view sınıflarında
zaten iterator türüyle sentinel türü farklı, şöyle bir ihtiyacı doğuruyor.

Elimizde bir range var ve bu rangei örneğin,
	a - Container ctoruna argüman olarak göndermek istiyoruz.STL in önemli özelliklerinden biri range cotura sahip olması.
		bu sayaede mesela list ile vectoru hayata getirebiliriz.
	b - Yada Elimizde bir range var bunu classic bir algoritmaya göndereceğiz. Range library deki algoritmalardan biri değil
		eskisine göndereceğiz.

BUrada ne yapacağız?
Elimde iterator ve sentinel türleri farklı olan rangeler var. Fakat benim istediğim aynı olan iterator türü ile sentinel türü
aynı olan bir range kullanmak.Çünkü kullanacağım kod böyle bir beklenti içinde. Bİr containerın ctoru gibi .

Burada ihtiyaç common_range. Adaptör ismi ise common. 

-------------------------------------------------------------------------------------------------------------------------------

COMMON RANGE
------------
Common range aynı zamanda concept ismi. Bu conceptin satisfied etmesi için rangein iterator türü ile sentinel türü aynı tür olmalı.

int main()
{
	list mylist{1,2,3,4,5,6,7,8,9};

	auto v = views::take(mylist, 5); // v de range mylistte range.mylist doğrudan bir container. v aynı zamanda bir view
					 // bütün viewlarda range idi zaten. Bir derste yazmıştık.
					 // mylist common range conceptini satisfied edecek çünkü onun begin ve endi aynı türden

	constexpr auto b = ranges::common_range<decltype(mylist)>; // compile timeda b true.
	same_as<decltype(mylist.begin()), decltype(mylist.end())>; // True verecek.
	
	constexpr auto b = ranges::common_range<decltype(v)>; // compile timeda b false oldu.
	same_as<decltype(v.begin()), decltype(v.end())>; // false verecek.

	-------------------------------------------------------------------------------------------------------
	
	SORU : Elimizde bir view var yukarıdaki gibi. Biz bunu kullanarak bir vector oluşturmak istesek
	
	auto v = views::take(mylist, 5);

	vector<int> ivec{v.begin(), v.end()} ; //SENTAKS HATASI begin ve end aynı değil ama ctor aynı türde iter ekliyor

	-------------------------------------------------------------------------------------------------------

	ÖYLE BİR ARACA İHTİYAÇ VARKİ YİNE AYNI RANGE KULLANACAK AMA İTERATORLERİ AYNI TÜRDEN OLACAK.BUNUN YOLU COMMON ADAPTÖR

	auto v = views::take(mylist, 5);
	
	auto v2 = views::common(v); // BURADA V2 NIN BEGINI ILE ENDININ RETURN DEĞERI TÜRÜ AYNI OLACAK ARTIK !!!!!!!!!!!!!!!!

	vector<int> ivec{v2.begin(), v2.end()}; // Geçerli

	-------------------------------------------------------------------------------------------------------

	auto v = views::take(mylist, 5);

	auto iter = std::find(v.begin(), v.end(),3); //SENTAKS HATASI. AYRI TÜR İTER DEĞİL

	-------------------------------------------------------------------------------------------------------

	auto v = views::take(mylist, 5) | views::common; //DİKKAT!!!

	auto iter = std::find(v.begin(), v.end(),3); //GEÇERLİ !!!!!!!!!!!!
	

}

SINIFIN ADI : ranges::common_view
ADAPTOR ADI : views::common

-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------

COUNTED VIEW
------------
Elimizde bir iterator var ve range olarak o iterator konumundan başlayarak belirli sayıda öğenin olduğu range/view 
kullanmak istiyoruz. Bu işi yapıyor.

int main()
{
	auto ivec = getvec(10,0,1000);
	print(ivec);

	auto iter = ivec.begin();

	auto v = views::counted(iter, 4);
	
	for(auto val : v)
	{
		std::cout << val << " "; // ilk 4 öğeyi dolaşmış oldum.
	}

}

-------------------------------------------------------------------------------------------------------------------------------

int main()
{
	auto ivec = getvec(10,0,1000);
	print(ivec);

	for(counted_iterator iter{ ivec.begin(), 5}; iter.count() > 0; ++iter) 
	{
		std::cout << *iter << " ";  // 5 taneyi dolaştı.
	}
}

-------------------------------------------------------------------------------------------------------------------------------

2. YOL
Eğer sentinel kullanacaksak karşılaştırma olarak default sentinel isimli türü kullanıyoruz.

int main()
{
	auto ivec = getvec(10,0,1000);
	print(ivec);

	for(counted_iterator iter{ivec.begin(), 5}; iter != default_sentinel; ++iter)
		std::cout << *iter << ' '; // ilk 5 tanesini yazar.
}

-------------------------------------------------------------------------------------------------------------------------------

IOTA ÖRNEK

int main()
{
	auto v = views::iota(10) | 
			 views::take(20) | 
			 views::reverse  | 
			 views::transform([](int x){ return x * 10;}) | 
			 views::common;

	vector<int> ivec{v.begin(), v.end()};

	print(ivec);
}
*/
