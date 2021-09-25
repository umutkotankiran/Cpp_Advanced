/*

SOHBET KISMI
------------

class Myclass{

public:
	Myclass(Myclass&&) = default;

private:
	std::string str;  
};

Derleyici buradaki move ctoru yazarkenede elemanlar noexcept ise move ctoru noexcept yazar.
biz bunu default etmesek yine noexcept.

Return değeri const olması const doğruluğunun abartılması. Bunun yapılması önerilmiyor.
copy ellisionu engelliyor.

Parametrelin const olması tercih edilir mi?
Interfacete olması mantıklı değil, implementasyonda olabilir.
void foo(const double x) 
{
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Nec {
public:
	Nec() = default;
	Nec(const Nec&) = default;
	Nec(Nec&&) = default;
	Nec& operator=(const Nec&)
	{
		std::cout << "copy assignment\n";
		return *this;
	}

	Nec& operator=(Nec&&)
	{
		std::cout << "move assignment\n";
		return *this;
	}
};


const Nec foo()
{
	Nec x;

	return x;
}

int main()
{
	Nec x;
	//..

	x = foo(); // foo nun return değeri const olduğundan eğer compiler mandatory olmayan copy ellision yapmazsa bile move ctor çalışmaz.
}				// const sağ taraf değeri için move ctor viable değil. Copy Ctor viable ve çağrılır.



-------------------------------------------------------------------------------------------------------------------------------------------------------------------

tempalte <typename T>
void func(const T& t) // Const olduğundan herşey gönderilir ama value category korunmaz.L mi R mı nedir belli değil.
{

}

tempalte <typename T>
void func(T&& t) // forwarding/universal reference
{

}

Universal her argümanı kabul ediyor. Ayrıca L ve R value expr gönderilir const lukta korunur
Tür çıkarımına kısaca değindi hoca.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

Forward diyorki sen bana rvalue verirsen ben bu ifadeyi std::move(x) e dönüştüreceğim
lvalue verirsen ben bunu doğrudan kendisi olarak kullanacağım. Conditional move gibi.

Önceki derste yazıldı sanırım bu kod.Overloadları yazıyor hoca.

class Nec {};

void f(Nec&)
{
	std::cout << "f(Nec &)\n";
}

void f(const Nec&)
{
	std::cout << "f(const Nec &)\n";
}

void f(Nec&&)
{
	std::cout << "f(Nec &&)\n";
}

void f(const Nec&&)
{
	std::cout << "f(const Nec &&)\n";
}

int main()
{
	auto call_f = [](auto&& ur) {
		f(std::forward<decltype(ur)>(ur));
	};

	Nec nec;
	const Nec c_nec;

	f(nec); call_f(nec);
	std::cout << "--------------------\n";

	f(c_nec); call_f(c_nec);
	std::cout << "--------------------\n";
	
	f(Nec{}); call_f(Nec{});
	std::cout << "--------------------\n";
	
	f(std::move(nec)); call_f(std::move(nec));
	std::cout << "--------------------\n";
	
	f(std::move(c_nec)); call_f(std::move(c_nec));

}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

2. BIR YÖNTEM VAR FORWARDING YERINE 

class Nec {};

template<typename T>
void foo(const char *p,  T&&)
{
	if constexpr (std::is_lvalue_reference_v<T>) {
		std::cout << "argument is '" << p << "'" << " has come here and it is L value\n";
	}
	else {
		std::cout << "argument is '" << p << "'" << " has come here and it is R value\n";
	}
}

int main()
{
	Nec n;
	const Nec cn;

	foo("n", n);
	foo("cn", n);
	foo("std::move(n)", std::move(n));
	foo("Nec{}", Nec{});
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

EN BASIT FORWARD İMPLEMENTASYONU

template <typename T>
T&& Forward(typename std::remove_reference<T>::type &param) // remove_reference_t yazsaydık başına typename yazmaya gerek kalmıyor.bağlı tür sentaksı kalmıyor artık
{
	return static_cast<T&&>(param);
}

T&& Forward(std::remove_reference_t<T> &param) // Buradaki gibi yazılabilir yani.C++20 de gevşetilmiş biraz.
{
	return static_cast<T&&>(param);
}

Forward sol taraf değeri gelirse forward bunu sol taraf değeri argümanı olarak kullanılmasını sağlıyor, sağ taraf değeriyse onu move func a argüman olarak gönderiyor.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

MOVE NASIL

template <typename T>
constexpr std::remove_reference_t<T>&& Move(T&& t)noexcept // Buradaki gibi yazılabilir yani.C++20 de gevşetilmiş biraz.
{
	return static_cast<std::remove_reference_t<T>&&>(t);
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

Gelen Rvalue mu Lvalue mu

tempalte <typename T>
void func(T&& x)
{
	if constexpr(std::is_lvalue_reference_v<T>)
	{
		//BURAYA GİRERSE GELEN LVALUE. int& ve int&& biri l diğeri xvalue. BUnlar değilse rvalue oluyor zaten
	}
	else
	{
		// RVALUE(PRVALUE DEĞİL.)
	}
}

int main()
{
	Myclass mx;
	func(mx);

	func(Myclass{});
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

tempalte <typename T>
void func(T&& x)
{
	if constexpr(std::is_const_v<std::remove_reference_t<T>>)  // remove_reference_t yazmasaydık doğru olmayacaktı.
	{
		//const değerle çağrılmış
	}
	else
	{
		//const değil.
	}
}

int main()
{
	int x = 10;
	const int cx = 20;
	const int& ref = x;

	func(x);
	func(cx);
	func(ref);

}

Forwarding parametre varsa value category dependent ve const dependent kodda yazılabilir.


-------------------------------------------------------------------------------------------------------------------------------------------------------------------

MAKE UNIQUE YAZILIYOR ŞİMDİ

template<typename T, typename ...Args>
std::unique_ptr<T> MakeUnique(Args&& ...args)
{
	return std::unique_ptr<T> {new T(std::forward<Args>(args)...)};
} 

// {new T(std::forward<Args>(args)...)} bunu doğrudan return etseydik ctor explicit olduğundan implicitly dönüşüm yok

// T(forward<int>x1,forward<double>x2 ) gibi gidebilir mesela böyle açılabilir içindeki ifade.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

Programcılar en çok universal parametre mi değil mi bunu anlamıyor.Burada sorular cevaplar oldu 57:00

-------------------------------------------------------------------------------------------------------------------------------------------------------------------


1:10 DA TEMPLATE DEDUCTION SORULARIYLA DEVAM EDILDI

-------------------------------------------------------------------------------------------------------------------------------------------------------------------
NOT : VECTOR LIST GİBİ CONTAINERLAR REFERANS TUTMAZ. REFERANS TUTANLAR CONTAINER DEĞİL, PAIR TUPLE REFERANS TUTMAZ.

#include <vector>
#include <string>

template <typename T>
void insert(std::vector<T>& vec, T&& elem)
{
	vec.push_back(std::forward<T>(elem));
}

int main()
{
	std::vector<std::string> vec;
	std::string s;

	insert(vec, s); //gecersiz
	insert(vec, std::string{}); //Geçerli
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

NASIL LEGAL HALE GELİR

#include <vector>
#include <string>

template<typename T>
void insert(std::vector<std::remove_reference_t<T>>& vec, T&& elem)
{
	vec.push_back(std::forward<T>(elem));
}

int main()
{
	std::vector<std::string> vec;
	std::string s;

	insert(vec, s);
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

AYRI BİR TEMPLATE PARAMETRESİ YAPARAKTA ÇÖZÜLÜR

#include <vector>
#include <string>

template<typename ElemType, typename T>  // 2 adet template tür parametresi yazdık Ayrı ayrı çıkarım oldu hata yok
void insert(std::vector<ElemType>& vec, T&& elem)
{
	vec.push_back(std::forward<T>(elem));
}

int main()
{
	std::vector<std::string> vec;
	std::string s;

	insert(vec, s);
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

CONTAINERIN KENDİSİNİ BİR TEMPLATE PARAMETRESİ YAPABİLİRDİK

#include <vector>
#include <string>

template<typename Container, typename T>
void insert(Container &con, T&& elem)
{
	con.push_back(std::forward<T>(elem));
}

int main()
{
	std::vector<std::string> vec;
	std::string s;

	insert(vec, s);
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

TYPETELLER

Tür çıkarımını veriyor.Temel C++ ta yazdık.

template<typename T>
class TypeTeller;

template <typename T>
void func(T)
{
	TypeTeller<T> x;
}

int main()
{
	int a[10];
	func(a);
}


-------------------------------------------------------------------------------------------------------------------------------------------------------------------

Function sadece int ile çağrılsın sorusu klasik soru.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

DANGLING REFERENCE IN ÇÖZÜMÜ İÇİN AŞAĞIDAKİ YAZILABİLİR.


class Person
{
private:
	std::string name;
public:
	void print() const
	{
		std::cout << "print()\n";
	}

	std::string getName()&&
	{
		return std::move(name);
	}

	const std::string& getName() const&
	{
		return name;
	}
};

template<typename T>
void foo(T&& x)
{
	x.print();
	auto s1 = x.getName(); // x lvalue bu sebeple function return değeri const & olan

	auto s2 = std::forward<T>(x).getName(); // buna gelen lvalue ise & olan rvalue ise forward --> std::move() olacağına göre bu seferde && olan çağrılacak.
}

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

Template tür çıkarımı ambigiuty durumlarını anlattı hoca kısaca

Bunlara conflicting durumu deniyor.

Nasıl önüne geçilebilir.
1. remove_reference_t<T> kullanılabilir.
2. Ayrı template tür parametresi yazılabilir.
3. Containerın kendisi function tür parametresi olarak yazıalbilir.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

UNIVERSAL REFERANS HER ZAMAN PERFECT FORWARDING AMAÇLI KULLANILMIYOR.

void iterate(std::string::iterator beg, std::string::iterator end)
{
	std::cout << "do some non-const stuff with the passed range\n";
}

void iterate(std::string::const_iterator beg, std::string::const_iterator end)
{
	std::cout << "do some const stuff with the passed range\n";
}

template<typename T>
void process(T&& coll)
{
	iterate(coll.begin(), coll.end());
}

int main()
{
	std::string v{ "v" };
	const std::string c{ "c" };
	process(v); // coll binds to a non-const lvalue, iterators passed
	process(c); // coll binds to a const lvalue, const_iterators passed
	process(std::string{ "t" }); // coll binds to a non-const prvalue, iterators passed
	process(std::move(v)); // coll binds to a non-const xvalue, iterators passed
	process(std::move(c)); // coll binds to a const xvalue , const_iterators passed
}

Func parametresi forwarding parametre ama forward edilmemiş. Bir şekilde constluk forward edilmiş.
T const container türüyse bu durumda const nesne için beginin return değeri const iterator olacak.


-----------------------------------------------------------------------------------------------------------------------------

Scott Meyers bir functionun parametresi template olmayacak şekidlde && ise bunun tek sebebi taşımadır demiş bir yazısında.

func(int && x) {}  // tek varlık sebebi taşımadır.

Ama function parametresi aşağıdaki gibi ise amacı forward etmek diyor ama artık universal referans parametre olmasına rağmen
doğrudan forward ın çağrılmadığı senaryolarda var.

template <typename T>
func(int && x) {}


-----------------------------------------------------------------------------------------------------------------------------

FORWARD A BİR ALTERNATİF DAHA VAR

class Nec {};

void f(Nec&)
{
	std::cout << "f(Nec &)\n";
}

void f(const Nec&)
{
	std::cout << "f(const Nec &)\n";
}

void f(Nec&&)
{
	std::cout << "f(Nec &&)\n";
}

void f(const Nec&&)
{
	std::cout << "f(const Nec &&)\n";
}

template <typename T>
void call_f(T &&param)
{
	f(std::forward<decltype(param)>(param));
}

int main()
{
	Myclass mx;
	const Myclass cmx;

	call_f(mx);
	call_f(cmx);
	call_f(Myclass{});

}

NEDEN BÖYLE BİRŞEY YAPALIM ? CEVAP : LAMBDA İFADELERİ İÇİN

-----------------------------------------------------------------------------------------------------------------------------

LAMBDA İFADELERİ

int main()
{
	[](auto val){} ; // Burada parametre parantezi içinde auto kullanırsak buna generalized lambda expression deniyor
}

Generalized lambda expression olduğunda derleyici nasıl bir closure type oluşturuyor. Function call operator functionu
ise bir functon template. Myclass ın kendisi template değil ama

class Myclass{
public:
	template <typename T>
	void operator()(T x)const;
};

ÖNEMLİ:
Biz auto koyduğumuz zaman yani generalized lambda expression yazdığımız zaman, aslında derleyici bizim closure type ımıza
Yukarıdaki gibi bir function yazıyor.

Böylece biz bu funca 12 (func(12); yazarsak)değerini gönderirsek T türünün int olduğu çıkıyor ve çağrılacak function int parametreli oluyor.

Yani auto yazınca, closure type ın operator func ını template olarak yaz demek. İçeride auto, auto&, auto&& te olabilir.

class Myclass{
public:
	template <typename T>
	void operator()(T&& x)const; // aşağıda auto&& kullanırsak bunu yazar.
};

int main()
{
	auto f = [](auto &&x) {};
}

-----------------------------------------------------------------------------------------------------------------------------

Burada doğrudan kullanılacak bir T türü yok.Forwarding aşağıdaki gibi yapılır.

auto f = [](auto &&x) {
	func(std::forward<decltype(x)>(x)); // Burada yine perfect Forwarding çalışacak.

};


-----------------------------------------------------------------------------------------------------------------------------

VARIADIC TEMPLATE OLARAKTA YAZILABİLİR

auto f = [](auto&& ...x) {
	func(std::forward<decltype(x)>(x)...);
};

-----------------------------------------------------------------------------------------------------------------------------

AŞAĞIDAKİ BİR TEMPLATE. C++20 ile geldi

void func(auto &&x)
{
	func(std::forward<decltype(x)>(x)); // bunu yazmakla template yazmak arasında fark yok.
}

-----------------------------------------------------------------------------------------------------------------------------

[]<typename T>(){} --> Bu özellik C++ 20 ile geldi.İleride görülecek baya bir olanak veriyor bu typename kısmı

[]<typename T>(T &&x){

	std::forward<x>(x)
}

-----------------------------------------------------------------------------------------------------------------------------

MÜLAKAT SORUSU

template <typename T, typename Val>
void assign(C &c, const Val &val)
{
	for(auto &x : c)
	{
		x = val;
	}
}

int main()
{
	using namespace std;
	vector<int> vec(10,5);

	assign(vec,20); // fill gibi çalışıyor. Tüm değerleri 20 yapacak.
}

Soru:

int main()
{
	using namespace std;
	vector<bool> vec(10,false);

	assign(vec,20);
}

NEDEN SENTAKS HATASI OLDU ??????
vector<bool> açılımı bir bit alanında tutuluyor ve Partial specialization çünkü allocator parametresi var.

TAM OLARAK HATANIN KAYNAĞI NEDIR? NASIL BIR DEĞIŞIKLIKLE VECTOR<BOOL> AÇILIMI GIBI SINIFLAR SÖZKONUSU OLUNCA HATAYI GIDEREBILIRIZ?
BU DEĞİŞİKLİK TEMPLATE TE OLACAK.
CEVAP : auto &&

template <typename T, typename Val>
void assign(C &c, const Val &val)
{
	for(auto &&x : c)
	{
		x = val;
	}
}

int main()
{
	using namespace std;
	vector<bool> vec(10,false);

	assign(vec,20);

	for(int i : vec) // hepsini 0 gösterecek false olduğu için
		std::cout << i << " ";

}

NEDEN DÜZELDİ?
auto&&r =   Burası kesinlikle bir forwarding referans. Herhangbir value categorisine bağlanabilir.

Forwarding referans oluşturmanın 2 yolu var. 1. function template 2. yukarıdaki bildirim

Range base for loop karşılığı derleyici nasıl bir kod üretiyor. Zaten yazmıştık.

int main()
{
	using namespace std;
	vector<int> ivec{1,2,3,4,5,6,7};

	for(auto x : ivec)
	{
		cout << x << " ";
	}

	//BURADA DERLEYICI NASIL BIR KOD YAZIYOR.RANGE OLUŞTURACAK BUNUN IÇIN VECTORUN ITERATORLERINE IHTIYAÇ VAR.
	// BU NEDENLE IKI NOKTANIN SAĞNDAKI IFADEYI BIR UNIVERSAL REFERENCE A BAĞLIYOR.
	// NEDEN UNIVERSAL REFERENCE A BAĞLIYOR? AUTO & VEYA AUTO DESEYDI NEDEN OLMAZDI
	// AUTO DESEYDİ VECTORUN KOPYASI ÇIKARTILIRDI. COPY CTOR ÇAĞRILIRDI.AUTO OLAMAZ
	// AUTO& OLSAYDI RVALUE YA BAĞLANMAZDI. AUTO&& OLDUĞUNDA LVALUE RVALUE BAĞLANABİLİR ARTIK.
	// BİZİM 2 İTERATOR ALMAMIZ LAZIM BEGİN VE END. BURADA DEĞERİ REFERANSA BAĞLADIĞIMIZDA
	// SAĞ TARAF VE SOL TARAF DEĞERLERİ İÇİN LİFE EXTENSİON OLACAK, BUNUN İÇİN HAYATI SONA ERMEYECEK.


	auto &&range = ivec;

	for(auto beg_iter = std::begin(ivec), end_iter = std::end(ivec); beg_iter != end_iter; ++beg_iter) //range base in karşılığı
	{
		auto x = *beg_iter;
		// Her turda cop ysemantics devreye girer saçma olur.
	}

	-------------------------

	EĞER
	& koyarsak
	for(auto &x : ivec)
	{
		cout << x << " ";
	}

	KODUN KARŞILIĞI

	for(auto beg_iter = std::begin(ivec), end_iter = std::end(ivec); beg_iter != end_iter; ++beg_iter) //range base in karşılığı
	{
		auto &x = *beg_iter;
		// her turda x iterator konumundaki nesnenin kendisi.
	}


	-------------------------

	EĞER
	&& koyarsak
	for(auto &&x : ivec)
	{
		cout << x << " ";
	}

	KODUN KARŞILIĞI

	for(auto beg_iter = std::begin(ivec), end_iter = std::end(ivec); beg_iter != end_iter; ++beg_iter) //range base in karşılığı
	{
		auto &&x = *beg_iter;
		// Buradaki auto&& forwarding reference. Buradada *beg_iter ifadesi forwarding refernece a bağlanıyor.
		// Burada auto& olsaydo *beg_iter rvalue expr olsaydı sentaks hatası olurdu.
		// auto&& yapılırsa *beg_iter rvalue da olsa lvalue da olsa buna bağlanır.
	}

	------------------------------------

	SORUYA GERİ DÖNELİM.

	DERLEYICININ YAZIDĞI KODDA FOR DÖNGÜSÜNÜN IÇINDEKI DEĞIŞKENI INCELEYELIM.
	VECTOR<BOOL> SÖZKONUSU OLDUĞUNDA (ÖRNEKTEKI *BEG_ITER), O ASLINDA REFERENCE DÖNDÜREN BIR SAĞ TARAF IFADESI.
	RVALUE EXPRESSION.BURADA AUTO& KULLANIRSAK SENTAKS HATASI OLUR. ÇÜNKÜ SOL TARAF REFERENCE, SAĞ TARAF DEĞERINE
	BAĞLANAMAZ. AUTO&& YAZDIĞIMIZDA : ÜN SAĞ TARAFINDAKI IFADELERE YANI RVALUE EXPRESSIONLARA BAĞLANABILIYOR.

	VECTOR<BOOL> ELEMANLARI BİR RVALUE EXPRESSİON.

	class vctor{
	public:

		class reference{

		};
	};

	*BEGIN(VECTOR<BOOL>) İFADESİ, REFERENCE TÜRDEN BİR SINIF NESNESİ ELDE EDİYORUZ.

	NOT : OLAYIN ÖZÜNDE ŞU VAR
	DOĞRUDAN AUTO YA BAĞLARSAK COPU SEMANTİK ÇALIŞIR.
	AUTO& A BAĞLARSAK RVALUE EXPRESSION GELİRSE BAĞLAYAMAYIZ Kİ *BEGIN(VECTOR<BOOL>) BİR RVALUE EXPR
	AUTO&& BAĞLARSAK ANCAK ÇALIŞABİLİR.SAĞ TARAFTAN NE GELİRSE GELSİN BAĞLANIR.FORWARDİNG REFERENCE UNIVERSAL REFERENCE

	VECTOR<BOOL> İÇİNDE BİR PROXY CLASS VAR.

	MESELA BITSETTE VAR.

}

*/
