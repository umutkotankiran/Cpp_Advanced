/*

Bunlar vocabulary types C++17 ile eklendi

optional
variant
any

any sınıf şablonu değil. ama variant ve optional sınıf şablonu

optional sınıfı türünden nesne ya bir değere sahip yada bir değere sahip değil bu durumda değeri
nullopt. nullopt, std::nullopt_t türünden constexpr bir nesne

ÖR:
std::optional<int> to_int(const std::string&s)
{
	try{
		return std::stoi(s); // stoi geçerli bir tamsayı tutmuyorsa exception throw ediyor.
	}
	catch(...) // catch all temel C++
	{
		return std::nulopt;
		// return {}; buda kullanılabilirdi
	}
}

--------------------------------------------------------------------------------------------------------------------------------------

Ağağıdaki gibide yazabilirdik

std::optional<int> to_int(const std::string&s)
{
	std::optional<int> ret;

	try{
		ret = std::stoi(s);
	}
	catch(...) // catch al temel C++
	{

	}
	return ret; // exception gönderilirse ret nullopt değerde ve bu değeri gönderir
				// exception göndermezse atanan değeri göndeirr
}

int main()
{
	for(auto str : {"345", "987","alican","01555"})
	{
		if(auto op = to_int(str)){
			std::cout << "gecerli... deger : " << *op << "\n";
		}
		else
		{
			std::cout << str << " gecersiz tamsayi";
		}
	}
}

ÇIKTI
-----
gecerli... deger : 345
gecerli... deger : 987
alican gecersiz tamsayi
gecerli... deger : 1555

--------------------------------------------------------------------------------------------------------------------------------------

TAŞIMA SEMANTİĞİNİDE DESTEKLİYOR

struct Nec{
	Nec()
	{
		std::cout << "default ctor\n";
	}
	Nec(const Nec&) = delete;
	Nec& operator=(const Nec&) = delete;
	Nec(Nec&&)
	{
		std::cout << "move ctor\n";
	}
};

int main()
{
	Nec nec;

	std::optional<Nec> op1{std::move(nec)};	// Move ctor çağrıldı.Taşıma işlemi yapıldı.Nec nesnesi moved from state te

	//std::optional<Nec> op2(std::move(*op1)); // op1 in tuttuğu değer için taşıma yapılmış.

	std::optional<Nec> op2(std::move(op1)); // Buda olabilirdi

	std::cout << (op1 ? "not empty" : "empty"); // Tuttuğu değer moved from state olması başka destroy olması başka
}						    // Not Empty yazar. Halen bir değeri var.Bu değerde moved from statete.

Tuttuğu nesnesin taşınması başka, nesnenin boş veya dolu olması başka

--------------------------------------------------------------------------------------------------------------------------------------

ÇOK GÜZEL BİR ÖRNEK

#include <algorithm>
#include <optional>
#include <vector>
#include <iostream>
#include <iterator>

template <typename Con, typename Pred>
auto find_if(Con &&c, Pred &&pred) // Bildiğimiz find_if bir iterator rangei alıyordu ama bizimki bir container alıyor.
{
	using std::begin , std::end; // Buda C++ 17 ile geldi virgülerle ayırma olayı
								 // Neden böyle bir bildirim yaptık? Custom bir begin veya end func varsa onu kullanabilmek için.
								 // Bu funcları visible hale getirdik.Custom bir begin varsada onun seçilme şansı olacak.

	auto beg_iter = begin(c),end_iter = end(c);

	auto result = std::find_if(beg_iter, end_iter, pred); // result bir iterator, türü compile timeda belli olacak

	using iterator = decltype(result); // Result iteratorune eş isim verdik.

	if(result == end_iter) // result end_iter ise bu öğe bulunamamıştır
	{
		return std::optional<iterator>(); // öğe bulunamadıysa bunu döndürüyoruz.Bu durumda default ctor çağrılacak yani değeri olmayan optional döndürülecek
	}									  // auto ile tür çıkarımı yapıldığı için nullopt değilde bu şekilde yazdık.

	return std::optional<iterator>(result);
}

//BU TEMPLATE SONRADAN YAZILDI.TAMAMLAYICI OLARAK DÜŞÜNÜLEBİLİR
template <typename Con, typename T>
auto find(Con&&c, const T& val)
{
	return find_if(std::forward<Con>(c), [&val](auto &&x) {return x == val;});
}

int main()
{
	using namespace std;
	vector<int> ivec{1,3,7,10,15,122};

	if(auto op = find_if(ivec,[](int x){ return x % 2 == 0;}))
	{
		cout << "bulundu " << **op; // *op iterator. **op ise iteratörün konumundaki nesneyi veriyor.
		cout << "index = " << std::distance(ivec.begin(), *op) << '\n';
	}

	-----------------------------------------------------------------------------------------------------------------------------------------

	// find ile de çağırabiliriz

}

--------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------

SINIFIN VERİ ELEMANI OLMASI DURUMU

class UserRecord
{
public:
	UserRecord(std::string name, std::optional<std::string> nick, std::optional<int> age) :
	m_name(std::move(name)), m_nick(nick), m_age(age) { }

	friend std::ostream& operator<<(std::ostream&os, const UserRecord &user)
	{
		os << user.m_name << ' ';

		if(user.m_nick) // bu ifler olmazsa Undefined behavior riski var.
		{
			os << *user.m_nick << ' ';
		}

		if(user.m_age)
		{
			os << *user.m_age << " yasinda";
		}

		return os;
	}

private:
	std::string m_name;
	std::optional<std::string> m_nick;
	std::optional<int> m_age;
}

int main()
{
	UserRecord urx = {"Salih Korkusuz", "Salo",40};
	std::cout << urx << "\n";

	UserRecord ury = {"Necati Ergin", std::nullopt,std::nullopt};
	std::cout << ury << "\n";
}

Salih Korkusuz Salo 40 yasinda
Necati Ergin

=====================================================================================================================================================
=====================================================================================================================================================
=====================================================================================================================================================
=====================================================================================================================================================
=====================================================================================================================================================
=====================================================================================================================================================

VARIANT
-------
Unionu hatırlamak gerekiyor.Kısaca bir hatırlatma yaptı hoca.
Unionun tüm elemanları ortak adreste. Unionlarda 2 tane durum var.

header file variant.Bu variadic bir sınıf şablonu

1 - Union içerisinde user defined tür olduğundan bunların ctor dtor çağrılması gerek bunu yapmak
çok zahmetli.Bunu bizim yapmamız gerekiyor.

2 - Union ne tuttuğunu göstermiyor.Hangi memberına erişeceimizi bilemeyiz.
Bu unionu bir struct ile sarmalayabiliriz bunu kolaylaştırmak için aşağıdakini yapıyoruz.

struct Data
{
	union{
		int x;
		double d;
		long c;
	};
	int type;
};

int main()
{
	Data mydata;  // type ile doğru alterntife erişebiliriz. Buna tagged/discreminated union deniyor.
	mydata.type = 0; // int olduğunu gösterdik
	mydata.x = 987;
}

C++17 discreminated/tagged unionu standrt bir sınıf şablonu haline getirdi.Bu sınıf variant sınıf şablonu
Bu aslında kalıtıma bir çok durumda polymorphic tür yaratmaya göre bir başka seçenek daha doğuyor.
Kalıtımda bunu sağlamak için aynı türden olmayan nesnelerin ortak bir arayüzü destekledikleri için
onları aynı türdenmiş gibi kulanma olanağı. Bunun için türünü değiştiriyoruz. Çok büyük bir bağımlılık.

Eğer hiyerarşi kapalıysa, yani daha sonradan ilave türetme yapılmayacak.

Not : C de union bir veriyi 2 farklı şekilde temsil etmek için kullanılıyor.
Union yerine variant neden kullanılır ?
- Unionda C++ta kalıtım yapılamıyor.
- Variantın kendisi bir sınıf olduğundan varianttan kalıtım gerçekleştirilebilir.

---------------------------------------------------------------------------------------------------------------

KALITIMA DA ALTERNATİF

int main()
{
	using namespace std;

	variant<int,double,string>;

	variant<Cat,Dog,Lamb> x; // Bir variant nesnesi 3 türüde tutabilsin

}

Burada benim hangisi var nasıl bileceğim ? Burada visit devreye girecek

---------------------------------------------------------------------------------------------------------------

VARIANT VARIADIC BIR SINIF ŞABLONU

Variantta tutulabilecek elemanlara variantın alternatifleri deniyor.
Dinamik bellek alanı kullanmıyor. Dolayısıyla pointer/referans semantiği kullanmıyor.
Tamamen value type kullanılıyor.

variant<int,double,long> x; // Bu heapten bir alan kullanmıyor

VARIANTA DIZI TÜRÜ OLAMAZ, REFERANS TÜRÜ(REFERENCE WRAPPER AYRI) OLAMAZ, VOID TÜRÜ OLAMAZ

int main()
{
	using namespace std;
	variant<int,double,long> x;// VARIANT SIZEOF DEĞERI IÇERISINDEKI EN BÜYÜK ÖĞE ILE ILGILI.

	variant<int,Biggie,long> x;
}

VARIANTIN HEAP İLE ALAKASI YOK.

---------------------------------------------------------------------------------------------------------------

Bir variantın hiçbirşey tutmama ihtimali yok ama std::monostate diye bir tür var. Bu empty class.
variant<monostate> açılımı bu durumda variant boş kabul edileilir. Doğrudan boş variant yok.

int main()
{
	variant<int, double, int> ; // Birden fazla aynı tür alternatifi olabilir.LEGAL ama ambigiuty durumlarına dikkatmek gerekiyor.

	variant<int, double, const int, volatile int> ; // GEÇERLİ

	variant<int,double,long> x; // Float diye bir alternatif yok. Sınıfın interfaceini kullanarak ben x teki değer float olarak kullanmak istesem SENTAKS HATASI

}

---------------------------------------------------------------------------------------------------------------

class Myclass;
class Yourclass;
class Herclass;

int main()
{
	variant<Myclass,Yourclass, Herclass> x; //Sentaks hatası
}

---------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------

DEFAULT INIT EDILEBİLİR

int main()
{
	variant<int, double, string> vx; // Default init edilen variant nesnesi 1. alternatif ne ise o türden değer tutuyor.o türden değer tutuyor.
									  // ilk öğe string olsaydı string türden tutacaktı ve Value init ediyor.

	cout << get<0>(vx) << "\n"; // Value initten ötürü 0.
	get<int>(vx);

	----------------------------------------------------------------------

	variant<Date, int, double, string> vx; // Default init edilen variant nesnesi 1. alternatif ne ise o türden değer tutuyor.o türden değer tutuyor.

	cout << get<Date>(vx) << "\n"; // Default init edilmiş bir Date nesnesi tutacak.tarih 1.1.1900 olabilir.

}

-------------------------------------------------------------------------------------------------------------

DEĞERE ERİŞMEK

int main()
{
	variant<Date, int, double, string> vx; // Default init edilen variant nesnesi 1. alternatif ne ise o türden değer tutuyor.o türden değer tutuyor.

	//vx = "Ali"; Buda olabilir.

	get<0>(vx) = Date{1,5,1987};

	get<Date>(vx) = Date{1,5,1987};// Böylede olur

	cout << get<Date>(vx) << "\n";

	cout << get<float>(vx); // SENTAKS HATASI.Float yok.Compile timeda kontrol etti

	cout << get<4>(vx); // SENTAKS HATASI.4 öğe yok.Compile timeda kontrol etti

}

-------------------------------------------------------------------------------------------------------------

DEFAULT CTORU OLMAYAN SINIF ÖRNEĞI

class Myclass //
{
public:
	Myclass(int);
};

int main()
{
	variant<Myclass,int, double> vx;  // Myclassın default ctoru olmadığından burada SENTAKS HATASI

	variant<int, Myclass, double> vx; // HATA DEĞİL.Burada variantın tuttuğu değer int.İlk alternatif kullanılıyor burada.
}

MOLA

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

SINIFIN DEFAULT CTORU YOK BU SEBEPLE VARIANTI ILK ELEMANI MYCLASS YAPIP BUNU DEFAULT CTOR EDEMEYIZ.

class Myclass
{
public:
	Myclass(int,int);
};



int main()
{

	variant<Myclass, int, double> vx; //SENTAKS HATASI

	--------------------------------------------------------------------------------------------------------------------

	variant<monostate, Myclass, int, double> vx; // Artık geçerli. Variant monostate tutuyor ama monostate empty class
												  // Bu variant default init edilebilir her durumda

	--------------------------------------------------------------------------------------------------------------------

	variant<Myclass, int, double> vx = 12; // Argüman int olduğundan int alternatifi kullanılır.

	--------------------------------------------------------------------------------------------------------------------

	variant<Myclass, int, double> vx = 1.2; // Argüman double olduğundan double alternatifi kullanılır.

	--------------------------------------------------------------------------------------------------------------------

	variant<Myclass, int, double> vx = 4.5f; // Floattan double a promotion oldu. GEÇERLİ

	--------------------------------------------------------------------------------------------------------------------

	variant<Myclass, int, double> vx = 5u; // Unsigned int olduğundan SENTAKS HATASI

	--------------------------------------------------------------------------------------------------------------------

	variant<Myclass, int, double> vx = Myclass{ 1,5 }; // Argüman myclass olduğundan myclass alternatifi kullanılır.

	--------------------------------------------------------------------------------------------------------------------

	DİKKAT !!!!!!!

	Bunu bir date nesnesiyle başlatmak istiyoruz ama Date e gönderdiğimiz argümanlarla doğrudan yerinde Construct edilmesini
	istiyoruz.

	variant<Date,Myclass,int,double> vx {in_place_index<0>,3,5,1985}; // 0 indexli alternatifte Default Ctor çalıştırıldı

	cout << get<Date>(vx)<< "\n";

	--------------------------------------------------------------------------------------------------------------------

	variant<Date,Myclass,int,double> vy {in_place_index<1>,3,5}; // Myclass oluturduk yine

	--------------------------------------------------------------------------------------------------------------------

	In place type ta var. Type kullanıyoruz.

	variant<Date,Myclass,int,double> vy {in_place_type<Myclass>,10,15};

}

---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------

TUTULAN DEĞERE ERİŞMEK
Çok kritik burası

GEÇERSİZ INDEX/TÜR ARGÜMANI KULLANIMI SENTAKS HATASI !!!!

int main()
{
	variant<char,double,float> vx;

	std::cout << get<int>(vx); //EXCEPTION THROW. bad_variant_access. int yok çünkü

	-----------------------------------------------------------------------------------------

	variant<char,double,float> vx;
	auto val = get<int>(vx); // SENTAKS HATASI.Burada chardan inte promotion yok.

	-----------------------------------------------------------------------------------------

	variant<char,double,float> vx;
	auto val = get<3>(vx); // COMPILE TIME HATASI.3 indexi yok.

	-----------------------------------------------------------------------------------------


	Alternatifi kontrol etmeden get ile erişirsek fakat o alternatif tutulmuyorsa Exception Throw eder
	bad_variant_access türünden exception throw eder.

	variant<char,double,float> vx{4.5};

	try{
		auto val = get<char>(vx); // Exception throw eder.Alternatif char değil, double.
		auto val = get<2>(vx); // Exception throw eder.Alternatif float değil, double.
	}
	catch(const std::bad_variant_access& ex)
	{
		std::cout << "exception caught: " << ex.what() << '\n';
	}

}

------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------

ALTERNATIFI GET EDEN FUNCTIONLAR VAR

1. HOLDS_ALTERNATIVE
--------------------
holds_alternative, return değeri boolean.
Tür argümanı var ama index argümanı alan overloadu yok.

int main()
{
	variant<char, double, float> vx{4.5}; // Alternatif float olsaydı double tutmazdı.
										  // Promotion olmayacak çünkü doğrudan float alternativ türü var.

	if(holds_alternative<double>(cx))
	{
		std::cout << "evet double tutuyor";
	}
	else
	{
		std::cout << "hayir double tutmuyor";
	}
}

Doğru

------------------------------------------------------------------------------------------------------------------------------------

2. INDEX
--------
Doğrudan indexi döndürüyor

int main()
{
	variant<char, double, float> vx{4.5f};
	std::cout << vx.index() << '\n';		// 2
}

NOT: emplace var
vx.emplace<double>
vx.emplace<1>   ikiside index verilebiliyor.

Özetle, türünü anlamanın yolu holds_alternative veya index çağrısı

------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------

MONOSTATE

İlk eleman monostate olabilir.Bu durumda default init edebiliriz ve tutulan alternatif monostate olacak.

class Data{
public:
	Data(int x) : mx{x} {}

private:
	int mx;
};

int main()
{
	variant<monostate,int,double,Date> vx; // Geçerli.

	std::cout << "index = " << vx.index(); // index 0

	----------------------------------------------------------------------------------------------------------

	variant<Data,int> ; //SENTAKS HATASI.DATANIN CTORU YOK

	----------------------------------------------------------------------------------------------------------

	variant<monostate,Data,int,double> vx;

	std::cout << "index = " << vx.index();

	if(holds_alternative<monostate>(vx)) // Böylede sınayabiliriz.
	{
		std::cout << "Variant bos\n";  // Bos değil aslında monostate tutuyor.monostatein kendisi empty class :D:D
	}
}

------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------

ATAMA YAPABİLİRİZ.

int main()
{
	variant<monostate,int,double,Date> vx;

	vx = 5;

	vx = Data{32};

	vx = 4.5;

	vx = monostate{}; // burada monostate geçici nesnesi atanmış.

	vx = {}; // Atama yapıldı. Aynı türden bir başka variant nesnesi oluşturup onu atamış oldum.Default init edildiğinden bu
			 // monostate nesnesi tutuyor.Çünkü ilk tür monostate. İlk elemanı int olsaydı türü int olacaktı

----------------------------------------------------------------------------------------------------------

BİR SORU ÜZERİNE HOCA AÇIKLAMA YAPTI ORAYI

int main()
{
	variant<int, double, long> vx{1};

	std::cout << vx.index() << '\n'; // 0

	vx = 2.3;

	std::cout << vx.index() << '\n'; // 1

	vx = 5L;

	std::cout << vx.index() << '\n'; // 2

}

-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------

GET_IF
------

int main()
{
	variant<int,double,string> vx{"Mustafa Ceyhan"}; // şaunda index 2

	// get_if<2>(&x) de yazılabilir aşağıda

	if(auto p = get_if<string>(&x))    // Alternatif string ise p nullptr olmuyor. Yani string * oluyor
	{								   // string değilse p nullptr oluyor.
		std::cout << *p << "\n";
	}
	else
	{
		std::cout << "alternatif string degil\n";
	}
}

---------------------------------------------------------------------------------------------------------------------------

IN PLACE INDEX ÖRNEĞİ

int main()
{
	variant<int, double, complex<double>> vx{in_place_index<2>, 3.5, 7.8};

	std::cout << get<2>(vx) << '\n'; // (3.5,  7.8) complex sınıfının ctoru çağrıldı
}

-------------------------------------------------------------------------------------------------------------------

int main()
{
	auto pred = [](int a, int b)
	{
		return std::abs(a) < std::abs(b);
	};

	variant<set<int>, set<int, decltype(pred)>> vx{in_place_index<1>, {1,4,6,9,-1>}, pred}; // 2 tane set alternatifi var
																							// C++17 olduğunda en sondaki pred yazılacak
}																							// C++20 de gerek yok.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

KARŞILAŞTIRMA

Farklı türden variantlar birbiriyle karşılaştırılamıyor.

int main()
{
	variant<monostate, int, string> vx;
	variant<int, string> vy;

	std::cout << (vx == vy); //SENTAKS HATASI.VARIANT TÜRLERİ FARKLI
}

-------------------------------------------------------------------------------------------------------------------

int main()
{
	variant<monostate, int, string> v1, v2{"mustafa"}, v3(123);
	variant<monostate, int, string> v4;

	v1 alternatifi monostate
	v2 alternatifi string yani index 2
	v3 te 1

	NOT : ALTERNATİFİ KÜÇÜK OLAN KÜÇÜK OLUYOR.ALTERNATİF AYNIYSA DOĞRUDAN DEĞERLER KARŞILAŞTIRILIR

	std::cout << boolalpha;

	std::cout << "v1 == v2 : " << (v1 == v2) << '\n'; // FALSE. V1 V2 V3 türleri aynı kıyaslama yapılabilir.

	std::cout << "v1 < v2 : " << (v1 < v2) << '\n'; // TRUE

	std::cout << "v2 < v3 : " << (v2 < v3) << '\n'; // FALSE

	v2 = 89

	std::cout << "v2 < v3 : " << (v2 < v3) << '\n';  // TRUE. İkiside int türünden artık. Değerler karşılaştırılıyor. 89 < 123  TRUE
}

Farklı türden variantları karşılaştırmak sentaks hatası

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

VISIT FUNCTION

Birinci parametreye callable gönderiyoruz ikinciye variant nesnesini

int main()
{
	variant<int, double, string> vx{46.7}; // diyelim ki runtimeda ne değer tuttuğunu bilmiyoruz.

	visit(mycallable); // içerideki herhangibir callable

	//VİSİT OLMASAYDI AŞAĞIDAKİ GİBİ YAPARDIK.
	if(vx.index() == 0)
	{
		std::cout << get<0>(vx) << '\n';
	}
	else if(vx.index() == 1)
	{
		std::cout << get<1>(vx) << '\n';
	}
	else if(vx.index() == 2)
	{
		std::cout << quoted(get<2>(vx)) << '\n';
	}

	//YUKARIDAKİLERİ YAPMANIN KOLAY YOLU VİSİT
}

----------------------------------------------------------------------------------------

VİSİT İLE YAPACAĞIZ
Visit functionuna herhangibir callable gönderebilirim.
Bu callable varianttaki alternatiflerin tamamını alabilecek bir operator funclar olmalı

struct Print{

	// BUNLAR MEMBER TEMPLATE İLE YAZILABİLİRDİ.
	void operator()(int x)const  // func call operator func yazıyoruz callable yazılacak çünkü visite
	{
		std::cout << x << '\n';
	}

	void operator()(double x)const
	{
		std::cout << x << '\n';
	}

	void operator()(string x)const
	{
		std::cout << quoted(x) << '\n';
	}

	//TEMPLATE YAZSAYDIK AŞAĞIDAKİ GİBİ OLACAKTI
	//template <typename T>
	//void operator()(const T& val)
	//{
	//		std::cout << val << '\n';
	//}
};


int main()
{
	variant<int,double,string> vx ("Murat Hepeyiler");

	visit{Print{},vx}; // Az önce yukarıda yazdığımız else if merdiveni Visit funcının içinde var zaten.

	vx = 4.5;
	visit(Print{},vx); // Bu seferde 4.5 i yazar.

	vx = "melike";
	visit(Print{},vx);

}

MOLA

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DİKKAT !!!!!!!!!!!!!!!!!!!!!!!!!!!

En son visitte kaldık. Bir variant var. Bunda kullanılan alternatif neyse bunu kullanmak istiyoruz.
Bunu aslında bir else if veya switch ile yapabiliriz fakat daha kolay hale getiren bir func şablonu var buda visit.

Bunu sağlamak için callable içerisinde, variattaki tüm alternatifleri alabilecek operator funclar olması gerekiyor
Bunuda sağlamak için,
1 - Her bir alternatif için operator func yazılabilir

struct Print
{
	void operator()(int)const;
	void operator()(double)const;
	void operator()(const string &r)const;
};

int main()
{
	variant<int, double,string> vx;
	visit(Print{},vx);
}


2 - Bunlar aynı aynı funclar olması yerine eğer template haline gelebilecek genel bir func ise operator funcını template
	yapabiliriz.

struct Print
{
	template <typename T>
	void operator()(const T& r)
	{
		std::cout << "(" << r << ")" << '\n';
	}
};

int main()
{
	variant<int, double,string> vx;
	visit(Print{},vx);
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

visit(callable, variant_object);

struct Print
{
	template <typename T>
	void operator()(const T& r)
	{
		std::cout << "(" << r << ")" << '\n';
	}
};

int main()
{
	variant<int,double,string> vx{"Melike"};
	visit(Print{}, vx); // Melike yazar

	--------------------------------------------------------------------------

	variant<int,double,string> vx = 65;
	visit(Print{}, vx); // 65 yazar
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

GENERALİZED LAMBDA EXPRESSIONDAN FAYDALANACAĞIZ

int main()
{
	variant<int,double,string> vx("Melike");

	visit([](const auto &r){ std::cout << r << '\n';}, vx);

	-------------------------------------------------------------------

	Lambda dışarıda da olabilirdi

	auto f = [](const auto &r){ std::cout << r << '\n';};
	visit(f,vx);

}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Triple
{
	template<typename T>
	void operator()(T& t)const // const demek nesneyi değiştirmeyecek demek. t burada parametre.
	{
		t = t + t + t;
	}
};

int main()
{
	variant<string, int, double> vx{21};

	std::cout << get<1>(vx); //21

	visit(Triple{}, vx); // Burada toplama işlemi yaptırdık :D

	std::cout << get<1>(vx); //63
}


-----------------------------------------------------------------------------------------------

int main()
{
	variant<int,string, double> vx{"salih"};

	std::cout << get<1>(vx).size() << '\n'; // salihin size 5

	auto setdef = [](auto &val){
	val = remove_reference_t<decltype(val)>{}; // İlk önce string& türü için string türünü elde ettik içeride açıkça
	};											// Bu uzunluğu default değere çekecek
												// val = string{} gibi bir işlem yapıyor yani
	
	visit(setdef, vx);

	std::cout << get<1>(vx).size() << '\n'; //
}

-----------------------------------------------------------------------------------------------
AYNI ÖRNEĞİN İNT VERSİONU

int main()
{
	variant<int,string, double> vx{3456};

	std::cout << get<1>(vx) << '\n'; 

	auto setdef = [](auto &val){
	val = remove_reference_t<decltype(val)>{}; 
	};											

	visit(setdef, vx);

	std::cout << get<1>(vx) << '\n';
}

ÇIKTI
-----
3456
0

-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

OVERLOADER IDIYOM
-----------------
Productionda en çok kullanılan idiyom.

template <typename ...Ts>
struct overload : Ts... {

	using Ts::operator()...; // Tüm Taban sınıfların tüm func call operator funclarını, overload sınıfı içinde visible hale getirdik
};

BİRDE DEDUCTION GUIDE VERİLİYOR. C++20 DE DEDUCTION GUIDE A GEREK YOK
template<typename ...Ts>
overload(Ts...) -> overload<Ts...>;

int main()
{
	using namespace std;

	variant<int,string> vx(123);

	visit(overload{
				[](int ival) {cout << "int : " << ival << '\n';},
				[](const std::string &s) {cout << "string : " << s << '\n';} }, vx);

				// Burada CTAD devreye girecek. Yani overloadun Ctoruna gönderdiğimiz argüamanlardan
				// Tür anlaşılacak. Overloadun taban sınıf parametrelerini ve lambda ifadelerini argüman
				// olarak gönderiyoruz.
	// 123 YAZAR
	
	---------------------------------------------------------------------------------
	
	variant<int,string> vy{"Murat"};

	visit(overload{
			[](int ival) {cout << "int : " << ival << '\n';},
			[](const std::string &s) {cout << "string : " << s << '\n';} }, vx);

	// Murat YAZAR
}

-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

KALITIM KONUSUNA BAKALIM

Kapalı bir hiyerarşi olacak. kalıtımda kullanılacak türler baştan belli olacak.
Bunları bir taban sınıftan kalıtımla elde etmek yerine variantın alternatifleri yapılacak.
Ve bu polymorphic davranışıda visiter kullanarak temsil edeceğiz.

Burada kısaca bir kalıtım anlatıldı


class Cat{

};

class Dog{

};

class Lamb{

};

using namespace std;
using Animal = std::variant<Dog,Cat,lamb>;

int main()
{
	vector<Animal> avec; // vocabulary types lar vectorde tutulabilir.

	Animal vx;

	visit( [](const auto &t){
		t.cry(); // cry yazamadık o yüzden hata var burada şuanda
	}, vx);
}

*/
