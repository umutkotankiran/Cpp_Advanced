/*

using vtype = std::variant<int,double,char>;
using vtype2 = std::variant<int,double,std::string>;
using vtype3 = std::variant<std::regex,int,double,std::string>;
using vtype4 = std::variant<std::mt19937,int,double,std::string>;
using vtype5 = std::variant<std::regex,int,double,std::string>;

int main()
{
	std::cout << "sizeof(vtype) = " << sizeof(vtype) << "\n"; // 16. Derleyiciye görede değişebilir tabi.
	std::cout << "sizeof(vtype2) = " << sizeof(vtype2) << "\n";  // 32
	std::cout << "sizeof(vtype3) = " << sizeof(vtype3) << "\n";  // 32
	std::cout << "sizeof(vtype4) = " << sizeof(vtype4) << "\n";  // 5008


	vtype x;
	std::cout << "sizeof(x) = " << sizeof(x) << "\n";
}

-----------------------------------------------------------------------------------------------

int main()
{
	using age_t = int;
	using weight_t = double;
	using name_t = std::string;

	std::variant<age_t, weight_t, name_t> vx;

	vx = 26;
	std::cout << "yas = " << get<age_t>(vx)<< "\n";
	
	vx = 87.56;
	std::cout << "agirlik = " << get<weight_t>(vx)<< "\n";

	vx = "Arda Uludag";
	std::cout << "isim = " << get<name_t>(vx)<< "\n";
}

NOT : Farklı variant türleri birbirine atanamıyor.
std::variant<int,double> vx;
std::variant<char,float> vx;
vx = vy; // SENTAKS HATASI

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------

EMPLACE FUNCTION
----------------

int main()
{
	variant<string, int, double, string> vx;

	vx.emplace<0>("murat");

	std::cout << get<string>(vx) << "\n"; //ambigiuty 2 string var
	cout << get<0>(vx) << "\n";

	vx.emplace<3>("aksoy");
	std::cout << get<3>(vx) << "\n";
}

------------------------------------------------------------------------------------------------------------------------------------------

KALITIMLA ILIŞKISI

Bİr çok durumda kapalı hiyerarşideki polimorfik davranışı oluşturmak için kalıtım yerine variant kullanmak hem
daha kolay, pointer semantiğini ortadan kaldırıyor ve birçok durumda maliyet açısından çoık daha iyi.

class Circle{
public:
	void render()
	{
		std::cout << "circle is being rendered\n";
	}
};

class Square{
public:
	void render()
	{
		std::cout << "Square is being rendered\n";
	}
};

class Triangle{
public:
	void render()
	{
		std::cout << "Triangle is being rendered\n";
	}
};

class Polygon{
public:
	void render()
	{
		std::cout << "Polygon is being rendered\n";
	}
};

using shape = std::variant<Circle, Square, Triangle, Polygon>;

int main()
{
	vector<shape> svec;

	svec.emplace_back(Circle{});
	svec.emplace_back(Square{});
	svec.emplace_back(Triangle{});
	svec.emplace_back(Polygon{});

	//BUNUN İÇİN BİR VİSİTER LAZIM.AŞAĞIDA YAZILDI LAMBDA OLARAK
	for(auto &object : svec)
	{
		visit([](auto &object){object.render();}, object); // visite lambda ve variant nesnesi argüman geçilmiş
	}
	//Polymorphic davranış ama halen value type bunlar.Değeri yok durumu içinde monostate i kullanabilirdik.
}

------------------------------------------------------------------------------------------------------------------------------------------

Burada bir örnek var. Hoca doğrudan yapıştırdı onu yazmıyorum.

------------------------------------------------------------------------------------------------------------------------------------------

İKİNCİ DERECEDEN DENKLEM ÇÖZÜMÜ ÖRNEĞİ

#include<iostream>
#include<variant>

using dpair = std::pair<double, double>;
using roots = std::variant<dpair, double,std::monostate>; // 2 kök olursa dpair, 1 kök varsa double, yoksa monostate

roots get_roots(double a, double b, double c)
{
	auto delta = b * b - 4. * a * c;

	if(delta > 0.)
	{
		auto rootdelta = std::sqrt(delta);
		return make_pair((-b - rootdelta) / (2. * a), (-b + rootdelta) / (2. * a));
	}

	if(!delta)
	{
		return - b / 2. / a;			// -b / (2. * a) ; ile aynı
	}

	return std::monostate{};
}

//BURASIDA VISITER
struct RootPrint{
	void operator()(const dpair &param)const
	{
		std::cout < "iki kok var : " << param.first << " " << param.second << " ";
	}

	void operator()(double param)const
	{
		std::cout < "tek kok var : " << param << "\n";
	}

	void operator()(std::monostate)const
	{
		std::cout < "kok yok : \n";
	}
};

int main()
{
	visit(RootPrint{}, get_roots(1,10.,16)); // get_roots burada variant return ediyor zaten
	visit(RootPrint{}, get_roots(3.,9.,8));  
	visit(RootPrint{}, get_roots(1.,8.,16)); 
}

------------------------------------------------------------------------------------------------------------------------------------------

ÖR:

using namespace std;

class Cat {
public:
	Cat(std::string name) : m_name{ std::move(name) } { }

	void meow()
	{
		std::cout << m_name << " miyavliyor\n";
	}

private:
	std::string m_name;
};

class Dog {
public:
	Dog(std::string name) : m_name{ std::move(name) } { }
	void bark()
	{
		std::cout << m_name << " havliyor\n";
	}
private:
	std::string m_name;
};

class Lamb {
public:
	Lamb(std::string name) : m_name{ std::move(name) } { }
	void bleat()
	{
		std::cout << m_name << " me'liyor\n";
	}
private:
	std::string m_name;
};

using Animal = std::variant<Dog, Cat, Lamb>;

// HOLDS ALTERNATIVE I TEMPLATE HALE GETIRDIK.
template <typename T>
bool is_type(const Animal& a)
{
	return std::holds_alternative<T>(a);
}

//VISITER OLUŞTURALIM
struct AnimalVoice {
	void operator()(Dog& dog)const {
		dog.bark();
	}

	void operator()(Cat& cat)const {
		cat.meow();
	}

	void operator()(Lamb& lamb)const {
		lamb.bleat();
	}
};

int main()
{
	vector<Animal> avec{ Cat{"pamuk"},Dog{"soko"}, Lamb{"kuzucuk"},Dog{"Kont"}, Lamb{"minnak"}, Dog{"lili"} };

	//switch case ile yaptık aslında Visit bunu kendi içinde yapıyor.
	for (Animal& a : avec)
	{
		switch (a.index()) // variantın index functionu bu
		{
		case 0: get<Dog>(a).bark(); break; //  get<0> da yazılabilirdi
		case 1: get<Cat>(a).meow(); break; //
		case 2: get<Lamb>(a).bleat(); break; //
		}
	}

	std::cout << "\n*********************************************************\n";

	//GET_IF İLE YAPIYORUZ.
	for (Animal& a : avec)
	{
		if (auto dog_ptr = get_if<Dog>(&a)) //girmezse nullptr olacak dog_ptr
		{
			dog_ptr->bark();
		}

		else if (auto cat_ptr = get_if<Cat>(&a)) //girmezse nullptr olacak
		{
			cat_ptr->meow();
		}

		else if (auto lamb_ptr = get_if<Lamb>(&a)) //girmezse nullptr olacak
		{
			lamb_ptr->bleat();
		}

	}

	std::cout << "\n*********************************************************\n";

	for (Animal& a : avec)
	{
		visit(AnimalVoice{}, a);
	}

	std::cout << "\n*********************************************************\n";

	std::cout << count_if(begin(avec), end(avec), is_type<Dog>) << " tane kopek var\n"; // is type ı yukarılarda tanımlamıştık
	std::cout << count_if(begin(avec), end(avec), is_type<Lamb>) << " tane kuzu var\n";
	std::cout << count_if(begin(avec), end(avec), is_type<Cat>) << " tane kedi var\n";

	std::cout << "\n*********************************************************\n";

}

ÇIKTI
-----
pamuk miyavliyor
soko havliyor
kuzucuk me'liyor
Kont havliyor
minnak me'liyor
lili havliyor

*********************************************************
pamuk miyavliyor
soko havliyor
kuzucuk me'liyor
Kont havliyor
minnak me'liyor
lili havliyor

*********************************************************
pamuk miyavliyor
soko havliyor
kuzucuk me'liyor
Kont havliyor
minnak me'liyor
lili havliyor

*********************************************************
3 tane kopek var 
2 tane kuzu var 
1 tane kedi var
*********************************************************


=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================

DINAMIK POLYMORPHYSM ILE VARIANT KULLANIMI ARASINDAKI TEMEL FARKLILIK ŞUNLAR

Kalıtımda ortak bir taban sınıf olması gerekliyor, diğer sınıflarından taban sınıftan kalıtımla elde edilmesi gerekiyor.
Bu durumda diğer sınıflar tür olarak taban sınıfla ilişkilendirilmiş oluyorlar. Artık bağımsız türler olmuyorlar ve
taban sınıftan kalıtım yoluyla elde edilen türler.Yani polimorfik davranış için türün kendisini dğeiştirmiş olduk.

Kalıtım olsaydı avantajı, daha sonradan kod ekleyebilmek.Eski kodlar değişmeyecek, eski kodlar yeni yazılanları kullanabilecek.
Variantta da bu mümkün değil. Variantta hiyerarşi kapalı. Baştan kullanılacak türler belli ise kullanılabilir.Daha
sonradan eklemeler yapılacaksa variant uygun seçenek olmaz.

Diğer taraftan kalıtımda yeni davranış yok.Taban sınıfın itnerfaceinde neler varsa türemiş sınıflar o funcları override
ediyor, bunlar için ayrı kod oluşturabiliyorlar. Variantta farklı farklı visiterlar oluşturarak farklı operasyonları
devreye alabiliriz. BUrada da alternatifler kapalı ama operasyonlar açık.

Kalıtımda hiyerarşi multilevel olabilir.Birden fazla derinlik olabilir yani.car->mercedes-> mercedes_coupe-> ....
Variantta ise tek bir seviye olmak zorunda. Virtual dispatch in getirdiği işlem maaliyetide var.
Bunlar bellek, işlem kodu, Vpointer tablosu, vpointer... En önemliside pointer semantiğin devre dışı kalması,
ortada poitne yok, dinamik ömürlü nesne olmak zorunda değil, double deletion, dangling reference gibi durumlar yok.
Ve daha hızlı.

Variantta her obje en büytük objenin sizeofu kadar oluyor. Bİraz uniona benziyor.

Variantta kopyalama maaliyeti yüksek olabilir.Burada kopyalama yapılmak zorunda
Virtual dispatch te ise clone işlemi yapılabiliyor.

Value type olduğundan containerda tutmak kolay.
Kalıtımda ise ya containerda pointer tutacaktık yada smart pointer tutacaktık.

VISIT ÇOK ELEŞTIRILIYOR
Olması gerekenden çok daha fazla maliyetli olduğu söyleniyor.Visit genellikle beğenilmiyor.

------------------------------------------------------------------------------------------------------------------------------------------

COMPILE TIME IF VISIT YAPISINDA KULLANILABİLİR
----------------------------------------------

#include <variant>

using namespace std;

int main()
{
	using vtype = variant<double, int, std::string>;

	//vector<vtype> vec{23,67, 2.234, "murat","deniz"};
	
	vec.emplace_back(87);  // yukarıdaki ilk değeri vermeseydik bu şekilde ekleyebilirdik.
	vec.emplace_back(5.12);
	vec.emplace_back("Huseyin");

	for(const auto &val : vec)
	{
		// visit([](const auto &x){std::cout << x << " ";}, val);
		
		visit([](const auto &x){
			if constexpr(is_same_v<decltype(x), string>) // burası compile time da belli oluytor.
				cout << quoted(x) << "\n";
			else
				std::cout << x << " "; 
		}, val);
	}
}

=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================

ANY
---
Çok az kullanılıyor. Kendisi bir sınıf şablonu değil. Header file any
Tuttuğu değeri bizim biliyor olmamız gerekiyor.Ne değer tuttuğunu bilip ona göre davranmamız gerekli

int main()
{
	any x;
	any y = 4.5;

	y = "mustafa";

	y = 3456; 
}

------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	any x;
	any y{};
	any z = {}; // Default ctor lar.Hepsi boş.

	----------------------------------------------------------------------------------------------------------------

	any a1{13}; // int
	any a2 = 4.5; // double
	any a3 = "remzi"; // const char *
	any a4 = "remzi"s; // string
	any a5 = std::bitset<16>(456u);
	any a6{ vector<int>{2,3,5,7,11} };

	----------------------------------------------------------------------------------------------------------------

	any a{in_place_type<string>,"necati"};	// Bu yerinde bir tane string nesnesi oluşturulacak.
											// Const char * da göndersem yerinde oluşturulacak nesne string

	any a{in_place_type<complex<double>>, 4.5, 7.6 };

	any a{in_place_type<Date>, 14,10,2010 }; // Date(14,10,2010) u tutacak

}

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------

MAKE ANY
--------
Diğer make funclarında oludğu gibi çalışıyor. Bİze bir any nesnesi verecek

int main()
{
	auto a = make_any<string>(15,'A');
	auto a2 = make_any<Date>(4,5,1987);
	auto a3 = make_any<complex<double>>(1.1, 5.6);
}

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------

HAS_VALUE
---------
Değer tutup tutmadığı bu func ile belli oluyor.Boolean değer döndürüyor ama 
operator bool yok

int main()
{
	any x{3};

	x.reset(); // Nesneyi boş hale getiriyoruz. Sınıf türden nesne olsaydı Dtoru çağrılacaktı

	if(x.has_value())
	{
		std::cout << "Deger tutuyor\n";
	}
	else
	{
		std::cout << "Deger tutmuyor\n";
	}

}

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------

TYPE
----
type_info nesnesi döndürüyor

int main()
{
	any x{13};
	any y{Date{1,5,1987}};
	
	std::cout << boolalpha;
	
	if(x.type()  == typeid(int))
	{
		// buraya devam etmedik	
	}
	
	std::cout << (x.type() == typeid(int)) << "\n"; // aynı ise  true, değil ise false
	std::cout << (x.type() == typeid(double)) << "\n";
	std::cout << (x.type() == typeid(Date)) << "\n";

	//Bunların name funclarıda var

	std::cout << x.type().name() << "\n"; // BURADA VERECEĞİ YAZI IMPLEMENTATION DEFINED

}

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------

ANY CAST - DEĞERE ERİŞMEK
-------------------------
Any void * kullanılan yerde alternatif oluşturuyor.Bir interface i var çünkü
any_cast kullanabiliyorum ama o türü tutmazsa exception throw ediyor. Eğer verdiğimiz tür argümanı doğruysa any_cast in ürettiği
değeri kullanabiliriz. 

int main()
{
	any x{2.35};

	//auto dval = any_cast<double>(x); // gerçekten double değer tutuyorsa, dval e bu değer kopyalanacak
					// Aksi halde bad any cast türünden hata gönderecek.

	try{
		std::cout << "deger : " << any_cast<double>(x) << '\n';
	}
	catch(const std::exception &ex)
	{
		std::cout << exp caught : << ex.what() << '\n';
	}
}

int main()
{
	any x {3.45};

	x = 45;

	try{
		auto &r = any_cast<int&>(x)
	}
	catch(const bad_any_cast&ex)
	{
		std::cout << "exception caught: " << ex.what() << '\n';  
	}

	-----------------------------------------------------------------------------------
	
	any x{"mustafa"s};

	any_cast<string&>(x).append("can");
	
	std::cout << any_cast<string>(x) << "\n";  

	-----------------------------------------------------------------------------------

	any a;

	std::cout << boolalpha;
	std::cout << a.has_value() << "\n";

	a = 45;
	std::cout << a.has_value() << "\n"; // var

	a.reset();
	std::cout << a.has_value() << "\n"; // yok
	
	a = true;
	std::cout << a.has_value() << "\n"; // var

	a = {};
	std::cout << a.has_value() << "\n"; // yok

}

--------------------------------------------------------------------------------------

class Nec{
public:
	Nec(int a, int b) : ma(a), mb(b) { }

private:
	int ma, mb;
};

int main(){

	any a;
	a = 45;
	a = Nec{3,5};
	
	a.emplace<Nec>(4,6);

	a.emplace<string>(100,'A');

	auto pred = [](int a, int b){ return std::abs(a) < std::abs(b); };

	a.emplace<set<int, decltype(pred)>>({1, 4, 7 ,9},pred); // C++17 yide desteklesin diye predi yine verdik

}

MOLA

==================================================================================================================================================================================================
==================================================================================================================================================================================================
==================================================================================================================================================================================================
==================================================================================================================================================================================================


int main()
{
	any a;

	a = "mustafa"; // copnst char * tutacak
	a.emplace<string>{mustafa}; bu seferde string tutacak
	
	a.emplace<string> ile operator= değer atamak için ama farkı empalce te türü belirtiyoruz
	a.has_value(); var
	a.operator bool() yok
	a.reset(); tuttuğu nesnenin hayatını sonlandırıp boş duruma çekiyor nesneyi.
	a.swap 2 any nesnesini swap ediyor.
	a.type ta type_info nesnesini döndürüyor.

}

------------------------------------------------------------------------------------------------------------------------------------------

ANY CAST FARKLI KULLANIMI
Birinde değer diğerinde ise adres gönderdik.

int main()
{
	any a = 3.4;

	auto val = any_cast<double>(a); // double tutmuyorsa exception throw eder

	any_cast<double&>(a) += 4.5; // Sentaks hatası değil.

	----------------------------------------------------------------------------------------------------

	// a = 'c'; dersek mesela ne test et
	
	if ( auto pd = any_cast<double>(&a)) // BURADA İSE A NIN ADRESİNİ GÖNDERDİK ANY CAST E.
	{
		cout << "Evet double turden deger : " << *pd << "\n";
	}
	else
	{
		std::cout << "tutulan deger double turden degil\n";
	}
}

ÇIKTI
-----
Evet double turden deger : 7.9

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------

any_cast<T>(a) // hata olursa exception throw

any_cast<T&>(a) // hata olursa exception throw

any_cast<T>(&a) // hata olursa nullptr

a.type()
a.reset()
a.swap()

make_any()
a.has_value()

İkna edici kullanım senaryosu pek yok gibi :D:D:D


==================================================================================================================================================================================================
==================================================================================================================================================================================================
==================================================================================================================================================================================================
==================================================================================================================================================================================================
==================================================================================================================================================================================================
==================================================================================================================================================================================================
==================================================================================================================================================================================================
==================================================================================================================================================================================================

C++20 İLE GELEN ARAÇLAR

SPACESHIP / THREE-WAY COMPARISON OPERATOR
------------------------------------------
Çok önemli bir ekleme. C++20 ile geldi.Bu bir operator.Karşılaştırma semantiğini değiştirmiş durumada
Daha az kod yazarak karşılaştırma kodları yazabiliyoruz.

<=>   Böyle tuhaf bir operator. Bu bir karşılaştırma operatörü

Custom bir sınıf türü için karşılaştırma operatorlerini nasıl overload ediyorduk?
Baya bir tekrar ediyordu kodlar. Bunların dezavantajları neydi
1 - Zahmetli
2 - Kod kalabalığı
3 - Yanlış kod yazma risli
4 - Karşılaştırma işlemleri farklı sınıflar türünden nesneleri karşılaştırınca yazılacak funcların sayısı artıyor.

C++20 ÖNCESINI YAZALIM	

class Nec
{
public:
	
	Nec(int x = 10) {} // C. CTOR

	bool operator==(const Nec &other)const
	{
		return id == other.id;
	}
	
	bool operator<(const Nec &other)const
	{
		return id < other.id;
	}
	
	bool operator!=(const Nec &other)const
	{
		return !(*this == other);
	}
	
	bool operator>=(const Nec &other)const
	{
		return !(*this < other);
	}
	
	bool operator>(const Nec &other)const
	{
		return (other <*this);
	}
	
	bool operator<=(const Nec &other)const
	{
		return !(*this < other );
	}

	friend bool operator< ... 


private:
	int id;
};

int main()
{
	Nec mynec(12);

	mynec == 5; // Derleyici int türden ifadeyi Nec sınıfına dönüştürür.

	5 == mynec; // yapılamaz şuanda ??Sol operand sınıf türünden olmalıydı.Bu member operator func olduğundan 
				// sol operand sınıf türünden olmalıydı bu yüzden global operator func / free olarak overload etmeliydik
				// o zaman  bu dönüşümler otomatik olarak yapılacak.
}


BURADAKİ PROBLEMLER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
-------------------------------------------------
	6 TANE FUNC YAZDIK. AMA HER DURUMDA ASLINDA YAZDIĞIMIZ 2 FUNCTİON VAR( < ve ==) BUNLARI ÇAĞIRARAK
  	DİĞER 4 FUNC İŞİNİ GÖRÜYOR.

	BU KODLARI DERLEYİCİ YAZABİLİR ASLINDA. DERLEYİCİ BUNUN KODUNU BENİM İÇİN YAZ DİYEBİLİYORUZ.
	BİR ÇOK DURUMDA HİÇ KOD YAZMADAN 6 TANE KARŞILAŞTIRMA OPERATÖRÜNÜDE EDİNMİŞ OLUYORUZ
	SADECE <=> SPACESHIP OPERATORUNU DEFAULT EDEREK.BİR ÇOK DURUMDA BU TÜR DÖNÜŞÜMÜYLE EXPLICIT OLMAYAN CTORUN KULLANILMASI DURUMUNDA
	YİNE İLAVE GLOBAL OPERATOR FUNC YAZMAMIZA GEREK KALMIYOR. HATTA YENİ GELEN ARAÇLA GLOBAL OPERATOR FUNC OLMASININ AVANTAJIDA
	ORTADAN KALMIŞ OLUYOR.

1- ARTIK 1. SEÇİMİMİZ SPACESHIP OPERATÖRÜNÜ MEMBER OPERATOR FUNC OLARAK OVERLOAD ETMEK.YENİ GELEN KURALLARLA
	NORMALDE BİNARY OPERATÖRÜN SOL OPERANDI SINIF TÜRÜNDEN OLMASI GEREKİRDİ AMA ŞİMDİ İLLAHA SNIF OLMASINA GEREKTE YOK.

2- 5 < a DEDİĞİMİZDE 5.operator< HATA IDI AMA ARTIK a.operator(5) OLARAK ÇEVIRIP ÇAĞRIYI YAPIYOR.
	DİLİN YENİ KURALLARI ARTIK OPERANDLARIN YERİNİ DEĞİŞTİRİLMESİ VE ÇAĞRILABİLİR FUNC OLUP OLMADIĞININ SINANMASINI BİR 
	GARANTİ OLARAK DİLE EKLEDİ.

3- BEN EXCEPTION THROW ETMEYECEK GARANTİSİNİ VERİYORSAM(NOTHROW GARANTİSİ) BUNUDA AÇIK OLARAK YAZMALIYIM.
	DERLEYİCİ BİR KODU BİZİM İÇİN YAZDIĞINDA KODA BAKARAK HERHANGİBİR ŞEKİLDE EXCEPTION GÖNDERMEME GARANTİSİ VERİYORSA
	NOTHROW GARANTİSİNİ KENDİSİ SAĞLAYABİLİR.

4- FUNCTIONNUN CONSTEXPR OLUP OLMAMASI. NORMALDE KENDİMİZ TANIMLAMALIYIZ AMA CONSTEXPR OLMASI İÇİNDE CONSTEXPR FUNC
	OLMA KOŞULLAIRNI SAĞLAMASI GEREKİYOR. DERLEYİCİ BUNUN KODUNU YAZDIĞINDA BU YİNE AVANTAJA DÖNÜŞECEK, DERLEYİCİ BAKACAK
	CONSTEXPR Yİ ÇİĞNEYEN DURUM YOKSA FUNCTIONU CONSTEXPR OLARAK TANIMLAYACAK.COMPILE TIMEDA YAPACAK TABİ BUNU

5- SINIF TÜRÜNDEN OLMAYAN DEĞERİ KARŞILAŞTIRMA OPERATÖRÜNÜN OPERANDI YAPTIĞIMIZDA TİPİK OLARAK FRIEND FUNC YAZMAMIZ GEREKİYOR
	VE GLOBAL OPERATOR FUNC OLMASI GEREKİYOR.GLOBAL OPERATOR FUNC OLMA ZORUNLULUĞUDA ORTADAN KALKIYOR.BÖYLECE SINIFIN ÜYE FUNCI 
	OLARAK KENDİMİZDE YAZABİLİRİZ DERLEYİCİYEDE YAZDIRABİLİRİZ.

6- DERLEYİCİNİN BU KODLARI YAZMASI, DERLEYİCİNİN BUNLARI NODİSCARD ATTRİBUTELARIYLA TANIMLAMA İMKANIDA VERİYOR DERLEYİCİYE.
	BİZ YAZSAYDIK NODISCARDI KENDİMİZ BELİRLEYECEKTİK VEYA CONSTEXPR

Yukarıda yazılanları derleyici kendi saptayabilecek.

------------------------------------------------------------------------------------------------------------------------------------------

KENDIM YAZSAM AŞAĞIDAKI GIBI YAZACAKTIM 
class Nec
{
public:
	constexpr Nec(int x) : m_id(x) {}

	[[nodiscard]] friend constexpr bool operator==(const Nec& lhs, const Nec& rhs)
	{
		return lhs.m_id == rhs.m_id;
	}
	
	[[nodiscard]] friend constexpr bool operator<(const Nec& lhs, const Nec& rhs)
	{
		return lhs.m_id == rhs.m_id;
	}

private:
	int m_id
};

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------

GELELİM C++20 YE YANİ SPACESHIP OPERATÖRÜNE 

C++ 20 ile sınıfların temel olarak 2 tane karşılaştırma operatörü olabiliyor.Bunlar == ve <=> operatörü.
Bir sınıfa == funcını eklersek derleyici artık != funcını kendisi yazıyor.Eskiden herşeyi kendimiz yazardık :D

1. Biz sınıfa operator== yazarsak operator!= i de kendimiz yazacaktık ama artık operator!= i derleyici bizim için yazıyor.

class Nec{
public:
	Nec(int i) : m_id{i} { }

	bool operator==(const Nec& other)const
	{
		return m_id == other.m_id;
	}

private:
	int m_id;
};

int main()
{
	Nec n1{13}, n2{98};

	cout << boolalpha;

	cout << "(n1 == n2) : " << (n1 == n2) << '\n';  // == Biz yazdık
	cout << "(n1 != n2) : " << (n1 != n2) << '\n';  // != ise derleyici tarafından yazıldı.GEÇERLİ

	----------------------------------------------------------------------------------------------------------------------

	CTOR EXPLICIT DEĞIL

	cout << (13 == n1) << "\n"; // DERLEYİNCE GEÇERLİ.SENTAKS HATASIYDI BU NORMALDE. SOL OPERAND SINIF TURDEN DEĞİL.
	cout << (13 != n1) << "\n"; // DERLEYİNCE GEÇERLİ.YİNE AYNI ŞEKİLDE

	// NASIL OLUYOR ? 
	// KURALLAR DIYORKI 13 == n1 OLDUĞUNDA DERLEYICI BUNU YAPACAK BIR FUNC BULAMAYINCA ARGÜMANLARI SWAP EDIYOR !!!!!!!!!!!!!!!!!
	// n1 == 13 ÇAĞRISINA DÖNÜŞTÜRÜYOR, BURADA CONVERSION CTOR ILE 13, NEC TÜRÜNE DÖNÜŞTÜRÜLECEK
}

1. BURADA SPACESHIP <=> İLE KARŞILAŞTIRMA == OPERATÖRÜNÜN BİRBİRİNDEN BAĞIMSIZ İKİ OPERATOR OLMASI VE == İ OVERLOAD ETTİĞİMİZDE
	DERLEYİCİNİN != İDE KENDİSİNİN YAZMASI. BİRİNCİ DEĞİŞİKLİK BU


2. 5 == n1 İFADESİ NORMALDE SENTAKS HATASIYKEN DERLEYİCİ COMPİLE TİME OPERANDLARI SWAP EDİP ÇAĞRI LEGAL Mİ DEĞİL Mİ DİYE BAKACAK
	n1 == 5 YAPACAK YANİ

3. == İN KENDİSİNİ DERLEYİCİYE YAZDIRABİLİRİZ.

	class Nec{
	public:
		bool operator==(const Nec&)const = default; //ŞİMDİ == VE != OPERATOR FUNCLARINI DERLEYİCİYE YAZDIRDIK !!!!
			
	};

	KODLARI KENDİMİZ YAZSAYDIK AŞAĞIDAKİ GİBİ OLURDU.
	class nec{
	public:
		bool operator==(const Nec&other)const
		{
			return m_id == other.m_id && name == other.name;
		}
	
		bool operator=!(const Nec&other)const
		{
			return !(*this == other);
		}
	private:
		int m_id;
		string name;	
	};



*/
