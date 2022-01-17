/*

Sentinel Kavramını inceliyorduk.
Daha önceden STL de range'i 2 iterator olarak gönderiyorduk algoritmaya.
Daha önceden algoritmaların range parametreleri aynı türdendi. begin ve end aynı iterator türünden olmalıydı.
Rangeste en büyük değişikliklerden biri artık yeni algoritmalar constraint ile kalmıyor ve sentinel kavramını
desteklemek için range in başı olan iterator farklı sonu belirleyen iteratör farklı olabilir.
Aynı türden olmak zorunda değil.

Sentinel olarak kullanılabilecek sabit bir değeri bir template olarak belirleme imkanı getiriyoruz.

template<auto EndVal> // auto burada tamsayı reel sayı veya literal type olabilir.
struct EndValue  // sentinel olarak kullanılacak sınıf template
{
	constexpr bool operator==(auto pos)const // C++20 de operator!= derleyici tarafından yazılmış oldu artık
	{
		return *pos == EndVal; // Ne zaman endval bir başka ifade ile karşılaştırıldığında aslında karşılaştırma
				       // *pos ifadesi ile template nontype argüman ile(Endval) ile karşılaştırılacak
	}
};

int main()
{
	using namespace std;

	vector ivec{12,5,218,9,3,24,-1,9,41,6}; // 

	ranges::for_each(ivec.begin(), EndValue<-1>{}, [](int x){ std::cout << x << ' ';} );
	// Döngü içinde sentinel değeri ile karşılaştırma var. karşılaştırmada öğelerden biri 
	// endvalue türünden olduğundan karşılaştırma işlemini içindeki operator== yapacak.
	// buda iteratörü dereference edip aslında -1 karşılaştırmasını yapacak.
	// -1 e kadar olan değerler yazıldı.
}

Burada amaç daha çok performans değil range i bir değere bağlamak. Sentinel değerine bağlamak yani.
Duruma göre performans açısından fayda sağlayabilir.

ÇIKTI
-----
12 5 218 9 3 24

---------------------------------------------------------------------------------------------------------

int main()
{
	using namespace std;

	vector ivec{12,5,218,9,3,24,-1,9,41,6}; //

	ranges::sort(ivec.begin(), EndValue<-1>{}); // default pred std::less. -1 e kadar yapacak sıralamayı

	print(ivec);

}

ÇIKTI
-----
3 5 9 12 24 218 -1 9 41 6

---------------------------------------------------------------------------------------------------------

int main()
{
	using namespace std;

	vector ivec{12,5,218,9,3,24,-1,9,41,6}; //

	auto iter = ranges::find(ivec.begin(), EndValue<-1>{},9);

	std::cout << distance(ivec.begin(), iter) << '\n'; // indexi verir. 3 

	print(ivec);
}

---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

ALGORITMALARDA OLAN YENILIKLER

1- Algoritmalara 2 iteratör yerine, container nesnesinin kendisini geçebiliyoruz. Contraint ler kontrol
ediliyor compile timeda.Dolayısıyla yanlış kod yazmanın sentaks hatası oluşturması riski çok fazla
Hata olunca hata mesajları çok daha açık ve net

2- İteratorler aynı türden olmak zorunda değil.

3- Projection kavramı. Bu kavrama bakıyoruz şimdi.

---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

PROJECTION
----------
Algoritmalar normalde iteratör konumundaki öğeyi kullanıyor. Şöyle bir mekanizma olduğunu düşünelim.
İşleme sokulan iteratör konumundaki nesne olmasın ama ona bağlı başka bir değer olabilsin.
Ör: iterator konumundaki nesnenin bir elemanı olabilsin.

Standart kütüphanede identity diye bir class var. Bu bir metaclass
Bu öyle bir sınıfki(C++20 ile geldi) identity türünden nesnenin func call operator func çağırınca
nesnenin kendisini döndürüyor.

Find_if algoritmasını incelediğimizde templatein bir parametresi daha var. Default argüman olarak 
std::identity'yi almış. Eğer bu template parametresine karşılık argüman belirlemezsek default argüman
kulanırsak, std::identity kullanmış oluruz.

Functionun Proj isimli parametresi var.En sonda görülüyor. 4. parametre default argüman olarak 
default init edilmiş bir projection nesnesi alıyor. Yani eğer bu funca argüman göndermezsek,
bu function identity türünden bir nesneyi alıyor argüman olarak.Aşağıda bildirim

template< std::input_iterator I, std::sentinel_for<I> S, class T, class Proj = std::identity >
requires std::indirect_binary_predicate<ranges::equal_to, std::projected<I, Proj>,const T*>
constexpr I find( I first, S last, const T& value, Proj proj = {} );

PROJ NE YAPIYOR?
----------------
İmplementasyona bakınca şu görülüyor, normalde find iteratör komnumundaki nesne value ya eşitse aradığımı buldum demekti.
Bunun yerine std::invoke ile func çağırma işi yapılmış. 1. param func object, 2. parametresine *first.
Eğer proj identity türündense gönderilen argümanın kendisini döndürecek. Dolayısı ile 
std::invoke(proj, *first) ==>> *first  olacak. Derleyici bunu compile timeda görecek.Inline olarak açılacak
ve func call da yapılmayacak.

proj identity türünden değilde bizim verdiğimiz türden bir func object ise o zaman burada çağrılacak function *firstü değil
*first ün proja argüman olarak gönderilmesiyle elde edilen return değerini kullanacak. Böylece iteratör
konumundaki nesneyi değilde onun dönüşmüş bir halini kullanabileceğiz.

Eğer algoritmanın iteratör konumundaki nesnenin kendisini kullanmak istersek son parametreye hiçbirşey geçmeyeceğiz.
Böylece identity kullaınlacak ve yapılan işlemlerde ne zaman iteratör konumundaki nesne kullanılsa o nesnenin
kendisini kullanmış olacağız.

---------------------------------------------------------------------------------------------------------

ÖR:

int main()
{
	using namespace std;

	vector ivec{12, 5, 218, 9, 3, 24, -1, -9, -41, 6};

	ranges::sort(ivec); // 2. parametre less 3. parametre identity

	print(ivec); // küçükten büyüğe sıralandı.

	---------------------------------------------------------------------------------------------
	
	PROJECTION KULANALIM !!!!!!!!!!!!!!!
	
	ranges::sort(ivec, {}, 
	[](int x){ return std::abs(x); }); // {} demek burada default construct edilmiş std::less{} demek	
					   // Ne zaman iteratör konumundaki nesne kullanılmak istense o zaman(invoke'u hatırla yukarıdan)
					   // o iteratör konumundaki nesnenin mutlak değer funcına gönderilmesinden
					   // elde edilen değer. Dolayısıyla karşılaştırma işlemleri nesnenin kendi
					   // üstünden değil mutlak değerleri üzerinden yapılacak

	
	print(ivec); // küçükten büyüğe sıralandı.

	//ÇIKTILARI YAZ.
}

---------------------------------------------------------------------------------------------------------

ÖR:

int main()
{
	vector<string> svec;
	rfill(svec,100,rname);

	ranges::sort(svec, {}, [](const string &s){ return s[1]};); // algoritma iteratördeki nesneyi değil 
								    // onun 1 indisli elemanını kullanacak.Minimum 2 char olmalı :D
}

---------------------------------------------------------------------------------------------------------

int main()
{
	vector<string> svec;

	rfill(svec, 20, rname());

	ranges::sort(svec,{}, [](const auto&s){ return accumulate(s.begin(), s.end(),0); }); 
	// yazının karakterlerinin kod nosunu toplayıp onu karşılaştıracak
	// sıralamada karakter kodlarının toplamı en düşükten en yükseğe doğru bir sıralama olacak																					

	print(svec,'\n');
}

---------------------------------------------------------------------------------------------------------

int main()
{
	vector<bitset<32>> bvec;
	mt19937 eng(random_device{}());

	for(int i{}; i < 50'000; ++i)
	{
		bvec.emplace_back(eng());
	}

	ranges::sort(bvec,{},[](const bitset<32> &bs){ return bs.count();}); 
	
	// ALGORITMADA 2 ITERATOR KONUMUNDAKI NESNE ARGÜMAN OLARAK GÖNDERILIYORDU.
	// ŞIMDI ONLAR LESS E ARGÜMAN OLARAK GÖNDERILMEDEN ÖNCE LAMBDA IFADESINE GÖNDERILECEKLER
	// BU CALLABLE IN RETURN DEĞERI LESS E ARGÜMAN OLARAK GÖNDERILECEK
	// SIRALAMA IÇINDEKI 1 SAYISI AZ OLANDAN ÇOK OLANA DOĞRU SIRALANDI

	ofstream ofs{"out.txt"};
	
}

---------------------------------------------------------------------------------------------------------

int main()
{
	vector<string> svec;

	ranges::find(svec,7u,[](const string &s){ return s.length(); }); // karşılaştırılan stringin kendisi değil,
									// stringin uzunluğu. uzunluğu 7 olan ilkini bulmuş olacak

}

---------------------------------------------------------------------------------------------------------

Date kullanılacak

int main()
{
	vector<Date> dvec;

	rfill(dvec, 100, Date::random);

	ranges::sort(dvec,{},[](const Date &d){ return d.month_day(); }); // Ayın gününe göre sıralayacak
	ranges::sort(dvec,{},[](const Date &d){ return d.month(); }); // Aya göre sıralar
	ranges::sort(dvec,{},[](const Date &d){ return d.week_day(); }); // Haftaya göre sıralar

	print(dvec,"\n");
}

---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

ÇOK ÖNEMLİ BİR ÖRNEK

struct Employee
{
	std::string name;
	int id;
};

struct Payslip
{
	std::string pay_info;
	int id;
};

using namespace std;

int main()
{
	vector<Employee> evec(100);
	vector<Payslip> pvec(100);

	sort(evec.begin(), evec.end(),[](const Employee &e1, const Employee &e2){ return e1.id < e2.id; });
	
	sort(pvec.begin(), pvec.end(),[](const Payslip &e1, const Payslip &e2){ return e1.id < e2.id; });

	auto b = equal(evec.begin(), evec.end(),pvec.begin(), pvec.end(), [](const Employee &e,const Payslip &p){   // son parametre normalde default olarak equal_to
		return e.id == p.id;
	}); 

	---------------------------------------------------------------------------------------------------------

	BUNU C++20 ILE YAZACAĞIZ

	vector<Employee> evec(100);
	vector<Payslip> pvec(100);

	ranges::sort(evec,[](const Employee &e1, const Employee &e2){ return e1.id < e2.id; }); // LAMBDA İFADESİ BURADA KARŞILAŞTIRMA İŞİNİ YAPAN CALLABLE !!!!!
	
	ranges::sort(pvec,[](const Payslip &e1, const Payslip &e2){ return e1.id < e2.id; });

	auto b = ranges::equal(evec,pvec, [](const Employee &e,const Payslip &p){  //Burada karşılaştırmak için doğrudan callable yazdık. Bu Projection değil.
		return e.id == p.id;													// projection bir sonraki örnek.
	}); 

	---------------------------------------------------------------------------------------------------------

	BU SEFERDE PROJECTION KULLANALIM

	vector<Employee> evec(100);
	vector<Payslip> pvec(100);

	ranges::sort(evec,ranges::less{}, [](const Employee &e){ return e.id; }); // KARŞILAŞTIRMA İŞİNİ ARTIK LESS YAPIYOR. PROJECTION ISE LAMBDA OLDU ARTIK. !!!!!
	
	ranges::sort(pvec,ranges::less{}, [](const Payslip &e){ return e.id; });

	
	//DİKKAT !!!!
	// 2 VECTOR İÇİNDE 2 AYRI PROJECTION PARAMETRE YAZDIK
	auto b = ranges::equal(evec,pvec, std::ranges::equal_to{}, [](const Employee &e){ return e.id; }, [](const Payslip &p){ return p.id; }); 

	---------------------------------------------------------------------------------------------------------
	
	BİRAZ DAHA SADELEŞTİRELİM

	vector<Employee> evec(100);
	vector<Payslip> pvec(100);

	ranges::sort(evec, {}, [](const Employee &e){ return e.id; }); // {} yazdık less yerine

	ranges::sort(pvec, {}, [](const Payslip &e){ return e.id; });

	auto b = ranges::equal(evec,pvec, {}, [](const Employee &e){ return e.id; }, [](const Payslip &p){ return p.id; }); // less yerine {} yazdık.

	---------------------------------------------------------------------------------------------------------
	
	Arka planda func çağrılması yerine invoke kullanılmasının bir nedeni daha var.
	Template teknikleriyle bir callable geçmek yerine biz doğrudan sınıfın veri elemanının adresini geçebiliriz.
	Bu durumda arkadaki template kod callable ı kendi oluşturuyor.Bir önceki kodla farkı yok aslında.
	Arka planda invoke çağrısı var yine.

	ranges::sort(evec,{}, &Employee::id); // veri elemanı public ise adresini gönderebiliriz böyle
	
	ranges::sort(pvec,{}, &Payslip::id);

	auto b = ranges::equal(evec,pvec, {}, &Employee::id, &Payslip::id); 

}

---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

ÖR: Sıralama kodu

struct Employee{
	Employee() = default;
	Employee(std::string name, std::string surname, int id) : mname{std::move(name)}, msurname{std::move(surname)} { } 
	
	auto operator<=>(const Employee&)const =  default;
	
	std::string mname;  // Buradaki bildirimler ne sırada ise spaceship o sırada karşılaştırma kodu üretir
	std::string msurname;	// surname üstte olsaydı surname e göre sıralanacaktı
	int mid{};
};

int main()
{
	using namespace std;

	vector<Employee> evec;

	for(int i = 0; i < 1000; ++i)
	{
		evec.emplace_back(rname(), rfname(), Irand{0,100000}());
	}

	sort(evec.begin(), evec.end()); // Operator< func olmasaydı sentaks hatası ama <=> sayesinde < de yazıldı

	------------------------------------------------------------------------------------------------------

	//RANGES::SORT KULLANALIM

	ranges::sort(evec, {}, &Employee::m_name); // Sıralama iste göre yapılır.Surname yazsaydık surname e göre yapılırdı.

	------------------------------------------------------------------------------------------------------

	// KARŞILAŞTIRMA KRITERI KIŞININ ADI VE SOYADININ TOPLAMI OLAN YAZI ÜSTÜNDEN YAPILDI.

	ranges::sort(evec,{}, [](const Employee &e){ return e.mname + e.msurname;});

	------------------------------------------------------------------------------------------------------
	
	//Yazdırdık
	for(const auto &e : evec)
	{
		cout << format("{:<12} {:<16} {:<16}",e.mid, e.mname, e.msurname);
	}

}

MOLA

==================================================================================================================================
==================================================================================================================================
==================================================================================================================================
==================================================================================================================================

INSERTER İLAVE EDELİM

struct Employee{
	Employee() = default;
	Employee(std::string name, std::string surname, int id) : mname{std::move(name)}, msurname{std::move(surname)} { }

	auto operator<=>(const Employee&)const =  default;

	friend std::ostream& operator<<(std::ostream&os, const Employee& e)  //Klasik inserter ekledik
	{
		return os << '{' << e.mid << ", " << e.mname << ", " << e.msurname << '}';   
	}

	std::string mname;  // Buradaki bildirimler ne sırada ise spaceship o sırada karşılaştırma kodu üretir
	std::string msurname;	// surname üstte olsaydı surname e göre sıralanacaktı
	int mid{};
};


//Func şablonu yazacağız.Projectionu kendisi kullanacak. Sözde bir range'i yazdıracak

template<std:ranges::input_range R, typename Projection = std::idnetity>	// std:ranges::input_range bu bir concept
void Print(R &&r, Projection proj = {})		// default init edilmiş projection nesnesi 2. param
{
	std::ranges::for_each(r, [](const auto &e){std:: << e << '\n';), proj);
	std::cout << '\n';
}


int main()
{
	using namespace std;

	vector<Employee> evec;

	for(int i = 0; i < 100; ++i)
	{
		evec.emplace_back(rname(), rfname(), Irand{0,100000}());
	}

	Print(evec);	

	Print(evec,&Employee::mid);	// employee lerin id sini yazdırmış olacağız

	Print(evec,&Employee::mname);	// employee lerin isimlerini yazdırmış olacağız.

	Print(evec,[](const Employee &e){ return to_string(e.mid) + '_' + e.mname + '_' + e.surname; });

}

=======================================================================================================================================================================
=======================================================================================================================================================================
=======================================================================================================================================================================
=======================================================================================================================================================================
=======================================================================================================================================================================

VIEW
----
Kendiside bir concept aslında.Range in niteliklerini sağlayan herhangibir tür. Bir view kendiside bir range olarak kullanılabilir ama her range bir view
olmak zorunda değil.View aslında varlık sebebi, lazy evaluationa olanak sağlayan range'lerdir. Bunlar hafifsiklet sınıflar. Sizeof değerleride küçük anlamında.
Kopyalanmasında bir maliyet içermeyen sınıflar. 

Destruction ve move operasyonları varsa copy operasyonları O(1) karmaşıklıkta.Fakat ben bir range'e bir view aldığımda, view nesnesinin kendisinin kopyalanması
O(1) karmaşıklıkta yani öğeler dolaşılmıyor.Geri planda kopyalananlar iterator veya pointerlar.Öyle bir sınıf düşünelimki ranbge gereken yerde range olarak kullnabiliyorum.
Örneğin sınıfın end ve begin funcları var, sınıfın iteratörünü artırabilir.Algoritmaya range geçmem gerekiyorsa ona range yerine bir view nesneside geçebilirim.

En önemli faydaları farklı işlem adımlarının birleştirilmesini/compose edilmesini sağlayabiliyorlar.Lazy evaluation. 
Yani farklı işlem adımlarının önceden belirlenip ama sonradan range dolaşılırken bu işlemin yapılmasına olanak sağlayacak
yeni bir paradigma.Functional ve declerative programlamaya yaklaşılıyor burada.

non owning ranges, light-weight ranges. Bunlar popüler tanımlamalar


View aslında sınıf.

ranges::take_view
ranges::filter_view
ranges::take_while
ranges::iota
....  Hepsi detaylı incelenecek

View sınıfları yani range kullandığım yerde kullanabileceğim view sınıfları doğrudan ranges namespace i içinde.

int main()
{
	auto vec = getvec(20,0,1000); // getvice hoca yazmış sanırım
	print(ivec);
	
	std::ranges::take_view v{vec,5}; // Normalde bunlar sınıf template.Ctad dan faydalalnıyoruz burada
					 // oluşturulan nesnenin türü std::ranges::take_view değil. bu sınıf template in ismi
					 // Burada Ctad dan faydalanıldı.
	
	std::cout << typeid(v).name() << '\n'; // uzun bir açılımı var.

	-----------------------------------------------------------------------------------------------------

	std::ranges::reverse_view
	std::ranges::iota
	std::ranges::common_view

}

==============================================================================================================================================================
==============================================================================================================================================================
==============================================================================================================================================================
==============================================================================================================================================================


VIEW SINIFI TÜRÜNDEN BIR NESNE ELDE ETMENIN BIR YOLU DAHA VAR.ÇOĞU ZAMANDA BU YOL KULLANILIYOR.
DOĞRUDAN SINIFIN CTORU KULLANMAK YERINE, RANGES NAMESPACE IÇINDEKI VIEWS NAMESPACEI IÇINDE FUNCTION OBJECTLER VAR
BU FUNC OBJECTLERE ÇAĞRI YAPINCA BUNLAR BIZE DOĞRUDAN RETURN DEĞERLERIYLE BIR VIEW NESNESI VERIYORLAR.

int main()
{
	auto ivec = getvec(20,0,1000);
	print(ivec);

	std::ranges::take_view x; // BÖYLE YAZMAK YERİNE AŞAĞIDAKİ TERCİH EDİLİYOR

	std::ranges::views::take(vec, 6); // BU FUNCA ÇAĞRI YAPITORUZ. Take bir func object.Dolayısıyla burada çağrılan take'in func call operator func.

	//ranges::views ın bir aliası var

	std::views::take // gibi de yazıalbilir
	
}

BURADAKI FUNCTION OBJECT SINIFLARINA RANGE ADAPTORS DENIYOR.YANI VIEWLAR ASLINDA HEMEN HEPSI SINIF ŞABLONU AMA RANGE ADAPTÖRLERI
BIR FUNCTION OBJECT. BİR RANGE ADAPTERÖNE ÇAĞRI YAPINCA O RETURN DEĞERİ OLARAK VIEW VERİYOR.DOLAYISI İLE VIEW KULLANMAM GEREKEN
YERDE VIEW SINIF TURDEN ESNNEYI KENDİM OLUŞTURABİLİRİM. BUNU YAPARSAM CTADDAN DA FAYDALANMAK GEREKECEK AMA DAHA İYİSİ O VIEWI VEREN
RANGE ADAPTER VARSA ONA ÇAĞRI YAPIYORUZ.


EN SIK KULLANACAKLARIMIZ
filter_view
transform_view



FILTER VIEW
-----------

Bir filter view nesnesi oluşturursam

int main()
{
	auto ivec = getvec(20,0,1000);
	print(ivec);

	ranges::filter_view v(ivec,[](int x){ return x % 5 == 0;}); // 1. parametre range veya viewın kendisini gönderyotruz.
								    // Range aldıkları için ve view da range olduğu için viewın ctoruna yine view geçilebilir.
								    // 2. parametre predicate


	// Yukarıdakini yazmanın daha rahat yolu
	// İsimlendirme şöyle, view sınıfının isimlerinin sonunda view var. mesela ranges::filter_view
	// range adapter olan func objectlerin isminde sondaki viewı atıyoruz. views::filter oldu
	// ÖR: 
	// 		 VIEWS CLASS			RANGE ADAPTERS
	//		 ---------------------------------------------
	//  		 ranges::take_view		views::take
	//		 ranges::iota_view		views::iota
	//		 ranges::drop_view		views::drop


	auto v = views::filter(ivec,[](int x){ 
	std::cout << "called\n";
	return x % 5 == 0; });				// YUKARIDAKİ GİBİDE OLUR BU ŞEKİLDE DE OLUR
							// ŞUANDA BİR FUNC ÇAĞRISI YOK AMA NE ZAMANKİ VIEW IN OLUŞTURDUĞU RANGEİ
							// DOLAŞACAĞIZ BUNU BİR ALGORİTMAYA GÖNMDEREREK YAPABİLİRİM YADA RANGE BASE FOR LOOP
							// İLE YAPABİLİRİZ, O ZAMAN VIEW NESNESİ BANA 5 E TAM BÖLÜNMEN NESNELERİ VERECEK


	for(auto i : ivec)
		cout << i << " "; // tüm öğeler
	
	for(auto i : v)
		cout << i << " "; // 5 e tam bölünenler gelecek. view::filter funcı biz bu rangei dolaştığımızda çağrılıyor.Lazy Evaluation bu.

}


-------------------------------------------------------------------------------------------------------------------------------

1. ÖZELLİK LAZY EVALUATION

HERHANGİBİR ALGORİTMADA BU RANGEİ KULLANABİLİRİZ

int main()
{
	auto ivec = getvec(20,0,1000);
	print(ivec);

	auto v = views::filter(ivec,[](int x){
	std::cout << "called\n";
	return x % 5 == 0; });

	std::ranges::for_each(v.begin(), v.end(), [](int x){ cout << x << " ";});

	//1. özelliğini gördük. Lazy evaluation
	
}

-------------------------------------------------------------------------------------------------------------------------------

2. ÖZELLİK, COMPOSE EDEBİLİRİZ.

views::filter func çağrısı bize bir view verir. O zaman range adaptörü bizden range istiyor. Viewda bir range olduğuna göre filterın
döndürdüğü range i başka bir range adaptörüne operand olarak gönderebiliriz.

int main()
{
	auto ivec = getvec(20,0,1000);
	print(ivec);

	auto v = views::take(views::filter(ivec,[](int x){ return x % 5 == 0;}), 5); // Bu rangei dolaşınca önce 5 e bölünenlere bakacak,
										     // bunlardan da ilk 5 tanesini alacak.

}

-------------------------------------------------------------------------------------------------------------------------------

ÖRNEK

Öyle bir range oluşturuyorum ki bu range oluşturulduğunda 5 e tam bölünenler dolaşılmış olacak ama onların karesini alacağız
Onların içinden de ilk 10 tanesini dolaşacağız. Tüm bunlar için tek bir view oluşturabiliriz. Bunun için farklı yollar var.

Önce kötü yol 

int main()
{
	auto ivec = getvec(100,0,1000);
	print(ivec);


	//ranges::filter_view(ivec, [](int x){return x % 5 == 0;}); // Temporary filter view object.Bu ifade bir sınıf.Rvalue expr.
								    // aşağıdakine argüman olarak gitti.transform da take e argüman olarak gitti.
	
	ranges::take_view v { 
	ranges::transform_view{ ranges::filter_view(ivec, [](int x) {return x % 5 == 0; }), [](int x) { return x * x; }}, 5};

	for(auto i : v)
	{
		std::cout << i << ' '; 
	}

}

-------------------------------------------------------------------------------------------------------------------------------

BUNU YAZMANIN DAHA KOLAY YOLU RANGE ADAPTER KULLANMAK

int main()
{
	auto ivec = getvec(100,0,1000);
	print(ivec);

	auto v = view::take(
	views::transform(
	views::filter(ivec, [](int x){return x % 5 == 0;}), [](int x){ return x * x;}), 5);

	for(auto i : v)
	{
		std::cout << i << ' '; // yine doğru çalıştı
	}

}

-------------------------------------------------------------------------------------------------------------------------------

DAHADA KOLAYIDA VAR :D:D

int main()
{
	auto ivec = getvec(200,0,1000);
	print(ivec);

	auto v = views::filter(ivec, [](int x){return x % 5 == 0;}) | views::transform([](int x){ return x * x;}) | views::take(10);

	for(auto i : v)
	{
		std::cout << i << ' ';
	}

	// View ında interfacei var. Böylede dolaşabiliriz.
	for(auto iter = v.begin(); iter != v.end(); ++iter)
	{
		std::cout << *iter << ' ';
	}

	ranges::for_each(v, [](auto x){ std::cout << "(" << x << ")"; }); // range isteyen algoritmaya bir view gönderebildik.

}

| kullanılıyor burada.
lazy Evaluationdan kasıt, burada range oluşturduk aama aslında o range oluşturduğumda kurallar tanımlamış oldum,
ne zaman bu range dolaşılırsa sırası ile bu işlemler yapılır. Range dolaşılmazsa funclar çağrılmıyor.

-------------------------------------------------------------------------------------------------------------------------------

BUNLARA NE GEREK VAR

1 - Biz bunları yapabilirdik.Biz bir döngü yazsaydık o döngü eagerly evaluate edecekti.Lazy evaluation yok.
Döngü neredeyse orada o işlemler yapılacak.

2 - Okunabilirlik

Raw döngüden mümkün olduğunca kaçınmak gerekli.

MOLA

=================================================================================================================================
=================================================================================================================================
=================================================================================================================================
=================================================================================================================================

Hew view sınıfı için range adaptörü var mı ?  Hayır. Aynı range adaptörü aynı view sınıfını döndürmüyor.Aldıkları argümana bağlı 
olarak farklı view sınıfları döndürüyorlar.

Range adaptörleri her zaman aynı view sınıfı türünden nesne mi döndürüyor?

Her view bir range, peki neden her range bir view değil? View hafif siklet bir range.string vector bir range. Bunları copy move
default constr. olmak zorunda değil, constant time garantiside yok. View olunca bazı operasyonlar O(1) komplexitisinde oluyor
Bİrde sizeof değeri var.

rangeleri constraint eden concepetler var, viewlar için standart conceptler var mı?

viewlar nasıl oluyorda begin, end gibi member funclara sahip oluyor?

kendim custom bir range sınıfı oluşturabilir miyim?

kendim custom bir iterator adaptörü oluşturabilir miyim?

tüm view sınıfları interface i aynı mı?

iki iteratörden bir view oluşturabilir miyim?

bir iterator ve bir tamsayı değerinden bir view oluşturabilir miyim?

SORULAR DERS İŞLENDİKÇE CEVAPLANACAK

---------------------------------------------------------------------------------------------------------------------------------

int main()
{
	vector<int> ivec(100);

	auto v1 = views::take(ivec,10);
	auto v2 = views::reverse(ivec);

	std::cout << "sizeof(v1) = " << sizeof(v1) << '\n'; // 8 byte
	std::cout << "sizeof(v2) = " << sizeof(v2) << '\n'; // 4 byte

}

---------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------

1. DERECEDE BİZİ İLGİLENDİREN VİEWLARI İNCELEYELİM

1. FILTER_VIEW
--------------
filter veren range araptörü views::filter, 
Sınıfı kullanırken ranges::filter_view şeklinde kullanılacak.

Filter diyorki bana bir unary predicate ve range vereceksin. Ben o rangedeki öğelerden
dolaşıldığında sadece predicate'ı sağlayanları dolaşacaksın.

Yani bizden bir range birde unary predicate isteyecek. Oluşturulan rangede underlying rangedeki o predicate'ı sağlayan öğelerin rangeini
oluşturmuş oluyoruz.

int main()
{
	vector<string> svec;
	rfill(svec,30,rname);
	
	print(svec);
	
	for(const auto &s : views::filter(svec, [](const auto &s){ return s.length() % 2 == 0;}))
			std::cout << s << '\n'; // Uzunluğu çift olanlar yazılacak
	
}

---------------------------------------------------------------------------------------------------------------------------------

int main()
{
	vector<string> svec;
	rfill(svec,30,rname);
	
	print(svec);
	
	char c;
	
	std::cout << "icinde hangi karkaterler olanlar : ";
	std::cin>> c;
	
	for(const auto &s : views::filter(svec, [c](const auto &s){ return s.find(c) != string::npos;}))
		std::cout << s << '\n'; // İçerisinde belirli bir karakter olanları yazdırdık
	
}

---------------------------------------------------------------------------------------------------------------------------------

İSİMLENDİRELİM

int main()
{
	
	vector<string> svec;
	rfill(svec, 30, rname);

	print(svec);

	char c;

	std::cout << "icinde hangi karkaterler olanlar : ";
	std::cin >> c;

	for (auto& s : views::filter(svec, [c](const auto& s) { return s.find(c) != string::npos; }))
		std::cout << s << '\n';

	//---------------------------------------------------------------------------------------------------------

	//std::cout << v.size() << '\n'; //SENTAKS HATASI SIZE FUNC YOK

	auto v = views::filter(svec, [c](const auto& s) { return s.find(c) != string::npos; };
	
	
	v.begin(); // GEÇERLİ
	v.end(); // GEÇERLİ

	// VEYA GLOBAL FUNCLARA ARGÜMAN OLARAKTA GÖNDEEBILIRIZ.
	ranges::begin(v); // GEÇERLİ
	ranges::end(v); // GEÇERLİ

	std::cout << v[3]; // BUDA SENTAKS HATASI OPERATOR[] YOK

	v.front(); //GEÇERLİ

}


---------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------

2. TRANSFORM_VIEW
-----------------
Öyle bir range oluşturuyorki o range te underlying range te olanları bir işleme sokuluyor
o işlemden elde ettiğimiz return değerleri range'imizi oluşturuyor.


int main()
{
	vector<string> svec;
	rfill(svec,30,rname);
	print(svec);
	
	char c;
	
	std::cout << "icinde hangi karkaterler olanlar : ";
	std::cin>> c;
	
	for ( const auto &s : views::transform(svec, [](const auto &s){ return s + "can";}))
			std::cout << s << '\n'; // kelimelerin sonuna can eklendi.
			
}

-----------------------------------------------------------------------------

int main()
{
	auto ivec = getvec(20,-20,50);
	
	print(ivec);
	
	auto v = views::transform(ivec,[](int x){ return std::abs(x);});
	
	for(auto i : v)
		std::cout << i << ' ';
	
}

-----------------------------------------------------------------------------

Yanlış anlaşılan bir konu var.

int main()
{
	vector<string> svec;
	rfill(svec,30,rname);
	print(svec);
	
	auto v = views::transform(svec,[](const auto &s){ return s.length(); });
	
	std::cout << typeid(*v.begin()).name() << '\n'; // DİKKAT!!!
							// Öğeler artık string değil, unsigned int
							// çünkü length return etti.
														
	for(auto len : v)
		std::cout << len << ' ';
}

-----------------------------------------------------------------------------

TRANSFORM İLE FİLTER BİRLİKTE ÇOK KULLANILIYORLAR

int main()
{
	vector<string> svec;
	rfill(svec,300,rname);
	print(svec);
	
	auto pred = [](const std::string &s){ return s.front() == s.back()};
	
	for(const auto &s : svec | views::filter(pred) |  views::take(10))  // take ilk 10 taneyi alıyor
	{
		std::cout << s << '\n';
	}
}


*/
